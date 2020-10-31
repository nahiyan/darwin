#ifndef __JUMPER_Boundary_H__
#define __JUMPER_Boundary_H__

#include "cocos2d.h"

namespace Jumper
{
    class Boundary
    {
    public:
        static cocos2d::DrawNode **create(cocos2d::Size &);
    };
} // namespace Jumper

#endif // __JUMPER_Boundary_H__
