#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <opennn/neural_network.h>
#include <core/EvolutionSession.h>
#include <helpers/time.h>
#include <fstream>
#include "MainScene.h"
#include "Jumper.h"
#include "Boundary.h"
#include "Obstacle.h"

#define POPULATION_SIZE 10
#define SPEED 1

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

    this->visibleSize = Director::getInstance()->getVisibleSize();

    // Jumpers
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        auto jumper = new Jumper(Vec2(890, 40), i);
        this->cGInfo.population.push_back(jumper);
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

    // Evolution session
    this->evolutionSession = new EvolutionSession<Jumper>(this->cGInfo.population);

    // Contact listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(MainScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    // Mouse listener
    // auto mouseListener = EventListenerMouse::create();
    // mouseListener->onMouseMove = CC_CALLBACK_1(MainScene::onMouseMove, this);
    // _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    return true;
}

void MainScene::update(float delta)
{
    for (auto jumper : this->cGInfo.population)
    {
        if (!jumper->isDead)
            jumper->update(delta);
    }
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
                auto jumper = this->cGInfo.population[(categoryBitmaskA == 1 ? bodyA : bodyB)->getNode()->getTag()];
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

void MainScene::onMouseMove(EventMouse *e)
{
    // auto mousePosition = Vec2(e->getCursorX(), e->getCursorY());
    // auto obstacleNewPosition = this->obstacle->getParent()->convertToNodeSpace(mousePosition);

    // this->obstacle->setPosition(obstacleNewPosition);
}

void MainScene::addObstacle(float delta)
{
    if (random<int>(0, 1))
    {
        auto obstacle = Obstacle::create(Vec2(visibleSize.width - 30, 23));
        this->addChild(obstacle, 1);
        this->cGInfo.obstaclesDeployed++;
    }
}

void MainScene::nextGeneration()
{
    // Remove all the obstacles and jumpers
    auto children = this->getChildren();
    for (auto child : children)
    {
        auto physicsBody = child->getPhysicsBody();
        if (physicsBody != nullptr && physicsBody->getCategoryBitmask() <= 2)
        {
            if (physicsBody->getCategoryBitmask() == 1)
                this->cGInfo.population[child->getTag()]->kill();

            this->removeChild(child);
        }
    }

    // Score the current population
    auto generationDuration = TimeHelper::now() - this->cGInfo.startTimestamp;

    for (auto jumper : this->cGInfo.population)
        jumper->setScore(this->cGInfo.startTimestamp, generationDuration, this->cGInfo.obstaclesDeployed);

    // Crossover and mutation function
    auto crossoverAndMutate = [&](Jumper *parentA, Jumper *parentB, Jumper *offspring, float mutationRate) -> void {
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
    };

    // Perform evolution
    this->evolutionSession->evolve(crossoverAndMutate);

    // Update timestamp
    this->cGInfo.startTimestamp = TimeHelper::now();

    // Reset jumpers
    for (auto jumper : this->cGInfo.population)
        jumper->prepareForNewGeneration();

    // Add jumpers
    for (int i = 0; i < this->cGInfo.population.size(); i++)
    {
        this->cGInfo.population[i]->generateNode();
        auto node = this->cGInfo.population[i]->node;
        this->addChild(node, 2, i);
        node->setPosition(890, 40);
    }

    // Reset attributes of current generation info
    this->cGInfo.obstaclesUsed = 0;
    this->cGInfo.obstaclesDeployed = 0;
    this->cGInfo.jumpersAlive = this->cGInfo.population.size();
    this->cGInfo.totalJumps = 0;
}

MainScene::~MainScene()
{
    for (auto jumper : this->cGInfo.population)
        delete jumper;

    delete this->evolutionSession;
}
