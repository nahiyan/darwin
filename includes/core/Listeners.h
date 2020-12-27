#ifndef __CORE_LISTENERS_H__
#define __CORE_LISTENERS_H__

#include "cocos2d.h"
#include "CoreSession.h"
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
                if (CoreSession::hudSelection == CoreSession::Speed)
                    CoreSession::hudSelection = CoreSession::MutationRate;
                break;
            case 142: // S
                if (CoreSession::hudSelection == CoreSession::MutationRate)
                    CoreSession::hudSelection = CoreSession::Speed;
                break;
            case 124: // A
                if (CoreSession::hudSelection == CoreSession::Speed)
                    CoreSession::speed = CoreSession::speed <= 0.1 ? 0 : CoreSession::speed - 0.1;
                else
                    eSession->setMutationRate(eSession->getMutationRate() <= 0.01 ? 0 : eSession->getMutationRate() - 0.01);
                break;

            case 127: // D
                if (CoreSession::hudSelection == CoreSession::Speed)
                    CoreSession::speed += 0.1;
                else
                    eSession->setMutationRate(eSession->getMutationRate() >= 1 ? 1 : eSession->getMutationRate() + 0.01);
                break;
            }

            scene->setSpeed(CoreSession::speed);
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
