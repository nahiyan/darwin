extends RigidBody2D

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
    pass


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
    pass

func _input(event: InputEvent) -> void:
    if event.is_action_pressed("flap"):
        apply_impulse(Vector2.ZERO, Vector2(0, -300))

func _on_Player_body_entered(_body: Node) -> void:
    kill()

func kill():
    queue_free()
