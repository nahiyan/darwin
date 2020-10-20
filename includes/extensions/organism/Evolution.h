#ifndef Evolution_h
#define Evolution_h

#include <cocos2d.h>
#include <vector>
#include "Organism.h"

class Evolution
{
private:
    std::map<int, Organism *> *organismMap;
    float mutationRate;
    float populationDivision[2];

public:
    Evolution(std::map<int, Organism *> *);
    void nextGeneration();
    std::vector<Organism *> getRanked();
};

#endif /* Evolution_hpp */
