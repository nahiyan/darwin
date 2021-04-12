#ifndef __FLAPPERS_FLAPPER_H__
#define __FLAPPERS_FLAPPER_H__

#include "cocos2d.h"
#include <opennn/opennn.h>

using namespace cocos2d;

namespace Flappers
{
    class Flapper
    {
    private:
        float score;
        bool dead;

        void generateNode();

    public:
        std::shared_ptr<OpenNN::NeuralNetwork> neuralNetwork;
        Node *node;

        Flapper(std::vector<double>);
        ~Flapper();
        void update(float);
        float getScore();
        void setScore();
        void kill();
        bool isDead();
        void reset();
        void flap();
    };
} // namespace Flappers

#endif // __FLAPPERS_FLAPPER_H__
