extends Node2D

var paddle_scene = preload("res://scenes/Paddle.tscn")
var population_size = 50
var population: Array = []
onready var neat: Node = $Neat
var starting_position: Vector2 = Vector2.ZERO
var generation_id: int = 1
onready var hud: Node = $Hud
var max_fitness: float = 0

func _ready() -> void:
    # Initialize the objects
    for i in range(population_size):
        var paddle: Node2D = paddle_scene.instance()
        paddle.id = i + 1
        add_child(paddle)
        population.append(paddle)

    # Prepare the population
    neat.prepare(population_size)
    $Ball.set_linear_velocity(Vector2(600, 600))

#    Quotations are going to be excluded
#    print(OS.get_cmdline_args())


func next_generation() -> void:
    var ball = $Ball
    remove_child(ball)
    ball.set_position(Vector2(500, 150))
    if generation_id % 2 != 0:
        ball.set_linear_velocity(Vector2(600, 600))
    else:
        ball.set_linear_velocity(Vector2(-600, -600))
    add_child(ball)

    var fitnesses = ''
    for member in population:
        fitnesses += str(member.fitness) + ' '
        max_fitness = max(member.fitness, max_fitness)

    print(fitnesses)
    generation_id += 1
    hud.set_generation(generation_id)
    hud.set_max_fitness(max_fitness)

    # Finalize fitness, reset fitness, position and add to scene
    for member in population:
        neat.set_fitness(member.id, member.fitness)
        member.fitness = 0
        if member.get_parent() == null:
            add_child(member)
        else:
            member.reposition()

    neat.next_generation()


func kill_except(exclusion: Array) -> void:
    for member in population:
        if !exclusion.has(member) and member.get_parent() != null:
            member.kill()
