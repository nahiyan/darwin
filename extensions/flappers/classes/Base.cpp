#include <extensions/flappers/Base.h>

using namespace Flappers;
USING_NS_CC;

Node *Base::create()
{
    auto screenWidth = Director::getInstance()->getVisibleSize().width;

    auto base = Node::create();
    base->setPosition(screenWidth / 2, 56);

    // Stack the sprites horizontally
    float hSpaceFilled = 0;
    while (true)
    {
        auto baseSprite = Sprite::createWithSpriteFrameName("base.png");
        baseSprite->setPosition(hSpaceFilled - (screenWidth / 2), 0);
        base->addChild(baseSprite);

        if (screenWidth - hSpaceFilled < 336)
        {
            // auto frameRectangle = baseSprite->getSpriteFrame()->getRect();
            // baseSprite->setTextureRect(Rect(frameRectangle.origin.x, frameRectangle.origin.y, frameRectangle.size.width, frameRectangle.size.height));
            break;
        }
        else
        {
            hSpaceFilled += 336;
        }
    }

    // Physics body
    auto physicsBody = PhysicsBody::createBox(Size(screenWidth, 112), PhysicsMaterial(0.1f, 0.0f, 0.1f));
    physicsBody->setDynamic(false);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(4);
    physicsBody->setCollisionBitmask(1);   // Flappers
    physicsBody->setContactTestBitmask(1); // Flappers
    base->addComponent(physicsBody);

    return base;
}
