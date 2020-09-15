#ifndef __Jumper_H__
#define __Jumper_H__

#include "cocos2d.h"
#include <neural_network.h>
#include <string>
#include <memory>
#include <ctime>

class Jumper
{
private:
    void setGeometry();
    float rayTraceFraction;
    std::time_t lastJumpTimestamp;

public:
    cocos2d::Sprite *node;
    bool isDead;
    std::time_t deathTimestamp;
    std::shared_ptr<OpenNN::NeuralNetwork> neuralNetwork;

    Jumper(const cocos2d::Vec2 &);
    ~Jumper();

    void setRayTraceFraction(float);
    void update(float);
    void prepareForNewGeneration();
    void setDead();
    void generateNode();
};

#endif // __Jumper_H__
