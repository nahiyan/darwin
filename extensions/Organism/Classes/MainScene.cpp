#include "MainScene.h"
#include "Organism.h"
#include "Boundary.h"
#include "Evolution.h"
#include "Food.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <neural_network.h>
#include <ctime>

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

    auto organismCount = 20;

    // this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // White background
    LayerColor *_bgColor = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(_bgColor, -10);

    this->visibleSize = Director::getInstance()->getVisibleSize();

    // Timer
    this->generationStartTimestamp = std::time(nullptr);

    // Organism
    for (int i = 0; i < organismCount; i++)
    {
        auto organism = new Organism(Vec2(this->visibleSize.width / 2, this->visibleSize.height / 2));
        this->organismMap.emplace(i, organism);
        this->addChild(organism->node, 2, i);
    }

    // Set scheduler to update the velocity of the organism[s]
    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::organismsTick), 2.5, CC_REPEAT_FOREVER, 0);
    this->organismsTick(0);

    // Boundary
    auto boundaries = Boundary::create(this->visibleSize);
    for (int i = 0; i < 4; i++)
    {
        this->addChild(boundaries[i]);
    }
    delete[] boundaries;

    // Spawn food
    this->index = 0;
    this->foodToBeAdded = 40;
    this->addFood(40);
    this->totalFoodEaten = 0;

    // Evolution session
    this->evolutionSession = new Evolution(&this->organismMap);

    // Contact listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactPostSolve = CC_CALLBACK_1(MainScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    // Mouse listener
    // auto mouseListener = EventListenerMouse::create();
    // mouseListener->onMouseMove = CC_CALLBACK_1(MainScene::onMouseMove, this);
    // _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    return true;
}

void MainScene::update(float delta)
{
    for (auto organism : this->organismMap)
        organism.second->update(delta);

    // auto children = this->getChildren();
    // for (auto child : children)
    // {
    //     auto physicsBody = child->getPhysicsBody();
    //     if (physicsBody != nullptr && physicsBody->getCategoryBitmask() == 2)
    //     {
    //         auto force = this->organismList[0]->node->convertToWorldSpace(this->organismList[0]->node->getPosition()) - child->convertToWorldSpace(child->getPosition());
    //         force.normalize();
    //         force.scale(1000);
    //         physicsBody->applyForce(force, Vec2::ZERO);
    //     }
    // }
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
            auto foodNode = (categoryBitmaskA == 2 ? bodyA : bodyB)->getNode();
            auto organism = this->organismMap[(categoryBitmaskA == 1 ? bodyA : bodyB)->getNode()->getTag()];

            if (foodNode != nullptr)
            {
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                    // std::cout << foodNode->getName() << " Food to be added: " << to_string(this->foodToBeAdded) << "\n";

                    this->removeChild(foodNode);
                    this->foodToBeAdded++;
                    organism->eatFood();
                    this->totalFoodEaten++;

                    // Start a new generation
                    if (this->totalFoodEaten >= 100)
                    {
                        this->evolutionSession->nextGeneration();
                        this->prepareNextGeneration();
                    }
                });
            }

            return false;
        }

        return true;
    }

    return false;
}

void MainScene::onMouseMove(EventMouse *e)
{
    auto mousePosition = Vec2(e->getCursorX(), e->getCursorY());
    auto organismNewPosition = this->organismMap[0]->node->getParent()->convertToNodeSpace(mousePosition);

    this->organismMap[0]->node->setPosition(organismNewPosition);
}

void MainScene::addFood(int count)
{
    for (int i = 0; i < count; i++)
    {
        auto location = Vec2(random((float)10, (float)(this->visibleSize.width - 10)), random((float)10, (float)(this->visibleSize.height - 10)));

        auto food = Food::create(location);

        this->addChild(food, 0, "food" + to_string(this->index++));

        this->foodToBeAdded--;
    }
}

void MainScene::organismsTick(float delta)
{
    if (std::time(nullptr) - this->generationStartTimestamp > 10)
    {
        this->evolutionSession->nextGeneration();
        this->prepareNextGeneration();
    }

    // for (auto organism : this->organismList)
    // {
    //     if (random(-1, 1) > 0 || delta == 0)
    //     {
    //         int speedX = random(60, 130);
    //         int speedY = random(60, 130);

    //         int directionX = random(-1, 1) > 0 ? 1 : -1;
    //         int directionY = random(-1, 1) > 0 ? 1 : -1;

    //         Vec2 newVelocity;
    //         newVelocity.x = speedX * directionX;
    //         newVelocity.y = speedY * directionY;

    //         organism->node->getPhysicsBody()->setVelocity(newVelocity);
    //     }
    // }
}

void MainScene::prepareNextGeneration()
{
    std::cout << "Food eaten:" << to_string(this->totalFoodEaten) << "\n";

    this->totalFoodEaten = 0;
    this->generationStartTimestamp = std::time(nullptr);

    // Reset organisms
    for (auto organism : this->organismMap)
        organism.second->reset();

    // Set velocity of all objects to zero
    for (auto child : this->getChildren())
    {
        auto physicsBody = child->getPhysicsBody();
        if (physicsBody != nullptr)
            physicsBody->setVelocity(Vec2::ZERO);
    }

    // Add all the food items over again
    for (auto child : this->getChildren())
    {
        auto physicsBody = child->getPhysicsBody();
        if (physicsBody != nullptr && physicsBody->getCategoryBitmask() == 2)
        {
            this->removeChild(child);
        }
    }

    this->addFood(40);
}

MainScene::~MainScene()
{
    for (auto organism : this->organismMap)
        delete organism.second;

    delete this->evolutionSession;
}
