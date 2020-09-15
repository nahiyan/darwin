#include "Evolution.h"
#include "Jumper.h"
#include <cocos2d.h>
#include <algorithm>
#include <cmath>
#include <vector>

USING_NS_CC;

Evolution::Evolution(std::vector<Jumper *> objectList)
{
    this->objectList = objectList;
    this->mutationRate = 0.01;
    this->populationDivision[0] = 0.05;
    this->populationDivision[1] = 0.3;
}

void Evolution::evolve()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    for (auto jumper : this->objectList)
    {
        if (!jumper->isDead)
            jumper->deathTimestamp = std::time(nullptr);
    }

    // Ranking
    this->rank();

    // Population size
    int populationSize = this->objectList.size();

    // Elitism
    int upperClassCount = round(populationDivision[0] * populationSize);
    int upperClassRange[2] = {0, upperClassCount - 1};

    // Best fit
    int middleClassCount = round(populationDivision[1] * populationSize);
    int middleClassRange[2] = {upperClassCount > 0 ? upperClassRange[1] + 1 : 0, (middleClassCount + upperClassCount) - 1};

    // Reproduction
    if (middleClassCount > 0)
    {
        for (int i = middleClassRange[0]; i < populationSize; i++)
        {
            auto parentAParameters = this->objectList[random<int>(middleClassRange[0], middleClassRange[1])]->neuralNetwork->get_parameters();
            auto parentBParameters = this->objectList[random<int>(middleClassRange[0], middleClassRange[1])]->neuralNetwork->get_parameters();

            OpenNN::Vector<double> newParameters(parentAParameters.size());
            for (int i = 0; i < parentAParameters.size(); i++)
            {
                // Crossover
                newParameters[i] = (random<int>(0, 1) == 0 ? parentAParameters[i] : parentBParameters[i]);

                // Mutation
                newParameters[i] += random<double>(-1, 1) * this->mutationRate * newParameters[i];
            }

            this->objectList[i]->neuralNetwork->set_parameters(newParameters);
        }
    }
}

void Evolution::rank()
{
    auto sort_ = [](Jumper *a, Jumper *b) {
        return a->deathTimestamp > b->deathTimestamp;
    };

    std::sort(this->objectList.begin(), this->objectList.end(), sort_);
}
