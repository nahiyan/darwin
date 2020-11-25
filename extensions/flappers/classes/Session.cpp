#include <extensions/flappers/Session.h>
#include <extensions/flappers/GenerationState_generated.h>
#include <extensions/flappers/Evolution.h>
#include <core/CoreSession.h>

using namespace Flappers;

EvolutionSession<Flapper> *Session::evolutionSession = nullptr;
Session::CurrentGenerationInfo Session::currentGenerationInfo = Session::CurrentGenerationInfo{0};
int Session::populationSize = 0;

void Session::setPopulationSize(int size)
{
    Session::populationSize = size;
    Session::currentGenerationInfo.flapperQuantity = size;
}

void Session::decrementFlapperQuantity()
{
    Session::currentGenerationInfo.flapperQuantity--;

    if (Session::currentGenerationInfo.flapperQuantity == 0)
        Session::nextGeneration();
}

void Session::setFlapperQuantity(int quantity)
{
    Session::currentGenerationInfo.flapperQuantity = quantity;
}

void Session::nextGeneration()
{
    // Session::currentGenerationInfo.flapperQuantity = Session::populationSize;
    // const auto mainScene = MainScene::getInstance();

    // // Score the current population
    // for (auto object : Session::evolutionSession->population)
    // {
    //     object->setScore();
    //     object->kill();
    // }

    // // Remove all the vehicles
    // auto children = mainScene->getChildren();
    // for (auto child : children)
    // {
    //     auto physicsBody = child->getPhysicsBody();
    //     if (physicsBody != nullptr && physicsBody->getCategoryBitmask() == 1)
    //     {
    //         if (physicsBody->getCategoryBitmask() == 1)
    //             Session::evolutionSession->population[child->getTag()]->kill();

    //         mainScene->removeChild(child);
    //     }
    // }

    // // Contructing a FlatBuffers builder
    // flatbuffers::FlatBufferBuilder builder(1024);

    // auto population_ = std::vector<flatbuffers::Offset<Member>>();

    // for (auto object : Session::evolutionSession->population)
    // {
    //     auto chromosomes = std::vector<double>();
    //     auto parameters = object->neuralNetwork->get_parameters();
    //     for (auto parameter : parameters)
    //         chromosomes.push_back(parameter);
    //     auto member = CreateMember(builder, builder.CreateVector<double>(chromosomes), object->getScore());
    //     population_.push_back(member);
    // }

    // auto population = builder.CreateVector(population_);

    // auto state = CreateGenerationState(builder, population);
    // builder.Finish(state);

    // // Perform evolution
    // Session::evolutionSession->evolve(Evolution::crossoverAndMutate, CoreSession::sessionId, builder.GetBufferPointer(), builder.GetSize());

    // // Add nodes
    // for (int i = 0; i < Session::evolutionSession->population.size(); i++)
    // {
    //     auto object = Session::evolutionSession->population[i];
    //     object->reset();
    //     mainScene->addChild(object->node, 2, i);
    // }
}
