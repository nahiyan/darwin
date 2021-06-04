extends KinematicBody2D

func _ready() -> void:
    pass


func _process(_delta: float) -> void:
    var extents: Vector2 = ($CollisionShape2D.shape as RectangleShape2D).extents
    position.x = clamp(get_viewport().get_mouse_position().x, extents.x, get_viewport().size.x - extents.x)

#    var ball: Node2D = $"/root/Main/Ball"
#    var ball_radius: float = (ball.get_node("CollisionShape2D").shape as CircleShape2D).radius
#    var vertical_distance: float = position.y - extents.y - ball.position.y - ball_radius
#    var horizontal_distance: float = abs(position.x - ball.position.x)
#    print(vertical_distance, " ", horizontal_distance)
