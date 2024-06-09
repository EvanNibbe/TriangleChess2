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
		result+=int(((global_pos.x+thing_autoloaded.minX)*(thing_autoloaded.maxZ-thing_autoloaded.minZ)+global_pos.z-thing_autoloaded.minZ)*2) #should be a strictly positive integer result
		#result*=2 #the least significant bit of the integer is only 1 if self_capture is true (this differentiates spots based on all the initial conditions of the calculation without cross-corruption)
		
	return result


func can_move_to(global_pos1: Vector3, self_capture: bool=false)->bool:
	if (dead or get_second_global_position().y<-10) and (global_pos1.y>-10):
		return false
	if global_pos1.y<-10:
		return true
	
	var result: bool=false
	
			
	
	var global_pos: Vector3=global_pos1- get_second_global_position() #the relative vector from this triangle to that spot.
	#note: I need to check that this direction makes sense in light of the points of the triangle (that the line is not bi-directional).
	var equation_pos: Vector2=Vector2(global_pos.x, -global_pos.z) #this is the appearance of the x-y plane in 2D in a geometry textbook.
	
	#check that the direction of the vector makes sense:
	var exists_a_dir: bool=(equation_pos.length()>sqrt(3)/3)
	if exists_a_dir:
		#the spot is farther from this spot than the radius of one circle inside a triangle.
		if global_pos.length()>=sqrt(15)-sqrt(3)/6 and global_pos.length()<=2*sqrt(5)+sqrt(3)/6: 
			#a knight in chess moves two spaces in one direction, then 1 space at a 90 degree angle to where it was starting to move.
			#this forms a right triangle with one side length of 2, and one side length of 1. the hypotenuse is then length =sqrt(2*2+1*1)=sqrt(5)
			#the "distance" metric in this game is based on the size of the triangle, but there are two ways of saying the "size" of an equilateral triangle
			#one way is to say "the height of the triangle" (this is from the tip of the triangle to the opposite side), 
			#the other way is to say "the base of the triangle" (every side length is the same, 2 meters)
			#thus, if a spot is between sqrt(5)*height and sqrt(5)*base from where the knight is currently, then that ought to be a valid spot.
			#the sqrt(3)/6 is half of the radius of a circle inscribed in one triangle, and provides a margin of error 
			#which allows the knight 3 more spaces to move, and makes the move options look more like a proper circle.
			#it does make the knight marginally faster in the straight line move that neither a bishop nor rook can make, which is proper 
			#(given how vastly more powerful this game has made rooks and bishops)
			result=true 
			
	
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
	
	pass




	
