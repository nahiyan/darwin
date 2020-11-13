#include <extensions/wheels/Listeners.h>
#include "cocos2d.h"

using namespace Wheels;

USING_NS_CC;

bool Listeners::onContactBegin(PhysicsContact &contact)
{
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    // TO DO: BUILD CONTACT LISTENER

    return true;
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

void Listeners::onMouseMove(Event *event, Scene *scene)
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

    scene->getPhysicsWorld()->queryPoint(rayCastCB, Vec2(e->getCursorX(), e->getCursorY()), nullptr);
}
