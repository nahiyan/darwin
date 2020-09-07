#include "Evolution.h"
#include "Organism.h"
#include <cocos2d.h>
#include <algorithm>
#include <cmath>
#include <vector>

USING_NS_CC;

Evolution::Evolution(vector<Organism *> *organismList)
{
    this->organismList = organismList;
}

void Evolution::nextGeneration()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Reset the position of the organisms
    for (auto organism : *this->organismList)
        organism->node->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    // Ranking
    this->rank();

    // Population size
    int populationSize = this->organismList->size();

    // Elitism
    int upperClassCount = round(0.05 * populationSize);
    int upperClassRange[2] = {0, upperClassCount - 1};

    // Best fit
    int middleClassCount = round(0.3 * populationSize);
    int middleClassRange[2] = {upperClassCount > 0 ? upperClassRange[1] + 1 : 0, (middleClassCount + upperClassCount) - 1};

    // Reproduction
    if (middleClassCount > 0)
    {
        for (int i = middleClassRange[0]; i < populationSize; i++)
        {
            auto parentAParameters = (*this->organismList)[random<int>(middleClassRange[0], middleClassRange[1])]->neuralNetwork->get_parameters();
            auto parentBParameters = (*this->organismList)[random<int>(middleClassRange[0], middleClassRange[1])]->neuralNetwork->get_parameters();

            OpenNN::Vector<double> newParameters(parentAParameters.size());
            for (int i = 0; i < parentAParameters.size(); i++)
            {
                // Crossover and mutation
                newParameters[i] = (random<int>(0, 1) == 0 ? parentAParameters[i] : parentBParameters[i]) + ((random<int>(0, 1) == 0 ? 1 : -1) * random<double>(0, .05));
            }

            (*this->organismList)[i]->neuralNetwork->set_parameters(newParameters);
        }
    }
}

void Evolution::rank()
{
    auto sortOrganism = [](Organism *a, Organism *b) {
        return a->getFoodEaten() > b->getFoodEaten();
    };

    std::sort(this->organismList->begin(), this->organismList->end(), sortOrganism);
}
