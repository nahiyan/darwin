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

    void prepare(int population_size)
    {
        pool = make_shared<neat::pool>(2, 1, 0, false);
        pool->speciating_parameters.population = population_size;
        srand(time(NULL));

        update();
    }

    void setFitness(int id, float fitness)
    {
        genomes[id]->fitness = fitness;
    }

    double evaluate(int id, float horizontalDistance, float verticalDistance)
    {
        vector<double> inputs, outputs(1, 0.0);
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
