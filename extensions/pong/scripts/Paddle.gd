extends KinematicBody2D

var id: int = -1
var fitness: float = 0
onready var neat: Node = $"/root/Main/Neat"
onready var ball: Node2D = $"/root/Main/Ball"
var type: int = 0

#func _ready() -> void:
#    reposition_center()


func _process(delta: float) -> void:
    var extents: Vector2 = ($CollisionShape2D.shape as RectangleShape2D).extents
#    position.x = clamp(get_viewport().get_mouse_position().x, extents.x, get_viewport().size.x - extents.x)

    var ball_radius: float = (ball.get_node("CollisionShape2D").shape as CircleShape2D).radius
    var horizontal_distance: float = position.x - ball.position.x
    var vertical_distance: float = abs(position.y - extents.y - ball.position.y - ball_radius)
#    print(neat.evaluate(id, horizontal_distance, vertical_distance))

#    position.x = clamp(position.x + neat.evaluate(id, horizontal_distance, vertical_distance) * 25, extents.x, get_viewport().size.x - extents.x)

    var evaluation = neat.evaluate(id, horizontal_distance, vertical_distance)
#    print(horizontal_distance, ' ', vertical_distance, '', evaluation)
    var _collision: KinematicCollision2D = move_and_collide(
        evaluation * Vector2(500, 0) * delta
    )

func reposition_center() -> void:
    var viewport_size: Vector2 = get_viewport_rect().size
    position = Vector2(viewport_size.x / 2, viewport_size.y - 10)

func reposition_left() -> void:
    var viewport_size: Vector2 = get_viewport_rect().size
    position = Vector2(($CollisionShape2D.shape as RectangleShape2D).extents.x, viewport_size.y - 10)

func reposition_right() -> void:
    var viewport_size: Vector2 = get_viewport_rect().size
    position = Vector2(viewport_size.x - ($CollisionShape2D.shape as RectangleShape2D).extents.x, viewport_size.y - 10)


func reward(amount: float) -> void:
    fitness += amount


func kill() -> void:
    fitness = max(fitness - 1, 0)
    var parent = get_parent()
    parent.remove_child(self)
