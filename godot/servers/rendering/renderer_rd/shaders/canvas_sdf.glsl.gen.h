/* WARNING, THIS FILE WAS GENERATED, DO NOT EDIT */
#ifndef CANVAS_SDF_GLSL_GEN_H_RD
#define CANVAS_SDF_GLSL_GEN_H_RD

#include "servers/rendering/renderer_rd/shader_rd.h"

class CanvasSdfShaderRD : public ShaderRD {

public:

	CanvasSdfShaderRD() {

		static const char _compute_code[] = {
10,35,118,101,114,115,105,111,110,32,52,53,48,10,10,35,86,69,82,83,73,79,78,95,68,69,70,73,78,69,83,10,10,108,97,121,111,117,116,40,108,111,99,97,108,95,115,105,122,101,95,120,32,61,32,56,44,32,108,111,99,97,108,95,115,105,122,101,95,121,32,61,32,56,44,32,108,111,99,97,108,95,115,105,122,101,95,122,32,61,32,49,41,32,105,110,59,10,10,108,97,121,111,117,116,40,114,56,44,32,115,101,116,32,61,32,48,44,32,98,105,110,100,105,110,103,32,61,32,49,41,32,117,110,105,102,111,114,109,32,114,101,115,116,114,105,99,116,32,114,101,97,100,111,110,108,121,32,105,109,97,103,101,50,68,32,115,114,99,95,112,105,120,101,108,115,59,10,108,97,121,111,117,116,40,114,49,54,95,115,110,111,114,109,44,32,115,101,116,32,61,32,48,44,32,98,105,110,100,105,110,103,32,61,32,50,41,32,117,110,105,102,111,114,109,32,114,101,115,116,114,105,99,116,32,119,114,105,116,101,111,110,108,121,32,105,109,97,103,101,50,68,32,100,115,116,95,115,100,102,59,10,10,108,97,121,111,117,116,40,114,103,49,54,105,44,32,115,101,116,32,61,32,48,44,32,98,105,110,100,105,110,103,32,61,32,51,41,32,117,110,105,102,111,114,109,32,114,101,115,116,114,105,99,116,32,114,101,97,100,111,110,108,121,32,105,105,109,97,103,101,50,68,32,115,114,99,95,112,114,111,99,101,115,115,59,10,108,97,121,111,117,116,40,114,103,49,54,105,44,32,115,101,116,32,61,32,48,44,32,98,105,110,100,105,110,103,32,61,32,52,41,32,117,110,105,102,111,114,109,32,114,101,115,116,114,105,99,116,32,119,114,105,116,101,111,110,108,121,32,105,105,109,97,103,101,50,68,32,100,115,116,95,112,114,111,99,101,115,115,59,10,10,108,97,121,111,117,116,40,112,117,115,104,95,99,111,110,115,116,97,110,116,44,32,115,116,100,52,51,48,41,32,117,110,105,102,111,114,109,32,80,97,114,97,109,115,32,123,10,9,105,118,101,99,50,32,115,105,122,101,59,10,9,105,110,116,32,115,116,114,105,100,101,59,10,9,105,110,116,32,115,104,105,102,116,59,10,9,105,118,101,99,50,32,98,97,115,101,95,115,105,122,101,59,10,9,117,118,101,99,50,32,112,97,100,59,10,125,10,112,97,114,97,109,115,59,10,10,35,100,101,102,105,110,101,32,83,68,70,95,77,65,88,95,76,69,78,71,84,72,32,49,54,51,56,52,46,48,10,10,118,111,105,100,32,109,97,105,110,40,41,32,123,10,9,105,118,101,99,50,32,112,111,115,32,61,32,105,118,101,99,50,40,103,108,95,71,108,111,98,97,108,73,110,118,111,99,97,116,105,111,110,73,68,46,120,121,41,59,10,9,105,102,32,40,97,110,121,40,103,114,101,97,116,101,114,84,104,97,110,69,113,117,97,108,40,112,111,115,44,32,112,97,114,97,109,115,46,115,105,122,101,41,41,41,32,123,32,10,9,9,114,101,116,117,114,110,59,10,9,125,10,10,35,105,102,100,101,102,32,77,79,68,69,95,76,79,65,68,10,10,9,98,111,111,108,32,115,111,108,105,100,32,61,32,105,109,97,103,101,76,111,97,100,40,115,114,99,95,112,105,120,101,108,115,44,32,112,111,115,41,46,114,32,62,32,48,46,53,59,10,9,105,109,97,103,101,83,116,111,114,101,40,100,115,116,95,112,114,111,99,101,115,115,44,32,112,111,115,44,32,115,111,108,105,100,32,63,32,105,118,101,99,52,40,105,118,101,99,50,40,45,51,50,55,54,55,41,44,32,48,44,32,48,41,32,58,32,105,118,101,99,52,40,105,118,101,99,50,40,51,50,55,54,55,41,44,32,48,44,32,48,41,41,59,10,35,101,110,100,105,102,10,10,35,105,102,100,101,102,32,77,79,68,69,95,76,79,65,68,95,83,72,82,73,78,75,10,10,9,105,110,116,32,115,32,61,32,49,32,60,60,32,112,97,114,97,109,115,46,115,104,105,102,116,59,10,9,105,118,101,99,50,32,98,97,115,101,32,61,32,112,111,115,32,60,60,32,112,97,114,97,109,115,46,115,104,105,102,116,59,10,9,105,118,101,99,50,32,99,101,110,116,101,114,32,61,32,98,97,115,101,32,43,32,105,118,101,99,50,40,112,97,114,97,109,115,46,115,104,105,102,116,41,59,10,10,9,105,118,101,99,50,32,114,101,108,32,61,32,105,118,101,99,50,40,51,50,55,54,55,41,59,10,9,102,108,111,97,116,32,100,32,61,32,49,101,50,48,59,10,9,105,110,116,32,102,111,117,110,100,32,61,32,48,59,10,9,105,110,116,32,115,111,108,105,100,95,102,111,117,110,100,32,61,32,48,59,10,9,102,111,114,32,40,105,110,116,32,105,32,61,32,48,59,32,105,32,60,32,115,59,32,105,43,43,41,32,123,10,9,9,102,111,114,32,40,105,110,116,32,106,32,61,32,48,59,32,106,32,60,32,115,59,32,106,43,43,41,32,123,10,9,9,9,105,118,101,99,50,32,115,114,99,95,112,111,115,32,61,32,98,97,115,101,32,43,32,105,118,101,99,50,40,105,44,32,106,41,59,10,9,9,9,105,102,32,40,97,110,121,40,103,114,101,97,116,101,114,84,104,97,110,69,113,117,97,108,40,115,114,99,95,112,111,115,44,32,112,97,114,97,109,115,46,98,97,115,101,95,115,105,122,101,41,41,41,32,123,10,9,9,9,9,99,111,110,116,105,110,117,101,59,10,9,9,9,125,10,9,9,9,98,111,111,108,32,115,111,108,105,100,32,61,32,105,109,97,103,101,76,111,97,100,40,115,114,99,95,112,105,120,101,108,115,44,32,115,114,99,95,112,111,115,41,46,114,32,62,32,48,46,53,59,10,9,9,9,105,102,32,40,115,111,108,105,100,41,32,123,10,9,9,9,9,102,108,111,97,116,32,100,105,115,116,32,61,32,108,101,110,103,116,104,40,118,101,99,50,40,115,114,99,95,112,111,115,32,45,32,99,101,110,116,101,114,41,41,59,10,9,9,9,9,105,102,32,40,100,105,115,116,32,60,32,100,41,32,123,10,9,9,9,9,9,100,32,61,32,100,105,115,116,59,10,9,9,9,9,9,114,101,108,32,61,32,115,114,99,95,112,111,115,59,10,9,9,9,9,125,10,9,9,9,9,115,111,108,105,100,95,102,111,117,110,100,43,43,59,10,9,9,9,125,10,9,9,9,102,111,117,110,100,43,43,59,10,9,9,125,10,9,125,10,10,9,105,102,32,40,115,111,108,105,100,95,102,111,117,110,100,32,61,61,32,102,111,117,110,100,41,32,123,10,9,9,10,9,9,114,101,108,32,61,32,105,118,101,99,50,40,45,51,50,55,54,55,41,59,10,9,125,10,10,9,105,109,97,103,101,83,116,111,114,101,40,100,115,116,95,112,114,111,99,101,115,115,44,32,112,111,115,44,32,105,118,101,99,52,40,114,101,108,44,32,48,44,32,48,41,41,59,10,35,101,110,100,105,102,10,10,35,105,102,100,101,102,32,77,79,68,69,95,80,82,79,67,69,83,83,10,10,9,105,118,101,99,50,32,98,97,115,101,32,61,32,112,111,115,32,60,60,32,112,97,114,97,109,115,46,115,104,105,102,116,59,10,9,105,118,101,99,50,32,99,101,110,116,101,114,32,61,32,98,97,115,101,32,43,32,105,118,101,99,50,40,112,97,114,97,109,115,46,115,104,105,102,116,41,59,10,10,9,105,118,101,99,50,32,114,101,108,32,61,32,105,109,97,103,101,76,111,97,100,40,115,114,99,95,112,114,111,99,101,115,115,44,32,112,111,115,41,46,120,121,59,10,10,9,98,111,111,108,32,115,111,108,105,100,32,61,32,114,101,108,46,120,32,60,32,48,59,10,10,9,105,102,32,40,115,111,108,105,100,41,32,123,10,9,9,114,101,108,32,61,32,45,114,101,108,32,45,32,105,118,101,99,50,40,49,41,59,10,9,125,10,10,9,105,102,32,40,99,101,110,116,101,114,32,33,61,32,114,101,108,41,32,123,10,9,9,10,9,9,99,111,110,115,116,32,105,110,116,32,111,102,115,95,116,97,98,108,101,95,115,105,122,101,32,61,32,56,59,10,9,9,99,111,110,115,116,32,105,118,101,99,50,32,111,102,115,95,116,97,98,108,101,91,111,102,115,95,116,97,98,108,101,95,115,105,122,101,93,32,61,32,105,118,101,99,50,91,93,40,10,9,9,9,9,105,118,101,99,50,40,45,49,44,32,45,49,41,44,10,9,9,9,9,105,118,101,99,50,40,48,44,32,45,49,41,44,10,9,9,9,9,105,118,101,99,50,40,43,49,44,32,45,49,41,44,10,10,9,9,9,9,105,118,101,99,50,40,45,49,44,32,48,41,44,10,9,9,9,9,105,118,101,99,50,40,43,49,44,32,48,41,44,10,10,9,9,9,9,105,118,101,99,50,40,45,49,44,32,43,49,41,44,10,9,9,9,9,105,118,101,99,50,40,48,44,32,43,49,41,44,10,9,9,9,9,105,118,101,99,50,40,43,49,44,32,43,49,41,41,59,10,10,9,9,102,108,111,97,116,32,100,105,115,116,32,61,32,108,101,110,103,116,104,40,118,101,99,50,40,114,101,108,32,45,32,99,101,110,116,101,114,41,41,59,10,9,9,102,111,114,32,40,105,110,116,32,105,32,61,32,48,59,32,105,32,60,32,111,102,115,95,116,97,98,108,101,95,115,105,122,101,59,32,105,43,43,41,32,123,10,9,9,9,105,118,101,99,50,32,115,114,99,95,112,111,115,32,61,32,112,111,115,32,43,32,111,102,115,95,116,97,98,108,101,91,105,93,32,42,32,112,97,114,97,109,115,46,115,116,114,105,100,101,59,10,9,9,9,105,102,32,40,97,110,121,40,108,101,115,115,84,104,97,110,40,115,114,99,95,112,111,115,44,32,105,118,101,99,50,40,48,41,41,41,32,124,124,32,97,110,121,40,103,114,101,97,116,101,114,84,104,97,110,69,113,117,97,108,40,115,114,99,95,112,111,115,44,32,112,97,114,97,109,115,46,115,105,122,101,41,41,41,32,123,10,9,9,9,9,99,111,110,116,105,110,117,101,59,10,9,9,9,125,10,9,9,9,105,118,101,99,50,32,115,114,99,95,114,101,108,32,61,32,105,109,97,103,101,76,111,97,100,40,115,114,99,95,112,114,111,99,101,115,115,44,32,115,114,99,95,112,111,115,41,46,120,121,59,10,9,9,9,98,111,111,108,32,115,114,99,95,115,111,108,105,100,32,61,32,115,114,99,95,114,101,108,46,120,32,60,32,48,59,10,9,9,9,105,102,32,40,115,114,99,95,115,111,108,105,100,41,32,123,10,9,9,9,9,115,114,99,95,114,101,108,32,61,32,45,115,114,99,95,114,101,108,32,45,32,105,118,101,99,50,40,49,41,59,10,9,9,9,125,10,10,9,9,9,105,102,32,40,115,114,99,95,115,111,108,105,100,32,33,61,32,115,111,108,105,100,41,32,123,10,9,9,9,9,115,114,99,95,114,101,108,32,61,32,105,118,101,99,50,40,115,114,99,95,112,111,115,32,60,60,32,112,97,114,97,109,115,46,115,104,105,102,116,41,59,32,10,9,9,9,125,10,10,9,9,9,102,108,111,97,116,32,115,114,99,95,100,105,115,116,32,61,32,108,101,110,103,116,104,40,118,101,99,50,40,115,114,99,95,114,101,108,32,45,32,99,101,110,116,101,114,41,41,59,10,9,9,9,105,102,32,40,115,114,99,95,100,105,115,116,32,60,32,100,105,115,116,41,32,123,10,9,9,9,9,100,105,115,116,32,61,32,115,114,99,95,100,105,115,116,59,10,9,9,9,9,114,101,108,32,61,32,115,114,99,95,114,101,108,59,10,9,9,9,125,10,9,9,125,10,9,125,10,10,9,105,102,32,40,115,111,108,105,100,41,32,123,10,9,9,114,101,108,32,61,32,45,114,101,108,32,45,32,105,118,101,99,50,40,49,41,59,10,9,125,10,10,9,105,109,97,103,101,83,116,111,114,101,40,100,115,116,95,112,114,111,99,101,115,115,44,32,112,111,115,44,32,105,118,101,99,52,40,114,101,108,44,32,48,44,32,48,41,41,59,10,35,101,110,100,105,102,10,10,35,105,102,100,101,102,32,77,79,68,69,95,83,84,79,82,69,10,10,9,105,118,101,99,50,32,114,101,108,32,61,32,105,109,97,103,101,76,111,97,100,40,115,114,99,95,112,114,111,99,101,115,115,44,32,112,111,115,41,46,120,121,59,10,10,9,98,111,111,108,32,115,111,108,105,100,32,61,32,114,101,108,46,120,32,60,32,48,59,10,10,9,105,102,32,40,115,111,108,105,100,41,32,123,10,9,9,114,101,108,32,61,32,45,114,101,108,32,45,32,105,118,101,99,50,40,49,41,59,10,9,125,10,10,9,102,108,111,97,116,32,100,32,61,32,108,101,110,103,116,104,40,118,101,99,50,40,114,101,108,32,45,32,112,111,115,41,41,59,10,10,9,105,102,32,40,115,111,108,105,100,41,32,123,10,9,9,100,32,61,32,45,100,59,10,9,125,10,10,9,100,32,47,61,32,83,68,70,95,77,65,88,95,76,69,78,71,84,72,59,10,9,100,32,61,32,99,108,97,109,112,40,100,44,32,45,49,46,48,44,32,49,46,48,41,59,10,9,105,109,97,103,101,83,116,111,114,101,40,100,115,116,95,115,100,102,44,32,112,111,115,44,32,118,101,99,52,40,100,41,41,59,10,10,35,101,110,100,105,102,10,10,35,105,102,100,101,102,32,77,79,68,69,95,83,84,79,82,69,95,83,72,82,73,78,75,10,10,9,105,118,101,99,50,32,98,97,115,101,32,61,32,112,111,115,32,60,60,32,112,97,114,97,109,115,46,115,104,105,102,116,59,10,9,105,118,101,99,50,32,99,101,110,116,101,114,32,61,32,98,97,115,101,32,43,32,105,118,101,99,50,40,112,97,114,97,109,115,46,115,104,105,102,116,41,59,10,10,9,105,118,101,99,50,32,114,101,108,32,61,32,105,109,97,103,101,76,111,97,100,40,115,114,99,95,112,114,111,99,101,115,115,44,32,112,111,115,41,46,120,121,59,10,10,9,98,111,111,108,32,115,111,108,105,100,32,61,32,114,101,108,46,120,32,60,32,48,59,10,10,9,105,102,32,40,115,111,108,105,100,41,32,123,10,9,9,114,101,108,32,61,32,45,114,101,108,32,45,32,105,118,101,99,50,40,49,41,59,10,9,125,10,10,9,102,108,111,97,116,32,100,32,61,32,108,101,110,103,116,104,40,118,101,99,50,40,114,101,108,32,45,32,99,101,110,116,101,114,41,41,59,10,10,9,105,102,32,40,115,111,108,105,100,41,32,123,10,9,9,100,32,61,32,45,100,59,10,9,125,10,9,100,32,47,61,32,83,68,70,95,77,65,88,95,76,69,78,71,84,72,59,10,9,100,32,61,32,99,108,97,109,112,40,100,44,32,45,49,46,48,44,32,49,46,48,41,59,10,9,105,109,97,103,101,83,116,111,114,101,40,100,115,116,95,115,100,102,44,32,112,111,115,44,32,118,101,99,52,40,100,41,41,59,10,10,35,101,110,100,105,102,10,125,10,0
		};
		setup(nullptr, nullptr, _compute_code, "CanvasSdfShaderRD");
	}
};

#endif