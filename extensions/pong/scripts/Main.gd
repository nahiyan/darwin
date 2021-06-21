extends Node2D

var paddle_scene = preload("res://scenes/Paddle.tscn")
var paddle_group_scene = preload("res://scenes/PaddleGroup.tscn")
var population_size = 100
var population: Array = []
onready var neat: Node = $Neat
var starting_position: Vector2 = Vector2.ZERO
var generation_id: int = 1
onready var hud: Node = $Hud
var max_fitness: float = 0
onready var ball: Node2D = $Ball

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
    neat.prepare(population_size)
    $Ball.reset()

#    Quotations are going to be excluded
#    print(OS.get_cmdline_args())
#    Configuration.parse('{"speed": 4}')


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
            if !exclusion.has(member) and member.get_parent() != null:
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
