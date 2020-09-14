#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <neural_network.h>
#include <ctime>
#include "MainScene.h"
#include "Jumper.h"
#include "Boundary.h"
#include "Evolution.h"
#include "Obstacle.h"

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

    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // White background
    LayerColor *_bgColor = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(_bgColor, -10);

    this->visibleSize = Director::getInstance()->getVisibleSize();

    // Timer
    this->generationStartTimestamp = std::time(nullptr);

    // Jumpers
    for (int i = 0; i < 18; i++)
    {
        auto jumper = new Jumper(Vec2(40 + i * 50, 40));
        this->jumperList.push_back(jumper);
        this->addChild(jumper->node, 2, i);
    }

    // Set scheduler to add obstacles
    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::addObstacle), 2.5, CC_REPEAT_FOREVER, 0);
    this->addObstacle(0);

    // Boundary
    auto boundaries = Boundary::create(this->visibleSize);
    for (int i = 0; i < 4; i++)
    {
        this->addChild(boundaries[i]);
    }
    delete[] boundaries;

    // Evolution session
    this->evolutionSession = new Evolution(this->jumperList);

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
    for (auto jumper : this->jumperList)
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
                auto jumper = this->jumperList[(categoryBitmaskA == 1 ? bodyA : bodyB)->getNode()->getTag()];
                jumper->deathTimestamp = std::time(nullptr);
                jumper->isDead = true;
                this->removeChild(jumper->node);
            });

            return false;
        }
        else if ((categoryBitmaskA == 8 && categoryBitmaskB == 2) || (categoryBitmaskA == 2 && categoryBitmaskB == 8))
        {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                auto obstacle = (categoryBitmaskA == 2 ? bodyA : bodyB)->getNode();
                this->removeChild(obstacle);
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
    this->addChild(Obstacle::create(Vec2(visibleSize.width - 30, 23)), 0);
}

void MainScene::prepareNextGeneration()
{
    this->generationStartTimestamp = std::time(nullptr);

    // Reset jumpers
    for (auto jumper : this->jumperList)
        jumper->reset();

    // Add all the food items over again
    for (auto child : this->getChildren())
    {
        auto physicsBody = child->getPhysicsBody();
        if (physicsBody != nullptr && physicsBody->getCategoryBitmask() == 2)
        {
            this->removeChild(child);
        }
    }
}

MainScene::~MainScene()
{
    for (auto jumper : this->jumperList)
        delete jumper;

    delete this->evolutionSession;
}
