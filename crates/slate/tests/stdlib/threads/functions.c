#include <stdatomic.h>
#include <stdio.h>
#include <threads.h>
#include <time.h>

static once_flag once_control = ONCE_FLAG_INIT;
static mtx_t signal_mutex;
static cnd_t signal_condition;
static tss_t storage_key;
static int ready;
static int once_calls;
static int destructor_calls;
static _Atomic int detached_done;

struct worker_state {
  int value;
  int tss_set_status;
  int tss_matches;
  int lock_status;
  int signal_status;
  int unlock_status;
};

static void initialize_once(void) { once_calls++; }

static void destroy_storage(void *value) {
  if (value != NULL)
    destructor_calls++;
}

static int signaling_worker(void *argument) {
  struct worker_state *state = argument;
  call_once(&once_control, initialize_once);
  call_once(&once_control, initialize_once);
  state->tss_set_status = tss_set(storage_key, &state->value);
  state->tss_matches = tss_get(storage_key) == &state->value;
  state->lock_status = mtx_lock(&signal_mutex);
  ready = 1;
  state->signal_status = cnd_signal(&signal_condition);
  state->unlock_status = mtx_unlock(&signal_mutex);
  thrd_yield();
  return 17;
}

static int exiting_worker(void *argument) {
  int result = *(int *)argument;
  thrd_exit(result);
}

static int detached_worker(void *argument) {
  int result = *(int *)argument;
  atomic_store(&detached_done, 1);
  return result;
}

int main(void) {
  struct worker_state state = {.value = 7};
  thrd_t signaling_thread;
  thrd_t exiting_thread;
  thrd_t detached_thread;
  int thread_result = 0;
  int exit_result = 0;
  int exit_value = 29;
  int detached_value = 31;
  int wait_status = thrd_success;
  struct timespec deadline;
  struct timespec duration = {.tv_sec = 0, .tv_nsec = 0};
  struct timespec remaining = {.tv_sec = 0, .tv_nsec = 0};

  printf("constants %d %d %d %d %d %d %d %d %d\n", thrd_success, thrd_timedout,
         thrd_busy, thrd_nomem, thrd_error, mtx_plain, mtx_recursive, mtx_timed,
         TSS_DTOR_ITERATIONS);
  printf("thrd_equal %d\n", thrd_equal(thrd_current(), thrd_current()) ? 1 : 0);
  printf("tss_create %d\n", tss_create(&storage_key, destroy_storage));
  printf("mtx_init plain %d\n", mtx_init(&signal_mutex, mtx_plain));
  printf("cnd_init %d\n", cnd_init(&signal_condition));
  int create_status = thrd_create(&signaling_thread, signaling_worker, &state);
  printf("thrd_create signaling %d\n", create_status);
  printf("mtx_lock wait %d\n", mtx_lock(&signal_mutex));
  while (!ready && wait_status == thrd_success)
    wait_status = cnd_wait(&signal_condition, &signal_mutex);
  printf("cnd_wait %d\n", wait_status);
  printf("mtx_unlock wait %d\n", mtx_unlock(&signal_mutex));
  printf("thrd_join signaling %d\n",
         thrd_join(signaling_thread, &thread_result));
  printf("worker %d %d %d %d %d %d\n", state.tss_set_status, state.tss_matches,
         state.lock_status, state.signal_status, state.unlock_status,
         thread_result);
  printf("call_once %d\n", once_calls);
  printf("tss_destructor %d\n", destructor_calls);
  printf("cnd_broadcast %d\n", cnd_broadcast(&signal_condition));

  timespec_get(&deadline, TIME_UTC);
  deadline.tv_sec -= 1;
  printf("mtx_lock timedwait %d\n", mtx_lock(&signal_mutex));
  printf("cnd_timedwait %d\n",
         cnd_timedwait(&signal_condition, &signal_mutex, &deadline));
  printf("mtx_unlock timedwait %d\n", mtx_unlock(&signal_mutex));
  cnd_destroy(&signal_condition);
  printf("cnd_destroy 1\n");
  mtx_destroy(&signal_mutex);
  printf("mtx_destroy plain 1\n");

  mtx_t try_mutex;
  printf("mtx_init try %d\n", mtx_init(&try_mutex, mtx_plain));
  printf("mtx_trylock %d\n", mtx_trylock(&try_mutex));
  printf("mtx_unlock try %d\n", mtx_unlock(&try_mutex));
  mtx_destroy(&try_mutex);
  printf("mtx_destroy try 1\n");

  mtx_t recursive_mutex;
  printf("mtx_init recursive %d\n", mtx_init(&recursive_mutex, mtx_recursive));
  printf("mtx_lock recursive first %d\n", mtx_lock(&recursive_mutex));
  printf("mtx_lock recursive second %d\n", mtx_lock(&recursive_mutex));
  printf("mtx_unlock recursive first %d\n", mtx_unlock(&recursive_mutex));
  printf("mtx_unlock recursive second %d\n", mtx_unlock(&recursive_mutex));
  mtx_destroy(&recursive_mutex);
  printf("mtx_destroy recursive 1\n");

  mtx_t timed_mutex;
  printf("mtx_init timed %d\n", mtx_init(&timed_mutex, mtx_timed));
  timespec_get(&deadline, TIME_UTC);
  deadline.tv_sec += 1;
  printf("mtx_timedlock %d\n", mtx_timedlock(&timed_mutex, &deadline));
  printf("mtx_unlock timed %d\n", mtx_unlock(&timed_mutex));
  mtx_destroy(&timed_mutex);
  printf("mtx_destroy timed 1\n");

  printf("thrd_sleep %d\n", thrd_sleep(&duration, &remaining));
  thrd_yield();
  printf("thrd_yield 1\n");

  printf("thrd_create exiting %d\n",
         thrd_create(&exiting_thread, exiting_worker, &exit_value));
  printf("thrd_join exiting %d\n", thrd_join(exiting_thread, &exit_result));
  printf("thrd_exit %d\n", exit_result);

  atomic_store(&detached_done, 0);
  printf("thrd_create detached %d\n",
         thrd_create(&detached_thread, detached_worker, &detached_value));
  printf("thrd_detach %d\n", thrd_detach(detached_thread));
  while (!atomic_load(&detached_done))
    thrd_yield();
  printf("detached_done %d\n", atomic_load(&detached_done));

  tss_delete(storage_key);
  printf("tss_delete 1\n");
  return 0;
}
