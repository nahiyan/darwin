# Week 2 Progress

## Goal

Build a new module of Darwin, featuring the C++ implementation of the NEAT algorithm. This
will involve the development of an API that will act as a front-end of the module intended for
connecting with the extension.

## C++ Implementation of the NEAT Algorithm

Since the extension, Pong, is developed using Godot, GDScript is used for doing all the scripting. GDScript, however, doesn't have a large community and isn't very popular. Moreover, it's designed for game scripting, not anything else.

For general-purpose and high-performance programming, and also considering the fact that the NEAT algorithm's most popular implementation is based on C/C++, it's logical to do all the neuroevolution programming in C/C++.

The official C++ implementation of the NEAT algorithm, however, isn't well documented and not very easy to use. This called for a hunt for much better libraries featuring the NEAT algorithm. Here are the CPP-based NEAT libraries that I found along the way:

- [MultiNEAT](http://www.multineat.com/)
- [hav4ik/tinyai](https://github.com/hav4ik/tinyai)
- [F3R70/NEAT](https://github.com/F3R70/NEAT)

__MultiNEAT__ appeared to be a well-structured and feature-rich NEAT library but it resulted in a lot of unexpected runtime errors, which were getting a bit too tedious to debug and fix. In short, the attempt to use MultiNEAT was a failure.

__FER70/NEAT__ is just a slightly improved version of the official C++ implementation of the NEAT algorithm with nothing extra added on top, which I considered as a backup library if __hav4ik/tinyai__ wouldn't work.

__hav4ik/tinyai__ is a compact NEAT implementation in C++ that is straightforward and simply works out of the box. It has just all the features that was needed for powering the neuroevolution of the pong simulation.

On a side note, a Rust implementation of the NEAT algorithm,  [TLmaK0/rustneat](https://github.com/TLmaK0/rustneat), also looked promising. However, Godot's FFI only supports C and C++, not Rust, which would mean that a C/C++ library would have to be created to talk with the Rust library on Godot's behalf, further complicating the process.

Thus, __hav4ik/tinyai__ has been chosen for integrating the NEAT algorithm into the extension.

## Development of the API

The API serves as the frontend of the NEAT module of Darwin, providing an interface for the extension to use the NEAT algorithm through GDScript.

The API is simple and hides the complexity of the module, exposing only the following methods:

- prepare
- evaluate
- setFitness
- nextGeneration

Internally, it holds the following properties:

- pool
- genomes
- neuralNetworks

### Properties

#### pool

`pool` represents the pool of genomes in the population, along with innovation tracking. It acts as the center of the whole system: handling settings, evolution, etc.

#### genomes

`genomes` holds the genomes of all species in the current generation. The list is cleared and populated at the start of every generation.

#### neuralNetworks

`neuralNetworks` is a list of the neural networks associated with each genome; or rather, the phenotype of each genotype. Since the neural networks, which are needed several times in a generation, have to be generated from a genome every single time, it's efficient to store them in a place instead of spawning them unnecessarily.

### Methods

#### prepare

This method takes the parameters and initiates the `pool` along with the rest of the properties for the initial generation. It's called once at the beginning of the session.

#### evaluate

Called by each genome to get the output value from the input values by evaluating the associated neural network.

#### setFitness

Registers the fitness value of a specific genome.

#### nextGeneration

Proceeds to the next generation by triggering the pool and updating the list of genotypes and phenotypes (neural networks).