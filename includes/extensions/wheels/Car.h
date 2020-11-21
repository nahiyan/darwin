#ifndef __WHEELS_CAR_H__
#define __WHEELS_CAR_H__

#include "cocos2d.h"
#include <opennn/opennn.h>

using namespace cocos2d;

namespace Wheels
{
    class Car
    {
    public:
        enum Direction1D
        {
            Forward,
            Left,
            Right,
            None
        };

        struct Direction2D
        {
            Direction1D y;
            Direction1D x;
        } direction2D;

    private:
        float distance;
        float steeringAngle;
        float sensors[3];

        struct SensorInfo
        {
            Car *car;
            Direction1D direction;
        } sensorInfo;

    public:
        std::shared_ptr<OpenNN::NeuralNetwork> neuralNetwork;
        Node *node;

        Car(std::vector<double>);
        ~Car();
        void update(float);
        float getScore();
        void setScore();
        void setSensor(Direction1D, float);
    };
} // namespace Wheels

#endif // __WHEELS_CAR_H__