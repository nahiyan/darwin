#include "MainScene.h"
#include "Organism.h"
#include "Food.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <neural_network.h>

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

    this->index = 0;

    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    LayerColor *_bgColor = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(_bgColor, -10);

    this->visibleSize = Director::getInstance()->getVisibleSize();

    // Organism
    for (int i = 0; i < 20; i++)
    {
        auto organism = new Organism(Vec2(this->visibleSize.width / 2, this->visibleSize.height / 2));
        this->organismList.push_back(organism);
        this->addChild(organism->node, 2);
    }

    // Set scheduler to update the velocity of the organism[s]
    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::organismsTick), 2.5, CC_REPEAT_FOREVER, 0);
    this->organismsTick(0);

    // Boundary
    this->addBoundary();

    // Spawn food
    this->addFood(100);

    // Contact listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactPostSolve = CC_CALLBACK_1(MainScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    // Mouse listener
//    auto mouseListener = EventListenerMouse::create();
//    mouseListener->onMouseMove = CC_CALLBACK_1(MainScene::onMouseMove, this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    return true;
}

void MainScene::update(float delta)
{
    auto rayCastCB = [](PhysicsWorld &world, const PhysicsRayCastInfo &info, void *organism) -> bool {
        if (info.shape->getBody()->getCategoryBitmask() == 2)
        {
            ((Organism *)organism)->setFoodIntersection();
            ((Organism *)organism)->targetedFoodName = info.shape->getBody()->getNode()->getName();

            return false;
        }

        return true;
    };

    for (auto organism : this->organismList)
    {
        // Raycast to determine if food is intersecting the antennae
        auto organismPosition = organism->node->getParent()->convertToWorldSpace(organism->node->getPosition());

        organism->unsetFoodIntersection();
        auto organismAngle = -organism->node->getPhysicsBody()->getRotation() * (M_PI / 180);

        auto rayCastStartingPosition = (organismPosition + Vec2(0, 20)).rotateByAngle(organismPosition, organismAngle);
        auto rayCastEndingPosition1 = (organismPosition + Vec2(40, 100)).rotateByAngle(organismPosition, organismAngle);
        auto rayCastEndingPosition2 = (organismPosition + Vec2(-40, 100)).rotateByAngle(organismPosition, organismAngle);

        this->getPhysicsWorld()->rayCast(rayCastCB, rayCastStartingPosition, rayCastEndingPosition1, organism);
        this->getPhysicsWorld()->rayCast(rayCastCB, rayCastStartingPosition, rayCastEndingPosition2, organism);

        // Set organism rotation
        auto organismVelocity = organism->node->getPhysicsBody()->getVelocity();
        float desiredRotation = organismVelocity.getAngle(Vec2(0, 1)) * (180 / M_PI);
        float organismRotation = organism->node->getPhysicsBody()->getRotation();
        float absRotDiff = abs(desiredRotation - organismRotation);
        float rotationChange = std::min(absRotDiff, 1.5f);

        if (desiredRotation > organismRotation)
            organism->node->setRotation(organismRotation + rotationChange);
        else if (desiredRotation < organismRotation)
            organism->node->setRotation(organismRotation - rotationChange);

        // Neural network
//        OpenNN::Tensor<double> input{1, 2};
//        input[0] = (double)organism->getFoodIntersection();
//        input[1] = organism->node->getPhysicsBody()->getVelocity().length();
//
//        auto output = organism->neuralNetwork->calculate_outputs(input);
//
//        bool goAfterFood = output[0] > 0.5 ? true : false;
//        bool shouldChangeVelocity = output[1] > 0.5 ? true : false;
//
//        if (goAfterFood && shouldChangeVelocity && organism->targetedFoodName.length() > 0)
//        {
//            auto targetedFood = this->getChildByName(organism->targetedFoodName);
//
//            if (targetedFood != nullptr)
//            {
//
//                int speedX = random(60, 130);
//                int speedY = random(60, 130);
//                auto foodPosition = targetedFood->getParent()->convertToWorldSpace(targetedFood->getPosition());
//
//                Vec2 newVelocity(random(60, 130), random(60, 130));
//                newVelocity.rotateByAngle(Vec2(0, 0), Vec2(0, 1).getAngle(foodPosition));
//
//                organism->node->getPhysicsBody()->setVelocity(foodPosition);
//            }
//        }
//        else if (!goAfterFood && shouldChangeVelocity)
//        {
//            int speedX = random(60, 130);
//            int speedY = random(60, 130);
//
//            int directionX = random(-1, 1) > 0 ? 1 : -1;
//            int directionY = random(-1, 1) > 0 ? 1 : -1;
//
//            organism->node->getPhysicsBody()->setVelocity(Vec2(speedX * directionX, speedY * directionY));
//        }
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
            auto foodNode = (categoryBitmaskA == 2 ? bodyA : bodyB)->getNode();
            if (foodNode != nullptr)
            {
                this->removeChild(foodNode);
                cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
                    this->addFood(1);
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
    auto organismNewPosition = this->organismList[0]->node->getParent()->convertToNodeSpace(mousePosition);

    this->organismList[0]->node->setPosition(organismNewPosition);
}

void MainScene::addFood(int count)
{
    for (int i = 0; i < count; i++){
        auto location = Vec2(random((float)10, (float)(this->visibleSize.width - 10)), random((float)10, (float)(this->visibleSize.height - 10)));
        
        auto food = Food::create(location);
        
        this->addChild(food, 0, "food" + std::to_string(this->index));
        this->index++;
    }
}

void MainScene::addBoundary() {
    const int boundaryCategoryBitmask = 4;
    const int boundaryCollisionBitmask = 1;
    const int boundaryContactTestBitmask = 0;

    {
        auto boundary = DrawNode::create();
        boundary->drawSolidRect(Vec2(0, 0), Vec2(5, this->visibleSize.height), Color4F::GRAY);

        auto physicsBody = PhysicsBody::createBox(Size(5, this->visibleSize.height - 10), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(2.5, this->visibleSize.height / 2));
        boundary->addComponent(physicsBody);
        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(boundaryCategoryBitmask);
        physicsBody->setCollisionBitmask(boundaryCollisionBitmask);
        physicsBody->setContactTestBitmask(boundaryContactTestBitmask);

        this->addChild(boundary, 2);
    }
    {
        auto boundary = DrawNode::create();
        boundary->drawSolidRect(Vec2(this->visibleSize.width - 5, 0), Vec2(this->visibleSize.width, this->visibleSize.height), Color4F::GRAY);

        auto physicsBody = PhysicsBody::createBox(Size(5, this->visibleSize.height - 10), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(this->visibleSize.width - 2.5, this->visibleSize.height / 2));
        boundary->addComponent(physicsBody);
        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(boundaryCategoryBitmask);
        physicsBody->setCollisionBitmask(boundaryCollisionBitmask);
        physicsBody->setContactTestBitmask(boundaryContactTestBitmask);

        this->addChild(boundary, 2);
    }
    {
        auto boundary = DrawNode::create();
        boundary->drawSolidRect(Vec2(0, 0), Vec2(this->visibleSize.width, 5), Color4F::GRAY);

        auto physicsBody = PhysicsBody::createBox(Size(this->visibleSize.width, 5), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(this->visibleSize.width / 2, 2.5));
        boundary->addComponent(physicsBody);
        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(boundaryCategoryBitmask);
        physicsBody->setCollisionBitmask(boundaryCollisionBitmask);
        physicsBody->setContactTestBitmask(boundaryContactTestBitmask);

        this->addChild(boundary, 2);
    }
    {
        auto boundary = DrawNode::create();
        boundary->drawSolidRect(Vec2(0, this->visibleSize.height), Vec2(this->visibleSize.width, this->visibleSize.height - 5), Color4F::GRAY);

        auto physicsBody = PhysicsBody::createBox(Size(this->visibleSize.width, 5), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(this->visibleSize.width / 2, this->visibleSize.height - 2.5));
        boundary->addComponent(physicsBody);
        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(boundaryCategoryBitmask);
        physicsBody->setCollisionBitmask(boundaryCollisionBitmask);
        physicsBody->setContactTestBitmask(boundaryContactTestBitmask);

        this->addChild(boundary, 2);
    }
}

void MainScene::organismsTick(float delta)
{
     for (auto organism : this->organismList)
     {
         if (random(-1, 1) > 0 || delta == 0)
         {
             int speedX = random(60, 130);
             int speedY = random(60, 130);

             int directionX = random(-1, 1) > 0 ? 1 : -1;
             int directionY = random(-1, 1) > 0 ? 1 : -1;

             Vec2 newVelocity;
             newVelocity.x = speedX * directionX;
             newVelocity.y = speedY * directionY;

             organism->node->getPhysicsBody()->setVelocity(newVelocity);
         }
     }
}

MainScene::~MainScene()
{
    for (auto organism : this->organismList)
        delete organism;
}
