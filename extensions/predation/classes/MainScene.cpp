#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <opennn/neural_network.h>
#include <helpers/time.h>
#include <extensions/predation/MainScene.h>
#include <extensions/predation/Boundary.h>
#include <core/Database.h>
#include <core/Session.h>

#define POPULATION_SIZE 10
#define SPEED 2

USING_NS_CC;

Scene *Predation::MainScene::createScene()
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
bool Predation::MainScene::init()
{
    if (!Scene::initWithPhysics())
        return false;

    // Set the speed of the simulation
    Director::getInstance()->getScheduler()->setTimeScale(SPEED);
    this->getPhysicsWorld()->setSpeed(SPEED);

    // this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // White background
    LayerColor *_bgColor = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(_bgColor, -10);

    this->visibleSize = Director::getInstance()->getVisibleSize();

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

void Predation::MainScene::update(float delta)
{
}

void Predation::MainScene::menuCloseCallback(Ref *pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

bool Predation::MainScene::onContactBegin(PhysicsContact &contact)
{
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    // TO DO: BUILD CONTACT LISTENER

    return false;
}

Predation::MainScene::~MainScene()
{
}
