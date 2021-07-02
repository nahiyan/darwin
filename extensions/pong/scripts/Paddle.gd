extends KinematicBody2D

var id: int = -1

var fitness: float = 0
onready var ball: Node2D = $"../Ball"
onready var ball_radius: float = (ball.get_node("CollisionShape2D").shape as CircleShape2D).radius


func _process(delta: float) -> void:
    var extents: Vector2 = ($CollisionShape2D.shape as RectangleShape2D).extents

    var horizontal_distance: float = position.x - ball.position.x
    var vertical_distance: float = abs(position.y - extents.y - ball.position.y - ball_radius)

    var evaluation = Neat.evaluate(id, ball.harmful, horizontal_distance, vertical_distance)
    var _collision: KinematicCollision2D = move_and_collide(
        evaluation * Vector2(1500, 0) * delta
    )


func reposition(type: int) -> void:
    match type:
        0:
            position = Vector2(($CollisionShape2D.shape as RectangleShape2D).extents.x, Global.viewport_size.y - 10)
        1:
            position = Vector2(Global.viewport_size.x / 2, Global.viewport_size.y - 10)
        _:
            position = Vector2(Global.viewport_size.x - ($CollisionShape2D.shape as RectangleShape2D).extents.x, Global.viewport_size.y - 10)


func reward(amount: float) -> void:
    fitness += amount


func reward_for_avoiding(object: Node2D) -> void:
    reward(abs(object.position.x - position.x) / object.get_viewport_rect().size.x)


func reward_for_approaching(object: Node2D) -> void:
    reward(1 - abs(object.position.x - position.x) / object.get_viewport_rect().size.x)


func penalize_ball_hit() -> void:
    reward(-2)


func reward_ball_hit() -> void:
    reward(2)


func kill() -> void:
    var parent = get_parent()
    parent.remove_child(self)
