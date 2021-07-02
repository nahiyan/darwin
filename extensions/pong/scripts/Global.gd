extends Node2D

var configuration: Configuration = Configuration.new()
enum {LEFT = 0, CENTER = 1, RIGHT = 2}
var test_mode: bool = false
onready var viewport_size: Vector2 = get_viewport_rect().size

func _ready() -> void:
    pass
