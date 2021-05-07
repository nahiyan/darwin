#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <opennn/neural_network.h>
#include <helpers/time.h>
#include "MainScene.h"
#include "Listeners.h"
#include "core/Session.h"
#include "core/Debug.h"
#include "core/HUD.h"
#include "Session.h"
#include "core/EvolutionCommon.h"
#include "core/Listeners.h"
#include "Pipe.h"
#include "Base.h"
#include "Roof.h"
#include "persistent_models.h"
#include "rapidjson/document.h"

USING_NS_CC;
using namespace Flappers;
using namespace rapidjson;

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
    this->setSpeed(Core::Session::speed);

    // Control gravity
    this->getPhysicsWorld()->setGravity(Vec2(0, -500));

    // White background
    LayerColor *_bgColor = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(_bgColor, -10);

    // Load sprite sheet
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprite-sheet.plist");

    // Add initial pipe
    this->addPipe();

    // Evolution session
    Session::evolutionSession = new Core::EvolutionSession<Flapper>(Core::Session::mutationRate, Core::Session::eliteFraction, Core::Session::fertileFraction, Core::Session::randomFraction);

    // Database
    std::vector<double> nnParameters[Core::Session::populationSize];

    // Initialize Persistent Models
    if (Session::modelsFilePath.size() > 0)
        Session::pm = pm_load_file(Session::modelsFilePath.c_str());

    // Load models from the models file
    if (Core::Session::startFromSavedModels)
    {
        int modelsCount = min((int)pm_count(&Session::pm), Core::Session::populationSize);

        for (int i = 0; i < modelsCount; i++)
        {
            auto definition = pm_get_model(&Session::pm, i).definition;

            Document document;
            document.Parse<rapidjson::ParseFlag::kParseFullPrecisionFlag>(definition);
            if (document.HasMember("genome") && document["genome"].IsArray())
            {
                auto genome = document["genome"].GetArray();
                auto genomeSize = genome.Size();
                for (int j = 0; j < genomeSize; j++)
                    nnParameters[i].push_back(genome[j].GetDouble());
            }
            pm_free_string((char *)definition);
        }
    }

    // Add flappers
    for (int i = 0; i < Core::Session::populationSize; i++)
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

    // Keyboard listener
    {
        auto listener = EventListenerKeyboard::create();
        listener->onKeyPressed = std::bind(Listeners::onKeyPressed, std::placeholders::_1, std::placeholders::_2, Session::evolutionSession->population[0]);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }

    // Base
    this->addChild(Base::create(), -1);

    // Roof
    this->addChild(Roof::create(), -1);

    // HUD
    Session::hud = Core::HUD::create(Session::evolutionSession->getMutationRate());
    this->addChild(Session::hud, 10);

    // Core listeners
    {
        auto listener = EventListenerKeyboard::create();
        listener->onKeyPressed = std::bind(Core::Listeners<Flapper, MainScene>::onKeyPressed, std::placeholders::_1, std::placeholders::_2, Session::hud, Session::evolutionSession, this);
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

    // Add pipe at constant interval
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

void MainScene::setSpeed(int speed)
{
    Director::getInstance()->getScheduler()->setTimeScale(Core::Session::speed);
    this->getPhysicsWorld()->setSpeed(Core::Session::speed);
    this->getPhysicsWorld()->setSubsteps(Core::Session::speed);
}
