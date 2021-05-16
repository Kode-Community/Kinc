#pragma once

#include <kinc/global.h>

#include <kinc/backend/graphics5/pipeline.h>

#include <kinc/graphics5/vertexstructure.h>

#include "constantlocation.h"
#include "graphics.h"

#ifdef __cplusplus
extern "C" {
#endif

struct kinc_g5_shader;

typedef struct kinc_g5_pipeline {
	kinc_g5_vertex_structure_t *inputLayout[16];
	struct kinc_g5_shader *vertexShader;
	struct kinc_g5_shader *fragmentShader;
	struct kinc_g5_shader *geometryShader;
	struct kinc_g5_shader *tessellationControlShader;
	struct kinc_g5_shader *tessellationEvaluationShader;

	kinc_g5_cull_mode_t cullMode;

	bool depthWrite;
	kinc_g5_compare_mode_t depthMode;

	kinc_g5_compare_mode_t stencilMode;
	kinc_g5_stencil_action_t stencilBothPass;
	kinc_g5_stencil_action_t stencilDepthFail;
	kinc_g5_stencil_action_t stencilFail;
	int stencilReferenceValue;
	int stencilReadMask;
	int stencilWriteMask;

	// One, Zero deactivates blending
	kinc_g5_blending_operation_t blendSource;
	kinc_g5_blending_operation_t blendDestination;
	// BlendingOperation blendOperation;
	kinc_g5_blending_operation_t alphaBlendSource;
	kinc_g5_blending_operation_t alphaBlendDestination;
	// BlendingOperation alphaBlendOperation;

	bool colorWriteMaskRed[8]; // Per render target
	bool colorWriteMaskGreen[8];
	bool colorWriteMaskBlue[8];
	bool colorWriteMaskAlpha[8];

	int colorAttachmentCount;
	kinc_g5_render_target_format_t colorAttachment[8];

	int depthAttachmentBits;
	int stencilAttachmentBits;

	bool conservativeRasterization;

	PipelineState5Impl impl;
} kinc_g5_pipeline_t;

typedef struct kinc_g5_compute_pipeline {
	struct kinc_g5_shader *compute_shader;

	ComputePipelineState5Impl impl;
} kinc_g5_compute_pipeline_t;

KINC_FUNC void kinc_g5_pipeline_init(kinc_g5_pipeline_t *pipeline);
void kinc_g5_internal_pipeline_init(kinc_g5_pipeline_t *pipeline);
KINC_FUNC void kinc_g5_pipeline_destroy(kinc_g5_pipeline_t *pipeline);
KINC_FUNC void kinc_g5_pipeline_compile(kinc_g5_pipeline_t *pipeline);
KINC_FUNC kinc_g5_constant_location_t kinc_g5_pipeline_get_constant_location(kinc_g5_pipeline_t *pipeline, const char *name);
KINC_FUNC kinc_g5_texture_unit_t kinc_g5_pipeline_get_texture_unit(kinc_g5_pipeline_t *pipeline, const char *name);

KINC_FUNC void kinc_g5_compute_pipeline_init(kinc_g5_compute_pipeline_t *pipeline);
void kinc_g5_internal_compute_pipeline_init(kinc_g5_compute_pipeline_t *pipeline);
KINC_FUNC void kinc_g5_compute_pipeline_destroy(kinc_g5_compute_pipeline_t *pipeline);
KINC_FUNC void kinc_g5_compute_pipeline_compile(kinc_g5_compute_pipeline_t *pipeline);
KINC_FUNC kinc_g5_constant_location_t kinc_g5_compute_pipeline_get_constant_location(kinc_g5_compute_pipeline_t *pipeline, const char *name);
KINC_FUNC kinc_g5_texture_unit_t kinc_g5_compute_pipeline_get_texture_unit(kinc_g5_compute_pipeline_t *pipeline, const char *name);

#ifdef __cplusplus
}
#endif
