#include <extensions/wheels/Listeners.h>
#include "cocos2d.h"
#include <extensions/wheels/MainScene.h>
#include <extensions/wheels/Session.h>

using namespace Wheels;

USING_NS_CC;

bool Listeners::onContactBegin(PhysicsContact &contact)
{
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    auto categoryBitmaskA = bodyA->getCategoryBitmask();
    auto categoryBitmaskB = bodyB->getCategoryBitmask();

    // Vehicles and tracks
    if (categoryBitmaskA + categoryBitmaskB == 3)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
            auto node = (categoryBitmaskA == 1 ? bodyA : bodyB)->getNode();

            if (node != nullptr)
            {
                auto car = Session::evolutionSession->population[node->getTag()];
                car->kill();
            }
        });

        return true;
    }

    return false;
}

void Listeners::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event, Car *car)
{
    switch ((int)keyCode)
    {
    case 124:
        car->direction2D.x = Car::Direction1D::Left;
        break;
    case 127:
        car->direction2D.x = Car::Direction1D::Right;
        break;
    case 146:
        car->direction2D.y = Car::Direction1D::Forward;
        break;
    }
}

void Listeners::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event, Car *car)
{
    switch ((int)keyCode)
    {
    case 124:
        car->direction2D.x = Car::Direction1D::None;
        break;
    case 127:
        car->direction2D.x = Car::Direction1D::None;
        break;
    case 146:
        car->direction2D.y = Car::Direction1D::None;
        break;
    }
}

void Listeners::onMouseMove(Event *event)
{
    EventMouse *e = (EventMouse *)event;
    // log("Mouse: (%f, %f)", e->getCursorX(), e->getCursorY());
    auto rayCastCB = [&](PhysicsWorld &world, const PhysicsShape &shape, void *organism) -> bool {
        // log("%f", 1 - info.fraction);
        if (shape.getBody()->getCategoryBitmask() == 2)
        {
            log("Track %f %f", e->getCursorX(), e->getCursorY());
            // ((Organism *)organism)->setFoodIntersection();

            return false;
        }

        return true;
    };

    MainScene::getInstance()->getPhysicsWorld()->queryPoint(rayCastCB, Vec2(e->getCursorX(), e->getCursorY()), nullptr);
}
