#ifndef _EVOLUTION_SESSION_H_
#define _EVOLUTION_SESSION_H_

#include <sqlite3/sqlite3.h>
#include <core/IEvolvable.h>
#include <ctime>
#include <vector>
#include <functional>

template <class T>
class EvolutionSession
{
private:
    float populationDivision[2];
    float mutationRate;
    int generationIndex;

public:
    std::vector<T *> objectList;

    EvolutionSession(std::vector<T *> objectList)
    {
        this->objectList = objectList;
        this->mutationRate = 0.01;
        this->populationDivision[0] = 0.05;
        this->populationDivision[1] = 0.3;
        this->generationIndex = 0;
    }
    void rank()
    {
        auto sort_ = [&](T *a, T *b) {
            return a->getScore() > b->getScore();
        };

        std::sort(this->objectList.begin(), this->objectList.end(), sort_);
    }
    void evolve(std::function<void(T *, T *, T *, float)> crossoverAndMutate)
    {
        // Ranking
        this->rank();

        // Record scores of current generation
        // this->recordScores(generationStartTime);

        // Population size
        int populationSize = this->objectList.size();

        // Upper class
        int upperClassCount = round(populationDivision[0] * populationSize);
        int upperClassRange[2] = {0, upperClassCount - 1};

        // Middle class
        int middleClassCount = round(populationDivision[1] * populationSize);
        int middleClassRange[2] = {upperClassCount > 0 ? upperClassRange[1] + 1 : 0, (middleClassCount + upperClassCount) - 1};

        // Reproduction
        if ((middleClassCount + upperClassCount) > 0)
        {
            for (int i = middleClassRange[0]; i < populationSize; i++)
            {
                auto parentA = this->objectList[rand() % (middleClassRange[1] + 1)];
                auto parentB = this->objectList[rand() % (middleClassRange[1] + 1)];

                (crossoverAndMutate)(parentA, parentB, this->objectList[i], this->mutationRate);
            }
        }

        this->generationIndex++;
    }
    // void recordScores(std::time_t generationStartTime)
    // {
    //     std::vector<std::time_t> currentGenerationScores;

    //     // Allocate spaces for each chromosome
    //     for (int i = 0; i < this->objectList.size(); i++)
    //     {
    //         currentGenerationScores.push_back(0);
    //     }

    //     // Insert the chromosomes
    //     for (auto object : this->objectList)
    //     {
    //         currentGenerationScores[object->getIndex()] = object->deathTimestamp - generationStartTime;

    //         log("Score: %lu", object->deathTimestamp - generationStartTime);
    //     }
    //     log("End of scores");

    //     // Insert the generation
    //     this->scores.push_back(currentGenerationScores);
    // }
};

#endif // _EVOLUTION_SESSION_H_
