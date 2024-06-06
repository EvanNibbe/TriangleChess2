#define DEBUG_LIGHT
#define DEBUG_FIND_SPOT
/**************************************************************************/
/*  light_3d.cpp                                                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "core/config/project_settings.h"

#include "light_3d.h"

void Light3D::set_param(Param p_param, real_t p_value) {
	ERR_FAIL_INDEX(p_param, PARAM_MAX);
	param[p_param] = p_value;

	RS::get_singleton()->light_set_param(light, RS::LightParam(p_param), p_value);

	if (p_param == PARAM_SPOT_ANGLE || p_param == PARAM_RANGE) {
		update_gizmos();

		if (p_param == PARAM_SPOT_ANGLE) {
			update_configuration_warnings();
		}
	}
}

real_t Light3D::get_param(Param p_param) const {
	ERR_FAIL_INDEX_V(p_param, PARAM_MAX, 0);
	return param[p_param];
}

void Light3D::set_shadow(bool p_enable) {
	shadow = p_enable;
	RS::get_singleton()->light_set_shadow(light, p_enable);

	notify_property_list_changed();
	update_configuration_warnings();
}

bool Light3D::has_shadow() const {
	return shadow;
}

void Light3D::set_negative(bool p_enable) {
	negative = p_enable;
	RS::get_singleton()->light_set_negative(light, p_enable);
}

bool Light3D::is_negative() const {
	return negative;
}

void Light3D::set_enable_distance_fade(bool p_enable) {
	distance_fade_enabled = p_enable;
	RS::get_singleton()->light_set_distance_fade(light, distance_fade_enabled, distance_fade_begin, distance_fade_shadow, distance_fade_length);
	notify_property_list_changed();
}

bool Light3D::is_distance_fade_enabled() const {
	return distance_fade_enabled;
}

void Light3D::set_distance_fade_begin(real_t p_distance) {
	distance_fade_begin = p_distance;
	RS::get_singleton()->light_set_distance_fade(light, distance_fade_enabled, distance_fade_begin, distance_fade_shadow, distance_fade_length);
}

real_t Light3D::get_distance_fade_begin() const {
	return distance_fade_begin;
}

void Light3D::set_distance_fade_shadow(real_t p_distance) {
	distance_fade_shadow = p_distance;
	RS::get_singleton()->light_set_distance_fade(light, distance_fade_enabled, distance_fade_begin, distance_fade_shadow, distance_fade_length);
}

real_t Light3D::get_distance_fade_shadow() const {
	return distance_fade_shadow;
}

void Light3D::set_distance_fade_length(real_t p_length) {
	distance_fade_length = p_length;
	RS::get_singleton()->light_set_distance_fade(light, distance_fade_enabled, distance_fade_begin, distance_fade_shadow, distance_fade_length);
}

real_t Light3D::get_distance_fade_length() const {
	return distance_fade_length;
}

void Light3D::set_cull_mask(uint32_t p_cull_mask) {
	cull_mask = p_cull_mask;
	RS::get_singleton()->light_set_cull_mask(light, p_cull_mask);
}

uint32_t Light3D::get_cull_mask() const {
	return cull_mask;
}

void Light3D::set_color(const Color &p_color) {
	color = p_color;

	if (GLOBAL_GET("rendering/lights_and_shadows/use_physical_light_units")) {
		Color combined = color.srgb_to_linear();
		combined *= correlated_color.srgb_to_linear();
		RS::get_singleton()->light_set_color(light, combined.linear_to_srgb());
	} else {
		RS::get_singleton()->light_set_color(light, color);
	}
	// The gizmo color depends on the light color, so update it.
	update_gizmos();
}

Color Light3D::get_color() const {
	return color;
}

void Light3D::set_shadow_reverse_cull_face(bool p_enable) {
	reverse_cull = p_enable;
	RS::get_singleton()->light_set_reverse_cull_face_mode(light, reverse_cull);
}

bool Light3D::get_shadow_reverse_cull_face() const {
	return reverse_cull;
}

AABB Light3D::get_aabb() const {
	if (type == RenderingServer::LIGHT_DIRECTIONAL) {
		return AABB(Vector3(-1, -1, -1), Vector3(2, 2, 2));

	} else if (type == RenderingServer::LIGHT_OMNI) {
		return AABB(Vector3(-1, -1, -1) * param[PARAM_RANGE], Vector3(2, 2, 2) * param[PARAM_RANGE]);

	} else if (type == RenderingServer::LIGHT_SPOT) {
		real_t cone_slant_height = param[PARAM_RANGE];
		real_t cone_angle_rad = Math::deg_to_rad(param[PARAM_SPOT_ANGLE]);

		if (cone_angle_rad > Math_PI / 2.0) {
			// Just return the AABB of an omni light if the spot angle is above 90 degrees.
			return AABB(Vector3(-1, -1, -1) * cone_slant_height, Vector3(2, 2, 2) * cone_slant_height);
		}

		real_t size = Math::sin(cone_angle_rad) * cone_slant_height;
		return AABB(Vector3(-size, -size, -cone_slant_height), Vector3(2 * size, 2 * size, cone_slant_height));
	}

	return AABB();
}

PackedStringArray Light3D::get_configuration_warnings() const {
	PackedStringArray warnings = VisualInstance3D::get_configuration_warnings();

	if (has_shadow() && OS::get_singleton()->get_current_rendering_method() == "gl_compatibility") {
		warnings.push_back(RTR("Shadows are not supported when using the GL Compatibility backend yet. Support will be added in a future release."));
	}

	if (!get_scale().is_equal_approx(Vector3(1, 1, 1))) {
		warnings.push_back(RTR("A light's scale does not affect the visual size of the light."));
	}

	return warnings;
}

void Light3D::set_bake_mode(BakeMode p_mode) {
	bake_mode = p_mode;
	RS::get_singleton()->light_set_bake_mode(light, RS::LightBakeMode(p_mode));
}

Light3D::BakeMode Light3D::get_bake_mode() const {
	return bake_mode;
}

void Light3D::set_projector(const Ref<Texture2D> &p_texture) {
	projector = p_texture;
	RID tex_id = projector.is_valid() ? projector->get_rid() : RID();
	RS::get_singleton()->light_set_projector(light, tex_id);
	update_configuration_warnings();
}

Ref<Texture2D> Light3D::get_projector() const {
	return projector;
}

void Light3D::owner_changed_notify() {
	// For cases where owner changes _after_ entering tree (as example, editor editing).
	_update_visibility();
}

// Temperature expressed in Kelvins. Valid range 1000 - 15000
// First converts to CIE 1960 then to sRGB
// As explained in the Filament documentation: https://google.github.io/filament/Filament.md.html#lighting/directlighting/lightsparameterization
Color _color_from_temperature(float p_temperature) {
	float T2 = p_temperature * p_temperature;
	float u = (0.860117757f + 1.54118254e-4f * p_temperature + 1.28641212e-7f * T2) /
			(1.0f + 8.42420235e-4f * p_temperature + 7.08145163e-7f * T2);
	float v = (0.317398726f + 4.22806245e-5f * p_temperature + 4.20481691e-8f * T2) /
			(1.0f - 2.89741816e-5f * p_temperature + 1.61456053e-7f * T2);

	// Convert to xyY space.
	float d = 1.0f / (2.0f * u - 8.0f * v + 4.0f);
	float x = 3.0f * u * d;
	float y = 2.0f * v * d;

	// Convert to XYZ space
	const float a = 1.0 / MAX(y, 1e-5f);
	Vector3 xyz = Vector3(x * a, 1.0, (1.0f - x - y) * a);

	// Convert from XYZ to sRGB(linear)
	Vector3 linear = Vector3(3.2404542f * xyz.x - 1.5371385f * xyz.y - 0.4985314f * xyz.z,
			-0.9692660f * xyz.x + 1.8760108f * xyz.y + 0.0415560f * xyz.z,
			0.0556434f * xyz.x - 0.2040259f * xyz.y + 1.0572252f * xyz.z);
	linear /= MAX(1e-5f, linear[linear.max_axis_index()]);
	// Normalize, clamp, and convert to sRGB.
	return Color(linear.x, linear.y, linear.z).clamp().linear_to_srgb();
}

void Light3D::set_temperature(const float p_temperature) {
	temperature = p_temperature;
	if (!GLOBAL_GET("rendering/lights_and_shadows/use_physical_light_units")) {
		return;
	}
	correlated_color = _color_from_temperature(temperature);

	Color combined = color.srgb_to_linear() * correlated_color.srgb_to_linear();

	RS::get_singleton()->light_set_color(light, combined.linear_to_srgb());
	// The gizmo color depends on the light color, so update it.
	update_gizmos();
}

Color Light3D::get_correlated_color() const {
	return correlated_color;
}

float Light3D::get_temperature() const {
	return temperature;
}

void Light3D::_update_visibility() {
	if (!is_inside_tree()) {
		return;
	}

	bool editor_ok = true;

#ifdef TOOLS_ENABLED
	if (editor_only) {
		if (!Engine::get_singleton()->is_editor_hint()) {
			editor_ok = false;
		} else {
			editor_ok = (get_tree()->get_edited_scene_root() && (this == get_tree()->get_edited_scene_root() || get_owner() == get_tree()->get_edited_scene_root()));
		}
	}
#else
	if (editor_only) {
		editor_ok = false;
	}
#endif

	RS::get_singleton()->instance_set_visible(get_instance(), is_visible_in_tree() && editor_ok);
}

void Light3D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_TRANSFORM_CHANGED: {
			update_configuration_warnings();
		} break;
		case NOTIFICATION_VISIBILITY_CHANGED:
		case NOTIFICATION_ENTER_TREE: {
			_update_visibility();
		} break;
	}
}

void Light3D::set_editor_only(bool p_editor_only) {
	editor_only = p_editor_only;
	_update_visibility();
}

bool Light3D::is_editor_only() const {
	return editor_only;
}

void Light3D::_validate_property(PropertyInfo &p_property) const {
	if (!shadow && (p_property.name == "shadow_bias" || p_property.name == "shadow_normal_bias" || p_property.name == "shadow_reverse_cull_face" || p_property.name == "shadow_transmittance_bias" || p_property.name == "shadow_opacity" || p_property.name == "shadow_blur" || p_property.name == "distance_fade_shadow")) {
		p_property.usage = PROPERTY_USAGE_NO_EDITOR;
	}

	if (get_light_type() != RS::LIGHT_DIRECTIONAL && (p_property.name == "light_angular_distance" || p_property.name == "light_intensity_lux")) {
		// Angular distance and Light Intensity Lux are only used in DirectionalLight3D.
		p_property.usage = PROPERTY_USAGE_NONE;
	} else if (get_light_type() == RS::LIGHT_DIRECTIONAL && p_property.name == "light_intensity_lumens") {
		p_property.usage = PROPERTY_USAGE_NONE;
	}

	if (!GLOBAL_GET("rendering/lights_and_shadows/use_physical_light_units") && (p_property.name == "light_intensity_lumens" || p_property.name == "light_intensity_lux" || p_property.name == "light_temperature")) {
		p_property.usage = PROPERTY_USAGE_NONE;
	}

	if (!distance_fade_enabled && (p_property.name == "distance_fade_begin" || p_property.name == "distance_fade_shadow" || p_property.name == "distance_fade_length")) {
		p_property.usage = PROPERTY_USAGE_NO_EDITOR;
	}
}

void Light3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_editor_only", "editor_only"), &Light3D::set_editor_only);
	ClassDB::bind_method(D_METHOD("is_editor_only"), &Light3D::is_editor_only);

	ClassDB::bind_method(D_METHOD("set_param", "param", "value"), &Light3D::set_param);
	ClassDB::bind_method(D_METHOD("get_param", "param"), &Light3D::get_param);

	ClassDB::bind_method(D_METHOD("set_shadow", "enabled"), &Light3D::set_shadow);
	ClassDB::bind_method(D_METHOD("has_shadow"), &Light3D::has_shadow);

	ClassDB::bind_method(D_METHOD("set_negative", "enabled"), &Light3D::set_negative);
	ClassDB::bind_method(D_METHOD("is_negative"), &Light3D::is_negative);

	ClassDB::bind_method(D_METHOD("set_cull_mask", "cull_mask"), &Light3D::set_cull_mask);
	ClassDB::bind_method(D_METHOD("get_cull_mask"), &Light3D::get_cull_mask);

	ClassDB::bind_method(D_METHOD("set_enable_distance_fade", "enable"), &Light3D::set_enable_distance_fade);
	ClassDB::bind_method(D_METHOD("is_distance_fade_enabled"), &Light3D::is_distance_fade_enabled);

	ClassDB::bind_method(D_METHOD("set_distance_fade_begin", "distance"), &Light3D::set_distance_fade_begin);
	ClassDB::bind_method(D_METHOD("get_distance_fade_begin"), &Light3D::get_distance_fade_begin);

	ClassDB::bind_method(D_METHOD("set_distance_fade_shadow", "distance"), &Light3D::set_distance_fade_shadow);
	ClassDB::bind_method(D_METHOD("get_distance_fade_shadow"), &Light3D::get_distance_fade_shadow);

	ClassDB::bind_method(D_METHOD("set_distance_fade_length", "distance"), &Light3D::set_distance_fade_length);
	ClassDB::bind_method(D_METHOD("get_distance_fade_length"), &Light3D::get_distance_fade_length);

	ClassDB::bind_method(D_METHOD("set_color", "color"), &Light3D::set_color);
	ClassDB::bind_method(D_METHOD("get_color"), &Light3D::get_color);

	ClassDB::bind_method(D_METHOD("set_shadow_reverse_cull_face", "enable"), &Light3D::set_shadow_reverse_cull_face);
	ClassDB::bind_method(D_METHOD("get_shadow_reverse_cull_face"), &Light3D::get_shadow_reverse_cull_face);

	ClassDB::bind_method(D_METHOD("set_bake_mode", "bake_mode"), &Light3D::set_bake_mode);
	ClassDB::bind_method(D_METHOD("get_bake_mode"), &Light3D::get_bake_mode);

	ClassDB::bind_method(D_METHOD("set_projector", "projector"), &Light3D::set_projector);
	ClassDB::bind_method(D_METHOD("get_projector"), &Light3D::get_projector);

	ClassDB::bind_method(D_METHOD("set_temperature", "temperature"), &Light3D::set_temperature);
	ClassDB::bind_method(D_METHOD("get_temperature"), &Light3D::get_temperature);
	ClassDB::bind_method(D_METHOD("get_correlated_color"), &Light3D::get_correlated_color);

	ADD_GROUP("Light", "light_");
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "light_intensity_lumens", PROPERTY_HINT_RANGE, "0,100000.0,0.01,or_greater,suffix:lm"), "set_param", "get_param", PARAM_INTENSITY);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "light_intensity_lux", PROPERTY_HINT_RANGE, "0,150000.0,0.01,or_greater,suffix:lx"), "set_param", "get_param", PARAM_INTENSITY);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "light_temperature", PROPERTY_HINT_RANGE, "1000,15000.0,1.0,suffix:k"), "set_temperature", "get_temperature");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "light_color", PROPERTY_HINT_COLOR_NO_ALPHA), "set_color", "get_color");
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "light_energy", PROPERTY_HINT_RANGE, "0,16,0.001,or_greater"), "set_param", "get_param", PARAM_ENERGY);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "light_indirect_energy", PROPERTY_HINT_RANGE, "0,16,0.001,or_greater"), "set_param", "get_param", PARAM_INDIRECT_ENERGY);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "light_volumetric_fog_energy", PROPERTY_HINT_RANGE, "0,16,0.001,or_greater"), "set_param", "get_param", PARAM_VOLUMETRIC_FOG_ENERGY);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "light_projector", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_projector", "get_projector");
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "light_size", PROPERTY_HINT_RANGE, "0,1,0.001,or_greater,suffix:m"), "set_param", "get_param", PARAM_SIZE);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "light_angular_distance", PROPERTY_HINT_RANGE, "0,90,0.01,degrees"), "set_param", "get_param", PARAM_SIZE);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "light_negative"), "set_negative", "is_negative");
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "light_specular", PROPERTY_HINT_RANGE, "0,16,0.001,or_greater"), "set_param", "get_param", PARAM_SPECULAR);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "light_bake_mode", PROPERTY_HINT_ENUM, "Disabled,Static (VoxelGI/SDFGI/LightmapGI),Dynamic (VoxelGI/SDFGI only)"), "set_bake_mode", "get_bake_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "light_cull_mask", PROPERTY_HINT_LAYERS_3D_RENDER), "set_cull_mask", "get_cull_mask");

	ADD_GROUP("Shadow", "shadow_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "shadow_enabled"), "set_shadow", "has_shadow");
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "shadow_bias", PROPERTY_HINT_RANGE, "0,10,0.001"), "set_param", "get_param", PARAM_SHADOW_BIAS);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "shadow_normal_bias", PROPERTY_HINT_RANGE, "0,10,0.001"), "set_param", "get_param", PARAM_SHADOW_NORMAL_BIAS);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "shadow_reverse_cull_face"), "set_shadow_reverse_cull_face", "get_shadow_reverse_cull_face");
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "shadow_transmittance_bias", PROPERTY_HINT_RANGE, "-16,16,0.001"), "set_param", "get_param", PARAM_TRANSMITTANCE_BIAS);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "shadow_opacity", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_param", "get_param", PARAM_SHADOW_OPACITY);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "shadow_blur", PROPERTY_HINT_RANGE, "0,10,0.001"), "set_param", "get_param", PARAM_SHADOW_BLUR);

	ADD_GROUP("Distance Fade", "distance_fade_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "distance_fade_enabled"), "set_enable_distance_fade", "is_distance_fade_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "distance_fade_begin", PROPERTY_HINT_RANGE, "0.0,4096.0,0.01,or_greater,suffix:m"), "set_distance_fade_begin", "get_distance_fade_begin");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "distance_fade_shadow", PROPERTY_HINT_RANGE, "0.0,4096.0,0.01,or_greater,suffix:m"), "set_distance_fade_shadow", "get_distance_fade_shadow");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "distance_fade_length", PROPERTY_HINT_RANGE, "0.0,4096.0,0.01,or_greater,suffix:m"), "set_distance_fade_length", "get_distance_fade_length");

	ADD_GROUP("Editor", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "editor_only"), "set_editor_only", "is_editor_only");

	ADD_GROUP("", "");

	BIND_ENUM_CONSTANT(PARAM_ENERGY);
	BIND_ENUM_CONSTANT(PARAM_INDIRECT_ENERGY);
	BIND_ENUM_CONSTANT(PARAM_VOLUMETRIC_FOG_ENERGY);
	BIND_ENUM_CONSTANT(PARAM_SPECULAR);
	BIND_ENUM_CONSTANT(PARAM_RANGE);
	BIND_ENUM_CONSTANT(PARAM_SIZE);
	BIND_ENUM_CONSTANT(PARAM_ATTENUATION);
	BIND_ENUM_CONSTANT(PARAM_SPOT_ANGLE);
	BIND_ENUM_CONSTANT(PARAM_SPOT_ATTENUATION);
	BIND_ENUM_CONSTANT(PARAM_SHADOW_MAX_DISTANCE);
	BIND_ENUM_CONSTANT(PARAM_SHADOW_SPLIT_1_OFFSET);
	BIND_ENUM_CONSTANT(PARAM_SHADOW_SPLIT_2_OFFSET);
	BIND_ENUM_CONSTANT(PARAM_SHADOW_SPLIT_3_OFFSET);
	BIND_ENUM_CONSTANT(PARAM_SHADOW_FADE_START);
	BIND_ENUM_CONSTANT(PARAM_SHADOW_NORMAL_BIAS);
	BIND_ENUM_CONSTANT(PARAM_SHADOW_BIAS);
	BIND_ENUM_CONSTANT(PARAM_SHADOW_PANCAKE_SIZE);
	BIND_ENUM_CONSTANT(PARAM_SHADOW_OPACITY);
	BIND_ENUM_CONSTANT(PARAM_SHADOW_BLUR);
	BIND_ENUM_CONSTANT(PARAM_TRANSMITTANCE_BIAS);
	BIND_ENUM_CONSTANT(PARAM_INTENSITY);
	BIND_ENUM_CONSTANT(PARAM_MAX);

	BIND_ENUM_CONSTANT(BAKE_DISABLED);
	BIND_ENUM_CONSTANT(BAKE_STATIC);
	BIND_ENUM_CONSTANT(BAKE_DYNAMIC);
}

Light3D::Light3D(RenderingServer::LightType p_type) {
	type = p_type;
	switch (p_type) {
		case RS::LIGHT_DIRECTIONAL:
			light = RenderingServer::get_singleton()->directional_light_create();
			break;
		case RS::LIGHT_OMNI:
			light = RenderingServer::get_singleton()->omni_light_create();
			break;
		case RS::LIGHT_SPOT:
			light = RenderingServer::get_singleton()->spot_light_create();
			break;
		default: {
		};
	}

	RS::get_singleton()->instance_set_base(get_instance(), light);

	set_color(Color(1, 1, 1, 1));
	set_shadow(false);
	set_negative(false);
	set_cull_mask(0xFFFFFFFF);

	set_param(PARAM_ENERGY, 1);
	set_param(PARAM_INDIRECT_ENERGY, 1);
	set_param(PARAM_VOLUMETRIC_FOG_ENERGY, 1);
	set_param(PARAM_SPECULAR, 0.5);
	set_param(PARAM_RANGE, 5);
	set_param(PARAM_SIZE, 0);
	set_param(PARAM_ATTENUATION, 1);
	set_param(PARAM_SPOT_ANGLE, 45);
	set_param(PARAM_SPOT_ATTENUATION, 1);
	set_param(PARAM_SHADOW_MAX_DISTANCE, 0);
	set_param(PARAM_SHADOW_SPLIT_1_OFFSET, 0.1);
	set_param(PARAM_SHADOW_SPLIT_2_OFFSET, 0.2);
	set_param(PARAM_SHADOW_SPLIT_3_OFFSET, 0.5);
	set_param(PARAM_SHADOW_FADE_START, 0.8);
	set_param(PARAM_SHADOW_PANCAKE_SIZE, 20.0);
	set_param(PARAM_SHADOW_OPACITY, 1.0);
	set_param(PARAM_SHADOW_BLUR, 1.0);
	set_param(PARAM_SHADOW_BIAS, 0.1);
	set_param(PARAM_SHADOW_NORMAL_BIAS, 1.0);
	set_param(PARAM_TRANSMITTANCE_BIAS, 0.05);
	set_param(PARAM_SHADOW_FADE_START, 1);
	// For OmniLight3D and SpotLight3D, specified in Lumens.
	set_param(PARAM_INTENSITY, 1000.0);
	set_temperature(6500.0); // Nearly white.
	set_disable_scale(true);
}

Light3D::Light3D() {
	ERR_PRINT("Light3D should not be instantiated directly; use the DirectionalLight3D, OmniLight3D or SpotLight3D subtypes instead.");
}

Light3D::~Light3D() {
	ERR_FAIL_NULL(RenderingServer::get_singleton());
	RS::get_singleton()->instance_set_base(get_instance(), RID());

	if (light.is_valid()) {
		RenderingServer::get_singleton()->free(light);
	}
}

/////////////////////////////////////////

void DirectionalLight3D::set_shadow_mode(ShadowMode p_mode) {
	shadow_mode = p_mode;
	RS::get_singleton()->light_directional_set_shadow_mode(light, RS::LightDirectionalShadowMode(p_mode));
	notify_property_list_changed();
}

DirectionalLight3D::ShadowMode DirectionalLight3D::get_shadow_mode() const {
	return shadow_mode;
}

void DirectionalLight3D::set_blend_splits(bool p_enable) {
	blend_splits = p_enable;
	RS::get_singleton()->light_directional_set_blend_splits(light, p_enable);
}

bool DirectionalLight3D::is_blend_splits_enabled() const {
	return blend_splits;
}

void DirectionalLight3D::set_sky_mode(SkyMode p_mode) {
	sky_mode = p_mode;
	RS::get_singleton()->light_directional_set_sky_mode(light, RS::LightDirectionalSkyMode(p_mode));
}

DirectionalLight3D::SkyMode DirectionalLight3D::get_sky_mode() const {
	return sky_mode;
}

void DirectionalLight3D::_validate_property(PropertyInfo &p_property) const {
	if (shadow_mode == SHADOW_ORTHOGONAL && (p_property.name == "directional_shadow_split_1" || p_property.name == "directional_shadow_blend_splits")) {
		// Split 2 and split blending are only used with the PSSM 2 Splits and PSSM 4 Splits shadow modes.
		p_property.usage = PROPERTY_USAGE_NO_EDITOR;
	}

	if ((shadow_mode == SHADOW_ORTHOGONAL || shadow_mode == SHADOW_PARALLEL_2_SPLITS) && (p_property.name == "directional_shadow_split_2" || p_property.name == "directional_shadow_split_3")) {
		// Splits 3 and 4 are only used with the PSSM 4 Splits shadow mode.
		p_property.usage = PROPERTY_USAGE_NO_EDITOR;
	}

	if (p_property.name == "light_size" || p_property.name == "light_projector" || p_property.name == "light_specular") {
		// Not implemented in DirectionalLight3D (`light_size` is replaced by `light_angular_distance`).
		p_property.usage = PROPERTY_USAGE_NONE;
	}

	if (p_property.name == "distance_fade_enabled" || p_property.name == "distance_fade_begin" || p_property.name == "distance_fade_shadow" || p_property.name == "distance_fade_length") {
		// Not relevant for DirectionalLight3D, as the light LOD system only pertains to point lights.
		// For DirectionalLight3D, `directional_shadow_max_distance` can be used instead.
		p_property.usage = PROPERTY_USAGE_NONE;
	}
}

void DirectionalLight3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_shadow_mode", "mode"), &DirectionalLight3D::set_shadow_mode);
	ClassDB::bind_method(D_METHOD("get_shadow_mode"), &DirectionalLight3D::get_shadow_mode);

	ClassDB::bind_method(D_METHOD("set_blend_splits", "enabled"), &DirectionalLight3D::set_blend_splits);
	ClassDB::bind_method(D_METHOD("is_blend_splits_enabled"), &DirectionalLight3D::is_blend_splits_enabled);

	ClassDB::bind_method(D_METHOD("set_sky_mode", "mode"), &DirectionalLight3D::set_sky_mode);
	ClassDB::bind_method(D_METHOD("get_sky_mode"), &DirectionalLight3D::get_sky_mode);

	ADD_GROUP("Directional Shadow", "directional_shadow_");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "directional_shadow_mode", PROPERTY_HINT_ENUM, "Orthogonal (Fast),PSSM 2 Splits (Average),PSSM 4 Splits (Slow)"), "set_shadow_mode", "get_shadow_mode");
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "directional_shadow_split_1", PROPERTY_HINT_RANGE, "0,1,0.001"), "set_param", "get_param", PARAM_SHADOW_SPLIT_1_OFFSET);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "directional_shadow_split_2", PROPERTY_HINT_RANGE, "0,1,0.001"), "set_param", "get_param", PARAM_SHADOW_SPLIT_2_OFFSET);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "directional_shadow_split_3", PROPERTY_HINT_RANGE, "0,1,0.001"), "set_param", "get_param", PARAM_SHADOW_SPLIT_3_OFFSET);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "directional_shadow_blend_splits"), "set_blend_splits", "is_blend_splits_enabled");
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "directional_shadow_fade_start", PROPERTY_HINT_RANGE, "0,1,0.001"), "set_param", "get_param", PARAM_SHADOW_FADE_START);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "directional_shadow_max_distance", PROPERTY_HINT_RANGE, "0,8192,0.1,or_greater,exp"), "set_param", "get_param", PARAM_SHADOW_MAX_DISTANCE);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "directional_shadow_pancake_size", PROPERTY_HINT_RANGE, "0,1024,0.1,or_greater,exp"), "set_param", "get_param", PARAM_SHADOW_PANCAKE_SIZE);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "sky_mode", PROPERTY_HINT_ENUM, "Light and Sky,Light Only,Sky Only"), "set_sky_mode", "get_sky_mode");

	BIND_ENUM_CONSTANT(SHADOW_ORTHOGONAL);
	BIND_ENUM_CONSTANT(SHADOW_PARALLEL_2_SPLITS);
	BIND_ENUM_CONSTANT(SHADOW_PARALLEL_4_SPLITS);

	BIND_ENUM_CONSTANT(SKY_MODE_LIGHT_AND_SKY);
	BIND_ENUM_CONSTANT(SKY_MODE_LIGHT_ONLY);
	BIND_ENUM_CONSTANT(SKY_MODE_SKY_ONLY);
}

DirectionalLight3D::DirectionalLight3D() :
		Light3D(RenderingServer::LIGHT_DIRECTIONAL) {
	set_param(PARAM_SHADOW_MAX_DISTANCE, 100);
	set_param(PARAM_SHADOW_FADE_START, 0.8);
	// Increase the default shadow normal bias to better suit most scenes.
	set_param(PARAM_SHADOW_NORMAL_BIAS, 2.0);
	set_param(PARAM_INTENSITY, 100000.0); // Specified in Lux, approximate mid-day sun.
	set_shadow_mode(SHADOW_PARALLEL_4_SPLITS);
	blend_splits = false;
	set_sky_mode(SKY_MODE_LIGHT_AND_SKY);
}
//Beginning of Evan Nibbe's major edit to run a C++ function to do the calculations for how the AI should move Triangle Chess pieces.
#ifdef DEBUG_LIGHT
//#include "debug_helper.h" //apparently, this causes godot to crash on println.
//#else
void println(int a) {
	char filename[15]="0000000000.bin";
	int temp=a;
	for (int i=9; i>=0 && temp>0; i--) {
		filename[i]=(char)('0'+temp%10);
		temp/=10;
	}
	FILE *fp=fopen(filename, "a");
	fprintf(fp, ".");
	fclose(fp);
}
void print_word_num_on_line(char *str1, int put, int a) {
	char filename[]="debug_0000000000.bin";
	int temp=a;
	for (int i=9; i>=0 && temp>0; i--) {
		filename[i]=(char)('0'+temp%10);
		temp/=10;
	}
	FILE *fp=fopen(filename, "a");
	
	fprintf(fp, " %s\t%d\n", str1, put);
	fclose(fp);

}
#else
void println(int a) {
	;
}
void print_word_num_on_line(char *str1, int put, int a) {
	;
}
#endif
//#include <unordered_map>
//std::unordered_map<int, Vector3> hash_pos_to_spot_table;
#define DEFAULT_SIZE 27307
Vector3 hash_pos_to_spot_table[DEFAULT_SIZE];
//std::unordered_map<Node3D*,bool> is_my_piece_table;
int is_my_piece_table[DEFAULT_SIZE];
//-1 is "don't know"
//0 is false
//1 is true
//std::unordered_map<Node3D *, int> pawn_improved_table;
int pawn_improved_table[DEFAULT_SIZE];

//std::unordered_map<Node3D *, float> spiral_directions; 
float spiral_directions[DEFAULT_SIZE];
//maps a spiral_rook onto which direction the spiral rotates based on whether the initial x coordinate 
//is smaller than 0 (in which case use radius = 1.1 ^ (-theta) as valid resulting spots), otherwise to use 
//radius=1.1^theta

//std::unordered_map<Node3D *, int> piece_to_type_table;
int piece_to_type_table[DEFAULT_SIZE];
/*
10000 is king
200 is spiral_rook
130 is rook
40 is bishop
5 is knight
1 is pawn
0 is other
*/

struct node3DIndex {
	Node3D *p;
	int i;
};

typedef struct node3DIndex ptrint;

int count_unique_pieces=-1;
#define MAX_PIECES 100
ptrint is_my_piece_hash_positions[MAX_PIECES];
int piece_seen_when[DEFAULT_SIZE]; //maps to a piece's index in is_my_piece_hash_positions from the initial index.
int line_call_is_my_piece_hash; //for use in debugging, printing which line was calling is_my_piece_hash
int is_my_piece_hash(Node3D *piece) {
	if (piece==nullptr) {
		
		return -1;
	}
	void *thing=(void*)piece;
	long num=((long)thing);
	if (num<0) {
		num=-num; //make positive
	}
	//I checked wolframalpha.com that 1000000007 is a prime number.
	num=num%1000000007; //make in the range of positive ints.
	int result=num%DEFAULT_SIZE;
	if (count_unique_pieces==-1) {//this is the first time this code has run
		count_unique_pieces=0;
		for (int i=0; i<DEFAULT_SIZE; i++) {
			is_my_piece_table[i]=-1;
			piece_seen_when[i]=-1;
		}
		for (int i=0; i<MAX_PIECES; i++) {
			is_my_piece_hash_positions[i].p=nullptr;
			is_my_piece_hash_positions[i].i=-1;
		}
	}
	if (piece_seen_when[result]==-1) {
		//note: this means the piece in question was never seen before
		piece_seen_when[result]=count_unique_pieces;
		is_my_piece_hash_positions[count_unique_pieces].p=piece;
		is_my_piece_hash_positions[count_unique_pieces].i=result;
		if (count_unique_pieces<MAX_PIECES) {
			count_unique_pieces+=1;
		}
	} else {
		//either this piece was seen before, or another piece that maps to the same index was seen before, in which case we increment to where the proper result index should be.
		int was_there=piece_seen_when[result];
		Node3D *compare=is_my_piece_hash_positions[was_there].p;
		if (compare==piece) {
			return result;
		} else {
			while (compare!=nullptr && compare!=piece) {
				result+=1;
				result=result%DEFAULT_SIZE;
				was_there=piece_seen_when[result];
				compare=is_my_piece_hash_positions[was_there].p;
			}
			if (compare==nullptr && count_unique_pieces<MAX_PIECES) {
				piece_seen_when[result]=count_unique_pieces;
				is_my_piece_hash_positions[count_unique_pieces].p=piece;
				is_my_piece_hash_positions[count_unique_pieces].i=result;
				count_unique_pieces+=1;
			}
		}
	}
	return result;
}

int piece_to_type(Node3D *a) {
	if (a==nullptr) {
		return 0;
	} 
	if (a->get_second_global_position().y<-10) {
		return 0; //dead pieces don't count
	}
	 int num=is_my_piece_hash(a);
	
	int  search=piece_to_type_table[num]; 
	if (search!=-1) { //ERROR
		return search;
	} else {
		return 0;
	}
}

#include <vector> //This is necessary because the Array Godot class doesn't work with my design, 
				//my design requires me to use as few casting steps as possible, because
				//the time constraints at the lowest levels are really tight given the thousands of combinatorial problems.
#define MIN_X_POS -16
#define MAX_X_POS 16
#define MAX_Z_POS 20
#define MIN_Z_POS -20

float min(float a, float b) {
	if (a>b) {
		return b;
	} else {
		return a;
	}
}
float absf(float a) {
	if (a<0) {
		return -a;
	} else {
		return a;
	}
}
float rad_to_deg(float a) {
	return a*180/3.141592653589793;
}
float deg_to_rad(float a) {
	return a*3.141592653589793/180;
}
float point_to_line(Vector3 global_pos, Vector3 start, Vector3 end) {
	float result=0;
	if (global_pos.y<-500 || start.y<-500 || end.y<-500) {
		result=10000000;
	} else {
		float a=(start-end).length();
		float b=(end-global_pos).length();
		float c=(global_pos-start).length();
		float area=0.25*sqrt((a+b+c)*(-a+b+c)*(a-b+c)*(a+b-c));
		result=2*area/a;
	}
	return result;
}

int simple_hash_pos(float x, float z) { 
	//with the commented out code, I discovered the hashes range from 1578 to 3785
	//to be safe, I can just use 4000 spots in an array of bools, and use the addresses directly. 
	int result=int((x-MIN_X_POS)*5.1);
	result=int(result*(MAX_Z_POS-MIN_Z_POS) + 5.1*(z - MIN_Z_POS));
	if (result<0) {
		result=-result;
	}
	return result;
}


int hash_pos(Vector3 pos) {
	int result=simple_hash_pos(pos.x, pos.z);
	hash_pos_to_spot_table[result%DEFAULT_SIZE]= pos; //ERROR
	return result;
}




Vector3 hash_pos_to_spot(int hash) {
	if (hash<0) {
		return Vector3_DOWN;
	}
	return hash_pos_to_spot_table[hash%DEFAULT_SIZE];
}


	//I have checked this function call again and again, and I see that the segmentation fault didn't occur in this function call
Node3D *fast_piece_at(Vector3 spot,  std::vector<Node3D*> my_pieces,  std::vector<Node3D*> human_pieces) {
	Node3D *result=my_pieces[0];
	//println(__LINE__); //this line got 66,933,083 calls over the course of an hour.
	
	
	float closest=(my_pieces[0]->get_second_global_position()-spot).length_squared();
	for (int i=1; i<my_pieces.size(); i++) {
		Vector3 sp2=my_pieces[i]->get_second_global_position();
		if (sp2.y>-1) {
			float d2=(sp2-spot).length_squared();
			if (d2<closest) {
				closest=d2;
				result=my_pieces[i];
			}
		}
	}
	for (int i=0; i<human_pieces.size(); i++) {
		Vector3 sp2=human_pieces[i]->get_second_global_position();
		if (sp2.y>-1) {
			float d2=(sp2-spot).length_squared();
			if (d2<closest) {
				closest=d2;
				result=human_pieces[i];
			}
		}
	}
	//println(__LINE__); //check to see if segmentation fault occured strictly during this function call.
	return result;
}


//returns -1 if piece is not in is_my_piece_table
//returns 0 if piece is a human piece
//returns 1 if piece is an AI piece 
//I have checked this function again and again, and the source of the segmentation fault doesn't come from here.
int line_call_is_my_piece=0;
int is_my_piece(Node3D *piece) {
	int result=-1;
	int num=is_my_piece_hash(piece);
	if (num<0) {
		
		return -1;
	}
	int search=is_my_piece_table[num];
	if (search!=-1) {
		if (search==1) {
			result=1; //position got back true, meaning an AI piece.	
		} else {
			result=0; //position got back false, meaning human piece.
		}
	}
	return result;
}
//I have tested this function again and again. the segmentation fault doesn't come from this function.
//returns false if movement is blocked; returns true if movement is valid.
bool fast_ray_cast(Node3D *piece, Vector3 start, Vector3 end, std::vector<Node3D*> my_pieces,  std::vector<Node3D*> human_pieces,  Vector3 *accepted_positions) {
	bool result=true;
	//println(__LINE__); //this line got 2,731,835 calls over the course of an hour. 
	//cycle through positions from start to end, then check to see if it is blocked according 
	//to whether the given piece discovered is (1) ==piece (not blocked), or 
	//(2) within sqrt(3)/3 of end and belonging to the array of pieces that does not contain piece 	//(only one of the two arrays need be searched, if it contains one and not the other, then that 	//piece would be taken, meaning that the ray is not blocked, meaning return true).
	//We have to make sure that temporary_piece_positions actually contains the pieces that have recently moved.
	float AB=(start-end).length();
	Node3D *found=nullptr;
	for (int i=0; i<my_pieces.size() && result==true; i++) {
		if (my_pieces[i]==nullptr || my_pieces[i]==piece) {
			continue;
		} else {
			Vector3 c=my_pieces[i]->get_second_global_position();
			float AC=(start-c).length();
			float BC=(end-c).length();
			if (AC>AB+.1 || BC>AB) { //the +.1 is to account for the fact that a piece could be exactly at end.
				continue;
			} else {
				if (point_to_line(c,  start,  end)<sqrt(3)/3) {
					result=false;
					Node3D *temp=found; //needs to be preserved just in case the way the loop is done is ever changed.
					found=my_pieces[i];
					
					//now we check if this piece is an opponent of piece located within sqrt(3)/3 of end.
					
					if (is_my_piece(piece)==is_my_piece(found)) {
						//don't need to do anything.
						continue;
					} else { //we can make found be nullptr and result=true (that a piece is allowed to move along the path)
						//but only if found is at end.
						if (BC<sqrt(3)/3) {
							result=true;
							found=temp; //just in case the way the loop is done is ever changed.
						}
					}
				}
			}
		}
	}
	for (int i=0; i<human_pieces.size() && result==true; i++) {
		if (human_pieces[i]==nullptr || human_pieces[i]==piece) {
			continue;
		} else {
			Vector3 c=human_pieces[i]->get_second_global_position();
			float AC=(start-c).length();
			float BC=(end-c).length();
			if (AC>AB+.1 || BC>AB) { //the +.1 is to account for the fact that a piece could be exactly at end.
				continue;
			} else {
				if (point_to_line(c,  start,  end)<sqrt(3)/3) {
					result=false;
					Node3D *temp=found; //needs to be preserved just in case the way the loop is done is ever changed.
					found=human_pieces[i];
					
					//now we check if this piece is an opponent of piece located within sqrt(3)/3 of end.
					
					if (is_my_piece(piece)==is_my_piece(found)) {
						//don't need to do anything.
						continue;
					} else { //we can make found be nullptr and result=true (that a piece is allowed to move along the path)
						//but only if found is at end.
						if (BC<sqrt(3)/3) {
							result=true;
							found=temp; //just in case the way the loop is done is ever changed.
						}
					}
				}
			}
		}
	}
	
	
	//println(__LINE__);
	return result;
}

bool is_redblackbrown(Vector3 spot) {
	bool result;
	Vector2 txtbk=Vector2(spot.x, -spot.z+13*sqrt(3)+.0001);
	int careful_mod=int(txtbk.y*3/sqrt(3)+.5);
	
	if (careful_mod%3==1) {
		result=true;
	} else if (careful_mod%3==2) {
		result=false;
	} else { //the below line was never called in an hour of running, which is good.
		println(__LINE__); //otherwise something went wrong, but so far that has not occurred with this code before,
	//other than in other fixed circumstances that I resolved while working in GDScript.
	} 
	return result;
}

//It seems fun to have the pawns gradually increase in power over the kights as the pawns approach the back ranks, while becoming more powerful than normal pawns just after reaching the place before the starting ranks.

//Note: the king referred to by king_start_pos needed to be of the same color as the pawn
//king_start_pos is always my_king_start_pos
//opp_king_pos is always human_king_start_pos
int pawn_improved(Node3D *pawn, Vector3 king_start_pos, Vector3 opp_king_start_pos) {
	int result=0;
	
	int num=is_my_piece_hash(pawn);
	int search=pawn_improved_table[num];
	if (search!=0) {
		result=search;
	} 
	int improvement;
	
	if (is_my_piece(pawn)) {
		improvement=int(((pawn->get_second_global_position()-king_start_pos).dot((king_start_pos-opp_king_start_pos).normalized())));
	} else {
		improvement=int(((pawn->get_second_global_position()-opp_king_start_pos).dot((opp_king_start_pos-king_start_pos).normalized())));
	}
	improvement-=int(18*sqrt(3));
	if (improvement>result) {
		//println(__LINE__); //has 26 calls made to it over the course of an hour.
		result=improvement;
	}
	
	pawn_improved_table[num%DEFAULT_SIZE]= result;
	return result;
}

//no need to consider self-capture since that is prevented by the fast_ray_cast function.
//note that we will need some way of indicating that a pawn is promoted.
//king_start_pos is always my_king_start_pos
//opp_king_pos is always human_king_start_pos
int pawn_hash_pos_to_can_move[DEFAULT_SIZE];
int relative_hash(Node3D*piece, Vector3 global_pos) {
	int result=-10000;
	if (piece!=nullptr && piece->get_second_global_position().y>-10) {
		result=hash_pos(global_pos-piece->get_second_global_position());
		result=result*2;
		if (is_redblackbrown(piece->get_second_global_position())) {
			result+=1;
		}
	}
	return result;
}
//-1 means unknown
//0 means known false
//1 means known true
int simple_pawn_can_move(Node3D*piece, Vector3 global_pos, int set) {
	//println(__LINE__); //has 1,790,664 calls to it over the course of an hour.
	int result=-1;
	int index=relative_hash(piece, global_pos)*2;
	
	if (is_my_piece(piece)) {
		index+=1; //because pawns owned by me and my opponent are different.
	}
	if (set>-1) {
		pawn_hash_pos_to_can_move[index%DEFAULT_SIZE]= set;
		result=set;
	} else {
		int search=pawn_hash_pos_to_can_move[index%DEFAULT_SIZE];
		if (search!=-1) {
			result=search;
			//println(__LINE__); //has 0 calls to it over the course of an hour.
		}
	}
	return result;
}

bool pawn_can_move_to(Node3D *pawn, Vector3 global_pos, Vector3 king_start_pos, Vector3 opp_king_start_pos) { 
	bool result=false;
	//println(__LINE__); //has 1,790,664 calls to it over the course of an hour
	if (pawn->get_second_global_position().y< -10) {
		result=false;
	} else if (global_pos.y < -10) {
		result=true; //it is always permitted for a piece to die.
	} else {//not the above two easy cases.
		int improved=pawn_improved(pawn, king_start_pos, opp_king_start_pos);
		if ( improved ) {
			if ((pawn->get_second_global_position()-global_pos).length()<2.1*improved) {
				result=true; //the pawn can move as it pleases within 2 times the value of 
						//improved around it.
			}
		} else { //not one of the two easy cases and not improved
			int simple_case=simple_pawn_can_move(pawn, global_pos, -1); //-1 because we don't know unless it is there.
			if (simple_case==1) {
				result=true;
			} else if (simple_case==0) {
				result=false;
			} else { //it makes 0 sense that the simple_pawn_can_move gets called (meaning that the improved code that executes doesn't result in the faster improved codepath), but yet the result is clear from simple_pawn that there is never a time that it returns something that is not -1.
				//println(__LINE__); //has 0 calls to it over the course of an hour; 
				
				if (is_my_piece(pawn)) {
					//The direction of movement is no more than 60 degrees 
					//off of the direction from king_start_pos to opp_king_start_pos.
					//60 degrees is basically equivalent to 1.05 radians (rounded up)
					Vector2 txtbk=Vector2(global_pos.x-pawn->get_second_global_position().x, -(global_pos.z-pawn->get_second_global_position().z));
					
					Vector2 standard_dir=Vector2(opp_king_start_pos.x-king_start_pos.x, -(opp_king_start_pos.z - king_start_pos.z));
					float dist=sqrt(txtbk.dot(standard_dir.normalized())); //we just have to 
							//make sure the distance moved
							//forward is no greater than 4*sqrt(3)/3
					float angle=txtbk.angle_to(standard_dir);
					if (dist<4*sqrt(3)/3+.2 && angle<1.05 && txtbk.length_squared()<5.5) {
						result=true;
					} else {
						result=false;
					} 
				} else {
					//The direction of movement is no more than 60 degrees 
					//off of the direction from opp_king_start_pos to king_start_pos.
					//60 degrees is basically equivalent to 1.05 radians (rounded up)
					Vector2 txtbk=Vector2(global_pos.x-pawn->get_second_global_position().x, -(global_pos.z-pawn->get_second_global_position().z));
					
					Vector2 standard_dir=Vector2(king_start_pos.x-opp_king_start_pos.x, -(king_start_pos.z - opp_king_start_pos.z));
					float dist=sqrt(txtbk.dot(standard_dir.normalized())); //we just have to 
							//make sure the distance moved
							//forward is no greater than 4*sqrt(3)/3
					float angle=txtbk.angle_to(standard_dir);
					if (dist<4*sqrt(3)/3+.2 && angle<1.05 && txtbk.length_squared()<5.5) {
						result=true;
					} else {
						result=false;
					}
				}
				if (result) {
					simple_pawn_can_move(pawn, global_pos, 1);
				} else {
					simple_pawn_can_move(pawn, global_pos, 0);
				}
			}
			
		}
	}
	
		
	return result;
}

int bishop_hash_pos_to_can_move[DEFAULT_SIZE];
int simple_bishop_can_move(Node3D*piece, Vector3 global_pos, int set) {
	int result=-1;
	int index=relative_hash(piece, global_pos);
	
	if (set>-1) {
		bishop_hash_pos_to_can_move[index%DEFAULT_SIZE]= set;
		result=set;
	} else {
		int search=bishop_hash_pos_to_can_move[index%DEFAULT_SIZE];
		if (search!=-1) {
			result=search;
		}
	}
	return result;
}

bool bishop_can_move_to(Node3D *bishop, Vector3 global_pos, Vector3 king_start_pos, Vector3 opp_king_start_pos) {
	//println(__LINE__); //called 36465
	bool result=false;
	if (bishop->get_second_global_position().y< -10) {
		result=false;
	} else if (global_pos.y < -10) {
		result=true; //it is always permitted for a piece to die.
	} else {//not the above two easy cases.
		int simple_case=simple_bishop_can_move(bishop, global_pos, -1); //-1 because we don't know.
		if (simple_case==1) {
			result=true;
		} else if (simple_case==0) {
			result=false;
		} else {
			//println(__LINE__);
			Vector2 txtbk=Vector2(global_pos.x-bishop->get_second_global_position().x, -(global_pos.z - bishop->get_second_global_position().z));
			if (txtbk.length()>2*sqrt(3)/3+sqrt(3)/12 && txtbk.length()<4*sqrt(3)/3+sqrt(3)/5) {
				result=true;
			} else { //Now figure out if global_pos is on a line with the center of the current
				//triangle and a corner of the current triangle
				float degree_variance=1.0; //remember to convert from radians to degrees.
				if ( is_redblackbrown(bishop->get_second_global_position())) {
					Vector2 up=Vector2(0, 2*sqrt(3)/3);
					Vector2 down_right=Vector2(1, -sqrt(3)/3);
					Vector2 down_left=Vector2(-1, -sqrt(3)/3);
					
					float angle=min(txtbk.angle_to(up), min(txtbk.angle_to(down_right), txtbk.angle_to(down_left)));
					angle=rad_to_deg(angle);
					if (angle<degree_variance) {
						result=true;
					} else {
						result=false;
					}
				} else { //bishop not on redblackbrown
					Vector2 down=Vector2(0, -2*sqrt(3)/3);
					Vector2 up_right=Vector2(1, sqrt(3)/3);
					Vector2 up_left=Vector2(-1, sqrt(3)/3);
					float angle=min(txtbk.angle_to(down), min(txtbk.angle_to(up_right), txtbk.angle_to(up_left)));
					angle=rad_to_deg(angle);
					if (angle<degree_variance) {
						result=true;
					} else {
						result=false;
					}
				}
			}
		}
		
	}
	if (result) {
		simple_bishop_can_move(bishop, global_pos, 1);
	} else {
		simple_bishop_can_move(bishop, global_pos, 0);
	}
	return result;
}



int rook_hash_pos_to_can_move[DEFAULT_SIZE];
int simple_rook_can_move(Node3D*piece, Vector3 global_pos, int set) {
	int result=-1;
	int index=relative_hash(piece, global_pos);
	
	if (set>-1) {
		rook_hash_pos_to_can_move[index%DEFAULT_SIZE]= set;
		result=set;
	} else {
		int search=rook_hash_pos_to_can_move[index%DEFAULT_SIZE];
		if (search!=-1) {
			result=search;
		}
	}
	return result;
}

bool rook_can_move_to(Node3D *rook, Vector3 global_pos, Vector3 king_start_pos, Vector3 opp_king_start_pos) {
	bool result=false;
//	println(__LINE__); //called 14586
	if (rook->get_second_global_position().y < -10) {
		result=false;
	} else if (global_pos.y<-10) {
		result=true; //it is always permitted for a piece to die
	} else { //not the above two easy cases.
		//println(__LINE__);
		int simple_case=simple_rook_can_move(rook, global_pos, -1); //-1 because we don't know.
		if (simple_case==1) {
			result=true;
		} else if (simple_case==0) {
			result=false;
		} else {
			Vector3 pos=global_pos - rook->get_second_global_position();
			pos.x=absf(pos.x);
			pos.y=0;
			pos.z=absf(global_pos.z);
			if (pos.x + pos.z>1.0/3.0) {
				float d1=point_to_line(pos, Vector3_ZERO, Vector3_RIGHT);
				Vector3 line_2=pos.rotated(Vector3_UP, deg_to_rad(60));
				float d2=min(d1, point_to_line(line_2, Vector3_ZERO, Vector3_RIGHT));
				if (d2<sqrt(3)/2) {
					result=true;
				}
			}
		}
		
	}
	if (result) {
		simple_rook_can_move(rook, global_pos, 1);
	} else {
		simple_rook_can_move(rook, global_pos, 0);
	}
	return result;
}

int sp_rook_hash_pos_to_can_move[DEFAULT_SIZE];
int simple_sp_rook_can_move(Node3D*piece, Vector3 global_pos, int set) {
	int result=-1;
	int index=relative_hash(piece, global_pos);
	
	if (set>-1) {
		sp_rook_hash_pos_to_can_move[index%DEFAULT_SIZE]= set;
		result=set;
	} else {
		int search=sp_rook_hash_pos_to_can_move[index%DEFAULT_SIZE];
		if (search!=-1) {
			result=search;
		}
	}
	return result;
}

bool spiral_rook_can_move_to(Node3D *rook, Vector3 global_pos, Vector3 king_start_pos, Vector3 opp_king_start_pos) {
	println(__LINE__);
	bool result=false;
	
	int num=is_my_piece_hash(rook);
	
	float  search=spiral_directions[num];
	float sp_dir=0;
	if (search!=0) {
		sp_dir=search;
	} else { //set spiral direction based on initial x coordinate being < or > 0.
		if (rook->get_second_global_position().x<0) {
			sp_dir=-1;
		} else {
			sp_dir=1;
		}
		spiral_directions[num%DEFAULT_SIZE]= sp_dir;
	}
	if (rook->get_second_global_position().y < -10) {
		result=false;
	} else if (global_pos.y<-10) {
		result=true; //it is always permitted for a piece to die
	} else { //not the above two easy cases.
		int simple_case=simple_sp_rook_can_move(rook, global_pos, -1); //-1 means unknown
		if (simple_case==1) {
			result=true;
		} else if (simple_case==0) {
			result=false;
		} else {
			println(__LINE__);
			Vector3 pos=global_pos-rook->get_second_global_position();
			float right=-pos.z;
			//the complicated math here arises from how geometry textbooks like to use the 
			//x direction as “right”, and the y direction as “up within the flat page”
			//so I have to change the values here thus.
			float up=pos.x;
			pos.x=right;
			pos.y=up;
			pos.z=0;
			float r=pos.length();
			if (r>sqrt(3)/3) { //valid move that goes far enough away from start point
				if (r<sqrt(3)+.1) { //another simple case
					result=true;
				} else {
					float theta= log(r)/log(1.1)/sp_dir; //sp_dir is either 1 or -1
					Vector3 t_pos=Vector3(r*cos(theta), r*sin(theta), 0);
					if ((t_pos-pos).length()<2*sqrt(3)/3) {
						result=true;
					} else { //need to figure out how far off the spot is from an 
						//acceptable spot using .signed_angle_to and 
						//binary search.
						Vector3 axis=Vector3(0, 0, 1);
						float dt=pos.signed_angle_to(t_pos, axis); 
						//if dt is negative, then increasing theta by abs(dt) 
						//should give us the point that is directly on line with the 
						//spot in question.
						//if dt is positive, then decreasing theta by -abs(dt)
						//should give us the point that is directly on line with the
						//spot in question.
						theta-=dt;
						t_pos.x=r*cos(theta);
						t_pos.y=r*sin(theta);
						if ((t_pos-pos).length()<2*sqrt(3)/3) {
							result=true;
						}//should be accurate enough
						//no need to waste more time on giving the AI extra uses
						//of spiral_rooks.
					}
				}
			}
		}
	}
	if (result) {
		simple_sp_rook_can_move(rook, global_pos, 1);
	} else {
		simple_sp_rook_can_move(rook, global_pos, 0);
	}
	return result;
}

//the below two calculations are so fast that I don't think it is worth it to use the hashTable, it might just take more time.
bool knight_can_move_to(Node3D *knight, Vector3 global_pos, Vector3 king_start_pos, Vector3 opp_king_start_pos) {
	//println(__LINE__);
	bool result=false;
	Vector3 d1=(global_pos-knight->get_second_global_position());
	d1.y=0;
	if (knight->get_second_global_position().y<-500) {
		result=false; //no move for dead piece
	} else if (global_pos.y<-500) {
		result=true; //pieces are allowed to die.
	} else {
		float dist=d1.length();
		if (dist<=2*sqrt(5)+sqrt(3)/6 && dist>=sqrt(15)-sqrt(3)/6) {
			result=true;
		} //forms the circular ring of acceptable spots.
	}

	return result;
}

bool king_can_move_to(Node3D *king, Vector3 global_pos, Vector3 king_start_pos, Vector3 opp_king_start_pos) {
	//println(__LINE__);
	bool result=false;
	Vector3 d1=(global_pos-king->get_second_global_position());
	d1.y=0;
	if (king->get_second_global_position().y<-500) {
		result=false; //no move for dead piece
	} else if (global_pos.y<-500) {
		result=true; //pieces are allowed to die.
	} else {
		float dist=d1.length();
		if (dist<=2.453 && dist>=sqrt(3)/6) {
			result=true;
		} //forms the circular ring of acceptable spots.
	}

	return result;
}
#define BLACKLIST_SIZE 100
Node3D* possible_blacklist_of_pieces[BLACKLIST_SIZE]; //just holds each Node3D pointer for all pieces.
int possible_blacklist_of_pieces_num_bad[BLACKLIST_SIZE];  //holds the number of times that the piece in question did not have a workable move (greater than 6 
int real_nth_optimal=0;
bool blacklist_setup_yet=false;
bool self_calling_back=false;
int already_visited_table[DEFAULT_SIZE];
int already_visited_table_size=0;
int already_visited_hash(Node3D *p, Vector3 s) {
	int result=-10000;
	if (p!=nullptr && p->get_second_global_position().y>-10) { 
		int num=is_my_piece_hash(p);
		result=num;
		result=result*1000 + hash_pos(p->get_second_global_position());
		result=result*1000 + hash_pos(s);
	}
	result=result%DEFAULT_SIZE;
	return result;
}
bool already_visited(Node3D *p, Vector3 s) {
	bool result=false;
	int index=already_visited_hash(p, s);
	if (index>-1) {
		int search=already_visited_table[index];
		result=(bool)search;
		if (search==-1) {
			result=false;
		}
	}
	return result;
}

void set_already_visited(Node3D *p, Vector3 s) {
	for (int i=0; i<BLACKLIST_SIZE; i++) {
		if (p==possible_blacklist_of_pieces[i]) {
			possible_blacklist_of_pieces_num_bad[i]+=1;
		}
	}
	int index=already_visited_hash(p, s);
	if (index>-1) {
		if (already_visited_table[index]==0) {
			already_visited_table_size+=1;
		}
		already_visited_table[index]= true;
	}
}

//extremely simple function it seems, but already runs into the problem of
//1: needs persistent storage to know that this piece started at one end of the board
//2: needs persistent storage to know that this piece (if a pawn) has or has not been promoted.
//3: needs to know if there is a piece in the way of its movement (that can be resolved by
//     a different function)
bool fast_can_move_to(Node3D *piece, Vector3 start, Vector3 end, std::vector<Node3D*> my_pieces, Vector3 my_king_start, std::vector<Node3D*> human_pieces, Vector3 human_king_start, Vector3 *accepted_positions) {
	//println(__LINE__); //called 167739 //called same number of times as corresponding one before return statement, so the segmentation fault error didn't occur in this function.
	bool result=false; 
	bool can_move=false; //set within the chain of below items based on what type of piece it is
	if (already_visited(piece, end)) {
		return false;
	}
	//if (accepted_positions[hash_pos(end)%DEFAULT_SIZE]!=Vector3_DOWN) { //should always be true; I can check whether this causes some lines to not run as a metric for whether accepted_positions is working.
		switch (piece_to_type(piece))
			{
			case 1:
				can_move=pawn_can_move_to(piece, end, my_king_start, human_king_start);
				break;
			
			case 5:
				can_move=knight_can_move_to(piece, end, my_king_start, human_king_start);
				
				if (can_move) {
					Node3D *found=fast_piece_at(end,  my_pieces,  human_pieces);
					if (found==piece || found==nullptr) {
						result=true;
					} else if ((found->get_second_global_position()-end).length_squared()>1/3.0) { //the piece found was not actually at the relevant position.
						result=true;
					} else if (is_my_piece(piece)!=is_my_piece(found)) {
						result=true; //the piece at the end location is of the opposite color.
					}
				}
				break;
			case 40:
				can_move=bishop_can_move_to(piece, end, my_king_start, human_king_start);
				break;
			case 130:
				can_move=rook_can_move_to(piece, end, my_king_start, human_king_start);
				break;
			case 200:
				can_move=spiral_rook_can_move_to(piece, end, my_king_start, human_king_start);
				break;
			case 10000:
				
				can_move=king_can_move_to(piece, end, my_king_start, human_king_start);
				if (can_move) {
					Node3D *found=fast_piece_at(end,  my_pieces,  human_pieces);
					if (found==piece || found==nullptr) {
						result=true;
					} else if ((found->get_second_global_position()-end).length_squared()>1/3.0) { //the piece found was not actually at the relevant position.
						result=true;
					} else if (is_my_piece(piece)!=is_my_piece(found)) {
						result=true; //the piece at the end location is of the opposite color.
					}
				}
				break;
			default:
				break;
			}
	//}
	result=can_move && (result || fast_ray_cast(piece, start, end, my_pieces,  human_pieces, accepted_positions));
	//println(__LINE__); //called 167739
	return result;
}

int error_move=0;
//precondition: moving to_move to spot must be an allowed move.
float value_from_state(Node3D *to_move, Vector3 spot,  std::vector<Node3D*> my_pieces, Vector3 my_king_start, std::vector<Node3D*> human_pieces, Vector3 human_king_start, std::vector<Vector3> possible_positions, Vector3 *accepted_positions) {
	//println(__LINE__); //called 44
	float result=0; error_move=0;
	Vector3 st_pos=to_move->get_second_global_position();
	Node3D *atkp=fast_piece_at(spot,  my_pieces,  human_pieces);
	if (atkp!=nullptr && (atkp->get_second_global_position()-spot).length_squared()>1/3.0) {
		atkp=nullptr; //it was not in range, so it might as well not exist.	
	}
	Vector3 atkpst=Vector3_ZERO;
	
	bool my_move=(is_my_piece(to_move)==is_my_piece(my_pieces[0])); //good, since failure only occurred w.r.t. line 1536, that means both to_move and my_pieces[0] are non-null, which is good.
	
	if (is_my_piece(to_move)==is_my_piece(atkp)) { //q.e.d., atkp is null all 94 times that we see something being sent to is_my_piece while null, meaning that the fact that is_my_piece was called on null 94 times was just a wild goose chase in terms of it not being the real issue with the program.
		atkp=nullptr; //not something that can be attacked.
		if (my_move) {
			result=-10000; error_move=-10000; //prevent choosing this if my_move.
		} else {
			result=10000; error_move=10000; //the human movements are trying to choose the moves which 
					//reduce the value of the value_from_state function.
		}
	} else { //a move that is not a self-capture.
		if (atkp!=nullptr) {
			atkpst=atkp->get_second_global_position();
			atkp->global_translate(Vector3(0, -10000, 0));
		}
		to_move->global_translate(spot - st_pos);
		for (int i=0; i<my_pieces.size(); i++) {
			//println(__LINE__); //called 575
			Node3D *piece=my_pieces[i];
			Vector3 start=piece->get_second_global_position();
			if (start.y>-1) {
				result+=piece_to_type(piece);
			}
		}
		for (int i=0; i<human_pieces.size(); i++) {
			//println(__LINE__); //called 575
			Node3D *piece=human_pieces[i];
			Vector3 start=piece->get_second_global_position();
			if (start.y>-1) { //if piece is alive
				result-=piece_to_type(piece);
			}
		}
	}

	//restore relevant pieces to spots.
	to_move->global_translate(st_pos - to_move->get_second_global_position());
	if (atkp!=nullptr) {
		atkp->global_translate(atkpst - atkp->get_second_global_position());
	}
	//println(__LINE__); //called 44, meaning program did not crash within this function.
	return result;
}


int possible_inc[15711]={2, 3, 1, 1, 2, 1, 1, 1, 2, 3, 2, 3, 3, 4, 1, 1, 1, 1, 1, 5, 2, 2, 1, 3, 3, 1, 2, 1, 4, 2, 5, 5, 3, 2, 1, 1, 1, 1, 1, 2, 1, 1, 3, 1, 1, 1, 1, 1, 1, 3, 1, 5, 2, 1, 2, 1, 1, 2, 1, 3, 1, 1, 1, 3, 1, 1, 4, 1, 2, 1, 2, 2, 1, 4, 3, 2, 2, 3, 4, 2, 3, 1, 3, 5, 1, 1, 1, 2, 1, 2, 3, 2, 1, 1, 3, 4, 3, 1, 1, 2, 1, 2, 1, 2, 2, 3, 1, 5, 1, 1, 2, 3, 2, 2, 3, 4, 2, 3, 2, 1, 2, 1, 3, 2, 2, 2, 2, 1, 1, 2, 4, 2, 1, 1, 1, 3, 4, 3, 1, 3, 1, 5, 1, 2, 4, 2, 2, 1, 2, 2, 4, 4, 3, 4, 2, 4, 4, 4, 2, 1, 1, 1, 2, 1, 1, 2, 3, 3, 1, 1, 2, 2, 2, 1, 1, 3, 1, 1, 1, 1, 2, 3, 3, 1, 2, 3, 1, 1, 1, 3, 1, 4, 4, 3, 1, 2, 2, 2, 1, 5, 3, 1, 3, 3, 2, 1, 3, 1, 1, 1, 2, 1, 3, 4, 1, 3, 1, 2, 5, 2, 1, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 4, 1, 1, 1, 1, 3, 1, 4, 2, 1, 1, 1, 1, 3, 1, 1, 2, 1, 1, 2, 1, 1, 2, 3, 4, 2, 2, 3, 3, 1, 1, 2, 3, 1, 3, 1, 1, 2, 2, 2, 2, 1, 4, 1, 2, 1, 2, 2, 4, 1, 1, 2, 1, 4, 3, 1, 1, 5, 3, 1, 3, 4, 1, 1, 2, 3, 3, 1, 4, 3, 1, 1, 2, 1, 3, 1, 2, 1, 1, 1, 4, 1, 1, 4, 3, 1, 2, 5, 1, 1, 1, 5, 3, 3, 1, 1, 4, 4, 2, 1, 1, 1, 3, 1, 4, 3, 1, 1, 2, 1, 1, 5, 1, 1, 5, 1, 1, 1, 2, 1, 1, 1, 3, 1, 2, 3, 5, 2, 1, 2, 1, 2, 1, 1, 2, 1, 1, 2, 2, 5, 1, 1, 1, 2, 1, 2, 3, 1, 2, 2, 1, 3, 4, 1, 1, 4, 1, 1, 1, 5, 1, 1, 2, 2, 1, 3, 1, 4, 2, 1, 3, 2, 2, 3, 1, 1, 1, 1, 3, 1, 2, 1, 3, 1, 1, 1, 1, 1, 1, 1, 2, 1, 5, 1, 1, 2, 1, 3, 1, 2, 2, 5, 1, 2, 4, 4, 1, 3, 4, 1, 3, 2, 2, 1, 1, 2, 4, 2, 5, 1, 2, 4, 2, 1, 1, 5, 1, 2, 1, 2, 2, 4, 1, 3, 2, 1, 1, 1, 2, 1, 5, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 3, 2, 1, 2, 1, 1, 4, 1, 1, 2, 2, 4, 1, 2, 1, 4, 2, 1, 2, 2, 1, 1, 1, 4, 1, 2, 1, 5, 2, 5, 4, 1, 1, 4, 2, 5, 2, 2, 2, 1, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1, 2, 5, 1, 2, 3, 3, 4, 1, 1, 1, 2, 2, 1, 1, 1, 2, 2, 3, 3, 5, 3, 1, 1, 1, 5, 1, 1, 2, 1, 4, 4, 5, 1, 1, 5, 1, 1, 1, 1, 2, 1, 2, 1, 5, 1, 1, 1, 1, 1, 1, 2, 1, 3, 2, 1, 4, 1, 4, 1, 1, 1, 1, 3, 1, 1, 4, 1, 1, 2, 3, 3, 2, 1, 2, 1, 1, 4, 1, 4, 5, 2, 3, 2, 3, 2, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 3, 4, 4, 5, 2, 1, 1, 1, 3, 1, 3, 2, 3, 1, 1, 3, 2, 3, 1, 4, 1, 1, 2, 4, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 3, 2, 1, 1, 1, 2, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 2, 4, 2, 3, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 3, 2, 5, 1, 2, 1, 3, 1, 4, 4, 2, 1, 3, 5, 1, 3, 1, 1, 1, 1, 1, 2, 1, 3, 1, 1, 2, 1, 1, 2, 4, 2, 1, 2, 1, 2, 1, 1, 4, 3, 4, 5, 1, 1, 2, 2, 1, 3, 1, 1, 3, 1, 2, 3, 4, 2, 1, 4, 1, 3, 2, 1, 2, 4, 4, 1, 1, 2, 1, 4, 2, 1, 2, 1, 3, 1, 1, 1, 1, 3, 3, 2, 1, 1, 3, 1, 1, 2, 2, 1, 2, 1, 4, 5, 2, 1, 2, 1, 1, 1, 2, 1, 2, 2, 1, 1, 2, 2, 4, 1, 2, 1, 2, 2, 1, 1, 1, 3, 1, 1, 3, 3, 3, 2, 1, 1, 2, 1, 3, 2, 1, 2, 5, 1, 2, 5, 2, 1, 3, 2, 2, 2, 1, 4, 1, 2, 2, 2, 1, 4, 1, 2, 1, 3, 2, 1, 3, 1, 4, 1, 2, 2, 3, 2, 4, 1, 2, 2, 1, 1, 2, 4, 1, 1, 2, 2, 2, 1, 2, 1, 1, 2, 1, 3, 2, 1, 1, 2, 1, 1, 1, 1, 2, 2, 2, 1, 2, 4, 2, 2, 1, 2, 3, 3, 1, 2, 1, 1, 3, 3, 1, 3, 3, 1, 2, 3, 1, 1, 1, 1, 3, 2, 4, 1, 1, 2, 1, 1, 4, 3, 1, 1, 1, 1, 4, 3, 1, 2, 1, 3, 1, 1, 1, 1, 1, 3, 1, 2, 2, 3, 1, 1, 1, 2, 1, 3, 3, 3, 1, 3, 3, 1, 2, 1, 1, 1, 1, 1, 5, 4, 5, 3, 4, 1, 1, 1, 3, 1, 4, 1, 2, 1, 1, 1, 1, 1, 4, 1, 1, 3, 2, 1, 2, 2, 1, 1, 4, 1, 5, 2, 1, 5, 5, 2, 1, 3, 3, 2, 1, 3, 4, 3, 1, 4, 1, 4, 3, 1, 4, 5, 1, 2, 5, 5, 1, 3, 2, 1, 1, 1, 1, 1, 2, 2, 1, 5, 1, 2, 3, 2, 3, 2, 1, 5, 2, 5, 1, 1, 2, 1, 2, 1, 1, 3, 1, 1, 1, 3, 4, 2, 3, 1, 2, 3, 3, 3, 3, 1, 1, 1, 3, 2, 1, 4, 3, 1, 1, 1, 1, 3, 2, 1, 2, 2, 5, 1, 1, 3, 1, 1, 4, 1, 3, 1, 1, 1, 1, 1, 4, 1, 5, 2, 1, 4, 1, 2, 1, 3, 2, 1, 5, 2, 4, 1, 1, 1, 3, 2, 1, 1, 1, 2, 3, 4, 1, 2, 3, 3, 1, 1, 2, 2, 2, 2, 4, 5, 2, 2, 3, 1, 3, 3, 2, 1, 2, 1, 1, 2, 1, 2, 3, 2, 2, 2, 2, 4, 3, 1, 1, 2, 1, 3, 3, 2, 1, 3, 1, 4, 2, 3, 3, 3, 2, 5, 2, 1, 4, 3, 1, 1, 1, 1, 1, 2, 3, 3, 1, 3, 2, 1, 1, 1, 1, 2, 2, 1, 2, 3, 3, 1, 1, 2, 1, 3, 2, 2, 4, 4, 2, 1, 1, 2, 1, 1, 4, 3, 3, 4, 1, 1, 3, 4, 1, 3, 2, 1, 1, 4, 1, 5, 1, 1, 5, 3, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 2, 2, 4, 4, 1, 5, 3, 1, 4, 5, 4, 2, 3, 1, 1, 1, 1, 4, 1, 1, 1, 2, 3, 4, 1, 2, 1, 4, 2, 1, 1, 2, 4, 1, 1, 2, 1, 2, 1, 1, 2, 2, 4, 1, 3, 1, 1, 2, 1, 1, 2, 1, 1, 3, 1, 1, 1, 1, 1, 2, 1, 2, 4, 4, 2, 1, 4, 1, 2, 1, 1, 1, 2, 2, 3, 1, 1, 1, 1, 3, 2, 3, 1, 3, 4, 5, 3, 1, 4, 3, 1, 1, 1, 1, 4, 1, 2, 1, 3, 3, 1, 2, 3, 3, 1, 2, 1, 1, 1, 4, 2, 1, 2, 5, 2, 3, 1, 2, 1, 1, 1, 2, 1, 1, 3, 3, 1, 1, 3, 1, 3, 2, 2, 1, 3, 1, 3, 3, 1, 4, 1, 3, 1, 3, 3, 1, 4, 2, 1, 1, 2, 3, 3, 3, 1, 3, 1, 1, 2, 1, 1, 4, 1, 2, 1, 5, 2, 1, 1, 1, 1, 3, 1, 1, 3, 2, 1, 1, 1, 1, 2, 2, 1, 1, 3, 1, 1, 2, 1, 1, 3, 4, 2, 1, 1, 1, 1, 1, 1, 1, 2, 4, 2, 2, 3, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 3, 2, 2, 2, 2, 1, 1, 2, 3, 1, 2, 4, 1, 1, 2, 1, 1, 2, 2, 4, 1, 3, 1, 1, 1, 1, 1, 2, 4, 2, 1, 1, 2, 1, 2, 3, 1, 1, 2, 1, 2, 4, 1, 1, 2, 3, 5, 1, 1, 1, 2, 5, 5, 2, 1, 3, 3, 2, 4, 1, 2, 5, 2, 3, 2, 5, 4, 2, 3, 2, 3, 1, 3, 4, 3, 1, 1, 1, 2, 1, 1, 1, 4, 3, 5, 1, 5, 1, 4, 2, 5, 1, 1, 2, 1, 1, 3, 3, 1, 1, 1, 1, 3, 1, 2, 2, 3, 5, 4, 4, 2, 5, 1, 2, 1, 1, 1, 2, 1, 4, 4, 1, 1, 3, 4, 2, 2, 3, 2, 2, 3, 2, 1, 2, 2, 1, 1, 2, 3, 2, 1, 5, 2, 1, 2, 2, 1, 2, 2, 2, 2, 1, 1, 1, 3, 1, 2, 1, 2, 2, 4, 2, 1, 1, 1, 2, 2, 1, 2, 4, 3, 1, 3, 3, 1, 4, 2, 1, 2, 3, 2, 5, 1, 2, 1, 2, 2, 4, 3, 3, 2, 1, 1, 2, 2, 4, 1, 3, 1, 4, 1, 1, 1, 1, 5, 4, 2, 1, 1, 5, 2, 1, 2, 1, 4, 1, 2, 1, 5, 1, 3, 3, 1, 1, 1, 3, 2, 3, 1, 1, 3, 3, 2, 1, 1, 1, 1, 3, 1, 4, 1, 2, 5, 1, 1, 1, 4, 1, 1, 1, 1, 2, 1, 1, 1, 4, 1, 3, 4, 3, 4, 1, 3, 1, 4, 2, 2, 1, 3, 1, 2, 1, 4, 3, 1, 3, 1, 2, 1, 1, 1, 1, 4, 3, 3, 5, 1, 1, 1, 2, 2, 1, 4, 2, 3, 1, 1, 3, 2, 1, 2, 1, 1, 1, 4, 2, 4, 1, 1, 1, 1, 2, 2, 3, 2, 1, 1, 1, 2, 4, 1, 3, 3, 1, 3, 1, 1, 2, 1, 1, 2, 1, 1, 3, 4, 2, 2, 3, 1, 1, 2, 2, 1, 4, 5, 1, 5, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 3, 1, 2, 1, 1, 2, 1, 2, 1, 3, 3, 3, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 4, 1, 1, 1, 1, 1, 2, 3, 1, 2, 3, 4, 2, 1, 4, 2, 1, 3, 2, 1, 3, 2, 1, 2, 1, 4, 1, 1, 2, 1, 1, 2, 2, 1, 1, 4, 1, 1, 3, 2, 1, 2, 1, 4, 3, 2, 4, 1, 1, 4, 1, 4, 2, 3, 1, 2, 2, 2, 4, 4, 1, 3, 4, 2, 1, 4, 4, 1, 2, 1, 3, 2, 1, 2, 2, 4, 1, 1, 1, 5, 1, 1, 1, 2, 1, 2, 2, 3, 1, 1, 4, 3, 1, 1, 4, 3, 1, 1, 1, 3, 1, 3, 1, 1, 1, 4, 4, 1, 4, 2, 3, 1, 3, 3, 1, 1, 2, 2, 1, 4, 4, 1, 1, 1, 4, 1, 3, 2, 1, 2, 1, 2, 1, 4, 1, 2, 2, 4, 3, 1, 1, 2, 3, 4, 2, 1, 3, 1, 1, 4, 5, 1, 1, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 5, 2, 1, 1, 3, 1, 1, 1, 3, 3, 4, 5, 1, 1, 1, 4, 1, 3, 2, 3, 2, 1, 1, 1, 3, 1, 3, 1, 4, 1, 2, 2, 1, 3, 1, 5, 2, 3, 1, 2, 2, 4, 3, 1, 1, 2, 1, 4, 2, 2, 1, 1, 2, 3, 1, 2, 1, 2, 2, 2, 2, 1, 1, 1, 3, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 3, 1, 1, 4, 1, 3, 3, 1, 1, 1, 2, 1, 3, 2, 2, 2, 3, 2, 1, 1, 1, 1, 1, 2, 3, 5, 2, 2, 2, 1, 1, 1, 2, 3, 5, 2, 3, 1, 1, 4, 2, 1, 4, 1, 1, 3, 1, 4, 2, 1, 1, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 1, 1, 1, 2, 2, 1, 3, 1, 1, 2, 3, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 3, 2, 2, 3, 1, 1, 1, 4, 1, 3, 2, 1, 2, 1, 1, 5, 2, 1, 1, 2, 1, 3, 1, 1, 1, 1, 3, 2, 4, 3, 1, 5, 1, 3, 1, 2, 1, 4, 2, 1, 4, 2, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 3, 3, 1, 1, 2, 1, 1, 4, 2, 1, 1, 1, 2, 2, 1, 4, 4, 1, 1, 1, 1, 1, 1, 2, 5, 1, 1, 2, 1, 2, 2, 3, 2, 1, 2, 1, 5, 1, 1, 3, 1, 4, 1, 2, 1, 2, 4, 1, 1, 2, 2, 1, 1, 1, 1, 2, 1, 2, 4, 3, 1, 1, 3, 1, 2, 2, 1, 3, 1, 2, 1, 1, 1, 1, 1, 1, 4, 2, 2, 1, 2, 2, 1, 1, 3, 1, 2, 3, 1, 1, 2, 2, 1, 2, 4, 4, 1, 2, 2, 1, 2, 1, 1, 1, 1, 3, 4, 1, 2, 3, 2, 1, 3, 1, 4, 1, 2, 2, 3, 3, 1, 2, 1, 2, 1, 1, 2, 2, 1, 4, 1, 3, 2, 1, 2, 3, 3, 2, 1, 1, 2, 2, 1, 2, 5, 4, 5, 3, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 4, 2, 4, 1, 3, 1, 3, 1, 1, 1, 1, 3, 1, 2, 1, 2, 1, 3, 5, 2, 1, 4, 1, 1, 3, 1, 3, 3, 3, 1, 3, 1, 1, 4, 1, 5, 1, 2, 4, 2, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 3, 4, 1, 3, 1, 1, 4, 4, 1, 2, 1, 3, 1, 1, 3, 4, 2, 3, 1, 2, 2, 2, 1, 2, 3, 2, 1, 3, 3, 1, 1, 4, 1, 1, 1, 1, 4, 1, 2, 1, 3, 3, 2, 4, 1, 1, 1, 2, 1, 1, 4, 1, 5, 3, 1, 1, 1, 2, 5, 2, 3, 3, 1, 3, 2, 2, 1, 1, 2, 1, 2, 1, 1, 4, 1, 2, 5, 1, 3, 1, 2, 1, 3, 2, 1, 3, 4, 3, 5, 1, 2, 5, 1, 3, 1, 3, 2, 1, 1, 2, 2, 1, 1, 4, 1, 4, 1, 2, 4, 5, 1, 4, 3, 3, 5, 4, 3, 1, 3, 5, 1, 1, 3, 2, 3, 2, 1, 1, 3, 3, 1, 3, 3, 2, 1, 1, 4, 1, 1, 3, 1, 1, 1, 1, 5, 4, 2, 1, 2, 1, 2, 1, 4, 2, 1, 2, 2, 1, 1, 2, 1, 3, 1, 1, 1, 4, 1, 1, 2, 1, 4, 1, 2, 2, 3, 2, 1, 1, 2, 1, 1, 1, 2, 2, 3, 5, 4, 4, 2, 1, 3, 1, 1, 4, 1, 1, 1, 2, 1, 2, 2, 1, 1, 1, 1, 2, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 3, 1, 3, 2, 2, 1, 3, 2, 1, 2, 3, 2, 3, 1, 2, 1, 1, 4, 1, 3, 2, 1, 1, 2, 2, 1, 1, 5, 3, 1, 1, 4, 1, 1, 1, 2, 2, 1, 1, 1, 2, 1, 4, 1, 1, 4, 1, 2, 2, 4, 2, 1, 1, 3, 1, 1, 1, 1, 1, 2, 4, 5, 1, 4, 2, 3, 4, 2, 1, 3, 4, 1, 2, 2, 1, 1, 3, 2, 1, 2, 1, 2, 1, 1, 2, 3, 2, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 2, 2, 3, 3, 1, 1, 1, 3, 1, 1, 1, 3, 2, 4, 1, 2, 1, 1, 1, 1, 4, 2, 1, 2, 5, 3, 2, 3, 3, 1, 2, 2, 4, 1, 4, 2, 1, 1, 1, 1, 1, 1, 1, 4, 2, 1, 1, 1, 2, 2, 4, 1, 1, 3, 1, 1, 2, 2, 2, 3, 4, 5, 5, 2, 1, 1, 1, 1, 2, 2, 5, 4, 1, 1, 2, 1, 1, 1, 1, 2, 4, 1, 2, 1, 5, 1, 4, 2, 1, 1, 1, 1, 2, 1, 3, 1, 4, 1, 4, 3, 1, 2, 2, 2, 1, 2, 3, 1, 2, 2, 1, 3, 2, 1, 3, 2, 2, 1, 5, 1, 2, 3, 4, 2, 2, 1, 1, 1, 4, 2, 5, 1, 1, 1, 2, 1, 1, 1, 2, 4, 4, 2, 2, 3, 1, 1, 1, 3, 1, 1, 2, 5, 5, 1, 4, 1, 2, 2, 4, 1, 1, 3, 1, 3, 4, 4, 1, 1, 3, 1, 1, 4, 1, 5, 2, 2, 2, 1, 4, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 4, 1, 2, 4, 3, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 2, 3, 1, 4, 2, 2, 1, 1, 5, 4, 1, 5, 3, 1, 3, 3, 3, 1, 2, 1, 1, 1, 5, 1, 1, 2, 2, 1, 1, 1, 3, 3, 2, 1, 1, 1, 1, 2, 4, 2, 1, 1, 1, 1, 5, 2, 2, 1, 1, 2, 3, 4, 2, 2, 1, 1, 1, 1, 1, 1, 2, 1, 4, 2, 1, 1, 1, 4, 4, 2, 1, 1, 1, 1, 3, 4, 3, 2, 1, 2, 2, 2, 1, 1, 1, 4, 1, 1, 2, 1, 1, 1, 3, 2, 1, 1, 1, 4, 1, 3, 2, 1, 3, 5, 1, 4, 3, 3, 1, 4, 2, 5, 1, 4, 2, 4, 1, 2, 1, 1, 2, 2, 2, 4, 4, 2, 2, 1, 5, 2, 4, 1, 1, 1, 1, 2, 1, 1, 1, 4, 1, 1, 1, 1, 1, 3, 3, 2, 3, 1, 2, 1, 3, 3, 3, 1, 3, 1, 2, 1, 2, 1, 1, 1, 3, 5, 1, 1, 2, 1, 2, 1, 1, 4, 1, 2, 1, 3, 1, 1, 1, 1, 2, 1, 4, 1, 1, 1, 1, 1, 1, 4, 2, 3, 1, 1, 1, 1, 1, 5, 2, 3, 1, 1, 2, 1, 2, 4, 1, 1, 1, 1, 1, 4, 1, 4, 2, 1, 1, 1, 1, 2, 3, 1, 1, 3, 3, 1, 1, 2, 4, 2, 5, 3, 5, 3, 2, 1, 1, 1, 5, 2, 3, 1, 1, 2, 1, 1, 1, 2, 5, 1, 1, 1, 2, 1, 3, 2, 2, 4, 2, 3, 3, 2, 1, 1, 1, 3, 1, 1, 1, 2, 1, 1, 3, 4, 2, 1, 2, 1, 3, 1, 2, 1, 2, 2, 1, 1, 3, 2, 3, 1, 1, 4, 2, 1, 2, 2, 1, 3, 1, 3, 1, 1, 2, 2, 2, 3, 1, 2, 2, 2, 1, 2, 2, 3, 2, 1, 1, 2, 1, 1, 1, 5, 1, 1, 4, 1, 1, 3, 1, 5, 1, 3, 3, 1, 1, 1, 3, 5, 4, 3, 1, 5, 2, 3, 3, 1, 2, 5, 4, 1, 2, 2, 1, 1, 3, 1, 2, 3, 1, 2, 1, 1, 1, 2, 3, 1, 1, 2, 3, 3, 1, 4, 5, 1, 2, 1, 2, 1, 1, 3, 1, 2, 1, 1, 1, 1, 1, 1, 1, 4, 3, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 4, 1, 4, 3, 2, 2, 4, 1, 2, 1, 1, 1, 1, 5, 3, 1, 4, 2, 1, 5, 5, 5, 5, 4, 2, 3, 4, 3, 2, 4, 2, 1, 1, 4, 1, 4, 2, 1, 1, 3, 1, 1, 2, 2, 4, 4, 4, 2, 2, 1, 5, 2, 4, 5, 1, 2, 3, 1, 5, 1, 2, 2, 1, 5, 4, 1, 1, 2, 1, 3, 2, 2, 3, 3, 1, 1, 3, 1, 2, 1, 1, 1, 1, 1, 2, 4, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 3, 1, 2, 1, 3, 2, 2, 1, 5, 1, 2, 1, 1, 4, 1, 1, 2, 1, 2, 1, 3, 1, 1, 1, 2, 1, 1, 1, 1, 3, 4, 5, 4, 1, 2, 1, 1, 2, 1, 3, 1, 1, 2, 2, 1, 5, 3, 1, 2, 1, 1, 2, 3, 4, 2, 4, 1, 2, 1, 4, 1, 4, 1, 1, 1, 1, 1, 1, 5, 5, 1, 5, 3, 1, 2, 1, 1, 1, 1, 4, 2, 1, 1, 1, 2, 3, 1, 1, 1, 2, 1, 1, 1, 1, 3, 2, 3, 1, 1, 2, 1, 3, 2, 1, 1, 2, 1, 1, 1, 4, 2, 2, 5, 3, 2, 2, 4, 2, 2, 1, 3, 3, 2, 2, 1, 3, 4, 2, 2, 1, 1, 3, 1, 1, 4, 2, 4, 4, 3, 2, 4, 4, 2, 3, 1, 2, 1, 1, 2, 1, 1, 3, 1, 3, 2, 1, 1, 1, 1, 1, 2, 1, 4, 2, 1, 1, 1, 1, 3, 2, 1, 1, 1, 2, 2, 2, 1, 1, 2, 2, 2, 4, 1, 1, 5, 1, 3, 3, 2, 3, 1, 1, 4, 1, 1, 1, 3, 3, 1, 2, 3, 2, 1, 1, 1, 4, 1, 2, 4, 5, 3, 3, 1, 1, 1, 1, 1, 1, 5, 2, 4, 1, 1, 2, 2, 1, 1, 1, 2, 2, 1, 1, 2, 5, 3, 2, 1, 3, 3, 2, 1, 1, 1, 1, 3, 2, 2, 2, 4, 1, 2, 1, 1, 2, 1, 3, 5, 2, 2, 1, 4, 2, 1, 2, 5, 1, 4, 2, 5, 1, 2, 2, 1, 2, 2, 2, 4, 1, 1, 1, 1, 1, 5, 5, 2, 1, 1, 1, 1, 1, 1, 4, 2, 1, 3, 1, 1, 1, 4, 1, 3, 3, 1, 3, 2, 2, 2, 2, 1, 1, 1, 2, 4, 1, 1, 1, 2, 5, 5, 2, 5, 1, 2, 1, 1, 2, 3, 4, 5, 2, 3, 1, 1, 4, 3, 2, 3, 2, 1, 1, 1, 3, 4, 2, 3, 3, 1, 3, 1, 3, 1, 2, 1, 2, 1, 1, 1, 1, 2, 2, 1, 1, 2, 1, 4, 3, 1, 2, 4, 1, 1, 1, 1, 3, 1, 3, 1, 2, 1, 5, 3, 2, 2, 4, 4, 1, 2, 3, 2, 1, 1, 1, 2, 3, 1, 1, 2, 2, 3, 3, 2, 1, 5, 1, 1, 3, 1, 2, 1, 5, 1, 1, 1, 1, 2, 2, 1, 3, 2, 4, 2, 3, 1, 4, 2, 2, 3, 5, 1, 2, 2, 1, 1, 3, 1, 3, 4, 2, 2, 1, 1, 2, 1, 2, 4, 3, 1, 3, 1, 1, 1, 1, 2, 3, 1, 3, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 2, 3, 1, 5, 1, 2, 1, 1, 1, 4, 5, 1, 1, 1, 4, 1, 3, 3, 3, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 4, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 4, 3, 1, 3, 2, 3, 4, 4, 1, 3, 2, 2, 1, 1, 2, 1, 1, 3, 5, 3, 1, 1, 1, 4, 3, 2, 1, 2, 1, 2, 1, 2, 4, 3, 1, 1, 2, 1, 4, 4, 3, 2, 1, 1, 1, 1, 1, 1, 3, 3, 5, 3, 4, 3, 1, 2, 3, 1, 4, 1, 1, 1, 1, 4, 1, 4, 2, 2, 1, 4, 1, 3, 2, 1, 3, 2, 1, 1, 2, 1, 2, 3, 2, 5, 2, 2, 1, 1, 1, 3, 4, 4, 1, 1, 4, 4, 2, 2, 1, 4, 2, 1, 2, 5, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 3, 1, 2, 1, 3, 3, 4, 4, 2, 1, 1, 1, 4, 1, 3, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 4, 1, 2, 1, 1, 2, 1, 3, 2, 1, 4, 2, 2, 3, 1, 3, 1, 1, 5, 5, 2, 1, 4, 1, 3, 3, 1, 1, 2, 2, 5, 2, 4, 1, 1, 1, 1, 2, 1, 3, 2, 5, 1, 2, 1, 1, 2, 2, 1, 1, 1, 2, 4, 3, 1, 2, 4, 1, 4, 3, 4, 4, 1, 1, 5, 3, 2, 1, 2, 1, 1, 2, 3, 1, 1, 1, 1, 5, 2, 2, 3, 2, 3, 1, 1, 1, 3, 2, 1, 3, 3, 3, 2, 3, 2, 2, 1, 1, 1, 4, 1, 5, 2, 1, 2, 1, 3, 3, 1, 2, 3, 3, 3, 2, 1, 3, 2, 1, 1, 1, 1, 2, 4, 5, 3, 4, 2, 1, 1, 2, 2, 1, 1, 3, 1, 1, 3, 1, 4, 1, 2, 5, 3, 1, 1, 4, 3, 1, 1, 1, 1, 4, 1, 4, 1, 2, 3, 2, 1, 2, 1, 4, 1, 1, 1, 1, 1, 1, 3, 1, 2, 2, 1, 1, 5, 3, 1, 1, 2, 1, 3, 2, 1, 2, 1, 1, 1, 2, 1, 3, 2, 2, 4, 1, 4, 1, 3, 2, 1, 1, 1, 3, 2, 2, 4, 3, 1, 1, 1, 5, 4, 4, 2, 4, 2, 1, 1, 4, 2, 2, 5, 1, 2, 2, 1, 1, 1, 1, 4, 3, 1, 1, 2, 2, 3, 1, 2, 1, 2, 1, 1, 4, 1, 1, 1, 3, 4, 4, 4, 1, 1, 3, 1, 1, 2, 2, 2, 1, 1, 2, 1, 1, 1, 1, 5, 4, 1, 2, 2, 1, 1, 1, 2, 3, 2, 5, 1, 3, 4, 1, 3, 2, 2, 2, 2, 3, 3, 1, 1, 1, 4, 3, 1, 1, 4, 2, 1, 1, 2, 1, 2, 2, 1, 1, 2, 3, 1, 1, 5, 2, 1, 5, 2, 1, 1, 4, 2, 2, 3, 1, 1, 2, 1, 2, 2, 1, 1, 2, 3, 5, 1, 4, 2, 2, 2, 1, 3, 2, 3, 2, 1, 2, 1, 2, 1, 1, 2, 1, 1, 1, 1, 5, 4, 2, 1, 1, 1, 3, 1, 1, 1, 1, 2, 2, 2, 2, 3, 2, 4, 4, 3, 4, 1, 1, 1, 2, 1, 1, 2, 3, 5, 2, 1, 1, 2, 1, 4, 3, 2, 1, 1, 1, 3, 4, 1, 1, 1, 1, 2, 2, 1, 2, 1, 2, 5, 4, 3, 1, 1, 1, 2, 2, 3, 1, 3, 1, 2, 1, 1, 2, 3, 1, 2, 2, 1, 3, 2, 1, 3, 2, 2, 1, 1, 3, 1, 1, 1, 1, 1, 4, 2, 1, 1, 4, 2, 1, 2, 1, 1, 1, 1, 1, 5, 2, 2, 1, 1, 3, 5, 4, 3, 1, 1, 1, 2, 1, 1, 3, 1, 1, 1, 1, 1, 1, 2, 3, 1, 2, 1, 2, 4, 4, 2, 1, 1, 3, 1, 2, 3, 2, 1, 1, 1, 1, 2, 2, 1, 4, 1, 1, 2, 3, 2, 3, 1, 1, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 1, 1, 2, 2, 3, 1, 1, 1, 2, 2, 2, 1, 1, 2, 1, 1, 5, 2, 1, 1, 2, 1, 1, 2, 4, 1, 3, 1, 1, 3, 5, 2, 3, 4, 1, 1, 1, 2, 3, 2, 2, 1, 1, 2, 1, 1, 3, 5, 1, 2, 3, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 2, 4, 1, 1, 3, 4, 3, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 3, 1, 2, 3, 4, 3, 2, 5, 1, 1, 1, 2, 1, 5, 1, 2, 5, 1, 3, 3, 1, 1, 2, 1, 1, 1, 2, 4, 1, 4, 1, 1, 1, 3, 1, 3, 2, 2, 2, 1, 1, 3, 1, 2, 1, 4, 1, 1, 3, 3, 1, 1, 4, 2, 3, 2, 2, 1, 2, 3, 3, 3, 1, 1, 5, 1, 4, 2, 5, 3, 1, 1, 4, 1, 4, 1, 1, 1, 2, 2, 1, 3, 2, 4, 3, 2, 1, 2, 4, 5, 1, 1, 2, 4, 3, 1, 2, 1, 2, 2, 1, 1, 1, 4, 1, 3, 1, 3, 2, 2, 3, 4, 2, 1, 2, 5, 4, 1, 1, 4, 2, 2, 1, 1, 1, 1, 2, 2, 4, 1, 2, 4, 1, 2, 1, 2, 2, 1, 5, 3, 1, 1, 2, 4, 4, 1, 1, 1, 1, 2, 1, 3, 2, 3, 2, 2, 1, 2, 2, 3, 2, 4, 1, 2, 1, 1, 2, 1, 4, 4, 1, 1, 3, 1, 3, 1, 1, 4, 4, 2, 3, 2, 2, 1, 4, 2, 2, 1, 1, 2, 4, 1, 1, 5, 1, 2, 1, 2, 4, 2, 3, 1, 2, 2, 1, 4, 2, 2, 3, 1, 2, 1, 4, 1, 1, 2, 4, 1, 1, 1, 3, 2, 1, 1, 1, 2, 1, 1, 1, 3, 1, 2, 4, 2, 4, 5, 2, 1, 2, 1, 2, 1, 2, 2, 4, 2, 2, 1, 5, 1, 1, 2, 3, 1, 1, 1, 1, 2, 1, 2, 2, 3, 4, 1, 1, 4, 1, 2, 1, 1, 1, 1, 1, 1, 1, 5, 1, 2, 5, 2, 3, 1, 1, 2, 3, 2, 1, 4, 3, 1, 5, 1, 1, 1, 2, 2, 1, 1, 1, 5, 1, 2, 2, 2, 1, 3, 2, 5, 2, 1, 1, 5, 1, 1, 3, 2, 2, 2, 1, 4, 1, 1, 1, 1, 2, 5, 3, 1, 2, 1, 1, 3, 1, 2, 2, 2, 1, 4, 1, 1, 2, 2, 1, 2, 1, 1, 4, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 1, 1, 2, 5, 2, 2, 2, 1, 1, 1, 2, 1, 5, 1, 2, 1, 2, 1, 2, 1, 3, 1, 3, 2, 4, 1, 1, 4, 3, 5, 2, 1, 2, 2, 1, 3, 5, 2, 1, 1, 1, 1, 1, 4, 2, 3, 2, 2, 4, 4, 2, 5, 4, 2, 4, 3, 2, 1, 3, 1, 2, 2, 1, 2, 2, 3, 3, 4, 1, 2, 3, 4, 1, 4, 2, 2, 2, 2, 2, 1, 3, 1, 1, 1, 5, 5, 3, 2, 3, 1, 3, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1, 3, 3, 1, 2, 2, 3, 2, 1, 2, 3, 4, 3, 2, 2, 3, 1, 1, 5, 1, 1, 2, 3, 3, 1, 3, 1, 3, 1, 1, 1, 3, 1, 3, 3, 5, 2, 1, 1, 1, 4, 3, 1, 1, 1, 4, 1, 3, 4, 4, 4, 3, 4, 2, 1, 3, 3, 2, 3, 5, 2, 1, 2, 1, 1, 4, 1, 3, 3, 2, 2, 1, 1, 1, 2, 2, 1, 5, 1, 2, 2, 1, 1, 3, 2, 4, 1, 2, 4, 5, 5, 1, 4, 3, 2, 2, 1, 2, 1, 1, 1, 3, 2, 1, 1, 1, 2, 3, 1, 2, 3, 1, 1, 1, 1, 2, 3, 4, 3, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 5, 2, 2, 3, 1, 1, 2, 2, 2, 3, 2, 1, 1, 1, 3, 2, 1, 1, 4, 2, 2, 1, 1, 4, 4, 4, 1, 1, 3, 3, 1, 3, 1, 1, 1, 1, 1, 5, 2, 1, 1, 5, 2, 1, 2, 2, 5, 2, 5, 1, 2, 2, 1, 1, 1, 2, 4, 5, 2, 1, 1, 3, 2, 3, 2, 2, 2, 2, 2, 2, 3, 2, 3, 3, 1, 1, 1, 3, 5, 1, 1, 3, 1, 1, 2, 1, 1, 3, 1, 2, 1, 3, 4, 3, 1, 5, 1, 3, 1, 3, 1, 3, 3, 4, 2, 2, 2, 4, 1, 5, 5, 2, 1, 1, 1, 2, 1, 3, 2, 2, 3, 1, 1, 1, 1, 2, 2, 1, 5, 1, 1, 1, 2, 1, 2, 1, 3, 3, 3, 3, 1, 2, 1, 2, 2, 1, 4, 1, 1, 2, 1, 3, 1, 1, 2, 2, 1, 1, 2, 1, 3, 2, 1, 3, 2, 2, 2, 4, 3, 3, 1, 1, 3, 4, 1, 1, 3, 1, 1, 2, 2, 3, 3, 2, 2, 1, 1, 1, 1, 2, 1, 2, 2, 1, 2, 3, 2, 1, 2, 1, 1, 3, 1, 2, 2, 1, 2, 1, 4, 1, 3, 2, 1, 4, 3, 3, 2, 1, 4, 5, 2, 2, 2, 2, 3, 3, 3, 2, 2, 1, 1, 1, 3, 1, 2, 2, 4, 1, 2, 2, 4, 1, 1, 1, 2, 4, 1, 2, 1, 1, 4, 5, 3, 1, 5, 3, 1, 1, 5, 2, 2, 1, 1, 1, 4, 3, 1, 3, 1, 3, 2, 2, 1, 2, 2, 3, 1, 1, 1, 1, 1, 4, 1, 1, 2, 1, 1, 1, 1, 2, 2, 1, 1, 4, 4, 1, 1, 4, 1, 2, 1, 3, 3, 3, 1, 1, 1, 1, 3, 2, 3, 3, 3, 1, 1, 1, 1, 3, 1, 2, 5, 3, 5, 3, 3, 5, 1, 4, 5, 3, 2, 1, 1, 2, 2, 2, 2, 3, 1, 1, 1, 1, 2, 5, 1, 2, 3, 1, 1, 2, 2, 1, 2, 3, 1, 5, 1, 2, 2, 3, 2, 1, 1, 2, 1, 2, 1, 4, 2, 1, 1, 3, 1, 3, 1, 2, 2, 1, 3, 2, 1, 1, 5, 4, 4, 2, 5, 3, 5, 1, 4, 3, 1, 3, 2, 3, 1, 3, 4, 1, 1, 4, 2, 1, 1, 3, 3, 2, 2, 3, 2, 1, 2, 1, 4, 3, 3, 3, 1, 2, 4, 1, 4, 2, 1, 1, 5, 2, 2, 3, 1, 1, 1, 1, 3, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 5, 2, 1, 3, 1, 2, 2, 1, 3, 3, 1, 1, 1, 3, 5, 1, 3, 1, 2, 1, 2, 5, 2, 2, 2, 2, 5, 3, 5, 5, 1, 1, 1, 5, 2, 1, 2, 1, 3, 2, 1, 1, 1, 1, 3, 1, 1, 2, 1, 3, 1, 1, 1, 1, 3, 1, 2, 2, 1, 2, 1, 1, 1, 3, 5, 1, 2, 3, 1, 1, 1, 1, 1, 1, 2, 5, 2, 1, 1, 1, 2, 4, 2, 1, 2, 2, 5, 1, 1, 3, 3, 4, 3, 1, 1, 1, 1, 1, 2, 1, 1, 1, 4, 4, 1, 2, 3, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 2, 2, 1, 1, 2, 1, 4, 1, 1, 1, 4, 2, 5, 1, 2, 5, 1, 3, 4, 1, 3, 4, 1, 4, 2, 2, 2, 1, 1, 2, 1, 1, 3, 1, 1, 3, 5, 4, 5, 3, 1, 2, 3, 1, 1, 2, 1, 2, 1, 1, 1, 4, 3, 1, 2, 5, 1, 2, 2, 4, 2, 2, 4, 1, 2, 2, 3, 4, 1, 1, 3, 4, 4, 1, 1, 1, 2, 4, 3, 2, 1, 2, 1, 4, 3, 3, 2, 1, 3, 3, 1, 3, 1, 1, 1, 1, 1, 3, 1, 2, 3, 1, 3, 1, 1, 3, 1, 1, 2, 4, 1, 4, 1, 4, 1, 2, 1, 2, 1, 1, 4, 2, 2, 3, 1, 5, 1, 4, 1, 1, 1, 4, 3, 5, 1, 1, 3, 1, 2, 5, 1, 1, 3, 1, 2, 2, 1, 1, 1, 3, 3, 3, 3, 3, 1, 5, 1, 1, 3, 1, 2, 3, 3, 4, 3, 1, 1, 2, 1, 1, 1, 5, 1, 1, 1, 2, 2, 1, 1, 1, 1, 5, 1, 1, 4, 1, 2, 2, 2, 3, 2, 4, 1, 3, 2, 2, 3, 2, 1, 3, 4, 2, 4, 5, 4, 2, 1, 2, 2, 1, 3, 1, 1, 5, 4, 3, 4, 3, 3, 3, 2, 1, 2, 2, 1, 1, 1, 1, 2, 1, 5, 4, 2, 5, 3, 4, 4, 1, 1, 2, 1, 4, 1, 5, 3, 3, 1, 2, 3, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 2, 1, 3, 3, 2, 4, 3, 1, 3, 1, 3, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1, 3, 2, 2, 1, 2, 2, 1, 4, 1, 3, 3, 1, 1, 1, 2, 1, 1, 2, 2, 2, 3, 1, 2, 1, 1, 1, 2, 4, 1, 2, 2, 1, 1, 3, 1, 5, 4, 3, 1, 2, 1, 1, 2, 2, 1, 3, 3, 3, 1, 1, 1, 3, 2, 1, 3, 4, 2, 4, 2, 2, 3, 1, 2, 2, 4, 5, 1, 5, 3, 3, 1, 1, 2, 2, 1, 1, 4, 2, 5, 2, 1, 1, 3, 1, 4, 5, 1, 3, 2, 3, 1, 2, 2, 1, 4, 2, 2, 4, 5, 1, 1, 2, 2, 2, 1, 5, 3, 2, 1, 4, 4, 3, 2, 3, 1, 1, 4, 4, 4, 4, 1, 3, 1, 4, 3, 1, 1, 4, 5, 2, 4, 3, 5, 4, 1, 2, 4, 4, 1, 2, 2, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 4, 3, 2, 1, 1, 3, 3, 2, 1, 4, 2, 1, 1, 2, 4, 3, 2, 5, 1, 5, 4, 4, 2, 3, 2, 4, 1, 4, 1, 2, 5, 3, 1, 3, 1, 1, 2, 2, 4, 1, 2, 3, 1, 1, 1, 3, 1, 2, 1, 1, 1, 5, 2, 1, 2, 1, 3, 4, 1, 2, 1, 3, 1, 1, 1, 2, 1, 1, 2, 1, 2, 1, 4, 1, 3, 2, 1, 3, 1, 3, 4, 1, 2, 4, 2, 5, 1, 1, 2, 1, 2, 1, 5, 1, 1, 1, 2, 1, 2, 1, 1, 3, 5, 1, 1, 1, 1, 1, 1, 1, 2, 5, 4, 2, 2, 4, 3, 1, 1, 3, 1, 4, 2, 2, 2, 3, 2, 1, 1, 1, 4, 1, 3, 1, 4, 1, 5, 1, 5, 1, 3, 1, 2, 4, 2, 3, 2, 2, 1, 2, 1, 2, 2, 1, 4, 2, 2, 2, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 4, 1, 2, 2, 4, 4, 1, 1, 1, 4, 4, 3, 3, 3, 3, 1, 3, 4, 1, 2, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 2, 2, 1, 1, 3, 2, 1, 1, 1, 2, 1, 3, 2, 1, 3, 2, 2, 2, 1, 1, 5, 1, 1, 1, 1, 5, 1, 2, 4, 2, 3, 1, 5, 1, 1, 1, 5, 4, 1, 2, 2, 1, 1, 1, 4, 2, 2, 2, 2, 1, 3, 1, 2, 5, 1, 2, 1, 1, 3, 2, 2, 2, 3, 1, 2, 1, 2, 4, 1, 3, 2, 1, 2, 1, 1, 4, 1, 1, 3, 2, 2, 3, 1, 1, 2, 1, 2, 4, 1, 3, 1, 3, 2, 1, 1, 1, 1, 1, 2, 2, 4, 3, 3, 1, 2, 1, 4, 1, 1, 2, 2, 1, 1, 5, 1, 1, 2, 4, 1, 2, 4, 3, 1, 2, 2, 5, 1, 2, 4, 4, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 4, 1, 1, 1, 1, 3, 2, 1, 2, 3, 5, 1, 1, 1, 4, 2, 1, 2, 2, 2, 1, 2, 2, 1, 4, 1, 4, 1, 1, 1, 3, 1, 1, 3, 1, 2, 1, 3, 3, 3, 3, 5, 1, 1, 1, 1, 5, 2, 3, 2, 3, 1, 1, 2, 2, 4, 2, 3, 1, 2, 2, 1, 2, 3, 1, 1, 2, 1, 1, 1, 4, 1, 2, 2, 3, 2, 1, 1, 1, 4, 2, 1, 4, 3, 2, 5, 1, 1, 3, 2, 1, 1, 1, 2, 1, 3, 3, 2, 1, 1, 2, 3, 1, 2, 3, 1, 1, 3, 1, 2, 4, 1, 4, 3, 2, 5, 3, 4, 1, 3, 3, 2, 4, 1, 2, 3, 3, 2, 3, 4, 1, 1, 1, 2, 1, 1, 1, 5, 1, 1, 2, 1, 2, 1, 2, 2, 1, 3, 4, 1, 1, 2, 2, 5, 3, 2, 3, 4, 1, 1, 2, 4, 2, 4, 4, 4, 4, 2, 1, 1, 1, 1, 3, 4, 2, 5, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 3, 4, 1, 1, 1, 2, 1, 3, 2, 1, 2, 1, 4, 1, 1, 1, 2, 3, 1, 3, 1, 2, 2, 4, 1, 1, 4, 1, 2, 1, 5, 2, 2, 2, 4, 1, 1, 1, 4, 1, 2, 1, 1, 1, 1, 4, 2, 2, 2, 3, 3, 3, 3, 3, 2, 2, 1, 1, 2, 2, 2, 2, 2, 3, 1, 1, 1, 1, 2, 2, 4, 1, 1, 1, 4, 1, 3, 1, 2, 1, 1, 1, 1, 1, 4, 1, 2, 1, 2, 2, 2, 4, 1, 4, 1, 1, 3, 2, 1, 3, 2, 4, 4, 2, 1, 2, 2, 1, 1, 2, 4, 1, 3, 1, 1, 1, 5, 3, 1, 5, 2, 1, 1, 1, 4, 1, 1, 1, 2, 4, 2, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 4, 5, 1, 2, 1, 1, 2, 3, 1, 2, 3, 1, 3, 2, 1, 2, 1, 5, 1, 2, 2, 1, 1, 2, 1, 1, 3, 1, 2, 1, 2, 2, 1, 1, 3, 5, 1, 1, 1, 3, 3, 1, 4, 3, 1, 3, 3, 3, 1, 3, 1, 2, 1, 4, 5, 4, 1, 1, 3, 1, 2, 2, 1, 3, 1, 1, 1, 2, 2, 2, 3, 2, 1, 1, 1, 5, 2, 3, 2, 1, 1, 2, 1, 1, 3, 5, 1, 1, 1, 2, 1, 5, 2, 4, 1, 2, 3, 1, 2, 2, 2, 3, 1, 2, 1, 1, 2, 1, 1, 5, 1, 2, 1, 2, 1, 1, 2, 2, 1, 4, 1, 1, 1, 2, 1, 3, 2, 4, 1, 5, 1, 2, 1, 1, 5, 4, 2, 1, 1, 2, 2, 2, 4, 1, 2, 2, 1, 5, 1, 1, 2, 1, 5, 1, 3, 4, 2, 2, 1, 2, 4, 2, 4, 1, 2, 4, 1, 1, 5, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 4, 1, 3, 2, 1, 1, 1, 1, 5, 2, 1, 3, 1, 1, 3, 3, 1, 3, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 2, 1, 2, 1, 3, 1, 3, 3, 2, 3, 1, 3, 4, 3, 3, 1, 2, 2, 1, 1, 2, 3, 1, 1, 1, 4, 3, 1, 1, 1, 3, 2, 1, 3, 4, 4, 1, 2, 1, 5, 1, 1, 2, 1, 3, 3, 2, 1, 3, 3, 2, 2, 5, 5, 3, 3, 1, 1, 1, 1, 2, 2, 1, 4, 1, 1, 2, 4, 2, 1, 1, 2, 2, 3, 3, 3, 1, 4, 1, 5, 3, 1, 4, 1, 1, 4, 2, 2, 4, 1, 4, 1, 1, 1, 5, 2, 3, 1, 2, 1, 2, 4, 2, 2, 2, 3, 1, 2, 4, 4, 1, 1, 1, 1, 2, 2, 3, 2, 1, 1, 1, 1, 1, 5, 1, 5, 2, 3, 2, 2, 1, 1, 4, 1, 4, 1, 1, 5, 1, 3, 3, 2, 1, 2, 2, 2, 1, 2, 1, 1, 2, 1, 5, 1, 4, 1, 1, 1, 2, 3, 1, 2, 4, 2, 4, 3, 1, 2, 1, 2, 5, 1, 3, 3, 2, 2, 1, 1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 4, 1, 3, 1, 1, 2, 5, 1, 2, 2, 4, 2, 4, 1, 1, 2, 3, 1, 3, 2, 1, 1, 1, 3, 4, 1, 3, 1, 2, 4, 2, 3, 3, 1, 3, 2, 3, 4, 1, 1, 2, 4, 3, 2, 1, 2, 2, 3, 1, 1, 1, 3, 2, 2, 1, 3, 1, 2, 3, 4, 1, 2, 1, 3, 1, 4, 3, 1, 1, 4, 2, 2, 2, 3, 1, 2, 1, 2, 1, 1, 2, 5, 4, 2, 2, 1, 1, 1, 1, 2, 2, 1, 2, 1, 4, 2, 2, 3, 1, 2, 3, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 2, 1, 2, 1, 3, 3, 2, 3, 2, 4, 3, 2, 1, 1, 1, 2, 2, 1, 1, 3, 3, 2, 1, 1, 3, 1, 4, 1, 5, 1, 3, 5, 1, 2, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 2, 4, 5, 1, 1, 2, 5, 1, 1, 1, 1, 1, 1, 2, 1, 3, 1, 2, 4, 1, 1, 1, 2, 1, 1, 1, 5, 2, 1, 1, 1, 3, 1, 3, 5, 1, 3, 1, 3, 1, 2, 3, 1, 1, 2, 1, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 1, 1, 3, 2, 3, 1, 1, 1, 2, 1, 5, 2, 1, 4, 1, 1, 2, 1, 2, 2, 1, 1, 2, 3, 1, 2, 1, 3, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 4, 2, 1, 3, 3, 2, 1, 1, 2, 1, 1, 1, 1, 3, 1, 1, 1, 3, 1, 2, 1, 1, 1, 2, 1, 1, 4, 3, 1, 3, 1, 2, 1, 4, 2, 1, 4, 3, 1, 1, 1, 2, 1, 1, 1, 3, 1, 1, 4, 4, 3, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 2, 2, 1, 2, 2, 2, 4, 1, 2, 1, 5, 2, 1, 2, 1, 1, 5, 1, 3, 1, 1, 1, 1, 5, 1, 1, 1, 1, 3, 4, 4, 1, 1, 1, 1, 3, 5, 5, 4, 2, 1, 1, 3, 3, 1, 1, 1, 1, 2, 1, 5, 1, 2, 2, 1, 2, 1, 2, 1, 4, 1, 2, 4, 2, 2, 3, 1, 4, 1, 1, 2, 1, 2, 1, 3, 2, 1, 2, 1, 1, 4, 4, 3, 3, 1, 1, 2, 2, 4, 3, 2, 1, 2, 2, 2, 5, 2, 1, 1, 5, 1, 3, 5, 1, 1, 1, 1, 4, 4, 1, 1, 1, 2, 1, 2, 1, 1, 4, 3, 2, 2, 1, 1, 1, 1, 1, 2, 3, 1, 3, 3, 4, 4, 1, 3, 1, 4, 1, 5, 1, 3, 4, 1, 1, 2, 2, 1, 2, 5, 5, 1, 1, 1, 1, 5, 2, 1, 1, 2, 1, 1, 2, 1, 3, 3, 2, 2, 3, 1, 2, 3, 2, 1, 2, 1, 5, 1, 2, 3, 1, 1, 1, 1, 5, 2, 1, 1, 2, 1, 1, 3, 4, 1, 3, 3, 1, 1, 3, 4, 1, 3, 4, 1, 2, 1, 1, 3, 2, 2, 2, 2, 5, 2, 1, 3, 3, 2, 3, 2, 4, 4, 2, 1, 3, 1, 5, 2, 1, 3, 1, 2, 2, 4, 2, 1, 2, 4, 5, 1, 3, 1, 1, 3, 4, 1, 2, 1, 5, 3, 4, 1, 1, 2, 2, 1, 1, 1, 5, 1, 1, 3, 2, 1, 2, 1, 1, 3, 1, 2, 4, 2, 1, 2, 3, 1, 1, 2, 5, 1, 1, 1, 1, 3, 2, 5, 3, 3, 1, 1, 1, 4, 4, 5, 1, 1, 2, 1, 2, 3, 3, 2, 1, 1, 1, 1, 1, 1, 5, 2, 4, 1, 2, 4, 5, 2, 1, 1, 1, 1, 1, 3, 1, 2, 4, 1, 1, 1, 2, 1, 2, 2, 2, 3, 1, 1, 2, 2, 4, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 5, 1, 3, 1, 1, 1, 2, 1, 1, 3, 1, 1, 4, 1, 3, 2, 3, 1, 1, 3, 1, 1, 2, 4, 1, 2, 3, 2, 3, 1, 3, 1, 1, 1, 1, 1, 1, 2, 2, 4, 1, 1, 1, 2, 1, 1, 3, 4, 3, 2, 2, 2, 2, 1, 2, 2, 1, 1, 2, 3, 1, 5, 4, 2, 1, 3, 1, 1, 1, 2, 3, 2, 4, 1, 1, 4, 1, 1, 3, 1, 1, 4, 4, 1, 2, 1, 4, 5, 2, 4, 1, 2, 4, 3, 2, 3, 4, 2, 3, 1, 1, 1, 1, 2, 3, 3, 1, 1, 5, 1, 4, 4, 1, 2, 2, 2, 1, 1, 2, 1, 3, 1, 1, 5, 1, 2, 1, 3, 3, 4, 1, 1, 3, 2, 4, 3, 1, 2, 1, 5, 1, 1, 2, 3, 4, 3, 1, 2, 2, 1, 1, 3, 3, 1, 2, 1, 1, 5, 1, 4, 4, 1, 2, 2, 1, 2, 1, 1, 1, 2, 4, 2, 4, 1, 4, 3, 3, 1, 1, 2, 1, 2, 1, 3, 3, 4, 2, 1, 1, 3, 1, 3, 2, 2, 1, 2, 1, 5, 2, 5, 2, 2, 2, 4, 1, 2, 3, 3, 1, 1, 3, 1, 3, 1, 1, 2, 1, 3, 1, 2, 3, 1, 2, 1, 3, 1, 3, 1, 4, 2, 1, 4, 2, 4, 2, 3, 5, 3, 3, 4, 2, 1, 3, 4, 3, 4, 4, 2, 1, 3, 1, 2, 1, 1, 3, 1, 4, 1, 5, 1, 2, 4, 3, 4, 3, 3, 1, 5, 2, 1, 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 3, 1, 2, 1, 1, 5, 3, 2, 2, 1, 1, 2, 3, 1, 1, 1, 1, 2, 3, 5, 1, 3, 3, 2, 2, 1, 1, 1, 2, 1, 4, 5, 1, 2, 3, 2, 1, 1, 1, 4, 1, 3, 2, 2, 1, 1, 2, 2, 1, 1, 1, 3, 2, 1, 1, 4, 1, 1, 2, 1, 2, 3, 1, 3, 1, 3, 2, 4, 4, 4, 2, 3, 4, 1, 4, 1, 1, 2, 2, 5, 2, 3, 1, 3, 1, 1, 4, 2, 2, 1, 3, 2, 4, 1, 1, 1, 2, 4, 2, 3, 3, 4, 1, 1, 3, 1, 3, 2, 1, 4, 1, 5, 1, 2, 4, 2, 1, 1, 2, 1, 5, 3, 1, 1, 2, 1, 1, 1, 1, 1, 2, 2, 3, 2, 1, 1, 2, 2, 4, 1, 3, 1, 1, 3, 2, 4, 1, 1, 1, 1, 1, 2, 3, 2, 1, 2, 1, 3, 3, 2, 1, 1, 3, 2, 2, 3, 4, 1, 4, 1, 2, 5, 1, 2, 1, 5, 2, 2, 4, 1, 2, 1, 1, 5, 2, 1, 5, 2, 1, 1, 2, 1, 2, 1, 1, 1, 4, 4, 2, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 3, 1, 3, 1, 2, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 2, 1, 1, 5, 4, 1, 1, 1, 1, 3, 3, 3, 4, 3, 2, 1, 2, 2, 2, 2, 1, 1, 1, 2, 1, 2, 1, 2, 1, 4, 1, 2, 2, 1, 4, 1, 1, 2, 2, 4, 4, 1, 2, 1, 1, 2, 2, 1, 1, 1, 4, 1, 2, 1, 1, 3, 2, 1, 1, 1, 2, 3, 1, 4, 2, 3, 1, 1, 2, 2, 1, 2, 1, 2, 2, 1, 1, 1, 1, 2, 3, 3, 1, 1, 3, 3, 1, 1, 2, 3, 3, 4, 1, 1, 3, 4, 1, 2, 4, 3, 3, 1, 1, 3, 3, 1, 1, 5, 3, 1, 1, 1, 4, 1, 1, 1, 4, 1, 1, 5, 5, 2, 1, 3, 1, 1, 2, 2, 1, 1, 4, 4, 1, 5, 1, 2, 3, 2, 1, 1, 3, 1, 1, 1, 3, 2, 1, 2, 4, 1, 4, 1, 2, 3, 3, 3, 4, 2, 2, 1, 1, 2, 1, 1, 1, 3, 2, 2, 3, 2, 3, 1, 1, 1, 2, 1, 1, 3, 1, 2, 1, 4, 5, 2, 3, 4, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3, 3, 1, 1, 4, 4, 2, 3, 4, 2, 2, 2, 1, 2, 2, 1, 1, 2, 3, 1, 2, 1, 3, 1, 2, 1, 2, 1, 3, 1, 2, 1, 1, 2, 3, 1, 3, 2, 2, 1, 3, 2, 1, 4, 2, 1, 3, 1, 2, 1, 1, 2, 3, 2, 1, 1, 2, 3, 4, 5, 3, 2, 3, 2, 2, 4, 1, 3, 3, 1, 1, 2, 1, 1, 2, 2, 1, 3, 2, 1, 1, 1, 1, 3, 2, 1, 4, 3, 1, 4, 1, 1, 4, 3, 4, 1, 4, 4, 5, 1, 2, 3, 1, 3, 1, 4, 3, 1, 1, 1, 2, 5, 2, 2, 2, 1, 1, 2, 2, 4, 1, 1, 1, 1, 1, 4, 1, 3, 4, 1, 2, 5, 1, 2, 3, 1, 4, 3, 3, 1, 2, 2, 1, 2, 1, 3, 1, 2, 2, 2, 2, 1, 3, 2, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 2, 2, 2, 1, 2, 3, 1, 4, 1, 3, 1, 1, 3, 4, 1, 3, 1, 5, 1, 2, 1, 3, 1, 1, 1, 2, 1, 1, 1, 1, 3, 1, 2, 3, 1, 1, 2, 1, 3, 1, 1, 3, 2, 1, 1, 1, 4, 1, 1, 1, 5, 2, 2, 2, 4, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 2, 1, 3, 2, 1, 2, 1, 1, 1, 2, 1, 1, 2, 2, 1, 1, 2, 1, 2, 1, 5, 3, 1, 1, 1, 1, 4, 1, 3, 2, 1, 1, 2, 2, 1, 1, 4, 1, 3, 5, 1, 5, 3, 1, 1, 1, 1, 2, 3, 1, 1, 4, 1, 2, 1, 1, 1, 1, 4, 2, 2, 1, 2, 1, 1, 1, 1, 2, 3, 2, 3, 3, 2, 1, 2, 1, 3, 3, 4, 1, 2, 1, 4, 2, 2, 1, 1, 3, 3, 4, 2, 1, 1, 3, 2, 1, 1, 2, 2, 1, 1, 1, 4, 1, 3, 1, 1, 4, 2, 2, 1, 2, 3, 2, 1, 1, 3, 1, 1, 2, 3, 1, 1, 4, 1, 1, 3, 1, 1, 5, 1, 5, 1, 2, 4, 1, 2, 1, 5, 1, 2, 3, 3, 1, 1, 4, 1, 1, 2, 1, 1, 1, 1, 4, 2, 1, 5, 2, 1, 1, 3, 2, 1, 1, 3, 1, 1, 1, 1, 1, 4, 1, 3, 2, 1, 1, 2, 3, 3, 5, 5, 1, 1, 3, 2, 3, 2, 2, 2, 3, 2, 4, 1, 1, 2, 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1, 4, 5, 1, 2, 3, 1, 2, 4, 1, 2, 1, 1, 2, 1, 3, 1, 1, 2, 2, 1, 4, 1, 4, 2, 2, 2, 1, 4, 1, 2, 4, 1, 1, 3, 1, 4, 1, 4, 2, 3, 1, 1, 2, 5, 1, 1, 1, 1, 1, 2, 3, 1, 1, 2, 3, 4, 1, 1, 1, 4, 4, 1, 2, 2, 5, 3, 1, 5, 2, 2, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 3, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 3, 3, 1, 3, 3, 5, 5, 3, 2, 2, 2, 1, 5, 1, 1, 2, 1, 2, 4, 2, 5, 2, 2, 3, 1, 1, 5, 2, 2, 2, 1, 1, 2, 1, 3, 2, 4, 4, 4, 2, 2, 1, 2, 1, 1, 4, 2, 4, 3, 3, 4, 2, 1, 3, 2, 3, 2, 3, 1, 2, 1, 1, 5, 1, 2, 1, 1, 1, 2, 3, 2, 2, 4, 4, 2, 1, 2, 1, 1, 4, 2, 1, 3, 4, 1, 1, 1, 2, 1, 3, 5, 3, 3, 1, 1, 1, 1, 2, 1, 1, 2, 3, 2, 1, 1, 1, 4, 1, 3, 5, 1, 5, 1, 2, 3, 1, 1, 2, 2, 1, 1, 1, 1, 3, 1, 2, 1, 4, 2, 1, 2, 2, 1, 2, 3, 2, 3, 1, 1, 2, 3, 1, 5, 2, 3, 4, 2, 1, 3, 1, 2, 1, 2, 1, 2, 4, 1, 3, 1, 2, 1, 3, 1, 1, 1, 2, 1, 2, 4, 1, 3, 1, 2, 2, 1, 1, 2, 2, 4, 1, 1, 3, 1, 4, 1, 1, 2, 1, 2, 4, 2, 1, 1, 4, 1, 1, 2, 1, 2, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 4, 1, 1, 1, 1, 4, 4, 2, 1, 1, 1, 1, 4, 1, 3, 3, 4, 1, 1, 1, 3, 1, 1, 1, 3, 5, 2, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 2, 1, 3, 2, 1, 2, 1, 4, 1, 2, 1, 4, 3, 1, 2, 1, 4, 5, 4, 1, 1, 3, 1, 2, 2, 1, 3, 1, 1, 1, 2, 2, 2, 3, 2, 1, 1, 1, 5, 2, 3, 2, 1, 1, 2, 1, 1, 3, 5, 1, 1, 1, 2, 1, 5, 2, 4, 1, 2, 3, 1, 2, 2, 2, 3, 1, 2, 1, 1, 2, 1, 1, 5, 1, 2, 1, 2, 1, 1, 2, 2, 1, 4, 1, 1, 1, 2, 1, 3, 2, 4, 1, 5, 1, 2, 1, 1, 5, 4, 2, 1, 1, 2, 2, 2, 4, 1, 2, 2, 1, 5, 1, 1, 2, 1, 5, 1, 3, 4, 2, 2, 1, 2, 4, 2, 4, 1, 2, 4, 1, 1, 5, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 4, 1, 3, 2, 1, 1, 1, 1, 5, 2, 1, 3, 1, 1, 3, 3, 1, 3, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 2, 1, 2, 1, 3, 1, 3, 3, 2, 3, 1, 3, 4, 3, 3, 1, 2, 2, 1, 1, 2, 3, 1, 1, 1, 4, 3, 1, 1, 1, 3, 2, 1, 3, 4, 4, 1, 2, 1, 5, 1, 1, 2, 1, 3, 3, 2, 1, 3, 3, 2, 2, 5, 5, 3, 3, 1, 1, 1, 1, 2, 2, 1, 4, 1, 1, 2, 4, 2, 1, 1, 2, 2, 3, 3, 3, 1, 4, 1, 5, 3, 1, 4, 1, 1, 4, 2, 2, 4, 1, 4, 1, 1, 1, 5, 2, 3, 1, 2, 1, 2, 4, 2, 2, 2, 3, 1, 2, 4, 4, 1, 1, 1, 1, 2, 2, 3, 2, 1, 1, 1, 1, 1, 5, 1, 5, 2, 3, 2, 2, 1, 1, 4, 1, 4, 1, 1, 5, 1, 3, 3, 2, 1, 2, 2, 2, 1, 2, 1, 1, 2, 1, 5, 1, 4, 1, 1, 1, 2, 3, 1, 2, 4, 2, 4, 3, 1, 2, 1, 2, 5, 1, 3, 3, 2, 2, 1, 1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 4, 1, 3, 1, 1, 2, 5, 1, 2, 2, 4, 2, 4, 1, 1, 2, 3, 1, 3, 2, 1, 1, 1, 3, 4, 1, 3, 1, 2, 4, 2, 3, 3, 1, 3, 2, 3, 4, 1, 1, 2, 4, 3, 2, 1, 2, 2, 3, 1, 1, 1, 3, 2, 2, 1, 3, 1, 2, 3, 4, 1, 2, 1, 3, 1, 4, 3, 1, 1, 4, 2, 2, 2, 3, 1, 2, 1, 2, 1, 1, 2, 5, 4, 2, 2, 1, 1, 1, 1, 2, 2, 1, 2, 1, 4, 2, 2, 3, 1, 2, 3, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 2, 1, 2, 1, 3, 3, 2, 3, 2, 4, 3, 2, 1, 1, 1, 2, 2, 1, 1, 3, 3, 2, 1, 1, 3, 1, 4, 1, 5, 1, 3, 5, 1, 2, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 2, 4, 5, 1, 1, 2, 5, 1, 1, 1, 1, 1, 1, 2, 1, 3, 1, 2, 4, 1, 1, 1, 2, 1, 1, 1, 5, 2, 1, 1, 1, 3, 1, 3, 5, 1, 3, 1, 3, 1, 2, 3, 1, 1, 2, 1, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 1, 1, 3, 2, 3, 1, 1, 1, 2, 1, 5, 2, 1, 4, 1, 1, 2, 1, 2, 2, 1, 1, 2, 3, 1, 2, 1, 3, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 4, 2, 1, 3, 3, 2, 1, 1, 2, 1, 1, 1, 1, 3, 1, 1, 1, 3, 1, 2, 1, 1, 1, 2, 1, 1, 4, 3, 1, 3, 1, 2, 1, 4, 2, 1, 4, 3, 1, 1, 1, 2, 1, 1, 1, 3, 1, 1, 4, 4, 3, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 2, 2, 1, 2, 2, 2, 4, 1, 2, 1, 5, 2, 1, 2, 1, 1, 5, 1, 3, 1, 1, 1, 1, 5, 1, 1, 1, 1, 3, 4, 4, 1, 1, 1, 1, 3, 5, 5, 4, 2, 1, 1, 3, 3, 1, 1, 1, 1, 2, 1, 5, 1, 2, 2, 1, 2, 1, 2, 1, 4, 1, 2, 4, 2, 2, 3, 1, 4, 1, 1, 2, 1, 2, 1, 3, 2, 1, 2, 1, 1, 4, 4, 3, 3, 1, 1, 2, 2, 4, 3, 2, 1, 2, 2, 2, 5, 2, 1, 1, 5, 1, 3, 5, 1, 1, 1, 1, 4, 4, 1, 1, 1, 2, 1, 2, 1, 1, 4, 3, 2, 2, 1, 1, 1, 1, 1, 2, 3, 1, 3, 3, 4, 4, 1, 3, 1, 4, 1, 5, 1, 3, 4, 1, 1, 2, 2, 1, 2, 5, 5, 1, 1, 1, 1, 5, 2, 1, 1, 2, 1, 1, 2, 1, 3, 3, 2, 2, 3, 1, 2, 3, 2, 1, 2, 1, 5, 1, 2, 3, 1, 1, 1, 1, 5, 2, 1, 1, 2, 1, 1, 3, 4, 1, 3, 3, 1, 1, 3, 4, 1, 3, 4, 1, 2, 1, 1, 3, 2, 2, 2, 2, 5, 2, 1, 3, 3, 2, 3, 2, 4, 4, 2, 1, 3, 1, 5, 2, 1, 3, 1, 2, 2, 4, 2, 1, 2, 4, 5, 1, 3, 1, 1, 3, 4, 1, 2, 1, 5, 3, 4, 1, 1, 2, 2, 1, 1, 1, 5, 1, 1, 3, 2, 1, 2, 1, 1, 3, 1, 2, 4, 2, 1, 2, 3, 1, 1, 2, 5, 1, 1, 1, 1, 3, 2, 5, 3, 3, 1, 1, 1, 4, 4, 5, 1, 1, 2, 1, 2, 3, 3, 2, 1, 1, 1, 1, 1, 1, 5, 2, 4, 1, 2, 4, 5, 2, 1, 1, 1, 1, 1, 3, 1, 2, 4, 1, 1, 1, 2, 1, 2, 2, 2, 3, 1, 1, 2, 2, 4, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 5, 1, 3, 1, 1, 1, 2, 1, 1, 3, 1, 1, 4, 1, 3, 2, 3, 1, 1, 3, 1, 1, 2, 4, 1, 2, 3, 2, 3, 1, 3, 1, 1, 1, 1, 1, 1, 2, 2, 4, 1, 1, 1, 2, 1, 1, 3, 4, 3, 2, 2, 2, 2, 1, 2, 2, 1, 1, 2, 3, 1, 5, 4, 2, 1, 3, 1, 1, 1, 2, 3, 2, 4, 1, 1, 4, 1, 1, 3, 1, 1, 4, 4, 1, 2, 1, 4, 5, 2, 4, 1, 2, 4, 3, 2, 3, 4, 2, 3, 1, 1, 1, 1, 2, 3, 3, 1, 1, 5, 1, 4, 4, 1, 2, 2, 2, 1, 1, 2, 1, 3, 1, 1, 5, 1, 2, 1, 3, 3, 4, 1, 1, 3, 2, 4, 3, 1, 2, 1, 5, 1, 1, 2, 3, 4, 3, 1, 2, 2, 1, 1, 3, 3, 1, 2, 1, 1, 5, 1, 4, 4, 1, 2, 2, 1, 2, 1, 1, 1, 2, 4, 2, 4, 1, 4, 3, 3, 1, 1, 2, 1, 2, 1, 3, 3, 4, 2, 1, 1, 3, 1, 3, 2, 2, 1, 2, 1, 5, 2, 5, 2, 2, 2, 4, 1, 2, 3, 3, 1, 1, 3, 1, 3, 1, 1, 2, 1, 3, 1, 2, 3, 1, 2, 1, 3, 1, 3, 1, 4, 2, 1, 4, 2, 4, 2, 3, 5, 3, 3, 4, 2, 1, 3, 4, 3, 4, 4, 2, 1, 3, 1, 2, 1, 1, 3, 1, 4, 1, 5, 1, 2, 4, 3, 4, 3, 3, 1, 5, 2, 1, 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 3, 1, 2, 1, 1, 5, 3, 2, 2, 1, 1, 2, 3, 1, 1, 1, 1, 2, 3, 5, 1, 3, 3, 2, 2, 1, 1, 1, 2, 1, 4, 5, 1, 2, 3, 2, 1, 1, 1, 4, 1, 3, 2, 2, 1, 1, 2, 2, 1, 1, 1, 3, 2, 1, 1, 4, 1, 1, 2, 1, 2, 3, 1, 3, 1, 3, 2, 4, 4, 4, 2, 3, 4, 1, 4, 1, 1, 2, 2, 5, 2, 3, 1, 3, 1, 1, 4, 2, 2, 1, 3, 2, 4, 1, 1, 1, 2, 4, 2, 3, 3, 4, 1, 1, 3, 1, 3, 2, 1, 4, 1, 5, 1, 2, 4, 2, 1, 1, 2, 1, 5, 3, 1, 1, 2, 1, 1, 1, 1, 1, 2, 2, 3, 2, 1, 1, 2, 2, 4, 1, 3, 1, 1, 3, 2, 4, 1, 1, 1, 1, 1, 2, 3, 2, 1, 2, 1, 3, 3, 2, 1, 1, 3, 2, 2, 3, 4, 1, 4, 1, 2, 5, 1, 2, 1, 5, 2, 2, 4, 1, 2, 1, 1, 5, 2, 1, 5, 2, 1, 1, 2, 1, 2, 1, 1, 1, 4, 4, 2, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 3, 1, 3, 1, 2, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 2, 1, 1, 5, 4, 1, 1, 1, 1, 3, 3, 3, 4, 3, 2, 1, 2, 2, 2, 2, 1, 1, 1, 2, 1, 2, 1, 2, 1, 4, 1, 2, 2, 1, 4, 1, 1, 2, 2, 4, 4, 1, 2, 1, 1, 2, 2, 1, 1, 1, 4, 1, 2, 1, 1, 3, 2, 1, 1, 1, 2, 3, 1, 4, 2, 3, 1, 1, 2, 2, 1, 2, 1, 2, 2, 1, 1, 1, 1, 2, 3, 3, 1, 1, 3, 3, 1, 1, 2, 3, 3, 4, 1, 1, 3, 4, 1, 2, 4, 3, 3, 1, 1, 3, 3, 1, 1, 5, 3, 1, 1, 1, 4, 1, 1, 1, 4, 1, 1, 5, 5, 2, 1, 3, 1, 1, 2, 2, 1, 1, 4, 4, 1, 5, 1, 2, 3, 2, 1, 1, 3, 1, 1, 1, 3, 2, 1, 2, 4, 1, 4, 1, 2, 3, 3, 3, 4, 2, 2, 1, 1, 2, 1, 1, 1, 3, 2, 2, 3, 2, 3, 1, 1, 1, 2, 1, 1, 3, 1, 2, 1, 4, 5, 2, 3, 4, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3, 3, 1, 1, 4, 4, 2, 3, 4, 2, 2, 2, 1, 2, 2, 1, 1, 2, 3, 1, 2, 1, 3, 1, 2, 1, 2, 1, 3, 1, 2, 1, 1, 2, 3, 1, 3, 2, 2, 1, 3, 2, 1, 4, 2, 1, 3, 1, 2, 1, 1, 2, 3, 2, 1, 1, 2, 3, 4, 5, 3, 2, 3, 2, 2, 4, 1, 3, 3, 1, 1, 2, 1, 1, 2, 2, 1, 3, 2, 1, 1, 1, 1, 3, 2, 1, 4, 3, 1, 4, 1, 1, 4, 3, 4, 1, 4, 4, 5, 1, 2, 3, 1, 3, 1, 4, 3, 1, 1, 1, 2, 5, 2, 2, 2, 1, 1, 2, 2, 4, 1, 1, 1, 1, 1, 4, 1, 3, 4, 1, 2, 5, 1, 2, 3, 1, 4, 3, 3, 1, 2, 2, 1, 2, 1, 3, 1, 2, 2, 2, 2, 1, 3, 2, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 2, 2, 2, 1, 2, 3, 1, 4, 1, 3, 1, 1, 3, 4, 1, 3, 1, 5, 1, 2, 1, 3, 1, 1, 1, 2, 1, 1, 1, 1, 3, 1, 2, 3, 1, 1, 2, 1, 3, 1, 1, 3, 2, 1, 1, 1, 4, 1, 1, 1, 5, 2, 2, 2, 4, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 2, 1, 3, 2, 1, 2, 1, 1, 1, 2, 1, 1, 2, 2, 1, 1, 2, 1, 2, 1, 5, 3, 1, 1, 1, 1, 4, 1, 3, 2, 1, 1, 2, 2, 1, 1, 4, 1, 3, 5, 1, 5, 3, 1, 1, 1, 1, 2, 3, 1, 1, 4, 1, 2, 1, 1, 1, 1, 4, 2, 2, 1, 2, 1, 1, 1, 1, 2, 3, 2, 3, 3, 2, 1, 2, 1, 3, 3, 4, 1, 2, 1, 4, 2, 2, 1, 1, 3, 3, 4, 2, 1, 1, 3, 2, 1, 1, 2, 2, 1, 1, 1, 4, 1, 3, 1, 1, 4, 2, 2, 1, 2, 3, 2, 1, 1, 3, 1, 1, 2, 3, 1, 1, 4, 1, 1, 3, 1, 1, 5, 1, 5, 1, 2, 4, 1, 2, 1, 5, 1, 2, 3, 3, 1, 1, 4, 1, 1, 2, 1, 1, 1, 1, 4, 2, 1, 5, 2, 1, 1, 3, 2, 1, 1, 3, 1, 1, 1, 1, 1, 4, 1, 3, 2, 1, 1, 2, 3, 3, 5, 5, 1, 1, 3, 2, 3, 2, 2, 2, 3, 2, 4, 1, 1, 2, 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1, 4, 5, 1, 2, 3, 1, 2, 4, 1, 2, 1, 1, 2, 1, 3, 1, 1, 2, 2, 1, 4, 1, 4, 2, 2, 2, 1, 4, 1, 2, 4, 1, 1, 3, 1, 4, 1, 4, 2, 3, 1, 1, 2, 5, 1, 1, 1, 1, 1, 2, 3, 1, 1, 2, 3, 4, 1, 1, 1, 4, 4, 1, 2, 2, 5, 3, 1, 5, 2, 2, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 3, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 3, 3, 1, 3, 3, 5, 5, 3, 2, 2, 2, 1, 5, 1, 1, 2, 1, 2, 4, 2, 5, 2, 2, 3, 1, 1, 5, 2, 2, 2, 1, 1, 2, 1, 3, 2, 4, 4, 4, 2, 2, 1, 2, 1, 1, 4, 2, 4, 3, 3, 4, 2, 1, 3, 2, 3, 2, 3, 1, 2, 1, 1, 5, 1, 2, 1, 1, 1, 2, 3, 2, 2, 4, 4, 2, 1, 2, 1, 1, 4, 2, 1, 3, 4, 1, 1, 1, 2, 1, 3, 5, 3, 3, 1, 1, 1, 1, 2, 1, 1, 2, 3, 2, 1, 1, 1, 4, 1, 3, 5, 1, 5, 1, 2, 3, 1, 1, 2, 2, 1, 1, 1, 1, 3, 1, 2, 1, 4, 2, 1, 2, 2, 1, 2, 3, 2, 3, 1, 1, 2, 3, 1, 5, 2, 3, 4, 2, 1, 3, 1, 2, 1, 2, 1, 2, 4, 1, 3, 1, 2, 1, 3, 1, 1, 1, 2, 1, 2, 4, 1, 3, 1, 2, 2, 1, 1, 2, 2, 4, 1, 1, 3, 1, 4, 1, 1, 2, 1, 2, 4, 2, 1, 1, 4, 1, 1, 2, 1, 2, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 4, 1, 1, 1, 1, 4, 4, 2, 1, 1, 1, 1, 4, 1, 3, 3, 4, 1, 1, 1, 3, 1, 1, 1, 3, 5, 2, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 2, 1, 3, 2, 1, 2, 1, 4, 1, 2, 1, 4, 3, 1, 2, 1, 4, 5, 4, 1, 1, 3, 1, 2, 2, 1, 3, 1, 1, 1, 2, 2, 2, 3, 2, 1, 1, 1, 5, 2, 3, 2, 1, 1, 2, 1, 1, 3, 5, 1, 1, 1, 2, 1, 5, 2, 4, 1, 2, 3, 1, 2, 2, 2, 3, 1, 2, 1, 1, 2, 1, 1, 5, 1, 2, 1, 2, 1, 1, 2, 2, 1, 4, 1, 1, 1, 2, 1, 3, 2, 4, 1, 5, 1, 2, 1, 1, 5, 4, 2, 1, 1, 2, 2, 2, 4, 1, 2, 2, 1, 5, 1, 1, 2, 1, 5, 1, 3, 4, 2, 2, 1, 2, 4, 2, 4, 1, 2, 4, 1, 1, 5, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 4, 1, 3, 2, 1, 1, 1, 1, 5, 2, 1, 3, 1, 1, 3, 3, 1, 3, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 2, 1, 2, 1, 3, 1, 3, 3, 2, 3, 1, 3, 4, 3, 3, 1, 2, 2, 1, 1, 2, 3, 1, 1, 1, 4, 3, 1, 1, 1, 3, 2, 1, 3, 4, 4, 1, 2, 1, 5, 1, 1, 2, 1, 3, 3, 2, 1, 3, 3, 2, 2, 5, 5, 3, 3, 1, 1, 1, 1, 2, 2, 1, 4, 1, 1, 2, 4, 2, 1, 1, 2, 2, 3, 3, 3, 1, 4, 1, 5, 3, 1, 4, 1, 1, 4, 2, 2, 4, 1, 4, 1, 1, 1, 5, 2, 3, 1, 2, 1, 2, 4, 2, 2, 2, 3, 1, 2, 4, 4, 1, 1, 1, 1, 2, 2, 3, 2, 1, 1, 1, 1, 1, 5, 1, 5, 2, 3, 2, 2, 1, 1, 4, 1, 4, 1, 1, 5, 1, 3, 3, 2, 1, 2, 2, 2, 1, 2, 1, 1, 2, 1, 5, 1, 4, 1, 1, 1, 2, 3, 1, 2, 4, 2, 4, 3, 1, 2, 1, 2, 5, 1, 3, 3, 2, 2, 1, 1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 4, 1, 3, 1, 1, 2, 5, 1, 2, 2, 4, 2, 4, 1, 1, 2, 3, 1, 3, 2, 1, 1, 1, 3, 4, 1, 3, 1, 2, 4, 2, 3, 3, 1, 3, 2, 3, 4, 1, 1, 2, 4, 3, 2, 1, 2, 2, 3, 1, 1, 1, 3, 2, 2, 1, 3, 1, 2, 3, 4, 1, 2, 1, 3, 1, 4, 3, 1, 1, 4, 2, 2, 2, 3, 1, 2, 1, 2, 1, 1, 2, 5, 4, 2, 2, 1, 1, 1, 1, 2, 2, 1, 2, 1, 4, 2, 2, 3, 1, 2, 3, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 2, 1, 2, 1, 3, 3, 2, 3, 2, 4, 3, 2, 1, 1, 1, 2, 2, 1, 1, 3, 3, 2, 1, 1, 3, 1, 4, 1, 5, 1, 3, 5, 1, 2, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 2, 4, 5, 1, 1, 2, 5, 1, 1, 1, 1, 1, 1, 2, 1, 3, 1, 2, 4, 1, 1, 1, 2, 1, 1, 1, 5, 2, 1, 1, 1, 3, 1, 3, 5, 1, 3, 1, 3, 1, 2, 3, 1, 1, 2, 1, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 1, 1, 3, 2, 3, 1, 1, 1, 2, 1, 5, 2, 1, 4, 1, 1, 2, 1, 2, 2, 1, 1, 2, 3, 1, 2, 1, 3, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 4, 2, 1, 3, 3, 2, 1, 1, 2, 1, 1, 1, 1, 3, 1, 1, 1, 3, 1, 2, 1, 1, 1, 2, 1, 1, 4, 3, 1, 3, 1, 2, 1, 4, 2, 1, 4, 3, 1, 1, 1, 2, 1, 1, 1, 3, 1, 1, 4, 4, 3, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 2, 2, 1, 2, 2, 2, 4, 1, 2, 1, 5, 2, 1, 2, 1, 1, 5, 1, 3, 1, 1, 1, 1, 5, 1, 1, 1, 1, 3, 4, 4, 1, 1, 1, 1, 3, 5, 5, 4, 2, 1, 1, 3, 3, 1, 1, 1, 1, 2, 1, 5, 1, 2, 2, 1, 2, 1, 2, 1, 4, 1, 2, 4, 2, 2, 3, 1, 4, 1, 1, 2, 1, 2, 1, 3, 2, 1, 2, 1, 1, 4, 4, 3, 3, 1, 1, 2, 2, 4, 3, 2, 1, 2, 2, 2, 5, 2, 1, 1, 5, 1, 3, 5, 1, 1, 1, 1, 4, 4, 1, 1, 1, 2, 1, 2, 1, 1, 4, 3, 2, 2, 1, 1, 1, 1, 1, 2, 3, 1, 3, 3, 4, 4, 1, 3, 1, 4, 1, 5, 1, 3, 4, 1, 1, 2, 2, 1, 2, 5, 5, 1, 1, 1, 1, 5, 2, 1, 1, 2, 1, 1, 2, 1, 3, 3, 2, 2, 3, 1, 2, 3, 2, 1, 2, 1, 5, 1, 2, 3, 1, 1, 1, 1, 5, 2, 1, 1, 2, 1, 1, 3, 4, 1, 3, 3, 1, 1, 3, 4, 1, 3, 4, 1, 2, 1, 1, 3, 2, 2, 2, 2, 5, 2, 1, 3, 3, 2, 3, 2, 4, 4, 2, 1, 3, 1, 5, 2, 1, 3, 1, 2, 2, 4, 2, 1, 2, 4, 5, 1, 3, 1, 1, 3, 4, 1, 2, 1, 5, 3, 4, 1, 1, 2, 2, 1, 1, 1, 5, 1, 1, 3, 2, 1, 2, 1, 1, 3, 1, 2, 4, 2, 1, 2, 3, 1, 1, 2, 5, 1, 1, 1, 1, 3, 2, 5, 3, 3, 1, 1, 1, 4, 4, 5, 1, 1, 2, 1, 2, 3, 3, 2, 1, 1, 1, 1, 1, 1, 5, 2, 4, 1, 2, 4, 5, 2, 1, 1, 1, 1, 1, 3, 1, 2, 4, 1, 1, 1, 2, 1, 2, 2, 2, 3, 1, 1, 2, 2, 4, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 5, 1, 3, 1, 1, 1, 2, 1, 1, 3, 1, 1, 4, 1, 3, 2, 3, 1, 1, 3, 1, 1, 2, 4, 1, 2, 3, 2, 3, 1, 3, 1, 1, 1, 1, 1, 1, 2, 2, 4, 1, 1, 1, 2, 1, 1, 3, 4, 3, 2, 2, 2, 2, 1, 2, 2, 1, 1, 2, 3, 1, 5, 4, 2, 1, 3, 1, 1, 1, 2, 3, 2, 4, 1, 1, 4, 1, 1, 3, 1, 1, 4, 4, 1, 2, 1, 4, 5, 2, 4, 1, 2, 4, 3, 2, 3, 4, 2, 3, 1, 1, 1, 1, 2, 3, 3, 1, 1, 5, 1, 4, 4, 1, 2, 2, 2, 1, 1, 2, 1, 3, 1, 1, 5, 1, 2, 1, 3, 3, 4, 1, 1, 3, 2, 4, 3, 1, 2, 1, 5, 1, 1, 2, 3, 4, 3, 1, 2, 2, 1, 1, 3, 3, 1, 2, 1, 1, 5, 1, 4, 4, 1, 2, 2, 1, 2, 1, 1, 1, 2, 4, 2, 4, 1, 4, 3, 3, 1, 1, 2, 1, 2, 1, 3, 3, 4, 2, 1, 1, 3, 1, 3, 2, 2, 1, 2, 1, 5, 2, 5, 2, 2, 2, 4, 1, 2, 3, 3, 1, 1, 3, 1, 3, 1, 1, 2, 1, 3, 1, 2, 3, 1, 2, 1, 3, 1, 3, 1, 4, 2, 1, 4, 2, 4, 2, 3, 5, 3, 3, 4, 2, 1, 3, 4, 3, 4, 4, 2, 1, 3, 1, 2, 1, 1, 3, 1, 4, 1, 5, 1, 2, 4, 3, 4, 3, 3, 1, 5, 2, 1, 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 3, 1, 2, 1, 1, 5, 3, 2, 2, 1, 1, 2, 3, 1, 1, 1, 1, 2, 3, 5, 1, 3, 3, 2, 2, 1, 1, 1, 2, 1, 4, 5, 1, 2, 3, 2, 1, 1, 1, 4, 1, 3, 2, 2, 1, 1, 2, 2, 1, 1, 1, 3, 2, 1, 1, 4, 1, 1, 2, 1, 2, 3, 1, 3, 1, 3, 2, 4, 4, 4, 2, 3, 4, 1, 4, 1, 1, 2, 2, 5, 2, 3, 1, 3, 1, 1, 4, 2, 2, 1, 3, 2, 4, 1, 1, 1, 2, 4, 2, 3, 3, 4, 1, 1, 3, 1, 3, 2, 1, 4, 1, 5, 1, 2, 4, 2, 1, 1, 2, 1, 5, 3, 1, 1, 2, 1, 1, 1, 1, 1, 2, 2, 3, 2, 1, 1, 2, 2, 4, 1, 3, 1, 1, 3, 2, 4, 1, 1, 1, 1, 1, 2, 3, 2, 1, 2, 1, 3, 3, 2, 1, 1, 3, 2, 2, 3, 4, 1, 4, 1, 2, 5, 1, 2, 1, 5, 2, 2, 4, 1, 2, 1, 1, 5, 2, 1, 5, 2, 1, 1, 2, 1, 2, 1, 1, 1, 4, 4, 2, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 3, 1, 3, 1, 2, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 2, 1, 1, 5, 4, 1, 1, 1, 1, 3, 3, 3, 4, 3, 2, 1, 2, 2, 2, 2, 1, 1, 1, 2, 1, 2, 1, 2, 1, 4, 1, 2, 2, 1, 4, 1, 1, 2, 2, 4, 4, 1, 2, 1, 1, 2, 2, 1, 1, 1, 4, 1, 2, 1, 1, 3, 2, 1, 1, 1, 2, 3, 1, 4, 2, 3, 1, 1, 2, 2, 1, 2, 1, 2, 2, 1, 1, 1, 1, 2, 3, 3, 1, 1, 3, 3, 1, 1, 2, 3, 3, 4, 1, 1, 3, 4, 1, 2, 4, 3, 3, 1, 1, 3, 3, 1, 1, 5, 3, 1, 1, 1, 4, 1, 1, 1, 4, 1, 1, 5, 5, 2, 1, 3, 1, 1, 2, 2, 1, 1, 4, 4, 1, 5, 1, 2, 3, 2, 1, 1, 3, 1, 1, 1, 3, 2, 1, 2, 4, 1, 4, 1, 2, 3, 3, 3, 4, 2, 2, 1, 1, 2, 1, 1, 1, 3, 2, 2, 3, 2, 3, 1, 1, 1, 2, 1, 1, 3, 1, 2, 1, 4, 5, 2, 3, 4, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3, 3, 1, 1, 4, 4, 2, 3, 4, 2, 2, 2, 1, 2, 2, 1, 1, 2, 3, 1, 2, 1, 3, 1, 2, 1, 2, 1, 3, 1, 2, 1, 1, 2, 3, 1, 3, 2, 2, 1, 3, 2, 1, 4, 2, 1, 3, 1, 2, 1, 1, 2, 3, 2, 1, 1, 2, 3, 4, 5, 3, 2, 3, 2, 2, 4, 1, 3, 3, 1, 1, 2, 1, 1, 2, 2, 1, 3, 2, 1, 1, 1, 1, 3, 2, 1, 4, 3, 1, 4, 1, 1, 4, 3, 4, 1, 4, 4, 5, 1, 2, 3, 1, 3, 1, 4, 3, 1, 1, 1, 2, 5, 2, 2, 2, 1, 1, 2, 2, 4, 1, 1, 1, 1, 1, 4, 1, 3, 4, 1, 2, 5, 1, 2, 3, 1, 4, 3, 3, 1, 2, 2, 1, 2, 1, 3, 1, 2, 2, 2, 2, 1, 3, 2, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 2, 2, 2, 1, 2, 3, 1, 4, 1, 3, 1, 1, 3, 4, 1, 3, 1, 5, 1, 2, 1, 3, 1, 1, 1, 2, 1, 1, 1, 1, 3, 1, 2, 3, 1, 1, 2, 1, 3, 1, 1, 3, 2, 1, 1, 1, 4, 1, 1, 1, 5, 2, 2, 2, 4, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 2, 1, 3, 2, 1, 2, 1, 1, 1, 2, 1, 1, 2, 2, 1, 1, 2, 1, 2, 1, 5, 3, 1, 1, 1, 1, 4, 1, 3, 2, 1, 1, 2, 2, 1, 1, 4, 1, 3, 5, 1, 5, 3, 1, 1, 1, 1, 2, 3, 1, 1, 4, 1, 2, 1, 1, 1, 1, 4, 2, 2, 1, 2, 1, 1, 1, 1, 2, 3, 2, 3, 3, 2, 1, 2, 1, 3, 3, 4, 1, 2, 1, 4, 2, 2, 1, 1, 3, 3, 4, 2, 1, 1, 3, 2, 1, 1, 2, 2, 1, 1, 1, 4, 1, 3, 1, 1, 4, 2, 2, 1, 2, 3, 2, 1, 1, 3, 1, 1, 2, 3, 1, 1, 4, 1, 1, 3, 1, 1, 5, 1, 5, 1, 2, 4, 1, 2, 1, 5, 1, 2, 3, 3, 1, 1, 4, 1, 1, 2, 1, 1, 1, 1, 4, 2, 1, 5, 2, 1, 1, 3, 2, 1, 1, 3, 1, 1, 1, 1, 1, 4, 1, 3, 2, 1, 1, 2, 3, 3, 5, 5, 1, 1, 3, 2, 3, 2, 2, 2, 3, 2, 4, 1, 1, 2, 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1, 4, 5, 1, 2, 3, 1, 2, 4, 1, 2, 1, 1, 2, 1, 3, 1, 1, 2, 2, 1, 4, 1, 4, 2, 2, 2, 1, 4, 1, 2, 4, 1, 1, 3, 1, 4, 1, 4, 2, 3, 1, 1, 2, 5, 1, 1, 1, 1, 1, 2, 3, 1, 1, 2, 3, 4, 1, 1, 1, 4, 4, 1, 2, 2, 5, 3, 1, 5, 2, 2, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 3, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 3, 3, 1, 3, 3, 5, 5, 3, 2, 2, 2, 1, 5, 1, 1, 2, 1, 2, 4, 2, 5, 2, 2, 3, 1, 1, 5, 2, 2, 2, 1, 1, 2, 1, 3, 2, 4, 4, 4, 2, 2, 1, 2, 1, 1, 4, 2, 4, 3, 3, 4, 2, 1, 3, 2, 3, 2, 3, 1, 2, 1, 1, 5, 1, 2, 1, 1, 1, 2, 3, 2, 2, 4, 4, 2, 1, 2, 1, 1, 4, 2, 1, 3, 4, 1, 1, 1, 2, 1, 3, 5, 3, 3, 1, 1, 1, 1, 2, 1, 1, 2, 3, 2, 1, 1, 1, 4, 1, 3, 5, 1, 5, 1, 2, 3, 1, 1, 2, 2, 1, 1, 1, 1, 3, 1, 2, 1, 4, 2, 1, 2, 2, 1, 2, 3, 2, 3, 1, 1, 2, 3, 1, 5, 2, 3, 4, 2, 1, 3, 1, 2, 1, 2, 1, 2, 4, 1, 3, 1, 2, 1, 3, 1, 1, 1, 2, 1, 2, 4, 1, 3, 1, 2, 2, 1, 1, 2, 2, 4, 1, 1, 3, 1, 4, 1, 1, 2, 1, 2, 4, 2, 1, 1, 4, 1, 1, 2, 1, 2, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 4, 1, 1, 1, 1, 4, 4, 2, 1, 1, 1, 1, 4, 1, 3, 3, 4, 1, 1, 1, 3, 1, 1, 1, 3, 5, 2, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 2, 1, 3, 2, 1, 2, 1, 4, 1, 2, 1, 4, 3, 1, 2, 1, 4, 5, 4, 1, 1, 3, 1, 2, 2, 1, 3, 1, 1, 1, 2, 2, 2, 3, 2, 1, 1, 1, 5, 2, 3, 2, 1, 1, 2, 1, 1, 3, 5, 1, 1, 1, 2, 1, 5, 2, 4, 1, 2, 3, 1, 2, 2, 2, 3, 1, 2, 1, 1, 2, 1, 1, 5, 1, 2, 1, 2, 1, 1, 2, 2, 1, 4, 1, 1, 1, 2, 1, 3, 2, 4, 1, 5, 1, 2, 1, 1, 5, 4, 2, 1, 1, 2, 2, 2, 4, 1, 2, 2, 1, 5, 1, 1, 2, 1, 5, 1, 3, 4, 2, 2, 1, 2, 4, 2, 4, 1, 2, 4, 1, 1, 5, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 4, 1, 3, 2, 1, 1, 1, 1, 5, 2, 1, 3, 1, 1, 3, 3, 1, 3, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 2, 1, 2, 1, 3, 1, 3, 3, 2, 3, 1, 3, 4, 3, 3, 1, 2, 2, 1, 1, 2, 3, 1, 1, 1, 4, 3, 1, 1, 1, 3, 2, 1, 3, 4, 4, 1, 2, 1, 5, 1, 1, 2, 1, 3, 3, 2, 1, 3, 3, 2, 2, 5, 5, 3, 3, 1, 1, 1, 1, 2, 2, 1, 4, 1, 1, 2, 4, 2, 1, 1, 2, 2, 3, 3, 3, 1, 4, 1, 5, 3, 1, 4, 1, 1, 4, 2, 2, 4, 1, 4, 1, 1, 1, 5, 2, 3, 1, 2, 1, 2, 4, 2, 2, 2, 3, 1, 2, 4, 4, 1, 1, 1, 1, 2, 2, 3, 2, 1, 1, 1, 1, 1, 5, 1, 5, 2, 3, 2, 2, 1, 1, 4, 1, 4, 1, 1, 5, 1, 3, 3, 2, 1, 2, 2, 2, 1, 2, 1, 1, 2, 1, 5, 1, 4, 1, 1, 1, 2, 3, 1, 2, 4, 2, 4, 3, 1, 2, 1, 2, 5, 1, 3, 3, 2, 2, 1, 1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 4, 1, 3, 1, 1, 2, 5, 1, 2, 2, 4, 2, 4, 1, 1, 2, 3, 1, 3, 2, 1, 1, 1, 3, 4, 1, 3, 1, 2, 4, 2, 3, 3, 1, 3, 2, 3, 4, 1, 1, 2, 4, 3, 2, 1, 2, 2, 3, 1, 1, 1, 3, 2, 2, 1, 3, 1, 2, 3, 4, 1, 2, 1, 3, 1, 4, 3, 1, 1, 4, 2, 2, 2, 3, 1, 2, 1, 2, 1, 1, 2, 5, 4, 2, 2, 1, 1, 1, 1, 2, 2, 1, 2, 1, 4, 2, 2, 3, 1, 2, 3, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 2, 1, 2, 1, 3, 3, 2, 3, 2, 4, 3, 2, 1, 1, 1, 2, 2, 1, 1, 3, 3, 2, 1, 1, 3, 1, 4, 1, 5, 1, 3, 5, 1, 2, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 2, 4, 5, 1, 1, 2, 5, 1, 1, 1, 1, 1, 1, 2, 1, 3, 1, 2, 4, 1, 1, 1, 2, 1, 1, 1, 5, 2, 1, 1, 1, 3, 1, 3, 5, 1, 3, 1, 3, 1, 2, 3, 1, 1, 2, 1, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 1, 1, 3, 2, 3, 1, 1, 1, 2, 1, 5, 2, 1, 4, 1, 1, 2, 1, 2, 2, 1, 1, 2, 3, 1, 2, 1, 3, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 4, 2, 1, 3, 3, 2, 1, 1, 2, 1, 1, 1, 1, 3, 1, 1, 1, 3, 1, 2, 1, 1, 1, 2, 1, 1, 4, 3, 1, 3, 1, 2, 1, 4, 2, 1, 4, 3, 1, 1, 1, 2, 1, 1, 1, 3, 1, 1, 4, 4, 3, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 1, 2, 2, 1, 2, 2, 2, 4, 1, 2, 1, 5, 2, 1, 2, 1, 1, 5, 1, 3, 1, 1, 1, 1, 5, 1, 1, 1, 1, 3, 4, 4, 1, 1, 1, 1, 3, 5, 5, 4, 2, 1, 1, 3, 3, 1, 1, 1, 1, 2, 1, 5, 1, 2, 2, 1, 2, 1, 2, 1, 4, 1, 2, 4, 2, 2, 3, 1, 4, 1, 1, 2, 1, 2, 1, 3, 2, 1, 2, 1, 1, 4, 4, 3, 3, 1, 1, 2, 2, 4, 3, 2, 1, 2, 2, 2, 5, 2, 1, 1, 5, 1, 3, 5, 1, 1, 1, 1, 4, 4, 1, 1, 1, 2, 1, 2, 1, 1, 4, 3, 2, 2, 1, 1, 1, 1, 1, 2, 3, 1, 3, 3, 4, 4, 1, 3, 1, 4, 1, 5, 1, 3, 4, 1, 1, 2, 2, 1, 2, 5, 5, 1, 1, 1, 1, 5, 2, 1, 1, 2, 1, 1, 2, 1, 3, 3, 2, 2, 3, 1, 2, 3, 2, 1, 2, 1, 5, 1, 2, 3, 1, 1, 1, 1, 5, 2, 1, 1, 2, 1, 1, 3, 4, 1, 3, 3, 1, 1, 3, 4, 1, 3, 4, 1, 2, 1, 1, 3, 2, 2, 2, 2, 5, 2, 1, 3, 3, 2, 3, 2, 4, 4, 2, 1, 3, 1, 5, 2, 1, 3, 1, 2, 2, 4, 2, 1, 2, 4, 5, 1, 3, 1, 1, 3, 4, 1, 2, 1, 5, 3, 4, 1, 1, 2, 2, 1, 1, 1, 5, 1, 1, 3, 2, 1, 2, 1, 1, 3, 1, 2, 4, 2, 1, 2, 3, 1, 1, 2, 5, 1, 1, 1, 1, 3, 2, 5, 3, 3, 1, 1, 1, 4, 4, 5, 1, 1, 2, 1, 2, 3, 3, 2, 1, 1, 1, 1, 1, 1, 5, 2, 4, 1, 2, 4, 5, 2, 1, 1, 1, 1, 1, 3, 1, 2, 4, 1, 1, 1, 2, 1, 2, 2, 2, 3, 1, 1, 2, 2, 4, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 5, 1, 3, 1, 1, 1, 2, 1, 1, 3, 1, 1, 4, 1, 3, 2, 3, 1, 1, 3, 1, 1, 2, 4, 1, 2, 3, 2, 3, 1, 3, 1, 1, 1, 1, 1, 1, 2, 2, 4, 1, 1, 1, 2, 1, 1, 3, 4, 3, 2, 2, 2, 2, 1, 2, 2, 1, 1, 2, 3, 1, 5, 4, 2, 1, 3, 1, 1, 1, 2, 3};

int random_spot=1438587;
int random_inc(Node3D* res_piece, int num_spot) {
	if (res_piece==nullptr || num_spot<46) { //always check the first 46 spots
		return 1; //go through loop normally until
			//a valid move is found.
	} else {
		if (random_spot>15710) {
			random_spot=0;	
		}
		int r=random_spot; random_spot++;
		r=possible_inc[r];
		return r;
	}
	return 1;
}


#define POSSIBLE_NTH 60
Node3D* move_node_array[POSSIBLE_NTH];
Vector3 move_vec_array[POSSIBLE_NTH];

//piece_out is the output pointer of what piece to move to get the best value (it is also the input piece //for effectively telling the function which color of piece to move (likely just passing in the pointer //to //a //stack variable that originally just holds the first item of the relevant array of pieces of one //side)), with the output vector3 being where to move that piece_out.
//the value of the Vector3.y component is the value of the move chosen (from the AI’s perspective)
int times_called_fast_board_value=0;
float fast_board_value(Node3D *piece_out, Vector3 move_to,  std::vector<Node3D*> my_pieces, Vector3 my_king_start, std::vector<Node3D*> human_pieces, Vector3 human_king_start, std::vector<Vector3> possible_positions, Vector3 *accepted_positions) {
	
	
	//need to make certain that is_my_piece is always set up properly.
	times_called_fast_board_value+=1;
	if (times_called_fast_board_value>1000000007) {
		times_called_fast_board_value=31;
	}
	if (times_called_fast_board_value<2 || times_called_fast_board_value%100==1) {
		for (int i=0; i<DEFAULT_SIZE; i++) {
			is_my_piece_table[i]=-1;
		}
		for (int i=0; i<my_pieces.size(); i++) {
			Node3D* item=my_pieces[i];
			 int spot=is_my_piece_hash(item);
			if (spot<0) {
				println(__LINE__);	//called 0 times
			} else {
				is_my_piece_table[spot]=1;
			}
		}
		for (int i=0; i<human_pieces.size(); i++) {
			Node3D* item=my_pieces[i];
			 int spot=is_my_piece_hash(item);
			if (spot<0) {
				println(__LINE__);	//called 0 times
			} else {
				is_my_piece_table[spot]=0;
			}
		}
	}
	
	
	println(__LINE__); //called one time
	float result=0;
	Node3D *res_piece=nullptr;
	float ai_v=-10000; //the AI value is put in the place of the y value of the returned Vector3
	//this is the value of the current board state to the AI given the human making optimal 
	//counter-moves, and this returned value is literally the whole point of this function working.
	Node3D *attacked_piece=nullptr;
	Vector3 atkdpst=Vector3_ZERO;
	float cur_ai_v=0;
	
	bool my_move=(is_my_piece(piece_out)==is_my_piece(my_pieces[0]));
	if (my_move) {
		if (already_visited(piece_out, move_to) || fast_can_move_to(piece_out, piece_out->get_second_global_position(), move_to, my_pieces, my_king_start,  human_pieces,  human_king_start,  accepted_positions)==false) {
			result=-10000;
		} else {
			//the calling function is calling based on the piece called piece_out being sent to Vector3 move_to
			for (int i=0; i<human_pieces.size(); i+=1) {
				//println(__LINE__); //called 23 times
				Node3D *to_move=human_pieces[i];
				Vector3 start=to_move->get_second_global_position();
				int found=-1;
				for (int j=0; j<possible_positions.size(); j+=random_inc(res_piece, j)) {
					//println(__LINE__); //called 3289 times
					Vector3 spot=possible_positions[j];
					Vector3 end=spot;
					if (fast_can_move_to(to_move, start, end, my_pieces, my_king_start,  human_pieces,  human_king_start,  accepted_positions)) {
						cur_ai_v=(float)(value_from_state(to_move, spot,  my_pieces,  my_king_start,  human_pieces,  human_king_start,  possible_positions,  accepted_positions));
						//AI specific condition, the human uses <
						
						//println(error_move+__LINE__+200000);
						
						if (error_move==0 && (cur_ai_v<ai_v || res_piece==nullptr)) {
							ai_v=cur_ai_v;
							res_piece=to_move;
							result=ai_v;
							
							attacked_piece=fast_piece_at(spot,  my_pieces,  human_pieces);
							if (attacked_piece!=nullptr && (attacked_piece->get_second_global_position()-spot).length_squared()>1/3.0) {
								attacked_piece=nullptr; //it was not in range, so it might as well not exist.	
							}
							atkdpst=spot;
						}
					}
				}
			}
		}
	} else {
		//the calling function is calling to figure out how bad of a move the human piece_out being sent to Vector3 move_to is.
		for (int i=0; i<my_pieces.size(); i+=1) {
			//println(__LINE__);
			Node3D *to_move=my_pieces[i];
			Vector3 start=to_move->get_second_global_position();
			for (int j=0; j<possible_positions.size(); j+=random_inc(res_piece, j)) {
				//println(__LINE__);
				Vector3 spot=possible_positions[j];
				Vector3 end=spot;
				if (already_visited(to_move, end) || fast_can_move_to(to_move, start, end, my_pieces, my_king_start, human_pieces, human_king_start, accepted_positions)==false) {
					continue; //do not check this loop iteration
				} else if (fast_can_move_to(to_move, start, end, my_pieces, my_king_start,  human_pieces,  human_king_start,    accepted_positions)) {
					cur_ai_v=(float)(value_from_state(to_move, spot,  my_pieces,  my_king_start,  human_pieces,  human_king_start,   possible_positions, accepted_positions));
					//AI specific condition, the human uses <
					if (error_move==0 && (cur_ai_v>ai_v || res_piece==nullptr)) {
						ai_v=cur_ai_v;
						res_piece=to_move;
						result=ai_v;
						
						
						attacked_piece=fast_piece_at(spot,  my_pieces,  human_pieces);
						if (attacked_piece!=nullptr && (attacked_piece->get_second_global_position()-spot).length_squared()>1/3.0) {
							attacked_piece=nullptr; //it was not in range, so it might as well not exist.	
						}
						atkdpst=spot;
					}
				}
			}
		}
	}
	//now, calculating the value of the piece taken.
	
	result=ai_v;
	return result;
}




//this function creates a larger area which would be covered by a given hash integer, thus causing 
//a not-to-be-deleted-spot to prevent the deletion of possibly several surrounding spots.
int removable_position_hash(Vector3 spot) {

	//maximum hash is 414, minimum hash is 10. I will have the array of bools be 450 long for safety.
	float x=spot.x;
	float z=spot.z;
	int result=int((x-MIN_X_POS)/(MAX_X_POS-MIN_X_POS+0.0)*20.0); //a number from 0 to 19
	result=int(result*20.0 + (z - MIN_Z_POS)/(MAX_Z_POS-MIN_Z_POS+0.0)*20.0); //x takes up higher-order bits; z is changed to be ordered from 0 to 19.
	
	return result;
}

void set_non_removable(Vector3 spot, bool positionsTable[450]) {
	println(__LINE__); //called 46 times.
	Vector3 start=spot;
	start.x-=7/2.0;
	start.z-=7/2.0;
	for (int i=0; i<15; i++) {
		start.z=spot.z-7/2.0;
		for (int j=0; j<15; j++) {
			positionsTable[removable_position_hash(start)]=false; //the position cannot be removed.
			start.z+=0.5;
		}
		start.x+=0.5;
	}
}

bool removable_spot(Vector3 spot, bool positionsTable[450]) {
	bool result=true; //if the position can't be found, return true, that this spot can be removed.
	int hash=removable_position_hash(spot);
	result=positionsTable[hash];
	return result;
}

int first_call=0;
float first_call_metric=-1; //this changed later to be the average distance between pieces of the same color to each other.
//this is used in order to establish 

//need a test of whether any pieces have moved from where they were previously.
struct Node3Dprev {
	Node3D *item;
	Vector3 prev;
};
typedef struct Node3Dprev Nwhere;
int previous_poses_size=0;
Nwhere previous_poses[100];
bool have_pieces_moved(Array my_pieces, Array human_pieces) {
	bool result=false; //assume that they have not moved.
	if (previous_poses_size==0) { //pieces have moved relative to the previous condition of just not existing.
		
		result=true;
		//just do setup of previous_poses
		for (int i=0; i<my_pieces.size(); i++) {
			Node3D *cur=static_cast<Node3D*>((Object *)( my_pieces[i]));
			previous_poses[previous_poses_size].item=cur;
			previous_poses[previous_poses_size].prev=cur->get_global_position();
			previous_poses_size+=1;
		}
		for (int i=0; i<human_pieces.size() && i+my_pieces.size()<100; i++) {
			Node3D *cur=static_cast<Node3D*>((Object *)( human_pieces[i]));
			previous_poses[previous_poses_size].item=cur;
			previous_poses[previous_poses_size].prev=cur->get_global_position();
			previous_poses_size+=1;
		}
	}
	if (result==false) {
		//check whether any of the pieces listed has moved relative to where it is in previous_poses	
		for (int i=0; i<my_pieces.size() && result==false; i++) {
			Node3D *cur=static_cast<Node3D*>((Object *)( my_pieces[i]));
			for (int j=0; j<previous_poses_size && j<100 && result==false; j++) {
				Node3D *temp=previous_poses[j].item;
				if (cur==temp) {
					if ((cur->get_global_position()-previous_poses[j].prev).length_squared()>1.0/3.0) {
						result=true;
					}
				}
			}
		}
		for (int i=0; i<human_pieces.size() && i+my_pieces.size()<100 && result==false; i++) {
			Node3D *cur=static_cast<Node3D*>((Object *)( human_pieces[i]));
			for (int j=0; j<previous_poses_size && j<100 && result==false; j++) {
				Node3D *temp=previous_poses[j].item;
				if (cur==temp) {
					if ((cur->get_global_position()-previous_poses[j].prev).length_squared()>1.0/3.0) {
						result=true;
					}
				}
			}
		}
		//set previous_poses to current locations of listed pieces.
		for (int i=0; i<my_pieces.size() ; i++) {
			Node3D *cur=static_cast<Node3D*>((Object *)( my_pieces[i]));
			for (int j=0; j<previous_poses_size && j<100 ; j++) {
				Node3D *temp=previous_poses[j].item;
				if (cur==temp) {
					previous_poses[j].prev=cur->get_global_position();
				}
			}
		}
		for (int i=0; i<human_pieces.size() && i+my_pieces.size()<100 ; i++) {
			Node3D *cur=static_cast<Node3D*>((Object *)( human_pieces[i]));
			for (int j=0; j<previous_poses_size && j<100 ; j++) {
				Node3D *temp=previous_poses[j].item;
				if (cur==temp) {
					previous_poses[j].prev=cur->get_global_position();
				}
			}
		}
	}
	return result;
}



Vector3 fast_make_move_best_move=Vector3(0,0,0); //the static assignment of space for holding the resultant best move.
Node3D* OmniLight3D::fast_make_move(Array my_pieces, float my_king_start_x, float my_king_start_y, float my_king_start_z, Array human_pieces, float human_king_start_x, float human_king_start_y, float human_king_start_z, Array possible_positions, int nth_optimal, int recurse_level) {
	bool internal_self_callback=self_calling_back;
	if (nth_optimal>0) {
		real_nth_optimal+=1;
	} else {
		real_nth_optimal=0;
		if (have_pieces_moved(my_pieces, human_pieces)) {
			for (int i=0; i<BLACKLIST_SIZE; i++) {
				possible_blacklist_of_pieces_num_bad[i]=0; //reset blacklist
			}
		} else {
			self_calling_back=false;
		}
	}
	
		Vector3 my_king_start=Vector3(my_king_start_x, my_king_start_y, my_king_start_z);
		Vector3 human_king_start=Vector3(human_king_start_x, human_king_start_y, human_king_start_z);
		std::vector<Node3D*> _my_pieces;
		std::vector<Node3D*> _human_pieces;
		std::vector<Vector3> _possible_positions; //needs to be revised so first 46 spots are where pieces are.
		Vector3 accepted_positions[DEFAULT_SIZE]; //this tells us whether a given position is being considered at all, preventing longer fast_piece_at calls in fast_raycast 
		_my_pieces.reserve(50);
		_human_pieces.reserve(50);
		_possible_positions.reserve(1000);
		//accepted_positions.reserve(1000);
		if (first_call==0) {
			first_call=1;
			//hash_pos_to_spot_table.reserve(5007);
			for (int i=0; i<DEFAULT_SIZE; i++) {
				hash_pos_to_spot_table[i]=Vector3_DOWN;
				is_my_piece_table[i]=-1;
				pawn_improved_table[i]=0;
				spiral_directions[i]=0;
				piece_to_type_table[i]=-1;
				pawn_hash_pos_to_can_move[i]=-1;
				bishop_hash_pos_to_can_move[i]=-1;
				rook_hash_pos_to_can_move[i]=-1;
				sp_rook_hash_pos_to_can_move[i]=-1;
				already_visited_table[i]=0; //50*546 would be the number of pieces*number of spots, significantly greater than the number of possible recommendations that could be given
			}
			//though technically, the number would be more correct if I used 7751016, or 26*546*546, since the hashes are based both on the piece, the current location of the piece
			//and the recommendation of where to put the piece.
		} else {
			first_call+=1;
		}
		
		bool positionsTable[450]; //used locally in this function in order to remove positions which are not reasonably close to actual pieces.
		for (int i=0; i<450; i++) {
			positionsTable[i]=true; 
			//I was thinking of setting this to false by default just because of how spiral_rook4 is being told to move to center all the time, which might be an indication that the computer has become 
			//much faster now that std::unordered_map is no longer included.
			//alternatively, solving the problem may stem from how the ray_cast function's stepping down the line of pieces that might be in the way may skip a couple 
			//pieces on account of hash_pos not creating a fine enough distinction between piece positions.
			//moving from 4.1 to 5.1 in the factor may solve that problem.
			//originally set to true, because
			//positions are removable by default.
		}
		
		for (int i=0; i<DEFAULT_SIZE; i++) {
			accepted_positions[i]=Vector3_DOWN;
			
		}
		int count_living=0;
		for (int i=0; i<my_pieces.size(); i++) {
			Node3D *piece= static_cast<Node3D*>((Object *)( my_pieces[i]));
			
			int num=is_my_piece_hash(piece);
			
			int index=num;
			if (piece->get_second_global_position().y>-10) {
				_my_pieces.push_back(piece);
				if (piece->get_second_global_position().y>-10) {
					count_living++;
				}
				if (String(piece->get_name()).contains("in")) { //king
					piece_to_type_table[index]=10000;
				} else if (String(piece->get_name()).contains("pir")) { 
					piece_to_type_table[index]= 200;
				} else if (String(piece->get_name()).contains("oo")) { 
					piece_to_type_table[index]= 130;
				} else if (String(piece->get_name()).contains("ish")) { 
					piece_to_type_table[index]= 40;
				} else if (String(piece->get_name()).contains("nig")) { //knight
					piece_to_type_table[index]= 5;
				} else if (String(piece->get_name()).contains("aw")) { 
					piece_to_type_table[index]= 1;
				}
				println(__LINE__); //ran 23 times.
				set_non_removable(piece->get_second_global_position(), positionsTable);
			}
		}
		for (int i=0; i<human_pieces.size(); i++) {
			Node3D *piece= static_cast<Node3D*>((Object *)( human_pieces[i]));
			
			int num=is_my_piece_hash(piece);
			
			int index=num;
			if (piece->get_second_global_position().y>-10) {
				_human_pieces.push_back(piece);
				if (String(piece->get_name()).contains("in")) { //king
					piece_to_type_table[index]=10000;
				} else if (String(piece->get_name()).contains("pir")) { 
					piece_to_type_table[index]= 200;
				} else if (String(piece->get_name()).contains("oo")) { 
					piece_to_type_table[index]= 130;
				} else if (String(piece->get_name()).contains("ish")) { 
					piece_to_type_table[index]= 40;
				} else if (String(piece->get_name()).contains("nig")) { //knight
					piece_to_type_table[index]= 5;
				} else if (String(piece->get_name()).contains("aw")) { 
					piece_to_type_table[index]= 1;
				}
				println(__LINE__); //ran 23 times.
				set_non_removable(piece->get_second_global_position(), positionsTable);
			}
			
		}
		int best_46_count=0;
		Vector3 best_46_positions[46]; //this array establishes which possible_positions already got 
			//included in _possible_positions.
		for (int i=0; i<_human_pieces.size() && best_46_count<46; i++) {
			if (_human_pieces[i]!=nullptr) {
				best_46_positions[best_46_count]=_human_pieces[i]->get_second_global_position();
				_possible_positions.push_back(_human_pieces[i]->get_second_global_position()); 
				best_46_count+=1;
			}
		}
		for (int i=0; i<_my_pieces.size() && best_46_count<46; i++) {
			if (_my_pieces[i]!=nullptr) {
				best_46_positions[best_46_count]=_my_pieces[i]->get_second_global_position();
				_possible_positions.push_back(_my_pieces[i]->get_second_global_position()); 
				best_46_count+=1;
			}
		}
		for (int i=0; i<possible_positions.size(); i++) {
			println(__LINE__); //ran 546 times.
			Vector3 piece= ((Vector3)( possible_positions[i]));
			if (removable_spot(piece, positionsTable)==true) {
				continue;
			} else {
				bool check_already_included=false;
				for (int j=0; j<best_46_count && check_already_included==false; j++) {
					if ((piece-best_46_positions[j]).length_squared()<.01) {
						check_already_included=true;
					}
				}
				if (check_already_included==false) {
					println(__LINE__); //ran 143 times when an array possible_positions is used, but runs 0 times when std::unordered_map<int, bool> is used, even when space is reserved, and even when switching from insert_or_assign and [] override.
					_possible_positions.push_back(piece); //include the spot if it is not removable.
					accepted_positions[hash_pos(piece)%DEFAULT_SIZE]=piece;
				}
			}
			
		}
		 //making sure that every piece has ownership set properly.
		for (int i=0; i<my_pieces.size(); i++) {
			println(__LINE__); //maybe ther error her here or later.
			 int num=is_my_piece_hash(_my_pieces[i]);
			char temp_str[]="for my AI: is_my_piece_hash ";
			char temp_str2[]=" at that hash, pointer as num would be ";
			print_word_num_on_line(temp_str, num, __LINE__); 
			print_word_num_on_line(temp_str2, (int)(((long)((void*)(_my_pieces[i]))) & 2147483647 ), __LINE__-1);
			is_my_piece_table[num]= 1;		
		}
		for (int i=0; i<_human_pieces.size(); i++) {
			println(__LINE__); //was able to run 23 times, so is_my_piece_hash is not the issue.
			 int num=is_my_piece_hash(_human_pieces[i]);
			char temp_str[]="for human: is_my_piece_hash ";
			char temp_str2[]=" at that hash, pointer as num would be ";
			print_word_num_on_line(temp_str, num, __LINE__); print_word_num_on_line(temp_str2, (int)(((long)((void*)(_human_pieces[i]))) & 2147483647 ) , __LINE__);
			is_my_piece_table[num]= 0;
		}
		
		if (first_call_metric<0) { //only happens before the proper value is calculated.
			first_call_metric=0;
			int count=0;
			for (int i=0; i<_my_pieces.size(); i++) {
				Vector3 a=_my_pieces[i]->get_second_global_position();
				for (int j=0; j<_my_pieces.size(); j++) {
					if (i!=j) {
						Vector3 b=_my_pieces[j]->get_second_global_position();
						first_call_metric+=(a-b).length();
						count+=1;
					}
				}
			}
			first_call_metric/=count;
		}
		
		if (count_living>=25) { 
			//during the course of the AI getting a pawn to promote, it must have lost at least 1 piece.
			// pawn_improved_table.clear(); //because no pieces lost probably indicates the start of the 
			for (int i=0; i<DEFAULT_SIZE; i++) {
				pawn_improved_table[i]=0;
			}
								//game.
			bool is_next_game=false;
			if (first_call<10) {
				is_next_game=false;
			} else {
				if (already_visited_table_size>25) {
					//we will need to see whether the pieces are all really close to each other. we can generate a metric 
					float metric=0;
					int count=0;
					for (int i=0; i<_my_pieces.size(); i++) {
						Vector3 a=_my_pieces[i]->get_second_global_position();
						for (int j=0; j<_my_pieces.size(); j++) {
							if (i!=j) {
								Vector3 b=_my_pieces[j]->get_second_global_position();
								metric+=(a-b).length();
								count+=1;
							}
						}
					}
					metric/=count;
					if (abs(metric-first_call_metric)<.1) {
						for (int i=0; i<DEFAULT_SIZE; i++) {
							//already_visited_table.clear();
							already_visited_table[i]=0;
						}
						already_visited_table_size=0;
						#ifdef DEBUG_FIND_SPOT
						printf("line %d with metric %f vs first_call_metric %f\n", __LINE__, metric, first_call_metric);
						#endif
					}
				}
			}
		}
		
		Node3D *best_node=_my_pieces[0];
		//std::unordered_map<int, Node3D*> piece_positions;
		
		//if (piece_positions.size()<my_pieces.size()) { //I know that piece_positions has not been set up yet.
		//	Node3D* piece=nullptr;
			
		//} //end of if statement (for if piece_positions has not been set up yet)
		
		//using fast_board_value instead of fast_move_value (both have nearly identical internals) because fast_board_value is one level lower in the decision tree, thus skipping a level of precision, thus making the AI
		//theoretically a couple orders of magnitude faster, even if it makes the AI slightly dumber.
		//after testing, the AI still doesn't find a move even if it is given 30 minutes (I had to go downstairs for dinner, so I couldn't just leave it running; this is when it is running at 100% CPU use, no file IO stoppages.
		Vector3 *best_move=&fast_make_move_best_move;
		//fast_move_value caused a segmentation fault, so besides just being an order of magnitude or two slower, fast_move_value also just doesn't work.
		//I think the better method of making the AI smarter is to have additional points towards the human possible moves that correspond to the values of pieces those human moves are capturing out of AI pieces.
		//this would be done inside the value_from_state function.
		float ai_v=-20000;
		if (true || nth_optimal!=0) {
			
			Node3D *res_piece=nullptr;
			float cur_ai_v=-10000;
			Vector3 atkdpst=Vector3_ZERO;
			for (int i=0; i<_my_pieces.size(); i+=1) {
				
				Node3D *to_move=_my_pieces[i];
				Vector3 start=to_move->get_second_global_position();
				for (int j=0; j<possible_positions.size(); j+=1) {
					//println(__LINE__); //this is where execution seemed to have ended, which is weird, since all I changed from the run where it reached 2202 was just adding in the 2167 println and the best_node==nullptr and res_piece==nullptr tests.
					Vector3 spot=possible_positions[j];
					Vector3 end=spot;
					if (already_visited(to_move, end) ) {
						continue; //do not check this loop iteration
					} else if (fast_can_move_to(to_move, start, end, _my_pieces, my_king_start,  _human_pieces,  human_king_start, accepted_positions)) {
						cur_ai_v=(float)(fast_board_value(to_move, end, _my_pieces, my_king_start,  _human_pieces,  human_king_start,  _possible_positions, accepted_positions));
						//AI specific condition, the human uses <
						if ((cur_ai_v>ai_v || best_node==nullptr) ) {
							//println(__LINE__);
							char temp_str[]="The replaced move for AI had an evaluation of ";
							char temp_str2[]="The new move for the  AI had an evaluation of ";
							char temp_str3[]="The replaced move used a piece with the following x coordinate and next given word as its name, followed in turn by its y coordinate ";
							char temp_str4[]="With a z coordinate of ";
							char temp_str5[]="With an arrival location x coordinate of ";
							char temp_str6[]="With an arrival location z coordinate of ";
							char temp_str7[]="The new move for the AI used a piece with the following x coordinate and next given word as its name, followed in turn by its y coordinate ";

							print_word_num_on_line(temp_str, (int)ai_v, __LINE__); 
							print_word_num_on_line(temp_str2, (int)cur_ai_v, __LINE__-1); //to put in same debug file.
							if (res_piece!=nullptr) {
								print_word_num_on_line(temp_str3, (int)(res_piece->get_second_global_position().x), __LINE__-3);
								//the "String(" operator function invented for Godot turns it into a Godot string (natively supporting UTF-32 information)
								// ".ascii()" turns a Godot string into a Godot CharString class instance, which has inside of itself a pointer to the information.
								print_word_num_on_line(String(res_piece->get_name()).ascii().ptrw(), (int)(res_piece->get_second_global_position().y), __LINE__-6);
								print_word_num_on_line(temp_str4, (int)(res_piece->get_second_global_position().z), __LINE__-7);
								print_word_num_on_line(temp_str5, (int)(best_move[0].x), __LINE__-8);
								print_word_num_on_line(temp_str6, (int)(best_move[0].z), __LINE__-9);
							}
							print_word_num_on_line(temp_str7, (int)(to_move->get_second_global_position().x), __LINE__-11);
							print_word_num_on_line(String(to_move->get_name()).ascii().ptrw(), (int)(to_move->get_second_global_position().y), __LINE__-12);
								print_word_num_on_line(temp_str4, (int)(to_move->get_second_global_position().z), __LINE__-13);
								print_word_num_on_line(temp_str5, (int)(spot.x), __LINE__-14);
								print_word_num_on_line(temp_str6, (int)(spot.z), __LINE__-15);
							
							ai_v=cur_ai_v;
							res_piece=to_move;
							atkdpst=spot;
							best_move[0]=atkdpst;
							best_node=res_piece;
						}
					}
				}
			}
			best_move[0]=atkdpst;
			best_node=res_piece;
		} 
		/*
		else {
			//if this is only happening once, then it is fine in terms of taking an order of magnitude more time, probably.
			//I know that with the failures of the c++ code in figuring out what moves are acceptable, this line of code will take longer than expected, and will not solve as many issues.
			//nor will it be as smart as it could otherwise be.
			best_move=fast_move_value(&best_node, _my_pieces, my_king_start,  _human_pieces,  human_king_start, piece_positions, temporary_piece_positions, _possible_positions, accepted_positions);
		}
		*/ printf("line 2203 has value of move at %f\t\n", ai_v);
		float move_value=ai_v;
		best_move[0].y=0;
		
		
		println(__LINE__); //this line was called.
		//best_node->set_place_loc(best_move);
		//the error could be that best_node is nullptr.
		if (best_node==nullptr) {
			println(__LINE__); //should theoretically be impossible.
			//turns out that best_node was null all along!
		} else {
			best_node->place_loc=best_move;
		}
		//note: prev_move will either be nullptr, or it will be an alias of best_move, just from how the memory is assigned.
		println(__LINE__); //this line was not called. Apparently, I guess best_move is not an appropriate value, or somehow becomes like null, or there is some other error within that function.
		
		
		#ifdef DEBUG_FIND_SPOT
							FILE *fp=fopen("debug_find_spot.txt", "a");
		Vector3 start=best_node->get_second_global_position();
							fprintf(fp, "line %d called with move value %f\tres_piece %s\tstart (%f, %f, %f)\tend (%f, %f, %f)\n", __LINE__, move_value, String(best_node->get_name()).ascii().ptr(), start.x, start.y, start.z, best_move[0].x, best_move[0].y, best_move[0].z);
							fclose(fp);
							#endif
		if (recurse_level==0)
		set_already_visited(best_node, best_move[0]);
		return best_node;
	
}

//End of Evan Nibbe's major edit to run a C++ function to do the calculations for how the AI should move Triangle Chess pieces.

void OmniLight3D::set_shadow_mode(ShadowMode p_mode) {
	shadow_mode = p_mode;
	RS::get_singleton()->light_omni_set_shadow_mode(light, RS::LightOmniShadowMode(p_mode));
}

OmniLight3D::ShadowMode OmniLight3D::get_shadow_mode() const {
	return shadow_mode;
}

PackedStringArray OmniLight3D::get_configuration_warnings() const {
	PackedStringArray warnings = Light3D::get_configuration_warnings();

	if (!has_shadow() && get_projector().is_valid()) {
		warnings.push_back(RTR("Projector texture only works with shadows active."));
	}

	if (get_projector().is_valid() && OS::get_singleton()->get_current_rendering_method() == "gl_compatibility") {
		warnings.push_back(RTR("Projector textures are not supported when using the GL Compatibility backend yet. Support will be added in a future release."));
	}

	return warnings;
}

void OmniLight3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_shadow_mode", "mode"), &OmniLight3D::set_shadow_mode);
	ClassDB::bind_method(D_METHOD("fast_make_move", "my_pieces", "my_king_start_x", "my_king_start_y", "my_king_start_z", "human_pieces", "human_king_start_x", "human_king_start_y", "human_king_start_z", "possible_positions", "nth_optimal", "recurse_level"), &OmniLight3D::fast_make_move);
	ClassDB::bind_method(D_METHOD("get_shadow_mode"), &OmniLight3D::get_shadow_mode);

	ADD_GROUP("Omni", "omni_");
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "omni_range", PROPERTY_HINT_RANGE, "0,4096,0.001,or_greater,exp"), "set_param", "get_param", PARAM_RANGE);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "omni_attenuation", PROPERTY_HINT_EXP_EASING, "attenuation"), "set_param", "get_param", PARAM_ATTENUATION);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "omni_shadow_mode", PROPERTY_HINT_ENUM, "Dual Paraboloid,Cube"), "set_shadow_mode", "get_shadow_mode");

	BIND_ENUM_CONSTANT(SHADOW_DUAL_PARABOLOID);
	BIND_ENUM_CONSTANT(SHADOW_CUBE);
}

OmniLight3D::OmniLight3D() :
		Light3D(RenderingServer::LIGHT_OMNI) {
	set_shadow_mode(SHADOW_CUBE);
}

PackedStringArray SpotLight3D::get_configuration_warnings() const {
	PackedStringArray warnings = Light3D::get_configuration_warnings();

	if (has_shadow() && get_param(PARAM_SPOT_ANGLE) >= 90.0) {
		warnings.push_back(RTR("A SpotLight3D with an angle wider than 90 degrees cannot cast shadows."));
	}

	if (!has_shadow() && get_projector().is_valid()) {
		warnings.push_back(RTR("Projector texture only works with shadows active."));
	}

	if (get_projector().is_valid() && OS::get_singleton()->get_current_rendering_method() == "gl_compatibility") {
		warnings.push_back(RTR("Projector textures are not supported when using the GL Compatibility backend yet. Support will be added in a future release."));
	}

	return warnings;
}

void SpotLight3D::_bind_methods() {
	ADD_GROUP("Spot", "spot_");
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "spot_range", PROPERTY_HINT_RANGE, "0,4096,0.001,or_greater,exp,suffix:m"), "set_param", "get_param", PARAM_RANGE);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "spot_attenuation", PROPERTY_HINT_EXP_EASING, "attenuation"), "set_param", "get_param", PARAM_ATTENUATION);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "spot_angle", PROPERTY_HINT_RANGE, "0,180,0.01,degrees"), "set_param", "get_param", PARAM_SPOT_ANGLE);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "spot_angle_attenuation", PROPERTY_HINT_EXP_EASING, "attenuation"), "set_param", "get_param", PARAM_SPOT_ATTENUATION);
}

SpotLight3D::SpotLight3D() :
		Light3D(RenderingServer::LIGHT_SPOT) {
	// Decrease the default shadow bias to better suit most scenes.
	set_param(PARAM_SHADOW_BIAS, 0.03);
}
