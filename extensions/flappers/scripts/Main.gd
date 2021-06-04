extends Node2D


onready var pipe_scene: PackedScene = preload("res://scenes/Pipe.tscn")

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
    _on_PipeAddTimer_timeout()


func _on_PipeAddTimer_timeout() -> void:
    var pipe: Node2D = pipe_scene.instance()
    pipe.position.x = get_viewport_rect().size.x
    pipe.top_pipe_fraction = rand_range(0.1, 0.9)
    add_child(pipe)
