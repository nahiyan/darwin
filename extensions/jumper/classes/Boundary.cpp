#include <extensions/jumper/Boundary.h>

#define BOUNDARY_COUNT 4
#define BOUNDARY_CATBM 4
#define BOUNDARY_COLBM 3
#define BOUNDARY_COTBM 0

USING_NS_CC;

DrawNode **Jumper::Boundary::create()
{
    auto visibleWidth = Director::getInstance()->getVisibleSize().width;
    auto boundaries = new DrawNode *[BOUNDARY_COUNT];

    for (int i = 0; i < BOUNDARY_COUNT; i++)
        boundaries[i] = DrawNode::create();

    auto material = PhysicsMaterial(1.0f, 0.0f, 1.0f);

    // Left
    {
        boundaries[0]->drawSolidRect(Vec2(0, 0), Vec2(15, 100), Color4F::GRAY);
        boundaries[0]->setPosition(Vec2(visibleWidth - 672, 15));

        auto physicsBody = PhysicsBody::createBox(Size(15, 100), material, Vec2(7.5, 50));
        boundaries[0]->addComponent(physicsBody);
    }
    // Middle
    {
        boundaries[1]->drawSolidRect(Vec2(0, 0), Vec2(15, 100), Color4F::GRAY);
        boundaries[1]->setPosition(Vec2(visibleWidth - 448, 15));

        auto physicsBody = PhysicsBody::createBox(Size(15, 100), material, Vec2(7.5, 50));
        boundaries[1]->addComponent(physicsBody);
    }
    // Right
    {
        boundaries[2]->drawSolidRect(Vec2(0, 0), Vec2(15, 100), Color4F::GRAY);
        boundaries[2]->setPosition(Vec2(visibleWidth - 224, 15));

        auto physicsBody = PhysicsBody::createBox(Size(15, 100), material, Vec2(7.5, 50));
        boundaries[2]->addComponent(physicsBody);
    }
    // Bottom
    {
        boundaries[3]->drawSolidRect(Vec2(0, 0), Vec2(visibleWidth, 15), Color4F::GRAY);

        auto physicsBody = PhysicsBody::createBox(Size(visibleWidth, 15), material, Vec2(visibleWidth / 2, 7.5));
        boundaries[3]->addComponent(physicsBody);
    }

    for (int i = 0; i < BOUNDARY_COUNT; i++)
    {
        auto physicsBody = boundaries[i]->getPhysicsBody();

        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(i == 3 ? BOUNDARY_CATBM : 8);
        physicsBody->setCollisionBitmask(BOUNDARY_COLBM);
        physicsBody->setContactTestBitmask(i == 3 ? BOUNDARY_COTBM : 2);
    }

    return boundaries;
}
