extends Node3D

var hex_piece: PackedScene=preload("res://Assets/hexagon.tscn")
var hex_list: Array=[]

@onready
var thing_autoloaded: Node=get_node("/root/piece_selected")
# Called when the node enters the scene tree for the first time.
func _ready():
	#var num_rotations: int=0 #there are 5 total rotations of hexes around the first hex in order to build the board.
	#while num_rotations<5:
	#	num_rotations+=1
		#we start with the hexagon at (0, 0, -4*sqrt(3)/2)
											# y      y       y        y     y        y       y      y        y         y        y        y        y        y       y          y         y           y         y           y      y      y         y    
	var white_player_pieces: Array[Node3D]=[$pawn, $pawn2, $pawn3, $pawn4, $pawn5, $pawn6, $pawn7, $pawn21, $pawn22, $pawn23, $pawn24, $pawn25, $pawn26, $knight, $knight2, $bishop, $bishop2, $bishop9, $bishop10, $bishop8, $rook, $rook2, $king_white]
											#  y       y       y        y        y        y        y        y       y        y         y        y         y     y           y          y          y        y        y          y        y       y          y                                
	var black_player_pieces: Array[Node3D]=[$pawn8, $pawn9, $pawn10, $pawn11, $pawn12, $pawn13, $pawn14, $pawn15, $pawn16, $pawn17, $pawn18, $pawn19, $pawn20, $knight3, $knight4, $bishop3, $bishop4, $bishop5, $bishop6, $bishop7, $rook3, $rook4, $king_black]
	
	
	#now we need to make sure each of the pieces listed above appears at the exact center_triangle of one of the triangles on the board.
	#first I need to create an array of each center_triangle
	var center_triangle_list: Array[Node3D]=[]
	for child in get_children():
		if str(child.name).begins_with("Hex"):
			#var triangle_bases: Array[Node3D]=[]
			for hexchild in child.get_children():
				if str(hexchild.name).begins_with("tri"): #this is one of the 6 triangles.
					for trichild in hexchild.get_children():
						if str(trichild.name).begins_with("cen"):
							var center_triangle: Node3D=trichild
							center_triangle_list.append(center_triangle)
	
	#second I need to, for each piece, find the closest center_triangle to it, then move that piece to that center_triangle
	for piece in white_player_pieces:
		if piece!=null:
			var closesttri: Node3D=center_triangle_list[0]
			var closestdist: float=100
			for tri in center_triangle_list:
				var tempd: float=(tri.global_position-piece.global_position).length()
				if tempd<closestdist:
					closestdist=tempd #
					closesttri=tri #the newly found closest triangle center
			piece.global_translate(closesttri.global_position-piece.get_second_global_position()) #the get_second_global_position updates immediately for every use of global_translate
	for piece in black_player_pieces:
		if piece!=null:
			var closesttri: Node3D=center_triangle_list[0]
			var closestdist: float=100
			for tri in center_triangle_list:
				var tempd: float=(tri.global_position-piece.global_position).length()
				if tempd<closestdist:
					closestdist=tempd #
					closesttri=tri #the newly found closest triangle center
			piece.global_translate(closesttri.global_position-piece.get_second_global_position()) #the get_second_global_position updates immediately for every use of global_translate
	
	thing_autoloaded.white_player_pieces.append_array(white_player_pieces)
	thing_autoloaded.black_player_pieces.append_array(black_player_pieces)
	
	if thing_autoloaded.is_black:
		$opponent_is_light.my_king_start=$king_white.global_position
		$opponent_is_light.human_king_start=$king_black.global_position
	else:
		$opponent_is_light.my_king_start=$king_black.global_position
		$opponent_is_light.human_king_start=$king_white.global_position
	#if $white_player_camera.current:
	#	thing_autoloaded.is_black=false
	#elif $black_player_camera.current:
	#	thing_autoloaded.is_black=true #
	#The above lines of code were based on the assumption that the 
	#way the player color could be chosen is to choose which camera is made current
	#that assumption is wrong for my actual players, who need to start in 
	#a separate 2D "main menu" scene, whose main ability to save information for use in the 
	#next scene is to save it to the 
	#thing_autoloaded
	#therefore, the thing_autoloaded script needs to take ultimate authority for 
	#deciding the color of the pieces.
	if thing_autoloaded.is_black==true:
		$black_player_camera.make_current()
	elif thing_autoloaded.is_black==false:
		$white_player_camera.make_current()
	pass # Replace with function body.

var current_time: float=0
var made_read_only: bool=false #after some time after the board is loaded, there are a bunch of tables that need to be made read_only in thing_autoloaded
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	current_time+=delta
	if not made_read_only and current_time>3 :
		made_read_only=true
		thing_autoloaded.white_player_pieces.make_read_only()
		thing_autoloaded.black_player_pieces.make_read_only()
		thing_autoloaded.possible_positions.make_read_only()
