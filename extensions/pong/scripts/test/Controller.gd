extends Node


func _ready() -> void:
    var count = PersistentModels.count()

    if count == 0:
        print("No saved model found.")
        return

    # Load the model
    for i in range(min(count, Global.configuration.test_models_count)):
        var model = PersistentModels.model(i)
        var definition:String = model[0]

        Neat.model_from_string(i, definition)

    # Add the ball
    var stage = $Stages/Stage1
    var ball = stage.get_node("Ball")
    ball.stage = stage
    ball.reset()
    ball.disconnect("body_entered", ball, "_on_Ball_body_entered")
    ball.disconnect("body_exited", ball, "_on_Ball_body_exited")
    ball.connect("body_entered", ball, "_on_Ball_body_entered_test_mode")

    # Add the paddle
    var paddle_scene = preload("res://scenes/Paddle.tscn")
    for i in range(min(count, Global.configuration.test_models_count)):
        var paddle: Node2D = paddle_scene.instance()
        paddle.id = i
        stage.add_child(paddle)
        paddle.reposition(Global.CENTER)


func _on_Main_tree_exited() -> void:
    pass
