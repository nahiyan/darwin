#include "persistent_models.h"
#include "tinyai/tinyann.hpp"
#include "tinyai/tinyneat.hpp"
#include <Godot.hpp>
#include <Node.hpp>
#include <memory>
#include <vector>

using namespace std;

// alias the structs
typedef PersistentModels PM;
typedef PMModel PMM;

namespace godot {
// NEAT
class Neat : public Node {
    GODOT_CLASS(Neat, Node)

private:
    shared_ptr<neat::pool> pool;
    vector<neat::genome*> genomes;
    vector<ann::neuralnet> neuralNetworks;

public:
    static void _register_methods()
    {
        register_method("prepare", &Neat::prepare);
        register_method("set_fitness", &Neat::setFitness);
        register_method("next_generation", &Neat::nextGeneration);
        register_method("evaluate", &Neat::evaluate);
        register_method("model_to_string", &Neat::modelToString);
        register_method("model_from_string", &Neat::modelFromString);
    }

    void _init()
    {
    }

    void update()
    {
        genomes.clear();
        neuralNetworks.clear();

        auto genomeSpeciePairs = pool->get_genomes();
        for (auto pair : genomeSpeciePairs) {
            genomes.push_back(pair.second);

            ann::neuralnet neuralNetwork;
            neuralNetwork.from_genome(*pair.second);
            neuralNetworks.push_back(neuralNetwork);
        }
    }

    void prepare(int population_size, double delta_disjoint, double delta_weights, double delta_threshold, unsigned int stale_species, double connection_mutate_chance, double perturb_chance, double crossover_chance, double link_mutation_chance, double node_mutation_chance, double bias_mutation_chance, double disable_mutation_chance, double enable_mutation_chance)
    {
        neat::mutation_rate_container mutation_rates;
        neat::speciating_parameter_container speciating_parameters;

        // population size
        speciating_parameters.population = population_size;

        // deltas
        speciating_parameters.delta_disjoint = delta_disjoint;
        speciating_parameters.delta_weights = delta_weights;
        speciating_parameters.delta_threshold = delta_threshold;

        // stale species
        speciating_parameters.stale_species = stale_species;

        // probabilities
        mutation_rates.connection_mutate_chance = connection_mutate_chance;
        mutation_rates.perturb_chance = perturb_chance;
        mutation_rates.crossover_chance = crossover_chance;
        mutation_rates.link_mutation_chance = link_mutation_chance;
        mutation_rates.node_mutation_chance = node_mutation_chance;
        mutation_rates.bias_mutation_chance = bias_mutation_chance;
        mutation_rates.disable_mutation_chance = disable_mutation_chance;
        mutation_rates.enable_mutation_chance = enable_mutation_chance;

        pool = make_shared<neat::pool>(3, 1, speciating_parameters, mutation_rates, 0, false);
        srand(time(NULL));

        update();
    }

    void setFitness(int id, float fitness)
    {
        try {
            genomes[id]->fitness = fitness;
        } catch (...) {
            Godot::print("Error occurred while trying to set fitness.");
        }
    }

    double evaluate(int id, bool harmfulBall, float horizontalDistance, float verticalDistance)
    {
        try {
            if (id < neuralNetworks.size()) {
                vector<double> inputs, outputs(1, 0.0);
                inputs.push_back(harmfulBall);
                inputs.push_back(horizontalDistance);
                inputs.push_back(verticalDistance);
                neuralNetworks[id].evaluate(inputs, outputs);

                return outputs[0];
            } else {
                Godot::print("Failed to evaluate model: Model with the ID doesn't exist.");
                cout << neuralNetworks.size() << "\n";
                return 0;
            }
        } catch (...) {
            Godot::print("Error occurred while trying to evaluate.");

            return 0;
        }
    }

    String modelToString(int id)
    {
        try {
            stringstream ss;
            neuralNetworks[id].export_to_stream(ss);

            return String(ss.str().c_str());
        } catch (...) {
            Godot::print("Error occurred while trying to convert stringify neural network.");

            return String("");
        }
    }

    void modelFromString(int id, godot::String string)
    {
        try {
            stringstream ss(string.utf8().get_data());
            neuralNetworks[id].import_from_stream(ss);
        } catch (...) {
            Godot::print("Error occurred while trying to convert string to neural network.");
        }
    }

    void nextGeneration()
    {
        pool->new_generation();
        update();
    }
};
// Persistent Models
struct PMModel {
    string definition;
    float fitness;
};

class PersistentModels : public Node {
    GODOT_CLASS(PersistentModels, Node)
private:
    PM* pm;
    string path;

public:
    static void _register_methods()
    {
        register_method("initiate", &PersistentModels::initiate);
        register_method("model", &PersistentModels::model);
        register_method("count", &PersistentModels::count);
        register_method("stage", &PersistentModels::stage);
        register_method("commit", &PersistentModels::commit);
        register_method("save", &PersistentModels::save);
    }

    void _init()
    {
        pm = nullptr;
    }

    void initiate(godot::String path)
    {
        this->path = path.utf8().get_data();
        pm = pm_load_file(this->path.c_str());
    }

    godot::Array model(int index)
    {
        PMM pmm = pm_get_model(&pm, index);

        godot::Variant definition(godot::String(pmm.definition));
        pm_free_string((char*)pmm.definition);
        godot::Variant fitness(pmm.fitness);

        godot::Array model;
        model.append(definition);
        model.append(fitness);

        return model;
    }

    int count()
    {
        if (pm == nullptr) {
            Godot::print("PM isn't initialized");
            return 0;
        }
        return pm_count(&pm);
    }

    void stage(godot::String definition, float fitness)
    {
        pm_add_to_stage(&pm, definition.utf8().get_data(), fitness);
    }

    void commit(int amount)
    {
        pm_commit(&pm, amount);
    }

    void save()
    {
        pm_save_file(&pm, path.c_str());
    }

    ~PersistentModels()
    {
        if (pm != nullptr)
            pm_free_models(pm);
    }
};
}

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options* o)
{
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options* o)
{
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void* handle)
{
    godot::Godot::nativescript_init(handle);

    godot::register_class<godot::Neat>();
    godot::register_class<godot::PersistentModels>();
}
