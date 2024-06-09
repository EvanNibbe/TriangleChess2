extends StaticBody3D


# Called when the node enters the scene tree for the first time.
func _ready():
	var thing_autoloaded: Node=get_node("/root/piece_selected")
	thing_autoloaded.add_rook_defender(self)
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
