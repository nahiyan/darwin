#include "Session.h"
#include "core/EvolutionCommon.h"
#include "Pipe.h"
#include "core/Session.h"
#include "core/HUD.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "persistent-models/persistent-models.h"

using namespace Flappers;
using namespace std;
using namespace rapidjson;

Core::EvolutionSession<Flapper> *Session::evolutionSession = nullptr;
vector<Node *> Session::pipes = vector<Node *>{};
int Session::pipeCounter = 0;
float Session::timeSinceLastPipe = 0;
Label *Session::hud = nullptr;
string Session::modelsFilePath = "";

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

    // Saving models
    if (modelsFilePath.size() > 0)
    {
        for (auto member : Session::evolutionSession->population)
        {
            StringBuffer s;
            Writer<StringBuffer> writer(s);
            writer.StartObject();
            writer.Key("genome");
            writer.StartArray();
            auto parameters = member->neuralNetwork->get_parameters();
            for (auto parameter : parameters)
                writer.Double(parameter);
            writer.EndArray();
            writer.EndObject();

            string definition;
            definition.append(s.GetString());

            pm_add_stage(definition.c_str(), member->getScore());
        }
        pm_commit(10);
        pm_save_file(modelsFilePath.c_str());
    }

    // Perform evolution
    Session::evolutionSession->evolve(Core::EvolutionCommon<Flapper>::crossoverAndMutate, Core::EvolutionCommon<Flapper>::randomize);

    // Add nodes
    for (int i = 0; i < Session::evolutionSession->population.size(); i++)
    {
        auto &object = Session::evolutionSession->population[i];
        object->reset();
        mainScene->addChild(object->node, 0, i);
    }

    mainScene->addPipe();
}
