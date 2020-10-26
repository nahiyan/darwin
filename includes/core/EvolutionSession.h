#ifndef _EVOLUTION_SESSION_H_
#define _EVOLUTION_SESSION_H_

#include <sqlite3/sqlite3.h>
#include <vector>
#include <functional>
#include <flatbuffers/flatbuffers.h>
#include <core/Database.h>

template <class T>
class EvolutionSession
{
private:
    float populationDivision[2];
    float mutationRate;
    int generationIndex;

public:
    std::vector<T *> population;

    EvolutionSession()
    {
        this->population = std::vector<T *>();
        this->mutationRate = 0.01;
        this->populationDivision[0] = 0.05;
        this->populationDivision[1] = 0.3;
        this->generationIndex = 0;
    }

    inline std::vector<T *> getRankedPopulation()
    {
        std::vector<T *> populationRanked;

        for (auto member : this->population)
            populationRanked.push_back(member);

        auto sort_ = [&](T *a, T *b) -> bool {
            return a->getScore() > b->getScore();
        };

        std::sort(populationRanked.begin(), populationRanked.end(), sort_);

        return populationRanked;
    }

    void evolve(std::function<void(T *, T *, T *, float)> crossoverAndMutate, int sessionId, uint8_t *fBBufferPoint, flatbuffers::uoffset_t fBBufferSize)
    {
        // Ranking
        auto rankedPopulation = this->getRankedPopulation();

        // Save state of tested generation
        if (sessionId != 0)
            this->saveState(sessionId, fBBufferPoint, fBBufferSize);

        printf("Scores: ");
        for (auto object : this->population)
        {
            printf("%f ", object->getScore());
        }
        printf("\n");

        // Population size
        int populationSize = this->population.size();

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
                auto parentA = rankedPopulation[rand() % (middleClassRange[1] + 1)];
                auto parentB = rankedPopulation[rand() % (middleClassRange[1] + 1)];

                (crossoverAndMutate)(parentA, parentB, rankedPopulation[i], this->mutationRate);
            }
        }

        this->generationIndex++;
    }

    void evolve(std::function<void(T *, T *, T *, float)> crossoverAndMutate)
    {
        this->evolve(crossoverAndMutate, 0, nullptr, 0);
    }

    void saveState(int sessionId, uint8_t *fBBufferPoint, flatbuffers::uoffset_t fBBufferSize)
    {
        Database::addGeneration(sessionId, fBBufferPoint, (int)fBBufferSize);
    }
};

#endif // _EVOLUTION_SESSION_H_
