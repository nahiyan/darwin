#ifndef __CORE_DEBUG_H__
#define __CORE_DEBUG_H__

#include "cocos2d.h"

USING_NS_CC;

class Debug
{
public:
    static Vec2 log(Vec2 vec)
    {
        cocos2d::log("Vec2(%f, %f)", vec.x, vec.y);

        return vec;
    }
};

#endif // __CORE_DEBUG_H__