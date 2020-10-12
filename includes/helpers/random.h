#ifndef _HELPERS_RANDOM_H_
#define _HELPERS_RANDOM_H_

#include <chrono>
#include <random>
#include <helpers/time.h>

namespace Darwin
{
    class RandomHelper
    {
    public:
        static double nnParameter(long long seed = TimeHelper::now())
        {
            static std::default_random_engine e(seed);
            static std::uniform_real_distribution<> dis(-1, 1);
            return dis(e);
        }
    };
} // namespace Darwin

#endif // _HELPERS_RANDOM_H_
