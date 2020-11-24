#ifndef __JUMPER_EVOLUTION_H__
#define __JUMPER_EVOLUTION_H__

#include "Car.h"

namespace Wheels
{
    class Evolution
    {
    public:
        static void crossoverAndMutate(Car *, Car *, Car *, float);
    };
} // namespace Wheels

#endif // __JUMPER_EVOLUTION_H__