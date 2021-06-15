class_name Configuration

# general
var models_file_path: String = ""
var start_from_saved_models: bool = false
var speed: float = 1
var population_size: int = 30

# genenome similarity
var gs_c_weights_difference: float = 1
var gs_c_disjoints_difference: float = 1
var gs_c_overall: float = 1

# probabilities
var species_staleness_threshold: float = 0.5
var crossover: float = 0.5
var connection_mutation: float = 0.5
var perturb: float = 0.5
var link_mutation: float = 0.5
var node_mutation: float = 0.5
var bias_mutation: float = 0.5
var disable_mutation: float = 0.5
var enable_mutation: float = 0.5

static func parse(text: String) -> void:
    var parse_result: JSONParseResult = JSON.parse(text)
    print(parse_result.result)
    print(parse_result.error == OK)
