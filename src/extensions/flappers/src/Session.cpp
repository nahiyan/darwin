#include "Session.h"
#include "core/EvolutionCommon.h"
#include "Pipe.h"
#include "core/Session.h"
#include "core/HUD.h"
#include "persistent_models.h"

using namespace Flappers;
using namespace std;

Core::EvolutionSession<Flapper> *Session::evolutionSession = nullptr;
vector<Node *> Session::pipes = vector<Node *>{};
int Session::pipeCounter = 0;
float Session::timeSinceLastPipe = 0;
Label *Session::hud = nullptr;
string Session::modelsFilePath = "";
PersistentModels *Session::pm = nullptr;

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

    for (auto &flapper : Session::evolutionSession->population)
        flapper->kill();

    Session::pipes.clear();

    // Saving models
    if (modelsFilePath.size() > 0)
    {
        vector<float> scores; // List of scores for logging
        for (auto member : Session::evolutionSession->population)
        {
            auto parameters = member->neuralNetwork->get_parameters();
            std::ostringstream strs;
            for (double parameter : parameters)
                strs << setprecision(numeric_limits<double>::digits10) << parameter << ",";

            string definition = strs.str();

            pm_add_to_stage(&Session::pm, definition.substr(0, definition.length() - 1).c_str(), member->getScore());

            // Add scores for logging
            scores.push_back(member->getScore());
        }

        pm_commit(&Session::pm, Core::Session::savedModelsCount);
        pm_save_file(&Session::pm, modelsFilePath.c_str());

        // Log scores
        string message = "";
        int i = 0;
        for (float score : scores)
        {
            message += to_string(i) + " -> " + to_string(score) + "\n";
            i++;
        }
        cout << message << "\n";

        message = "Scores for generation ";
        message += to_string(Core::Session::generationIndex - 1);
        message += ": ";
        sort(scores.begin(), scores.end());
        reverse(scores.begin(), scores.end());

        for (float score : scores)
            message += to_string(score) + " ";

        cout << message << "\n";
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
