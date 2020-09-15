#ifndef Evolution_h
#define Evolution_h

#include <cocos2d.h>
#include <vector>
#include "Jumper.h"

class Evolution
{
private:
    std::vector<Jumper *> objectList;
    float mutationRate;
    float populationDivision[2];

public:
    Evolution(std::vector<Jumper *>);
    void evolve();
    void rank();
};

#endif /* Evolution_hpp */
