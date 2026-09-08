#include <sys/types.h>

extern int slate_oracle_pselect(int, struct fd_set *restrict, struct fd_set *restrict, struct fd_set *restrict, const struct timespec *restrict, const struct __sigset_t *restrict);
extern int slate_oracle_select(int, struct fd_set *restrict, struct fd_set *restrict, struct fd_set *restrict, struct timeval *restrict);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pselect), __typeof__(pselect)),
    "sys/types.h:pselect declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_select), __typeof__(select)),
    "sys/types.h:select declaration differs from oracle");

static __typeof__(pselect) *const slate_reference_pselect = &pselect;
static __typeof__(select) *const slate_reference_select = &select;

typedef long slate_oracle_typedef_blkcnt64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_blkcnt64_t, blkcnt64_t), "typedef blkcnt64_t differs from oracle");

typedef long slate_oracle_typedef_blkcnt_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_blkcnt_t, blkcnt_t), "typedef blkcnt_t differs from oracle");

typedef long slate_oracle_typedef_blksize_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_blksize_t, blksize_t), "typedef blksize_t differs from oracle");

typedef char * slate_oracle_typedef_caddr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_caddr_t, caddr_t), "typedef caddr_t differs from oracle");

typedef long slate_oracle_typedef_clock_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_clock_t, clock_t), "typedef clock_t differs from oracle");

typedef int slate_oracle_typedef_clockid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_clockid_t, clockid_t), "typedef clockid_t differs from oracle");

typedef int slate_oracle_typedef_daddr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_daddr_t, daddr_t), "typedef daddr_t differs from oracle");

typedef unsigned long slate_oracle_typedef_dev_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_dev_t, dev_t), "typedef dev_t differs from oracle");

typedef long slate_oracle_typedef_fd_mask;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fd_mask, fd_mask), "typedef fd_mask differs from oracle");

typedef struct struct fd_set slate_oracle_typedef_fd_set;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fd_set, fd_set), "typedef fd_set differs from oracle");

typedef unsigned long slate_oracle_typedef_fsblkcnt64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fsblkcnt64_t, fsblkcnt64_t), "typedef fsblkcnt64_t differs from oracle");

typedef unsigned long slate_oracle_typedef_fsblkcnt_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fsblkcnt_t, fsblkcnt_t), "typedef fsblkcnt_t differs from oracle");

typedef unsigned long slate_oracle_typedef_fsfilcnt64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fsfilcnt64_t, fsfilcnt64_t), "typedef fsfilcnt64_t differs from oracle");

typedef unsigned long slate_oracle_typedef_fsfilcnt_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fsfilcnt_t, fsfilcnt_t), "typedef fsfilcnt_t differs from oracle");

typedef struct struct __fsid_t slate_oracle_typedef_fsid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fsid_t, fsid_t), "typedef fsid_t differs from oracle");

typedef unsigned int slate_oracle_typedef_gid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_gid_t, gid_t), "typedef gid_t differs from oracle");

typedef unsigned int slate_oracle_typedef_id_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_id_t, id_t), "typedef id_t differs from oracle");

typedef unsigned long slate_oracle_typedef_ino64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ino64_t, ino64_t), "typedef ino64_t differs from oracle");

typedef unsigned long slate_oracle_typedef_ino_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ino_t, ino_t), "typedef ino_t differs from oracle");

typedef short slate_oracle_typedef_int16_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int16_t, int16_t), "typedef int16_t differs from oracle");

typedef int slate_oracle_typedef_int32_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int32_t, int32_t), "typedef int32_t differs from oracle");

typedef long slate_oracle_typedef_int64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int64_t, int64_t), "typedef int64_t differs from oracle");

typedef signed char slate_oracle_typedef_int8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int8_t, int8_t), "typedef int8_t differs from oracle");

typedef int slate_oracle_typedef_key_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_key_t, key_t), "typedef key_t differs from oracle");

typedef long slate_oracle_typedef_loff_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_loff_t, loff_t), "typedef loff_t differs from oracle");

typedef unsigned int slate_oracle_typedef_mode_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_mode_t, mode_t), "typedef mode_t differs from oracle");

typedef unsigned long slate_oracle_typedef_nlink_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_nlink_t, nlink_t), "typedef nlink_t differs from oracle");

typedef long slate_oracle_typedef_off64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_off64_t, off64_t), "typedef off64_t differs from oracle");

typedef long slate_oracle_typedef_off_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_off_t, off_t), "typedef off_t differs from oracle");

typedef int slate_oracle_typedef_pid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pid_t, pid_t), "typedef pid_t differs from oracle");

typedef union union pthread_attr_t slate_oracle_typedef_pthread_attr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_attr_t, pthread_attr_t), "typedef pthread_attr_t differs from oracle");

typedef union union pthread_barrier_t slate_oracle_typedef_pthread_barrier_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_barrier_t, pthread_barrier_t), "typedef pthread_barrier_t differs from oracle");

typedef union union pthread_barrierattr_t slate_oracle_typedef_pthread_barrierattr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_barrierattr_t, pthread_barrierattr_t), "typedef pthread_barrierattr_t differs from oracle");

typedef union union pthread_cond_t slate_oracle_typedef_pthread_cond_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_cond_t, pthread_cond_t), "typedef pthread_cond_t differs from oracle");

typedef union union pthread_condattr_t slate_oracle_typedef_pthread_condattr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_condattr_t, pthread_condattr_t), "typedef pthread_condattr_t differs from oracle");

typedef unsigned int slate_oracle_typedef_pthread_key_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_key_t, pthread_key_t), "typedef pthread_key_t differs from oracle");

typedef union union pthread_mutex_t slate_oracle_typedef_pthread_mutex_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_mutex_t, pthread_mutex_t), "typedef pthread_mutex_t differs from oracle");

typedef union union pthread_mutexattr_t slate_oracle_typedef_pthread_mutexattr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_mutexattr_t, pthread_mutexattr_t), "typedef pthread_mutexattr_t differs from oracle");

typedef int slate_oracle_typedef_pthread_once_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_once_t, pthread_once_t), "typedef pthread_once_t differs from oracle");

typedef union union pthread_rwlock_t slate_oracle_typedef_pthread_rwlock_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_rwlock_t, pthread_rwlock_t), "typedef pthread_rwlock_t differs from oracle");

typedef union union pthread_rwlockattr_t slate_oracle_typedef_pthread_rwlockattr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_rwlockattr_t, pthread_rwlockattr_t), "typedef pthread_rwlockattr_t differs from oracle");

typedef volatile int slate_oracle_typedef_pthread_spinlock_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_spinlock_t, pthread_spinlock_t), "typedef pthread_spinlock_t differs from oracle");

typedef unsigned long slate_oracle_typedef_pthread_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_t, pthread_t), "typedef pthread_t differs from oracle");

typedef long slate_oracle_typedef_quad_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_quad_t, quad_t), "typedef quad_t differs from oracle");

typedef long slate_oracle_typedef_register_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_register_t, register_t), "typedef register_t differs from oracle");

typedef struct struct __sigset_t slate_oracle_typedef_sigset_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_sigset_t, sigset_t), "typedef sigset_t differs from oracle");

typedef unsigned long slate_oracle_typedef_size_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_size_t, size_t), "typedef size_t differs from oracle");

typedef long slate_oracle_typedef_ssize_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ssize_t, ssize_t), "typedef ssize_t differs from oracle");

typedef long slate_oracle_typedef_suseconds_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_suseconds_t, suseconds_t), "typedef suseconds_t differs from oracle");

typedef long slate_oracle_typedef_time_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_time_t, time_t), "typedef time_t differs from oracle");

typedef void * slate_oracle_typedef_timer_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_timer_t, timer_t), "typedef timer_t differs from oracle");

typedef unsigned char slate_oracle_typedef_u_char;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_char, u_char), "typedef u_char differs from oracle");

typedef unsigned int slate_oracle_typedef_u_int;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_int, u_int), "typedef u_int differs from oracle");

typedef unsigned short slate_oracle_typedef_u_int16_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_int16_t, u_int16_t), "typedef u_int16_t differs from oracle");

typedef unsigned int slate_oracle_typedef_u_int32_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_int32_t, u_int32_t), "typedef u_int32_t differs from oracle");

typedef unsigned long slate_oracle_typedef_u_int64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_int64_t, u_int64_t), "typedef u_int64_t differs from oracle");

typedef unsigned char slate_oracle_typedef_u_int8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_int8_t, u_int8_t), "typedef u_int8_t differs from oracle");

typedef unsigned long slate_oracle_typedef_u_long;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_long, u_long), "typedef u_long differs from oracle");

typedef unsigned long slate_oracle_typedef_u_quad_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_quad_t, u_quad_t), "typedef u_quad_t differs from oracle");

typedef unsigned short slate_oracle_typedef_u_short;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_short, u_short), "typedef u_short differs from oracle");

typedef unsigned int slate_oracle_typedef_uid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uid_t, uid_t), "typedef uid_t differs from oracle");

typedef unsigned int slate_oracle_typedef_uint;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uint, uint), "typedef uint differs from oracle");

typedef unsigned long slate_oracle_typedef_ulong;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ulong, ulong), "typedef ulong differs from oracle");

typedef unsigned int slate_oracle_typedef_useconds_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_useconds_t, useconds_t), "typedef useconds_t differs from oracle");

typedef unsigned short slate_oracle_typedef_ushort;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ushort, ushort), "typedef ushort differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_cond_s, __wseq) == 0, "struct __pthread_cond_s.__wseq offset differs from oracle");

typedef union union __atomic_wide_counter slate_oracle_struct___pthread_cond_s___wseq;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_cond_s *)0)->__wseq), slate_oracle_struct___pthread_cond_s___wseq), "struct __pthread_cond_s.__wseq field type differs from oracle");

typedef union union __atomic_wide_counter slate_oracle_struct___pthread_cond_s___g1_start;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_cond_s *)0)->__g1_start), slate_oracle_struct___pthread_cond_s___g1_start), "struct __pthread_cond_s.__g1_start field type differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_cond_s___g1_orig_size;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_cond_s *)0)->__g1_orig_size), slate_oracle_struct___pthread_cond_s___g1_orig_size), "struct __pthread_cond_s.__g1_orig_size field type differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_cond_s___wrefs;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_cond_s *)0)->__wrefs), slate_oracle_struct___pthread_cond_s___wrefs), "struct __pthread_cond_s.__wrefs field type differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_cond_s___unused_initialized_1;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_cond_s *)0)->__unused_initialized_1), slate_oracle_struct___pthread_cond_s___unused_initialized_1), "struct __pthread_cond_s.__unused_initialized_1 field type differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_cond_s___unused_initialized_2;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_cond_s *)0)->__unused_initialized_2), slate_oracle_struct___pthread_cond_s___unused_initialized_2), "struct __pthread_cond_s.__unused_initialized_2 field type differs from oracle");

typedef struct __pthread_internal_list * slate_oracle_struct___pthread_internal_list___prev;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_internal_list *)0)->__prev), slate_oracle_struct___pthread_internal_list___prev), "struct __pthread_internal_list.__prev field type differs from oracle");

typedef struct __pthread_internal_list * slate_oracle_struct___pthread_internal_list___next;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_internal_list *)0)->__next), slate_oracle_struct___pthread_internal_list___next), "struct __pthread_internal_list.__next field type differs from oracle");

typedef struct __pthread_internal_slist * slate_oracle_struct___pthread_internal_slist___next;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_internal_slist *)0)->__next), slate_oracle_struct___pthread_internal_slist___next), "struct __pthread_internal_slist.__next field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_mutex_s, __lock) == 0, "struct __pthread_mutex_s.__lock offset differs from oracle");

typedef int slate_oracle_struct___pthread_mutex_s___lock;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_mutex_s *)0)->__lock), slate_oracle_struct___pthread_mutex_s___lock), "struct __pthread_mutex_s.__lock field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_mutex_s, __count) == 4, "struct __pthread_mutex_s.__count offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_mutex_s___count;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_mutex_s *)0)->__count), slate_oracle_struct___pthread_mutex_s___count), "struct __pthread_mutex_s.__count field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_mutex_s, __owner) == 8, "struct __pthread_mutex_s.__owner offset differs from oracle");

typedef int slate_oracle_struct___pthread_mutex_s___owner;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_mutex_s *)0)->__owner), slate_oracle_struct___pthread_mutex_s___owner), "struct __pthread_mutex_s.__owner field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_mutex_s, __nusers) == 12, "struct __pthread_mutex_s.__nusers offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_mutex_s___nusers;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_mutex_s *)0)->__nusers), slate_oracle_struct___pthread_mutex_s___nusers), "struct __pthread_mutex_s.__nusers field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_mutex_s, __kind) == 16, "struct __pthread_mutex_s.__kind offset differs from oracle");

typedef int slate_oracle_struct___pthread_mutex_s___kind;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_mutex_s *)0)->__kind), slate_oracle_struct___pthread_mutex_s___kind), "struct __pthread_mutex_s.__kind field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_mutex_s, __spins) == 20, "struct __pthread_mutex_s.__spins offset differs from oracle");

typedef short slate_oracle_struct___pthread_mutex_s___spins;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_mutex_s *)0)->__spins), slate_oracle_struct___pthread_mutex_s___spins), "struct __pthread_mutex_s.__spins field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_mutex_s, __glibc_reserved) == 22, "struct __pthread_mutex_s.__glibc_reserved offset differs from oracle");

typedef short slate_oracle_struct___pthread_mutex_s___glibc_reserved;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_mutex_s *)0)->__glibc_reserved), slate_oracle_struct___pthread_mutex_s___glibc_reserved), "struct __pthread_mutex_s.__glibc_reserved field type differs from oracle");

typedef struct __pthread_internal_list slate_oracle_struct___pthread_mutex_s___list;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_mutex_s *)0)->__list), slate_oracle_struct___pthread_mutex_s___list), "struct __pthread_mutex_s.__list field type differs from oracle");

_Static_assert(sizeof(struct __pthread_rwlock_arch_t) == 56, "struct __pthread_rwlock_arch_t size differs from oracle");

_Static_assert(_Alignof(struct __pthread_rwlock_arch_t) == 8, "struct __pthread_rwlock_arch_t alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __readers) == 0, "struct __pthread_rwlock_arch_t.__readers offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_rwlock_arch_t___readers;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__readers), slate_oracle_struct___pthread_rwlock_arch_t___readers), "struct __pthread_rwlock_arch_t.__readers field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __writers) == 4, "struct __pthread_rwlock_arch_t.__writers offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_rwlock_arch_t___writers;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__writers), slate_oracle_struct___pthread_rwlock_arch_t___writers), "struct __pthread_rwlock_arch_t.__writers field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __wrphase_futex) == 8, "struct __pthread_rwlock_arch_t.__wrphase_futex offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_rwlock_arch_t___wrphase_futex;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__wrphase_futex), slate_oracle_struct___pthread_rwlock_arch_t___wrphase_futex), "struct __pthread_rwlock_arch_t.__wrphase_futex field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __writers_futex) == 12, "struct __pthread_rwlock_arch_t.__writers_futex offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_rwlock_arch_t___writers_futex;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__writers_futex), slate_oracle_struct___pthread_rwlock_arch_t___writers_futex), "struct __pthread_rwlock_arch_t.__writers_futex field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __pad3) == 16, "struct __pthread_rwlock_arch_t.__pad3 offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_rwlock_arch_t___pad3;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__pad3), slate_oracle_struct___pthread_rwlock_arch_t___pad3), "struct __pthread_rwlock_arch_t.__pad3 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __pad4) == 20, "struct __pthread_rwlock_arch_t.__pad4 offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_rwlock_arch_t___pad4;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__pad4), slate_oracle_struct___pthread_rwlock_arch_t___pad4), "struct __pthread_rwlock_arch_t.__pad4 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __cur_writer) == 24, "struct __pthread_rwlock_arch_t.__cur_writer offset differs from oracle");

typedef int slate_oracle_struct___pthread_rwlock_arch_t___cur_writer;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__cur_writer), slate_oracle_struct___pthread_rwlock_arch_t___cur_writer), "struct __pthread_rwlock_arch_t.__cur_writer field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __shared) == 28, "struct __pthread_rwlock_arch_t.__shared offset differs from oracle");

typedef int slate_oracle_struct___pthread_rwlock_arch_t___shared;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__shared), slate_oracle_struct___pthread_rwlock_arch_t___shared), "struct __pthread_rwlock_arch_t.__shared field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __pad1) == 32, "struct __pthread_rwlock_arch_t.__pad1 offset differs from oracle");

typedef unsigned long slate_oracle_struct___pthread_rwlock_arch_t___pad1;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__pad1), slate_oracle_struct___pthread_rwlock_arch_t___pad1), "struct __pthread_rwlock_arch_t.__pad1 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __pad2) == 40, "struct __pthread_rwlock_arch_t.__pad2 offset differs from oracle");

typedef unsigned long slate_oracle_struct___pthread_rwlock_arch_t___pad2;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__pad2), slate_oracle_struct___pthread_rwlock_arch_t___pad2), "struct __pthread_rwlock_arch_t.__pad2 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __flags) == 48, "struct __pthread_rwlock_arch_t.__flags offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_rwlock_arch_t___flags;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__flags), slate_oracle_struct___pthread_rwlock_arch_t___flags), "struct __pthread_rwlock_arch_t.__flags field type differs from oracle");

_Static_assert(sizeof(union pthread_attr_t) == 56, "union pthread_attr_t size differs from oracle");

_Static_assert(_Alignof(union pthread_attr_t) == 8, "union pthread_attr_t alignment differs from oracle");

_Static_assert(__builtin_offsetof(union pthread_attr_t, __size) == 0, "union pthread_attr_t.__size offset differs from oracle");

_Static_assert(__builtin_offsetof(union pthread_attr_t, __align) == 0, "union pthread_attr_t.__align offset differs from oracle");

typedef long slate_oracle_union_pthread_attr_t___align;
_Static_assert(__builtin_types_compatible_p(__typeof__((( union pthread_attr_t *)0)->__align), slate_oracle_union_pthread_attr_t___align), "union pthread_attr_t.__align field type differs from oracle");

_Static_assert(sizeof(struct timespec) == 16, "struct timespec size differs from oracle");

_Static_assert(_Alignof(struct timespec) == 8, "struct timespec alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct timespec, tv_sec) == 0, "struct timespec.tv_sec offset differs from oracle");

typedef long slate_oracle_struct_timespec_tv_sec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timespec *)0)->tv_sec), slate_oracle_struct_timespec_tv_sec), "struct timespec.tv_sec field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timespec, tv_nsec) == 8, "struct timespec.tv_nsec offset differs from oracle");

typedef long slate_oracle_struct_timespec_tv_nsec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timespec *)0)->tv_nsec), slate_oracle_struct_timespec_tv_nsec), "struct timespec.tv_nsec field type differs from oracle");

_Static_assert(sizeof(struct timeval) == 16, "struct timeval size differs from oracle");

_Static_assert(_Alignof(struct timeval) == 8, "struct timeval alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct timeval, tv_sec) == 0, "struct timeval.tv_sec offset differs from oracle");

typedef long slate_oracle_struct_timeval_tv_sec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timeval *)0)->tv_sec), slate_oracle_struct_timeval_tv_sec), "struct timeval.tv_sec field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timeval, tv_usec) == 8, "struct timeval.tv_usec offset differs from oracle");

typedef long slate_oracle_struct_timeval_tv_usec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timeval *)0)->tv_usec), slate_oracle_struct_timeval_tv_usec), "struct timeval.tv_usec field type differs from oracle");

#ifndef BIG_ENDIAN
#error "sys/types.h:BIG_ENDIAN macro is missing from libc-shim"
#endif

#ifndef BYTE_ORDER
#error "sys/types.h:BYTE_ORDER macro is missing from libc-shim"
#endif

#ifndef FD_CLR
#error "sys/types.h:FD_CLR macro is missing from libc-shim"
#endif

#ifndef FD_ISSET
#error "sys/types.h:FD_ISSET macro is missing from libc-shim"
#endif

#ifndef FD_SET
#error "sys/types.h:FD_SET macro is missing from libc-shim"
#endif

#ifndef FD_SETSIZE
#error "sys/types.h:FD_SETSIZE macro is missing from libc-shim"
#endif

#ifndef FD_ZERO
#error "sys/types.h:FD_ZERO macro is missing from libc-shim"
#endif

#ifndef LITTLE_ENDIAN
#error "sys/types.h:LITTLE_ENDIAN macro is missing from libc-shim"
#endif

#ifndef NFDBITS
#error "sys/types.h:NFDBITS macro is missing from libc-shim"
#endif

#ifndef PDP_ENDIAN
#error "sys/types.h:PDP_ENDIAN macro is missing from libc-shim"
#endif

#ifndef _ATFILE_SOURCE
#error "sys/types.h:_ATFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _BITS_ATOMIC_WIDE_COUNTER_H
#error "sys/types.h:_BITS_ATOMIC_WIDE_COUNTER_H macro is missing from libc-shim"
#endif

#ifndef _BITS_BYTESWAP_H
#error "sys/types.h:_BITS_BYTESWAP_H macro is missing from libc-shim"
#endif

#ifndef _BITS_ENDIANNESS_H
#error "sys/types.h:_BITS_ENDIANNESS_H macro is missing from libc-shim"
#endif

#ifndef _BITS_ENDIAN_H
#error "sys/types.h:_BITS_ENDIAN_H macro is missing from libc-shim"
#endif

#ifndef _BITS_PTHREADTYPES_ARCH_H
#error "sys/types.h:_BITS_PTHREADTYPES_ARCH_H macro is missing from libc-shim"
#endif

#ifndef _BITS_PTHREADTYPES_COMMON_H
#error "sys/types.h:_BITS_PTHREADTYPES_COMMON_H macro is missing from libc-shim"
#endif

#ifndef _BITS_STDINT_INTN_H
#error "sys/types.h:_BITS_STDINT_INTN_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TIME64_H
#error "sys/types.h:_BITS_TIME64_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TYPESIZES_H
#error "sys/types.h:_BITS_TYPESIZES_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TYPES_H
#error "sys/types.h:_BITS_TYPES_H macro is missing from libc-shim"
#endif

#ifndef _BITS_UINTN_IDENTITY_H
#error "sys/types.h:_BITS_UINTN_IDENTITY_H macro is missing from libc-shim"
#endif

#ifndef _DEFAULT_SOURCE
#error "sys/types.h:_DEFAULT_SOURCE macro is missing from libc-shim"
#endif

#ifndef _DYNAMIC_STACK_SIZE_SOURCE
#error "sys/types.h:_DYNAMIC_STACK_SIZE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ENDIAN_H
#error "sys/types.h:_ENDIAN_H macro is missing from libc-shim"
#endif

#ifndef _FEATURES_H
#error "sys/types.h:_FEATURES_H macro is missing from libc-shim"
#endif

#ifndef _ISOC11_SOURCE
#error "sys/types.h:_ISOC11_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC23_SOURCE
#error "sys/types.h:_ISOC23_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC2Y_SOURCE
#error "sys/types.h:_ISOC2Y_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC95_SOURCE
#error "sys/types.h:_ISOC95_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC99_SOURCE
#error "sys/types.h:_ISOC99_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE64_SOURCE
#error "sys/types.h:_LARGEFILE64_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE_SOURCE
#error "sys/types.h:_LARGEFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_C_SOURCE
#error "sys/types.h:_POSIX_C_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_SOURCE
#error "sys/types.h:_POSIX_SOURCE macro is missing from libc-shim"
#endif

#ifndef _RWLOCK_INTERNAL_H
#error "sys/types.h:_RWLOCK_INTERNAL_H macro is missing from libc-shim"
#endif

#ifndef _SIGSET_NWORDS
#error "sys/types.h:_SIGSET_NWORDS macro is missing from libc-shim"
#endif

#ifndef _SIZE_T
#error "sys/types.h:_SIZE_T macro is missing from libc-shim"
#endif

#ifndef _STDC_PREDEF_H
#error "sys/types.h:_STDC_PREDEF_H macro is missing from libc-shim"
#endif

#ifndef _STRUCT_TIMESPEC
#error "sys/types.h:_STRUCT_TIMESPEC macro is missing from libc-shim"
#endif

#ifndef _SYS_CDEFS_H
#error "sys/types.h:_SYS_CDEFS_H macro is missing from libc-shim"
#endif

#ifndef _SYS_SELECT_H
#error "sys/types.h:_SYS_SELECT_H macro is missing from libc-shim"
#endif

#ifndef _SYS_TYPES_H
#error "sys/types.h:_SYS_TYPES_H macro is missing from libc-shim"
#endif

#ifndef _THREAD_MUTEX_INTERNAL_H
#error "sys/types.h:_THREAD_MUTEX_INTERNAL_H macro is missing from libc-shim"
#endif

#ifndef _THREAD_SHARED_TYPES_H
#error "sys/types.h:_THREAD_SHARED_TYPES_H macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE
#error "sys/types.h:_XOPEN_SOURCE macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE_EXTENDED
#error "sys/types.h:_XOPEN_SOURCE_EXTENDED macro is missing from libc-shim"
#endif

#ifndef be16toh
#error "sys/types.h:be16toh macro is missing from libc-shim"
#endif

#ifndef be32toh
#error "sys/types.h:be32toh macro is missing from libc-shim"
#endif

#ifndef be64toh
#error "sys/types.h:be64toh macro is missing from libc-shim"
#endif

#ifndef htobe16
#error "sys/types.h:htobe16 macro is missing from libc-shim"
#endif

#ifndef htobe32
#error "sys/types.h:htobe32 macro is missing from libc-shim"
#endif

#ifndef htobe64
#error "sys/types.h:htobe64 macro is missing from libc-shim"
#endif

#ifndef htole16
#error "sys/types.h:htole16 macro is missing from libc-shim"
#endif

#ifndef htole32
#error "sys/types.h:htole32 macro is missing from libc-shim"
#endif

#ifndef htole64
#error "sys/types.h:htole64 macro is missing from libc-shim"
#endif

#ifndef le16toh
#error "sys/types.h:le16toh macro is missing from libc-shim"
#endif

#ifndef le32toh
#error "sys/types.h:le32toh macro is missing from libc-shim"
#endif

#ifndef le64toh
#error "sys/types.h:le64toh macro is missing from libc-shim"
#endif

int main(void) { return 0; }
