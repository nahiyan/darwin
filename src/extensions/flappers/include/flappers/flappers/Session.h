#ifndef __FLAPPERS_SESSION_H__
#define __FLAPPERS_SESSION_H__

#include "cocos2d.h"
#include "MainScene.h"
#include "Flapper.h"
#include "core/EvolutionSession.h"

using namespace cocos2d;
using namespace std;

namespace Flappers
{
    class Session
    {
    public:
        static vector<Node *> pipes;
        static int pipeCounter;
        static float timeSinceLastPipe;
        static Label* hud;
        static Core::EvolutionSession<Flapper> *evolutionSession;
        static string modelsFilePath;

        static void nextGeneration();
    };
} // namespace Flappers
#endif // __FLAPPERS_SESSION_H__
