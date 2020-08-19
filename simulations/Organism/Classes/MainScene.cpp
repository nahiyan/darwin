#include "MainScene.h"
#include "Organism.h"
#include <iostream>
#include <string>

USING_NS_CC;

DrawNode *organismNode;

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
    this->index = 0;

    if (!Scene::initWithPhysics())
    {
        return false;
    }

    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    LayerColor *_bgColor = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(_bgColor, -10);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Organism
    Organism organism(Vec2(200, 200));
    organismNode = organism.getNode();
    organismNode->getPhysicsBody()->setVelocity(Vec2(40, 40));
    this->addChild(organismNode, 2);

    // Set scheduler to update the velocity of the organism[s]
    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::organismsTick), 1, CC_REPEAT_FOREVER, 0);

    // Boundary
    const int boundaryCategoryBitmask = 4;
    const int boundaryCollisionBitmask = 1;
    const int boundaryContactTestBitmask = 0;

    {
        auto boundary = DrawNode::create();
        boundary->drawSolidRect(Vec2(0, 0), Vec2(5, visibleSize.height), Color4F::GRAY);

        auto physicsBody = PhysicsBody::createBox(Size(5, visibleSize.height - 10), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(2.5, visibleSize.height / 2));
        boundary->addComponent(physicsBody);
        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(boundaryCategoryBitmask);
        physicsBody->setCollisionBitmask(boundaryCollisionBitmask);
        physicsBody->setContactTestBitmask(boundaryContactTestBitmask);

        this->addChild(boundary, 2);
    }
    {
        auto boundary = DrawNode::create();
        boundary->drawSolidRect(Vec2(visibleSize.width - 5, 0), Vec2(visibleSize.width, visibleSize.height), Color4F::GRAY);

        auto physicsBody = PhysicsBody::createBox(Size(5, visibleSize.height - 10), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(visibleSize.width - 2.5, visibleSize.height / 2));
        boundary->addComponent(physicsBody);
        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(boundaryCategoryBitmask);
        physicsBody->setCollisionBitmask(boundaryCollisionBitmask);
        physicsBody->setContactTestBitmask(boundaryContactTestBitmask);

        this->addChild(boundary, 2);
    }
    {
        auto boundary = DrawNode::create();
        boundary->drawSolidRect(Vec2(0, 0), Vec2(visibleSize.width, 5), Color4F::GRAY);

        auto physicsBody = PhysicsBody::createBox(Size(visibleSize.width, 5), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(visibleSize.width / 2, 2.5));
        boundary->addComponent(physicsBody);
        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(boundaryCategoryBitmask);
        physicsBody->setCollisionBitmask(boundaryCollisionBitmask);
        physicsBody->setContactTestBitmask(boundaryContactTestBitmask);

        this->addChild(boundary, 2);
    }
    {
        auto boundary = DrawNode::create();
        boundary->drawSolidRect(Vec2(0, visibleSize.height), Vec2(visibleSize.width, visibleSize.height - 5), Color4F::GRAY);

        auto physicsBody = PhysicsBody::createBox(Size(visibleSize.width, 5), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(visibleSize.width / 2, visibleSize.height - 2.5));
        boundary->addComponent(physicsBody);
        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(boundaryCategoryBitmask);
        physicsBody->setCollisionBitmask(boundaryCollisionBitmask);
        physicsBody->setContactTestBitmask(boundaryContactTestBitmask);

        this->addChild(boundary, 2);
    }

    // Spawn food
    for (int i = 0; i < 100; i++)
    {
        Food food(Vec2(random((float)10, (float)(visibleSize.width - 10)), random((float)10, (float)(visibleSize.height - 10))));
        this->addChild(food.getNode(), 0, "food" + std::to_string(i));
    }

    this->index += 101;

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(MainScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    return true;
}

void MainScene::update(float delta)
{
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
            if (categoryBitmaskA == 2)
            {
                this->removeChildByName(bodyA->getNode()->getName());
            }
            else
            {
                this->removeChildByName(bodyB->getNode()->getName());
            }

            this->addFood();

            return false;
        }

        return true;
    }

    return false;
}

void MainScene::addFood()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    Food food(Vec2(random((float)10, (float)(visibleSize.width - 10)), random((float)10, (float)(visibleSize.height - 10))));
    this->addChild(food.getNode(), 0, "food" + std::to_string(this->index));

    this->index++;
}

void MainScene::organismsTick(float delta)
{
    int speedX = random(60, 130);
    int speedY = random(60, 130);

    int directionX = random(-1, 1) > 0 ? 1 : -1;
    int directionY = random(-1, 1) > 0 ? 1 : -1;

    organismNode->getPhysicsBody()->setVelocity(Vec2(speedX * directionX, speedY * directionY));
}
