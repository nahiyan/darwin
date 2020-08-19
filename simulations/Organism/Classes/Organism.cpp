#include "Organism.h"
#include <iostream>

USING_NS_CC;

Organism::Organism(const Vec2 &position)
{
    this->node = DrawNode::create();
    this->node->drawDot(position, 20, Color4F::GREEN);

    auto physicsBody = PhysicsBody::createCircle(20, PhysicsMaterial(0.1f, 1.0f, 0.0f), position);
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(1);
    physicsBody->setCollisionBitmask(6);
    physicsBody->setContactTestBitmask(2);

    this->node->addComponent(physicsBody);
}

DrawNode *Organism::getNode()
{
    return this->node;
}
