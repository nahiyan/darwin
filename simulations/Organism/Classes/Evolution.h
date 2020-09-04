#ifndef Evolution_h
#define Evolution_h

#include <cocos2d.h>
#include <vector>
#include "Organism.h"

class Evolution
{
public:
    static void nextGeneration(std::vector<Organism *>);
};

#endif /* Evolution_hpp */
