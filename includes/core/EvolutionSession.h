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
    float upperFraction;  // Elite/Unchaged
    float middleFraction; // Reproduced from best fit
    float randomFraction; // Totally randomly generated
    float mutationRate;
    int generationIndex;

public:
    std::vector<T *> population;

    EvolutionSession()
    {
        this->population = std::vector<T *>();
        this->mutationRate = 0.01;
        this->upperFraction = 0.05;
        this->middleFraction = 0.3;
        this->randomFraction = 0.2;
        this->generationIndex = 0;
    }

    EvolutionSession(float mutationRate, float upperFraction, float middleFraction, float randomFraction)
    {
        this->population = std::vector<T *>();
        this->mutationRate = mutationRate;
        this->upperFraction = upperFraction;
        this->middleFraction = middleFraction;
        this->randomFraction = randomFraction;
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

    void evolve(std::function<void(T *, T *, T *, float)> crossoverAndMutate, std::function<void(T *)> randomize, int sessionId, uint8_t *fBBufferPoint, flatbuffers::uoffset_t fBBufferSize)
    {
        // Ranking
        auto rankedPopulation = this->getRankedPopulation();

        // Save state of tested generation
        if (sessionId != 0)
        {
            this->saveState(sessionId, fBBufferPoint, fBBufferSize);

            printf("Scores: ");
            for (auto object : rankedPopulation)
                printf("%f ", object->getScore());
            printf("\n");
        }

        // Population size
        int populationSize = this->population.size();

        // Upper class
        int upperClassCount = round(upperFraction * populationSize);
        int upperClassRange[2] = {0, upperClassCount - 1};

        // Middle class
        int middleClassCount = round(middleFraction * populationSize);
        int middleClassRange[2] = {upperClassCount > 0 ? upperClassRange[1] + 1 : 0, (middleClassCount + upperClassCount) - 1};

        // Random class
        int randomMembersCount = round(middleFraction * populationSize);
        int randomMembersRange[2] = {randomMembersCount > 0 ? populationSize - randomMembersCount : 0, populationSize - 1};

        // Reproduction
        if ((middleClassCount + upperClassCount) > 0)
        {
            for (int i = middleClassRange[0]; i < populationSize - randomMembersCount; i++)
            {
                auto parentA = rankedPopulation[rand() % (middleClassRange[1] + 1)];
                auto parentB = rankedPopulation[rand() % (middleClassRange[1] + 1)];

                (crossoverAndMutate)(parentA, parentB, rankedPopulation[i], this->mutationRate);
            }
        }

        // Randomization
        if (randomMembersCount > 0)
            for (int i = randomMembersRange[0]; i < populationSize; i++)
                (randomize)(rankedPopulation[i]);

        this->generationIndex++;
    }

    void evolve(std::function<void(T *, T *, T *, float)> crossoverAndMutate, std::function<void(T *)> randomize)
    {
        this->evolve(crossoverAndMutate, randomize, 0, nullptr, 0);
    }

    void saveState(int sessionId, uint8_t *fBBufferPoint, flatbuffers::uoffset_t fBBufferSize)
    {
        Database::addGeneration(sessionId, fBBufferPoint, (int)fBBufferSize);
    }
};

#endif // _EVOLUTION_SESSION_H_
