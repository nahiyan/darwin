#include <extensions/flappers/Listeners.h>
#include "cocos2d.h"
#include <extensions/flappers/MainScene.h>
#include <extensions/flappers/Session.h>

using namespace Flappers;

USING_NS_CC;

bool Listeners::onContactBegin(PhysicsContact &contact)
{
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    auto categoryBitmaskA = bodyA->getCategoryBitmask();
    auto categoryBitmaskB = bodyB->getCategoryBitmask();

    // Bird and obstacle
    if (categoryBitmaskA + categoryBitmaskB == 3)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
            auto node = (categoryBitmaskA == 1 ? bodyA : bodyB)->getNode();

            if (node != nullptr)
            {
                auto flapper = Session::evolutionSession->population[node->getTag()];
                flapper->kill();
            }
        });

        return true;
    }

    return false;
}

void Listeners::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event, Flapper *flapper)
{
    switch ((int)keyCode)
    {
    case 59:
        flapper->flap();
        break;
    }
}

// void Listeners::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event, Flapper *flapper)
// {
//     switch ((int)keyCode)
//     {
//     case 59:
//         flapper->flap();
//         break;
//     }
// }

void Listeners::onMouseMove(Event *event)
{
    EventMouse *e = (EventMouse *)event;

    log("x: %f, y: %f", e->getCursorX(), e->getCursorY());
}
