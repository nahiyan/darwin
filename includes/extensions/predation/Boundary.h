#ifndef __PREDATION_Boundary_H__
#define __PREDATION_Boundary_H__

#include "cocos2d.h"

namespace Predation
{
    class Boundary
    {
    public:
        static cocos2d::DrawNode **create(cocos2d::Size &);
    };
} // namespace Predation

#endif // __PREDATION_Boundary_H__
