extends Node2D
var configuration: Configuration = Configuration.new()
var population: Array = []


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

    # Stage settings
    var stage_settings: Array = [{
        "ball_harmful": false
       }, {
        "ball_harmful": false
       }, {
        "ball_harmful": false
       }, {
        "ball_harmful": true
       }, {
        "ball_harmful": true
       }, {
        "ball_harmful": true
       }]


    # Add the balls
    var ball_scene = preload("res://scenes/Ball.tscn")
    for i in range($Stages.get_child_count()):
        var ball = ball_scene.instance()
        $Stages.get_child(i).add_child(ball)
        if stage_settings[i]["ball_harmful"]:
            ball.toggle_harmful()
        ball.reset()

    # Add the paddles
    var paddle_group_scene = preload("res://scenes/PaddleGroup.tscn")
    var paddle_scene = preload("res://scenes/Paddle.tscn")

    for i in range(configuration.population_size):
        var paddle_group: Node = paddle_group_scene.instance()
        population.append(paddle_group)

        # Add the paddle to each scene
        for j in range($Stages.get_child_count()):
            var paddle: Node2D = paddle_scene.instance()
            paddle.id = i
            paddle.group_position = j + 1
            paddle_group.add(paddle)
            $Stages.get_child(j).add_child(paddle)
            paddle.reposition_center()


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta: float) -> void:
#    pass
