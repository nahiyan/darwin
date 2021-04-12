#ifndef __CORE_LISTENERS_H__
#define __CORE_LISTENERS_H__

#include "cocos2d.h"
#include "Session.h"
#include "HUD.h"

using namespace cocos2d;

namespace Core
{
    template <class T1, class T2>
    class Listeners
    {
    public:
        static void onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event, Label *hudNode, EvolutionSession<T1> *eSession, T2 *scene)
        {
            switch ((int)keyCode)
            {
            case 146: // W
                if (Core::Session::hudSelection == Core::Session::Speed)
                    Core::Session::hudSelection = Core::Session::MutationRate;
                break;
            case 142: // S
                if (Core::Session::hudSelection == Core::Session::MutationRate)
                    Core::Session::hudSelection = Core::Session::Speed;
                break;
            case 124: // A
                if (Core::Session::hudSelection == Core::Session::Speed)
                    Core::Session::speed = Core::Session::speed <= 0.1 ? 0 : Core::Session::speed - 0.1;
                else
                    eSession->setMutationRate(eSession->getMutationRate() <= 0.01 ? 0 : eSession->getMutationRate() - 0.01);
                break;

            case 127: // D
                if (Core::Session::hudSelection == Core::Session::Speed)
                    Core::Session::speed += 0.1;
                else
                    eSession->setMutationRate(eSession->getMutationRate() >= 1 ? 1 : eSession->getMutationRate() + 0.01);
                break;
            }

            scene->setSpeed(Core::Session::speed);
            HUD::update(hudNode, eSession->getMutationRate());
        }

        static void onMouseMove(Event *event)
        {
            EventMouse *e = (EventMouse *)event;

            log("x: %f, y: %f", e->getCursorX(), e->getCursorY());
        }
    };
} // namespace Core
#endif // __CORE_LISTENERS_H__
