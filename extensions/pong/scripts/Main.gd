extends Node2D

var generation_id: int = 1
var max_fitness: float = 0
var starting_position: Vector2 = Vector2.ZERO
var ball_scene = preload("res://scenes/Ball.tscn")
var paddle_scene = preload("res://scenes/Paddle.tscn")
var paddle_group_scene = preload("res://scenes/PaddleGroup.tscn")
var hardcoded_paddle_scene = preload("res://scenes/HardcodedPaddle.tscn")
var population: Array = []
var configuration: Configuration = Configuration.new()
onready var hud: Node = $Hud
onready var neat: Node = $Neat
onready var ball: Node2D = ball_scene.instance()
onready var hardcoded_paddle: Node2D = hardcoded_paddle_scene.instance()


func _ready() -> void:
	# configuration
	var args = OS.get_cmdline_args()
	configuration.parse(args[args.size() - 1])

	# Prepare the population
	neat.prepare(
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

	print(configuration.population_size)
	print(configuration.delta_disjoint)
	print(configuration.delta_weights)
	print(configuration.delta_threshold)
	print(configuration.stale_species)
	print(configuration.connection_mutate_chance)
	print(configuration.perturb_chance)
	print(configuration.crossover_chance)
	print(configuration.link_mutation_chance)
	print(configuration.node_mutation_chance)
	print(configuration.bias_mutation_chance)
	print(configuration.disable_mutation_chance)
	print(configuration.enable_mutation_chance)

	# Initialize the objects
	ball.reset()
	add_child(ball)

	add_child(hardcoded_paddle)

	for i in range(configuration.population_size):
		var paddle_group: Node = paddle_group_scene.instance()
		add_child(paddle_group)
		population.append(paddle_group)

		var paddle_left: Node2D = paddle_scene.instance()
		paddle_left.id = i
		paddle_left.type = 0

		paddle_group.add_child(paddle_left)
		paddle_group.members.append(paddle_left)
		paddle_left.reposition_center()

	hud.set_survivors(configuration.population_size, configuration.population_size)


func _process(_delta: float) -> void:
	# Limit the speed of the ball
	if (
		abs(ball.get_linear_velocity().x) > ball.max_speed
		or abs(ball.get_linear_velocity().y) > ball.max_speed
	):
		var new_speed = ball.get_linear_velocity().normalized()
		new_speed *= ball.max_speed
		ball.set_sleeping(true)
		ball.call_deferred("set_linear_velocity", new_speed)
		ball.set_sleeping(false)

	# Prevent the ball from leaving the viewport
	var boundaries: Vector2 = get_viewport_rect().size
	if (
		ball.position.x <= -10
		or ball.position.x >= boundaries.x + 10
		or ball.position.y <= -10
		or ball.position.y >= boundaries.y + 10
	):
		reset_ball()

	# Update survivors
	var children: Array = get_children()
	var survivors: int = 0
	for child in children:
		if child.is_in_group("paddle_groups"):
			for member in child.members:
				if member.get_parent() != null:
					survivors += 1

	hud.set_survivors(survivors, configuration.population_size)


func next_generation() -> void:
	ball.cycles_count = 0
	reset_ball()

	var scores = []
	for group in population:
		var score: float = 0
		for member in group.members:
			score += member.fitness

		scores.append(score)
		max_fitness = max(score, max_fitness)

	scores.sort()
#    var message = ''
#    for i in configuration.population_size:
#        message += str(scores[configuration.population_size - i - 1]) + ' '
#    print(message, '\n')

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


func reset_ball() -> void:
	if ball.get_parent() == self:
		remove_child(ball)
		ball.reset()
		call_deferred("add_child", ball)


func revive_dead_paddles() -> void:
	for group in population:
		for member in group.members:
			if member.get_parent() == null:
				group.add_child(member)
				member.reposition_center()


func _on_Main_tree_exited() -> void:
	print("Exit")
	for group in population:
		for member in group.members:
			if member.get_parent() == null:
				member.queue_free()

			member.queue_free()
