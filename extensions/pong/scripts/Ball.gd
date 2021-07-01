extends RigidBody2D

#var max_speed = 848
onready var main: Node2D = $"/root/Main"
onready var viewport_rect: Rect2 = get_viewport_rect()
var harmful: bool = false
var stage: Node2D = null
var to_be_killed: bool = false


func reset() -> void:
    set_position(Vector2(512, 20))
    set_linear_velocity(stage.ball_initial_velocity)

    if stage.ball_harmful and !harmful:
        toggle_harmful()


func toggle_harmful() -> void:
    harmful = !harmful
    if harmful:
        set_modulate(Color(1, .392, .392, 1))
    else:
        set_modulate(Color(1, 1, 1, 1))


func _on_Ball_body_entered(body: Node) -> void:
    if body.is_in_group("paddles"):
        if harmful:
            body.penalize_ball_hit()
        else:
            body.reward_ball_hit()


func _on_Ball_body_exited(body: Node) -> void:
    # Escaping from a paddle collision
    if !to_be_killed and (body.is_in_group("paddles") or body.is_in_group("harmful_boundaries")):
        stage.reward_all_paddles()
        to_be_killed = true


func kill() -> void:
    if get_parent() != null:
        to_be_killed = false
        stage.remove_child(self)

