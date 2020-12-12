#include "cocos2d.h"
#include <helpers/random.h>
#include <helpers/time.h>

USING_NS_CC;

template <class T>
class EvolutionCommon
{
public:
    static void crossoverAndMutate(T *parentA, T *parentB, T *offspring, float mutationRate)
    {
        auto parentAParameters = parentA->neuralNetwork->get_parameters();
        auto parentBParameters = parentB->neuralNetwork->get_parameters();

        OpenNN::Vector<double> newParameters(parentAParameters.size());

        for (int i = 0; i < parentAParameters.size(); i++)
        {
            // Crossover
            newParameters[i] = random<int>(0, 1) == 0 ? parentAParameters[i] : parentBParameters[i];

            // Mutation
            newParameters[i] += random<double>(-1, 1) * mutationRate * newParameters[i];
        }

        offspring->neuralNetwork->set_parameters(newParameters);
    }

    static void randomize(T *member)
    {
        std::vector<double> parameters;
        for (int i = 0; i < member->neuralNetwork->get_parameters_number(); i++)
            parameters.push_back(Darwin::RandomHelper::nnParameter(TimeHelper::now() * i));
        member->neuralNetwork->set_parameters(parameters);
    }
};
