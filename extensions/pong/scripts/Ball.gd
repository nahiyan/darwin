extends RigidBody2D

var max_speed = 424

func _process(_delta: float) -> void:
    if abs(get_linear_velocity().x) > max_speed or abs(get_linear_velocity().y) > max_speed:
        var new_speed = get_linear_velocity().normalized()
        new_speed *= max_speed
        set_linear_velocity(new_speed)


func _on_Ball_body_entered(_body: Node) -> void:
    $Particles2D.emitting = true
    var velocity_normal: Vector2 = get_linear_velocity().normalized()
    ($Particles2D.process_material as ParticlesMaterial).direction = Vector3(-velocity_normal.x, -velocity_normal.y, 0)
