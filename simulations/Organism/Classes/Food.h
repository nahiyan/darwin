#ifndef __FOOD_H__
#define __FOOD_H__

#include "cocos2d.h"

class Food
{
private:
    cocos2d::DrawNode *node;

public:
    Food(const cocos2d::Vec2 &);
    cocos2d::DrawNode *getNode();
};

#endif // __FOOD_H__
