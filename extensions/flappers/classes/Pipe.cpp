#include <extensions/flappers/Pipe.h>
#include <extensions/flappers/Session.h>

#define BASE_HEIGHT 112
#define MIDDLE_SPACE 150.0f

using namespace Flappers;
USING_NS_CC;

Node *Pipe::create(float topFraction)
{
    auto screenHeight = Director::getInstance()->getVisibleSize().height;
    auto verticalSpace = screenHeight - BASE_HEIGHT - 48 - MIDDLE_SPACE;
    auto topPipeHeight = topFraction * verticalSpace;
    auto bottomPipeHeight = verticalSpace - topPipeHeight;

    auto pipe = Node::create();
    pipe->setPosition(Director::getInstance()->getVisibleSize().width, 0);

    auto topPipeBody = Sprite::createWithSpriteFrameName("pipe-green-body.png");
    topPipeBody->setPosition(0, screenHeight - (topPipeHeight / 2));
    topPipeBody->setScaleY(topPipeHeight / 296);
    topPipeBody->setFlippedY(true);
    pipe->addChild(topPipeBody);

    auto topPipeHead = Sprite::createWithSpriteFrameName("pipe-green-head.png");
    topPipeHead->setPosition(0, screenHeight - topPipeHeight - 12);
    pipe->addChild(topPipeHead);

    auto bottomPipeBody = Sprite::createWithSpriteFrameName("pipe-green-body.png");
    bottomPipeBody->setPosition(0, (bottomPipeHeight / 2) + BASE_HEIGHT);
    bottomPipeBody->setScaleY(bottomPipeHeight / 296);
    pipe->addChild(bottomPipeBody);

    auto bottomPipeHead = Sprite::createWithSpriteFrameName("pipe-green-head.png");
    bottomPipeHead->setPosition(0, bottomPipeHeight + BASE_HEIGHT + 12);
    pipe->addChild(bottomPipeHead);

    // Physics body
    auto physicsBody = PhysicsBody::create();
    physicsBody->setDynamic(false);
    physicsBody->setGravityEnable(false);
    pipe->addComponent(physicsBody);

    // Physics shapes
    {
        auto shape = PhysicsShapeBox::create(Size(52, bottomPipeHeight + 24), PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(0, BASE_HEIGHT + ((bottomPipeHeight + 24) / 2)));
        shape->setCategoryBitmask(2);
        shape->setCollisionBitmask(1);   // Flappers
        shape->setContactTestBitmask(1); // Flappers
        physicsBody->addShape(shape);
    }
    {
        auto shape = PhysicsShapeBox::create(Size(52, topPipeHeight + 24), PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(0, MIDDLE_SPACE + BASE_HEIGHT + bottomPipeHeight + 36 + topPipeHeight / 2));
        shape->setCategoryBitmask(2);
        shape->setCollisionBitmask(1);   // Flappers
        shape->setContactTestBitmask(1); // Flappers
        physicsBody->addShape(shape);
    }

    pipe->getPhysicsBody()->setVelocity(Vec2(-100, 0));

    Session::pipes.push_back(pipe);

    return pipe;
}