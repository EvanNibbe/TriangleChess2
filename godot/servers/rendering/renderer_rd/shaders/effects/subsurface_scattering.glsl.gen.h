/* WARNING, THIS FILE WAS GENERATED, DO NOT EDIT */
#ifndef SUBSURFACE_SCATTERING_GLSL_GEN_H_RD
#define SUBSURFACE_SCATTERING_GLSL_GEN_H_RD

#include "servers/rendering/renderer_rd/shader_rd.h"

class SubsurfaceScatteringShaderRD : public ShaderRD {

public:

	SubsurfaceScatteringShaderRD() {

		static const char _compute_code[] = {
10,35,118,101,114,115,105,111,110,32,52,53,48,10,10,35,86,69,82,83,73,79,78,95,68,69,70,73,78,69,83,10,10,108,97,121,111,117,116,40,108,111,99,97,108,95,115,105,122,101,95,120,32,61,32,56,44,32,108,111,99,97,108,95,115,105,122,101,95,121,32,61,32,56,44,32,108,111,99,97,108,95,115,105,122,101,95,122,32,61,32,49,41,32,105,110,59,10,10,35,105,102,100,101,102,32,85,83,69,95,50,53,95,83,65,77,80,76,69,83,10,99,111,110,115,116,32,105,110,116,32,107,101,114,110,101,108,95,115,105,122,101,32,61,32,49,51,59,10,10,99,111,110,115,116,32,118,101,99,50,32,107,101,114,110,101,108,91,107,101,114,110,101,108,95,115,105,122,101,93,32,61,32,118,101,99,50,91,93,40,10,9,9,118,101,99,50,40,48,46,53,51,48,54,48,53,44,32,48,46,48,41,44,10,9,9,118,101,99,50,40,48,46,48,50,49,49,52,49,50,44,32,48,46,48,50,48,56,51,51,51,41,44,10,9,9,118,101,99,50,40,48,46,48,52,48,50,55,56,52,44,32,48,46,48,56,51,51,51,51,51,41,44,10,9,9,118,101,99,50,40,48,46,48,52,57,51,53,56,56,44,32,48,46,49,56,55,53,41,44,10,9,9,118,101,99,50,40,48,46,48,52,49,48,49,55,50,44,32,48,46,51,51,51,51,51,51,41,44,10,9,9,118,101,99,50,40,48,46,48,50,54,51,54,52,50,44,32,48,46,53,50,48,56,51,51,41,44,10,9,9,118,101,99,50,40,48,46,48,49,55,57,50,52,44,32,48,46,55,53,41,44,10,9,9,118,101,99,50,40,48,46,48,49,50,56,52,57,54,44,32,49,46,48,50,48,56,51,41,44,10,9,9,118,101,99,50,40,48,46,48,48,57,52,51,56,57,44,32,49,46,51,51,51,51,51,41,44,10,9,9,118,101,99,50,40,48,46,48,48,55,48,48,57,55,54,44,32,49,46,54,56,55,53,41,44,10,9,9,118,101,99,50,40,48,46,48,48,53,48,48,51,54,52,44,32,50,46,48,56,51,51,51,41,44,10,9,9,118,101,99,50,40,48,46,48,48,51,51,51,56,48,52,44,32,50,46,53,50,48,56,51,41,44,10,9,9,118,101,99,50,40,48,46,48,48,48,57,55,51,55,57,52,44,32,51,46,48,41,41,59,10,10,99,111,110,115,116,32,118,101,99,52,32,115,107,105,110,95,107,101,114,110,101,108,91,107,101,114,110,101,108,95,115,105,122,101,93,32,61,32,118,101,99,52,91,93,40,10,9,9,118,101,99,52,40,48,46,53,51,48,54,48,53,44,32,48,46,54,49,51,53,49,52,44,32,48,46,55,51,57,54,48,49,44,32,48,41,44,10,9,9,118,101,99,52,40,48,46,48,50,49,49,52,49,50,44,32,48,46,48,52,53,57,50,56,54,44,32,48,46,48,51,55,56,49,57,54,44,32,48,46,48,50,48,56,51,51,51,41,44,10,9,9,118,101,99,52,40,48,46,48,52,48,50,55,56,52,44,32,48,46,48,54,53,55,50,52,52,44,32,48,46,48,52,54,51,49,44,32,48,46,48,56,51,51,51,51,51,41,44,10,9,9,118,101,99,52,40,48,46,48,52,57,51,53,56,56,44,32,48,46,48,51,54,55,55,50,54,44,32,48,46,48,50,49,57,52,56,53,44,32,48,46,49,56,55,53,41,44,10,9,9,118,101,99,52,40,48,46,48,52,49,48,49,55,50,44,32,48,46,48,49,57,57,56,57,57,44,32,48,46,48,49,49,56,52,56,49,44,32,48,46,51,51,51,51,51,51,41,44,10,9,9,118,101,99,52,40,48,46,48,50,54,51,54,52,50,44,32,48,46,48,49,49,57,55,49,53,44,32,48,46,48,48,54,56,52,53,57,56,44,32,48,46,53,50,48,56,51,51,41,44,10,9,9,118,101,99,52,40,48,46,48,49,55,57,50,52,44,32,48,46,48,48,55,49,49,54,57,49,44,32,48,46,48,48,51,52,55,49,57,52,44,32,48,46,55,53,41,44,10,9,9,118,101,99,52,40,48,46,48,49,50,56,52,57,54,44,32,48,46,48,48,51,53,54,51,50,57,44,32,48,46,48,48,49,51,50,48,49,54,44,32,49,46,48,50,48,56,51,41,44,10,9,9,118,101,99,52,40,48,46,48,48,57,52,51,56,57,44,32,48,46,48,48,49,51,57,49,49,57,44,32,48,46,48,48,48,52,49,54,53,57,56,44,32,49,46,51,51,51,51,51,41,44,10,9,9,118,101,99,52,40,48,46,48,48,55,48,48,57,55,54,44,32,48,46,48,48,48,52,57,51,54,54,44,32,48,46,48,48,48,49,53,49,57,51,56,44,32,49,46,54,56,55,53,41,44,10,9,9,118,101,99,52,40,48,46,48,48,53,48,48,51,54,52,44,32,48,46,48,48,48,50,48,48,57,52,44,32,53,46,50,56,56,52,56,101,45,48,48,53,44,32,50,46,48,56,51,51,51,41,44,10,9,9,118,101,99,52,40,48,46,48,48,51,51,51,56,48,52,44,32,55,46,56,53,52,52,51,101,45,48,48,53,44,32,49,46,50,57,52,53,101,45,48,48,53,44,32,50,46,53,50,48,56,51,41,44,10,9,9,118,101,99,52,40,48,46,48,48,48,57,55,51,55,57,52,44,32,49,46,49,49,56,54,50,101,45,48,48,53,44,32,57,46,52,51,52,51,55,101,45,48,48,55,44,32,51,41,41,59,10,10,35,101,110,100,105,102,32,10,10,35,105,102,100,101,102,32,85,83,69,95,49,55,95,83,65,77,80,76,69,83,10,99,111,110,115,116,32,105,110,116,32,107,101,114,110,101,108,95,115,105,122,101,32,61,32,57,59,10,99,111,110,115,116,32,118,101,99,50,32,107,101,114,110,101,108,91,107,101,114,110,101,108,95,115,105,122,101,93,32,61,32,118,101,99,50,91,93,40,10,9,9,118,101,99,50,40,48,46,53,51,54,51,52,51,44,32,48,46,48,41,44,10,9,9,118,101,99,50,40,48,46,48,51,50,52,52,54,50,44,32,48,46,48,51,49,50,53,41,44,10,9,9,118,101,99,50,40,48,46,48,53,56,50,52,49,54,44,32,48,46,49,50,53,41,44,10,9,9,118,101,99,50,40,48,46,48,53,55,49,48,53,54,44,32,48,46,50,56,49,50,53,41,44,10,9,9,118,101,99,50,40,48,46,48,51,52,55,51,49,55,44,32,48,46,53,41,44,10,9,9,118,101,99,50,40,48,46,48,50,49,54,51,48,49,44,32,48,46,55,56,49,50,53,41,44,10,9,9,118,101,99,50,40,48,46,48,49,52,52,54,48,57,44,32,49,46,49,50,53,41,44,10,9,9,118,101,99,50,40,48,46,48,49,48,48,51,56,54,44,32,49,46,53,51,49,50,53,41,44,10,9,9,118,101,99,50,40,48,46,48,48,51,49,55,51,57,52,44,32,50,46,48,41,41,59,10,10,99,111,110,115,116,32,118,101,99,52,32,115,107,105,110,95,107,101,114,110,101,108,91,107,101,114,110,101,108,95,115,105,122,101,93,32,61,32,118,101,99,52,91,93,40,10,9,9,118,101,99,52,40,48,46,53,51,54,51,52,51,44,32,48,46,54,50,52,54,50,52,44,32,48,46,55,52,56,56,54,55,44,32,48,41,44,10,9,9,118,101,99,52,40,48,46,48,51,50,52,52,54,50,44,32,48,46,48,54,53,54,55,49,56,44,32,48,46,48,53,51,50,56,50,49,44,32,48,46,48,51,49,50,53,41,44,10,9,9,118,101,99,52,40,48,46,48,53,56,50,52,49,54,44,32,48,46,48,54,53,57,57,53,57,44,32,48,46,48,52,49,49,51,50,57,44,32,48,46,49,50,53,41,44,10,9,9,118,101,99,52,40,48,46,48,53,55,49,48,53,54,44,32,48,46,48,50,56,55,52,51,50,44,32,48,46,48,49,55,50,56,52,52,44,32,48,46,50,56,49,50,53,41,44,10,9,9,118,101,99,52,40,48,46,48,51,52,55,51,49,55,44,32,48,46,48,49,53,49,48,56,53,44,32,48,46,48,48,56,55,49,57,56,51,44,32,48,46,53,41,44,10,9,9,118,101,99,52,40,48,46,48,50,49,54,51,48,49,44,32,48,46,48,48,55,57,52,54,49,56,44,32,48,46,48,48,51,55,54,57,57,49,44,32,48,46,55,56,49,50,53,41,44,10,9,9,118,101,99,52,40,48,46,48,49,52,52,54,48,57,44,32,48,46,48,48,51,49,55,50,54,57,44,32,48,46,48,48,49,48,54,51,57,57,44,32,49,46,49,50,53,41,44,10,9,9,118,101,99,52,40,48,46,48,49,48,48,51,56,54,44,32,48,46,48,48,48,57,49,52,54,55,57,44,32,48,46,48,48,48,50,55,53,55,48,50,44,32,49,46,53,51,49,50,53,41,44,10,9,9,118,101,99,52,40,48,46,48,48,51,49,55,51,57,52,44,32,48,46,48,48,48,49,51,52,56,50,51,44,32,51,46,55,55,50,54,57,101,45,48,48,53,44,32,50,41,41,59,10,35,101,110,100,105,102,32,10,10,35,105,102,100,101,102,32,85,83,69,95,49,49,95,83,65,77,80,76,69,83,10,99,111,110,115,116,32,105,110,116,32,107,101,114,110,101,108,95,115,105,122,101,32,61,32,54,59,10,99,111,110,115,116,32,118,101,99,50,32,107,101,114,110,101,108,91,107,101,114,110,101,108,95,115,105,122,101,93,32,61,32,118,101,99,50,91,93,40,10,9,9,118,101,99,50,40,48,46,53,54,48,52,55,57,44,32,48,46,48,41,44,10,9,9,118,101,99,50,40,48,46,48,55,55,49,56,48,50,44,32,48,46,48,56,41,44,10,9,9,118,101,99,50,40,48,46,48,56,50,49,57,48,52,44,32,48,46,51,50,41,44,10,9,9,118,101,99,50,40,48,46,48,51,54,51,57,44,32,48,46,55,50,41,44,10,9,9,118,101,99,50,40,48,46,48,49,57,50,56,51,49,44,32,49,46,50,56,41,44,10,9,9,118,101,99,50,40,48,46,48,48,52,55,49,54,57,49,44,32,50,46,48,41,41,59,10,10,99,111,110,115,116,32,118,101,99,52,32,115,107,105,110,95,107,101,114,110,101,108,91,107,101,114,110,101,108,95,115,105,122,101,93,32,61,32,118,101,99,52,91,93,40,10,10,9,9,118,101,99,52,40,48,46,53,54,48,52,55,57,44,32,48,46,54,54,57,48,56,54,44,32,48,46,55,56,52,55,50,56,44,32,48,41,44,10,9,9,118,101,99,52,40,48,46,48,55,55,49,56,48,50,44,32,48,46,49,49,51,52,57,49,44,32,48,46,48,55,57,51,56,48,51,44,32,48,46,48,56,41,44,10,9,9,118,101,99,52,40,48,46,48,56,50,49,57,48,52,44,32,48,46,48,51,53,56,54,48,56,44,32,48,46,48,50,48,57,50,54,49,44,32,48,46,51,50,41,44,10,9,9,118,101,99,52,40,48,46,48,51,54,51,57,44,32,48,46,48,49,51,48,57,57,57,44,32,48,46,48,48,54,52,51,54,56,53,44,32,48,46,55,50,41,44,10,9,9,118,101,99,52,40,48,46,48,49,57,50,56,51,49,44,32,48,46,48,48,50,56,50,48,49,56,44,32,48,46,48,48,48,56,52,50,49,52,44,32,49,46,50,56,41,44,10,9,9,118,101,99,52,40,48,46,48,48,52,55,49,54,57,49,44,32,48,46,48,48,48,49,56,52,55,55,49,44,32,53,46,48,55,53,54,53,101,45,48,48,53,44,32,50,41,41,59,10,10,35,101,110,100,105,102,32,10,10,108,97,121,111,117,116,40,112,117,115,104,95,99,111,110,115,116,97,110,116,44,32,115,116,100,52,51,48,41,32,117,110,105,102,111,114,109,32,80,97,114,97,109,115,32,123,10,9,105,118,101,99,50,32,115,99,114,101,101,110,95,115,105,122,101,59,10,9,102,108,111,97,116,32,99,97,109,101,114,97,95,122,95,102,97,114,59,10,9,102,108,111,97,116,32,99,97,109,101,114,97,95,122,95,110,101,97,114,59,10,10,9,98,111,111,108,32,118,101,114,116,105,99,97,108,59,10,9,98,111,111,108,32,111,114,116,104,111,103,111,110,97,108,59,10,9,102,108,111,97,116,32,117,110,105,116,95,115,105,122,101,59,10,9,102,108,111,97,116,32,115,99,97,108,101,59,10,10,9,102,108,111,97,116,32,100,101,112,116,104,95,115,99,97,108,101,59,10,9,117,105,110,116,32,112,97,100,91,51,93,59,10,125,10,112,97,114,97,109,115,59,10,10,108,97,121,111,117,116,40,115,101,116,32,61,32,48,44,32,98,105,110,100,105,110,103,32,61,32,48,41,32,117,110,105,102,111,114,109,32,115,97,109,112,108,101,114,50,68,32,115,111,117,114,99,101,95,105,109,97,103,101,59,10,108,97,121,111,117,116,40,114,103,98,97,49,54,102,44,32,115,101,116,32,61,32,49,44,32,98,105,110,100,105,110,103,32,61,32,48,41,32,117,110,105,102,111,114,109,32,114,101,115,116,114,105,99,116,32,119,114,105,116,101,111,110,108,121,32,105,109,97,103,101,50,68,32,100,101,115,116,95,105,109,97,103,101,59,10,108,97,121,111,117,116,40,115,101,116,32,61,32,50,44,32,98,105,110,100,105,110,103,32,61,32,48,41,32,117,110,105,102,111,114,109,32,115,97,109,112,108,101,114,50,68,32,115,111,117,114,99,101,95,100,101,112,116,104,59,10,10,118,111,105,100,32,100,111,95,102,105,108,116,101,114,40,105,110,111,117,116,32,118,101,99,51,32,99,111,108,111,114,95,97,99,99,117,109,44,32,105,110,111,117,116,32,118,101,99,51,32,100,105,118,105,115,111,114,44,32,118,101,99,50,32,117,118,44,32,118,101,99,50,32,115,116,101,112,44,32,98,111,111,108,32,112,95,115,107,105,110,41,32,123,10,9,10,9,102,111,114,32,40,105,110,116,32,105,32,61,32,49,59,32,105,32,60,32,107,101,114,110,101,108,95,115,105,122,101,59,32,105,43,43,41,32,123,10,9,9,10,9,9,118,101,99,50,32,111,102,102,115,101,116,32,61,32,117,118,32,43,32,107,101,114,110,101,108,91,105,93,46,121,32,42,32,115,116,101,112,59,10,9,9,118,101,99,52,32,99,111,108,111,114,32,61,32,116,101,120,116,117,114,101,40,115,111,117,114,99,101,95,105,109,97,103,101,44,32,111,102,102,115,101,116,41,59,10,10,9,9,105,102,32,40,97,98,115,40,99,111,108,111,114,46,97,41,32,60,32,48,46,48,48,49,41,32,123,10,9,9,9,98,114,101,97,107,59,32,10,9,9,125,10,10,9,9,118,101,99,51,32,119,59,10,9,9,105,102,32,40,112,95,115,107,105,110,41,32,123,10,9,9,9,10,9,9,9,119,32,61,32,115,107,105,110,95,107,101,114,110,101,108,91,105,93,46,114,103,98,59,10,9,9,125,32,101,108,115,101,32,123,10,9,9,9,119,32,61,32,118,101,99,51,40,107,101,114,110,101,108,91,105,93,46,120,41,59,10,9,9,125,10,10,9,9,99,111,108,111,114,95,97,99,99,117,109,32,43,61,32,99,111,108,111,114,46,114,103,98,32,42,32,119,59,10,9,9,100,105,118,105,115,111,114,32,43,61,32,119,59,10,9,125,10,125,10,10,118,111,105,100,32,109,97,105,110,40,41,32,123,10,9,10,9,105,118,101,99,50,32,115,115,67,32,61,32,105,118,101,99,50,40,103,108,95,71,108,111,98,97,108,73,110,118,111,99,97,116,105,111,110,73,68,46,120,121,41,59,10,10,9,105,102,32,40,97,110,121,40,103,114,101,97,116,101,114,84,104,97,110,69,113,117,97,108,40,115,115,67,44,32,112,97,114,97,109,115,46,115,99,114,101,101,110,95,115,105,122,101,41,41,41,32,123,32,10,9,9,114,101,116,117,114,110,59,10,9,125,10,10,9,118,101,99,50,32,117,118,32,61,32,40,118,101,99,50,40,115,115,67,41,32,43,32,48,46,53,41,32,47,32,118,101,99,50,40,112,97,114,97,109,115,46,115,99,114,101,101,110,95,115,105,122,101,41,59,10,10,9,10,9,118,101,99,52,32,98,97,115,101,95,99,111,108,111,114,32,61,32,116,101,120,116,117,114,101,40,115,111,117,114,99,101,95,105,109,97,103,101,44,32,117,118,41,59,10,9,102,108,111,97,116,32,115,116,114,101,110,103,116,104,32,61,32,97,98,115,40,98,97,115,101,95,99,111,108,111,114,46,97,41,59,10,10,9,105,102,32,40,115,116,114,101,110,103,116,104,32,62,32,48,46,48,41,32,123,10,9,9,118,101,99,50,32,100,105,114,32,61,32,112,97,114,97,109,115,46,118,101,114,116,105,99,97,108,32,63,32,118,101,99,50,40,48,46,48,44,32,49,46,48,41,32,58,32,118,101,99,50,40,49,46,48,44,32,48,46,48,41,59,10,10,9,9,10,9,9,102,108,111,97,116,32,100,101,112,116,104,32,61,32,116,101,120,116,117,114,101,40,115,111,117,114,99,101,95,100,101,112,116,104,44,32,117,118,41,46,114,32,42,32,50,46,48,32,45,32,49,46,48,59,10,9,9,102,108,111,97,116,32,100,101,112,116,104,95,115,99,97,108,101,59,10,10,9,9,105,102,32,40,112,97,114,97,109,115,46,111,114,116,104,111,103,111,110,97,108,41,32,123,10,9,9,9,100,101,112,116,104,32,61,32,40,40,100,101,112,116,104,32,43,32,40,112,97,114,97,109,115,46,99,97,109,101,114,97,95,122,95,102,97,114,32,43,32,112,97,114,97,109,115,46,99,97,109,101,114,97,95,122,95,110,101,97,114,41,32,47,32,40,112,97,114,97,109,115,46,99,97,109,101,114,97,95,122,95,102,97,114,32,45,32,112,97,114,97,109,115,46,99,97,109,101,114,97,95,122,95,110,101,97,114,41,41,32,42,32,40,112,97,114,97,109,115,46,99,97,109,101,114,97,95,122,95,102,97,114,32,45,32,112,97,114,97,109,115,46,99,97,109,101,114,97,95,122,95,110,101,97,114,41,41,32,47,32,50,46,48,59,10,9,9,9,100,101,112,116,104,95,115,99,97,108,101,32,61,32,112,97,114,97,109,115,46,117,110,105,116,95,115,105,122,101,59,32,10,9,9,125,32,101,108,115,101,32,123,10,9,9,9,100,101,112,116,104,32,61,32,50,46,48,32,42,32,112,97,114,97,109,115,46,99,97,109,101,114,97,95,122,95,110,101,97,114,32,42,32,112,97,114,97,109,115,46,99,97,109,101,114,97,95,122,95,102,97,114,32,47,32,40,112,97,114,97,109,115,46,99,97,109,101,114,97,95,122,95,102,97,114,32,43,32,112,97,114,97,109,115,46,99,97,109,101,114,97,95,122,95,110,101,97,114,32,45,32,100,101,112,116,104,32,42,32,40,112,97,114,97,109,115,46,99,97,109,101,114,97,95,122,95,102,97,114,32,45,32,112,97,114,97,109,115,46,99,97,109,101,114,97,95,122,95,110,101,97,114,41,41,59,10,9,9,9,100,101,112,116,104,95,115,99,97,108,101,32,61,32,112,97,114,97,109,115,46,117,110,105,116,95,115,105,122,101,32,47,32,100,101,112,116,104,59,32,10,9,9,125,10,10,9,9,102,108,111,97,116,32,115,99,97,108,101,32,61,32,109,105,120,40,112,97,114,97,109,115,46,115,99,97,108,101,44,32,100,101,112,116,104,95,115,99,97,108,101,44,32,112,97,114,97,109,115,46,100,101,112,116,104,95,115,99,97,108,101,41,59,10,10,9,9,10,9,9,118,101,99,50,32,115,116,101,112,32,61,32,115,99,97,108,101,32,42,32,100,105,114,59,10,9,9,115,116,101,112,32,42,61,32,115,116,114,101,110,103,116,104,59,10,9,9,115,116,101,112,32,47,61,32,51,46,48,59,10,9,9,10,10,9,9,118,101,99,51,32,100,105,118,105,115,111,114,59,10,9,9,98,111,111,108,32,115,107,105,110,32,61,32,98,111,111,108,40,98,97,115,101,95,99,111,108,111,114,46,97,32,60,32,48,46,48,41,59,10,10,9,9,105,102,32,40,115,107,105,110,41,32,123,10,9,9,9,10,9,9,9,100,105,118,105,115,111,114,32,61,32,115,107,105,110,95,107,101,114,110,101,108,91,48,93,46,114,103,98,59,10,9,9,125,32,101,108,115,101,32,123,10,9,9,9,100,105,118,105,115,111,114,32,61,32,118,101,99,51,40,107,101,114,110,101,108,91,48,93,46,120,41,59,10,9,9,125,10,10,9,9,118,101,99,51,32,99,111,108,111,114,32,61,32,98,97,115,101,95,99,111,108,111,114,46,114,103,98,32,42,32,100,105,118,105,115,111,114,59,10,10,9,9,100,111,95,102,105,108,116,101,114,40,99,111,108,111,114,44,32,100,105,118,105,115,111,114,44,32,117,118,44,32,115,116,101,112,44,32,115,107,105,110,41,59,10,9,9,100,111,95,102,105,108,116,101,114,40,99,111,108,111,114,44,32,100,105,118,105,115,111,114,44,32,117,118,44,32,45,115,116,101,112,44,32,115,107,105,110,41,59,10,10,9,9,98,97,115,101,95,99,111,108,111,114,46,114,103,98,32,61,32,99,111,108,111,114,32,47,32,100,105,118,105,115,111,114,59,10,9,125,10,10,9,105,109,97,103,101,83,116,111,114,101,40,100,101,115,116,95,105,109,97,103,101,44,32,115,115,67,44,32,98,97,115,101,95,99,111,108,111,114,41,59,10,125,10,0
		};
		setup(nullptr, nullptr, _compute_code, "SubsurfaceScatteringShaderRD");
	}
};

#endif
