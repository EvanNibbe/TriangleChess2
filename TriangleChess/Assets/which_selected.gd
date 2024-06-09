extends Node
var fog_of_war: bool=false
var is_black: bool=false #is the black side the "real" human player on this computer?
var piece_selected: Node3D=null
var any_piece_selected: bool=false
var move_to_position: Vector3=Vector3(0,0,0)
var when_move_to_position: float=-10
var any_move_to_position: bool=false
var white_player_pieces: Array[Node3D]=[]
var black_player_pieces: Array[Node3D]=[]
var possible_positions: Array[Vector3]=[] #the centers of each triangle, which are passed to 
	#this spot with the ready() function on triangle_bais.gd

var white_rook_defenders: Array[CollisionShape3D]=[]
var black_rook_defenders: Array[CollisionShape3D]=[]

func add_rook_defender(thing: Node3D):
	var cols: Array[CollisionShape3D]=[]
	for th in thing.get_children():
		if str(th.name).contains("oll"):
			cols.append(th)
	if thing.global_position.z>0:
		white_rook_defenders.append_array(cols)
	else:
		black_rook_defenders.append_array(cols)

var hash_pos_to_spot: Dictionary={} #k: hash_pos(global_pos: Vector3); v: global_pos
var hashTable_pos_to_piece: Dictionary={} #translates the integer from hash_pos(global_position) to the Node3D at that position
const maxZ: float=20
const minZ: float=-20
const minX: float=-16
const maxX: float=16

var time_from_selection: float=-10
var current_time: float=0
var start_pos: Vector3=Vector3(0,0,0) 
var currently_moving_piece: bool=false
const MAX_FORCE: float=100
var check_rigid: RigidBody3D=null
var moved_so_far: float=0
var prev_pos: Vector3=Vector3()
var world: World3D=null
var is_max_pos_found: bool=false #
var AI_script_node: Node=null
var first_move_happened: bool=false #if the AI is white, it must move before the player, but after that
#first move, the AI can replicate the fact that it is still moving first by moving immediately after the player plays (mathematically the same).
#since no player can capture on the first turn, we don't have much to worry about.

func set_AI(thing: Node):
	if thing.has_method("make_move"):
		AI_script_node=thing

func _ready():
	check_rigid=RigidBody3D.new()
	add_child(check_rigid)
	check_rigid.name="null"
	check_rigid.global_translate(Vector3(-10000, 0, -10000))

#hash_pos is important for replacing the need for RayCasting, because now we can use the hash_pos 
#function to reference the Vector3 position of a triangle in possible_positions (incrementing and decrementing x and z to check all 9 possible places for the center of a triangle
#then, once those triangles are found, those triangles form references to the pieces on said triangles,
#which are then checked for having global_positions within 2*sqrt(3)/3 of the line of a piece's movement (where stepping along that line of movement 
func hash_pos(global_pos: Vector3)->int:
	var result: int=0
	if global_pos.y>-500:
		result=int(((global_pos.x-minX)*(maxZ-minZ)+global_pos.z-minZ)*8)
	return result

func simple_hash_pos(x: float, z: float)->int:
	var result: int=int(((x-minX)*(maxZ-minZ)+z-minZ)*2)
	return result

func exists_position(global_pos: Vector3)->bool:
	return hash_pos_to_spot.has(hash_pos(global_pos))

func point_to_line(global_pos: Vector3, st_pos: Vector3, end_pos: Vector3)->float:
	var result: float=0
	if global_pos.y<-500: #we don't waste time on dead pieces.
		result=1000000
	else:
		var a: float=(st_pos-end_pos).length()
		var b: float=(end_pos-global_pos).length()
		var c: float=(global_pos-st_pos).length()
		#use Heron's formula to find the area of the triangle whose vertices are at the three initial vector points.
		var area: float=0.25*sqrt((a+b+c)*(-a+b+c)*(a-b+c)*(a+b-c))
		#then I can use the reverse of area=1/2*base*height to find the height (where base is the length "a" from start to end, therefore height would be from global_pos to the generalized line from start to end)
		result=2*area/a 
		
	return result
	
func reset_hashTable():
	hashTable_pos_to_piece.clear() #remove all entries and start over.
	if hash_pos_to_spot.is_empty() and len(possible_positions)>40:
		for pos in possible_positions:
			hash_pos_to_spot[hash_pos(pos)]=pos #covers all possible positions, provides a fast way to see if a move goes on the board.
		#hash_pos_to_spot.make_read_only() #we don't want to add any move valid positions.
	for piece in black_player_pieces:
		var index=hash_pos(piece.get_second_global_position())
		hashTable_pos_to_piece[index]=piece
	for piece in white_player_pieces:
		var index=hash_pos(piece.get_second_global_position())
		hashTable_pos_to_piece[index]=piece
		




func piece_near(global_pos: Vector3)->Node3D: #searches the 1/16th of the table closest to global_pos. (with an extra margin of error)
	return search_all_pieces(global_pos) #this ended up being faster and more generally effective.

#the ray_cast function will be used by the opponent AI script for figuring out "real" movement options.
#it won't be used in the can_move_to function because I like the fact that the player can see where pieces would be able to go if
#those pieces were actually able to move through things (as well as it allows the player to be assured through the fog that the opponent has the same pieces he or she does).
#if this returns null, then don't use the value, otherwise, if the name of the piece returned is "null", (the check_rigid), then the path is clear (or it starts and ends with two nodes: a Node3D within sqrt(3)/3 of start, a Node3D within sqrt(3)/3 of end.
#if this returns a valid Node3D piece, then that piece appears within sqrt(3)/3 of the line of the movement.
const OFFSET_DIST: float=sqrt(3)/3+.1 #provide some margin of error.
func ray_cast(start: Vector3, end: Vector3)->Node3D:
	
	if len(hashTable_pos_to_piece)<2:
		return null #there is a problem if the table isn't set up, this function can't be used
	var result: Node3D=check_rigid
	#nyqw25sd
	var steps: int=int((start-end).length()+.99)*2 #need to make sure nothing gets skipped, this would have increments generally smaller than .5
	var piece_close: Node3D=piece_near(start)
	var piece_far: Node3D=piece_near(end)
	var exclude: Array[Node3D]=[]
	if (piece_close.get_second_global_position()-start).length_squared()<1/10:
		exclude.append(piece_close)
	if (piece_far.get_second_global_position()-end).length_squared()<1/10:
		exclude.append(piece_far)
	var piece_in: Node3D=search_all_pieces_close_to_line(start, end, exclude)
	if piece_in!=null:
		result=piece_in
		#var df: float=1.0/steps
		#var f: float=0
		#var cur: Vector3=start*(1-f)+end*f #linear interpolation to the end.
		#while f<1.0 and result==check_rigid:
		#	f+=df #has to be incremented first because it is useless to start at start, where we know that a Node3D is and we should ignore it.
		#	cur=start*(1-f)+end*f
		#	var temp: Node3D=piece_near(cur)
		#	if not temp==null and (temp.get_second_global_position()-start).length_squared()>1/8 and (temp.get_second_global_position()-end).length_squared()>1/8 and point_to_line(temp.get_second_global_position(), start, end)<OFFSET_DIST:
		#		result=temp #this will automatically break the while loop.
			
	return result
	

func _process(delta):
	current_time+=delta
	if len(possible_positions)>40 and len(white_player_pieces)>2 and len(black_player_pieces)>2 and len(hashTable_pos_to_piece)<4:
		reset_hashTable() #make sure it is set up with all the pieces before accidents happen.
	
	if world==null and piece_selected!=null and any_piece_selected:
		world=piece_selected.get_world_3d()
	if currently_moving_piece and any_piece_selected and piece_selected!=null:
		#this means that piece_selected is a RigidBody3D and has been moving from start_pos, and has not finished moving from start_pos
		#this means that no pieces should be allowed to be clicked in the intervening time
		#if the time elapsed since the start of the move (when_move_to_position) is too long, and the piece has not moved an appropriate amount of distance,
		#then the amount of force applied (multiplied by the mass of the RigidBody3D) will be gradually ramped up to MAX_FORCE, and then the force applied 
		#will stop, and currently_moving_piece will stop, even if the piece has not moved the distance it is required to move.
		pass
		var time_diff: float=current_time-time_from_selection #how long has the movement operation taken to this point.
		moved_so_far+=(piece_selected.get_second_global_position()-prev_pos).length()
		var move_diff: float=moved_so_far #how far has the piece gone? This is basically adding up the tiny discrete movements of the piece.
		var move_tot: float=(start_pos-move_to_position).length() #how far does the piece have to go overall?
		#var move_left: float=move_tot-move_diff #you will see that this is not the same as (global_position-move_to_position).length()
			#when the piece gets deflected in its movement by an intervening piece, it will try to move in the general direction
			#it was moving, but no longer to the chosen target position, and it will not try moving any farther than
			#the total distance it was supposed to move in the first place (or, at least, the force won't continue farther than that).
		var rigid: RigidBody3D=piece_selected
		var rel_move: Vector3=(move_to_position-piece_selected.get_second_global_position()).normalized() #the normalized vector of where to move to.
		
		if time_diff>1 and move_diff/move_tot<.5:
			rigid.apply_force(rel_move*rigid.mass*min(MAX_FORCE, 1+time_diff*time_diff))
		if time_diff<=1 and move_diff/move_tot<.1:
			rigid.apply_force(rel_move*rigid.mass*min(MAX_FORCE, 1+time_diff))
		if move_diff/move_tot<.7:
			rigid.apply_force(rel_move*rigid.mass*min(MAX_FORCE, time_diff + time_diff*time_diff*time_diff/1000))
		if move_diff/move_tot<.8:
			rigid.apply_force(rel_move*rigid.mass*min(MAX_FORCE, time_diff + time_diff*time_diff*time_diff/10000)*delta)
		if move_diff/move_tot<.9:
			rigid.apply_force(rel_move*rigid.mass*min(MAX_FORCE, time_diff + time_diff*time_diff*time_diff/100000)*delta)
		elif move_diff/move_tot<.95 and time_diff<10:
			rigid.apply_force(rel_move*rigid.mass*max(0, min(MAX_FORCE-rigid.linear_velocity.length(), time_diff ))*delta)
		elif move_diff/move_tot<.99 and time_diff<10:
			rigid.apply_force(rel_move*rigid.mass*min(MAX_FORCE-rigid.linear_velocity.length(), time_diff)*delta)
		else:
			currently_moving_piece=false #
			any_move_to_position=false #
			any_piece_selected=false
			RenderingServer.global_shader_parameter_set("any_piece_selected", false)
		prev_pos=piece_selected.get_second_global_position()
	
func piece_clicked(piece: Node3D)->bool:
	if currently_moving_piece:
		return false #can't click a new piece while a previous piece is moving.
	#this both checks if enough time has passed to make a new selection (if not enough time has passed, then returns false and doesn't change the values).
	#it also, if it returns true, then, it chooses the piece selected thus.
	if piece==piece_selected:
		return true
	else:
		if time_from_selection+1<current_time or not any_piece_selected:
			piece_selected=piece
			time_from_selection=current_time
			any_piece_selected=true
			any_move_to_position=false #
			when_move_to_position=-10 #this number needs to be larger than "time_from_selection" in order to move the piece there.
			RenderingServer.global_shader_parameter_set("any_piece_selected", true)
			RenderingServer.global_shader_parameter_set("selected_piece_position", piece_selected.get_second_global_position())
			return true #
		else:
			return false

func translate_to_geometry_txtbk(global_pos: Vector3)->Vector2:
	return Vector2(global_pos.x, -global_pos.z)
func translate_from_geometry_txtbk(effective_pos: Vector2)->Vector3:
	return Vector3(effective_pos.x, .1, -effective_pos.y)

func search_all_pieces(spot: Vector3)->Node3D: #returns the piece closest to spot.
	var close_piece: Node3D=white_player_pieces[0]
	var close_dist: float=(spot-close_piece.get_second_global_position()).length_squared()
	for piece in white_player_pieces:
		var temp: float=(piece.get_second_global_position()-spot).length_squared()
		if temp<close_dist:
			close_dist=temp #
			close_piece=piece #
	for piece in black_player_pieces:
		var temp: float=(piece.get_second_global_position()-spot).length_squared()
		if temp<close_dist:
			close_dist=temp #
			close_piece=piece #
	return close_piece



func search_all_pieces_close_to_line(start: Vector3, end: Vector3, exclude: Array[Node3D])->Node3D: #returns null if nothing is within sqrt(3)/6 of the line
	var result: Node3D=null # prefer pieces closer to start.
	var closest: Node3D=null
	var closest_dist: float=10000
	var over_dist: float=(start-end).length()
	var start_white: int=-1 #-1 means undecided, 0 means the piece at start is black, 1 means piece at start is white.
	for piece in white_player_pieces:
		if exclude.has(piece):
			continue
		elif (piece.get_second_global_position()-start).length()<over_dist and (piece.get_second_global_position()-end).length()<over_dist:
			var dist: float=point_to_line(piece.get_second_global_position(), start, end)
			if dist<sqrt(3)/3:
				#don't get exactly start, because that is not useful
				var tdist: float=(piece.get_second_global_position()-start).length_squared()
				if tdist<1/8:
					start_white=1
				if tdist>1/8 and result==null:
					result=piece #no contest.
				else:
					if tdist>1/8 and tdist<(result.get_second_global_position()-start).length_squared():
						result=piece #go for things closer to start.
			if dist<closest_dist:
				closest_dist=dist #
				closest=piece
	for piece in black_player_pieces:
		if exclude.has(piece):
			continue
		elif (piece.get_second_global_position()-start).length()<over_dist and (piece.get_second_global_position()-end).length()<over_dist:
			var dist: float=point_to_line(piece.get_second_global_position(), start, end)
			if dist<sqrt(3)/3:
				#don't get exactly start, because that is not useful
				var tdist: float=(piece.get_second_global_position()-start).length_squared()
				if tdist<1/8:
					start_white=0 #a black piece is at the position of "start"
				if tdist>1/8 and result==null:
					result=piece #no contest.
				else:
					if tdist>1/8 and tdist<(result.get_second_global_position()-start).length_squared():
						result=piece #go for things closer to start.
			if dist<closest_dist:
				closest_dist=dist #
				closest=piece
	if start_white==1:
		#search through all black_rook_defenders to see if one is in the way of a capture.
		for piece in black_rook_defenders:
			if (piece.get_second_global_position()-start).length()<over_dist and (piece.get_second_global_position()-end).length()<over_dist:
				var dist: float=point_to_line(piece.get_second_global_position(), start, end)
				if dist<1/10:
					#don't get exactly start, because that is not useful
					var tdist: float=(piece.get_second_global_position()-start).length_squared()
					if tdist<1/8:
						start_white=0 #a black piece is at the position of "start"
					if tdist>1/8 and result==null:
						result=piece #no contest.
					else:
						if tdist>1/8 and tdist<(result.get_second_global_position()-start).length_squared():
							result=piece #go for things closer to start.
				if dist<closest_dist:
					closest_dist=dist #
					closest=piece
	elif start_white==0:
		#search through all white_rook_defenders to see if one is in the way of a capture.
		for piece in white_rook_defenders:
			if (piece.get_second_global_position()-start).length()<over_dist and (piece.get_second_global_position()-end).length()<over_dist:
				var dist: float=point_to_line(piece.get_second_global_position(), start, end)
				if dist<1/10:
					#don't get exactly start, because that is not useful
					var tdist: float=(piece.get_second_global_position()-start).length_squared()
					if tdist<1/8:
						start_white=1
					if tdist>1/8 and result==null:
						result=piece #no contest.
					else:
						if tdist>1/8 and tdist<(result.get_second_global_position()-start).length_squared():
							result=piece #go for things closer to start.
				if dist<closest_dist:
					closest_dist=dist #
					closest=piece
	return result

func check_move_piece_to(global_pos: Vector3, moveNode: Node3D)->Array:
	#this function will check if the piece to move is a RigidBody3D, in which case currently_moving_piece 
	#gets set to true, and force applied will gradually ramp up to MAX_FORCE, then stop, and reopen choosing a new piece.
	print("line 96") #is printed
	var mandatory_repeat: bool=false #that a player hits its own piece.
	var st_pos=moveNode.get_second_global_position() #it's already established above that moveNode is not null, which means that it MUST be of Node3D type.
	var res_pos=st_pos
	var piece_removed: Node3D=null
	var result: Array=[res_pos, piece_removed, null, Vector3.DOWN] #the latter two items are for what the replacement move is if the initial move is invalid
	if moveNode.has_method("can_move_to"):
		print("line 98: Node is ", moveNode, " global_pos is ", global_pos, " st_pos is ", st_pos) #is printed
		if moveNode.can_move_to(global_pos):
			print("line 100") #is printed
			st_pos.y=.1
			time_from_selection=current_time 
			if moveNode.get_class()==check_rigid.get_class():
				print("line 105") #NOT printed (which is correct operation since I wasn't using RigidBody3D
				currently_moving_piece=true
				moved_so_far=0
				prev_pos=st_pos
			else:
				print("line 110") #is printed
				currently_moving_piece=false
				RenderingServer.global_shader_parameter_set("any_piece_selected", false)
				
				if moveNode!=null and not str(moveNode.name).begins_with("k") and (moveNode.get_second_global_position()-st_pos).length()<sqrt(3)/3: #the knight and spiral rook operate differently
					#The spiral rook falls back on straight line movement when it can't move in a spiral.
					print("line 116") #is printed (correct operation since I was not yet using knights).
					var steps: int=int((global_pos-st_pos).length()/(2*sqrt(3)/3)+.5)
					var cur_pos: Vector3=st_pos
					var step_f: float=1/float(steps)
					var step_delta: float=step_f
					var next_pos: Vector3=(global_pos*step_f+st_pos*(1-step_f))
					var relative_final: Vector3=(global_pos-st_pos)
					var totlen2: float=relative_final.length_squared()
					var too_far: float=1/3
					var ex_collider: Node3D=check_rigid #something that is obviously impermissible when checking distance and relative_final.angle_to(ex_collider.get_second_global_position())
					cur_pos.y=.1
					next_pos.y=.1
					var ignore: Array[RID]=[moveNode.get_rid()]
					for ch in moveNode.get_children():
						if ch.has_method("get_rid"):
							var ch_rid: RID=ch.get_rid()
							ignore.append(ch_rid)
					var query: PhysicsRayQueryParameters3D=PhysicsRayQueryParameters3D.create(cur_pos, global_pos, 4294967295, ignore)
					var collision: Dictionary={}  #world.direct_space_state.intersect_ray(query)
					#print("initial ", collision)																		#avoids hitting one's own rook_defender
					#var col_v: Vector3= (moveNode.get_second_global_position())
					#if collision.has("collider"):
					#	col_v=collision.collider.get_second_global_position()-col_v
					#var hit_self: Vector2=Vector2(col_v.x, col_v.z)
					collision["collider"]=search_all_pieces_close_to_line(st_pos, global_pos, [moveNode])
					if collision.collider!=null:
						print("collision was ", collision, " where collider was at ", collision.collider.get_second_global_position())
					#now check if the collision is the opposite color as this piece, in which case it will take it's spot. Otherwise, this piece will move to the spot immediately prior 
					#(use a ray cast downward to see the triangle, then move to the center of the triangle, up sufficiently).
					
					print("line 137") #is printed
					if collision!=null and collision.has("collider") and collision.collider!=null:
						if collision.collider.has_method("is_black"):
							print("line 139") #NOT printed (correct operation since I was moving a pawn to an empty space)
							if collision.collider.is_black() and not moveNode.is_black():
								res_pos=collision.collider.get_second_global_position()
								piece_removed=collision.collider
								#moveNode.global_translate(collision.collider.get_second_global_position()-st_pos)
								#collision.collider.global_translate(Vector3(0, -10000, 0)) #killing off this piece. (Allows an easier time resetting the game to the pieces in their start locations. (easier relative to using queue_free))
								if str(collision.collider.name).begins_with("king"): 
									print("White wins! Black loses.")
								
							elif not collision.collider.is_black() and moveNode.is_black():
								print("line 149")
								res_pos=collision.collider.get_second_global_position()
								piece_removed=collision.collider
								#moveNode.global_translate(collision.collider.get_second_global_position()-st_pos)
								#collision.collider.global_translate(Vector3(0, -10000, 0)) #killing off this piece. (Allows an easier time resetting the game to the pieces in their start locations. (easier relative to using queue_free))
								if str(collision.collider.name).begins_with("king"): 
									print("Black wins! White loses.")
							elif collision.collider.get_second_global_position().y<-10:
								res_pos=collision.collider.get_second_global_position()
								res_pos.y=moveNode.get_second_global_position().y

								
						else:
							print("line 158")
							next_pos=cur_pos
							cur_pos.y=10
							next_pos.y=-10 #the triangle should definitely be hit by the ray.
							query.set_from(cur_pos)
							query.set_to(next_pos)
							collision=world.direct_space_state.intersect_ray(query)
							#apparently, the below code wasn't reached on July 28th at 2:11 pm, the triangle wasn't detected.
							if collision!=null and collision.has("collider") and str(collision.collider.get_parent().name).begins_with("triangl"):
								var triangle: Node3D=collision.collider.get_parent()
								print("moving piece "+str(moveNode.name)+" to triangle "+str(triangle)+" child of "+str(triangle.get_parent_node_3d()))
								res_pos=triangle.find_child("center_triangle").get_second_global_position()
								res_pos.y=moveNode.get_second_global_position().y
								#moveNode.global_translate(res_pos-moveNode.get_second_global_position())
								
								#any_move_to_position=false
							elif collision.collider!=null and collision.collider.get_second_global_position().y<-10:
								res_pos=collision.collider.get_second_global_position()
								res_pos.y=moveNode.get_second_global_position().y
					else:
						print("line 176") #is printed (correct operation since the corresponding if was false)
						cur_pos=global_pos
						next_pos=cur_pos
						cur_pos.y=10
						next_pos.y=-10 #the triangle should definitely be hit by the ray.
						query.set_from(cur_pos)
						query.set_to(next_pos)
						collision=world.direct_space_state.intersect_ray(query)
						print(str(collision))
						
						if collision!=null and collision.has("collider"):
							if not collision.collider.has_method("is_black"): #should be a non-chesspiece object.
								#print("collider's parent: ", str(collision.collider.get_parent()))
								#print("collider's parent's name: ", str(collision.collider.get_parent().name))
								#print("collider's parent's children: ", str(collision.collider.get_parent().get_children()))
								var triangle: Node3D=collision.collider.get_parent()
								var center_triangle: Node3D=triangle.find_child("center_triangle")
								#print("collider's triangle's center: ", str(center_triangle), " at ", str(center_triangle.get_second_global_position()))
								if str(collision.collider.get_parent().name).begins_with("triangl"):
									#print("moving piece "+str(moveNode.name)+" to triangle "+str(triangle)+" child of "+str(triangle.get_parent_node_3d())) #NOT printed (incorrect operation, means that a ray going straight down doesn't detect a triangle).
									res_pos=center_triangle.get_second_global_position()
									res_pos.y=moveNode.get_second_global_position().y
									#var trans_diff: Vector3=res_pos-moveNode.get_second_global_position()
									#trans_diff.y=0
									#moveNode.global_translate(trans_diff)
								elif collision.collider!=null and collision.collider.get_second_global_position().y<-10:
									res_pos=collision.collider.get_second_global_position()
									res_pos.y=moveNode.get_second_global_position().y

									
							else: #this is where a goto would be handy
								if collision.collider.is_black() and not moveNode.is_black():
									res_pos=collision.collider.get_second_global_position()
									piece_removed=collision.collider
									#moveNode.global_translate(collision.collider.get_second_global_position()-st_pos)
									#collision.collider.global_translate(Vector3(0, -10000, 0)) #killing off this piece. (Allows an easier time resetting the game to the pieces in their start locations. (easier relative to using queue_free))
									if str(collision.collider.name).begins_with("king"): 
										print("White wins! Black loses.")

									
								elif not collision.collider.is_black() and moveNode.is_black():
									print("line 149")
									res_pos=collision.collider.get_second_global_position()
									#moveNode.global_translate(collision.collider.get_second_global_position()-st_pos)
									piece_removed=collision.collider
									#collision.collider.global_translate(Vector3(0, -10000, 0)) #killing off this piece. (Allows an easier time resetting the game to the pieces in their start locations. (easier relative to using queue_free))
									if str(collision.collider.name).begins_with("king"): 
										print("Black wins! White loses.")
								elif collision.collider!=null and collision.collider.get_second_global_position().y<-10:
									res_pos=collision.collider.get_second_global_position()
									res_pos.y=moveNode.get_second_global_position().y
								else:
									mandatory_repeat=true

									
				elif moveNode!=null and str(moveNode.name).begins_with("k"): #both king and knight should use this code in order that kings don't get stopped by 
					#a kiddie-corner pawn.
					print("line 193")
					var cur_pos: Vector3=global_pos
					cur_pos.y=10
					#var step_f: float=1/float(steps)
					#var step_delta: float=step_f
					var next_pos: Vector3=(global_pos)
					next_pos.y=-10
					var query: PhysicsRayQueryParameters3D=PhysicsRayQueryParameters3D.create(cur_pos, next_pos, 4294967295, [moveNode.get_rid()])
					var collision=world.direct_space_state.intersect_ray(query)
					if collision!=null and collision.has("collider"):
						print("line 203")
						#either a triangle (open spot) or a piece (check to see if opposite color, otherwise do nothing).
						if collision.collider.has_method("is_black"):
							var oppo: Node3D=collision.collider
							if oppo.is_black() and not moveNode.is_black():
								res_pos=oppo.get_second_global_position()
								#moveNode.global_translate(oppo.get_second_global_position()-moveNode.get_second_global_position())
								piece_removed=oppo
								#oppo.global_translate(Vector3(0, -10000, 0)) #kill the opponent piece.
								if str(oppo.name).begins_with("king"):
									print("White wins! Black loses.")

								
							elif not oppo.is_black() and moveNode.is_black():
								piece_removed=oppo
								res_pos=oppo.get_second_global_position()
								#moveNode.global_translate(oppo.get_second_global_position()-moveNode.get_second_global_position())
								#oppo.global_translate(Vector3(0, -10000, 0)) #kill the opponent piece.
								if str(oppo.name).begins_with("king"):
									print("Black wins! White loses.")
							elif collision.collider!=null and collision.collider.get_second_global_position().y<-10:
								res_pos=collision.collider.get_second_global_position()
								res_pos.y=moveNode.get_second_global_position().y
							else:
								mandatory_repeat=true
								print("line 428: ", oppo.name, " would hit its own piece.")

								
						elif str(collision.collider.get_parent().name).begins_with("triang"):
							var place: Node3D=collision.collider.get_parent() #
							res_pos=place.find_child("center_triangle").get_second_global_position()
							#moveNode.global_translate(place.find_child("center_triangle").get_second_global_position()-moveNode.get_second_global_position())
							
				#the spiral rook falls back on using spiral movement when it can't make a straight line move
				
	result=[res_pos, piece_removed, mandatory_repeat]
	return result

func is_AI_piece_there(spot: Vector3, start_poses: Array[Array])->bool:
	#prerequisite: start_poses must hold where the pieces are (without being messed up by the AI_script_node.make_move() (internal calculations involve moving pieces)).
	for nodeVec in start_poses: #restore all positions.
		var node: Node3D=nodeVec[0]
		var vec: Vector3=nodeVec[1]
		node.global_translate(vec-node.get_second_global_position())
	var result: bool=false #
	if is_black: #the player is black, meaning AI pieces are white.
		for piece in white_player_pieces:
			if (spot-piece.get_second_global_position()).length_squared()<1/3:
				result=true # there is an AI piece there.
				break #no need to continue the loop.
	else: #the player is white, meaning the AI pieces are black.
		for piece in black_player_pieces:
			if (spot-piece.get_second_global_position()).length_squared()<1/3:
				result=true # there is an AI piece there.
				break #no need to continue the loop.
	return result

func move_piece_to(global_pos: Vector3)->bool: 
	var AI_move: Array=[] #this should later consist of [piece_to_move: Node3D, g_pos: Vector3] 
		#the piece positions are all saved at the start of this function
		#and restored at the end of this function, which prevents the AI from accidentally leaving pieces at the 
		#wrong locations.
	var start_poses: Array[Array]=[] #this holds Arrays with [Node3D, where_it_started: Vector3] for all pieces
	for piece in black_player_pieces:
		start_poses.append([piece, piece.get_second_global_position()]) #I checked to make sure that Vector3 doesn't seem to have any references to type aliasing, 
		#so there theoretically shouldn't be any issue of this reference becoming useless when global_position changes.
	for piece in white_player_pieces:
		start_poses.append([piece, piece.get_second_global_position()])
	
	global_pos.y=.1 #make sure that we aren't sending out pieces below the grid.
	if currently_moving_piece or piece_selected==null or (not any_piece_selected) or  current_time<=time_from_selection:
		print("line 91")
		return false #can't choose a new spot to move to while moving a piece.
	var result_array: Array=check_move_piece_to(global_pos, piece_selected)
	var result: bool=false
	var piece_removed=result_array[1]
	move_to_position=result_array[0]
	any_move_to_position=true
	if piece_selected!=null and (start_pos-move_to_position).length_squared()>1/3:
		if piece_selected.can_move_to(move_to_position):
			if is_black and AI_script_node!=null and not first_move_happened:
				AI_move=AI_script_node.make_move()
				first_move_happened=true
				for nodeVec in start_poses: #restore all positions.
					var node: Node3D=nodeVec[0]
					var vec: Vector3=nodeVec[1]
					node.global_translate(vec-node.get_second_global_position())
					hashTable_pos_to_piece[hash_pos(node.get_second_global_position())]=node
				#now we have to use the above code to run the test if a piece can move somewhere.
				#I should load the above code into a straight function.
				if AI_move[0]==null:
					print("Stalemate, the AI can't find a move.")
				else:
					var real_AI_move=check_move_piece_to(AI_move[1], AI_move[0])
					
					var piece_selected_AI: Node3D=AI_move[0]
					var move_to_position_AI: Vector3=real_AI_move[0]
					var piece_removed_by_AI: Node3D=real_AI_move[1]
					var start_pos_AI: Vector3=piece_selected_AI.get_second_global_position()
					if piece_selected_AI!=null and (start_pos_AI-move_to_position_AI).length_squared()>1/3:
						if piece_selected_AI.can_move_to(move_to_position_AI):
							piece_selected_AI.global_translate(move_to_position_AI-start_pos_AI)
							hashTable_pos_to_piece[hash_pos(piece_selected_AI.get_second_global_position())]=piece_selected_AI
							if piece_removed_by_AI!=null: #this code is just included for completeness, and just in case 
								#an artificial starting position is made with the white AI being given a chance to capture in the first move (which will cause problems if the black human player is able to make a capture using the piece white is attacking (both pieces would end up getting removed).)
								piece_removed_by_AI.global_translate(Vector3(0, -10000, 0))
				
			piece_selected.global_translate(move_to_position-piece_selected.get_second_global_position())
			hashTable_pos_to_piece[hash_pos(piece_selected.get_second_global_position())]=piece_selected
			if piece_removed!=null:
				piece_removed.global_translate(Vector3(0, -10000, 0))
			#reset_hashTable() #need to figure out where all the pieces are again after the player moved things.
			
			if AI_script_node!=null: #both white and black AI use this, the white AI just got one extra move over the course of a whole game.
				for i in range(len(start_poses)): # to be able to restore all positions.
					#var node: Node3D=nodeVec[0]
					#var vec: Vector3=nodeVec[1]
					start_poses[i][1]=start_poses[i][0].get_second_global_position()
				AI_move=AI_script_node.make_move()
				#while AI_move[0]!=null and AI_move[0].has_method("can_move_to") and is_AI_piece_there(AI_move[1], start_poses):
				#	AI_move=AI_script_node.make_move()
				print("The AI move was: ", AI_move)
				for nodeVec in start_poses: #restore all positions.
					var node: Node3D=nodeVec[0]
					var vec: Vector3=nodeVec[1]
					node.global_translate(vec-node.get_second_global_position())
					hashTable_pos_to_piece[hash_pos(node.get_second_global_position())]=node
				if AI_move[0]==null:
					print("Stalemate: the AI can't find a move.")
				else:
					var piece_AI_wants_to_move: Node3D=AI_move[0]
					var real_AI_move=check_move_piece_to(AI_move[1], piece_AI_wants_to_move)
					var where_AI_would_move_to: Vector3=real_AI_move[0] #the third item [2] is mandatory_repeat, which occurs when the AI attacks its own piece.

					var piece_close: Node3D=search_all_pieces(where_AI_would_move_to)
					if real_AI_move[2] or (piece_close.is_black()==piece_AI_wants_to_move.is_black() and (piece_close.get_second_global_position()-where_AI_would_move_to).length()<.1):
						var tpiece_close=search_all_pieces(AI_move[1])
						if tpiece_close!=null and tpiece_close.is_black()==piece_AI_wants_to_move.is_black():
							piece_close=tpiece_close
						else:
							var start: Vector3=AI_move[0].get_second_global_position()
							var end: Vector3=AI_move[1]
							var exclude: Array[Node3D]=[AI_move[0]]
							tpiece_close=search_all_pieces_close_to_line(start, end, exclude)
							if tpiece_close!=null: #
								piece_close=tpiece_close
						if piece_close.is_black()==piece_AI_wants_to_move.is_black():
							#see if we can move piece_close instead
							var exists_possible: bool=false
							var move_chosen: Vector3=Vector3.DOWN
							for move in possible_positions:
								if piece_close.can_move_to(move):
									var check_result: Array=check_move_piece_to(move, piece_close)
									var temp_piece_close=search_all_pieces(move)
									if check_result[2] or (temp_piece_close.is_black()==piece_close.is_black() and (temp_piece_close.get_second_global_position()-move).length()<.1):
										continue
									else:
										real_AI_move[0]=move
										where_AI_would_move_to=move #
										piece_AI_wants_to_move=piece_close
										exists_possible=true #
										move_chosen=move #
										AI_move[0]=piece_close
										AI_move[1]=move
										break
						#AI_move=AI_script_node.make_move() #because of the already_visited Dictionary in the AI script, the AI will either return a null move, or come up with a proper move.
						#piece_AI_wants_to_move=AI_move[0]
						for nodeVec in start_poses: #restore all positions.
							var node: Node3D=nodeVec[0]
							var vec: Vector3=nodeVec[1]
							node.global_translate(vec-node.get_second_global_position()) #make sure the AI isn't cheating on a move.
						if piece_AI_wants_to_move==null or not piece_AI_wants_to_move.has_method("can_move_to"):
							print("Stalemate, the AI can't find a move.")
							return false
						real_AI_move=check_move_piece_to(AI_move[1], piece_AI_wants_to_move)
						where_AI_would_move_to=real_AI_move[0]
						piece_close=search_all_pieces(where_AI_would_move_to)
					while real_AI_move[2]:
						#################################################################################################
						AI_move=AI_script_node.make_move()
						#while AI_move[0]!=null and AI_move[0].has_method("can_move_to") and is_AI_piece_there(AI_move[1], start_poses):
						#	AI_move=AI_script_node.make_move()
						print("The AI move was: ", AI_move)
						for nodeVec in start_poses: #restore all positions.
							var node: Node3D=nodeVec[0]
							var vec: Vector3=nodeVec[1]
							node.global_translate(vec-node.get_second_global_position())
							hashTable_pos_to_piece[hash_pos(node.get_second_global_position())]=node
						if AI_move[0]==null:
							print("Stalemate: the AI can't find a move.")
						else:
							piece_AI_wants_to_move=AI_move[0]
							real_AI_move=check_move_piece_to(AI_move[1], piece_AI_wants_to_move)
							where_AI_would_move_to=real_AI_move[0] #the third item [2] is mandatory_repeat, which occurs when the AI attacks its own piece.

							piece_close=search_all_pieces(where_AI_would_move_to)
							if real_AI_move[2] or (piece_close.is_black()==piece_AI_wants_to_move.is_black() and (piece_close.get_second_global_position()-where_AI_would_move_to).length()<.1):
								var tpiece_close=search_all_pieces(AI_move[1])
								if tpiece_close!=null and tpiece_close.is_black()==piece_AI_wants_to_move.is_black():
									piece_close=tpiece_close
								else:
									var start: Vector3=AI_move[0].get_second_global_position()
									var end: Vector3=AI_move[1]
									var exclude: Array[Node3D]=[AI_move[0]]
									tpiece_close=search_all_pieces_close_to_line(start, end, exclude)
									if tpiece_close!=null: #
										piece_close=tpiece_close
								if piece_close.is_black()==piece_AI_wants_to_move.is_black():
									#see if we can move piece_close instead
									var exists_possible: bool=false
									var move_chosen: Vector3=Vector3.DOWN
									for move in possible_positions:
										if piece_close.can_move_to(move):
											var check_result: Array=check_move_piece_to(move, piece_close)
											var temp_piece_close=search_all_pieces(move)
											if check_result[2] or (temp_piece_close.is_black()==piece_close.is_black() and (temp_piece_close.get_second_global_position()-move).length()<.1):
												continue
											else:
												real_AI_move[0]=move
												where_AI_would_move_to=move #
												piece_AI_wants_to_move=piece_close
												exists_possible=true #
												move_chosen=move #
												AI_move[0]=piece_close
												AI_move[1]=move
												break
								#AI_move=AI_script_node.make_move() #because of the already_visited Dictionary in the AI script, the AI will either return a null move, or come up with a proper move.
								#piece_AI_wants_to_move=AI_move[0]
								for nodeVec in start_poses: #restore all positions.
									var node: Node3D=nodeVec[0]
									var vec: Vector3=nodeVec[1]
									node.global_translate(vec-node.get_second_global_position()) #make sure the AI isn't cheating on a move.
								if piece_AI_wants_to_move==null or not piece_AI_wants_to_move.has_method("can_move_to"):
									print("Stalemate, the AI can't find a move.")
									return false
								real_AI_move=check_move_piece_to(AI_move[1], piece_AI_wants_to_move)
								where_AI_would_move_to=real_AI_move[0]
								piece_close=search_all_pieces(where_AI_would_move_to)
					
					
					####################################################################################################################################
					print("AI was able to get past ", real_AI_move, " with piece there ", piece_close, " at ", piece_close.get_second_global_position())
					if piece_AI_wants_to_move==null or not piece_AI_wants_to_move.has_method("can_move_to"):
						print("Stalemate, the AI can't find a move.")
						return false
					var piece_selected_AI: Node3D=piece_AI_wants_to_move
					var move_to_position_AI: Vector3=where_AI_would_move_to
					#while AI_move[0]!=null and AI_move[0].has_method("can_move_to") and is_AI_piece_there(AI_move[1], start_poses):
					#	AI_move=AI_script_node.make_move()
					#real_AI_move=check_move_piece_to(AI_move[1], piece_AI_wants_to_move)
					piece_AI_wants_to_move=AI_move[0]
					piece_selected_AI=piece_AI_wants_to_move
					move_to_position_AI=real_AI_move[0]
					var piece_removed_by_AI: Node3D=real_AI_move[1]
					var start_pos_AI: Vector3=piece_selected_AI.get_second_global_position()
					if piece_selected_AI!=null and (start_pos_AI-move_to_position_AI).length_squared()>1/3:
						if piece_selected_AI.can_move_to(move_to_position_AI):
							piece_selected_AI.global_translate(move_to_position_AI-start_pos_AI)
							hashTable_pos_to_piece[hash_pos(piece_selected_AI.get_second_global_position())]=piece_selected_AI
							if piece_removed_by_AI!=null:
								piece_removed_by_AI.global_translate(Vector3(0, -10000, 0))
				
			piece_selected=null # waiting until after the AI makes a move allows it to be more efficient in figuring out which human piece moved.
			any_piece_selected=false
			any_move_to_position=false
			result=true
		else:
			result=false
	elif piece_selected!=null:
		any_piece_selected=true
		result=false
	else:
		piece_selected=null
		any_move_to_position=false #
		any_piece_selected=false
		result=false
		
	return result #
