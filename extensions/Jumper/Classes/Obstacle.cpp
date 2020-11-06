#include <extensions/jumper/Obstacle.h>

#define IMPULSE_SLOW -7 * 1000
#define IMPULSE_MEDIUM -10 * 1000
#define IMPULSE_FAST -13 * 1000

USING_NS_CC;

Sprite *Jumper::Obstacle::create(const Vec2 &position, Speed speed)
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

    int impulse;
    switch (speed)
    {
    case Slow:
        impulse = IMPULSE_SLOW;
        break;
    case Medium:
        impulse = IMPULSE_MEDIUM;
        break;
    case Fast:
        impulse = IMPULSE_FAST;
        break;
    }
    physicsBody->applyImpulse(Vec2(impulse, 0));

    return node;
}
