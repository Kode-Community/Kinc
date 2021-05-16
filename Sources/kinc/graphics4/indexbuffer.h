#pragma once

#include <kinc/global.h>

#include <kinc/backend/graphics4/indexbuffer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kinc_g4_index_buffer_format { KINC_G4_INDEX_BUFFER_FORMAT_32BIT, KINC_G4_INDEX_BUFFER_FORMAT_16BIT } kinc_g4_index_buffer_format_t;

typedef struct kinc_g4_index_buffer {
	kinc_g4_index_buffer_impl_t impl;
} kinc_g4_index_buffer_t;

KINC_FUNC void kinc_g4_index_buffer_init(kinc_g4_index_buffer_t *buffer, int count, kinc_g4_index_buffer_format_t format);
KINC_FUNC void kinc_g4_index_buffer_destroy(kinc_g4_index_buffer_t *buffer);
KINC_FUNC int *kinc_g4_index_buffer_lock(kinc_g4_index_buffer_t *buffer);
KINC_FUNC void kinc_g4_index_buffer_unlock(kinc_g4_index_buffer_t *buffer);
KINC_FUNC int kinc_g4_index_buffer_count(kinc_g4_index_buffer_t *buffer);

void kinc_internal_g4_index_buffer_set(kinc_g4_index_buffer_t *buffer);

KINC_FUNC void kinc_g4_set_index_buffer(kinc_g4_index_buffer_t *buffer);

#ifdef __cplusplus
}
#endif
