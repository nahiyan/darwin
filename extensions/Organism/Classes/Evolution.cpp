#include "Evolution.h"
#include "Organism.h"
#include <cocos2d.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>

USING_NS_CC;

Evolution::Evolution(std::map<int, Organism *> *organismMap)
{
    this->organismMap = organismMap;
    this->mutationRate = 0.01;
    this->populationDivision[0] = 0.05;
    this->populationDivision[1] = 0.3;
}

void Evolution::nextGeneration()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Reset the position of the organisms
    for (auto organism : *this->organismMap)
        organism.second->node->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    // Ranking
    auto rankedPopulation = this->getRanked();

    // Population size
    int populationSize = this->organismMap->size();

    // Elitism
    int upperClassCount = round(populationDivision[0] * populationSize);
    int upperClassRange[2] = {0, upperClassCount - 1};

    // Best fit
    int middleClassCount = round(populationDivision[1] * populationSize);
    int middleClassRange[2] = {upperClassCount > 0 ? upperClassRange[1] + 1 : 0, (middleClassCount + upperClassCount) - 1};

    std::cout << "Middle class scores: ";
    for (int i = middleClassRange[0]; i <= middleClassRange[1]; i++)
    {
        std::cout << to_string(rankedPopulation[i]->getFoodEaten()) << ", ";
    }
    std::cout << "\n";

    // Reproduction
    if (middleClassCount > 0)
    {
        for (int i = middleClassRange[0]; i < populationSize; i++)
        {
            auto parentAParameters = rankedPopulation[random<int>(middleClassRange[0], middleClassRange[1])]->neuralNetwork->get_parameters();
            auto parentBParameters = rankedPopulation[random<int>(middleClassRange[0], middleClassRange[1])]->neuralNetwork->get_parameters();

            OpenNN::Vector<double> newParameters(parentAParameters.size());
            for (int i = 0; i < parentAParameters.size(); i++)
            {
                // Crossover and mutation
                newParameters[i] = random<int>(0, 1) == 0 ? parentAParameters[i] : parentBParameters[i];
                newParameters[i] += random<double>(-1, 1) * this->mutationRate * newParameters[i];
            }

            rankedPopulation[i]->neuralNetwork->set_parameters(newParameters);
        }
    }
}

std::vector<Organism *> Evolution::getRanked()
{
    auto sortOrganism = [](Organism *a, Organism *b) {
        return a->getFoodEaten() > b->getFoodEaten();
    };

    std::vector<Organism *> rankedOrganisms;
    for (auto pair : *this->organismMap)
    {
        rankedOrganisms.push_back(pair.second);
    }

    std::sort(rankedOrganisms.begin(), rankedOrganisms.end(), sortOrganism);

    return rankedOrganisms;
}
