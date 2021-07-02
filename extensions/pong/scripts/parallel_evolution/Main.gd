extends Node2D
var population: Array = []
var stages_complete = 0
# Theoretical maximum fitness = 6 + HitReward * 3 = 12 if HitReward = 2
var max_fitness: float = 0
var generation_id: int = 1

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
    # Fetch CLI arguments
    var args = OS.get_cmdline_args()
    Global.configuration.parse(args[args.size() - 1])

    # Prepare the population
    Neat.prepare(
        Global.configuration.population_size,
        Global.configuration.delta_disjoint,
        Global.configuration.delta_weights,
        Global.configuration.delta_threshold,
        Global.configuration.stale_species,
        Global.configuration.connection_mutate_chance,
        Global.configuration.perturb_chance,
        Global.configuration.crossover_chance,
        Global.configuration.link_mutation_chance,
        Global.configuration.node_mutation_chance,
        Global.configuration.bias_mutation_chance,
        Global.configuration.disable_mutation_chance,
        Global.configuration.enable_mutation_chance
    )

    # Load models
    PersistentModels.initiate(Global.configuration.models_file_path)

    # Switch to the test scene if enabled
    if Global.configuration.test_best_model:
        var test_scene = preload("res://scenes/Test.tscn")
        var result = get_tree().change_scene_to(test_scene)
        if result != OK:
            print("Failed to change scene.")
        Global.test_mode = true

    # Initiate the population with saved models if enabled
    if Global.configuration.start_from_saved_models:
        var count = PersistentModels.count()
        for i in range(min(count, Global.configuration.population_size)):
            var model = PersistentModels.model(i)
            var definition:String = model[0]

            Neat.model_from_string(i, definition)

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

    for i in range(Global.configuration.population_size):
        var paddle_group: Node = paddle_group_scene.instance()
        population.append(paddle_group)

        # Add the paddle to each scene
        for stage in $Stages.get_children():
            var paddle: Node2D = paddle_scene.instance()
            paddle.id = i
            paddle_group.add(paddle)
            stage.add_child(paddle)
            paddle.reposition(Global.CENTER)


func _process(_delta: float) -> void:
    var move_to_next_generation = true
    for stage in $Stages.get_children():
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
            member.reposition(Global.CENTER)

        if group.members.size() > 0:
            var model_id = group.members[0].id
            Neat.set_fitness(model_id, score)
            scores.append(score)
            max_fitness = max(score, max_fitness)
            PersistentModels.stage(Neat.model_to_string(model_id), score)
    PersistentModels.commit(Global.configuration.saved_models_count)
    PersistentModels.save()

    scores.sort()
#    var message = ''
#    for i in Global.configuration.population_size:
#        message += str(scores[Global.configuration.population_size - i - 1]) + ' '
#    print(message, '\n')

    generation_id += 1
    $Hud.set_generation(generation_id)
    $Hud.set_max_fitness(max_fitness)

    Neat.next_generation()

    for stage in $Stages.get_children():
        stage.revive_ball()

func _on_Main_tree_exited() -> void:
    pass
