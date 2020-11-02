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

#define POPULATION_SIZE 10
#define SPEED 2

USING_NS_CC;

// Crossover and mutation function
void crossoverAndMutate(Jumper::Jumper *parentA, Jumper::Jumper *parentB, Jumper::Jumper *offspring, float mutationRate)
{
    auto parentAParameters = parentA->neuralNetwork->get_parameters();
    auto parentBParameters = parentB->neuralNetwork->get_parameters();

    OpenNN::Vector<double> newParameters(parentAParameters.size());

    for (int i = 0; i < parentAParameters.size(); i++)
    {
        // Crossover
        newParameters[i] = random<int>(0, 1) == 0 ? parentAParameters[i] : parentBParameters[i];

        // Mutation
        newParameters[i] += random<double>(-1, 1) * mutationRate * newParameters[i];
    }

    offspring->neuralNetwork->set_parameters(newParameters);
}

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
    this->evolutionSession = new EvolutionSession<Jumper>;

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

        this->evolutionSession->evolve(crossoverAndMutate);
    }

    // Jumpers
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        auto jumper = new Jumper(Vec2(890, 40), i, nnParameters[i]);
        this->evolutionSession->population.push_back(jumper);
        this->addChild(jumper->node, 2, i);
    }

    // Current generation info
    this->cGInfo.startTimestamp = TimeHelper::now();
    this->cGInfo.obstaclesUsed = 0;
    this->cGInfo.obstaclesDeployed = 0;
    this->cGInfo.jumpersAlive = POPULATION_SIZE;
    this->cGInfo.totalJumps = 0;

    // Set scheduler to add obstacles
    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::addObstacle), 2.3, CC_REPEAT_FOREVER, 0);
    this->addObstacle(0);

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
    for (auto jumper : this->evolutionSession->population)
    {
        if (!jumper->isDead)
            jumper->update(delta);
    }
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
                auto jumper = this->evolutionSession->population[(categoryBitmaskA == 1 ? bodyA : bodyB)->getNode()->getTag()];
                jumper->kill();
                this->removeChild(jumper->node);

                this->cGInfo.jumpersAlive--;

                if (this->cGInfo.jumpersAlive == 0)
                    this->nextGeneration();
            });

            return false;
        }
        else if ((categoryBitmaskA == 8 && categoryBitmaskB == 2) || (categoryBitmaskA == 2 && categoryBitmaskB == 8))
        {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                auto obstacle = (categoryBitmaskA == 2 ? bodyA : bodyB)->getNode();
                this->removeChild(obstacle);
                this->cGInfo.obstaclesUsed++;

                if (this->cGInfo.obstaclesUsed == 2)
                    this->nextGeneration();
            });

            return false;
        }

        return true;
    }

    return false;
}

void Jumper::MainScene::onMouseMove(EventMouse *e)
{
    // auto mousePosition = Vec2(e->getCursorX(), e->getCursorY());
    // auto obstacleNewPosition = this->obstacle->getParent()->convertToNodeSpace(mousePosition);

    // this->obstacle->setPosition(obstacleNewPosition);
}

void Jumper::MainScene::addObstacle(float delta)
{
    if (random<int>(0, 1))
    {
        auto obstacle = Obstacle::create(Vec2(visibleSize.width - 30, 23));
        this->addChild(obstacle, 1);
        this->cGInfo.obstaclesDeployed++;
    }
}

void Jumper::MainScene::nextGeneration()
{
    // Remove all the obstacles and jumpers
    auto children = this->getChildren();
    for (auto child : children)
    {
        auto physicsBody = child->getPhysicsBody();
        if (physicsBody != nullptr && physicsBody->getCategoryBitmask() <= 2)
        {
            if (physicsBody->getCategoryBitmask() == 1)
                this->evolutionSession->population[child->getTag()]->kill();

            this->removeChild(child);
        }
    }

    // Score the current population
    auto generationDuration = TimeHelper::now() - this->cGInfo.startTimestamp;

    for (auto jumper : this->evolutionSession->population)
        jumper->setScore(this->cGInfo.startTimestamp, generationDuration, this->cGInfo.obstaclesDeployed);

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
    this->evolutionSession->evolve(crossoverAndMutate, CoreSession::sessionId, builder.GetBufferPointer(), builder.GetSize());

    // Update timestamp
    this->cGInfo.startTimestamp = TimeHelper::now();

    // Reset jumpers
    for (auto jumper : this->evolutionSession->population)
        jumper->prepareForNewGeneration();

    // Add jumpers
    for (int i = 0; i < this->evolutionSession->population.size(); i++)
    {
        this->evolutionSession->population[i]->generateNode();
        auto node = this->evolutionSession->population[i]->node;
        this->addChild(node, 2, i);
        node->setPosition(890, 40);
    }

    // Reset attributes of current generation info
    this->cGInfo.obstaclesUsed = 0;
    this->cGInfo.obstaclesDeployed = 0;
    this->cGInfo.jumpersAlive = this->evolutionSession->population.size();
    this->cGInfo.totalJumps = 0;
}

Jumper::MainScene::~MainScene()
{
    for (auto jumper : this->evolutionSession->population)
        delete jumper;

    delete this->evolutionSession;
}
