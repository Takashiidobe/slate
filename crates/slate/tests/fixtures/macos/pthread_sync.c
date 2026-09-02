#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <stddef.h>
#include <time.h>

_Static_assert(sizeof(pthread_t) == 8, "pthread_t");
_Static_assert(sizeof(pthread_key_t) == 8, "pthread_key_t");
_Static_assert(sizeof(pthread_attr_t) == 64, "pthread_attr_t");
_Static_assert(sizeof(pthread_mutex_t) == 64, "pthread_mutex_t");
_Static_assert(sizeof(pthread_mutexattr_t) == 16, "pthread_mutexattr_t");
_Static_assert(sizeof(pthread_cond_t) == 48, "pthread_cond_t");
_Static_assert(sizeof(pthread_condattr_t) == 16, "pthread_condattr_t");
_Static_assert(sizeof(pthread_once_t) == 16, "pthread_once_t");
_Static_assert(sizeof(pthread_rwlock_t) == 200, "pthread_rwlock_t");
_Static_assert(sizeof(pthread_rwlockattr_t) == 24, "pthread_rwlockattr_t");
_Static_assert(_Alignof(pthread_attr_t) == 8, "pthread attr alignment");
_Static_assert(_Alignof(pthread_mutex_t) == 8, "pthread mutex alignment");
_Static_assert(_Alignof(pthread_cond_t) == 8, "pthread cond alignment");
_Static_assert(_Alignof(pthread_once_t) == 8, "pthread once alignment");
_Static_assert(_Alignof(pthread_rwlock_t) == 8, "pthread rwlock alignment");
_Static_assert(offsetof(pthread_mutex_t, __opaque) == 8,
               "pthread mutex opaque");
_Static_assert(offsetof(pthread_rwlock_t, __opaque) == 8,
               "pthread rwlock opaque");
_Static_assert(sizeof(sem_t) == 4, "sem_t");
_Static_assert(sizeof(struct sched_param) == 8, "sched_param");

_Static_assert(PTHREAD_CREATE_JOINABLE == 1, "joinable");
_Static_assert(PTHREAD_CREATE_DETACHED == 2, "detached");
_Static_assert(PTHREAD_CANCEL_ENABLE == 1, "cancel enable");
_Static_assert(PTHREAD_CANCEL_DISABLE == 0, "cancel disable");
_Static_assert(PTHREAD_CANCEL_DEFERRED == 2, "cancel deferred");
_Static_assert(PTHREAD_CANCEL_ASYNCHRONOUS == 0, "cancel async");
_Static_assert(PTHREAD_PROCESS_SHARED == 1, "process shared");
_Static_assert(PTHREAD_PROCESS_PRIVATE == 2, "process private");
_Static_assert(PTHREAD_MUTEX_ERRORCHECK == 1, "errorcheck mutex");
_Static_assert(PTHREAD_MUTEX_RECURSIVE == 2, "recursive mutex");
_Static_assert(_PTHREAD_MUTEX_SIG_init == 0x32AAABA7,
               "mutex initializer signature");
_Static_assert(_PTHREAD_COND_SIG_init == 0x3CB0B1BB,
               "condition initializer signature");
_Static_assert(_PTHREAD_ONCE_SIG_init == 0x30B1BCBA,
               "once initializer signature");
_Static_assert(_PTHREAD_RWLOCK_SIG_init == 0x2DA8B3B4,
               "rwlock initializer signature");
_Static_assert(SCHED_OTHER == 1, "SCHED_OTHER");
_Static_assert(SCHED_RR == 2, "SCHED_RR");
_Static_assert(SCHED_FIFO == 4, "SCHED_FIFO");
_Static_assert(SEM_VALUE_MAX == 32767, "SEM_VALUE_MAX");

_Static_assert(__builtin_types_compatible_p(pthread_t,
                                            struct _opaque_pthread_t *),
               "pthread_t type");
_Static_assert(__builtin_types_compatible_p(pthread_key_t, unsigned long),
               "pthread_key_t type");
_Static_assert(
    __builtin_types_compatible_p(__typeof__(&pthread_create),
                                 int (*)(pthread_t *__restrict,
                                         const pthread_attr_t *__restrict,
                                         void *(*)(void *), void *__restrict)),
    "pthread_create signature");
_Static_assert(
    __builtin_types_compatible_p(__typeof__(&pthread_cond_timedwait),
                                 int (*)(pthread_cond_t *__restrict,
                                         pthread_mutex_t *__restrict,
                                         const struct timespec *__restrict)),
    "pthread_cond_timedwait signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&sem_open),
                                            sem_t *(*)(const char *, int, ...)),
               "sem_open signature");

pthread_mutex_t  slate_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t   slate_condition = PTHREAD_COND_INITIALIZER;
pthread_rwlock_t slate_rwlock    = PTHREAD_RWLOCK_INITIALIZER;
pthread_once_t   slate_once      = PTHREAD_ONCE_INIT;

void *slate_thread_entry(void *argument) { return argument; }

void slate_once_routine(void) {}

// @lowering-fn-begin
// @rewrite-fn-begin
int slate_create_and_join(pthread_t *thread, void *argument) {
  void *result = 0;
  return pthread_create(thread, 0, slate_thread_entry, argument) ||
         pthread_join(*thread, &result);
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int slate_wait_until(const struct timespec *deadline) {
  int result = pthread_mutex_lock(&slate_mutex);
  if (result == 0)
    result = pthread_cond_timedwait(&slate_condition, &slate_mutex, deadline);
  return pthread_mutex_unlock(&slate_mutex) || result;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int slate_read_lock(void) {
  return pthread_rwlock_rdlock(&slate_rwlock) ||
         pthread_rwlock_unlock(&slate_rwlock);
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int slate_tls_once(pthread_key_t *key, void *value) {
  return pthread_once(&slate_once, slate_once_routine) ||
         pthread_key_create(key, 0) || pthread_setspecific(*key, value);
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int slate_post_named_semaphore(const char *name) {
  sem_t *semaphore = sem_open(name, 0);
  return semaphore == SEM_FAILED || sem_post(semaphore) || sem_close(semaphore);
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int slate_yield(void) { return sched_yield(); }
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering-macos
// LOWERING-MACOS-DAG: unsafe fn slate_create_and_join(
// LOWERING-MACOS-DAG:     {{arg[0-9]+}}: *mut *mut _opaque_pthread_t,
// LOWERING-MACOS-DAG:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-MACOS-DAG: ) -> i32 {
// LOWERING-MACOS-DAG:     let mut thread: *mut *mut _opaque_pthread_t = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     let mut result: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     thread = {{arg[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     result = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut *mut _opaque_pthread_t = thread;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut _opaque_pthread_attr_t = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-MACOS-DAG:         pthread_create(
// LOWERING-MACOS-DAG:             {{_v[0-9]+}} as *mut *mut _opaque_pthread_t,
// LOWERING-MACOS-DAG:             {{_v[0-9]+}} as *const _opaque_pthread_attr_t,
// LOWERING-MACOS-DAG:             Some(slate_thread_entry),
// LOWERING-MACOS-DAG:             {{arg[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-MACOS-DAG:         )
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     } else {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut *mut _opaque_pthread_t = thread;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut _opaque_pthread_t = unsafe { *{{_v[0-9]+}} };
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-MACOS-DAG:             pthread_join(
// LOWERING-MACOS-DAG:                 {{_v[0-9]+}} as *mut _opaque_pthread_t,
// LOWERING-MACOS-DAG:                 std::ptr::addr_of_mut!(result) as *mut *mut core::ffi::c_void,
// LOWERING-MACOS-DAG:             )
// LOWERING-MACOS-DAG:         };
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-MACOS-DAG:     return {{_v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: fn slate_wait_until({{arg[0-9]+}}: *mut libc::timespec) -> i32 {
// LOWERING-MACOS-DAG:     let mut deadline: *mut libc::timespec = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     let mut result: i32 = 0;
// LOWERING-MACOS-DAG:     deadline = {{arg[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-MACOS-DAG:         pthread_mutex_lock(std::ptr::addr_of_mut!(slate_mutex) as *mut _opaque_pthread_mutex_t)
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     result = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = result;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-MACOS-DAG:         if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: *mut libc::timespec = deadline;
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-MACOS-DAG:                 pthread_cond_timedwait(
// LOWERING-MACOS-DAG:                     std::ptr::addr_of_mut!(slate_condition) as *mut _opaque_pthread_cond_t,
// LOWERING-MACOS-DAG:                     std::ptr::addr_of_mut!(slate_mutex) as *mut _opaque_pthread_mutex_t,
// LOWERING-MACOS-DAG:                     {{_v[0-9]+}} as *const libc::timespec,
// LOWERING-MACOS-DAG:                 )
// LOWERING-MACOS-DAG:             };
// LOWERING-MACOS-DAG:             result = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:         }
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-MACOS-DAG:         pthread_mutex_unlock(std::ptr::addr_of_mut!(slate_mutex) as *mut _opaque_pthread_mutex_t)
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     } else {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = result;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-MACOS-DAG:     return {{_v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: fn slate_read_lock() -> i32 {
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-MACOS-DAG:         pthread_rwlock_rdlock(std::ptr::addr_of_mut!(slate_rwlock) as *mut _opaque_pthread_rwlock_t)
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     } else {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-MACOS-DAG:             pthread_rwlock_unlock(
// LOWERING-MACOS-DAG:                 std::ptr::addr_of_mut!(slate_rwlock) as *mut _opaque_pthread_rwlock_t
// LOWERING-MACOS-DAG:             )
// LOWERING-MACOS-DAG:         };
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-MACOS-DAG:     return {{_v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: unsafe fn slate_tls_once({{arg[0-9]+}}: *mut u64, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-MACOS-DAG:     let mut key: *mut u64 = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     let mut value: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     key = {{arg[0-9]+}};
// LOWERING-MACOS-DAG:     value = {{arg[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-MACOS-DAG:         pthread_once(
// LOWERING-MACOS-DAG:             std::ptr::addr_of_mut!(slate_once) as *mut _opaque_pthread_once_t,
// LOWERING-MACOS-DAG:             Some(slate_once_routine),
// LOWERING-MACOS-DAG:         )
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     } else {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut u64 = key;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: Option<unsafe extern "C" fn(*mut core::ffi::c_void)> = None;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = unsafe { pthread_key_create({{_v[0-9]+}} as *mut u64, {{_v[0-9]+}}) };
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     } else {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut u64 = key;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: u64 = unsafe { *{{_v[0-9]+}} };
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut core::ffi::c_void = value;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 =
// LOWERING-MACOS-DAG:             unsafe { pthread_setspecific({{_v[0-9]+}} as u64, {{_v[0-9]+}} as *const core::ffi::c_void) };
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-MACOS-DAG:     return {{_v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: fn slate_post_named_semaphore({{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-MACOS-DAG:     let mut semaphore: *mut i32 = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut i32 = unsafe { sem_open({{arg[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}} as i32) };
// LOWERING-MACOS-DAG:     semaphore = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut i32 = semaphore;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u64 = 18446744073709551615u64;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     } else {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut i32 = semaphore;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = unsafe { sem_post({{_v[0-9]+}} as *mut i32) };
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     } else {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut i32 = semaphore;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = unsafe { sem_close({{_v[0-9]+}} as *mut i32) };
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-MACOS-DAG:     return {{_v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: fn slate_yield() -> i32 {
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe { sched_yield() };
// LOWERING-MACOS-DAG:     return {{_v[0-9]+}};
// LOWERING-MACOS-DAG: }
// SLATE-FILECHECK-END lowering-macos

// SLATE-FILECHECK-BEGIN rewrites-macos
// REWRITES-MACOS-DAG: unsafe fn slate_create_and_join(
// REWRITES-MACOS-DAG:     mut thread: *mut *mut _opaque_pthread_t,
// REWRITES-MACOS-DAG:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// REWRITES-MACOS-DAG: ) -> i32 {
// REWRITES-MACOS-DAG:     let mut result: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-MACOS-DAG:     result = std::ptr::null_mut();
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: *mut _opaque_pthread_attr_t = std::ptr::null_mut();
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-MACOS-DAG:         pthread_create(
// REWRITES-MACOS-DAG:             thread as *mut *mut _opaque_pthread_t,
// REWRITES-MACOS-DAG:             {{_v[0-9]+}} as *const _opaque_pthread_attr_t,
// REWRITES-MACOS-DAG:             Some(slate_thread_entry),
// REWRITES-MACOS-DAG:             {{arg[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-MACOS-DAG:         )
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     } else {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-MACOS-DAG:             pthread_join(
// REWRITES-MACOS-DAG:                 (unsafe { *thread }) as *mut _opaque_pthread_t,
// REWRITES-MACOS-DAG:                 std::ptr::addr_of_mut!(result) as *mut *mut core::ffi::c_void,
// REWRITES-MACOS-DAG:             )
// REWRITES-MACOS-DAG:         };
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     {{_v[0-9]+}} as i32
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: fn slate_wait_until(mut deadline: *mut libc::timespec) -> i32 {
// REWRITES-MACOS-DAG:     let mut result: i32 = unsafe {
// REWRITES-MACOS-DAG:         pthread_mutex_lock(std::ptr::addr_of_mut!(slate_mutex) as *mut _opaque_pthread_mutex_t)
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = result == 0;
// REWRITES-MACOS-DAG:     if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         result = unsafe {
// REWRITES-MACOS-DAG:             pthread_cond_timedwait(
// REWRITES-MACOS-DAG:                 std::ptr::addr_of_mut!(slate_condition) as *mut _opaque_pthread_cond_t,
// REWRITES-MACOS-DAG:                 std::ptr::addr_of_mut!(slate_mutex) as *mut _opaque_pthread_mutex_t,
// REWRITES-MACOS-DAG:                 deadline as *const libc::timespec,
// REWRITES-MACOS-DAG:             )
// REWRITES-MACOS-DAG:         };
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-MACOS-DAG:         pthread_mutex_unlock(std::ptr::addr_of_mut!(slate_mutex) as *mut _opaque_pthread_mutex_t)
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     } else {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = result != 0;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     {{_v[0-9]+}} as i32
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: fn slate_read_lock() -> i32 {
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-MACOS-DAG:         pthread_rwlock_rdlock(std::ptr::addr_of_mut!(slate_rwlock) as *mut _opaque_pthread_rwlock_t)
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     } else {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-MACOS-DAG:             pthread_rwlock_unlock(
// REWRITES-MACOS-DAG:                 std::ptr::addr_of_mut!(slate_rwlock) as *mut _opaque_pthread_rwlock_t
// REWRITES-MACOS-DAG:             )
// REWRITES-MACOS-DAG:         };
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     {{_v[0-9]+}} as i32
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: unsafe fn slate_tls_once(mut key: *mut u64, mut value: *mut core::ffi::c_void) -> i32 {
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-MACOS-DAG:         pthread_once(
// REWRITES-MACOS-DAG:             std::ptr::addr_of_mut!(slate_once) as *mut _opaque_pthread_once_t,
// REWRITES-MACOS-DAG:             Some(slate_once_routine),
// REWRITES-MACOS-DAG:         )
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     } else {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: Option<unsafe extern "C" fn(*mut core::ffi::c_void)> = None;
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: i32 = unsafe { pthread_key_create(key as *mut u64, {{_v[0-9]+}}) };
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     } else {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-MACOS-DAG:             pthread_setspecific((unsafe { *key }) as u64, value as *const core::ffi::c_void)
// REWRITES-MACOS-DAG:         };
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     {{_v[0-9]+}} as i32
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: fn slate_post_named_semaphore({{arg[0-9]+}}: *mut i8) -> i32 {
// REWRITES-MACOS-DAG:     let mut semaphore: *mut i32 = std::ptr::null_mut();
// REWRITES-MACOS-DAG:     semaphore = unsafe { sem_open({{arg[0-9]+}} as *const core::ffi::c_char, 0 as i32) };
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = semaphore == (18446744073709551615u64 as *mut i32);
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     } else {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: i32 = unsafe { sem_post(semaphore as *mut i32) };
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     } else {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: i32 = unsafe { sem_close(semaphore as *mut i32) };
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     {{_v[0-9]+}} as i32
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: fn slate_yield() -> i32 {
// REWRITES-MACOS-DAG:     unsafe { sched_yield() }
// REWRITES-MACOS-DAG: }
// SLATE-FILECHECK-END rewrites-macos
