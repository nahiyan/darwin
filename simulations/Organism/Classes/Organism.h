#ifndef __ORGANISM_H__
#define __ORGANISM_H__

#include "cocos2d.h"
#include <neural_network.h>
#include <string>
#include <memory>

class Organism
{
private:
    void setGeometry();
    void resetGeometry();
    bool foodIntersection;
    cocos2d::Label *label;
    int foodEaten;

public:
    std::shared_ptr<OpenNN::NeuralNetwork> neuralNetwork;
    // OpenNN::NeuralNetwork *;
    cocos2d::DrawNode *node;
    std::string targetedFoodName;

    Organism(const cocos2d::Vec2 &);
    ~Organism();

    void setFoodIntersection();
    void unsetFoodIntersection();
    bool getFoodIntersection();
    void eatFood();
};

#endif // __ORGANISM_H__
