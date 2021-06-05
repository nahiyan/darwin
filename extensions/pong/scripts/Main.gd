extends Node2D

var Paddle = preload("res://scenes/Paddle.tscn")
var population_size = 30
var population: Array = []
#var neat: Node = null

func _ready() -> void:
    var viewport_size: Vector2 = get_viewport_rect().size

    # Initialize the objects
    for i in range(population_size):
        var paddle: Node2D = Paddle.instance()
        paddle.position = Vector2(viewport_size.x / 2, viewport_size.y - 10)
        paddle.id = i + 1
        add_child(paddle)
        population.append(paddle)

    # Prepare the population
    $Neat.prepare(population_size)

#func next_generation() -> void:
#    for i in range(population_size):
#        neat.set_fitness(i, population[i].fitness)
#    neat.next_generation()
