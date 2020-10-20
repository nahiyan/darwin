#include <extensions/jumper/Boundary.h>

USING_NS_CC;

DrawNode **Jumper::Boundary::create(Size &visibleSize)
{
    const int boundaryCategoryBitmask = 4;
    const int boundaryCollisionBitmask = 3;
    const int boundaryContactTestBitmask = 0;

    DrawNode **boundaries = (DrawNode **)malloc(sizeof(DrawNode *) * 4);

    for (int i = 0; i < 4; i++)
        boundaries[i] = DrawNode::create();

    auto material = PhysicsMaterial(1.0f, 0.0f, 1.0f);

    // Left
    {
        boundaries[0]->drawSolidRect(Vec2(0, 0), Vec2(5, visibleSize.height), Color4F::GRAY);

        auto physicsBody = PhysicsBody::createBox(Size(5, visibleSize.height - 10), material, Vec2(2.5, visibleSize.height / 2));
        boundaries[0]->addComponent(physicsBody);
    }
    // Right
    {
        boundaries[1]->drawSolidRect(Vec2(visibleSize.width - 5, 0), Vec2(visibleSize.width, visibleSize.height), Color4F::GRAY);

        auto physicsBody = PhysicsBody::createBox(Size(5, visibleSize.height - 10), material, Vec2(visibleSize.width - 2.5, visibleSize.height / 2));
        boundaries[1]->addComponent(physicsBody);
    }
    // Bottom
    {
        boundaries[2]->drawSolidRect(Vec2(0, 0), Vec2(visibleSize.width, 5), Color4F::GRAY);

        auto physicsBody = PhysicsBody::createBox(Size(visibleSize.width, 5), material, Vec2(visibleSize.width / 2, 2.5));
        boundaries[2]->addComponent(physicsBody);
    }
    // Top
    {
        boundaries[3]->drawSolidRect(Vec2(0, visibleSize.height), Vec2(visibleSize.width, visibleSize.height - 5), Color4F::GRAY);

        auto physicsBody = PhysicsBody::createBox(Size(visibleSize.width, 5), material, Vec2(visibleSize.width / 2, visibleSize.height - 2.5));
        boundaries[3]->addComponent(physicsBody);
    }

    for (int i = 0; i < 4; i++)
    {
        auto physicsBody = boundaries[i]->getPhysicsBody();

        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(i == 0 ? 8 : boundaryCategoryBitmask);
        physicsBody->setCollisionBitmask(boundaryCollisionBitmask);
        physicsBody->setContactTestBitmask(i == 0 ? 2 : boundaryContactTestBitmask);
    }

    return boundaries;
}
