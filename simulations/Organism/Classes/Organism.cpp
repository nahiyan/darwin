#include "Organism.h"
#include <iostream>

USING_NS_CC;

Organism::Organism(const Vec2 &position)
{
    this->node = DrawNode::create();
    this->node->setPosition(position);
    this->setGeometry();

    auto physicsBody = PhysicsBody::createCircle(20, PhysicsMaterial(0.1f, 1.0f, 0.0f));
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(1);
    physicsBody->setCollisionBitmask(6);
    physicsBody->setContactTestBitmask(2);

    this->node->addComponent(physicsBody);

    this->foodIntersection = false;
}

void Organism::setGeometry()
{
    this->node->drawDot(Vec2::ZERO, 20, Color4F::GREEN);
    this->node->drawLine(Vec2::ZERO + Vec2(0, 20), Vec2::ZERO + Vec2(40, 100), this->foodIntersection ? Color4F::GREEN : Color4F::RED);
    this->node->drawLine(Vec2::ZERO + Vec2(0, 20), Vec2::ZERO + Vec2(-40, 100), this->foodIntersection ? Color4F::GREEN : Color4F::RED);
}

void Organism::setFoodIntersection()
{
    this->foodIntersection = true;

    this->node->clear();
    this->setGeometry();
}
void Organism::unsetFoodIntersection()
{
    this->foodIntersection = false;

    this->node->clear();
    this->setGeometry();
}

DrawNode *Organism::getNode()
{
    return this->node;
}
