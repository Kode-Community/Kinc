#pragma once

#include <kinc/global.h>

#include <kinc/image.h>

#include "rendertarget.h"
#include "shader.h"
#include "texture.h"
#include "textureunit.h"
#include "vertexstructure.h"

#include <kinc/backend/graphics5/graphics.h>

#include <kinc/math/matrix.h>
#include <kinc/math/vector.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kinc_g5_texture_addressing {
	KINC_G5_TEXTURE_ADDRESSING_REPEAT,
	KINC_G5_TEXTURE_ADDRESSING_MIRROR,
	KINC_G5_TEXTURE_ADDRESSING_CLAMP,
	KINC_G5_TEXTURE_ADDRESSING_BORDER
} kinc_g5_texture_addressing_t;

typedef enum kinc_g5_texture_filter {
	KINC_G5_TEXTURE_FILTER_POINT,
	KINC_G5_TEXTURE_FILTER_LINEAR,
	KINC_G5_TEXTURE_FILTER_ANISOTROPIC
} kinc_g5_texture_filter_t;

typedef enum kinc_g5_mipmap_filter {
	KINC_G5_MIPMAP_FILTER_NONE,
	KINC_G5_MIPMAP_FILTER_POINT,
	KINC_G5_MIPMAP_FILTER_LINEAR // linear texture filter + linear mip filter -> trilinear filter
} kinc_g5_mipmap_filter_t;

typedef enum kinc_g5_blending_operation {
	KINC_G5_BLEND_MODE_ONE,
	KINC_G5_BLEND_MODE_ZERO,
	KINC_G5_BLEND_MODE_SOURCE_ALPHA,
	KINC_G5_BLEND_MODE_DEST_ALPHA,
	KINC_G5_BLEND_MODE_INV_SOURCE_ALPHA,
	KINC_G5_BLEND_MODE_INV_DEST_ALPHA,
	KINC_G5_BLEND_MODE_SOURCE_COLOR,
	KINC_G5_BLEND_MODE_DEST_COLOR,
	KINC_G5_BLEND_MODE_INV_SOURCE_COLOR,
	KINC_G5_BLEND_MODE_INV_DEST_COLOR
} kinc_g5_blending_operation_t;

typedef enum kinc_g5_compare_mode {
	KINC_G5_COMPARE_MODE_ALWAYS,
	KINC_G5_COMPARE_MODE_NEVER,
	KINC_G5_COMPARE_MODE_EQUAL,
	KINC_G5_COMPARE_MODE_NOT_EQUAL,
	KINC_G5_COMPARE_MODE_LESS,
	KINC_G5_COMPARE_MODE_LESS_EQUAL,
	KINC_G5_COMPARE_MODE_GREATER,
	KINC_G5_COMPARE_MODE_GREATER_EQUAL
} kinc_g5_compare_mode_t;

typedef enum kinc_g5_cull_mode { KINC_G5_CULL_MODE_CLOCKWISE, KINC_G5_CULL_MODE_COUNTERCLOCKWISE, KINC_G5_CULL_MODE_NEVER } kinc_g5_cull_mode_t;

typedef enum kinc_g5_texture_direction { KINC_G5_TEXTURE_DIRECTION_U, KINC_G5_TEXTURE_DIRECTION_V, KINC_G5_TEXTURE_DIRECTION_W } kinc_g5_texture_direction_t;

/*typedef enum kinc_g5_render_target_format {
    KINC_G5_RENDER_TARGET_FORMAT_32BIT,
    KINC_G5_RENDER_TARGET_FORMAT_64BIT_FLOAT,
    KINC_G5_RENDER_TARGET_FORMAT_32BIT_RED_FLOAT,
    KINC_G5_RENDER_TARGET_FORMAT_128BIT_FLOAT,
    KINC_G5_RENDER_TARGET_FORMAT_16BIT_DEPTH,
    KINC_G5_RENDER_TARGET_FORMAT_8BIT_RED
} kinc_g5_render_target_format_t;*/
// typedef kinc_g4_render_target_format_t kinc_g5_render_target_format_t;

typedef enum kinc_g5_stencil_action {
	KINC_G5_STENCIL_ACTION_KEEP,
	KINC_G5_STENCIL_ACTION_ZERO,
	KINC_G5_STENCIL_ACTION_REPLACE,
	KINC_G5_STENCIL_ACTION_INCREMENT,
	KINC_G5_STENCIL_ACTION_INCREMENT_WRAP,
	KINC_G5_STENCIL_ACTION_DECREMENT,
	KINC_G5_STENCIL_ACTION_DECREMENT_WRAP,
	KINC_G5_STENCIL_ACTION_INVERT
} kinc_g5_stencil_action_t;

typedef enum kinc_g5_texture_operation {
	KINC_G5_TEXTURE_OPERATION_MODULATE,
	KINC_G5_TEXTURE_OPERATION_SELECT_FIRST,
	KINC_G5_TEXTURE_OPERATION_SELECT_SECOND
} kinc_g5_texture_operation_t;

typedef enum kinc_g5_texture_argument { KINC_G5_TEXTURE_ARGUMENT_CURRENT_COLOR, KINC_G5_TEXTURE_ARGUMENT_TEXTURE_COLOR } kinc_g5_texture_argument_t;

#define KINC_G5_CLEAR_COLOR 1
#define KINC_G5_CLEAR_DEPTH 2
#define KINC_G5_CLEAR_STENCIL 4

KINC_FUNC void kinc_g5_init(int window, int depthBufferBits, int stencilBufferBits, bool vsync);
KINC_FUNC void kinc_g5_destroy(int window);

KINC_FUNC extern bool kinc_g5_fullscreen;

KINC_FUNC void kinc_g5_flush(void);

KINC_FUNC void kinc_g5_set_texture(kinc_g5_texture_unit_t unit, kinc_g5_texture_t *texture);
KINC_FUNC void kinc_g5_set_image_texture(kinc_g5_texture_unit_t unit, kinc_g5_texture_t *texture);

KINC_FUNC void kinc_g5_draw_indexed_vertices_instanced(int instanceCount);
KINC_FUNC void kinc_g5_draw_indexed_vertices_instanced_from_to(int instanceCount, int start, int count);

KINC_FUNC int kinc_g5_antialiasing_samples(void);
KINC_FUNC void kinc_g5_set_antialiasing_samples(int samples);

KINC_FUNC bool kinc_g5_non_pow2_textures_qupported(void);
KINC_FUNC bool kinc_g5_render_targets_inverted_y(void);
KINC_FUNC void kinc_g5_set_render_target_face(kinc_g5_render_target_t *texture, int face);

KINC_FUNC void kinc_g5_begin(kinc_g5_render_target_t *renderTarget, int window);
KINC_FUNC void kinc_g5_end(int window);
KINC_FUNC bool kinc_g5_swap_buffers(void);

void kinc_internal_g5_resize(int window, int width, int height);

KINC_FUNC void kinc_g5_set_texture_addressing(kinc_g5_texture_unit_t unit, kinc_g5_texture_direction_t dir, kinc_g5_texture_addressing_t addressing);
KINC_FUNC void kinc_g5_set_texture_magnification_filter(kinc_g5_texture_unit_t texunit, kinc_g5_texture_filter_t filter);
KINC_FUNC void kinc_g5_set_texture_minification_filter(kinc_g5_texture_unit_t texunit, kinc_g5_texture_filter_t filter);
KINC_FUNC void kinc_g5_set_texture_mipmap_filter(kinc_g5_texture_unit_t texunit, kinc_g5_mipmap_filter_t filter);
KINC_FUNC void kinc_g5_set_texture_operation(kinc_g5_texture_operation_t operation, kinc_g5_texture_argument_t arg1, kinc_g5_texture_argument_t arg2);
KINC_FUNC int kinc_g5_max_bound_textures(void);

// Occlusion Query
KINC_FUNC bool kinc_g5_init_occlusion_query(unsigned *occlusionQuery);
KINC_FUNC void kinc_g5_delete_occlusion_query(unsigned occlusionQuery);
KINC_FUNC void kinc_g5_render_occlusion_query(unsigned occlusionQuery, int triangles);
KINC_FUNC bool kinc_g5_are_query_results_available(unsigned occlusionQuery);
KINC_FUNC void kinc_g5_get_query_result(unsigned occlusionQuery, unsigned *pixelCount);

#ifdef __cplusplus
}
#endif
