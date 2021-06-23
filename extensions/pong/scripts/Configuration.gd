class_name Configuration

# general
var models_file_path: String = ""
var start_from_saved_models: bool = false
var population_size: int = 30

# genenome similarity
var gs_c_disjoints_difference: float = 2.0
var gs_c_weights_difference: float = 0.4
var gs_c_overall: float = 1.3

# stale species threshold
var species_staleness_threshold: float = 15

# probabilities
var connection_mutation: float = 0.25
var perturb: float = 0.90
var crossover: float = 0.75
var link_mutation: float = 2.0
var node_mutation: float = 0.50
var bias_mutation: float = 0.40
var disable_mutation: float = 0.4
var enable_mutation: float = 0.2


func parse(text: String) -> void:
    var parse_result: JSONParseResult = JSON.parse(text)
    if parse_result.error == OK:
        # models_file_path
        if parse_result.result.has('start_from_saved_modmodels_file_pathels'):
            models_file_path = parse_result.result.get('models_file_path')

        # start_from_saved_models
        if parse_result.result.has('start_from_saved_models'):
            start_from_saved_models = parse_result.result.get('start_from_saved_models')

        # speed
        if parse_result.result.has('speed'):
            Engine.set_time_scale(parse_result.result.get('speed'))

        # population_size
        if parse_result.result.has('population_size'):
            population_size = parse_result.result.get('population_size')

        # gs_c_weights_difference
        if parse_result.result.has('gs_c_weights_difference'):
            gs_c_weights_difference = parse_result.result.get('gs_c_weights_difference')

        # gs_c_disjoints_difference
        # gs_c_overall
        # species_staleness_threshold
        # crossover
        # connection_mutation
        # perturb
        # link_mutation
        # node_mutation
        # bias_mutation
        # disable_mutation
        # enable_mutation
