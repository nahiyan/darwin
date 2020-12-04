#include <extensions/flappers/Session.h>
#include <extensions/flappers/GenerationState_generated.h>
#include <extensions/flappers/Evolution.h>
#include <extensions/flappers/Pipe.h>
#include <core/CoreSession.h>

using namespace Flappers;

EvolutionSession<Flapper> *Session::evolutionSession = nullptr;
int Session::flapperQuantity = 0;
int Session::populationSize = 0;
std::vector<Node *> Session::pipes = std::vector<Node *>{};

void Session::setPopulationSize(int size)
{
    Session::populationSize = size;
    Session::flapperQuantity = size;
}

void Session::decrementFlapperQuantity()
{
    Session::flapperQuantity--;

    // log("%d", Session::flapperQuantity);

    if (Session::flapperQuantity == 0)
        Session::nextGeneration();
}

void Session::setFlapperQuantity(int quantity)
{
    Session::flapperQuantity = quantity;
}

void Session::nextGeneration()
{
    Session::flapperQuantity = Session::populationSize;
    const auto mainScene = MainScene::getInstance();

    // Score the current population
    for (auto object : Session::evolutionSession->population)
    {
        object->setScore();
        object->kill();
    }

    // Remove all the objects
    for (auto &child : mainScene->getChildren())
    {
        auto physicsBody = child->getPhysicsBody();
        if (physicsBody != nullptr && (physicsBody->getCategoryBitmask() == 1 || physicsBody->getCategoryBitmask() == 2))
        {
            if (physicsBody->getCategoryBitmask() == 1)
                Session::evolutionSession->population[child->getTag()]->kill();

            mainScene->removeChild(child);
        }
    }
    Session::pipes.clear();

    // Contructing a FlatBuffers builder
    flatbuffers::FlatBufferBuilder builder(1024);

    auto population_ = std::vector<flatbuffers::Offset<Member>>();

    for (auto object : Session::evolutionSession->population)
    {
        auto chromosomes = std::vector<double>();
        auto parameters = object->neuralNetwork->get_parameters();
        for (auto parameter : parameters)
            chromosomes.push_back(parameter);
        auto member = CreateMember(builder, builder.CreateVector<double>(chromosomes), object->getScore());
        population_.push_back(member);
    }

    auto population = builder.CreateVector(population_);

    auto state = CreateGenerationState(builder, population);
    builder.Finish(state);

    // Perform evolution
    Session::evolutionSession->evolve(Evolution::crossoverAndMutate, CoreSession::sessionId, builder.GetBufferPointer(), builder.GetSize());

    // Add nodes
    for (int i = 0; i < Session::evolutionSession->population.size(); i++)
    {
        auto object = Session::evolutionSession->population[i];
        object->reset();
        mainScene->addChild(object->node, 2, i);
    }
}
