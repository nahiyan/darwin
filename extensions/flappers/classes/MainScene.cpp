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
#include <extensions/flappers/Session.h>
#include <extensions/flappers/GenerationState_generated.h>
#include <extensions/flappers/Evolution.h>
#include <extensions/flappers/Pipe.h>
#include <extensions/flappers/Base.h>

#define POPULATION_SIZE 1
#define SPEED 1

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
    Director::getInstance()->getScheduler()->setTimeScale(SPEED);
    this->getPhysicsWorld()->setSpeed(SPEED);

    //    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // Control gravity
    this->getPhysicsWorld()->setGravity(Vec2(0, -750));

    // White background
    LayerColor *_bgColor = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(_bgColor, -10);

    // Load sprite sheet
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("flappers/sprite-sheet.plist");

    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::addPipe), 2.5, CC_REPEAT_FOREVER, 0);
    this->addPipe(0);

    // Evolution session
    Session::setPopulationSize(POPULATION_SIZE);
    Session::evolutionSession = new EvolutionSession<Flapper>;

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
        {
            for (int j = 0; j < state->population()->Get(i)->chromosomes()->size(); j++)
            {
                nnParameters[i].push_back(state->population()->Get(i)->chromosomes()->Get(j));
            }
        }

        Session::evolutionSession->evolve(Evolution::crossoverAndMutate);
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
    auto base = Base::create();
    this->addChild(base, -1);

    return true;
}

void MainScene::update(float delta)
{
    int i = 0;
    for (auto &pipe : Session::pipes)
    {
        auto position = pipe->getParent()->convertToWorldSpace(pipe->getPosition());
        if (position.x <= 0)
        {
            this->removeChild(pipe);
            Session::pipes.erase(Session::pipes.begin() + i);
        }
        i++;
    }
    // for (auto car : Session::evolutionSession->population)
    //     if (!car->isDead())
    //         car->update(delta);
}

void MainScene::menuCloseCallback(Ref *pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

MainScene::~MainScene()
{
}

void MainScene::addPipe(float delta)
{
    auto pipe = Pipe::create(random<float>(0.1, 0.9));
    this->addChild(pipe);
}
