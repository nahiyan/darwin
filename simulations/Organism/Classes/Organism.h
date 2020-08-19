#ifndef __ORGANISM_H__
#define __ORGANISM_H__

#include "cocos2d.h"

class Organism
{
private:
    cocos2d::DrawNode *node;

public:
    Organism(const cocos2d::Vec2 &);
    cocos2d::DrawNode *getNode();
};

#endif // __ORGANISM_H__
