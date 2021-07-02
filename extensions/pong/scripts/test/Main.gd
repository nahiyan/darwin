extends Node


func _ready() -> void:
    var count = PersistentModels.count()
    for i in range(min(count, 1)):
        var model = PersistentModels.model(i)
        var definition:String = model[0]

        Neat.model_from_string(i, definition)


func _on_Main_tree_exited() -> void:
    pass
