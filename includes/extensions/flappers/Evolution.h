#ifndef __FLAPPERS_EVOLUTION_H__
#define __FLAPPERS_EVOLUTION_H__

#include "Flapper.h"

namespace Flappers
{
    class Evolution
    {
    public:
        static void crossoverAndMutate(Flapper *, Flapper *, Flapper *, float);
    };
} // namespace Flappers

#endif // __FLAPPERS_EVOLUTION_H__