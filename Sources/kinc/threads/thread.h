#pragma once

#include <kinc/global.h>

#include <kinc/backend/thread.h>

/*! \file thread.h
    \brief Supports the creation and destruction of threads.
*/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kinc_thread {
	kinc_thread_impl_t impl;
} kinc_thread_t;

/// <summary>
/// Initializes the threading system. This has to be called before calling any kinc_thread functions.
/// </summary>
KINC_FUNC void kinc_threads_init(void);

/// <summary>
/// Shuts down the threading system.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
KINC_FUNC void kinc_threads_quit(void);

/// <summary>
/// Starts a thread via the provided function.
/// </summary>
/// <param name="thread">The thread-object to initialize with the new thread</param>
/// <param name="func">The function used to start a new thread</param>
/// <param name="param">A parameter that is passed to the thread-function when the thread starts running</param>
KINC_FUNC void kinc_thread_init(kinc_thread_t *thread, void (*func)(void *param), void *param);

/// <summary>
/// Waits for the thread to complete execution and then destroys it.
/// </summary>
/// <param name="thread">The thread to destroy</param>
KINC_FUNC void kinc_thread_wait_and_destroy(kinc_thread_t *thread);

/// <summary>
/// Attempts to destroy a thread.
/// </summary>
/// <param name="thread">The thread to destroy</param>
/// <returns>Returns if the thread is still running and therefore couldn't be destroyed</returns>
KINC_FUNC bool kinc_thread_try_to_destroy(kinc_thread_t *thread);

/// <summary>
/// Puts the current thread to sleep.
/// </summary>
/// <param name="milliseconds">How long to sleep</param>
KINC_FUNC void kinc_thread_sleep(int milliseconds);

#ifdef __cplusplus
}
#endif
