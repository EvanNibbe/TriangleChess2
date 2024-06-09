extends OmniLight3D

var current_time: float=0 #this script will not have _process(delta), and so needs to rely on the autoloaded script for finding TIME
var my_prev_change_time: float=0
var human_change_time: float=3 #when human_change_time>my_prev_change_time and everything is already set up and human_change_time<thing_autoloaded.current_time, then play my move.
var thing_autoloaded: Node=null #
var possible_positions: Array=[] #Vector3 #will take the values of all $center_triangle.get_second_global_position() after 1 second of gameplay has elapsed
	#(to give time for all the other _ready() functions to execute)
var my_pieces: Array=[] #Node3D
var human_pieces: Array=[] #Node3D
var my_king_start: Vector3=Vector3()
var human_king_start: Vector3=Vector3()



# Called when the node enters the scene tree for the first time.
func _ready():
	thing_autoloaded=get_node("/root/piece_selected")
	thing_autoloaded.set_AI(self)
	pass # Replace with function body.



var nth_optimal: int=0


#this function is what is called by the thing_autoloaded (which_selected.gd) script, it is called every time a player actually moves a piece.
func make_move()->Array: #this function uses the thing_autoloaded.ray_cast(start: Vector3, end: Vector3) function
	if abs(current_time-thing_autoloaded.current_time)>.01:
		nth_optimal=0
	else:
		nth_optimal+=1 #if the time value has not changed, then that means the game has not progressed, meaning that a different solution should have been tried 
		#by fast_make_move while it was looking for possible moves.
	current_time=thing_autoloaded.current_time
	var result: Array=[null, Vector3.ZERO] #the piece_to_move and the global_pos to move it
	if possible_positions.size()<10: #this code operates exactly once.
		for pos in thing_autoloaded.possible_positions:
			possible_positions.append(pos)
		if thing_autoloaded.is_black: #means that the human is black, so my_pieces is the white_player_pieces
			for piece in thing_autoloaded.white_player_pieces:
				my_pieces.append(piece)
				if str(piece.name).contains("in"):
					my_king_start=piece.global_position
			for piece in thing_autoloaded.black_player_pieces:
				human_pieces.append(piece)
				if str(piece.name).contains("in"):
					human_king_start=piece.global_position
			
		else: #means the human is white, so my_pieces is the black_player_pieces
			for piece in thing_autoloaded.white_player_pieces:
				human_pieces.append(piece)
				if str(piece.name).contains("in"): #this code seemed to have been messed up, or just didn't run, from what I can see in the debugger
					human_king_start=piece.global_position
			for piece in thing_autoloaded.black_player_pieces:
				my_pieces.append(piece)
				if str(piece.name).contains("in"):
					my_king_start=piece.global_position
	#The fast_make_move function itself just makes the move and returns the value of the position.
	#the make_move function here needs to have the board be in the same place that it was
	#so we below will store the current positions of all the pieces, and figure out which piece was moved
	#then pass that moved piece back to the calling code.
	#we don't actually need to restore any piece positions, as the calling code does that already.
	#var my_pieces_were: Dictionary={}
	#for ch in my_pieces:
	#	var piece: Node3D=ch
	#	my_pieces_were[piece]=(piece.get_second_global_position())
	var remmy: int=-1
	var remhu: int=-1
	var i0: int=-1
	for i in range(len(my_pieces)):
		i0+=1
		if my_pieces[i0].get_second_global_position().y<-10:
			remmy=i0 #
	i0=-1
	for i in range(len(human_pieces)):
		i0+=1
		if human_pieces[i0].get_second_global_position().y<-10:
			remhu=i0
	if remmy>-1:
		my_pieces.remove_at(remmy)
	if remhu>-1:
		human_pieces.remove_at(remhu)
	
	var piece_to_move: Node3D=fast_make_move(my_pieces, my_king_start.x, my_king_start.y, my_king_start.z, human_pieces, human_king_start.x, human_king_start.y, human_king_start.z, possible_positions, nth_optimal, 0)
	print("The AI's value from the move is ", piece_to_move)
	result[0]=piece_to_move
	result[1]=piece_to_move.get_place_loc()
	#for ch in my_pieces:
	#	var piece: Node3D=ch
	#	if (my_pieces_were[piece]-piece.get_second_global_position()).length_squared()>1/3:
	#		result[0]=piece
	#		result[1]=piece.get_second_global_position()
	#		break
	print("AI move was ", result)
	return result #set above as the Array[piece_to_move: Node3D, where_to_move_it: Vector3]



