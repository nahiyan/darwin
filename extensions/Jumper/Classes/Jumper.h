#ifndef __Jumper_H__
#define __Jumper_H__

#include "cocos2d.h"
#include <opennn/neural_network.h>
#include <string>
#include <memory>
#include <ctime>

class Jumper
{
private:
    void setGeometry();
    float rayTraceFraction;
    std::time_t lastJumpTimestamp;
    int index;

public:
    cocos2d::Sprite *node;
    bool isDead;
    std::time_t deathTimestamp;
    std::time_t generationStartTimestamp;
    std::shared_ptr<OpenNN::NeuralNetwork> neuralNetwork;

    Jumper(const cocos2d::Vec2 &, int);
    ~Jumper();

    void setRayTraceFraction(float);
    void update(float);
    void prepareForNewGeneration();
    void generateNode();
    int &getIndex();
    float getScore();
    void kill();
};

#endif // __Jumper_H__
