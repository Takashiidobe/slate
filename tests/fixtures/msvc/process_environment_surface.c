#include <process.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                              \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

_Static_assert(_P_WAIT == 0, "_P_WAIT");
_Static_assert(_P_NOWAIT == 1, "_P_NOWAIT");
_Static_assert(_OLD_P_OVERLAY == 2, "_OLD_P_OVERLAY");
_Static_assert(_P_NOWAITO == 3, "_P_NOWAITO");
_Static_assert(_P_DETACH == 4, "_P_DETACH");
_Static_assert(_P_OVERLAY == 2, "_P_OVERLAY");
_Static_assert(_WAIT_CHILD == 0, "_WAIT_CHILD");
_Static_assert(_WAIT_GRANDCHILD == 1, "_WAIT_GRANDCHILD");
_Static_assert(_MAX_ENV == 32767, "_MAX_ENV");

_Static_assert(__builtin_types_compatible_p(_beginthread_proc_type,
                                            void (*)(void *)),
               "_beginthread_proc_type");
_Static_assert(__builtin_types_compatible_p(_beginthreadex_proc_type,
                                            unsigned int (*)(void *)),
               "_beginthreadex_proc_type");
_Static_assert(__builtin_types_compatible_p(
                   _tls_callback_type,
                   void (*)(void *, unsigned long, void *)),
               "_tls_callback_type");

TYPE_IS(&_beginthread,
        uintptr_t (*)(_beginthread_proc_type, unsigned int, void *));
TYPE_IS(&_endthread, void (*)(void));
TYPE_IS(&_beginthreadex,
        uintptr_t (*)(void *, unsigned int, _beginthreadex_proc_type, void *,
                      unsigned int, unsigned int *));
TYPE_IS(&_endthreadex, void (*)(unsigned int));
TYPE_IS(&_register_thread_local_exe_atexit_callback,
        void (*)(_tls_callback_type));
TYPE_IS(&_getpid, int (*)(void));
TYPE_IS(&_cwait, intptr_t (*)(int *, intptr_t, int));

#define CHECK_EXEC(prefix, char_type)                                          \
  TYPE_IS(&prefix##execl,                                                      \
          intptr_t (*)(const char_type *, const char_type *, ...));            \
  TYPE_IS(&prefix##execle,                                                     \
          intptr_t (*)(const char_type *, const char_type *, ...));            \
  TYPE_IS(&prefix##execlp,                                                     \
          intptr_t (*)(const char_type *, const char_type *, ...));            \
  TYPE_IS(&prefix##execlpe,                                                    \
          intptr_t (*)(const char_type *, const char_type *, ...));            \
  TYPE_IS(&prefix##execv,                                                      \
          intptr_t (*)(const char_type *, const char_type *const *));          \
  TYPE_IS(&prefix##execve,                                                     \
          intptr_t (*)(const char_type *, const char_type *const *,            \
                       const char_type *const *));                             \
  TYPE_IS(&prefix##execvp,                                                     \
          intptr_t (*)(const char_type *, const char_type *const *));          \
  TYPE_IS(&prefix##execvpe,                                                    \
          intptr_t (*)(const char_type *, const char_type *const *,            \
                       const char_type *const *))

#define CHECK_SPAWN(prefix, char_type)                                         \
  TYPE_IS(&prefix##spawnl,                                                     \
          intptr_t (*)(int, const char_type *, const char_type *, ...));       \
  TYPE_IS(&prefix##spawnle,                                                    \
          intptr_t (*)(int, const char_type *, const char_type *, ...));       \
  TYPE_IS(&prefix##spawnlp,                                                    \
          intptr_t (*)(int, const char_type *, const char_type *, ...));       \
  TYPE_IS(&prefix##spawnlpe,                                                   \
          intptr_t (*)(int, const char_type *, const char_type *, ...));       \
  TYPE_IS(&prefix##spawnv,                                                     \
          intptr_t (*)(int, const char_type *, const char_type *const *));     \
  TYPE_IS(&prefix##spawnve,                                                    \
          intptr_t (*)(int, const char_type *, const char_type *const *,       \
                       const char_type *const *));                             \
  TYPE_IS(&prefix##spawnvp,                                                    \
          intptr_t (*)(int, const char_type *, const char_type *const *));     \
  TYPE_IS(&prefix##spawnvpe,                                                   \
          intptr_t (*)(int, const char_type *, const char_type *const *,       \
                       const char_type *const *))

CHECK_EXEC(_, char);
CHECK_EXEC(_w, wchar_t);
CHECK_SPAWN(_, char);
CHECK_SPAWN(_w, wchar_t);

TYPE_IS(&__p__pgmptr, char **(*)(void));
TYPE_IS(&__p__wpgmptr, wchar_t **(*)(void));
TYPE_IS(&_get_pgmptr, errno_t (*)(char **));
TYPE_IS(&_get_wpgmptr, errno_t (*)(wchar_t **));
TYPE_IS(&__p___argc, int *(*)(void));
TYPE_IS(&__p___argv, char ***(*)(void));
TYPE_IS(&__p___wargv, wchar_t ***(*)(void));
TYPE_IS(&__p__environ, char ***(*)(void));
TYPE_IS(&__p__wenviron, wchar_t ***(*)(void));
TYPE_IS(&_initialize_narrow_environment, int (*)(void));
TYPE_IS(&_initialize_wide_environment, int (*)(void));
TYPE_IS(&_get_initial_narrow_environment, char **(*)(void));
TYPE_IS(&_get_initial_wide_environment, wchar_t **(*)(void));
TYPE_IS(&_wgetenv, wchar_t *(*)(const wchar_t *));
TYPE_IS(&_putenv, int (*)(const char *));
TYPE_IS(&_wputenv, int (*)(const wchar_t *));
TYPE_IS(&_searchenv, void (*)(const char *, const char *, char *));
TYPE_IS(&_wsearchenv,
        void (*)(const wchar_t *, const wchar_t *, wchar_t *));
TYPE_IS(&_wsystem, int (*)(const wchar_t *));

extern int CreateProcessA;
extern int CreateProcessW;
extern int WaitForSingleObject;

int main(void) { return 0; }
