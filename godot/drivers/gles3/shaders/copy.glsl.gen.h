/* WARNING, THIS FILE WAS GENERATED, DO NOT EDIT */
#ifndef COPY_GLSL_GEN_HGLES3_GLES3
#define COPY_GLSL_GEN_HGLES3_GLES3


#include "drivers/gles3/shader_gles3.h"


class CopyShaderGLES3 : public ShaderGLES3 {

public:

	enum Uniforms {
		COPY_SECTION,
		SOURCE_SECTION,
		COLOR_IN,
		PIXEL_SIZE,
	};

	enum ShaderVariant {
		MODE_DEFAULT,
		MODE_COPY_SECTION,
		MODE_GAUSSIAN_BLUR,
		MODE_MIPMAP,
		MODE_SIMPLE_COLOR,
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
			"copy_section",
			"source_section",
			"color_in",
			"pixel_size",
		};

		static const char* _variant_defines[]={
			"#define MODE_SIMPLE_COPY",
			"#define USE_COPY_SECTION \n#define MODE_SIMPLE_COPY",
			"#define MODE_GAUSSIAN_BLUR",
			"#define MODE_MIPMAP",
			"#define MODE_SIMPLE_COLOR \n#define USE_COPY_SECTION",
		};

		static TexUnitPair _texunit_pairs[]={
			{"source",0},
		};

		static UBOPair *_ubo_pairs=nullptr;
		static Specialization *_spec_pairs=nullptr;
		static const Feedback* _feedbacks=nullptr;
		static const char _vertex_code[]={
10,108,97,121,111,117,116,40,108,111,99,97,116,105,111,110,32,61,32,48,41,32,105,110,32,118,101,99,50,32,118,101,114,116,101,120,95,97,116,116,114,105,98,59,10,10,111,117,116,32,118,101,99,50,32,117,118,95,105,110,116,101,114,112,59,10,47,42,32,99,108,97,110,103,45,102,111,114,109,97,116,32,111,110,32,42,47,10,10,35,105,102,32,100,101,102,105,110,101,100,40,85,83,69,95,67,79,80,89,95,83,69,67,84,73,79,78,41,32,124,124,32,100,101,102,105,110,101,100,40,77,79,68,69,95,71,65,85,83,83,73,65,78,95,66,76,85,82,41,10,47,47,32,68,101,102,105,110,101,100,32,105,110,32,48,45,49,32,99,111,111,114,100,115,46,10,117,110,105,102,111,114,109,32,104,105,103,104,112,32,118,101,99,52,32,99,111,112,121,95,115,101,99,116,105,111,110,59,10,35,101,110,100,105,102,10,35,105,102,100,101,102,32,77,79,68,69,95,71,65,85,83,83,73,65,78,95,66,76,85,82,10,117,110,105,102,111,114,109,32,104,105,103,104,112,32,118,101,99,52,32,115,111,117,114,99,101,95,115,101,99,116,105,111,110,59,10,35,101,110,100,105,102,10,10,118,111,105,100,32,109,97,105,110,40,41,32,123,10,9,117,118,95,105,110,116,101,114,112,32,61,32,118,101,114,116,101,120,95,97,116,116,114,105,98,32,42,32,48,46,53,32,43,32,48,46,53,59,10,9,103,108,95,80,111,115,105,116,105,111,110,32,61,32,118,101,99,52,40,118,101,114,116,101,120,95,97,116,116,114,105,98,44,32,49,46,48,44,32,49,46,48,41,59,10,10,35,105,102,32,100,101,102,105,110,101,100,40,85,83,69,95,67,79,80,89,95,83,69,67,84,73,79,78,41,32,124,124,32,100,101,102,105,110,101,100,40,77,79,68,69,95,71,65,85,83,83,73,65,78,95,66,76,85,82,41,10,9,103,108,95,80,111,115,105,116,105,111,110,46,120,121,32,61,32,40,99,111,112,121,95,115,101,99,116,105,111,110,46,120,121,32,43,32,117,118,95,105,110,116,101,114,112,46,120,121,32,42,32,99,111,112,121,95,115,101,99,116,105,111,110,46,122,119,41,32,42,32,50,46,48,32,45,32,49,46,48,59,10,35,101,110,100,105,102,10,35,105,102,100,101,102,32,77,79,68,69,95,71,65,85,83,83,73,65,78,95,66,76,85,82,10,9,117,118,95,105,110,116,101,114,112,32,61,32,115,111,117,114,99,101,95,115,101,99,116,105,111,110,46,120,121,32,43,32,117,118,95,105,110,116,101,114,112,32,42,32,115,111,117,114,99,101,95,115,101,99,116,105,111,110,46,122,119,59,10,35,101,110,100,105,102,10,125,10,10,47,42,32,99,108,97,110,103,45,102,111,114,109,97,116,32,111,102,102,32,42,47,10,		0};

		static const char _fragment_code[]={
10,105,110,32,118,101,99,50,32,117,118,95,105,110,116,101,114,112,59,10,47,42,32,99,108,97,110,103,45,102,111,114,109,97,116,32,111,110,32,42,47,10,35,105,102,100,101,102,32,77,79,68,69,95,83,73,77,80,76,69,95,67,79,76,79,82,10,117,110,105,102,111,114,109,32,118,101,99,52,32,99,111,108,111,114,95,105,110,59,10,35,101,110,100,105,102,10,10,35,105,102,100,101,102,32,77,79,68,69,95,71,65,85,83,83,73,65,78,95,66,76,85,82,10,47,47,32,68,101,102,105,110,101,100,32,105,110,32,48,45,49,32,99,111,111,114,100,115,46,10,117,110,105,102,111,114,109,32,104,105,103,104,112,32,118,101,99,50,32,112,105,120,101,108,95,115,105,122,101,59,10,35,101,110,100,105,102,10,10,117,110,105,102,111,114,109,32,115,97,109,112,108,101,114,50,68,32,115,111,117,114,99,101,59,32,47,47,32,116,101,120,117,110,105,116,58,48,10,10,108,97,121,111,117,116,40,108,111,99,97,116,105,111,110,32,61,32,48,41,32,111,117,116,32,118,101,99,52,32,102,114,97,103,95,99,111,108,111,114,59,10,10,118,111,105,100,32,109,97,105,110,40,41,32,123,10,35,105,102,100,101,102,32,77,79,68,69,95,83,73,77,80,76,69,95,67,79,80,89,10,9,118,101,99,52,32,99,111,108,111,114,32,61,32,116,101,120,116,117,114,101,40,115,111,117,114,99,101,44,32,117,118,95,105,110,116,101,114,112,41,59,10,9,102,114,97,103,95,99,111,108,111,114,32,61,32,99,111,108,111,114,59,10,35,101,110,100,105,102,10,10,35,105,102,100,101,102,32,77,79,68,69,95,83,73,77,80,76,69,95,67,79,76,79,82,10,9,102,114,97,103,95,99,111,108,111,114,32,61,32,99,111,108,111,114,95,105,110,59,10,35,101,110,100,105,102,10,10,47,47,32,69,102,102,105,99,105,101,110,116,32,98,111,120,32,102,105,108,116,101,114,32,102,114,111,109,32,74,105,109,101,110,101,122,58,32,104,116,116,112,58,47,47,119,119,119,46,105,114,121,111,107,117,46,99,111,109,47,110,101,120,116,45,103,101,110,101,114,97,116,105,111,110,45,112,111,115,116,45,112,114,111,99,101,115,115,105,110,103,45,105,110,45,99,97,108,108,45,111,102,45,100,117,116,121,45,97,100,118,97,110,99,101,100,45,119,97,114,102,97,114,101,10,47,47,32,65,112,112,114,111,120,105,109,97,116,101,115,32,97,32,71,97,117,115,115,105,97,110,32,105,110,32,97,32,115,105,110,103,108,101,32,112,97,115,115,46,10,35,105,102,100,101,102,32,77,79,68,69,95,71,65,85,83,83,73,65,78,95,66,76,85,82,10,9,118,101,99,52,32,65,32,61,32,116,101,120,116,117,114,101,76,111,100,40,115,111,117,114,99,101,44,32,117,118,95,105,110,116,101,114,112,32,43,32,112,105,120,101,108,95,115,105,122,101,32,42,32,118,101,99,50,40,45,49,46,48,44,32,45,49,46,48,41,44,32,48,46,48,41,59,10,9,118,101,99,52,32,66,32,61,32,116,101,120,116,117,114,101,76,111,100,40,115,111,117,114,99,101,44,32,117,118,95,105,110,116,101,114,112,32,43,32,112,105,120,101,108,95,115,105,122,101,32,42,32,118,101,99,50,40,48,46,48,44,32,45,49,46,48,41,44,32,48,46,48,41,59,10,9,118,101,99,52,32,67,32,61,32,116,101,120,116,117,114,101,76,111,100,40,115,111,117,114,99,101,44,32,117,118,95,105,110,116,101,114,112,32,43,32,112,105,120,101,108,95,115,105,122,101,32,42,32,118,101,99,50,40,49,46,48,44,32,45,49,46,48,41,44,32,48,46,48,41,59,10,9,118,101,99,52,32,68,32,61,32,116,101,120,116,117,114,101,76,111,100,40,115,111,117,114,99,101,44,32,117,118,95,105,110,116,101,114,112,32,43,32,112,105,120,101,108,95,115,105,122,101,32,42,32,118,101,99,50,40,45,48,46,53,44,32,45,48,46,53,41,44,32,48,46,48,41,59,10,9,118,101,99,52,32,69,32,61,32,116,101,120,116,117,114,101,76,111,100,40,115,111,117,114,99,101,44,32,117,118,95,105,110,116,101,114,112,32,43,32,112,105,120,101,108,95,115,105,122,101,32,42,32,118,101,99,50,40,48,46,53,44,32,45,48,46,53,41,44,32,48,46,48,41,59,10,9,118,101,99,52,32,70,32,61,32,116,101,120,116,117,114,101,76,111,100,40,115,111,117,114,99,101,44,32,117,118,95,105,110,116,101,114,112,32,43,32,112,105,120,101,108,95,115,105,122,101,32,42,32,118,101,99,50,40,45,49,46,48,44,32,48,46,48,41,44,32,48,46,48,41,59,10,9,118,101,99,52,32,71,32,61,32,116,101,120,116,117,114,101,76,111,100,40,115,111,117,114,99,101,44,32,117,118,95,105,110,116,101,114,112,44,32,48,46,48,41,59,10,9,118,101,99,52,32,72,32,61,32,116,101,120,116,117,114,101,76,111,100,40,115,111,117,114,99,101,44,32,117,118,95,105,110,116,101,114,112,32,43,32,112,105,120,101,108,95,115,105,122,101,32,42,32,118,101,99,50,40,49,46,48,44,32,48,46,48,41,44,32,48,46,48,41,59,10,9,118,101,99,52,32,73,32,61,32,116,101,120,116,117,114,101,76,111,100,40,115,111,117,114,99,101,44,32,117,118,95,105,110,116,101,114,112,32,43,32,112,105,120,101,108,95,115,105,122,101,32,42,32,118,101,99,50,40,45,48,46,53,44,32,48,46,53,41,44,32,48,46,48,41,59,10,9,118,101,99,52,32,74,32,61,32,116,101,120,116,117,114,101,76,111,100,40,115,111,117,114,99,101,44,32,117,118,95,105,110,116,101,114,112,32,43,32,112,105,120,101,108,95,115,105,122,101,32,42,32,118,101,99,50,40,48,46,53,44,32,48,46,53,41,44,32,48,46,48,41,59,10,9,118,101,99,52,32,75,32,61,32,116,101,120,116,117,114,101,76,111,100,40,115,111,117,114,99,101,44,32,117,118,95,105,110,116,101,114,112,32,43,32,112,105,120,101,108,95,115,105,122,101,32,42,32,118,101,99,50,40,45,49,46,48,44,32,49,46,48,41,44,32,48,46,48,41,59,10,9,118,101,99,52,32,76,32,61,32,116,101,120,116,117,114,101,76,111,100,40,115,111,117,114,99,101,44,32,117,118,95,105,110,116,101,114,112,32,43,32,112,105,120,101,108,95,115,105,122,101,32,42,32,118,101,99,50,40,48,46,48,44,32,49,46,48,41,44,32,48,46,48,41,59,10,9,118,101,99,52,32,77,32,61,32,116,101,120,116,117,114,101,76,111,100,40,115,111,117,114,99,101,44,32,117,118,95,105,110,116,101,114,112,32,43,32,112,105,120,101,108,95,115,105,122,101,32,42,32,118,101,99,50,40,49,46,48,44,32,49,46,48,41,44,32,48,46,48,41,59,10,10,9,102,108,111,97,116,32,119,101,105,103,104,116,32,61,32,48,46,53,32,47,32,52,46,48,59,10,9,102,108,111,97,116,32,108,101,115,115,101,114,95,119,101,105,103,104,116,32,61,32,48,46,49,50,53,32,47,32,52,46,48,59,10,10,9,102,114,97,103,95,99,111,108,111,114,32,61,32,40,68,32,43,32,69,32,43,32,73,32,43,32,74,41,32,42,32,119,101,105,103,104,116,59,10,9,102,114,97,103,95,99,111,108,111,114,32,43,61,32,40,65,32,43,32,66,32,43,32,71,32,43,32,70,41,32,42,32,108,101,115,115,101,114,95,119,101,105,103,104,116,59,10,9,102,114,97,103,95,99,111,108,111,114,32,43,61,32,40,66,32,43,32,67,32,43,32,72,32,43,32,71,41,32,42,32,108,101,115,115,101,114,95,119,101,105,103,104,116,59,10,9,102,114,97,103,95,99,111,108,111,114,32,43,61,32,40,70,32,43,32,71,32,43,32,76,32,43,32,75,41,32,42,32,108,101,115,115,101,114,95,119,101,105,103,104,116,59,10,9,102,114,97,103,95,99,111,108,111,114,32,43,61,32,40,71,32,43,32,72,32,43,32,77,32,43,32,76,41,32,42,32,108,101,115,115,101,114,95,119,101,105,103,104,116,59,10,35,101,110,100,105,102,10,125,10,		0};

		_setup(_vertex_code,_fragment_code,"CopyShaderGLES3",4,_uniform_strings,0,_ubo_pairs,0,_feedbacks,1,_texunit_pairs,0,_spec_pairs,5,_variant_defines);
	}

};

#endif

