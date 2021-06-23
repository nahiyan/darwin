extends Node2D

var generation_id: int = 1
onready var hud: Node = $Hud
var max_fitness: float = 0
var starting_position: Vector2 = Vector2.ZERO
var paddle_scene = preload("res://scenes/Paddle.tscn")
var paddle_group_scene = preload("res://scenes/PaddleGroup.tscn")
var population_size = 100
var population: Array = []
onready var neat: Node = $Neat
onready var ball: Node2D = $Ball
onready var configuration: Configuration = Configuration.new()


func _ready() -> void:
    # Initialize the objects
    for i in range(population_size):
        var paddle_group: Node = paddle_group_scene.instance()
        add_child(paddle_group)
        population.append(paddle_group)

        var paddle_left: Node2D = paddle_scene.instance()
#        var paddle_right: Node2D = paddle_scene.instance()
        paddle_left.id = i + 1
        paddle_left.type = 0
#        paddle_right.id = i + 1
#        paddle_right.type = 1

        paddle_group.add_child(paddle_left)
#        paddle_group.add_child(paddle_right)
        paddle_group.members.append(paddle_left)
#        paddle_group.members.append(paddle_right)
        paddle_left.reposition_left()
#        paddle_right.reposition_right()

    # Prepare the population
    configuration.parse('{"speed": 1}')
    neat.prepare(
        population_size,
        configuration.gs_c_disjoints_difference,
        configuration.gs_c_weights_difference,
        configuration.gs_c_overall,
        configuration.species_staleness_threshold,
        configuration.connection_mutation,
        configuration.perturb,
        configuration.crossover,
        configuration.link_mutation,
        configuration.node_mutation,
        configuration.bias_mutation,
        configuration.disable_mutation,
        configuration.enable_mutation
    )

    $Ball.reset()


#    Quotations are going to be excluded
#    print(OS.get_cmdline_args())


func next_generation() -> void:
    remove_child(ball)
    ball.reset()
    add_child(ball)

    var scores = []
    for group in population:
        var score: float = 0
        for member in group.members:
            score += member.fitness

        scores.append(score)
        max_fitness = max(score, max_fitness)

    scores.sort()
    var message = ''
    for i in population_size:
        message += str(scores[population_size - i - 1]) + ' '
    print(message, '\n')

    generation_id += 1
    hud.set_generation(generation_id)
    hud.set_max_fitness(max_fitness)

    # Finalize fitness, reset fitness, position and add to scene
    for group in population:
        var score: float = 0
        for member in group.members:
            score += member.fitness
            member.fitness = 0

            if member.get_parent() == null:
                group.add_child(member)

            if member.type == 0:
                member.reposition_left()
            else:
                member.reposition_right()

        if group.members.size() > 0:
            neat.set_fitness(group.members[0].id, score)

    neat.next_generation()


func kill_except(exclusion: Array) -> void:
    for group in population:
        for member in group.members:
            if ! exclusion.has(member) and member.get_parent() != null:
                member.kill()


func kill(inclusion: Array) -> void:
    for member in inclusion:
        if member.get_parent() != null:
            member.kill()


func reward_all_paddles() -> void:
    for group in population:
        for member in group.members:
            if member.get_parent() != null:
                if ball.harmful:
                    member.reward_for_avoiding(ball)
                else:
                    member.reward_for_approaching(ball)


func force_reset_ball() -> void:
    ball.sleeping = true
    print("Reset ball position by force")
    ball.reset()
    ball.sleeping = false
