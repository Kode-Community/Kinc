#pragma once

#include <kinc/global.h>

#include "usage.h"

#include <kinc/backend/graphics4/indexbuffer.h>

/*! \file indexbuffer.h
    \brief Provides functions for setting up and using index-buffers.
*/

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kinc_g4_index_buffer_format { KINC_G4_INDEX_BUFFER_FORMAT_32BIT, KINC_G4_INDEX_BUFFER_FORMAT_16BIT } kinc_g4_index_buffer_format_t;

typedef struct kinc_g4_index_buffer {
	kinc_g4_index_buffer_impl_t impl;
} kinc_g4_index_buffer_t;

/// <summary>
/// Initializes an index-buffer.
/// </summary>
/// <param name="buffer">The buffer to initialize</param>
/// <param name="count">The number of indices to allocate for the buffer</param>
/// <param name="format">The integer-format of the buffer</param>
/// <param name="usage">A hint for how the buffer will be used</param>
KINC_FUNC void kinc_g4_index_buffer_init(kinc_g4_index_buffer_t *buffer, int count, kinc_g4_index_buffer_format_t format, kinc_g4_usage_t usage);

/// <summary>
/// Destroys an index-buffer.
/// </summary>
/// <param name="buffer">The buffer to destroy</param>
KINC_FUNC void kinc_g4_index_buffer_destroy(kinc_g4_index_buffer_t *buffer);

/// <summary>
/// Locks an index-buffer so its contents can be modified.
/// </summary>
/// <param name="buffer">The buffer to lock</param>
/// <returns>The contents of the index-buffer</returns>
KINC_FUNC int *kinc_g4_index_buffer_lock(kinc_g4_index_buffer_t *buffer);

/// <summary>
/// Unlocks an index-buffer after locking it so the changed buffer-contents can be used.
/// </summary>
/// <param name="buffer">The buffer to unlock</param>
KINC_FUNC void kinc_g4_index_buffer_unlock(kinc_g4_index_buffer_t *buffer);

/// <summary>
/// Returns the number of indices in the buffer.
/// </summary>
/// <param name="buffer">The buffer to query for its number of indices</param>
/// <returns>The number of indices</returns>
KINC_FUNC int kinc_g4_index_buffer_count(kinc_g4_index_buffer_t *buffer);

void kinc_internal_g4_index_buffer_set(kinc_g4_index_buffer_t *buffer);

/// <summary>
/// Sets an index-buffer to be used for the next draw-command.
/// </summary>
/// <param name="buffer">The buffer to use</param>
KINC_FUNC void kinc_g4_set_index_buffer(kinc_g4_index_buffer_t *buffer);

#ifdef __cplusplus
}
#endif
