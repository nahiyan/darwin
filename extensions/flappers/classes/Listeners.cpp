#include <extensions/flappers/Listeners.h>
#include "cocos2d.h"
#include <extensions/flappers/MainScene.h>
#include <extensions/flappers/Session.h>

using namespace Flappers;

USING_NS_CC;

// bool Listeners::onContactBegin(PhysicsContact &contact)
// {
//     auto bodyA = contact.getShapeA()->getBody();
//     auto bodyB = contact.getShapeB()->getBody();

//     auto categoryBitmaskA = bodyA->getCategoryBitmask();
//     auto categoryBitmaskB = bodyB->getCategoryBitmask();

//     // Vehicles and tracks
//     if (categoryBitmaskA + categoryBitmaskB == 3)
//     {
//         Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
//             auto node = (categoryBitmaskA == 1 ? bodyA : bodyB)->getNode();

//             if (node != nullptr)
//             {
//                 auto car = Session::evolutionSession->population[node->getTag()];
//                 car->kill();
//             }
//         });

//         return true;
//     }

//     return false;
// }

// void Listeners::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event, Car *car)
// {
//     switch ((int)keyCode)
//     {
//     case 124:
//         car->direction2D.x = Car::Direction1D::Left;
//         break;
//     case 127:
//         car->direction2D.x = Car::Direction1D::Right;
//         break;
//     case 146:
//         car->direction2D.y = Car::Direction1D::Forward;
//         break;
//     }
// }

// void Listeners::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event, Car *car)
// {
//     switch ((int)keyCode)
//     {
//     case 124:
//         car->direction2D.x = Car::Direction1D::None;
//         break;
//     case 127:
//         car->direction2D.x = Car::Direction1D::None;
//         break;
//     case 146:
//         car->direction2D.y = Car::Direction1D::None;
//         break;
//     }
// }

void Listeners::onMouseMove(Event *event)
{
    EventMouse *e = (EventMouse *)event;

    log("x: %f, y: %f", e->getCursorX(), e->getCursorY());
}
