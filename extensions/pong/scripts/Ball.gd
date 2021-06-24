extends RigidBody2D

var max_speed = 848
var selection: Array = []  # selection for survivors in the current round
onready var main: Node2D = $"/root/Main"
var cycles_count: int = 0 # Cycles count for current generation
#var trigger_next_generation: bool = false
onready var viewport_rect: Rect2 = get_viewport_rect()
var harmful: bool = false
var initial_velocity: Vector2 = Vector2(600, 600)
var starting_position: Vector2 = Vector2(512, 20)


func reset() -> void:
#    call_deferred("set_position", starting_position)
#    call_deferred("set_linear_velocity", initial_velocity)
    set_position(starting_position)
    set_linear_velocity(initial_velocity)


func toggle_harmful() -> void:
    harmful = !harmful
    if harmful:
        set_modulate(Color(1, .392, .392, 1))
    else:
        set_modulate(Color(1, 1, 1, 1))


func _process(_delta: float) -> void:
    pass
#    if abs(get_linear_velocity().x) > max_speed or abs(get_linear_velocity().y) > max_speed:
#        var new_speed = get_linear_velocity().normalized()
#        new_speed *= max_speed
#        call_deferred("set_linear_velocity", new_speed)


    # Prevent ball from leaving the viewport
#    var boundaries: Vector2 = get_viewport_rect().size
#    if position.x <= -10 or position.x >= boundaries.x + 10 or position.y <= -10  or position.y >= boundaries.y + 10:
#        main.reset_ball()

#    if trigger_next_generation:
#        main.next_generation()
#        trigger_next_generation = false
#        cycles_count = 0


func _on_Ball_body_entered(body: Node) -> void:
#    $Particles2D.emitting = true
#    var velocity_normal: Vector2 = get_linear_velocity().normalized()
#    ($Particles2D.process_material as ParticlesMaterial).direction = Vector3(-velocity_normal.x, -velocity_normal.y, 0)

    call_deferred("body_entered", body)


func _on_Ball_body_exited(body: Node) -> void:
    call_deferred("body_exited", body)


func body_entered(body: Node) -> void:
    if body.is_in_group("paddles"):
        if harmful:
            body.penalize_ball_hit()
        else:
            body.reward_ball_hit()

        selection.append(body)
    elif body.is_in_group("harmful_boundaries"):
        main.reward_all_paddles()

        toggle_harmful()

        if harmful:
            main.next_generation()
        else:
            main.reset_ball()

func body_exited(body: Node) -> void:
    if body.is_in_group("paddles") and selection.size() > 0:
        main.reward_all_paddles()
        if !harmful:
            main.kill_except(selection)
        else:
            main.kill(selection)

        cycles_count += 1

        if cycles_count >= 2:
            if !harmful:
                print("Cycles count exhausted (first stage)")
                toggle_harmful()
                cycles_count = 0
                main.revive_dead_paddles()
            else:
                print("Cycles count exhausted (final stage)")
                main.next_generation()
                toggle_harmful()

        selection = []
