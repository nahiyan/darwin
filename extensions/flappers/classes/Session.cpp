#include <extensions/flappers/Session.h>
#include <core/GenerationState_generated.h>
#include <core/EvolutionCommon.h>
#include <extensions/flappers/Pipe.h>
#include <core/Session.h>
#include <core/HUD.h>

using namespace Flappers;

Core::EvolutionSession<Flapper> *Session::evolutionSession = nullptr;
std::vector<Node *> Session::pipes = std::vector<Node *>{};
int Session::pipeCounter = 0;
float Session::timeSinceLastPipe = 0;
Label *Session::hud = nullptr;

void Session::nextGeneration()
{
    // Increment the generation index
    Core::Session::generationIndex++;
    Core::HUD::update(Session::hud, Session::evolutionSession->getMutationRate());

    Session::pipeCounter = 0;
    const auto &mainScene = MainScene::getInstance();

    // Score the current population, killing any surviving member
    for (auto &member : Session::evolutionSession->population)
        member->setScore();

    for (auto &pipe : Session::pipes)
        mainScene->removeChild(pipe);

    Session::pipes.clear();

    // Contructing a FlatBuffers builder
    flatbuffers::FlatBufferBuilder builder(1024);

    auto population_ = std::vector<flatbuffers::Offset<Core::Member>>();

    for (auto object : Session::evolutionSession->population)
    {
        auto chromosomes = std::vector<double>();
        auto parameters = object->neuralNetwork->get_parameters();
        for (auto parameter : parameters)
            chromosomes.push_back(parameter);
        auto member = Core::CreateMember(builder, builder.CreateVector<double>(chromosomes), object->getScore());
        population_.push_back(member);
    }

    auto population = builder.CreateVector(population_);

    auto state = CreateGenerationState(builder, population);
    builder.Finish(state);

    // Perform evolution
    Session::evolutionSession->evolve(Core::EvolutionCommon<Flapper>::crossoverAndMutate, Core::EvolutionCommon<Flapper>::randomize, Core::Session::sessionId, builder.GetBufferPointer(), builder.GetSize());

    // Add nodes
    for (int i = 0; i < Session::evolutionSession->population.size(); i++)
    {
        auto &object = Session::evolutionSession->population[i];
        object->reset();
        mainScene->addChild(object->node, 0, i);
    }

    mainScene->addPipe();
}
