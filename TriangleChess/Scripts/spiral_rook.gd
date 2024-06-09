extends StaticBody3D
#Note: this script was copied from bishop.gd, so needed to use the pawn.gd method of finding whether the current triangle it is on is pointing up or down.

var current_time: float=0
var is_chosen_v: bool=false 
var when_chosen_v: float=-10
var is_black_v: bool=false
var material: GeometryInstance3D=null
var start_pos: Vector3=Vector3()
var was_moving: bool=false
var has_moved: bool=false

var all_hashed_positions: Dictionary={}
var time_of_check: float=-1
var on_red_black_brown: bool=false

var thing_autoloaded: Node=null
var pos_theta: bool=true #determined by the x coordinate at _ready() 
	#this defines whether the equation used is r=1.1^(theta) or r=1.1^(-theta)
	#in determining where this piece can go.

# Called when the node enters the scene tree for the first time.
func _ready():
	material=$MeshInstance3D
	if get_second_global_position().z<0:
		is_black_v=true
		material.set_instance_shader_parameter("black", true)
		$rook_defender/MeshInstance3D.set_instance_shader_parameter("black", true)
	else:
		is_black_v=false
		material.set_instance_shader_parameter("black", false)
		$rook_defender/MeshInstance3D.set_instance_shader_parameter("black", false)
	if get_second_global_position().x<0:
		pos_theta=false #
	else:
		pos_theta=true
	
	current_time=0
	is_chosen_v=false 
	thing_autoloaded=get_node("/root/piece_selected")
	pass # Replace with function body.

func is_chosen()->bool:
	return is_chosen_v

func is_black()->bool:
	return is_black_v

func unclick(): #where you click the same piece again in order to return to the starting options.
	if is_chosen_v and when_chosen_v+.1<current_time:
		when_chosen_v=-10
		is_chosen_v=false
		RenderingServer.global_shader_parameter_set("any_piece_selected", false)
		#var thing_autoloaded: Node=get_node("/root/piece_selected")
		thing_autoloaded.any_piece_selected=false
	elif not is_chosen_v:
		when_chosen_v=-10

func click():
	when_chosen_v=current_time
	if is_chosen_v==false:
		
		#var thing_autoloaded: Node=get_node("/root/piece_selected")
		is_chosen_v=thing_autoloaded.piece_clicked(self)
		#thing_autoloaded.piece_selected=self
		#thing_autoloaded.any_piece_selected=true
		thing_autoloaded.any_move_to_position=false #need to make sure that it doesn't start moving before the user chooses where to put it.
		#var really_is: bool=get_node("/root/any_piece_selected")
	#is_chosen_v=true #

func when_chosen()->float:
	return when_chosen_v
	
var dead: bool=false
func hash_pos(global_pos: Vector3, self_capture: bool=false)->int:
	var result: int=0
	if not dead:
		#only counting things that are outside the dead space.
		result+=int(((global_pos.x+thing_autoloaded.minX)*(thing_autoloaded.maxZ-thing_autoloaded.minZ)+global_pos.z-thing_autoloaded.minZ)*8) #should be a strictly positive integer result
		#result*=2 #the least significant bit of the integer is only 1 if self_capture is true (this differentiates spots based on all the initial conditions of the calculation without cross-corruption)
		
	return result
	
func can_move_to(global_pos1: Vector3, self_capture: bool=false)->bool:
	if (dead or get_second_global_position().y<-10) and (global_pos1.y>-10):
		return false
	if global_pos1.y<-10:
		return true
	
	var result: bool=false
	var fast_result: bool=false
	
	if (all_hashed_positions.has((hash_pos(global_pos1-get_second_global_position())))):
		fast_result=true
		result=all_hashed_positions[hash_pos(global_pos1-get_second_global_position())]
	if result and self_capture:
		return true #this is a valid spot to move to generally, and we don't care if our own piece is there.
	elif fast_result and result==false:
		return false #even if self-capture is allowed, the piece can't move there.
	elif fast_result and result==true and self_capture==false:
		#need to check for whether the spot is within sqrt(3)/3 of another piece of the same color.
		var piece_near: Node3D=thing_autoloaded.piece_near(global_pos1)
		if piece_near!=null and piece_near.name!=name and piece_near.has_method("is_black") and piece_near.is_black()==is_black() and (piece_near.get_second_global_position()-global_pos1).length_squared()<1/3:
			return false #there is a piece of the same color as this piece (and it is not this piece) at the location you are trying to move to.
		else:
			return true
			
	
	var global_pos: Vector3=global_pos1- get_second_global_position() #the relative vector from this triangle to that spot.
	#note: I need to check that this direction makes sense in light of the points of the triangle (that the line is not bi-directional).
	var equation_pos: Vector2=Vector2(global_pos.x, -global_pos.z) #this is the appearance of the x-y plane in 2D in a geometry textbook.
	
	#check that the direction of the vector makes sense:
	var exists_a_dir: bool=(equation_pos.length()>sqrt(3)/3)
	if exists_a_dir:
		var r: float=global_pos.length()
		if pos_theta:
			#use r=1.1^theta
			var theta: float=log(r)/log(1.1)
			var pos: Vector3=Vector3(r*cos(theta), 0.06, r*sin(theta))
			if (pos-global_pos).length()<2*sqrt(3)/3:
				result=true #
			else:
				var tl: float=theta-pos.angle_to(global_pos)*2 #both pos's are in the same relative coordinate system, so this works, the angle is doubled just to make a good "high" value for the binary search
				var rl: float=pow(1.1, tl)
				var pos_l: Vector3=Vector3(rl*cos(tl), 0.06, rl*sin(tl))
				var theta_right: float=theta+pos.angle_to(global_pos)*2 #the angle is doubled just to make a good "low" value for a binary search.
				var rr: float=pow(1.1, theta_right)
				var pos_r: Vector3=Vector3(rr*cos(theta_right), 0.06, rr*sin(theta_right))
				var lel: float=(pos_l-global_pos).length() #length of l to where the person chose
				var ler: float=(pos_r-global_pos).length()
				if lel<sqrt(3)/3:
					result=true #
				elif ler<sqrt(3)/3:
					result=true #
				elif lel<ler:
					theta_right=(theta_right*4+tl)/5 #not exactly a binary seach method, as it is a smaller change in the low (normally it's low=(low+high)/2) 
						#but the difference is that the curve is exponential, so I want to avoid accidentally skipping a good value
					rr=pow(1.1, theta_right)
					pos_r=Vector3(rr*cos(theta_right), 0.06, rr*sin(theta_right))
					ler=(pos_r-global_pos).length()
				elif ler<lel:
					tl=(tl*4+theta_right)/5
					rl=pow(1.1, tl)
					pos_l=Vector3(rl*cos(tl), 0.06, rl*sin(tl))
					lel=(pos_l-global_pos).length()
				while lel>sqrt(3)/3 and ler>sqrt(3)/3 and abs(theta_right-tl)>.005 and lel!=ler: #the last condition should never be false, since these are floating point numbers generated distinctly, so lel==ler can almost be guaranteed to be false
					if lel<ler:
						theta_right=(theta_right*4+tl)/5 #not exactly a binary seach method, as it is a smaller change in the low (normally it's low=(low+high)/2) 
							#but the difference is that the curve is exponential, so I want to avoid accidentally skipping a good value
						rr=pow(1.1, theta_right)
						pos_r=Vector3(rr*cos(theta_right), 0.06, rr*sin(theta_right))
						ler=(pos_r-global_pos).length()
					elif ler<lel:
						tl=(tl*4+theta_right)/5
						rl=pow(1.1, tl)
						pos_l=Vector3(rl*cos(tl), 0.06, rl*sin(tl))
						lel=(pos_l-global_pos).length()
				if lel<sqrt(3)/3:
					result=true #
				elif ler<sqrt(3)/3:
					result=true #
				#the linearization method
				#var left_t: Vector3=Vector3.DOWN.cross(pos).normalized()
				#var right_theory: Vector3=-left_t+pos #
				#var left_theory: Vector3=left_t+pos #
				#if (left_theory-global_pos).length()<2*sqrt(3)/3 or (right_theory-global_pos).length()<2*sqrt(3)/3:
				#	result=true #the linearization is based on the "locally circular" nature of a spiral
				#else:
				#	right_theory=right_theory-left_t #2 moves in a line.
				#	left_theory=left_theory+left_t #2 moves the other direction.
				#	if (left_theory-global_pos).length()<2*sqrt(3)/3 or (right_theory-global_pos).length()<2*sqrt(3)/3:
				#		result=true
		else:
			#use r=1.1^(-theta)
			var theta: float=-log(r)/log(1.1)
			var pos: Vector3=Vector3(r*cos(theta), 0.06, r*sin(theta))
			if (pos-global_pos).length()<2*sqrt(3)/3:
				result=true #
			else:
				var tl: float=theta-pos.angle_to(global_pos)*2 #both pos's are in the same relative coordinate system, so this works, the angle is doubled just to make a good "high" value for the binary search
				var rl: float=pow(1.1, -tl)
				var pos_l: Vector3=Vector3(rl*cos(tl), 0.06, rl*sin(tl))
				var theta_right: float=theta+pos.angle_to(global_pos)*2 #the angle is doubled just to make a good "low" value for a binary search.
				var rr: float=pow(1.1, -theta_right)
				var pos_r: Vector3=Vector3(rr*cos(theta_right), 0.06, rr*sin(theta_right))
				var lel: float=(pos_l-global_pos).length() #length of l to where the person chose
				var ler: float=(pos_r-global_pos).length()
				if lel<sqrt(3)/3:
					result=true #
				elif ler<sqrt(3)/3:
					result=true #
				elif lel<ler:
					theta_right=(theta_right*4+tl)/5 #not exactly a binary seach method, as it is a smaller change in the low (normally it's low=(low+high)/2) 
						#but the difference is that the curve is exponential, so I want to avoid accidentally skipping a good value
					rr=pow(1.1, -theta_right)
					pos_r=Vector3(rr*cos(theta_right), 0.06, rr*sin(theta_right))
					ler=(pos_r-global_pos).length()
				elif ler<lel:
					tl=(tl*4+theta_right)/5
					rl=pow(1.1, -tl)
					pos_l=Vector3(rl*cos(tl), 0.06, rl*sin(tl))
					lel=(pos_l-global_pos).length()
				while lel>sqrt(3)/3 and ler>sqrt(3)/3 and abs(theta_right-tl)>.005 and lel!=ler: #the last condition should never be false, since these are floating point numbers generated distinctly, so lel==ler can almost be guaranteed to be false
					if lel<ler:
						theta_right=(theta_right*4+tl)/5 #not exactly a binary seach method, as it is a smaller change in the low (normally it's low=(low+high)/2) 
							#but the difference is that the curve is exponential, so I want to avoid accidentally skipping a good value
						rr=pow(1.1, -theta_right)
						pos_r=Vector3(rr*cos(theta_right), 0.06, rr*sin(theta_right))
						ler=(pos_r-global_pos).length()
					elif ler<lel:
						tl=(tl*4+theta_right)/5
						rl=pow(1.1, -tl)
						pos_l=Vector3(rl*cos(tl), 0.06, rl*sin(tl))
						lel=(pos_l-global_pos).length()
				if lel<sqrt(3)/3:
					result=true #
				elif ler<sqrt(3)/3:
					result=true #
				#the linearization method
				#var left_t: Vector3=Vector3.DOWN.cross(pos).normalized()
				#var right_theory: Vector3=-left_t+pos #
				#var left_theory: Vector3=left_t+pos #
				#if (left_theory-global_pos).length()<2*sqrt(3)/3 or (right_theory-global_pos).length()<2*sqrt(3)/3:
				#	result=true #the linearization is based on the "locally circular" nature of a spiral
				#else:
				#	right_theory=right_theory-left_t #2 moves in a line.
				#	left_theory=left_theory+left_t #2 moves the other direction.
				#	if (left_theory-global_pos).length()<2*sqrt(3)/3 or (right_theory-global_pos).length()<2*sqrt(3)/3:
				#		result=true
			
			
	all_hashed_positions[hash_pos(global_pos1-get_second_global_position())]=result #this dictionary assumes self-capture is permitted.
	#note: we only need to have the dictionary consider relative moves, not absolute moves.
	if result and not self_capture:
		#check if the position is shared by a piece of the same color.
		var piece_near: Node3D=thing_autoloaded.piece_near(global_pos1)
		if piece_near!=null and piece_near.name!=name and piece_near.has_method("is_black") and piece_near.is_black()==is_black() and (piece_near.get_second_global_position()-global_pos1).length_squared()<1/3:
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
	if get_second_global_position().y<-100:
		dead=true #
	elif get_second_global_position().y>-5 and get_second_global_position().y<5 and int(current_time+delta)>int(current_time):
		global_translate(Vector3(0, .06-get_second_global_position().y, 0))
		on_red_black_brown=is_on_red_black_brown()
	
	pass




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
		rotation_degrees.y=90
	elif careful_mod%3==2:
		result=false
		rotation_degrees.y=-90
	else: #something went wrong, my calculation is out of whack
		print("something went wrong, spiral_rook "+str(name)+" is at geometry textbook coordinate "+str(txtbk_coord)+" where the y*sqrt(3)/3 mod 3 equals 0, which is not supposed to be true. (it would mean the center of a triangle aligning with the bottom edge of another triangle, or with the tip)")
	return result #



	
