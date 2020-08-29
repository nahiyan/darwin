#include "Food.h"

USING_NS_CC;

Sprite* Food::create(const Vec2 &position)
{
    auto node = Sprite::create("food.png");
    node->setPosition(position);

    auto physicsBody = PhysicsBody::createCircle(5, PHYSICSBODY_MATERIAL_DEFAULT);
    physicsBody->setDynamic(false);
    physicsBody->setCategoryBitmask(2);
    physicsBody->setCollisionBitmask(1);
    physicsBody->setContactTestBitmask(1);
    node->addComponent(physicsBody);
    
    return node;
}
