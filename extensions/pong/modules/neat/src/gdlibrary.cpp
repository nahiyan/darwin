#include "tinyai/tinyann.hpp"
#include "tinyai/tinyneat.hpp"
#include <Godot.hpp>
#include <memory>
#include <vector>

using namespace std;

namespace godot {
class Neat : public Reference {
    GODOT_CLASS(Neat, Reference)

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
        pool->speciating_parameters.population = population_size;
        srand(time(NULL));

        update();
    }

    void setFitness(int id, float fitness)
    {
        genomes[id]->fitness = fitness;
    }

    double evaluate(int id, bool harmfulBall, float horizontalDistance, float verticalDistance)
    {
        vector<double> inputs, outputs(1, 0.0);
        inputs.push_back(harmfulBall);
        inputs.push_back(horizontalDistance);
        inputs.push_back(verticalDistance);
        neuralNetworks[id].evaluate(inputs, outputs);

        return outputs[0];
    }

    void nextGeneration()
    {
        pool->new_generation();
        update();
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
}
