#ifndef _SLATE_BITS_FREEBSD_SPAWN_H
#define _SLATE_BITS_FREEBSD_SPAWN_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/spawn.h> directly; include a public header instead."
#endif

struct sched_param;

typedef struct __posix_spawnattr           *posix_spawnattr_t;
typedef struct __posix_spawn_file_actions  *posix_spawn_file_actions_t;

#define POSIX_SPAWN_RESETIDS         0x01
#define POSIX_SPAWN_SETPGROUP        0x02
#define POSIX_SPAWN_SETSCHEDPARAM    0x04
#define POSIX_SPAWN_SETSCHEDULER     0x08
#define POSIX_SPAWN_SETSIGDEF        0x10
#define POSIX_SPAWN_SETSIGMASK       0x20
#define POSIX_SPAWN_DISABLE_ASLR_NP  0x40

int posix_spawn(pid_t *__restrict, const char *__restrict,
                const posix_spawn_file_actions_t *,
                const posix_spawnattr_t *__restrict, char *const *__restrict,
                char *const *__restrict);
int posix_spawnp(pid_t *__restrict, const char *__restrict,
                 const posix_spawn_file_actions_t *,
                 const posix_spawnattr_t *__restrict, char *const *__restrict,
                 char *const *__restrict);

int posix_spawnattr_init(posix_spawnattr_t *);
int posix_spawnattr_destroy(posix_spawnattr_t *);

int posix_spawnattr_setflags(posix_spawnattr_t *, short);
int posix_spawnattr_getflags(const posix_spawnattr_t *__restrict,
                             short *__restrict);

int posix_spawnattr_setpgroup(posix_spawnattr_t *, pid_t);
int posix_spawnattr_getpgroup(const posix_spawnattr_t *__restrict,
                              pid_t *__restrict);

int posix_spawnattr_setsigmask(posix_spawnattr_t *__restrict,
                               const sigset_t *__restrict);
int posix_spawnattr_getsigmask(const posix_spawnattr_t *__restrict,
                               sigset_t *__restrict);

int posix_spawnattr_setsigdefault(posix_spawnattr_t *__restrict,
                                  const sigset_t *__restrict);
int posix_spawnattr_getsigdefault(const posix_spawnattr_t *__restrict,
                                  sigset_t *__restrict);

int posix_spawnattr_setschedparam(posix_spawnattr_t *__restrict,
                                  const struct sched_param *__restrict);
int posix_spawnattr_getschedparam(const posix_spawnattr_t *__restrict,
                                  struct sched_param *__restrict);
int posix_spawnattr_setschedpolicy(posix_spawnattr_t *, int);
int posix_spawnattr_getschedpolicy(const posix_spawnattr_t *__restrict,
                                   int *__restrict);

int posix_spawn_file_actions_init(posix_spawn_file_actions_t *);
int posix_spawn_file_actions_destroy(posix_spawn_file_actions_t *);

int posix_spawn_file_actions_addopen(posix_spawn_file_actions_t *__restrict,
                                     int, const char *__restrict, int, mode_t);
int posix_spawn_file_actions_addclose(posix_spawn_file_actions_t *, int);
int posix_spawn_file_actions_adddup2(posix_spawn_file_actions_t *, int, int);

#endif
