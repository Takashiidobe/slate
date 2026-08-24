#ifndef _SLATE_BITS_MSVC_PROCESS_H
#define _SLATE_BITS_MSVC_PROCESS_H

#define _P_WAIT         0
#define _P_NOWAIT       1
#define _OLD_P_OVERLAY  2
#define _P_NOWAITO      3
#define _P_DETACH       4
#define _P_OVERLAY      2
#define _WAIT_CHILD      0
#define _WAIT_GRANDCHILD 1

_Noreturn void _exit(int);
void           _cexit(void);
void           _c_exit(void);

typedef void (*_tls_callback_type)(void *, unsigned long, void *);
void _register_thread_local_exe_atexit_callback(_tls_callback_type);

typedef void (*_beginthread_proc_type)(void *);
typedef unsigned int (*_beginthreadex_proc_type)(void *);

__uintptr_t _beginthread(_beginthread_proc_type, unsigned int, void *);
void        _endthread(void);
__uintptr_t _beginthreadex(void *, unsigned int, _beginthreadex_proc_type,
                           void *, unsigned int, unsigned int *);
void        _endthreadex(unsigned int);

int        _getpid(void);
__intptr_t _cwait(int *, __intptr_t, int);

__intptr_t _execl(const char *, const char *, ...);
__intptr_t _execle(const char *, const char *, ...);
__intptr_t _execlp(const char *, const char *, ...);
__intptr_t _execlpe(const char *, const char *, ...);
__intptr_t _execv(const char *, const char *const *);
__intptr_t _execve(const char *, const char *const *, const char *const *);
__intptr_t _execvp(const char *, const char *const *);
__intptr_t _execvpe(const char *, const char *const *, const char *const *);

__intptr_t _spawnl(int, const char *, const char *, ...);
__intptr_t _spawnle(int, const char *, const char *, ...);
__intptr_t _spawnlp(int, const char *, const char *, ...);
__intptr_t _spawnlpe(int, const char *, const char *, ...);
__intptr_t _spawnv(int, const char *, const char *const *);
__intptr_t _spawnve(int, const char *, const char *const *,
                    const char *const *);
__intptr_t _spawnvp(int, const char *, const char *const *);
__intptr_t _spawnvpe(int, const char *, const char *const *,
                     const char *const *);

__intptr_t _wexecl(const wchar_t *, const wchar_t *, ...);
__intptr_t _wexecle(const wchar_t *, const wchar_t *, ...);
__intptr_t _wexeclp(const wchar_t *, const wchar_t *, ...);
__intptr_t _wexeclpe(const wchar_t *, const wchar_t *, ...);
__intptr_t _wexecv(const wchar_t *, const wchar_t *const *);
__intptr_t _wexecve(const wchar_t *, const wchar_t *const *,
                    const wchar_t *const *);
__intptr_t _wexecvp(const wchar_t *, const wchar_t *const *);
__intptr_t _wexecvpe(const wchar_t *, const wchar_t *const *,
                     const wchar_t *const *);

__intptr_t _wspawnl(int, const wchar_t *, const wchar_t *, ...);
__intptr_t _wspawnle(int, const wchar_t *, const wchar_t *, ...);
__intptr_t _wspawnlp(int, const wchar_t *, const wchar_t *, ...);
__intptr_t _wspawnlpe(int, const wchar_t *, const wchar_t *, ...);
__intptr_t _wspawnv(int, const wchar_t *, const wchar_t *const *);
__intptr_t _wspawnve(int, const wchar_t *, const wchar_t *const *,
                     const wchar_t *const *);
__intptr_t _wspawnvp(int, const wchar_t *, const wchar_t *const *);
__intptr_t _wspawnvpe(int, const wchar_t *, const wchar_t *const *,
                      const wchar_t *const *);

#endif
