extends Node

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
    if Global.configuration.test_models:
        var test_scene = preload("res://scenes/Test.tscn")
        var result = get_tree().change_scene_to(test_scene)
        if result != OK:
            print("Failed to change scene.")
        Global.test_mode = true
    # Or else, switch to the parallel evolution scene
    else:
        var parallel_evolution_scene = preload("res://scenes/ParallelEvolution.tscn")
        var result = get_tree().change_scene_to(parallel_evolution_scene)
        if result != OK:
            print("Failed to change scene.")
