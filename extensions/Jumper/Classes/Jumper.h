#ifndef __Jumper_H__
#define __Jumper_H__

#include "cocos2d.h"
#include <opennn/neural_network.h>
#include <string>
#include <memory>

class Jumper
{
private:
    void setGeometry();
    float rayTraceFraction;
    long long lastJumpTimestamp;
    int index;
    float score;
    int jumps = 0;

public:
    cocos2d::Sprite *node;
    bool isDead;
    long long deathTimestamp;
    std::shared_ptr<OpenNN::NeuralNetwork> neuralNetwork;

    Jumper(const cocos2d::Vec2 &, int);
    ~Jumper();

    void setRayTraceFraction(float);
    void update(float);
    void prepareForNewGeneration();
    void generateNode();
    int &getIndex();
    float getScore();
    void setScore(long long, long long, int);
    void kill();
};

#endif // __Jumper_H__
