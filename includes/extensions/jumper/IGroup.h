#ifndef __IGROUP_H__
#define __IGROUP_H__

#include <memory>
#include <opennn/neural_network.h>

namespace Jumper
{
    class IGroup
    {
    public:
        std::shared_ptr<OpenNN::NeuralNetwork> neuralNetwork;

        virtual float getScore()
        {
            return 0;
        }
        virtual void setScore()
        {
        }
    };
} // namespace Jumper

#endif // __IGROUP_H__