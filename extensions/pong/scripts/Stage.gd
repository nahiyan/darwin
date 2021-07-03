extends Node2D

export var ball_initial_velocity = Vector2(0, 0)
export var ball_harmful = false
var ball: Node2D = null
onready var main: Node2D = $"/root/Main"


func reward_all_paddles() -> void:
    var members = get_children()
    for member in members:
        if member.get_parent() != null and member.is_in_group("paddles"):
            if ball.harmful:
                member.reward_for_avoiding(ball)
            else:
                member.reward_for_approaching(ball)


func add_ball(new_ball: Node2D) -> void:
    ball = new_ball
    add_child(ball)


func revive_ball():
    ball.reset()
    add_child(ball)

func _draw() -> void:
    var shape:RectangleShape2D = $HarmlessBoundaries/Left.shape as RectangleShape2D
    var size:Vector2 = shape.extents * 2
    # Top
    draw_rect(Rect2(Vector2(0, -size.x), $HarmlessBoundaries/Top.shape.extents * 2), Color.white)

    # Bottom
    draw_rect(Rect2(Vector2(0, Global.viewport_size.y), $HarmfulBoundaries/Bottom.shape.extents * 2), Color.white)

    # Left
    draw_rect(Rect2(Vector2(-size.x, -size.x), $HarmlessBoundaries/Left.shape.extents * 2), Color.white)

    # Right
    draw_rect(Rect2(Vector2(Global.viewport_size.x, -size.x), $HarmlessBoundaries/Right.shape.extents * 2), Color.white)
