#include <extensions/wheels/Tracks.h>

#include "cocos2d.h"
#include <vector>

#define VERTEX_LEFT_COUNT 9
#define VERTEX_RIGHT_COUNT 8

using namespace Wheels;
using namespace cocos2d;

std::vector<Node *> Tracks::create()
{
    Vec2 verticesLeft[VERTEX_LEFT_COUNT] = {
        Vec2(50, 0),
        Vec2(50, 267),
        Vec2(150, 486),
        Vec2(404, 657),
        Vec2(618, 670),
        Vec2(821, 580),
        Vec2(943, 432),
        Vec2(965, 223),
        Vec2(683, 48)};
    Vec2 verticesRight[VERTEX_RIGHT_COUNT] = {
        Vec2(250, 0),
        Vec2(250, 267),
        Vec2(350, 400),
        Vec2(500, 500),
        Vec2(650, 470),
        Vec2(750, 380),
        Vec2(780, 280),
        Vec2(630, 150)};

    auto nodes = std::vector<Node *>{};

    for (int i = 0; i < 2; i++)
    {
        auto vertices = (i == 0) ? verticesLeft : verticesRight;
        auto verticesCount = (i == 0) ? VERTEX_LEFT_COUNT : VERTEX_RIGHT_COUNT;

        auto node = DrawNode::create();
        node->drawPoly(vertices, verticesCount, false, Color4F::MAGENTA);

        auto physicsBody = PhysicsBody::createEdgeChain(vertices, verticesCount, PHYSICSBODY_MATERIAL_DEFAULT, 1.0f);
        physicsBody->setGravityEnable(false);
        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(2);
        physicsBody->setCollisionBitmask(1);   // Car
        physicsBody->setContactTestBitmask(1); // Car
        node->addComponent(physicsBody);
        nodes.push_back(node);
    }

    return nodes;
}