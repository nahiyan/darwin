#include "Obstacle.h"

USING_NS_CC;

Sprite *Obstacle::create(const Vec2 &position)
{
    auto node = Sprite::create("ball.png");
    node->setPosition(position);

    auto physicsBody = PhysicsBody::createCircle(5, PhysicsMaterial(0.1f, 1.0f, 1.0f));
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(2);
    physicsBody->setCollisionBitmask(5);
    physicsBody->setContactTestBitmask(1);
    node->addComponent(physicsBody);

    return node;
}
