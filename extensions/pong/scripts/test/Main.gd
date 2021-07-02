extends Node


func _ready() -> void:
    var count = PersistentModels.count()

    if count == 0:
        print("No saved model found.")
        return

    # Load the model
    var model = PersistentModels.model(0)
    var definition:String = model[0]
    Neat.model_from_string(0, definition)

    # Add the ball
    var stage = $Stages/Stage1
    var ball = stage.get_node("Ball")
    ball.stage = stage
    ball.reset()
    ball.disconnect("body_entered", ball, "_on_Ball_body_entered")
    ball.disconnect("body_exited", ball, "_on_Ball_body_exited")
    ball.connect("body_exited", ball, "_on_Ball_body_exited_test_mode")

    # Add the paddle
    var paddle = stage.get_node("Paddle")
    paddle.id = 0
    paddle.reposition(Global.CENTER)


func _on_Main_tree_exited() -> void:
    pass
