#include <setjmp.h>
#include <signal.h>
#include <stddef.h>
#include <sys/time.h>
#include <sys/ucontext.h>
#include <time.h>

_Static_assert(sizeof(clock_t) == 8, "clock_t");
_Static_assert(sizeof(time_t) == 8, "time_t");
_Static_assert(sizeof(clockid_t) == 4, "clockid_t");
_Static_assert(__builtin_types_compatible_p(clockid_t, unsigned int),
               "clockid_t type");
_Static_assert(sizeof(struct timespec) == 16, "timespec");
_Static_assert(sizeof(struct timeval) == 16, "timeval");
_Static_assert(sizeof(struct timezone) == 8, "timezone");
_Static_assert(sizeof(struct tm) == 56, "tm");
_Static_assert(sizeof(struct itimerval) == 32, "itimerval");
_Static_assert(CLOCK_REALTIME == 0, "CLOCK_REALTIME");
_Static_assert(CLOCK_MONOTONIC_RAW == 4, "CLOCK_MONOTONIC_RAW");
_Static_assert(CLOCK_MONOTONIC == 6, "CLOCK_MONOTONIC");
_Static_assert(CLOCK_UPTIME_RAW == 8, "CLOCK_UPTIME_RAW");
_Static_assert(CLOCK_PROCESS_CPUTIME_ID == 12, "CLOCK_PROCESS_CPUTIME_ID");
_Static_assert(CLOCK_THREAD_CPUTIME_ID == 16, "CLOCK_THREAD_CPUTIME_ID");

_Static_assert(sizeof(sigset_t) == 4, "sigset_t");
_Static_assert(sizeof(union sigval) == 8, "sigval");
_Static_assert(sizeof(struct sigevent) == 32, "sigevent");
_Static_assert(offsetof(struct sigevent, sigev_notify) == 0, "sigevent notify");
_Static_assert(offsetof(struct sigevent, sigev_value) == 8, "sigevent value");
_Static_assert(sizeof(siginfo_t) == 104, "siginfo_t");
_Static_assert(offsetof(siginfo_t, si_addr) == 24, "siginfo_t addr");
_Static_assert(offsetof(siginfo_t, si_value) == 32, "siginfo_t value");
_Static_assert(sizeof(struct sigaction) == 16, "sigaction");
_Static_assert(offsetof(struct sigaction, sa_mask) == 8, "sigaction mask");
_Static_assert(sizeof(stack_t) == 24, "stack_t");
_Static_assert(offsetof(stack_t, ss_size) == 8, "stack_t size");
_Static_assert(sizeof(ucontext_t) == 56, "ucontext_t");
_Static_assert(offsetof(ucontext_t, uc_mcontext) == 48, "ucontext_t mcontext");
_Static_assert(sizeof(jmp_buf) == 192, "jmp_buf");
_Static_assert(sizeof(sigjmp_buf) == 196, "sigjmp_buf");

_Static_assert(SIG_BLOCK == 1, "SIG_BLOCK");
_Static_assert(SIG_UNBLOCK == 2, "SIG_UNBLOCK");
_Static_assert(SIG_SETMASK == 3, "SIG_SETMASK");
_Static_assert(SA_ONSTACK == 0x0001, "SA_ONSTACK");
_Static_assert(SA_RESTART == 0x0002, "SA_RESTART");
_Static_assert(SA_RESETHAND == 0x0004, "SA_RESETHAND");
_Static_assert(SA_SIGINFO == 0x0040, "SA_SIGINFO");
_Static_assert(SS_ONSTACK == 0x0001, "SS_ONSTACK");
_Static_assert(SS_DISABLE == 0x0004, "SS_DISABLE");
_Static_assert(MINSIGSTKSZ == 32768, "MINSIGSTKSZ");
_Static_assert(SIGSTKSZ == 131072, "SIGSTKSZ");

_Static_assert(__builtin_types_compatible_p(__typeof__(&clock_gettime),
                                            int (*)(clockid_t,
                                                    struct timespec *)),
               "clock_gettime signature");
_Static_assert(__builtin_types_compatible_p(
                   __typeof__(&setitimer),
                   int (*)(int, const struct itimerval *__restrict,
                           struct itimerval *__restrict)),
               "setitimer signature");
_Static_assert(__builtin_types_compatible_p(
                   __typeof__(&sigaction),
                   int (*)(int, const struct sigaction *__restrict,
                           struct sigaction *__restrict)),
               "sigaction signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&sigaltstack),
                                            int (*)(const stack_t *__restrict,
                                                    stack_t *__restrict)),
               "sigaltstack signature");

void slate_signal_handler(int signo, siginfo_t *info, void *context) {
  (void)signo;
  (void)info;
  (void)context;
}

// @lowering-fn-begin
// @rewrite-fn-begin
int slate_clock_now(struct timespec *value) {
  return clock_gettime(CLOCK_MONOTONIC, value);
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int slate_arm_timer(const struct itimerval *value) {
  return setitimer(ITIMER_REAL, value, 0);
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int slate_install_handler(struct sigaction *action, stack_t *stack) {
  action->sa_sigaction = slate_signal_handler;
  action->sa_flags     = SA_SIGINFO | SA_ONSTACK;
  return sigaltstack(stack, 0) || sigaction(SIGUSR1, action, 0);
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int slate_block_signal(sigset_t *mask) {
  return sigemptyset(mask) || sigaddset(mask, SIGUSR1) ||
         sigprocmask(SIG_BLOCK, mask, 0);
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
void slate_jump(sigjmp_buf environment) {
  if (sigsetjmp(environment, 1) != 0)
    return;
  siglongjmp(environment, 1);
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering-macos
// LOWERING-MACOS-DAG: fn slate_clock_now({{arg[0-9]+}}: *mut libc::timespec) -> i32 {
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: u32 = 6;
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: i32 = unsafe { clock_gettime({{__v[0-9]+}} as u32, {{arg[0-9]+}} as *mut libc::timespec) };
// LOWERING-MACOS-DAG:     return {{__v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: fn slate_arm_timer({{arg[0-9]+}}: *mut itimerval) -> i32 {
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: *mut itimerval = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-MACOS-DAG:         setitimer(
// LOWERING-MACOS-DAG:             {{__v[0-9]+}} as i32,
// LOWERING-MACOS-DAG:             {{arg[0-9]+}} as *const itimerval,
// LOWERING-MACOS-DAG:             {{__v[0-9]+}} as *mut itimerval,
// LOWERING-MACOS-DAG:         )
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     return {{__v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: unsafe fn slate_install_handler({{arg[0-9]+}}: *mut sigaction, {{arg[0-9]+}}: *mut __darwin_sigaltstack) -> i32 {
// LOWERING-MACOS-DAG:     let mut action: *mut sigaction = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     action = {{arg[0-9]+}};
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: *mut sigaction = action;
// LOWERING-MACOS-DAG:     unsafe {
// LOWERING-MACOS-DAG:         (*{{__v[0-9]+}}).__sigaction_u.__sa_sigaction = unsafe {
// LOWERING-MACOS-DAG:             std::mem::transmute::<
// LOWERING-MACOS-DAG:                 *const (),
// LOWERING-MACOS-DAG:                 Option<unsafe extern "C-unwind" fn(i32, *mut __siginfo, *mut core::ffi::c_void)>,
// LOWERING-MACOS-DAG:             >(slate_signal_handler as *const ())
// LOWERING-MACOS-DAG:         };
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: i32 = 64;
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: *mut sigaction = action;
// LOWERING-MACOS-DAG:     unsafe {
// LOWERING-MACOS-DAG:         (*{{__v[0-9]+}}).sa_flags = {{__v[0-9]+}};
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: *mut __darwin_sigaltstack = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-MACOS-DAG:         sigaltstack(
// LOWERING-MACOS-DAG:             {{arg[0-9]+}} as *const __darwin_sigaltstack,
// LOWERING-MACOS-DAG:             {{__v[0-9]+}} as *mut __darwin_sigaltstack,
// LOWERING-MACOS-DAG:         )
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: bool = true;
// LOWERING-MACOS-DAG:         {{__v[0-9]+}}
// LOWERING-MACOS-DAG:     } else {
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: i32 = 30;
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: *mut sigaction = action;
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: *mut sigaction = std::ptr::null_mut();
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-MACOS-DAG:             sigaction(
// LOWERING-MACOS-DAG:                 {{__v[0-9]+}} as i32,
// LOWERING-MACOS-DAG:                 {{__v[0-9]+}} as *const sigaction,
// LOWERING-MACOS-DAG:                 {{__v[0-9]+}} as *mut sigaction,
// LOWERING-MACOS-DAG:             )
// LOWERING-MACOS-DAG:         };
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:         {{__v[0-9]+}}
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-MACOS-DAG:     return {{__v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: fn slate_block_signal({{arg[0-9]+}}: *mut u32) -> i32 {
// LOWERING-MACOS-DAG:     let mut mask: *mut u32 = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     mask = {{arg[0-9]+}};
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: *mut u32 = mask;
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: i32 = unsafe { sigemptyset({{__v[0-9]+}} as *mut u32) };
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: bool = true;
// LOWERING-MACOS-DAG:         {{__v[0-9]+}}
// LOWERING-MACOS-DAG:     } else {
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: *mut u32 = mask;
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: i32 = 30;
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: i32 = unsafe { sigaddset({{__v[0-9]+}} as *mut u32, {{__v[0-9]+}} as i32) };
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:         {{__v[0-9]+}}
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: bool = true;
// LOWERING-MACOS-DAG:         {{__v[0-9]+}}
// LOWERING-MACOS-DAG:     } else {
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: *mut u32 = mask;
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: *mut u32 = std::ptr::null_mut();
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: i32 =
// LOWERING-MACOS-DAG:             unsafe { sigprocmask({{__v[0-9]+}} as i32, {{__v[0-9]+}} as *const u32, {{__v[0-9]+}} as *mut u32) };
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:         {{__v[0-9]+}}
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-MACOS-DAG:     return {{__v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: fn slate_jump({{arg[0-9]+}}: *mut i32) {
// LOWERING-MACOS-DAG:     let mut environment: *mut i32 = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     environment = {{arg[0-9]+}};
// LOWERING-MACOS-DAG:     {
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: *mut i32 = environment;
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: i32 = unsafe { sigsetjmp({{__v[0-9]+}} as *mut i32, {{__v[0-9]+}} as i32) };
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-MACOS-DAG:         if {{__v[0-9]+}} {
// LOWERING-MACOS-DAG:             return;
// LOWERING-MACOS-DAG:         }
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: *mut i32 = environment;
// LOWERING-MACOS-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-MACOS-DAG:     unsafe { siglongjmp({{__v[0-9]+}} as *mut i32, {{__v[0-9]+}} as i32) };
// LOWERING-MACOS-DAG:     return;
// LOWERING-MACOS-DAG: }
// SLATE-FILECHECK-END lowering-macos

// SLATE-FILECHECK-BEGIN rewrites-macos
// REWRITES-MACOS-DAG: fn slate_clock_now({{arg[0-9]+}}: *mut libc::timespec) -> i32 {
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: u32 = 6;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: i32 = unsafe { clock_gettime({{__v[0-9]+}} as u32, {{arg[0-9]+}} as *mut libc::timespec) };
// REWRITES-MACOS-DAG:     {{__v[0-9]+}}
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: fn slate_arm_timer({{arg[0-9]+}}: *mut itimerval) -> i32 {
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: *mut itimerval = std::ptr::null_mut();
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-MACOS-DAG:         setitimer(
// REWRITES-MACOS-DAG:             {{__v[0-9]+}} as i32,
// REWRITES-MACOS-DAG:             {{arg[0-9]+}} as *const itimerval,
// REWRITES-MACOS-DAG:             {{__v[0-9]+}} as *mut itimerval,
// REWRITES-MACOS-DAG:         )
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     {{__v[0-9]+}}
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: unsafe fn slate_install_handler(
// REWRITES-MACOS-DAG:     mut action: *mut sigaction,
// REWRITES-MACOS-DAG:     {{arg[0-9]+}}: *mut __darwin_sigaltstack,
// REWRITES-MACOS-DAG: ) -> i32 {
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: *mut sigaction = action;
// REWRITES-MACOS-DAG:     unsafe {
// REWRITES-MACOS-DAG:         (*{{__v[0-9]+}}).__sigaction_u.__sa_sigaction = unsafe {
// REWRITES-MACOS-DAG:             std::mem::transmute::<
// REWRITES-MACOS-DAG:                 *const (),
// REWRITES-MACOS-DAG:                 Option<unsafe extern "C-unwind" fn(i32, *mut __siginfo, *mut core::ffi::c_void)>,
// REWRITES-MACOS-DAG:             >(slate_signal_handler as *const ())
// REWRITES-MACOS-DAG:         };
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: i32 = 64;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: *mut sigaction = action;
// REWRITES-MACOS-DAG:     unsafe {
// REWRITES-MACOS-DAG:         (*{{__v[0-9]+}}).sa_flags = {{__v[0-9]+}};
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: *mut __darwin_sigaltstack = std::ptr::null_mut();
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-MACOS-DAG:         sigaltstack(
// REWRITES-MACOS-DAG:             {{arg[0-9]+}} as *const __darwin_sigaltstack,
// REWRITES-MACOS-DAG:             {{__v[0-9]+}} as *mut __darwin_sigaltstack,
// REWRITES-MACOS-DAG:         )
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-MACOS-DAG:         {{__v[0-9]+}}
// REWRITES-MACOS-DAG:     } else {
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: i32 = 30;
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: *mut sigaction = action;
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: *mut sigaction = std::ptr::null_mut();
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-MACOS-DAG:             sigaction(
// REWRITES-MACOS-DAG:                 {{__v[0-9]+}} as i32,
// REWRITES-MACOS-DAG:                 {{__v[0-9]+}} as *const sigaction,
// REWRITES-MACOS-DAG:                 {{__v[0-9]+}} as *mut sigaction,
// REWRITES-MACOS-DAG:             )
// REWRITES-MACOS-DAG:         };
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:         {{__v[0-9]+}}
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-MACOS-DAG:     {{__v[0-9]+}}
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: fn slate_block_signal(mut mask: *mut u32) -> i32 {
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: *mut u32 = mask;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: i32 = unsafe { sigemptyset({{__v[0-9]+}} as *mut u32) };
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-MACOS-DAG:         {{__v[0-9]+}}
// REWRITES-MACOS-DAG:     } else {
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: *mut u32 = mask;
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: i32 = 30;
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: i32 = unsafe { sigaddset({{__v[0-9]+}} as *mut u32, {{__v[0-9]+}} as i32) };
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:         {{__v[0-9]+}}
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-MACOS-DAG:         {{__v[0-9]+}}
// REWRITES-MACOS-DAG:     } else {
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: *mut u32 = mask;
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: *mut u32 = std::ptr::null_mut();
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: i32 =
// REWRITES-MACOS-DAG:             unsafe { sigprocmask({{__v[0-9]+}} as i32, {{__v[0-9]+}} as *const u32, {{__v[0-9]+}} as *mut u32) };
// REWRITES-MACOS-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:         {{__v[0-9]+}}
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-MACOS-DAG:     {{__v[0-9]+}}
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: fn slate_jump(mut environment: *mut i32) {
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: *mut i32 = environment;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: i32 = unsafe { sigsetjmp({{__v[0-9]+}} as *mut i32, {{__v[0-9]+}} as i32) };
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-MACOS-DAG:     if {{__v[0-9]+}} {
// REWRITES-MACOS-DAG:         return;
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: *mut i32 = environment;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-MACOS-DAG:     unsafe { siglongjmp({{__v[0-9]+}} as *mut i32, {{__v[0-9]+}} as i32) };
// REWRITES-MACOS-DAG:     return;
// REWRITES-MACOS-DAG: }
// SLATE-FILECHECK-END rewrites-macos
