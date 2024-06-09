extends StaticBody3D

var current_time: float=0
var is_clicked_v: bool=false #
var when_clicked_v: float=-10
var is_black_v: bool=false
var has_improved: bool=false #once a pawn reaches the opposite end (following the z axis) from where it started, and reaches either z<=-9*sqrt(3) or z>=9*sqrt(3)
	#then it gains the power to move to any spot within 2*sqrt(5) of its current location.
var start_pos: Vector3=Vector3()
var all_hashed_positions: Dictionary={}
var time_of_check: float=-1


var thing_autoloaded: Node=null

# Called when the node enters the scene tree for the first time.
func _ready():
	thing_autoloaded=get_node("/root/piece_selected")
	if get_second_global_position().z<0:
		$MeshInstance3D.set_instance_shader_parameter("black", true)
		is_black_v=true #black always starts on the -z side.
	pass # Replace with function body.

var dead: bool=false
func hash_pos(global_pos: Vector3, redblackbrown: bool)->int:
	var result: int=0
	if not dead:
		#only counting things that are outside the dead space.
		result+=int(((global_pos.x+thing_autoloaded.minX)*(thing_autoloaded.maxZ-thing_autoloaded.minZ)+global_pos.z-thing_autoloaded.minZ)*2) #should be a strictly positive integer result
		result*=2 #the least significant bit of the integer is only 1 if self_capture is true (this differentiates spots based on all the initial conditions of the calculation without cross-corruption)
		if redblackbrown:
			result+=1
	return result

func is_on_red_black_brown()->bool:
	if dead:
		return false
	var result: bool=false #
	var txtbk_coord: Vector2=Vector2(get_second_global_position().x, -get_second_global_position().z+13*sqrt(3)+.0001) #all positions on the right side of the board are in the 1st quadrant of
	#geometry textbook space.
	var careful_mod: int =int(txtbk_coord.y*3/sqrt(3)+.5) #the +.5 should stop the rounding error encountered when a white pawn attacks a black pawn.
	#if this doesn't work, then I will need to switch over to the 
	#update, the +.5 does work.
	
	if careful_mod%3==1:
		result=true #
	elif careful_mod%3==2:
		result=false
	else: #something went wrong, my calculation is out of whack
		print("something went wrong, pawn "+str(name)+" is at geometry textbook coordinate "+str(txtbk_coord)+" where the y*sqrt(3)/3 mod 3 equals 0, which is not supposed to be true. (it would mean the center of a triangle aligning with the bottom edge of another triangle, or with the tip)")
	return result #

func can_move_to(global_pos: Vector3, self_capture: bool=false)->bool:
	if (dead or get_second_global_position().y<-10) and (global_pos.y>-10):
		return false
	if global_pos.y<-10:
		return true
	
	var result: bool=false
	var fast_result: bool=false
	if not has_improved:
		if (all_hashed_positions.has((hash_pos(global_pos-get_second_global_position(), is_on_red_black_brown())))):
			fast_result=true
			result=all_hashed_positions[hash_pos(global_pos-get_second_global_position(), is_on_red_black_brown())]
		if result and self_capture:
			return true #this is a valid spot to move to generally, and we don't care if our own piece is there.
		elif fast_result and result==false:
			return false #even if self-capture is allowed, the piece can't move there.
		elif fast_result and result==true and self_capture==false:
			#need to check for whether the spot is within sqrt(3)/3 of another piece of the same color.
			var piece_near: Node3D=thing_autoloaded.piece_near(global_pos)
			if piece_near!=null and piece_near.name!=name and piece_near.has_method("is_black") and piece_near.is_black()==is_black() and (piece_near.get_second_global_position()-global_pos).length_squared()<1/3:
				return false #there is a piece of the same color as this piece (and it is not this piece) at the location you are trying to move to.
			else:
				return true
	
	var textbook_coord: Vector2=Vector2(global_pos.x-get_second_global_position().x, -(global_pos.z-get_second_global_position().z)) #this is putting the center of the current pawn at effectively (0,0) w.r.t. the places that can be moved to 
	#how a geometry textbook represents the values for the lines I drew in my notes. 
	#As long as the board triangles remain in the same size and orientation,
	#I can figure out whether a triangle is of one of two sets ({red, black, brown} or {blue, green, white}) by using a very careful mod function from
	#a geometry textbook origin that, in Godot space, would be at (0, 0, 13*sqrt(3)), but in geometry textbook space could be considered
	#as set to (0, -13*sqrt(3)) relative to the center of the board (so that the board shows up in the 1st quadrant of the XY plane on paper).
	if has_improved:
		if (global_pos-get_second_global_position()).length()<=sqrt(5)*2:
			result=true
	elif not is_black_v:
		if is_on_red_black_brown():
			var places_to_go: Array[Vector2]=[Vector2(-1, sqrt(3)/3), Vector2(-1, sqrt(3)), Vector2(0, 4*sqrt(3)/3), Vector2(1, sqrt(3)), Vector2(1, sqrt(3)/3)]
			for i in places_to_go:
				if (i-textbook_coord).length()<sqrt(3)/3:
					result=true #
					break
		else :
			var places_to_go: Array[Vector2]=[Vector2(-2, 2*sqrt(3)/3), Vector2(-1, sqrt(3)), Vector2(0, 2*sqrt(3)/3), Vector2(1, sqrt(3)), Vector2(1, 2*sqrt(3)/3), Vector2(2, 2*sqrt(3)/3)]
			for i in places_to_go:
				if (i-textbook_coord).length()<sqrt(3)/3:
					result=true #
					break
			#if not result and (global_pos-get_second_global_position()).length()<=4*sqrt(3)/3+sqrt(3)/6 and Vector3.FORWARD.dot(global_pos-get_second_global_position())>0: #there was an edge case of the kiddie-corner move not being recognized at one end
			#	result=true
	elif is_black_v:
		if not is_on_red_black_brown(): #everything "forward" is reversed for black, including the idea of red, black, brown pointing "forward", which they no longer do.
			var places_to_go: Array[Vector2]=[Vector2(-1, -sqrt(3)/3), Vector2(-1, -sqrt(3)), Vector2(0, -4*sqrt(3)/3), Vector2(1, -sqrt(3)), Vector2(1, -sqrt(3)/3)]
			for i in places_to_go:
				if (i-textbook_coord).length()<sqrt(3)/3:
					result=true #
					break
		else :
			var places_to_go: Array[Vector2]=[Vector2(-2, -2*sqrt(3)/3), Vector2(-1, -sqrt(3)), Vector2(0, -2*sqrt(3)/3), Vector2(1, -sqrt(3)), Vector2(1, -2*sqrt(3)/3), Vector2(2, -2*sqrt(3)/3)]
			for i in places_to_go:
				if (i-textbook_coord).length()<sqrt(3)/3:
					result=true #
					break
			#if not result and (global_pos-get_second_global_position()).length()<=4*sqrt(3)/3+sqrt(3)/6 and Vector3.BACK.dot(global_pos-get_second_global_position())>0: #there was an edge case of the kiddie-corner move not being recognized at one end
			#	result=true
		
	all_hashed_positions[hash_pos(global_pos-get_second_global_position(), is_on_red_black_brown())]=result #this dictionary assumes self-capture is permitted.
	#note: we only need to have the dictionary consider relative moves, not absolute moves.
	if result and not self_capture:
		#check if the position is shared by a piece of the same color.
		var piece_near: Node3D=thing_autoloaded.piece_near(global_pos)
		if piece_near!=null and piece_near.name!=name and piece_near.has_method("is_black") and piece_near.is_black()==is_black() and (piece_near.get_second_global_position()-global_pos).length_squared()<1/3:
			return false #there is a piece of the same color as this piece (and it is not this piece) at the location you are trying to move to.
		else:
			return true
	return result



# Called every frame. 'delta' is the elapsed time since the previous frame.
var previous_pos: Vector3=Vector3()
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	current_time+=delta
	if current_time<1:
		previous_pos=get_second_global_position()
	elif previous_pos.y>-100:
		if (previous_pos-get_second_global_position()).length_squared()>1/3:
			var present_pos: Vector3=get_second_global_position()
			global_translate(previous_pos-present_pos)
			if can_move_to(present_pos):
				global_translate(present_pos-previous_pos)
				previous_pos=present_pos
			else:
				var piece_near: Node3D=thing_autoloaded.search_all_pieces(previous_pos)
				if (piece_near.has_method("is_black") and piece_near.is_black()!=is_black()) and (piece_near.get_second_global_position()-previous_pos).length_squared()<1/3:
					global_translate(Vector3.DOWN*10000)
					previous_pos=get_second_global_position() #the elif clause above thus stops this piece from being checked anymore.
				else:
					pass #there's no piece at the previous spot, so no need for this piece to die.
					#global_translate(previous_pos-present_pos) #already done farther up.
	if is_black_v and get_second_global_position().z>9*sqrt(3):
		has_improved=true
	elif not is_black_v and get_second_global_position().z<-9*sqrt(3):
		has_improved=true
	if is_clicked_v:
		
		if thing_autoloaded.any_move_to_position:
			print("pawn "+str(name)+" told to move to "+str(thing_autoloaded.move_to_position)+" from "+str(get_second_global_position()))
			#global_translate(thing_autoloaded.move_to_position-get_second_global_position())
			#thing_autoloaded.any_move_to_position=false
			#thing_autoloaded.any_piece_selected=false #
			#thing_autoloaded.move_to_position=Vector3()
			is_clicked_v=false #
			when_clicked_v=-10
			#thing_autoloaded.piece_selected=null #
	if get_second_global_position().y<-100:
		dead=true #
	elif get_second_global_position().y>-5 and get_second_global_position().y<5 and int(current_time+delta)>int(current_time):
		global_translate(Vector3(0, .06-get_second_global_position().y, 0))
	

func click():
	if not is_clicked_v:
		#thing_autoloaded.piece_selected=self #
		#thing_autoloaded.any_piece_selected=true #
		is_clicked_v=thing_autoloaded.piece_clicked(self)
		if is_clicked_v:
			when_clicked_v=current_time
			print("pawn "+str(name)+" selected at "+str(get_second_global_position()))
		
	
func unclick(): #where you click the same piece again in order to return to the starting options.
	
	if is_clicked_v and when_clicked_v+.1<current_time:
		when_clicked_v=-10
		is_clicked_v=false
		RenderingServer.global_shader_parameter_set("any_piece_selected", false)
		#var thing: Node=get_node("/root/piece_selected")
		thing_autoloaded.any_piece_selected=false
	elif not is_clicked_v:
		when_clicked_v=-10

func is_chosen()->bool:
	return is_clicked_v

func when_chosen()->float:
	return when_clicked_v



func is_black()->bool:
	return is_black_v
