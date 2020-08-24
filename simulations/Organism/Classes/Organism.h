#ifndef __ORGANISM_H__
#define __ORGANISM_H__

#include "cocos2d.h"
#include <neural_network.h>
#include <string>

class Organism
{
private:
    void setGeometry();
    void resetGeometry();
    bool foodIntersection;

public:
    OpenNN::NeuralNetwork *neuralNetwork;
    cocos2d::DrawNode *node;
    std::string targetedFoodName;

    Organism(const cocos2d::Vec2 &);
    ~Organism();

    void setFoodIntersection();
    void unsetFoodIntersection();
    bool getFoodIntersection();
};

#endif // __ORGANISM_H__
