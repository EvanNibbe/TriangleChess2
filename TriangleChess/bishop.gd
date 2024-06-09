extends StaticBody3D

const degree_variance: float=1
var current_time: float=0
var is_chosen_v: bool=false 
var when_chosen_v: float=-10
var is_black_v: bool=false
var material: GeometryInstance3D=null
var start_pos: Vector3=Vector3()
var was_moving: bool=false
var has_moved: bool=false
#the line_list assumes that the bishop starts on either the red, black, or brown triangles. 
#If the color under the bishop is different, then the x and y value need to be interchanged with the x value now being negative of the previous y value.
var line_list: Array[Vector3]=[Vector3(0.14434, 1, 0), Vector3(0.57735, 1, 0), Vector3(1.27017, 1, 0), Vector3(-.9382, 0, 0), Vector3(-1.1547, .6, 0), Vector3(-1.5011, -.8125, 0), Vector3(0.4691, -1.3, 0), Vector3(0.057735, -.89375, 0), Vector3(1.4542, 1.05625, 0)]
var dir_list: Array[Vector2]=[Vector2(3, -4.041452), Vector2(1, -.57735), Vector2(4, -.57735), Vector2(1.5, 3.752777), Vector2(0, 1.154701), Vector2(-1.5, 3.752777), Vector2(-4, -.57735), Vector2(-3, -4.041452)]
#note: dir_list assumes the bishop starts on either the red, black or brown triangles.
#dir_list contains the points I drew out in my notebook as the directions of the triangle's points would be pointing 
#with the assumption that the bishop starts at (0, 0)
#to (3, -7sqrt(3)/3)
#to (1, -sqrt(3)/3)
#to (4, -sqrt(3)/3)
#to (1.5, 13sqrt(3)/6)
#to (0, 2sqrt(3)/3)
#to (-1.5, 13sqrt(3)/6)
#to (-4, -sqrt(3)/3)
#to (-1, -sqrt(3)/3)
#to (-3, -7sqrt(3)/3)

var color_of_triangle: String="red"
var on_red_black_brown: bool=false
var time_color_detected: float=-1.5 #the color the bishop is on can be detected once every 1.5 seconds without affecting gameplay

var all_hashed_positions: Dictionary={}
var time_of_check: float=-1
var thing_autoloaded: Node=null

# Called when the node enters the scene tree for the first time.
func _ready():
	material=$MeshInstance3D
	if get_second_global_position().z<0:
		is_black_v=true
		material.set_instance_shader_parameter("black", true)
	else:
		is_black_v=false
		material.set_instance_shader_parameter("black", false)
	current_time=0
	is_chosen_v=false 
	thing_autoloaded=get_node("/root/piece_selected")
	#start_pos=get_second_global_position() (the value of start_pos must only be set by full_check_of_positions() in order for full_check_of_positions() to ever be run.
	pass # Replace with function body.

func is_chosen()->bool:
	return is_chosen_v

func is_black()->bool:
	return is_black_v

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
		on_red_black_brown=true
	elif careful_mod%3==2:
		result=false
		on_red_black_brown=false
	else: #something_autoloaded went wrong, my calculation is out of whack
		print("something went wrong, bishop "+str(name)+" is at geometry textbook coordinate "+str(txtbk_coord)+" where the y*sqrt(3)/3 mod 3 equals 0, which is not supposed to be true. (it would mean the center of a triangle aligning with the bottom edge of another triangle, or with the tip)")
	return result #

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
	on_red_black_brown=is_on_red_black_brown()
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

func hash_pos(global_pos: Vector3, redblackbrown: bool)->int:
	var result: int=0
	if not dead:
		#only counting things that are outside the dead space.
		result+=int(8*((global_pos.x+thing_autoloaded.minX)*(thing_autoloaded.maxZ-thing_autoloaded.minZ)+2*global_pos.z-2*thing_autoloaded.minZ)) #should be a strictly positive integer result
		result*=2 #the least significant bit of the integer is only 1 if self_capture is true (this differentiates spots based on all the initial conditions of the calculation without cross-corruption)
		if redblackbrown: #this fundamentally changes where it can move to.
			result+=1
	return result
	


#the following "can_move_to" function requires defining a couple additional functions
func can_move_to(global_pos1: Vector3, self_capture: bool=false)->bool:
	if (dead or get_second_global_position().y<-10) and (global_pos1.y>-10):
		return false
	if global_pos1.y<-10:
		return true
		
	var result: bool=false
	var fast_result: bool=false
	
	if (all_hashed_positions.has((hash_pos(global_pos1-get_second_global_position(), is_on_red_black_brown())))):
		fast_result=true
		result=all_hashed_positions[hash_pos(global_pos1-get_second_global_position(), is_on_red_black_brown())]
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
	if global_pos.length()<4*sqrt(3)/3+sqrt(3)/5 and global_pos.length()>2*sqrt(3)/3+sqrt(3)/12:
		result=true #get all of the surrounding triangles that are not immediately side-adjacent
	
	elif on_red_black_brown:
		#need to check if equation_pos is closer to 
		var up: Vector2=Vector2 (0, 2*sqrt(3)/3)
		var down_right: Vector2=Vector2 (1, -sqrt(3)/3)
		var down_left: Vector2=Vector2(-1, -sqrt(3)/3)
		#then, move the coordinate system of equation_pos into the position found above (as if the position found above were at (0,0)), then we check if the Vector2.UP.angle_to(equation_pos) or whatever gives a correct angle.
		var len_up: float=(up-equation_pos).length_squared()
		var len_dr: float=(down_right-equation_pos).length_squared()
		var len_dl: float=(down_left-equation_pos).length_squared()
		if len_up<len_dl and len_up<len_dr:
			#set the position of global_pos as if origin is now (0,0, -2*sqrt(3)/3), then check if Vector3.FORWARD.angle_to(global_pos) is either within 5 degrees of 0, or 60
			global_pos=global_pos-Vector3(0, 0, -2*sqrt(3)/3) #because the z axis is flipped relative to the geometry textbook that would use y horizontally.
			var angle: float=Vector3.FORWARD.angle_to(global_pos)
			angle=rad_to_deg(angle)
			if angle>-degree_variance and angle<degree_variance:
				result=true #
			#elif angle>55 and angle<65:
			#	result=true #
			#angle_to gives the minimum positive angle between the vectors, so no further test is needed.
		elif len_dr<len_up and len_dr<len_dl:
			global_pos=global_pos-Vector3(1, 0, sqrt(3)/3) #since the z axis is flipped relative to the geometry textbook y axis in that direction.
			var angle: float=Vector3.RIGHT.angle_to(global_pos)
			angle=rad_to_deg(angle)
			if angle>30-degree_variance and angle<30+degree_variance:
				result=true #
				angle=Vector3.BACK.angle_to(global_pos)
				angle=rad_to_deg(angle)
				if angle>60-degree_variance and angle<60+degree_variance:
					result=true #
				else:
					result=false #not pointing in the correct direction.
		elif len_dl<len_dr and len_dl<len_up:
			global_pos=global_pos-Vector3(-1, 0, sqrt(3)/3) #since the z axis is flipped relative to the geometry textbook y axis in that direction.
			var angle: float=Vector3.LEFT.angle_to(global_pos)
			angle=rad_to_deg(angle)
			if angle>30-degree_variance and angle<30+degree_variance:
				result=true #
				angle=Vector3.BACK.angle_to(global_pos)
				angle=rad_to_deg(angle)
				if angle>60-degree_variance and angle<60+degree_variance:
					result=true #
				else:
					result=false #not pointing in the correct direction.
		
	else:
		#var list_to_use: Array[Vector2]=[]
		var up_right: Vector2=Vector2(1, sqrt(3)/3)
		var down: Vector2=Vector2(0, -2*sqrt(3)/3)
		var up_left: Vector2=Vector2(-1, sqrt(3)/3)
		
		var len_dn: float=(equation_pos-down).length_squared()
		var len_ur: float=(equation_pos-up_right).length_squared()
		var len_ul: float=(equation_pos-up_left).length_squared()
		
		if len_dn<len_ul and len_dn<len_ur:
			global_pos=global_pos-Vector3(0, 0, 2*sqrt(3)/3) #since the z axis is flipped relative to the geometry textbook y axis in that direction.
			var angle: float=Vector3.BACK.angle_to(global_pos)
			angle=rad_to_deg(angle)
			if angle>-degree_variance and angle<degree_variance:
				result=true #
			#elif angle>55 and angle<65:
			#	result=true #
		elif len_ur<len_dn and len_ur<len_ul:
			global_pos=global_pos-Vector3(1, 0, -sqrt(3)/3) #since the z axis is flipped relative to the geometry textbook y axis in that direction.
			var angle: float=Vector3.FORWARD.angle_to(global_pos)
			angle=rad_to_deg(angle)
			if angle>60-degree_variance and angle<60+degree_variance:
				result=true #
				angle=Vector3.RIGHT.angle_to(global_pos)
				angle=rad_to_deg(angle)
				if angle>30-degree_variance and angle<30+degree_variance:
					result=true #
				else:
					result=false #not pointing along the point of the triangle.
		elif len_ul<len_ur and len_ul<len_dn:
			global_pos=global_pos-Vector3(-1, 0, -sqrt(3)/3) #since the z axis is flipped relative to the geometry textbook y axis in that direction.
			var angle: float=Vector3.FORWARD.angle_to(global_pos)
			angle=rad_to_deg(angle)
			if angle>60-degree_variance and angle<60+degree_variance:
				result=true #
				angle=Vector3.LEFT.angle_to(global_pos)
				angle=rad_to_deg(angle)
				if angle>30-degree_variance and angle<30+degree_variance:
					result=true #
				else:
					result=false #not pointing along the point of the triangle.
	
	
	all_hashed_positions[hash_pos(global_pos1-get_second_global_position(), is_on_red_black_brown())]=result #this dictionary assumes self-capture is permitted.
	#note: we only need to have the dictionary consider relative moves, not absolute moves.
	if result and not self_capture:
		#check if the position is shared by a piece of the same color.
		var piece_near: Node3D=thing_autoloaded.piece_near(global_pos1)
		if piece_near!=null and piece_near.name!=name and piece_near.has_method("is_black") and piece_near.is_black()==is_black() and (piece_near.get_second_global_position()-global_pos1).length_squared()<1/3:
			return false #there is a piece of the same color as this piece (and it is not this piece) at the location you are trying to move to.
		else:
			return true
	return result

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
	#if get_second_global_position().y>-100 and (start_pos-get_second_global_position()).length_squared()>.1:
	#	full_check_of_positions()
	pass


func point_to_line(point: Vector2, line: Vector3)->float: #line is Vector3(a,b,c) where ax+by+c=0 is the equation of the line
	var num: float=line.x*point.x+line.y*(point.y)+line.z 
	#need to translate the point's position from Godot editor (-z axis is forward horizontal) to 2D geometry math textbook (+y axis is forward horizontal)
	#translation is already done at the top of func can_move_to
	num=abs(num)
	var denom=line.x*line.x+line.y*line.y
	denom=sqrt(denom)
	return num/denom



func close_to_line(point: Vector2)->bool: #checks all 9 lines, after resolving which color of triangle this bishop is on.
	if dead:
		return false
	var result: bool=false
	var line_dist: float=100
	var threshold: float=sqrt(3)/3 #min dist must be below this threshold
	
	if on_red_black_brown:  #point_to_line(point: Vector2, line: Vector3)
		for l in line_list:
			var cur_dist: float=point_to_line(point, l)
			if cur_dist<line_dist:
				line_dist=cur_dist
	else:
		var list_to_use: Array[Vector3]=[]
		for l in line_list:
			var cur: Vector3=Vector3(l.x, -l.y, l.z)
			list_to_use.append(cur)
		for l in list_to_use:
			var cur_dist: float=point_to_line(point, l)
			if cur_dist<line_dist:
				line_dist=cur_dist
	if line_dist<threshold:
		result=true
	
	return result




	
