# Week 1 Progress

## Goal

Design the basic functionalities of the extension, including physics, controls, and communication through a predefined port.

## Overview

In this week, the new extension of Darwin, Pong, has its foundation set. The extension is developed using [Godot](https://en.wikipedia.org/wiki/Godot_(game_engine)), a powerful [game development environment](https://en.wikipedia.org/wiki/Game_integrated_development_environment), which is much more powerful and way easier to develop the environment of the simulation compared to cocos2d-x. However, this required programming most of the components of this extension in a language called [GDScript](https://en.wikipedia.org/wiki/Godot_(game_engine)#GDScript), in contrast to C++ that was used for developing the cocos2d-x-based extensions.

The physics, which involves setting the shapes of the objects involved for collision detection, motion, controls, etc, has been set. Moreover, the executable of the extension, which has the task of communicating with the control panel of Darwin, needs to have the communication system built too. Thus, a [JSON](https://en.wikipedia.org/wiki/JSON) reading library has been integrated to take data, such as the configuration entities, in a single compact string (based on the protocol of Darwin).

The image below shows all the physics objects in one frame:

![alt text](https://github.com/nahiyan/darwin/raw/gh-pages/pong-progress/book/images/overview.png "Overview")

## Structure

The extension is comprises the following classes of objects:

- Paddle
- Ball
- Boundary

### Paddle

The paddle acts as a table-tennis bat, since the game, pong, itself is inspired by it. It's simply a wide and thin rectangular box in the simulation that is powered through a [KinematicBody2D](https://docs.godotengine.org/en/stable/classes/class_kinematicbody2d.html#class-kinematicbody2d) in Godot. KinematicBody2D makes it possible for the paddle to move, collide with the ball and also be obstructed by the boundaries.

The mechanism for controlling the paddle, which includes its horizontal motion, has been programmed in GDScript.

### Ball

The ball poses the main challenge for the players (controllers of the paddles). The only objective of the game is to hit the ball and not miss it. If the ball hits the bottom boundary, the player is out. It's represented by a [RigidBody2D](https://docs.godotengine.org/en/stable/classes/class_rigidbody2d.html) in Godot. RigidBody2D allows the physics engine to calculate the position and velocity of the ball at any given time based on the applied force/impulse, torque, etc. It allows the ball to bounce off the boundary/paddle[s]. Continuous collision detection based on the ball's casted shape is used to avoid the ball tunneling through the boundary due to sudden great change in speed.

### Boundary

The boundary is what keeps the ball and paddles from escaping the game scene. It's represented by a [StaticBody2D](https://docs.godotengine.org/en/stable/classes/class_staticbody2d.html) in Godot, since it doesn't move but will have a role in collision detection. The boundary will cover all 4 sides of the viewport.

## Communication

The extension will communicate with Darwin's components, such as the control panel, through JSON. The control panel will relay configuration details through JSON of the following entities:

> The machine name, used as keys in the JSON data, is in parenthesis.

- Models File Path (`models_file_path`) 
- Initial Population Source (`start_from_saved_models`)
- Speed (`speed`)
- Population Size (`population_size`)
- Genome Similarity:
    - Weights Difference Coefficient (`gs_c_weights_difference`)
    - Disjoints Difference Coefficient (`gs_c_disjoints_difference`)
    - Overall Coefficient (`gs_c_overall`)
- Species Staleness Threshold (`species_staleness_threshold`)
- Probabilities:
    - Crossover (`crossover`)
    - Connection Mutation (`connection_mutation`)
    - Perturb (`perturb`)
    - Link Mutation (`link_mutation`)
    - Node Mutation (`node_mutation`)
    - Bias Mutation (`bias_mutation`)
    - Disable Mutation (`disable_mutation`)
    - Enable Mutation (`enable_mutation`)
