#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <opennn/neural_network.h>
#include <helpers/time.h>
#include <flatbuffers/flatbuffers.h>
#include <extensions/jumper/MainScene.h>
#include <extensions/jumper/Jumper.h>
#include <extensions/jumper/Boundary.h>
#include <extensions/jumper/Obstacle.h>
#include <extensions/jumper/GenerationState_generated.h>
#include <core/Database.h>
#include <core/CoreSession.h>
#include <extensions/jumper/Evolution.h>

#define POPULATION_SIZE 10
#define SPEED 1

USING_NS_CC;

Scene *Jumper::MainScene::createScene()
{
    return MainScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char *filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Jumper::MainScene::init()
{
    if (!Scene::initWithPhysics())
        return false;

    this->getPhysicsWorld()->setGravity(Vec2(0, -400));

    // Set the speed of the simulation
    Director::getInstance()->getScheduler()->setTimeScale(SPEED);
    this->getPhysicsWorld()->setSpeed(SPEED);

    // this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // White background
    LayerColor *_bgColor = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(_bgColor, -10);

    this->visibleSize = Director::getInstance()->getVisibleSize();

    // Evolution session
    this->evolutionSession = new EvolutionSession<JumperGroup>;

    // Database
    std::vector<double> nnParameters[POPULATION_SIZE];

    if (CoreSession::sessionId == 0)
    {
        // Create new session
        CoreSession::extensionId = Database::getExtensionId("Jumper");
        CoreSession::sessionId = Database::addSession(CoreSession::extensionId);
    }
    else if (CoreSession::generationId == 0)
    {
        // Start the session over from scratch
        Database::clearSession(CoreSession::sessionId);
    }
    else
    {
        auto stateBinary = Database::getGenerationState(CoreSession::generationId);

        auto state = GetGenerationState(stateBinary);

        for (int i = 0; i < state->population()->size(); i++)
        {
            for (int j = 0; j < state->population()->Get(i)->chromosomes()->size(); j++)
            {
                nnParameters[i].push_back(state->population()->Get(i)->chromosomes()->Get(j));
            }
        }

        this->evolutionSession->evolve(Evolution::crossoverAndMutate);
    }

    // Jumpers
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        auto jumperGroup = new JumperGroup(nnParameters[i]);
        this->evolutionSession->population.push_back(jumperGroup);

        for (auto member : jumperGroup->members)
            this->addChild(member->node, 2, i);
    }

    // Add obstacles
    this->addObstacles();

    // Boundary
    auto boundaries = Boundary::create(this->visibleSize);
    for (int i = 0; i < 4; i++)
    {
        this->addChild(boundaries[i], -9);
    }
    delete[] boundaries;

    // Contact listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(MainScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    return true;
}

void Jumper::MainScene::update(float delta)
{
    for (auto jumperGroup : this->evolutionSession->population)
        jumperGroup->update(delta);
}

void Jumper::MainScene::menuCloseCallback(Ref *pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

bool Jumper::MainScene::onContactBegin(PhysicsContact &contact)
{
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    auto categoryBitmaskA = bodyA->getCategoryBitmask();
    auto categoryBitmaskB = bodyB->getCategoryBitmask();

    if (categoryBitmaskA != categoryBitmaskB)
    {
        if ((categoryBitmaskA == 1 && categoryBitmaskB == 2) || (categoryBitmaskA == 2 && categoryBitmaskB == 1))
        {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                auto node = (categoryBitmaskA == 1 ? bodyA : bodyB)->getNode();
                auto jumperGroup = this->evolutionSession->population[node->getTag()];
                jumperGroup->kill(node);
                this->removeChild(node);
            });

            return false;
        }
        else if ((categoryBitmaskA == 8 && categoryBitmaskB == 2) || (categoryBitmaskA == 2 && categoryBitmaskB == 8))
        {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                auto obstacle = (categoryBitmaskA == 2 ? bodyA : bodyB)->getNode();
                this->removeChild(obstacle);
                this->currentGenerationInfo.obstacles--;

                if (this->currentGenerationInfo.obstacles == 0)
                    this->nextGeneration();
            });

            return false;
        }

        return true;
    }

    return false;
}

void Jumper::MainScene::addObstacles()
{
    {
        auto obstacle = Obstacle::create(Vec2(visibleSize.width - 0, 33), Obstacle::Slow);
        this->addChild(obstacle, 1);
    }
    {
        auto obstacle = Obstacle::create(Vec2(visibleSize.width - 240, 33), Obstacle::Medium);
        this->addChild(obstacle, 1);
    }
    {
        auto obstacle = Obstacle::create(Vec2(visibleSize.width - 464, 33), Obstacle::Fast);
        this->addChild(obstacle, 1);
    }

    this->currentGenerationInfo.obstacles = 3;
}

void Jumper::MainScene::nextGeneration()
{
    // Score the current population
    for (auto jumperGroup : this->evolutionSession->population)
        jumperGroup->setScore();

    // Remove all the obstacles and jumpers
    auto children = this->getChildren();
    for (auto child : children)
    {
        auto physicsBody = child->getPhysicsBody();
        if (physicsBody != nullptr && physicsBody->getCategoryBitmask() <= 2)
        {
            if (physicsBody->getCategoryBitmask() == 1)
                this->evolutionSession->population[child->getTag()]->kill(child);

            this->removeChild(child);
        }
    }

    // Contructing a FlatBuffers builder
    flatbuffers::FlatBufferBuilder builder(1024);

    auto population_ = std::vector<flatbuffers::Offset<Member>>();

    for (auto object : this->evolutionSession->population)
    {
        auto chromosomes = std::vector<double>();
        auto parameters = object->neuralNetwork->get_parameters();
        for (auto parameter : parameters)
            chromosomes.push_back(parameter);
        auto member = CreateMember(builder, builder.CreateVector<double>(chromosomes), object->getScore());
        population_.push_back(member);
    }

    auto population = builder.CreateVector(population_);

    auto state = CreateGenerationState(builder, population);
    builder.Finish(state);

    // Perform evolution
    this->evolutionSession->evolve(Evolution::crossoverAndMutate, CoreSession::sessionId, builder.GetBufferPointer(), builder.GetSize());

    // Add jumper nodes
    for (int i = 0; i < this->evolutionSession->population.size(); i++)
    {
        auto jumperGroup = this->evolutionSession->population[i];
        jumperGroup->reset();
        auto members = jumperGroup->members;
        for (int j = 0; j < 3; j++)
            this->addChild(members[j]->node, 2, i);
        jumperGroup->positionNodes();
    }

    // Add obstacles
    this->addObstacles();
}

Jumper::MainScene::~MainScene()
{
    for (auto jumper : this->evolutionSession->population)
        delete jumper;

    delete this->evolutionSession;
}
