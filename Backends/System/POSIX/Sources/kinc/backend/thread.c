#include <kinc/threads/thread.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <unistd.h>

#if !defined(KORE_IOS) && !defined(KORE_MACOS)

struct thread_start {
	void (*thread)(void* param);
	void* param;
};

#define THREAD_STARTS 64
static struct thread_start starts[THREAD_STARTS];
static int thread_start_index = 0;

static void* ThreadProc(void* arg) {
	int start_index = (int)arg;
	starts[start_index].thread(starts[start_index].param);
	pthread_exit(NULL);
	return NULL;
}

void kinc_thread_init(kinc_thread_t *t, void (*thread)(void* param), void* param) {
	t->impl.param = param;
	t->impl.thread = thread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, 1024 * 64);
	struct sched_param sp;
	memset(&sp, 0, sizeof(sp));
	sp.sched_priority = 0;
	pthread_attr_setschedparam(&attr, &sp);
	int start_index = thread_start_index++;
	if (thread_start_index >= THREAD_STARTS) {
		thread_start_index = 0;
	}
	starts[start_index].thread = thread;
	starts[start_index].param = param;
	int ret = pthread_create(&t->impl.pthread, &attr, &ThreadProc, (void*)start_index);
	assert(ret == 0);
	pthread_attr_destroy(&attr);
}

void kinc_thread_wait_and_destroy(kinc_thread_t *thread) {
    int ret;
    do {
        ret = pthread_join(thread->impl.pthread, NULL);
    } while (ret != 0);
}

bool kinc_thread_try_to_destroy(kinc_thread_t *thread) {
    return pthread_join(thread->impl.pthread, NULL) == 0;
}

void kinc_threads_init() {

}

void kinc_threads_quit() {

}

#endif

void kinc_thread_sleep(int milliseconds) {
	usleep(1000 * (useconds_t)milliseconds);
}
