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
#include <core/GenerationState_generated.h>
#include <core/Database.h>
#include <core/Session.h>
#include <core/EvolutionCommon.h>

#define POPULATION_SIZE 10
#define SPEED 1

using namespace Jumper;
USING_NS_CC;

Scene *MainScene::createScene()
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
bool MainScene::init()
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

    // Load sprite sheet
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("jumpers/sprite-sheet.plist");

    // Evolution session
    this->evolutionSession = new Core::EvolutionSession<JumperGroup>;

    // Database
    std::vector<double> nnParameters[POPULATION_SIZE];

    if (Core::Session::sessionId == 0)
    {
        // Create new session
        Core::Session::extensionId = Core::Database::getExtensionId("Jumper");
        Core::Session::sessionId = Core::Database::addSession(Core::Session::extensionId);
    }
    else if (Core::Session::generationId == 0)
    {
        // Start the session over from scratch
        Core::Database::clearSession(Core::Session::sessionId);
    }
    else
    {
        auto stateBinary = Core::Database::getGenerationState(Core::Session::generationId);

        auto state = Core::GetGenerationState(stateBinary);

        for (int i = 0; i < state->population()->size(); i++)
        {
            for (int j = 0; j < state->population()->Get(i)->chromosomes()->size(); j++)
            {
                nnParameters[i].push_back(state->population()->Get(i)->chromosomes()->Get(j));
            }
        }

        this->evolutionSession->evolve(Core::EvolutionCommon<JumperGroup>::crossoverAndMutate, Core::EvolutionCommon<JumperGroup>::randomize);
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
    auto boundaries = Boundary::create();
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

void MainScene::update(float delta)
{
    for (auto jumperGroup : this->evolutionSession->population)
        jumperGroup->update(delta);
}

void MainScene::menuCloseCallback(Ref *pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

bool MainScene::onContactBegin(PhysicsContact &contact)
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

void MainScene::addObstacles()
{
    auto visibleWidth = Director::getInstance()->getVisibleSize().width;
    {
        auto obstacle = Obstacle::create(Vec2(visibleWidth - 0, 33), Obstacle::Slow);
        this->addChild(obstacle, 1);
    }
    {
        auto obstacle = Obstacle::create(Vec2(visibleWidth - 240, 33), Obstacle::Medium);
        this->addChild(obstacle, 1);
    }
    {
        auto obstacle = Obstacle::create(Vec2(visibleWidth - 464, 33), Obstacle::Fast);
        this->addChild(obstacle, 1);
    }

    this->currentGenerationInfo.obstacles = 3;
}

void MainScene::nextGeneration()
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

    auto population_ = std::vector<flatbuffers::Offset<Core::Member>>();

    for (auto object : this->evolutionSession->population)
    {
        auto chromosomes = std::vector<double>();
        auto parameters = object->neuralNetwork->get_parameters();
        for (auto parameter : parameters)
            chromosomes.push_back(parameter);
        auto member = Core::CreateMember(builder, builder.CreateVector<double>(chromosomes), object->getScore());
        population_.push_back(member);
    }

    auto population = builder.CreateVector(population_);

    auto state = CreateGenerationState(builder, population);
    builder.Finish(state);

    // Perform evolution
    this->evolutionSession->evolve(Core::EvolutionCommon<JumperGroup>::crossoverAndMutate, Core::EvolutionCommon<JumperGroup>::randomize, Core::Session::sessionId, builder.GetBufferPointer(), builder.GetSize());

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

MainScene::~MainScene()
{
    for (auto jumper : this->evolutionSession->population)
        delete jumper;

    delete this->evolutionSession;
}
