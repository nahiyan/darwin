extends Node


onready var generation: Label = $Generation
onready var max_fitness: Label = $MaxFitness

func _ready() -> void:
    set_generation(1)
    set_max_fitness(0)

func set_generation(id: int) -> void:
    generation.text = "Generation " + str(id)

func set_max_fitness(value: float) -> void:
    max_fitness.text = "Max Fitness: " + str(value)
