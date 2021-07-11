#pragma once

#include <kinc/global.h>

#include <kinc/backend/graphics4/rendertarget.h>

#include "textureunit.h"

#include <stdint.h>

/*! \file rendertarget.h
    \brief Provides functions for handling render-targets.
*/

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kinc_g4_render_target_format {
	KINC_G4_RENDER_TARGET_FORMAT_32BIT,
	KINC_G4_RENDER_TARGET_FORMAT_64BIT_FLOAT,
	KINC_G4_RENDER_TARGET_FORMAT_32BIT_RED_FLOAT,
	KINC_G4_RENDER_TARGET_FORMAT_128BIT_FLOAT,
	KINC_G4_RENDER_TARGET_FORMAT_16BIT_DEPTH,
	KINC_G4_RENDER_TARGET_FORMAT_8BIT_RED,
	KINC_G4_RENDER_TARGET_FORMAT_16BIT_RED_FLOAT
} kinc_g4_render_target_format_t;

typedef struct kinc_g4_render_target {
	int width;
	int height;
	int texWidth;
	int texHeight;
	int contextId;
	bool isCubeMap;
	bool isDepthAttachment;

	kinc_g4_render_target_impl_t impl;
} kinc_g4_render_target_t;

/// <summary>
/// Allocates and initializes a regular render-target.
/// </summary>
/// <param name="renderTarget"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="depthBufferBits"></param>
/// <param name="antialiasing"></param>
/// <param name="format"></param>
/// <param name="stencilBufferBits"></param>
/// <param name="contextId"></param>
KINC_FUNC void kinc_g4_render_target_init(kinc_g4_render_target_t *renderTarget, int width, int height, int depthBufferBits, bool antialiasing,
                                          kinc_g4_render_target_format_t format, int stencilBufferBits, int contextId);

/// <summary>
/// Allocates and initializes a render-target-cube-map.
/// </summary>
/// <param name="renderTarget"></param>
/// <param name="cubeMapSize"></param>
/// <param name="depthBufferBits"></param>
/// <param name="antialiasing"></param>
/// <param name="format"></param>
/// <param name="stencilBufferBits"></param>
/// <param name="contextId"></param>
KINC_FUNC void kinc_g4_render_target_init_cube(kinc_g4_render_target_t *renderTarget, int cubeMapSize, int depthBufferBits, bool antialiasing,
                                               kinc_g4_render_target_format_t format, int stencilBufferBits, int contextId);

/// <summary>
/// Deallocates and destroys a render-target.
/// </summary>
/// <param name="renderTarget">The render-target to destroy</param>
KINC_FUNC void kinc_g4_render_target_destroy(kinc_g4_render_target_t *renderTarget);

/// <summary>
/// Uses the color-component of a render-target as a texture.
/// </summary>
/// <param name="renderTarget">The render-target to use</param>
/// <param name="unit">The texture-unit to assign the render-target to</param>
KINC_FUNC void kinc_g4_render_target_use_color_as_texture(kinc_g4_render_target_t *renderTarget, kinc_g4_texture_unit_t unit);

/// <summary>
/// Uses the depth-component of a render-target as a texture.
/// </summary>
/// <param name="renderTarget">The render-target to use</param>
/// <param name="unit">The texture-unit to assign the render-target to</param>
KINC_FUNC void kinc_g4_render_target_use_depth_as_texture(kinc_g4_render_target_t *renderTarget, kinc_g4_texture_unit_t unit);

/// <summary>
/// Copies the depth and stencil-components of one render-target into another one.
/// </summary>
/// <param name="renderTarget">The render-target to copy the data into</param>
/// <param name="source">The render-target from which to copy the data</param>
/// <returns></returns>
KINC_FUNC void kinc_g4_render_target_set_depth_stencil_from(kinc_g4_render_target_t *renderTarget, kinc_g4_render_target_t *source);

/// <summary>
/// Copies out the color-data from a render-target. Beware, this is very slow.
/// </summary>
/// <param name="renderTarget">The render-target to copy the color-data from</param>
/// <param name="data">A pointer to where the data will be copied to</param>
KINC_FUNC void kinc_g4_render_target_get_pixels(kinc_g4_render_target_t *renderTarget, uint8_t *data);

/// <summary>
/// Generates the mipmap-chain for a render-target.
/// </summary>
/// <param name="renderTarget">The render-target to create the mipmaps for</param>
/// <param name="levels">The number of mipmap-levels to generate</param>
KINC_FUNC void kinc_g4_render_target_generate_mipmaps(kinc_g4_render_target_t *renderTarget, int levels);

#ifdef __cplusplus
}
#endif
