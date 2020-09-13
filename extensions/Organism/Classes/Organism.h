#ifndef __ORGANISM_H__
#define __ORGANISM_H__

#include "cocos2d.h"
#include <neural_network.h>
#include <string>
#include <memory>
#include <ctime>

class Organism
{
private:
    void setGeometry();
    void resetGeometry();
    bool foodIntersection;
    cocos2d::Label *label;
    int foodEaten;
    std::time_t lastVelocityChangeTimestamp;
    std::time_t lastSuckInTimestamp;
    bool suckingIn;

public:
    std::shared_ptr<OpenNN::NeuralNetwork> neuralNetwork;
    // OpenNN::NeuralNetwork *;
    cocos2d::DrawNode *node;

    Organism(const cocos2d::Vec2 &);
    ~Organism();

    void setFoodIntersection();
    void unsetFoodIntersection();
    bool getFoodIntersection();
    int getFoodEaten();
    void eatFood();
    void update(float);
    void reset();
    bool isSuckingIn();
};

#endif // __ORGANISM_H__
