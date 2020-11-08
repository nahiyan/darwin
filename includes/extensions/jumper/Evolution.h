#ifndef __JUMPER_EVOLUTION_H__
#define __JUMPER_EVOLUTION_H__

#include "Jumper.h"
#include "JumperGroup.h"

namespace Jumper
{
    class Evolution
    {
    public:
        static void crossoverAndMutate(JumperGroup *, JumperGroup *, JumperGroup *, float);
    };
} // namespace Jumper

#endif // __JUMPER_EVOLUTION_H__