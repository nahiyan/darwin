extends KinematicBody2D

var id: int = -1
var fitness: float = 0
onready var neat: Node = $"/root/Main/Neat"
onready var ball: Node2D = $"/root/Main/Ball"

func _ready() -> void:
    reposition()


func _process(delta: float) -> void:
    var extents: Vector2 = ($CollisionShape2D.shape as RectangleShape2D).extents
#    position.x = clamp(get_viewport().get_mouse_position().x, extents.x, get_viewport().size.x - extents.x)

    var ball_radius: float = (ball.get_node("CollisionShape2D").shape as CircleShape2D).radius
    var horizontal_distance: float = abs(position.x - ball.position.x)
    var vertical_distance: float = abs(position.y - extents.y - ball.position.y - ball_radius)
#    print(neat.evaluate(id, horizontal_distance, vertical_distance))

#    position.x = clamp(position.x + neat.evaluate(id, horizontal_distance, vertical_distance) * 25, extents.x, get_viewport().size.x - extents.x)

    var _collision: KinematicCollision2D = move_and_collide(
        neat.evaluate(id, horizontal_distance, vertical_distance) * Vector2(1000, 0) * delta
    )

func reposition() -> void:
    var viewport_size: Vector2 = get_viewport_rect().size
    position = Vector2(viewport_size.x / 2, viewport_size.y - 10)


func reward(amount: float) -> void:
    fitness += amount


func kill() -> void:
    $"/root/Main".remove_child(self)
