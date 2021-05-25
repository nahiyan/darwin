extends KinematicBody2D

var top_pipe_fraction = .4
var gap = 150
var ground_vertical_space: float = 60
var pipe_width: float = 109


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
    var pipes_vertical_space: float = get_viewport_rect().size.y - gap - ground_vertical_space
    var top_pipe_vertical_space: float = pipes_vertical_space * top_pipe_fraction
    var bottom_pipe_vertical_space: float = pipes_vertical_space - top_pipe_vertical_space
    var top_pipe_rectangle: RectangleShape2D = RectangleShape2D.new()

    var bottom_pipe_rectangle: RectangleShape2D = RectangleShape2D.new()
    var topPipeVerticalPosition: float = top_pipe_vertical_space / 2
    var bottomPipeVerticalPosition: float = (
        top_pipe_vertical_space
        + gap
        + bottom_pipe_vertical_space / 2
    )

    $TopPipeSprite.position.y = topPipeVerticalPosition
    $TopPipeCollisionShape.position.y = topPipeVerticalPosition
    $TopPipeCollisionShape.set_shape(top_pipe_rectangle)
    $TopPipeSprite.region_rect.size.y = top_pipe_vertical_space
    top_pipe_rectangle.extents.x = pipe_width / 2
    top_pipe_rectangle.extents.y = top_pipe_vertical_space / 2

    $BottomPipeSprite.position.y = bottomPipeVerticalPosition
    $BottomPipeCollisionShape.position.y = bottomPipeVerticalPosition
    $BottomPipeCollisionShape.set_shape(bottom_pipe_rectangle)
    $BottomPipeSprite.region_rect.size.y = bottom_pipe_vertical_space
    bottom_pipe_rectangle.extents.x = pipe_width / 2
    bottom_pipe_rectangle.extents.y = bottom_pipe_vertical_space / 2


func _physics_process(delta: float) -> void:
    var collision = move_and_collide(Vector2(-100, 0) * delta)
    if collision:
        queue_free()
