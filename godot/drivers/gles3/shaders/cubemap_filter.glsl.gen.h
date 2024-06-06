/* WARNING, THIS FILE WAS GENERATED, DO NOT EDIT */
#ifndef CUBEMAP_FILTER_GLSL_GEN_HGLES3_GLES3
#define CUBEMAP_FILTER_GLSL_GEN_HGLES3_GLES3


#include "drivers/gles3/shader_gles3.h"


class CubemapFilterShaderGLES3 : public ShaderGLES3 {

public:

	enum Uniforms {
		FACE_ID,
		SAMPLE_COUNT,
		SAMPLE_DIRECTIONS_MIP,
		WEIGHT,
	};

	enum ShaderVariant {
		MODE_DEFAULT,
		MODE_COPY,
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
			"face_id",
			"sample_count",
			"sample_directions_mip",
			"weight",
		};

		static const char* _variant_defines[]={
			"",
			"#define MODE_DIRECT_WRITE",
		};

		static TexUnitPair _texunit_pairs[]={
			{"source_cube",0},
		};

		static UBOPair *_ubo_pairs=nullptr;
		static Specialization *_spec_pairs=nullptr;
		static const Feedback* _feedbacks=nullptr;
		static const char _vertex_code[]={
10,108,97,121,111,117,116,40,108,111,99,97,116,105,111,110,32,61,32,48,41,32,105,110,32,104,105,103,104,112,32,118,101,99,50,32,118,101,114,116,101,120,95,97,116,116,114,105,98,59,10,47,42,32,99,108,97,110,103,45,102,111,114,109,97,116,32,111,110,32,42,47,10,10,111,117,116,32,104,105,103,104,112,32,118,101,99,50,32,117,118,95,105,110,116,101,114,112,59,10,10,118,111,105,100,32,109,97,105,110,40,41,32,123,10,9,117,118,95,105,110,116,101,114,112,32,61,32,118,101,114,116,101,120,95,97,116,116,114,105,98,59,10,9,103,108,95,80,111,115,105,116,105,111,110,32,61,32,118,101,99,52,40,117,118,95,105,110,116,101,114,112,44,32,48,46,48,44,32,49,46,48,41,59,10,125,10,10,47,42,32,99,108,97,110,103,45,102,111,114,109,97,116,32,111,102,102,32,42,47,10,		0};

		static const char _fragment_code[]={
10,10,35,100,101,102,105,110,101,32,77,95,80,73,32,51,46,49,52,49,53,57,50,54,53,51,53,57,10,10,117,110,105,102,111,114,109,32,115,97,109,112,108,101,114,67,117,98,101,32,115,111,117,114,99,101,95,99,117,98,101,59,32,47,47,116,101,120,117,110,105,116,58,48,10,10,47,42,32,99,108,97,110,103,45,102,111,114,109,97,116,32,111,110,32,42,47,10,10,117,110,105,102,111,114,109,32,105,110,116,32,102,97,99,101,95,105,100,59,10,10,35,105,102,110,100,101,102,32,77,79,68,69,95,68,73,82,69,67,84,95,87,82,73,84,69,10,117,110,105,102,111,114,109,32,117,105,110,116,32,115,97,109,112,108,101,95,99,111,117,110,116,59,10,117,110,105,102,111,114,109,32,118,101,99,52,32,115,97,109,112,108,101,95,100,105,114,101,99,116,105,111,110,115,95,109,105,112,91,77,65,88,95,83,65,77,80,76,69,95,67,79,85,78,84,93,59,10,117,110,105,102,111,114,109,32,102,108,111,97,116,32,119,101,105,103,104,116,59,10,35,101,110,100,105,102,10,10,105,110,32,104,105,103,104,112,32,118,101,99,50,32,117,118,95,105,110,116,101,114,112,59,10,10,108,97,121,111,117,116,40,108,111,99,97,116,105,111,110,32,61,32,48,41,32,111,117,116,32,118,101,99,52,32,102,114,97,103,95,99,111,108,111,114,59,10,10,35,100,101,102,105,110,101,32,77,95,80,73,32,51,46,49,52,49,53,57,50,54,53,51,53,57,10,10,47,47,32,68,111,110,39,116,32,105,110,99,108,117,100,101,32,116,111,110,101,109,97,112,95,105,110,99,46,103,108,115,108,32,98,101,99,97,117,115,101,32,97,108,108,32,119,101,32,119,97,110,116,32,105,115,32,116,104,101,115,101,32,102,117,110,99,116,105,111,110,115,44,32,119,101,32,100,111,110,39,116,32,119,97,110,116,32,116,104,101,32,117,110,105,102,111,114,109,115,10,118,101,99,51,32,108,105,110,101,97,114,95,116,111,95,115,114,103,98,40,118,101,99,51,32,99,111,108,111,114,41,32,123,10,9,114,101,116,117,114,110,32,109,97,120,40,118,101,99,51,40,49,46,48,53,53,41,32,42,32,112,111,119,40,99,111,108,111,114,44,32,118,101,99,51,40,48,46,52,49,54,54,54,54,54,54,55,41,41,32,45,32,118,101,99,51,40,48,46,48,53,53,41,44,32,118,101,99,51,40,48,46,48,41,41,59,10,125,10,10,118,101,99,51,32,115,114,103,98,95,116,111,95,108,105,110,101,97,114,40,118,101,99,51,32,99,111,108,111,114,41,32,123,10,9,114,101,116,117,114,110,32,99,111,108,111,114,32,42,32,40,99,111,108,111,114,32,42,32,40,99,111,108,111,114,32,42,32,48,46,51,48,53,51,48,54,48,49,49,32,43,32,48,46,54,56,50,49,55,49,49,49,49,41,32,43,32,48,46,48,49,50,53,50,50,56,55,56,41,59,10,125,10,10,118,101,99,51,32,116,101,120,101,108,67,111,111,114,100,84,111,86,101,99,40,118,101,99,50,32,117,118,44,32,105,110,116,32,102,97,99,101,73,68,41,32,123,10,9,109,97,116,51,32,102,97,99,101,85,118,86,101,99,116,111,114,115,91,54,93,59,10,10,9,47,47,32,45,120,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,49,93,91,48,93,32,61,32,118,101,99,51,40,48,46,48,44,32,48,46,48,44,32,49,46,48,41,59,32,47,47,32,117,32,45,62,32,43,122,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,49,93,91,49,93,32,61,32,118,101,99,51,40,48,46,48,44,32,45,49,46,48,44,32,48,46,48,41,59,32,47,47,32,118,32,45,62,32,45,121,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,49,93,91,50,93,32,61,32,118,101,99,51,40,45,49,46,48,44,32,48,46,48,44,32,48,46,48,41,59,32,47,47,32,45,120,32,102,97,99,101,10,10,9,47,47,32,43,120,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,48,93,91,48,93,32,61,32,118,101,99,51,40,48,46,48,44,32,48,46,48,44,32,45,49,46,48,41,59,32,47,47,32,117,32,45,62,32,45,122,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,48,93,91,49,93,32,61,32,118,101,99,51,40,48,46,48,44,32,45,49,46,48,44,32,48,46,48,41,59,32,47,47,32,118,32,45,62,32,45,121,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,48,93,91,50,93,32,61,32,118,101,99,51,40,49,46,48,44,32,48,46,48,44,32,48,46,48,41,59,32,47,47,32,43,120,32,102,97,99,101,10,10,9,47,47,32,45,121,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,51,93,91,48,93,32,61,32,118,101,99,51,40,49,46,48,44,32,48,46,48,44,32,48,46,48,41,59,32,47,47,32,117,32,45,62,32,43,120,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,51,93,91,49,93,32,61,32,118,101,99,51,40,48,46,48,44,32,48,46,48,44,32,45,49,46,48,41,59,32,47,47,32,118,32,45,62,32,45,122,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,51,93,91,50,93,32,61,32,118,101,99,51,40,48,46,48,44,32,45,49,46,48,44,32,48,46,48,41,59,32,47,47,32,45,121,32,102,97,99,101,10,10,9,47,47,32,43,121,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,50,93,91,48,93,32,61,32,118,101,99,51,40,49,46,48,44,32,48,46,48,44,32,48,46,48,41,59,32,47,47,32,117,32,45,62,32,43,120,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,50,93,91,49,93,32,61,32,118,101,99,51,40,48,46,48,44,32,48,46,48,44,32,49,46,48,41,59,32,47,47,32,118,32,45,62,32,43,122,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,50,93,91,50,93,32,61,32,118,101,99,51,40,48,46,48,44,32,49,46,48,44,32,48,46,48,41,59,32,47,47,32,43,121,32,102,97,99,101,10,10,9,47,47,32,45,122,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,53,93,91,48,93,32,61,32,118,101,99,51,40,45,49,46,48,44,32,48,46,48,44,32,48,46,48,41,59,32,47,47,32,117,32,45,62,32,45,120,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,53,93,91,49,93,32,61,32,118,101,99,51,40,48,46,48,44,32,45,49,46,48,44,32,48,46,48,41,59,32,47,47,32,118,32,45,62,32,45,121,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,53,93,91,50,93,32,61,32,118,101,99,51,40,48,46,48,44,32,48,46,48,44,32,45,49,46,48,41,59,32,47,47,32,45,122,32,102,97,99,101,10,10,9,47,47,32,43,122,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,52,93,91,48,93,32,61,32,118,101,99,51,40,49,46,48,44,32,48,46,48,44,32,48,46,48,41,59,32,47,47,32,117,32,45,62,32,43,120,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,52,93,91,49,93,32,61,32,118,101,99,51,40,48,46,48,44,32,45,49,46,48,44,32,48,46,48,41,59,32,47,47,32,118,32,45,62,32,45,121,10,9,102,97,99,101,85,118,86,101,99,116,111,114,115,91,52,93,91,50,93,32,61,32,118,101,99,51,40,48,46,48,44,32,48,46,48,44,32,49,46,48,41,59,32,47,47,32,43,122,32,102,97,99,101,10,10,9,47,47,32,111,117,116,32,61,32,117,32,42,32,115,95,102,97,99,101,85,118,91,48,93,32,43,32,118,32,42,32,115,95,102,97,99,101,85,118,91,49,93,32,43,32,115,95,102,97,99,101,85,118,91,50,93,46,10,9,118,101,99,51,32,114,101,115,117,108,116,32,61,32,40,102,97,99,101,85,118,86,101,99,116,111,114,115,91,102,97,99,101,73,68,93,91,48,93,32,42,32,117,118,46,120,41,32,43,32,40,102,97,99,101,85,118,86,101,99,116,111,114,115,91,102,97,99,101,73,68,93,91,49,93,32,42,32,117,118,46,121,41,32,43,32,102,97,99,101,85,118,86,101,99,116,111,114,115,91,102,97,99,101,73,68,93,91,50,93,59,10,9,114,101,116,117,114,110,32,110,111,114,109,97,108,105,122,101,40,114,101,115,117,108,116,41,59,10,125,10,10,118,111,105,100,32,109,97,105,110,40,41,32,123,10,9,118,101,99,51,32,99,111,108,111,114,32,61,32,118,101,99,51,40,48,46,48,41,59,10,9,118,101,99,50,32,117,118,32,61,32,117,118,95,105,110,116,101,114,112,59,10,9,118,101,99,51,32,78,32,61,32,116,101,120,101,108,67,111,111,114,100,84,111,86,101,99,40,117,118,44,32,102,97,99,101,95,105,100,41,59,10,10,35,105,102,100,101,102,32,77,79,68,69,95,68,73,82,69,67,84,95,87,82,73,84,69,10,9,102,114,97,103,95,99,111,108,111,114,32,61,32,118,101,99,52,40,116,101,120,116,117,114,101,76,111,100,40,115,111,117,114,99,101,95,99,117,98,101,44,32,78,44,32,48,46,48,41,46,114,103,98,44,32,49,46,48,41,59,10,35,101,108,115,101,10,10,9,118,101,99,52,32,115,117,109,32,61,32,118,101,99,52,40,48,46,48,41,59,10,9,118,101,99,51,32,85,112,86,101,99,116,111,114,32,61,32,97,98,115,40,78,46,122,41,32,60,32,48,46,57,57,57,32,63,32,118,101,99,51,40,48,46,48,44,32,48,46,48,44,32,49,46,48,41,32,58,32,118,101,99,51,40,49,46,48,44,32,48,46,48,44,32,48,46,48,41,59,10,9,109,97,116,51,32,84,59,10,9,84,91,48,93,32,61,32,110,111,114,109,97,108,105,122,101,40,99,114,111,115,115,40,85,112,86,101,99,116,111,114,44,32,78,41,41,59,10,9,84,91,49,93,32,61,32,99,114,111,115,115,40,78,44,32,84,91,48,93,41,59,10,9,84,91,50,93,32,61,32,78,59,10,10,9,102,111,114,32,40,117,105,110,116,32,115,97,109,112,108,101,95,110,117,109,32,61,32,48,117,59,32,115,97,109,112,108,101,95,110,117,109,32,60,32,115,97,109,112,108,101,95,99,111,117,110,116,59,32,115,97,109,112,108,101,95,110,117,109,43,43,41,32,123,10,9,9,118,101,99,52,32,115,97,109,112,108,101,95,100,105,114,101,99,116,105,111,110,95,109,105,112,32,61,32,115,97,109,112,108,101,95,100,105,114,101,99,116,105,111,110,115,95,109,105,112,91,115,97,109,112,108,101,95,110,117,109,93,59,10,9,9,118,101,99,51,32,76,32,61,32,84,32,42,32,115,97,109,112,108,101,95,100,105,114,101,99,116,105,111,110,95,109,105,112,46,120,121,122,59,10,9,9,118,101,99,51,32,118,97,108,32,61,32,116,101,120,116,117,114,101,76,111,100,40,115,111,117,114,99,101,95,99,117,98,101,44,32,76,44,32,115,97,109,112,108,101,95,100,105,114,101,99,116,105,111,110,95,109,105,112,46,119,41,46,114,103,98,59,10,9,9,47,47,32,77,105,120,32,117,115,105,110,103,32,108,105,110,101,97,114,10,9,9,118,97,108,32,61,32,115,114,103,98,95,116,111,95,108,105,110,101,97,114,40,118,97,108,41,59,10,9,9,115,117,109,46,114,103,98,32,43,61,32,118,97,108,32,42,32,115,97,109,112,108,101,95,100,105,114,101,99,116,105,111,110,95,109,105,112,46,122,59,10,9,125,10,10,9,115,117,109,32,47,61,32,119,101,105,103,104,116,59,10,10,9,115,117,109,46,114,103,98,32,61,32,108,105,110,101,97,114,95,116,111,95,115,114,103,98,40,115,117,109,46,114,103,98,41,59,10,9,102,114,97,103,95,99,111,108,111,114,32,61,32,118,101,99,52,40,115,117,109,46,114,103,98,44,32,49,46,48,41,59,10,35,101,110,100,105,102,10,125,10,		0};

		_setup(_vertex_code,_fragment_code,"CubemapFilterShaderGLES3",4,_uniform_strings,0,_ubo_pairs,0,_feedbacks,1,_texunit_pairs,0,_spec_pairs,2,_variant_defines);
	}

};

#endif

