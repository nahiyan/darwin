#ifndef __JUMPER_Obstacle_H__
#define __JUMPER_Obstacle_H__

#include "cocos2d.h"

namespace Jumper
{
    class Obstacle
    {
    public:
        static cocos2d::Sprite *create(const cocos2d::Vec2 &);
    };
} // namespace Jumper

#endif // __JUMPER_Obstacle_H__
