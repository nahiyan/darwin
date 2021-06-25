# Week 3 Progress

## Goal

Integrate the NEAT algorithm module, along with all other required modules of Darwin, into the extension. The extension needs to handle the evaluation of the genomes, calculating fitness values at the end of every generation, and connecting each player instance with its respective phenotype.

## Connecting the Library with Godot

The extension, Pong, uses the game framework, Godot, as its base. As a result, for connecting the NEAT algorithm's module, which is a library, to Godot, GDNative has to be used. GDNative is a mechanism for connecting the library with the game framework, Godot.

To do so, the library, written in C++ and comprises the API built in the previous week, has to be linked with godot-cpp, a C++ library containing everything required to let Godot load and use it.

Once connected, the library and its functions can be accessed through GDScript, as with any other GDScript-based function.

## Controlling the Paddles

```python
func _process(delta: float) -> void:
    var extents: Vector2 = ($CollisionShape2D.shape as RectangleShape2D).extents

    var ball_radius: float = (ball.get_node("CollisionShape2D").shape as CircleShape2D).radius
    var horizontal_distance: float = position.x - ball.position.x
    var vertical_distance: float = abs(position.y - extents.y - ball.position.y - ball_radius)

    var evaluation = neat.evaluate(id, ball.harmful, horizontal_distance, vertical_distance)
    var _collision: KinematicCollision2D = move_and_collide(
        evaluation * Vector2(1000, 0) * delta
    )
```

Each paddle is controlled every frame/step of the game framework by the respective neural network. The vertical and horizontal distance, along with the type of the ball, is fed into the neural network through its input layer.

The output is simply a value ranging from -1 to 1 (inclusive) that is used to determine the horizontal velocity of the paddle.

`neat::evaluate` belongs to the NEAT algorithm module, which plays the role of evaluating the input parameters using the neural network.

This is how it's handled by the C++ library:

```cpp
double evaluate(int id, bool harmfulBall, float horizontalDistance, float verticalDistance)
{
    try {
        vector<double> inputs, outputs(1, 0.0);
        inputs.push_back(harmfulBall);
        inputs.push_back(horizontalDistance);
        inputs.push_back(verticalDistance);
        neuralNetworks[id].evaluate(inputs, outputs);

        return outputs[0];
    } catch (...) {
        Godot::print("Error occurred while trying to evaluate.");

        return 0;
    }
}
```

## Calculating the Fitness of Each Genome

Whenever the ball hits a paddle or the bottom boundary, the fitness values are updated for the genomes of each paddle.

- Each genome is rewarded for its horizontal distance to the ball, depending on whether the ball is harmful or harmless.

- A harmless ball when hit successfully by a paddle is rewarded, whereas hitting a harmful ball penalizes the paddle that hit it.

This is how the fitness values are handled for the current population to evolve to the next:

```python
func next_generation() -> void:
    ball.cycles_count = 0
    reset_ball()

    for group in population:
        var score: float = 0
        for member in group.members:
            score += member.fitness

        max_fitness = max(score, max_fitness)

    generation_id += 1
    hud.set_generation(generation_id)
    hud.set_max_fitness(max_fitness)

    # Finalize the fitness, reset fitness and position
    for group in population:
        var score: float = 0
        for member in group.members:
            score += member.fitness
            member.fitness = 0

        if group.members.size() > 0:
            neat.set_fitness(group.members[0].id, score)

    neat.next_generation()

    # Add dead paddles to the scene
    revive_dead_paddles()
```

## Connecting the Phenotypes With the Paddles

Each paddle instance is going to have an ID representing it, allocated during the beginning of the evolution session. This is going to be used by the library for identifying the genome to retrieve its phenotype, evaluate it, and register its fitness value.

The genomes and their associated neural networks (phenotypes) are stored in the library in ordered lists:

```cpp
shared_ptr<neat::pool> pool;
vector<neat::genome*> genomes;
vector<ann::neuralnet> neuralNetworks; 
```

This is how the list is populated:

```cpp
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
```