#ifndef __JUMPER_Jumper_H__
#define __JUMPER_Jumper_H__

#include "cocos2d.h"
#include <opennn/neural_network.h>
#include <string>
#include <memory>

namespace Jumper
{
    class Jumper
    {
    private:
        void setGeometry();
        float rayTraceFraction;
        int index;
        float score;
        int jumps = 0;

    public:
        cocos2d::Sprite *node;
        bool isDead;
        long long deathTimestamp;
        std::shared_ptr<OpenNN::NeuralNetwork> neuralNetwork;

        Jumper(const cocos2d::Vec2 &, int, std::vector<double>);
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
} // namespace Jumper

#endif // __JUMPER_Jumper_H__
