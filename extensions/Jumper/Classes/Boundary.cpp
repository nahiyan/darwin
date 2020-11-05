#include <extensions/jumper/Boundary.h>

USING_NS_CC;

DrawNode **Jumper::Boundary::create(Size &visibleSize)
{
    const int boundaryCategoryBitmask = 4;
    const int boundaryCollisionBitmask = 3;
    const int boundaryContactTestBitmask = 0;

    auto boundaries = new DrawNode *[2];

    for (int i = 0; i < 2; i++)
        boundaries[i] = DrawNode::create();

    auto material = PhysicsMaterial(1.0f, 0.0f, 1.0f);

    // Left
    {
        boundaries[0]->drawSolidRect(Vec2(0, 0), Vec2(15, 100), Color4F::GRAY);
        boundaries[0]->setPosition(Vec2(visibleSize.width - 224, 15));

        auto physicsBody = PhysicsBody::createBox(Size(15, 100), material, Vec2(7.5, 50));
        boundaries[0]->addComponent(physicsBody);
    }
    // Bottom
    {
        boundaries[1]->drawSolidRect(Vec2(0, 0), Vec2(visibleSize.width, 15), Color4F::GRAY);

        auto physicsBody = PhysicsBody::createBox(Size(visibleSize.width, 15), material, Vec2(visibleSize.width / 2, 7.5));
        boundaries[1]->addComponent(physicsBody);
    }

    for (int i = 0; i < 2; i++)
    {
        auto physicsBody = boundaries[i]->getPhysicsBody();

        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(i == 0 ? 8 : boundaryCategoryBitmask);
        physicsBody->setCollisionBitmask(boundaryCollisionBitmask);
        physicsBody->setContactTestBitmask(i == 0 ? 2 : boundaryContactTestBitmask);
    }

    return boundaries;
}
