extends RigidBody2D

var score: float = 0
onready var bird_start_x: float = position.x - 30

func _ready() -> void:
    pass

func _process(delta: float) -> void:
    $AnimatedSprite.play()
    score += delta

#    Prelude: Find the nearest pipe infront of the bird (focused pipe)
    var pipes: Array = get_tree().get_nodes_in_group("pipes")
    var focused_pipe: Node2D = null

    for pipe in pipes:
        var pipe_end_x: float = pipe.position.x + 109

        if bird_start_x <= pipe_end_x:
            focused_pipe = pipe
            break

#    1 -> Horizontal distance to pipes' column
#    position.x + 30 - focused_pipe.position.x

#    2 -> Vertical distance to top pipe's edge
#    3 -> Vertical distance to bottom pipe's edge

#    4 -> Vertical velocity
#    linear_velocity.y

func _input(event: InputEvent) -> void:
    if event.is_action_pressed("flap"):
        apply_impulse(Vector2.ZERO, Vector2(0, -300))

func _on_Player_body_entered(_body: Node) -> void:
    kill()

func kill():
    queue_free()
