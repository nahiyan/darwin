#ifndef __FLAPPERS_LISTENERS_H__
#define __FLAPPERS_LISTENERS_H__

#include "cocos2d.h"
#include "MainScene.h"

using namespace cocos2d;

namespace Flappers
{
    class Listeners
    {
    public:
        // static bool onContactBegin(PhysicsContact &);
        // static void onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event, Car *);
        // static void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event, Car *);
        static void onMouseMove(Event *event);
    };
} // namespace Flappers
#endif // __FLAPPERS_LISTENERS_H__
