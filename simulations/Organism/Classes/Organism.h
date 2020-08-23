#ifndef __ORGANISM_H__
#define __ORGANISM_H__

#include "cocos2d.h"
#include <neural_network.h>

class Organism
{
private:
    void setGeometry();
    void resetGeometry();

public:
    enum Motive
    {
        ConsumeFood,
        ScoutForFood
    };
    bool foodIntersection;
    OpenNN::NeuralNetwork *neuralNetwork;
    cocos2d::DrawNode *node;

    Organism(const cocos2d::Vec2 &);
    ~Organism();
    cocos2d::DrawNode *getNode();
};

#endif // __ORGANISM_H__
