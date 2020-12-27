#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <opennn/neural_network.h>
#include <helpers/time.h>
#include <extensions/flappers/MainScene.h>
#include <extensions/flappers/Listeners.h>
#include <core/Database.h>
#include <core/CoreSession.h>
#include <core/Debug.h>
#include <core/HUD.h>
#include <extensions/flappers/Session.h>
#include <extensions/flappers/GenerationState_generated.h>
#include <core/EvolutionCommon.h>
#include <core/Listeners.h>
#include <extensions/flappers/Pipe.h>
#include <extensions/flappers/Base.h>
#include <extensions/flappers/Roof.h>

#define POPULATION_SIZE 50

USING_NS_CC;
using namespace Flappers;

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
    Director::getInstance()->getScheduler()->setTimeScale(CoreSession::speed);
    this->getPhysicsWorld()->setSpeed(CoreSession::speed);

    // this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // Control gravity
    this->getPhysicsWorld()->setGravity(Vec2(0, -500));

    // White background
    LayerColor *_bgColor = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(_bgColor, -10);

    // Load sprite sheet
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("flappers/sprite-sheet.plist");

    // Add initial pipe
    // this->schedule(CC_SCHEDULE_SELECTOR(MainScene::addPipe), 2.5, CC_REPEAT_FOREVER, 0);
    this->addPipe();

    // Evolution session
    Session::evolutionSession = new EvolutionSession<Flapper>(.01, 0.05, .3, .05);

    // // Database
    std::vector<double> nnParameters[POPULATION_SIZE];

    if (CoreSession::sessionId == 0)
    {
        // Create new session
        CoreSession::extensionId = Database::getExtensionId("Flappers");
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
            for (int j = 0; j < state->population()->Get(i)->chromosomes()->size(); j++)
                nnParameters[i].push_back(state->population()->Get(i)->chromosomes()->Get(j));

        Session::evolutionSession->evolve(EvolutionCommon<Flapper>::crossoverAndMutate, EvolutionCommon<Flapper>::randomize);
    }

    // Add flappers
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        auto flapper = new Flapper(nnParameters[i]);
        this->addChild(flapper->node, 0, i);
        Session::evolutionSession->population.push_back(flapper);
    }

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
    {
        auto listener = EventListenerKeyboard::create();
        listener->onKeyPressed = std::bind(Listeners::onKeyPressed, std::placeholders::_1, std::placeholders::_2, Session::evolutionSession->population[0]);
        // listener->onKeyReleased = std::bind(Listeners::onKeyReleased, std::placeholders::_1, std::placeholders::_2, Session::evolutionSession->population[0]);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }

    // Mouse listener
    // {
    //     auto listener = EventListenerMouse::create();
    //     listener->onMouseMove = Listeners::onMouseMove;
    //     _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    // }

    // Base
    this->addChild(Base::create(), -1);

    // Roof
    this->addChild(Roof::create(), -1);

    // HUD
    Session::hud = HUD::create(Session::evolutionSession->getMutationRate());
    this->addChild(Session::hud, 10);

    // Core listeners
    {
        auto listener = EventListenerKeyboard::create();
        listener->onKeyPressed = std::bind(Core::Listeners<Flapper>::onKeyPressed, std::placeholders::_1, std::placeholders::_2, Session::hud, Session::evolutionSession);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }

    return true;
}

void MainScene::update(float delta)
{
    for (auto &pipe : Session::pipes)
    {
        auto position = pipe->getParent()->convertToWorldSpace(pipe->getPosition());
        if (position.x < -26)
        {
            this->removeChild(pipe);
            Session::pipes.erase(Session::pipes.begin());
        }
    }

    int flapperCount = 0;
    for (auto &flapper : Session::evolutionSession->population)
        if (!flapper->isDead())
        {
            flapper->update(delta);
            flapperCount++;
        }

    if (flapperCount == 0)
        Session::nextGeneration();

    Session::timeSinceLastPipe += delta;
    if (Session::timeSinceLastPipe >= 2.5)
    {
        this->addPipe();
        Session::timeSinceLastPipe = 0;
    }
}

void MainScene::menuCloseCallback(Ref *pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

MainScene::~MainScene()
{
}

void MainScene::addPipe()
{
    float values[10] = {0.3, .8, 0.5, 0.25, 0.5, 0.15, 0.35, 0.9, 0.85, 0.6};

    auto pipe = Pipe::create(values[Session::pipeCounter]);
    // auto pipe = Pipe::create(random<float>(0.1, 0.9));
    this->addChild(pipe);
    if (Session::pipeCounter == 9)
        Session::nextGeneration();
    else
        Session::pipeCounter++;

    Session::timeSinceLastPipe = 0;
}
