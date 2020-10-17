#include "Obstacle.h"

#define IMPULSE -10 * 1000

USING_NS_CC;

Sprite *Obstacle::create(const Vec2 &position)
{
    auto node = Sprite::create("ball.png");
    node->setPosition(position);

    auto physicsBody = PhysicsBody::createCircle(16, PhysicsMaterial(0.1f, 0.0f, 0.0f));
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(true);
    physicsBody->setCategoryBitmask(2);
    physicsBody->setCollisionBitmask(12);  // 4 + 8
    physicsBody->setContactTestBitmask(9); // 1 + 8
    physicsBody->setAngularVelocity(1.8);
    node->addComponent(physicsBody);

    // physicsBody->setVelocity(Vec2(-100, 0));
    physicsBody->applyImpulse(Vec2(IMPULSE, 0));

    return node;
}
