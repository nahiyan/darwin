#ifndef __WHEELS_CAR_H__
#define __WHEELS_CAR_H__

#include "cocos2d.h"

using namespace cocos2d;

namespace Wheels
{
    class Car
    {
    public:
        enum Direction1D
        {
            Forward,
            Left,
            Right,
            None
        };
        struct Direction2D
        {
            Direction1D y;
            Direction1D x;
        } direction2D;
        Node* node;
        Car();
        ~Car();
        void update(float);
    };
} // namespace Wheels

#endif // __WHEELS_CAR_H__