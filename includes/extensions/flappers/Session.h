#ifndef __FLAPPERS_SESSION_H__
#define __FLAPPERS_SESSION_H__

#include "cocos2d.h"
#include "MainScene.h"
#include "Flapper.h"
#include <core/EvolutionSession.h>

using namespace cocos2d;

namespace Flappers
{
    class Session
    {
    private:
        static int flapperQuantity;
        static int populationSize;

    public:
        static std::vector<Node *> pipes;

        static void nextGeneration();
        static EvolutionSession<Flapper> *evolutionSession;
        static void decrementFlapperQuantity();
        static void setFlapperQuantity(int);
        static void setPopulationSize(int);
    };
} // namespace Flappers
#endif // __FLAPPERS_SESSION_H__
