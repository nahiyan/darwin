#ifndef __ORGANISM_H__
#define __ORGANISM_H__

#include "cocos2d.h"
#include <neural_network.h>

class Organism
{
private:
    cocos2d::DrawNode *node;
    bool foodIntersection;
    OpenNN::NeuralNetwork *neuralNetwork;

    void setGeometry();
    void resetGeometry();

public:
    Organism(const cocos2d::Vec2 &);
    ~Organism();
    cocos2d::DrawNode *getNode();
    void setFoodIntersection();
    void unsetFoodIntersection();
};

#endif // __ORGANISM_H__
