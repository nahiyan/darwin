#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <opennn/neural_network.h>
#include <helpers/time.h>
#include <extensions/wheels/MainScene.h>
#include <extensions/wheels/Car.h>
#include <extensions/wheels/Listeners.h>
#include <core/Database.h>
#include <core/CoreSession.h>
#include <core/Debug.h>
#include <extensions/wheels/Tracks.h>

#define POPULATION_SIZE 1
#define SPEED 2

USING_NS_CC;

Scene *Wheels::MainScene::createScene()
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
bool Wheels::MainScene::init()
{
    if (!Scene::initWithPhysics())
        return false;

    // Set the speed of the simulation
    Director::getInstance()->getScheduler()->setTimeScale(SPEED);
    this->getPhysicsWorld()->setSpeed(SPEED);

    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // White background
    LayerColor *_bgColor = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(_bgColor, -10);

    this->visibleSize = Director::getInstance()->getVisibleSize();

    // Add cars
    this->evolutionSession = new EvolutionSession<Car>;
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        auto car = new Car;
        this->addChild(car->node);
        this->evolutionSession->population.push_back(car);
    }

    // Add tracks
    auto tracks = Tracks::create();
    for (auto track : tracks)
        this->addChild(track);

    // Contact listener
    {
        auto listener = EventListenerPhysicsContact::create();
        listener->onContactBegin = Listeners::onContactBegin;
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }
    // Mouse listener
    // {
    //     auto listener = EventListenerMouse::create();
    //     listener->onMouseMove = std::bind(Listeners::onMouseMove, std::placeholders::_1, this);
    //     _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    // }

    // Keyboard listener
    {
        auto listener = EventListenerKeyboard::create();
        listener->onKeyPressed = std::bind(Listeners::onKeyPressed, std::placeholders::_1, std::placeholders::_2, this->evolutionSession->population[0]);
        listener->onKeyReleased = std::bind(Listeners::onKeyReleased, std::placeholders::_1, std::placeholders::_2, this->evolutionSession->population[0]);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }

    return true;
}

void Wheels::MainScene::update(float delta)
{
    for (auto car : this->evolutionSession->population)
    {
        car->update(delta);
    }
}

void Wheels::MainScene::menuCloseCallback(Ref *pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

Wheels::MainScene::~MainScene()
{
}
