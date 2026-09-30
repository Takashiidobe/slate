#include <process.h>

extern void slate_oracle__Exit(int) __attribute__((noreturn));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__Exit), __typeof__(_Exit)),
    "process.h:_Exit declaration differs from oracle");

static __typeof__(_Exit) *const slate_reference__Exit = &_Exit;

extern unsigned long long slate_oracle__beginthread(void (*)(void *) __attribute__((cdecl)), unsigned int, void *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__beginthread), __typeof__(_beginthread)),
    "process.h:_beginthread declaration differs from oracle");

static __typeof__(_beginthread) *const slate_reference__beginthread = &_beginthread;

extern unsigned long long slate_oracle__beginthreadex(void *, unsigned int, unsigned int (*)(void *) __attribute__((stdcall)), void *, unsigned int, unsigned int *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__beginthreadex), __typeof__(_beginthreadex)),
    "process.h:_beginthreadex declaration differs from oracle");

static __typeof__(_beginthreadex) *const slate_reference__beginthreadex = &_beginthreadex;

extern void slate_oracle__c_exit(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__c_exit), __typeof__(_c_exit)),
    "process.h:_c_exit declaration differs from oracle");

static __typeof__(_c_exit) *const slate_reference__c_exit = &_c_exit;

extern void slate_oracle__cexit(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__cexit), __typeof__(_cexit)),
    "process.h:_cexit declaration differs from oracle");

static __typeof__(_cexit) *const slate_reference__cexit = &_cexit;

extern long long slate_oracle__cwait(int *, long long, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__cwait), __typeof__(_cwait)),
    "process.h:_cwait declaration differs from oracle");

static __typeof__(_cwait) *const slate_reference__cwait = &_cwait;

extern void slate_oracle__endthread(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__endthread), __typeof__(_endthread)),
    "process.h:_endthread declaration differs from oracle");

static __typeof__(_endthread) *const slate_reference__endthread = &_endthread;

extern void slate_oracle__endthreadex(unsigned int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__endthreadex), __typeof__(_endthreadex)),
    "process.h:_endthreadex declaration differs from oracle");

static __typeof__(_endthreadex) *const slate_reference__endthreadex = &_endthreadex;

extern long long slate_oracle__execl(const char *, const char *, ...) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__execl), __typeof__(_execl)),
    "process.h:_execl declaration differs from oracle");

static __typeof__(_execl) *const slate_reference__execl = &_execl;

extern long long slate_oracle__execle(const char *, const char *, ...) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__execle), __typeof__(_execle)),
    "process.h:_execle declaration differs from oracle");

static __typeof__(_execle) *const slate_reference__execle = &_execle;

extern long long slate_oracle__execlp(const char *, const char *, ...) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__execlp), __typeof__(_execlp)),
    "process.h:_execlp declaration differs from oracle");

static __typeof__(_execlp) *const slate_reference__execlp = &_execlp;

extern long long slate_oracle__execlpe(const char *, const char *, ...) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__execlpe), __typeof__(_execlpe)),
    "process.h:_execlpe declaration differs from oracle");

static __typeof__(_execlpe) *const slate_reference__execlpe = &_execlpe;

extern long long slate_oracle__execv(const char *, const char *const *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__execv), __typeof__(_execv)),
    "process.h:_execv declaration differs from oracle");

static __typeof__(_execv) *const slate_reference__execv = &_execv;

extern long long slate_oracle__execve(const char *, const char *const *, const char *const *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__execve), __typeof__(_execve)),
    "process.h:_execve declaration differs from oracle");

static __typeof__(_execve) *const slate_reference__execve = &_execve;

extern long long slate_oracle__execvp(const char *, const char *const *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__execvp), __typeof__(_execvp)),
    "process.h:_execvp declaration differs from oracle");

static __typeof__(_execvp) *const slate_reference__execvp = &_execvp;

extern long long slate_oracle__execvpe(const char *, const char *const *, const char *const *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__execvpe), __typeof__(_execvpe)),
    "process.h:_execvpe declaration differs from oracle");

static __typeof__(_execvpe) *const slate_reference__execvpe = &_execvpe;

extern void slate_oracle__exit(int) __attribute__((noreturn));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__exit), __typeof__(_exit)),
    "process.h:_exit declaration differs from oracle");

static __typeof__(_exit) *const slate_reference__exit = &_exit;

extern int slate_oracle__getpid(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__getpid), __typeof__(_getpid)),
    "process.h:_getpid declaration differs from oracle");

static __typeof__(_getpid) *const slate_reference__getpid = &_getpid;

extern long long slate_oracle__loaddll(char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__loaddll), __typeof__(_loaddll)),
    "process.h:_loaddll declaration differs from oracle");

static __typeof__(_loaddll) *const slate_reference__loaddll = &_loaddll;

extern void slate_oracle__register_thread_local_exe_atexit_callback(void (*)(void *, unsigned long, void *) __attribute__((stdcall))) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__register_thread_local_exe_atexit_callback), __typeof__(_register_thread_local_exe_atexit_callback)),
    "process.h:_register_thread_local_exe_atexit_callback declaration differs from oracle");

static __typeof__(_register_thread_local_exe_atexit_callback) *const slate_reference__register_thread_local_exe_atexit_callback = &_register_thread_local_exe_atexit_callback;

extern long long slate_oracle__spawnl(int, const char *, const char *, ...) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__spawnl), __typeof__(_spawnl)),
    "process.h:_spawnl declaration differs from oracle");

static __typeof__(_spawnl) *const slate_reference__spawnl = &_spawnl;

extern long long slate_oracle__spawnle(int, const char *, const char *, ...) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__spawnle), __typeof__(_spawnle)),
    "process.h:_spawnle declaration differs from oracle");

static __typeof__(_spawnle) *const slate_reference__spawnle = &_spawnle;

extern long long slate_oracle__spawnlp(int, const char *, const char *, ...) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__spawnlp), __typeof__(_spawnlp)),
    "process.h:_spawnlp declaration differs from oracle");

static __typeof__(_spawnlp) *const slate_reference__spawnlp = &_spawnlp;

extern long long slate_oracle__spawnlpe(int, const char *, const char *, ...) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__spawnlpe), __typeof__(_spawnlpe)),
    "process.h:_spawnlpe declaration differs from oracle");

static __typeof__(_spawnlpe) *const slate_reference__spawnlpe = &_spawnlpe;

extern long long slate_oracle__spawnv(int, const char *, const char *const *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__spawnv), __typeof__(_spawnv)),
    "process.h:_spawnv declaration differs from oracle");

static __typeof__(_spawnv) *const slate_reference__spawnv = &_spawnv;

extern long long slate_oracle__spawnve(int, const char *, const char *const *, const char *const *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__spawnve), __typeof__(_spawnve)),
    "process.h:_spawnve declaration differs from oracle");

static __typeof__(_spawnve) *const slate_reference__spawnve = &_spawnve;

extern long long slate_oracle__spawnvp(int, const char *, const char *const *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__spawnvp), __typeof__(_spawnvp)),
    "process.h:_spawnvp declaration differs from oracle");

static __typeof__(_spawnvp) *const slate_reference__spawnvp = &_spawnvp;

extern long long slate_oracle__spawnvpe(int, const char *, const char *const *, const char *const *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__spawnvpe), __typeof__(_spawnvpe)),
    "process.h:_spawnvpe declaration differs from oracle");

static __typeof__(_spawnvpe) *const slate_reference__spawnvpe = &_spawnvpe;

extern int slate_oracle__unloaddll(long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__unloaddll), __typeof__(_unloaddll)),
    "process.h:_unloaddll declaration differs from oracle");

static __typeof__(_unloaddll) *const slate_reference__unloaddll = &_unloaddll;

extern void slate_oracle_abort(void) __attribute__((noreturn));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_abort), __typeof__(abort)),
    "process.h:abort declaration differs from oracle");

static __typeof__(abort) *const slate_reference_abort = &abort;

extern long long slate_oracle_cwait(int *, long long, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cwait), __typeof__(cwait)),
    "process.h:cwait declaration differs from oracle");

static __typeof__(cwait) *const slate_reference_cwait = &cwait;

extern long long slate_oracle_execl(const char *, const char *, ...) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_execl), __typeof__(execl)),
    "process.h:execl declaration differs from oracle");

static __typeof__(execl) *const slate_reference_execl = &execl;

extern long long slate_oracle_execle(const char *, const char *, ...) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_execle), __typeof__(execle)),
    "process.h:execle declaration differs from oracle");

static __typeof__(execle) *const slate_reference_execle = &execle;

extern long long slate_oracle_execlp(const char *, const char *, ...) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_execlp), __typeof__(execlp)),
    "process.h:execlp declaration differs from oracle");

static __typeof__(execlp) *const slate_reference_execlp = &execlp;

extern long long slate_oracle_execlpe(const char *, const char *, ...) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_execlpe), __typeof__(execlpe)),
    "process.h:execlpe declaration differs from oracle");

static __typeof__(execlpe) *const slate_reference_execlpe = &execlpe;

extern long long slate_oracle_execv(const char *, const char *const *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_execv), __typeof__(execv)),
    "process.h:execv declaration differs from oracle");

static __typeof__(execv) *const slate_reference_execv = &execv;

extern long long slate_oracle_execve(const char *, const char *const *, const char *const *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_execve), __typeof__(execve)),
    "process.h:execve declaration differs from oracle");

static __typeof__(execve) *const slate_reference_execve = &execve;

extern long long slate_oracle_execvp(const char *, const char *const *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_execvp), __typeof__(execvp)),
    "process.h:execvp declaration differs from oracle");

static __typeof__(execvp) *const slate_reference_execvp = &execvp;

extern long long slate_oracle_execvpe(const char *, const char *const *, const char *const *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_execvpe), __typeof__(execvpe)),
    "process.h:execvpe declaration differs from oracle");

static __typeof__(execvpe) *const slate_reference_execvpe = &execvpe;

extern void slate_oracle_exit(int) __attribute__((noreturn));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exit), __typeof__(exit)),
    "process.h:exit declaration differs from oracle");

static __typeof__(exit) *const slate_reference_exit = &exit;

extern int slate_oracle_getpid(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getpid), __typeof__(getpid)),
    "process.h:getpid declaration differs from oracle");

static __typeof__(getpid) *const slate_reference_getpid = &getpid;

extern void slate_oracle_quick_exit(int) __attribute__((noreturn)) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_quick_exit), __typeof__(quick_exit)),
    "process.h:quick_exit declaration differs from oracle");

static __typeof__(quick_exit) *const slate_reference_quick_exit = &quick_exit;

extern long long slate_oracle_spawnl(int, const char *, const char *, ...) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_spawnl), __typeof__(spawnl)),
    "process.h:spawnl declaration differs from oracle");

static __typeof__(spawnl) *const slate_reference_spawnl = &spawnl;

extern long long slate_oracle_spawnle(int, const char *, const char *, ...) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_spawnle), __typeof__(spawnle)),
    "process.h:spawnle declaration differs from oracle");

static __typeof__(spawnle) *const slate_reference_spawnle = &spawnle;

extern long long slate_oracle_spawnlp(int, const char *, const char *, ...) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_spawnlp), __typeof__(spawnlp)),
    "process.h:spawnlp declaration differs from oracle");

static __typeof__(spawnlp) *const slate_reference_spawnlp = &spawnlp;

extern long long slate_oracle_spawnlpe(int, const char *, const char *, ...) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_spawnlpe), __typeof__(spawnlpe)),
    "process.h:spawnlpe declaration differs from oracle");

static __typeof__(spawnlpe) *const slate_reference_spawnlpe = &spawnlpe;

extern long long slate_oracle_spawnv(int, const char *, const char *const *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_spawnv), __typeof__(spawnv)),
    "process.h:spawnv declaration differs from oracle");

static __typeof__(spawnv) *const slate_reference_spawnv = &spawnv;

extern long long slate_oracle_spawnve(int, const char *, const char *const *, const char *const *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_spawnve), __typeof__(spawnve)),
    "process.h:spawnve declaration differs from oracle");

static __typeof__(spawnve) *const slate_reference_spawnve = &spawnve;

extern long long slate_oracle_spawnvp(int, const char *, const char *const *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_spawnvp), __typeof__(spawnvp)),
    "process.h:spawnvp declaration differs from oracle");

static __typeof__(spawnvp) *const slate_reference_spawnvp = &spawnvp;

extern long long slate_oracle_spawnvpe(int, const char *, const char *const *, const char *const *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_spawnvpe), __typeof__(spawnvpe)),
    "process.h:spawnvpe declaration differs from oracle");

static __typeof__(spawnvpe) *const slate_reference_spawnvpe = &spawnvpe;

extern int slate_oracle_system(const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_system), __typeof__(system)),
    "process.h:system declaration differs from oracle");

static __typeof__(system) *const slate_reference_system = &system;

#ifndef OLD_P_OVERLAY
#error "process.h:OLD_P_OVERLAY macro is missing from libc-shim"
#endif

#ifndef P_DETACH
#error "process.h:P_DETACH macro is missing from libc-shim"
#endif

#ifndef P_NOWAIT
#error "process.h:P_NOWAIT macro is missing from libc-shim"
#endif

#ifndef P_NOWAITO
#error "process.h:P_NOWAITO macro is missing from libc-shim"
#endif

#ifndef P_OVERLAY
#error "process.h:P_OVERLAY macro is missing from libc-shim"
#endif

#ifndef P_WAIT
#error "process.h:P_WAIT macro is missing from libc-shim"
#endif

#ifndef WAIT_CHILD
#error "process.h:WAIT_CHILD macro is missing from libc-shim"
#endif

#ifndef WAIT_GRANDCHILD
#error "process.h:WAIT_GRANDCHILD macro is missing from libc-shim"
#endif

#ifndef _INC_PROCESS
#error "process.h:_INC_PROCESS macro is missing from libc-shim"
#endif

#ifndef _OLD_P_OVERLAY
#error "process.h:_OLD_P_OVERLAY macro is missing from libc-shim"
#endif

#ifndef _P_DETACH
#error "process.h:_P_DETACH macro is missing from libc-shim"
#endif

#ifndef _P_NOWAIT
#error "process.h:_P_NOWAIT macro is missing from libc-shim"
#endif

#ifndef _P_NOWAITO
#error "process.h:_P_NOWAITO macro is missing from libc-shim"
#endif

#ifndef _P_OVERLAY
#error "process.h:_P_OVERLAY macro is missing from libc-shim"
#endif

#ifndef _P_WAIT
#error "process.h:_P_WAIT macro is missing from libc-shim"
#endif

#ifndef _WAIT_CHILD
#error "process.h:_WAIT_CHILD macro is missing from libc-shim"
#endif

#ifndef _WAIT_GRANDCHILD
#error "process.h:_WAIT_GRANDCHILD macro is missing from libc-shim"
#endif

int main(void) { return 0; }
