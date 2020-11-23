#ifndef __WHEELS_SESSION_H__
#define __WHEELS_SESSION_H__

#include "cocos2d.h"
#include "MainScene.h"
#include "Car.h"
#include <core/EvolutionSession.h>

using namespace cocos2d;

namespace Wheels
{
    class Session
    {
    private:
        struct CurrentGenerationInfo
        {
            int carQuantity;
        };
        static CurrentGenerationInfo currentGenerationInfo;

    public:
        static void nextGeneration();
        static EvolutionSession<Car> *evolutionSession;
        static void decrementCarQuantity();
        static void setCarQuantity(int);
    };
} // namespace Wheels
#endif // __WHEELS_SESSION_H__