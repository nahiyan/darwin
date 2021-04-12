#ifndef __JUMPER_Jumper_H__
#define __JUMPER_Jumper_H__

#include "cocos2d.h"
#include <string>
#include <memory>
#include "IGroup.h"

namespace Jumper
{
    class Jumper
    {
    private:
        void setGeometry();
        float rayTraceFraction;
        int jumps;

    public:
        cocos2d::Sprite *node;
        IGroup *group;
        bool isDead;

        Jumper();
        ~Jumper();

        void setRayTraceFraction(float);
        void update(float);
        void generateNode();
        int getJumps();
        void reset();
    };
} // namespace Jumper

#endif // __JUMPER_Jumper_H__
