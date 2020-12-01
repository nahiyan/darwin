#include <extensions/flappers/Evolution.h>
#include "cocos2d.h"

using namespace Flappers;
USING_NS_CC;

// Crossover and mutation function
void Evolution::crossoverAndMutate(Flapper *parentA, Flapper *parentB, Flapper *offspring, float mutationRate)
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