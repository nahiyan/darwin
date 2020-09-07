#ifndef Evolution_h
#define Evolution_h

#include <cocos2d.h>
#include <vector>
#include "Organism.h"

class Evolution
{
private:
    std::vector<Organism *> *organismList;

public:
    Evolution(std::vector<Organism *> *);
    void nextGeneration();
    void rank();
};

#endif /* Evolution_hpp */
