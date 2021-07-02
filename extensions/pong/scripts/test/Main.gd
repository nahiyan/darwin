extends Node


func _ready() -> void:
    var count = PersistentModels.count()

    if count == 0:
        print("No saved model found.")

    # Load the model
    for i in range(min(count, 1)):
        var model = PersistentModels.model(i)
        var definition:String = model[0]

        Neat.model_from_string(i, definition)

    # Add the ball
    var ball_scene = preload("res://scenes/Ball.tscn")
    var stage = $Stages/Stage1
    var ball = ball_scene.instance()
    ball.stage = stage
    ball.reset()
    stage.add_ball(ball)

    # Add the paddle
    var paddle_scene = preload("res://scenes/Paddle.tscn")
    var paddle: Node2D = paddle_scene.instance()
    paddle.id = 0
    stage.add_child(paddle)
    paddle.reposition(Global.CENTER)


func _on_Main_tree_exited() -> void:
    pass
