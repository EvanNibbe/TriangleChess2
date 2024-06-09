# TriangleChess2
The previous TriangleChess lacked an AI to play against. This version technically has an AI you can play against, however you can't associate the word "intelligence" with this AI since it is both basically deterministic and very stupid. As of the time of the original publication I could use just one rook to capture 7 pawns.

# Playing
Once you have the engine compiled, you run the executable on your computer (this is a standalone executable that can be moved to any folder and still run just fine) 
That executable would be in the TriangleChess2/godot/bin folder.
When you run the executable, you should see a pop-up in the middle of your screen with buttons internal to that window.
Press "scan", and then find the TriangleChess2/TriangleChess folder.
Press "choose current folder".
Once the new window loads with the TriangleChess assets, you should press the " ▶ " button in the top right, which plays the main scene or the current scene you are in. (Currently Board.tscn is where you need to be in order to play a game of chess.)
Click on a piece and wait a second for a couple triangles to turn orange, which are the spots you can move a piece to.
If clicking on a piece doesn't seem to work properly, click on a different piece, wait a second, and then click on the piece you actually want to move.

## Compilation Instructions
See https://docs.godotengine.org/en/stable/contributing/development/compiling/index.html#toc-devel-compiling



# Linux
If you don't already have the scons build system, the instructions on https://docs.godotengine.org/en/stable/contributing/development/compiling/compiling_for_linuxbsd.html
have you use the Ubuntu one-liner to get going of: 
sudo apt-get update
sudo apt-get install -y \
  build-essential \
  scons \
  pkg-config \
  libx11-dev \
  libxcursor-dev \
  libxinerama-dev \
  libgl1-mesa-dev \
  libglu1-mesa-dev \
  libasound2-dev \
  libpulse-dev \
  libudev-dev \
  libxi-dev \
  libxrandr-dev
  

Once you have the scons build system in place, you can use the following code snippet each time you want to run the engine after first setting up the engine or after making changes to the Godot engine source code.
rm -fdR TriangleChess2
git clone https://github.com/EvanNibbe/TriangleChess2.git
cd TriangleChess2
cd godot
scons target=editor CXXFLAGS=-O3 use_lto=yes platform=linuxbsd

# Compiling with MacOS
cd ~                                                                                                                <br>
brew install cmake                                                                                                  <br>
brew install python3                                                                                                <br>
git clone https://github.com/KhronosGroup/MoltenVK.git                                                              <br>
cd MoltenVK                                                                                                         <br>
./fetchDependencies --macos                                                                                         <br>
make macos                                                                                                          <br>
                                                                                                                    <br>
#cd back to the godot folder, presumably "cd &#126;/TriangleChess/godot"                                                 <br>
scons target=editor CXXFLAGS=-O3 use_lto=yes platform=macos vulkan_sdk_path=&#126;/MoltenVK                              <br>

## Contributing
The errors in my design of the AI opponent you are playing against can be found in the TriangleChess2/godot/scene/3d/light_3d.cpp.
To figure out where an error is happening, I have created a println(__LINE__); function that sends a single period to a file whose name is that line number; such files can be found in the TriangleChess2/TriangleChess folder.
Additionally, there is the print_word_num_on_line(char *str1, int put, int a); (where the last argument is always given as some variation of __LINE__ or __LINE__-1, etc, since the file those debug messages are sent to is named by the number in the last argument)
The place to start for finding where the error could be occurring would be "Node3D\* OmniLight3D::fast_make_move("
That line is the "hook" that returns a modified Node3D pointer to a piece owned by the AI (the function calls it as though it were one of the Array my_pieces) which is modified to include a Vector3 that contains the new location of where to move to.
It is then inside of the codeblock inside that function with the label "if (true || nth_optimal!=0) {", that codeblock is where the AI has been failing to find a good move, as it seems to consistently just choose the move that is the first valid pawn move instead of any other move.
Following the functions called from within that "if (true || nth_optimal!=0) {" codeblock will eventually allow you to find where the AI has gotten messed up.
