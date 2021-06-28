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

