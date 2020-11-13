#ifndef __WHEELS_LISTENERS_H__
#define __WHEELS_LISTENERS_H__

#include "cocos2d.h"
#include "Car.h"

using namespace cocos2d;

namespace Wheels
{
    class Listeners
    {
    public:
        static bool onContactBegin(PhysicsContact &);
        static void onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event, Car *);
        static void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event, Car *);
        static void onMouseMove(Event *event, Scene *scene);
    };
} // namespace Wheels
#endif // __WHEELS_LISTENERS_H__
