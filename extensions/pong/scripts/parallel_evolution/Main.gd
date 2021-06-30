extends Node2D
var configuration: Configuration = Configuration.new()
var population: Array = []
var stages_complete = 0
var max_fitness: float = 0
var generation_id: int = 1


enum {LEFT = 0, CENTER = 1, RIGHT = 2}

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
    # Fetch CLI arguments
    var args = OS.get_cmdline_args()
    configuration.parse(args[args.size() - 1])

    # Prepare the population
    $Neat.prepare(
        configuration.population_size,
        configuration.delta_disjoint,
        configuration.delta_weights,
        configuration.delta_threshold,
        configuration.stale_species,
        configuration.connection_mutate_chance,
        configuration.perturb_chance,
        configuration.crossover_chance,
        configuration.link_mutation_chance,
        configuration.node_mutation_chance,
        configuration.bias_mutation_chance,
        configuration.disable_mutation_chance,
        configuration.enable_mutation_chance
    )

    # Scale the HUD
    $Hud.set_scale(Vector2($Camera2D.zoom.x, $Camera2D.zoom.y))

    # Add the balls
    var ball_scene = preload("res://scenes/Ball.tscn")
    for stage in $Stages.get_children():
        var ball = ball_scene.instance()
        ball.stage = stage
        ball.reset()
        stage.add_ball(ball)

    # Add the paddles
    var paddle_group_scene = preload("res://scenes/PaddleGroup.tscn")
    var paddle_scene = preload("res://scenes/Paddle.tscn")

    for i in range(configuration.population_size):
        var paddle_group: Node = paddle_group_scene.instance()
        population.append(paddle_group)

        # Add the paddle to each scene
        for stage in $Stages.get_children():
            var paddle: Node2D = paddle_scene.instance()
            paddle.id = i
            paddle_group.add(paddle)
            stage.add_child(paddle)
            paddle.reposition(CENTER)


func _process(delta: float) -> void:
    var move_to_next_generation = true
    for stage in [$Stages/Stage6]:
        if stage.ball.to_be_killed:
            stage.ball.kill()
            continue

        if stage.ball.get_parent() != null:
            move_to_next_generation = false

    if move_to_next_generation:
        next_generation()



func next_generation() -> void:
    stages_complete = 0

    # Calculate the scores and max fitness
    var scores = []
    for group in population:
        var score: float = 0
        for member in group.members:
            score += member.fitness
            member.fitness = 0
            member.reposition(CENTER)

        if group.members.size() > 0:
            $Neat.set_fitness(group.members[0].id, score)
            scores.append(score)
            max_fitness = max(score, max_fitness)

    scores.sort()
#    var message = ''
#    for i in configuration.population_size:
#        message += str(scores[configuration.population_size - i - 1]) + ' '
#    print(message, '\n')

    generation_id += 1
    $Hud.set_generation(generation_id)
    $Hud.set_max_fitness(max_fitness)

    $Neat.next_generation()

    for stage in [$Stages/Stage6]:
        stage.revive_ball()


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta: float) -> void:
#    pass
