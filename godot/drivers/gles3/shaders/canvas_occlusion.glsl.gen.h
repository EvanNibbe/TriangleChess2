/* WARNING, THIS FILE WAS GENERATED, DO NOT EDIT */
#ifndef CANVAS_OCCLUSION_GLSL_GEN_HGLES3_GLES3
#define CANVAS_OCCLUSION_GLSL_GEN_HGLES3_GLES3


#include "drivers/gles3/shader_gles3.h"


class CanvasOcclusionShaderGLES3 : public ShaderGLES3 {

public:

	enum Uniforms {
		PROJECTION,
		MODELVIEW1,
		MODELVIEW2,
		DIRECTION,
		Z_FAR,
	};

	enum ShaderVariant {
		MODE_SDF,
		MODE_SHADOW,
		MODE_SHADOW_RGBA,
	};

	_FORCE_INLINE_ bool version_bind_shader(RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { return _version_bind_shader(p_version,p_variant,p_specialization); }

	_FORCE_INLINE_ int version_get_uniform(Uniforms p_uniform,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { return _version_get_uniform(p_uniform,p_version,p_variant,p_specialization); }

	#define _FU if (version_get_uniform(p_uniform,p_version,p_variant,p_specialization)<0) return; 

 	_FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, float p_value,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { _FU glUniform1f(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),p_value); }

	_FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, double p_value,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { _FU glUniform1f(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),p_value); }

	_FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, uint8_t p_value,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { _FU glUniform1ui(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),p_value); }

	_FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, int8_t p_value,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { _FU glUniform1i(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),p_value); }

	_FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, uint16_t p_value,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { _FU glUniform1ui(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),p_value); }

	_FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, int16_t p_value,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { _FU glUniform1i(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),p_value); }

	_FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, uint32_t p_value,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { _FU glUniform1ui(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),p_value); }

	_FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, int32_t p_value,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { _FU glUniform1i(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),p_value); }

	_FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, const Color& p_color,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { _FU GLfloat col[4]={p_color.r,p_color.g,p_color.b,p_color.a}; glUniform4fv(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),1,col); }

	_FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, const Vector2& p_vec2,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { _FU GLfloat vec2[2]={float(p_vec2.x),float(p_vec2.y)}; glUniform2fv(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),1,vec2); }

	_FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, const Size2i& p_vec2,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { _FU GLint vec2[2]={GLint(p_vec2.x),GLint(p_vec2.y)}; glUniform2iv(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),1,vec2); }

	_FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, const Vector3& p_vec3,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { _FU GLfloat vec3[3]={float(p_vec3.x),float(p_vec3.y),float(p_vec3.z)}; glUniform3fv(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),1,vec3); }

	_FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, float p_a, float p_b,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { _FU glUniform2f(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),p_a,p_b); }

	_FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, float p_a, float p_b, float p_c,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { _FU glUniform3f(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),p_a,p_b,p_c); }

	_FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, float p_a, float p_b, float p_c, float p_d,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) { _FU glUniform4f(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),p_a,p_b,p_c,p_d); }

	_FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, const Transform3D& p_transform,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) {  _FU

            const Transform3D &tr = p_transform;

            GLfloat matrix[16]={ /* build a 16x16 matrix */
                (GLfloat)tr.basis.rows[0][0],
                (GLfloat)tr.basis.rows[1][0],
                (GLfloat)tr.basis.rows[2][0],
                (GLfloat)0,
                (GLfloat)tr.basis.rows[0][1],
                (GLfloat)tr.basis.rows[1][1],
                (GLfloat)tr.basis.rows[2][1],
                (GLfloat)0,
                (GLfloat)tr.basis.rows[0][2],
                (GLfloat)tr.basis.rows[1][2],
                (GLfloat)tr.basis.rows[2][2],
                (GLfloat)0,
                (GLfloat)tr.origin.x,
                (GLfloat)tr.origin.y,
                (GLfloat)tr.origin.z,
                (GLfloat)1
            };

                    glUniformMatrix4fv(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),1,false,matrix);

        }

        _FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, const Transform2D& p_transform,RID p_version,ShaderVariant p_variant,uint64_t p_specialization=0) {  _FU

            const Transform2D &tr = p_transform;

        GLfloat matrix[16]={ /* build a 16x16 matrix */
            (GLfloat)tr.columns[0][0],
            (GLfloat)tr.columns[0][1],
            (GLfloat)0,
            (GLfloat)0,
            (GLfloat)tr.columns[1][0],
            (GLfloat)tr.columns[1][1],
            (GLfloat)0,
            (GLfloat)0,
            (GLfloat)0,
            (GLfloat)0,
            (GLfloat)1,
            (GLfloat)0,
            (GLfloat)tr.columns[2][0],
            (GLfloat)tr.columns[2][1],
            (GLfloat)0,
            (GLfloat)1
        };

            glUniformMatrix4fv(version_get_uniform(p_uniform,p_version,p_variant,p_specialization),1,false,matrix);

        }

        _FORCE_INLINE_ void version_set_uniform(Uniforms p_uniform, const Projection& p_matrix, RID p_version, ShaderVariant p_variant,uint64_t p_specialization=0) {  _FU

            GLfloat matrix[16];

            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    matrix[i * 4 + j] = p_matrix.columns[i][j];
                }
            }

            glUniformMatrix4fv(version_get_uniform(p_uniform, p_version, p_variant, p_specialization), 1, false, matrix);
    }

#undef _FU


protected:

	virtual void _init() override {

		static const char* _uniform_strings[]={
			"projection",
			"modelview1",
			"modelview2",
			"direction",
			"z_far",
		};

		static const char* _variant_defines[]={
			"",
			"#define MODE_SHADOW",
			"#define MODE_SHADOW \n#define USE_RGBA_SHADOWS",
		};

		static TexUnitPair *_texunit_pairs=nullptr;
		static UBOPair *_ubo_pairs=nullptr;
		static Specialization *_spec_pairs=nullptr;
		static const Feedback* _feedbacks=nullptr;
		static const char _vertex_code[]={
10,108,97,121,111,117,116,40,108,111,99,97,116,105,111,110,32,61,32,48,41,32,105,110,32,118,101,99,51,32,118,101,114,116,101,120,59,10,10,117,110,105,102,111,114,109,32,104,105,103,104,112,32,109,97,116,52,32,112,114,111,106,101,99,116,105,111,110,59,10,117,110,105,102,111,114,109,32,104,105,103,104,112,32,118,101,99,52,32,109,111,100,101,108,118,105,101,119,49,59,10,117,110,105,102,111,114,109,32,104,105,103,104,112,32,118,101,99,52,32,109,111,100,101,108,118,105,101,119,50,59,10,117,110,105,102,111,114,109,32,104,105,103,104,112,32,118,101,99,50,32,100,105,114,101,99,116,105,111,110,59,10,117,110,105,102,111,114,109,32,104,105,103,104,112,32,102,108,111,97,116,32,122,95,102,97,114,59,10,10,35,105,102,100,101,102,32,77,79,68,69,95,83,72,65,68,79,87,10,111,117,116,32,102,108,111,97,116,32,100,101,112,116,104,59,10,35,101,110,100,105,102,10,10,118,111,105,100,32,109,97,105,110,40,41,32,123,10,9,104,105,103,104,112,32,118,101,99,52,32,118,116,120,32,61,32,118,101,99,52,40,118,101,114,116,101,120,44,32,49,46,48,41,32,42,32,109,97,116,52,40,109,111,100,101,108,118,105,101,119,49,44,32,109,111,100,101,108,118,105,101,119,50,44,32,118,101,99,52,40,48,46,48,44,32,48,46,48,44,32,49,46,48,44,32,48,46,48,41,44,32,118,101,99,52,40,48,46,48,44,32,48,46,48,44,32,48,46,48,44,32,49,46,48,41,41,59,10,10,35,105,102,100,101,102,32,77,79,68,69,95,83,72,65,68,79,87,10,9,100,101,112,116,104,32,61,32,100,111,116,40,100,105,114,101,99,116,105,111,110,44,32,118,116,120,46,120,121,41,59,10,35,101,110,100,105,102,10,9,103,108,95,80,111,115,105,116,105,111,110,32,61,32,112,114,111,106,101,99,116,105,111,110,32,42,32,118,116,120,59,10,125,10,10,		0};

		static const char _fragment_code[]={
10,10,117,110,105,102,111,114,109,32,104,105,103,104,112,32,109,97,116,52,32,112,114,111,106,101,99,116,105,111,110,59,10,117,110,105,102,111,114,109,32,104,105,103,104,112,32,118,101,99,52,32,109,111,100,101,108,118,105,101,119,49,59,10,117,110,105,102,111,114,109,32,104,105,103,104,112,32,118,101,99,52,32,109,111,100,101,108,118,105,101,119,50,59,10,117,110,105,102,111,114,109,32,104,105,103,104,112,32,118,101,99,50,32,100,105,114,101,99,116,105,111,110,59,10,117,110,105,102,111,114,109,32,104,105,103,104,112,32,102,108,111,97,116,32,122,95,102,97,114,59,10,10,35,105,102,100,101,102,32,77,79,68,69,95,83,72,65,68,79,87,10,105,110,32,104,105,103,104,112,32,102,108,111,97,116,32,100,101,112,116,104,59,10,35,101,110,100,105,102,10,10,35,105,102,100,101,102,32,85,83,69,95,82,71,66,65,95,83,72,65,68,79,87,83,10,108,97,121,111,117,116,40,108,111,99,97,116,105,111,110,32,61,32,48,41,32,111,117,116,32,108,111,119,112,32,118,101,99,52,32,111,117,116,95,98,117,102,59,10,35,101,108,115,101,10,108,97,121,111,117,116,40,108,111,99,97,116,105,111,110,32,61,32,48,41,32,111,117,116,32,104,105,103,104,112,32,102,108,111,97,116,32,111,117,116,95,98,117,102,59,10,35,101,110,100,105,102,10,10,118,111,105,100,32,109,97,105,110,40,41,32,123,10,32,32,32,32,102,108,111,97,116,32,111,117,116,95,100,101,112,116,104,32,61,32,49,46,48,59,10,10,35,105,102,100,101,102,32,77,79,68,69,95,83,72,65,68,79,87,10,9,111,117,116,95,100,101,112,116,104,32,61,32,100,101,112,116,104,32,47,32,122,95,102,97,114,59,10,35,101,110,100,105,102,10,10,35,105,102,100,101,102,32,85,83,69,95,82,71,66,65,95,83,72,65,68,79,87,83,10,9,111,117,116,95,100,101,112,116,104,32,61,32,99,108,97,109,112,40,111,117,116,95,100,101,112,116,104,44,32,45,49,46,48,44,32,49,46,48,41,59,10,9,111,117,116,95,100,101,112,116,104,32,61,32,111,117,116,95,100,101,112,116,104,32,42,32,48,46,53,32,43,32,48,46,53,59,10,9,104,105,103,104,112,32,118,101,99,52,32,99,111,109,112,32,61,32,102,114,97,99,116,40,111,117,116,95,100,101,112,116,104,32,42,32,118,101,99,52,40,50,53,53,46,48,32,42,32,50,53,53,46,48,32,42,32,50,53,53,46,48,44,32,50,53,53,46,48,32,42,32,50,53,53,46,48,44,32,50,53,53,46,48,44,32,49,46,48,41,41,59,10,9,99,111,109,112,32,45,61,32,99,111,109,112,46,120,120,121,122,32,42,32,118,101,99,52,40,48,46,48,44,32,49,46,48,32,47,32,50,53,53,46,48,44,32,49,46,48,32,47,32,50,53,53,46,48,44,32,49,46,48,32,47,32,50,53,53,46,48,41,59,10,9,111,117,116,95,98,117,102,32,61,32,99,111,109,112,59,10,35,101,108,115,101,10,9,111,117,116,95,98,117,102,32,61,32,111,117,116,95,100,101,112,116,104,59,10,35,101,110,100,105,102,10,125,10,		0};

		_setup(_vertex_code,_fragment_code,"CanvasOcclusionShaderGLES3",5,_uniform_strings,0,_ubo_pairs,0,_feedbacks,0,_texunit_pairs,0,_spec_pairs,3,_variant_defines);
	}

};

#endif

