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
    public:
        static std::vector<Node *> pipes;
        static int pipeCounter;
        static float timeSinceLastPipe;

        static void nextGeneration();
        static EvolutionSession<Flapper> *evolutionSession;
    };
} // namespace Flappers
#endif // __FLAPPERS_SESSION_H__
