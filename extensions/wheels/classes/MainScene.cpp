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
#include <extensions/wheels/Session.h>
#include <extensions/wheels/GenerationState_generated.h>
#include <core/EvolutionCommon.h>

#define POPULATION_SIZE 100
#define SPEED 2

USING_NS_CC;
using namespace Wheels;

MainScene *MainScene::instance = nullptr;

MainScene *MainScene::getInstance()
{
    return MainScene::instance;
}

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
    MainScene::instance = this;

    if (!Scene::initWithPhysics())
        return false;

    // Set the speed of the simulation
    Director::getInstance()->getScheduler()->setTimeScale(SPEED);
    this->getPhysicsWorld()->setSpeed(SPEED);

    // this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // White background
    LayerColor *_bgColor = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(_bgColor, -10);

    // Load sprite sheet
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("wheels/sprite-sheet.plist");

    // Evolution session
    Session::setPopulationSize(POPULATION_SIZE);
    Session::evolutionSession = new EvolutionSession<Car>;

    // Database
    std::vector<double> nnParameters[POPULATION_SIZE];

    if (CoreSession::sessionId == 0)
    {
        // Create new session
        CoreSession::extensionId = Database::getExtensionId("Wheels");
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

        Session::evolutionSession->evolve(EvolutionCommon<Car>::crossoverAndMutate, EvolutionCommon<Car>::randomize);
    }

    // Add cars
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        auto car = new Car(nnParameters[i]);
        this->addChild(car->node, 0, i);
        Session::evolutionSession->population.push_back(car);
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
    //     listener->onMouseMove = Listeners::onMouseMove;
    //     _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    // }

    // Keyboard listener
    // {
    //     auto listener = EventListenerKeyboard::create();
    //     listener->onKeyPressed = std::bind(Listeners::onKeyPressed, std::placeholders::_1, std::placeholders::_2, this->evolutionSession->population[0]);
    //     listener->onKeyReleased = std::bind(Listeners::onKeyReleased, std::placeholders::_1, std::placeholders::_2, this->evolutionSession->population[0]);
    //     _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    // }

    return true;
}

void MainScene::update(float delta)
{
    for (auto car : Session::evolutionSession->population)
        if (!car->isDead())
            car->update(delta);
}

void MainScene::menuCloseCallback(Ref *pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

MainScene::~MainScene()
{
}
