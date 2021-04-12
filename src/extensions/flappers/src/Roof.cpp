#include "Roof.h"

using namespace Flappers;
USING_NS_CC;

Node *Roof::create()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto roof = Node::create();
    roof->setPosition(visibleSize.width / 2, visibleSize.height);

    // Physics body
    auto physicsBody = PhysicsBody::createBox(Size(visibleSize.width, 5), PhysicsMaterial(0.1f, 0.0f, 0.1f));
    physicsBody->setDynamic(false);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(4);
    physicsBody->setCollisionBitmask(1);   // Flappers
    physicsBody->setContactTestBitmask(1); // Flappers
    roof->addComponent(physicsBody);

    return roof;
}
