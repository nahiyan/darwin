extends KinematicBody2D

onready var ball: Node2D = $"../Stages/Stage1/Ball"

func _process(delta: float) -> void:
    var _collision: KinematicCollision2D = move_and_collide(Vector2(ball.position.x - position.x, 0), delta)
