#include "Food.h"

USING_NS_CC;

Food::Food(const Vec2 &position)
{
    this->node = DrawNode::create();
    this->node->drawDot(position, 5, Color4F::YELLOW);

    auto physicsBody = PhysicsBody::createCircle(5, PHYSICSBODY_MATERIAL_DEFAULT, position);
    physicsBody->setDynamic(false);
    physicsBody->setCategoryBitmask(2);
    physicsBody->setCollisionBitmask(1);
    physicsBody->setContactTestBitmask(1);

    this->node->addComponent(physicsBody);
}

DrawNode *Food::getNode()
{
    return this->node;
}
