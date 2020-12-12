#include <extensions/flappers/Pipe.h>
#include <extensions/flappers/Session.h>
#include <extensions/flappers/Constants.h>

using namespace Flappers;
USING_NS_CC;

Node *Pipe::create(float topFraction)
{
    auto screenHeight = Director::getInstance()->getVisibleSize().height;
    auto verticalSpace = screenHeight - BASE_HEIGHT - PIPE_HEAD_HEIGHT * 2 - PIPE_MIDDLE_SPACE;
    auto topPipeHeight = topFraction * verticalSpace;
    auto bottomPipeHeight = verticalSpace - topPipeHeight;

    auto pipe = Node::create();
    pipe->setPosition(Director::getInstance()->getVisibleSize().width, 0);

    auto topPipeBody = Sprite::createWithSpriteFrameName("pipe-green-body.png");
    topPipeBody->setPosition(0, screenHeight - (topPipeHeight / 2));
    topPipeBody->setScaleY(topPipeHeight / PIPE_BODY_HEIGHT);
    topPipeBody->setFlippedY(true);
    pipe->addChild(topPipeBody);

    auto topPipeHead = Sprite::createWithSpriteFrameName("pipe-green-head.png");
    topPipeHead->setPosition(0, screenHeight - topPipeHeight - (PIPE_HEAD_HEIGHT / 2));
    pipe->addChild(topPipeHead);

    auto bottomPipeBody = Sprite::createWithSpriteFrameName("pipe-green-body.png");
    bottomPipeBody->setPosition(0, (bottomPipeHeight / 2) + BASE_HEIGHT);
    bottomPipeBody->setScaleY(bottomPipeHeight / PIPE_BODY_HEIGHT);
    pipe->addChild(bottomPipeBody);

    auto bottomPipeHead = Sprite::createWithSpriteFrameName("pipe-green-head.png");
    bottomPipeHead->setPosition(0, bottomPipeHeight + BASE_HEIGHT + (PIPE_HEAD_HEIGHT / 2));
    pipe->addChild(bottomPipeHead);

    // Physics body
    auto physicsBody = PhysicsBody::create();
    physicsBody->setDynamic(false);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(2);
    physicsBody->setCollisionBitmask(1);   // Flappers
    physicsBody->setContactTestBitmask(1); // Flappers
    pipe->addComponent(physicsBody);

    // Physics shapes
    {
        auto shape = PhysicsShapeBox::create(Size(PIPE_BODY_WIDTH, bottomPipeHeight + PIPE_HEAD_HEIGHT), PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(0, BASE_HEIGHT + ((bottomPipeHeight + PIPE_HEAD_HEIGHT) / 2)));
        shape->setCategoryBitmask(2);
        shape->setCollisionBitmask(1);   // Flappers
        shape->setContactTestBitmask(1); // Flappers
        physicsBody->addShape(shape);
    }
    {
        auto shape = PhysicsShapeBox::create(Size(PIPE_BODY_WIDTH, topPipeHeight + PIPE_HEAD_HEIGHT), PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(0, PIPE_MIDDLE_SPACE + BASE_HEIGHT + bottomPipeHeight + PIPE_HEAD_HEIGHT * 1.5 + topPipeHeight / 2));
        shape->setCategoryBitmask(2);
        shape->setCollisionBitmask(1);   // Flappers
        shape->setContactTestBitmask(1); // Flappers
        physicsBody->addShape(shape);
    }

    pipe->getPhysicsBody()->setVelocity(Vec2(-100, 0));
    Session::pipes.push_back(pipe);

    return pipe;
}