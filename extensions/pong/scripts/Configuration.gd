class_name Configuration

# general
var models_file_path: String = ""
var start_from_saved_models: bool = false
var saved_models_count: int = 10
var population_size: int = 240
var test_models: bool = false
var test_models_count: int = 1

# genenome similarity
var delta_disjoint: float = 2.0
var delta_weights: float = 0.4
var delta_threshold: float = 1.3

# stale species threshold
var stale_species: float = 15

# probabilities
var connection_mutate_chance: float = 0.25
var perturb_chance: float = 0.90
var crossover_chance: float = 0.75
var link_mutation_chance: float = 2.0
var node_mutation_chance: float = 0.5
var bias_mutation_chance: float = 0.4
var disable_mutation_chance: float = 0.4
var enable_mutation_chance: float = 0.2


func parse(text: String) -> void:
    var parse_result: JSONParseResult = JSON.parse(text)
    if parse_result.error == OK:
        # models_file_path
        if parse_result.result.has('models_file_path'):
            models_file_path = parse_result.result.get('models_file_path')

        # start_from_saved_models
        if parse_result.result.has('start_from_saved_models'):
            start_from_saved_models = parse_result.result.get('start_from_saved_models')

        # saved_models_count
        if parse_result.result.has('saved_models_count'):
            saved_models_count = parse_result.result.get('saved_models_count')

        # speed
        if parse_result.result.has('speed'):
            Engine.set_time_scale(parse_result.result.get('speed'))

        # population_size
        if parse_result.result.has('population_size'):
            population_size = parse_result.result.get('population_size')

        # test_models
        if parse_result.result.has('test_models'):
            test_models = parse_result.result.get('test_models')

        # test_models_count
        if parse_result.result.has('test_models_count'):
            test_models_count = parse_result.result.get('test_models_count')

        # delta_disjoint
        if parse_result.result.has('delta_disjoint'):
            delta_disjoint = parse_result.result.get('delta_disjoint')

        # delta_weights
        if parse_result.result.has('delta_weights'):
            delta_weights = parse_result.result.get('delta_weights')

        # delta_threshold
        if parse_result.result.has('delta_threshold'):
            delta_threshold = parse_result.result.get('delta_threshold')

        # stale_species
        if parse_result.result.has('stale_species'):
            stale_species = parse_result.result.get('stale_species')

        # connection_mutate_chance
        if parse_result.result.has('connection_mutate_chance'):
            connection_mutate_chance = parse_result.result.get('connection_mutate_chance')

        # perturb_chance
        if parse_result.result.has('perturb_chance'):
            perturb_chance = parse_result.result.get('perturb_chance')

        # crossover_chance
        if parse_result.result.has('crossover_chance'):
            crossover_chance = parse_result.result.get('crossover_chance')

        # link_mutation_chance
        if parse_result.result.has('link_mutation_chance'):
            link_mutation_chance = parse_result.result.get('link_mutation_chance')

        # node_mutation_chance
        if parse_result.result.has('node_mutation_chance'):
            node_mutation_chance = parse_result.result.get('node_mutation_chance')

        # bias_mutation_chance
        if parse_result.result.has('bias_mutation_chance'):
            bias_mutation_chance = parse_result.result.get('bias_mutation_chance')

        # disable_mutation_chance
        if parse_result.result.has('disable_mutation_chance'):
            disable_mutation_chance = parse_result.result.get('disable_mutation_chance')

        # enable_mutation_chance
        if parse_result.result.has('enable_mutation_chance'):
            enable_mutation_chance = parse_result.result.get('enable_mutation_chance')
