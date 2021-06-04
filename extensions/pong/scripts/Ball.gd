extends RigidBody2D

var max_speed = 424

func _process(delta: float) -> void:
    if abs(get_linear_velocity().x) > max_speed or abs(get_linear_velocity().y) > max_speed:
        var new_speed = get_linear_velocity().normalized()
        new_speed *= max_speed
        set_linear_velocity(new_speed)
