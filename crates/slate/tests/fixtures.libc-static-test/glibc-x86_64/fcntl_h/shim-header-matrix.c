#include <fcntl.h>

extern int slate_oracle_creat(const char *, unsigned int);
extern int slate_oracle_creat64(const char *, unsigned int);
extern int slate_oracle_fallocate(int, int, long, long);
extern int slate_oracle_fallocate64(int, int, long, long);
extern int slate_oracle_fcntl(int, int, ...);
extern int slate_oracle_fcntl64(int, int, ...);
extern int slate_oracle_lockf(int, int, long);
extern int slate_oracle_lockf64(int, int, long);
extern int slate_oracle_name_to_handle_at(int, const char *, struct file_handle *, int *, int);
extern int slate_oracle_open(const char *, int, ...);
extern int slate_oracle_open64(const char *, int, ...);
extern int slate_oracle_open_by_handle_at(int, struct file_handle *, int);
extern int slate_oracle_openat(int, const char *, int, ...);
extern int slate_oracle_openat2(int, const char *, const struct open_how *, unsigned long);
extern int slate_oracle_openat64(int, const char *, int, ...);
extern int slate_oracle_posix_fadvise(int, long, long, int);
extern int slate_oracle_posix_fadvise64(int, long, long, int);
extern int slate_oracle_posix_fallocate(int, long, long);
extern int slate_oracle_posix_fallocate64(int, long, long);
extern long slate_oracle_readahead(int, long, unsigned long);
extern long slate_oracle_splice(int, long *, int, long *, unsigned long, unsigned int);
extern int slate_oracle_sync_file_range(int, long, long, unsigned int);
extern long slate_oracle_tee(int, int, unsigned long, unsigned int);
extern long slate_oracle_vmsplice(int, const struct iovec *, unsigned long, unsigned int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_creat), __typeof__(creat)),
    "fcntl.h:creat declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_creat64), __typeof__(creat64)),
    "fcntl.h:creat64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fallocate), __typeof__(fallocate)),
    "fcntl.h:fallocate declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fallocate64), __typeof__(fallocate64)),
    "fcntl.h:fallocate64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fcntl), __typeof__(fcntl)),
    "fcntl.h:fcntl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fcntl64), __typeof__(fcntl64)),
    "fcntl.h:fcntl64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lockf), __typeof__(lockf)),
    "fcntl.h:lockf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lockf64), __typeof__(lockf64)),
    "fcntl.h:lockf64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_name_to_handle_at), __typeof__(name_to_handle_at)),
    "fcntl.h:name_to_handle_at declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_open), __typeof__(open)),
    "fcntl.h:open declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_open64), __typeof__(open64)),
    "fcntl.h:open64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_open_by_handle_at), __typeof__(open_by_handle_at)),
    "fcntl.h:open_by_handle_at declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_openat), __typeof__(openat)),
    "fcntl.h:openat declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_openat2), __typeof__(openat2)),
    "fcntl.h:openat2 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_openat64), __typeof__(openat64)),
    "fcntl.h:openat64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_posix_fadvise), __typeof__(posix_fadvise)),
    "fcntl.h:posix_fadvise declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_posix_fadvise64), __typeof__(posix_fadvise64)),
    "fcntl.h:posix_fadvise64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_posix_fallocate), __typeof__(posix_fallocate)),
    "fcntl.h:posix_fallocate declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_posix_fallocate64), __typeof__(posix_fallocate64)),
    "fcntl.h:posix_fallocate64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_readahead), __typeof__(readahead)),
    "fcntl.h:readahead declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_splice), __typeof__(splice)),
    "fcntl.h:splice declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sync_file_range), __typeof__(sync_file_range)),
    "fcntl.h:sync_file_range declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tee), __typeof__(tee)),
    "fcntl.h:tee declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_vmsplice), __typeof__(vmsplice)),
    "fcntl.h:vmsplice declaration differs from oracle");

static __typeof__(creat) *const slate_reference_creat = &creat;
static __typeof__(creat64) *const slate_reference_creat64 = &creat64;
static __typeof__(fallocate) *const slate_reference_fallocate = &fallocate;
static __typeof__(fallocate64) *const slate_reference_fallocate64 = &fallocate64;
static __typeof__(fcntl) *const slate_reference_fcntl = &fcntl;
static __typeof__(fcntl64) *const slate_reference_fcntl64 = &fcntl64;
static __typeof__(lockf) *const slate_reference_lockf = &lockf;
static __typeof__(lockf64) *const slate_reference_lockf64 = &lockf64;
static __typeof__(name_to_handle_at) *const slate_reference_name_to_handle_at = &name_to_handle_at;
static __typeof__(open) *const slate_reference_open = &open;
static __typeof__(open64) *const slate_reference_open64 = &open64;
static __typeof__(open_by_handle_at) *const slate_reference_open_by_handle_at = &open_by_handle_at;
static __typeof__(openat) *const slate_reference_openat = &openat;
static __typeof__(openat2) *const slate_reference_openat2 = &openat2;
static __typeof__(openat64) *const slate_reference_openat64 = &openat64;
static __typeof__(posix_fadvise) *const slate_reference_posix_fadvise = &posix_fadvise;
static __typeof__(posix_fadvise64) *const slate_reference_posix_fadvise64 = &posix_fadvise64;
static __typeof__(posix_fallocate) *const slate_reference_posix_fallocate = &posix_fallocate;
static __typeof__(posix_fallocate64) *const slate_reference_posix_fallocate64 = &posix_fallocate64;
static __typeof__(readahead) *const slate_reference_readahead = &readahead;
static __typeof__(splice) *const slate_reference_splice = &splice;
static __typeof__(sync_file_range) *const slate_reference_sync_file_range = &sync_file_range;
static __typeof__(tee) *const slate_reference_tee = &tee;
static __typeof__(vmsplice) *const slate_reference_vmsplice = &vmsplice;

typedef unsigned int slate_oracle_typedef_mode_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_mode_t, mode_t), "typedef mode_t differs from oracle");

typedef long slate_oracle_typedef_off64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_off64_t, off64_t), "typedef off64_t differs from oracle");

typedef long slate_oracle_typedef_off_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_off_t, off_t), "typedef off_t differs from oracle");

typedef int slate_oracle_typedef_pid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pid_t, pid_t), "typedef pid_t differs from oracle");

typedef unsigned long slate_oracle_typedef_size_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_size_t, size_t), "typedef size_t differs from oracle");

typedef long slate_oracle_typedef_time_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_time_t, time_t), "typedef time_t differs from oracle");

_Static_assert(sizeof(struct f_owner_ex) == 8, "struct f_owner_ex size differs from oracle");

_Static_assert(_Alignof(struct f_owner_ex) == 4, "struct f_owner_ex alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct f_owner_ex, type) == 0, "struct f_owner_ex.type offset differs from oracle");

typedef enum __pid_type slate_oracle_struct_f_owner_ex_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct f_owner_ex *)0)->type), slate_oracle_struct_f_owner_ex_type), "struct f_owner_ex.type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct f_owner_ex, pid) == 4, "struct f_owner_ex.pid offset differs from oracle");

typedef int slate_oracle_struct_f_owner_ex_pid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct f_owner_ex *)0)->pid), slate_oracle_struct_f_owner_ex_pid), "struct f_owner_ex.pid field type differs from oracle");

_Static_assert(sizeof(struct file_handle) == 8, "struct file_handle size differs from oracle");

_Static_assert(_Alignof(struct file_handle) == 4, "struct file_handle alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct file_handle, handle_bytes) == 0, "struct file_handle.handle_bytes offset differs from oracle");

typedef unsigned int slate_oracle_struct_file_handle_handle_bytes;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct file_handle *)0)->handle_bytes), slate_oracle_struct_file_handle_handle_bytes), "struct file_handle.handle_bytes field type differs from oracle");

_Static_assert(__builtin_offsetof(struct file_handle, handle_type) == 4, "struct file_handle.handle_type offset differs from oracle");

typedef int slate_oracle_struct_file_handle_handle_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct file_handle *)0)->handle_type), slate_oracle_struct_file_handle_handle_type), "struct file_handle.handle_type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct file_handle, f_handle) == 8, "struct file_handle.f_handle offset differs from oracle");

_Static_assert(sizeof(struct flock) == 32, "struct flock size differs from oracle");

_Static_assert(_Alignof(struct flock) == 8, "struct flock alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct flock, l_type) == 0, "struct flock.l_type offset differs from oracle");

typedef short slate_oracle_struct_flock_l_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct flock *)0)->l_type), slate_oracle_struct_flock_l_type), "struct flock.l_type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct flock, l_whence) == 2, "struct flock.l_whence offset differs from oracle");

typedef short slate_oracle_struct_flock_l_whence;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct flock *)0)->l_whence), slate_oracle_struct_flock_l_whence), "struct flock.l_whence field type differs from oracle");

_Static_assert(__builtin_offsetof(struct flock, l_start) == 8, "struct flock.l_start offset differs from oracle");

typedef long slate_oracle_struct_flock_l_start;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct flock *)0)->l_start), slate_oracle_struct_flock_l_start), "struct flock.l_start field type differs from oracle");

_Static_assert(__builtin_offsetof(struct flock, l_len) == 16, "struct flock.l_len offset differs from oracle");

typedef long slate_oracle_struct_flock_l_len;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct flock *)0)->l_len), slate_oracle_struct_flock_l_len), "struct flock.l_len field type differs from oracle");

_Static_assert(__builtin_offsetof(struct flock, l_pid) == 24, "struct flock.l_pid offset differs from oracle");

typedef int slate_oracle_struct_flock_l_pid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct flock *)0)->l_pid), slate_oracle_struct_flock_l_pid), "struct flock.l_pid field type differs from oracle");

_Static_assert(sizeof(struct flock64) == 32, "struct flock64 size differs from oracle");

_Static_assert(_Alignof(struct flock64) == 8, "struct flock64 alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct flock64, l_type) == 0, "struct flock64.l_type offset differs from oracle");

typedef short slate_oracle_struct_flock64_l_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct flock64 *)0)->l_type), slate_oracle_struct_flock64_l_type), "struct flock64.l_type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct flock64, l_whence) == 2, "struct flock64.l_whence offset differs from oracle");

typedef short slate_oracle_struct_flock64_l_whence;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct flock64 *)0)->l_whence), slate_oracle_struct_flock64_l_whence), "struct flock64.l_whence field type differs from oracle");

_Static_assert(__builtin_offsetof(struct flock64, l_start) == 8, "struct flock64.l_start offset differs from oracle");

typedef long slate_oracle_struct_flock64_l_start;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct flock64 *)0)->l_start), slate_oracle_struct_flock64_l_start), "struct flock64.l_start field type differs from oracle");

_Static_assert(__builtin_offsetof(struct flock64, l_len) == 16, "struct flock64.l_len offset differs from oracle");

typedef long slate_oracle_struct_flock64_l_len;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct flock64 *)0)->l_len), slate_oracle_struct_flock64_l_len), "struct flock64.l_len field type differs from oracle");

_Static_assert(__builtin_offsetof(struct flock64, l_pid) == 24, "struct flock64.l_pid offset differs from oracle");

typedef int slate_oracle_struct_flock64_l_pid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct flock64 *)0)->l_pid), slate_oracle_struct_flock64_l_pid), "struct flock64.l_pid field type differs from oracle");

_Static_assert(sizeof(struct iovec) == 16, "struct iovec size differs from oracle");

_Static_assert(_Alignof(struct iovec) == 8, "struct iovec alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct iovec, iov_base) == 0, "struct iovec.iov_base offset differs from oracle");

typedef void * slate_oracle_struct_iovec_iov_base;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct iovec *)0)->iov_base), slate_oracle_struct_iovec_iov_base), "struct iovec.iov_base field type differs from oracle");

_Static_assert(__builtin_offsetof(struct iovec, iov_len) == 8, "struct iovec.iov_len offset differs from oracle");

typedef unsigned long slate_oracle_struct_iovec_iov_len;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct iovec *)0)->iov_len), slate_oracle_struct_iovec_iov_len), "struct iovec.iov_len field type differs from oracle");

_Static_assert(sizeof(struct open_how) == 24, "struct open_how size differs from oracle");

_Static_assert(_Alignof(struct open_how) == 8, "struct open_how alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct open_how, flags) == 0, "struct open_how.flags offset differs from oracle");

typedef unsigned long long slate_oracle_struct_open_how_flags;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct open_how *)0)->flags), slate_oracle_struct_open_how_flags), "struct open_how.flags field type differs from oracle");

_Static_assert(__builtin_offsetof(struct open_how, mode) == 8, "struct open_how.mode offset differs from oracle");

typedef unsigned long long slate_oracle_struct_open_how_mode;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct open_how *)0)->mode), slate_oracle_struct_open_how_mode), "struct open_how.mode field type differs from oracle");

_Static_assert(__builtin_offsetof(struct open_how, resolve) == 16, "struct open_how.resolve offset differs from oracle");

typedef unsigned long long slate_oracle_struct_open_how_resolve;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct open_how *)0)->resolve), slate_oracle_struct_open_how_resolve), "struct open_how.resolve field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_dev) == 0, "struct stat.st_dev offset differs from oracle");

typedef unsigned long slate_oracle_struct_stat_st_dev;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_dev), slate_oracle_struct_stat_st_dev), "struct stat.st_dev field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_ino) == 8, "struct stat.st_ino offset differs from oracle");

typedef unsigned long slate_oracle_struct_stat_st_ino;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_ino), slate_oracle_struct_stat_st_ino), "struct stat.st_ino field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_nlink) == 16, "struct stat.st_nlink offset differs from oracle");

typedef unsigned long slate_oracle_struct_stat_st_nlink;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_nlink), slate_oracle_struct_stat_st_nlink), "struct stat.st_nlink field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_mode) == 24, "struct stat.st_mode offset differs from oracle");

typedef unsigned int slate_oracle_struct_stat_st_mode;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_mode), slate_oracle_struct_stat_st_mode), "struct stat.st_mode field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_uid) == 28, "struct stat.st_uid offset differs from oracle");

typedef unsigned int slate_oracle_struct_stat_st_uid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_uid), slate_oracle_struct_stat_st_uid), "struct stat.st_uid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_gid) == 32, "struct stat.st_gid offset differs from oracle");

typedef unsigned int slate_oracle_struct_stat_st_gid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_gid), slate_oracle_struct_stat_st_gid), "struct stat.st_gid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, __pad0) == 36, "struct stat.__pad0 offset differs from oracle");

typedef int slate_oracle_struct_stat___pad0;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->__pad0), slate_oracle_struct_stat___pad0), "struct stat.__pad0 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_rdev) == 40, "struct stat.st_rdev offset differs from oracle");

typedef unsigned long slate_oracle_struct_stat_st_rdev;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_rdev), slate_oracle_struct_stat_st_rdev), "struct stat.st_rdev field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_size) == 48, "struct stat.st_size offset differs from oracle");

typedef long slate_oracle_struct_stat_st_size;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_size), slate_oracle_struct_stat_st_size), "struct stat.st_size field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_blksize) == 56, "struct stat.st_blksize offset differs from oracle");

typedef long slate_oracle_struct_stat_st_blksize;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_blksize), slate_oracle_struct_stat_st_blksize), "struct stat.st_blksize field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat, st_blocks) == 64, "struct stat.st_blocks offset differs from oracle");

typedef long slate_oracle_struct_stat_st_blocks;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_blocks), slate_oracle_struct_stat_st_blocks), "struct stat.st_blocks field type differs from oracle");

typedef struct timespec slate_oracle_struct_stat_st_atim;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_atim), slate_oracle_struct_stat_st_atim), "struct stat.st_atim field type differs from oracle");

typedef struct timespec slate_oracle_struct_stat_st_mtim;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_mtim), slate_oracle_struct_stat_st_mtim), "struct stat.st_mtim field type differs from oracle");

typedef struct timespec slate_oracle_struct_stat_st_ctim;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat *)0)->st_ctim), slate_oracle_struct_stat_st_ctim), "struct stat.st_ctim field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat64, st_dev) == 0, "struct stat64.st_dev offset differs from oracle");

typedef unsigned long slate_oracle_struct_stat64_st_dev;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat64 *)0)->st_dev), slate_oracle_struct_stat64_st_dev), "struct stat64.st_dev field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat64, st_ino) == 8, "struct stat64.st_ino offset differs from oracle");

typedef unsigned long slate_oracle_struct_stat64_st_ino;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat64 *)0)->st_ino), slate_oracle_struct_stat64_st_ino), "struct stat64.st_ino field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat64, st_nlink) == 16, "struct stat64.st_nlink offset differs from oracle");

typedef unsigned long slate_oracle_struct_stat64_st_nlink;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat64 *)0)->st_nlink), slate_oracle_struct_stat64_st_nlink), "struct stat64.st_nlink field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat64, st_mode) == 24, "struct stat64.st_mode offset differs from oracle");

typedef unsigned int slate_oracle_struct_stat64_st_mode;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat64 *)0)->st_mode), slate_oracle_struct_stat64_st_mode), "struct stat64.st_mode field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat64, st_uid) == 28, "struct stat64.st_uid offset differs from oracle");

typedef unsigned int slate_oracle_struct_stat64_st_uid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat64 *)0)->st_uid), slate_oracle_struct_stat64_st_uid), "struct stat64.st_uid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat64, st_gid) == 32, "struct stat64.st_gid offset differs from oracle");

typedef unsigned int slate_oracle_struct_stat64_st_gid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat64 *)0)->st_gid), slate_oracle_struct_stat64_st_gid), "struct stat64.st_gid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat64, __pad0) == 36, "struct stat64.__pad0 offset differs from oracle");

typedef int slate_oracle_struct_stat64___pad0;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat64 *)0)->__pad0), slate_oracle_struct_stat64___pad0), "struct stat64.__pad0 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat64, st_rdev) == 40, "struct stat64.st_rdev offset differs from oracle");

typedef unsigned long slate_oracle_struct_stat64_st_rdev;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat64 *)0)->st_rdev), slate_oracle_struct_stat64_st_rdev), "struct stat64.st_rdev field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat64, st_size) == 48, "struct stat64.st_size offset differs from oracle");

typedef long slate_oracle_struct_stat64_st_size;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat64 *)0)->st_size), slate_oracle_struct_stat64_st_size), "struct stat64.st_size field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat64, st_blksize) == 56, "struct stat64.st_blksize offset differs from oracle");

typedef long slate_oracle_struct_stat64_st_blksize;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat64 *)0)->st_blksize), slate_oracle_struct_stat64_st_blksize), "struct stat64.st_blksize field type differs from oracle");

_Static_assert(__builtin_offsetof(struct stat64, st_blocks) == 64, "struct stat64.st_blocks offset differs from oracle");

typedef long slate_oracle_struct_stat64_st_blocks;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat64 *)0)->st_blocks), slate_oracle_struct_stat64_st_blocks), "struct stat64.st_blocks field type differs from oracle");

typedef struct timespec slate_oracle_struct_stat64_st_atim;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat64 *)0)->st_atim), slate_oracle_struct_stat64_st_atim), "struct stat64.st_atim field type differs from oracle");

typedef struct timespec slate_oracle_struct_stat64_st_mtim;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat64 *)0)->st_mtim), slate_oracle_struct_stat64_st_mtim), "struct stat64.st_mtim field type differs from oracle");

typedef struct timespec slate_oracle_struct_stat64_st_ctim;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct stat64 *)0)->st_ctim), slate_oracle_struct_stat64_st_ctim), "struct stat64.st_ctim field type differs from oracle");

_Static_assert(sizeof(struct timespec) == 16, "struct timespec size differs from oracle");

_Static_assert(_Alignof(struct timespec) == 8, "struct timespec alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct timespec, tv_sec) == 0, "struct timespec.tv_sec offset differs from oracle");

typedef long slate_oracle_struct_timespec_tv_sec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timespec *)0)->tv_sec), slate_oracle_struct_timespec_tv_sec), "struct timespec.tv_sec field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timespec, tv_nsec) == 8, "struct timespec.tv_nsec offset differs from oracle");

typedef long slate_oracle_struct_timespec_tv_nsec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timespec *)0)->tv_nsec), slate_oracle_struct_timespec_tv_nsec), "struct timespec.tv_nsec field type differs from oracle");

_Static_assert(__builtin_types_compatible_p(__typeof__(F_OWNER_TID), __typeof__((int)0)), "enum F_OWNER_TID type differs from oracle");

_Static_assert(F_OWNER_TID == (0), "enum F_OWNER_TID value differs from oracle");

_Static_assert(__builtin_types_compatible_p(__typeof__(F_OWNER_PID), __typeof__((int)0)), "enum F_OWNER_PID type differs from oracle");

_Static_assert(__builtin_types_compatible_p(__typeof__(F_OWNER_PGRP), __typeof__((int)0)), "enum F_OWNER_PGRP type differs from oracle");

_Static_assert(__builtin_types_compatible_p(__typeof__(F_OWNER_GID), __typeof__((int)0)), "enum F_OWNER_GID type differs from oracle");

_Static_assert(F_OWNER_GID == (2), "enum F_OWNER_GID value differs from oracle");

#ifndef AT_EACCESS
#error "fcntl.h:AT_EACCESS macro is missing from libc-shim"
#endif

#ifndef AT_EMPTY_PATH
#error "fcntl.h:AT_EMPTY_PATH macro is missing from libc-shim"
#endif

#ifndef AT_EXECVE_CHECK
#error "fcntl.h:AT_EXECVE_CHECK macro is missing from libc-shim"
#endif

#ifndef AT_FDCWD
#error "fcntl.h:AT_FDCWD macro is missing from libc-shim"
#endif

#ifndef AT_HANDLE_CONNECTABLE
#error "fcntl.h:AT_HANDLE_CONNECTABLE macro is missing from libc-shim"
#endif

#ifndef AT_HANDLE_FID
#error "fcntl.h:AT_HANDLE_FID macro is missing from libc-shim"
#endif

#ifndef AT_HANDLE_MNT_ID_UNIQUE
#error "fcntl.h:AT_HANDLE_MNT_ID_UNIQUE macro is missing from libc-shim"
#endif

#ifndef AT_NO_AUTOMOUNT
#error "fcntl.h:AT_NO_AUTOMOUNT macro is missing from libc-shim"
#endif

#ifndef AT_RECURSIVE
#error "fcntl.h:AT_RECURSIVE macro is missing from libc-shim"
#endif

#ifndef AT_REMOVEDIR
#error "fcntl.h:AT_REMOVEDIR macro is missing from libc-shim"
#endif

#ifndef AT_STATX_DONT_SYNC
#error "fcntl.h:AT_STATX_DONT_SYNC macro is missing from libc-shim"
#endif

#ifndef AT_STATX_FORCE_SYNC
#error "fcntl.h:AT_STATX_FORCE_SYNC macro is missing from libc-shim"
#endif

#ifndef AT_STATX_SYNC_AS_STAT
#error "fcntl.h:AT_STATX_SYNC_AS_STAT macro is missing from libc-shim"
#endif

#ifndef AT_STATX_SYNC_TYPE
#error "fcntl.h:AT_STATX_SYNC_TYPE macro is missing from libc-shim"
#endif

#ifndef AT_SYMLINK_FOLLOW
#error "fcntl.h:AT_SYMLINK_FOLLOW macro is missing from libc-shim"
#endif

#ifndef AT_SYMLINK_NOFOLLOW
#error "fcntl.h:AT_SYMLINK_NOFOLLOW macro is missing from libc-shim"
#endif

#ifndef DN_ACCESS
#error "fcntl.h:DN_ACCESS macro is missing from libc-shim"
#endif

#ifndef DN_ATTRIB
#error "fcntl.h:DN_ATTRIB macro is missing from libc-shim"
#endif

#ifndef DN_CREATE
#error "fcntl.h:DN_CREATE macro is missing from libc-shim"
#endif

#ifndef DN_DELETE
#error "fcntl.h:DN_DELETE macro is missing from libc-shim"
#endif

#ifndef DN_MODIFY
#error "fcntl.h:DN_MODIFY macro is missing from libc-shim"
#endif

#ifndef DN_MULTISHOT
#error "fcntl.h:DN_MULTISHOT macro is missing from libc-shim"
#endif

#ifndef DN_RENAME
#error "fcntl.h:DN_RENAME macro is missing from libc-shim"
#endif

#ifndef FALLOC_FL_ALLOCATE_RANGE
#error "fcntl.h:FALLOC_FL_ALLOCATE_RANGE macro is missing from libc-shim"
#endif

#ifndef FALLOC_FL_COLLAPSE_RANGE
#error "fcntl.h:FALLOC_FL_COLLAPSE_RANGE macro is missing from libc-shim"
#endif

#ifndef FALLOC_FL_INSERT_RANGE
#error "fcntl.h:FALLOC_FL_INSERT_RANGE macro is missing from libc-shim"
#endif

#ifndef FALLOC_FL_KEEP_SIZE
#error "fcntl.h:FALLOC_FL_KEEP_SIZE macro is missing from libc-shim"
#endif

#ifndef FALLOC_FL_NO_HIDE_STALE
#error "fcntl.h:FALLOC_FL_NO_HIDE_STALE macro is missing from libc-shim"
#endif

#ifndef FALLOC_FL_PUNCH_HOLE
#error "fcntl.h:FALLOC_FL_PUNCH_HOLE macro is missing from libc-shim"
#endif

#ifndef FALLOC_FL_UNSHARE_RANGE
#error "fcntl.h:FALLOC_FL_UNSHARE_RANGE macro is missing from libc-shim"
#endif

#ifndef FALLOC_FL_WRITE_ZEROES
#error "fcntl.h:FALLOC_FL_WRITE_ZEROES macro is missing from libc-shim"
#endif

#ifndef FALLOC_FL_ZERO_RANGE
#error "fcntl.h:FALLOC_FL_ZERO_RANGE macro is missing from libc-shim"
#endif

#ifndef FAPPEND
#error "fcntl.h:FAPPEND macro is missing from libc-shim"
#endif

#ifndef FASYNC
#error "fcntl.h:FASYNC macro is missing from libc-shim"
#endif

#ifndef FD_CLOEXEC
#error "fcntl.h:FD_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef FD_NSFS_ROOT
#error "fcntl.h:FD_NSFS_ROOT macro is missing from libc-shim"
#endif

#ifndef FD_PIDFS_ROOT
#error "fcntl.h:FD_PIDFS_ROOT macro is missing from libc-shim"
#endif

#ifndef FFSYNC
#error "fcntl.h:FFSYNC macro is missing from libc-shim"
#endif

#ifndef FNDELAY
#error "fcntl.h:FNDELAY macro is missing from libc-shim"
#endif

#ifndef FNONBLOCK
#error "fcntl.h:FNONBLOCK macro is missing from libc-shim"
#endif

#ifndef F_ADD_SEALS
#error "fcntl.h:F_ADD_SEALS macro is missing from libc-shim"
#endif

#ifndef F_CREATED_QUERY
#error "fcntl.h:F_CREATED_QUERY macro is missing from libc-shim"
#endif

#ifndef F_DUPFD
#error "fcntl.h:F_DUPFD macro is missing from libc-shim"
#endif

#ifndef F_DUPFD_CLOEXEC
#error "fcntl.h:F_DUPFD_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef F_DUPFD_QUERY
#error "fcntl.h:F_DUPFD_QUERY macro is missing from libc-shim"
#endif

#ifndef F_EXLCK
#error "fcntl.h:F_EXLCK macro is missing from libc-shim"
#endif

#ifndef F_GETDELEG
#error "fcntl.h:F_GETDELEG macro is missing from libc-shim"
#endif

#ifndef F_GETFD
#error "fcntl.h:F_GETFD macro is missing from libc-shim"
#endif

#ifndef F_GETFL
#error "fcntl.h:F_GETFL macro is missing from libc-shim"
#endif

#ifndef F_GETLEASE
#error "fcntl.h:F_GETLEASE macro is missing from libc-shim"
#endif

#ifndef F_GETLK
#error "fcntl.h:F_GETLK macro is missing from libc-shim"
#endif

#ifndef F_GETLK64
#error "fcntl.h:F_GETLK64 macro is missing from libc-shim"
#endif

#ifndef F_GETOWN
#error "fcntl.h:F_GETOWN macro is missing from libc-shim"
#endif

#ifndef F_GETOWN_EX
#error "fcntl.h:F_GETOWN_EX macro is missing from libc-shim"
#endif

#ifndef F_GETPIPE_SZ
#error "fcntl.h:F_GETPIPE_SZ macro is missing from libc-shim"
#endif

#ifndef F_GETSIG
#error "fcntl.h:F_GETSIG macro is missing from libc-shim"
#endif

#ifndef F_GET_FILE_RW_HINT
#error "fcntl.h:F_GET_FILE_RW_HINT macro is missing from libc-shim"
#endif

#ifndef F_GET_RW_HINT
#error "fcntl.h:F_GET_RW_HINT macro is missing from libc-shim"
#endif

#ifndef F_GET_SEALS
#error "fcntl.h:F_GET_SEALS macro is missing from libc-shim"
#endif

#ifndef F_LOCK
#error "fcntl.h:F_LOCK macro is missing from libc-shim"
#endif

#ifndef F_NOTIFY
#error "fcntl.h:F_NOTIFY macro is missing from libc-shim"
#endif

#ifndef F_OFD_GETLK
#error "fcntl.h:F_OFD_GETLK macro is missing from libc-shim"
#endif

#ifndef F_OFD_SETLK
#error "fcntl.h:F_OFD_SETLK macro is missing from libc-shim"
#endif

#ifndef F_OFD_SETLKW
#error "fcntl.h:F_OFD_SETLKW macro is missing from libc-shim"
#endif

#ifndef F_OK
#error "fcntl.h:F_OK macro is missing from libc-shim"
#endif

#ifndef F_RDLCK
#error "fcntl.h:F_RDLCK macro is missing from libc-shim"
#endif

#ifndef F_SEAL_EXEC
#error "fcntl.h:F_SEAL_EXEC macro is missing from libc-shim"
#endif

#ifndef F_SEAL_FUTURE_WRITE
#error "fcntl.h:F_SEAL_FUTURE_WRITE macro is missing from libc-shim"
#endif

#ifndef F_SEAL_GROW
#error "fcntl.h:F_SEAL_GROW macro is missing from libc-shim"
#endif

#ifndef F_SEAL_SEAL
#error "fcntl.h:F_SEAL_SEAL macro is missing from libc-shim"
#endif

#ifndef F_SEAL_SHRINK
#error "fcntl.h:F_SEAL_SHRINK macro is missing from libc-shim"
#endif

#ifndef F_SEAL_WRITE
#error "fcntl.h:F_SEAL_WRITE macro is missing from libc-shim"
#endif

#ifndef F_SETDELEG
#error "fcntl.h:F_SETDELEG macro is missing from libc-shim"
#endif

#ifndef F_SETFD
#error "fcntl.h:F_SETFD macro is missing from libc-shim"
#endif

#ifndef F_SETFL
#error "fcntl.h:F_SETFL macro is missing from libc-shim"
#endif

#ifndef F_SETLEASE
#error "fcntl.h:F_SETLEASE macro is missing from libc-shim"
#endif

#ifndef F_SETLK
#error "fcntl.h:F_SETLK macro is missing from libc-shim"
#endif

#ifndef F_SETLK64
#error "fcntl.h:F_SETLK64 macro is missing from libc-shim"
#endif

#ifndef F_SETLKW
#error "fcntl.h:F_SETLKW macro is missing from libc-shim"
#endif

#ifndef F_SETLKW64
#error "fcntl.h:F_SETLKW64 macro is missing from libc-shim"
#endif

#ifndef F_SETOWN
#error "fcntl.h:F_SETOWN macro is missing from libc-shim"
#endif

#ifndef F_SETOWN_EX
#error "fcntl.h:F_SETOWN_EX macro is missing from libc-shim"
#endif

#ifndef F_SETPIPE_SZ
#error "fcntl.h:F_SETPIPE_SZ macro is missing from libc-shim"
#endif

#ifndef F_SETSIG
#error "fcntl.h:F_SETSIG macro is missing from libc-shim"
#endif

#ifndef F_SET_FILE_RW_HINT
#error "fcntl.h:F_SET_FILE_RW_HINT macro is missing from libc-shim"
#endif

#ifndef F_SET_RW_HINT
#error "fcntl.h:F_SET_RW_HINT macro is missing from libc-shim"
#endif

#ifndef F_SHLCK
#error "fcntl.h:F_SHLCK macro is missing from libc-shim"
#endif

#ifndef F_TEST
#error "fcntl.h:F_TEST macro is missing from libc-shim"
#endif

#ifndef F_TLOCK
#error "fcntl.h:F_TLOCK macro is missing from libc-shim"
#endif

#ifndef F_ULOCK
#error "fcntl.h:F_ULOCK macro is missing from libc-shim"
#endif

#ifndef F_UNLCK
#error "fcntl.h:F_UNLCK macro is missing from libc-shim"
#endif

#ifndef F_WRLCK
#error "fcntl.h:F_WRLCK macro is missing from libc-shim"
#endif

#ifndef LOCK_EX
#error "fcntl.h:LOCK_EX macro is missing from libc-shim"
#endif

#ifndef LOCK_MAND
#error "fcntl.h:LOCK_MAND macro is missing from libc-shim"
#endif

#ifndef LOCK_NB
#error "fcntl.h:LOCK_NB macro is missing from libc-shim"
#endif

#ifndef LOCK_READ
#error "fcntl.h:LOCK_READ macro is missing from libc-shim"
#endif

#ifndef LOCK_RW
#error "fcntl.h:LOCK_RW macro is missing from libc-shim"
#endif

#ifndef LOCK_SH
#error "fcntl.h:LOCK_SH macro is missing from libc-shim"
#endif

#ifndef LOCK_UN
#error "fcntl.h:LOCK_UN macro is missing from libc-shim"
#endif

#ifndef LOCK_WRITE
#error "fcntl.h:LOCK_WRITE macro is missing from libc-shim"
#endif

#ifndef MAX_HANDLE_SZ
#error "fcntl.h:MAX_HANDLE_SZ macro is missing from libc-shim"
#endif

#ifndef O_ACCMODE
#error "fcntl.h:O_ACCMODE macro is missing from libc-shim"
#endif

#ifndef O_APPEND
#error "fcntl.h:O_APPEND macro is missing from libc-shim"
#endif

#ifndef O_ASYNC
#error "fcntl.h:O_ASYNC macro is missing from libc-shim"
#endif

#ifndef O_CLOEXEC
#error "fcntl.h:O_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef O_CREAT
#error "fcntl.h:O_CREAT macro is missing from libc-shim"
#endif

#ifndef O_DIRECT
#error "fcntl.h:O_DIRECT macro is missing from libc-shim"
#endif

#ifndef O_DIRECTORY
#error "fcntl.h:O_DIRECTORY macro is missing from libc-shim"
#endif

#ifndef O_DSYNC
#error "fcntl.h:O_DSYNC macro is missing from libc-shim"
#endif

#ifndef O_EXCL
#error "fcntl.h:O_EXCL macro is missing from libc-shim"
#endif

#ifndef O_FSYNC
#error "fcntl.h:O_FSYNC macro is missing from libc-shim"
#endif

#ifndef O_LARGEFILE
#error "fcntl.h:O_LARGEFILE macro is missing from libc-shim"
#endif

#ifndef O_NDELAY
#error "fcntl.h:O_NDELAY macro is missing from libc-shim"
#endif

#ifndef O_NOATIME
#error "fcntl.h:O_NOATIME macro is missing from libc-shim"
#endif

#ifndef O_NOCTTY
#error "fcntl.h:O_NOCTTY macro is missing from libc-shim"
#endif

#ifndef O_NOFOLLOW
#error "fcntl.h:O_NOFOLLOW macro is missing from libc-shim"
#endif

#ifndef O_NONBLOCK
#error "fcntl.h:O_NONBLOCK macro is missing from libc-shim"
#endif

#ifndef O_PATH
#error "fcntl.h:O_PATH macro is missing from libc-shim"
#endif

#ifndef O_RDONLY
#error "fcntl.h:O_RDONLY macro is missing from libc-shim"
#endif

#ifndef O_RDWR
#error "fcntl.h:O_RDWR macro is missing from libc-shim"
#endif

#ifndef O_RSYNC
#error "fcntl.h:O_RSYNC macro is missing from libc-shim"
#endif

#ifndef O_SYNC
#error "fcntl.h:O_SYNC macro is missing from libc-shim"
#endif

#ifndef O_TMPFILE
#error "fcntl.h:O_TMPFILE macro is missing from libc-shim"
#endif

#ifndef O_TRUNC
#error "fcntl.h:O_TRUNC macro is missing from libc-shim"
#endif

#ifndef O_WRONLY
#error "fcntl.h:O_WRONLY macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_DONTNEED
#error "fcntl.h:POSIX_FADV_DONTNEED macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_NOREUSE
#error "fcntl.h:POSIX_FADV_NOREUSE macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_NORMAL
#error "fcntl.h:POSIX_FADV_NORMAL macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_RANDOM
#error "fcntl.h:POSIX_FADV_RANDOM macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_SEQUENTIAL
#error "fcntl.h:POSIX_FADV_SEQUENTIAL macro is missing from libc-shim"
#endif

#ifndef POSIX_FADV_WILLNEED
#error "fcntl.h:POSIX_FADV_WILLNEED macro is missing from libc-shim"
#endif

#ifndef RESOLVE_BENEATH
#error "fcntl.h:RESOLVE_BENEATH macro is missing from libc-shim"
#endif

#ifndef RESOLVE_CACHED
#error "fcntl.h:RESOLVE_CACHED macro is missing from libc-shim"
#endif

#ifndef RESOLVE_IN_ROOT
#error "fcntl.h:RESOLVE_IN_ROOT macro is missing from libc-shim"
#endif

#ifndef RESOLVE_NO_MAGICLINKS
#error "fcntl.h:RESOLVE_NO_MAGICLINKS macro is missing from libc-shim"
#endif

#ifndef RESOLVE_NO_SYMLINKS
#error "fcntl.h:RESOLVE_NO_SYMLINKS macro is missing from libc-shim"
#endif

#ifndef RESOLVE_NO_XDEV
#error "fcntl.h:RESOLVE_NO_XDEV macro is missing from libc-shim"
#endif

#ifndef RWF_WRITE_LIFE_NOT_SET
#error "fcntl.h:RWF_WRITE_LIFE_NOT_SET macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_EXTREME
#error "fcntl.h:RWH_WRITE_LIFE_EXTREME macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_LONG
#error "fcntl.h:RWH_WRITE_LIFE_LONG macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_MEDIUM
#error "fcntl.h:RWH_WRITE_LIFE_MEDIUM macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_NONE
#error "fcntl.h:RWH_WRITE_LIFE_NONE macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_NOT_SET
#error "fcntl.h:RWH_WRITE_LIFE_NOT_SET macro is missing from libc-shim"
#endif

#ifndef RWH_WRITE_LIFE_SHORT
#error "fcntl.h:RWH_WRITE_LIFE_SHORT macro is missing from libc-shim"
#endif

#ifndef R_OK
#error "fcntl.h:R_OK macro is missing from libc-shim"
#endif

#ifndef SEEK_CUR
#error "fcntl.h:SEEK_CUR macro is missing from libc-shim"
#endif

#ifndef SEEK_END
#error "fcntl.h:SEEK_END macro is missing from libc-shim"
#endif

#ifndef SEEK_SET
#error "fcntl.h:SEEK_SET macro is missing from libc-shim"
#endif

#ifndef SPLICE_F_GIFT
#error "fcntl.h:SPLICE_F_GIFT macro is missing from libc-shim"
#endif

#ifndef SPLICE_F_MORE
#error "fcntl.h:SPLICE_F_MORE macro is missing from libc-shim"
#endif

#ifndef SPLICE_F_MOVE
#error "fcntl.h:SPLICE_F_MOVE macro is missing from libc-shim"
#endif

#ifndef SPLICE_F_NONBLOCK
#error "fcntl.h:SPLICE_F_NONBLOCK macro is missing from libc-shim"
#endif

#ifndef SYNC_FILE_RANGE_WAIT_AFTER
#error "fcntl.h:SYNC_FILE_RANGE_WAIT_AFTER macro is missing from libc-shim"
#endif

#ifndef SYNC_FILE_RANGE_WAIT_BEFORE
#error "fcntl.h:SYNC_FILE_RANGE_WAIT_BEFORE macro is missing from libc-shim"
#endif

#ifndef SYNC_FILE_RANGE_WRITE
#error "fcntl.h:SYNC_FILE_RANGE_WRITE macro is missing from libc-shim"
#endif

#ifndef SYNC_FILE_RANGE_WRITE_AND_WAIT
#error "fcntl.h:SYNC_FILE_RANGE_WRITE_AND_WAIT macro is missing from libc-shim"
#endif

#ifndef S_IFBLK
#error "fcntl.h:S_IFBLK macro is missing from libc-shim"
#endif

#ifndef S_IFCHR
#error "fcntl.h:S_IFCHR macro is missing from libc-shim"
#endif

#ifndef S_IFDIR
#error "fcntl.h:S_IFDIR macro is missing from libc-shim"
#endif

#ifndef S_IFIFO
#error "fcntl.h:S_IFIFO macro is missing from libc-shim"
#endif

#ifndef S_IFLNK
#error "fcntl.h:S_IFLNK macro is missing from libc-shim"
#endif

#ifndef S_IFMT
#error "fcntl.h:S_IFMT macro is missing from libc-shim"
#endif

#ifndef S_IFREG
#error "fcntl.h:S_IFREG macro is missing from libc-shim"
#endif

#ifndef S_IFSOCK
#error "fcntl.h:S_IFSOCK macro is missing from libc-shim"
#endif

#ifndef S_IRGRP
#error "fcntl.h:S_IRGRP macro is missing from libc-shim"
#endif

#ifndef S_IROTH
#error "fcntl.h:S_IROTH macro is missing from libc-shim"
#endif

#ifndef S_IRUSR
#error "fcntl.h:S_IRUSR macro is missing from libc-shim"
#endif

#ifndef S_IRWXG
#error "fcntl.h:S_IRWXG macro is missing from libc-shim"
#endif

#ifndef S_IRWXO
#error "fcntl.h:S_IRWXO macro is missing from libc-shim"
#endif

#ifndef S_IRWXU
#error "fcntl.h:S_IRWXU macro is missing from libc-shim"
#endif

#ifndef S_ISGID
#error "fcntl.h:S_ISGID macro is missing from libc-shim"
#endif

#ifndef S_ISUID
#error "fcntl.h:S_ISUID macro is missing from libc-shim"
#endif

#ifndef S_ISVTX
#error "fcntl.h:S_ISVTX macro is missing from libc-shim"
#endif

#ifndef S_IWGRP
#error "fcntl.h:S_IWGRP macro is missing from libc-shim"
#endif

#ifndef S_IWOTH
#error "fcntl.h:S_IWOTH macro is missing from libc-shim"
#endif

#ifndef S_IWUSR
#error "fcntl.h:S_IWUSR macro is missing from libc-shim"
#endif

#ifndef S_IXGRP
#error "fcntl.h:S_IXGRP macro is missing from libc-shim"
#endif

#ifndef S_IXOTH
#error "fcntl.h:S_IXOTH macro is missing from libc-shim"
#endif

#ifndef S_IXUSR
#error "fcntl.h:S_IXUSR macro is missing from libc-shim"
#endif

#ifndef UTIME_NOW
#error "fcntl.h:UTIME_NOW macro is missing from libc-shim"
#endif

#ifndef UTIME_OMIT
#error "fcntl.h:UTIME_OMIT macro is missing from libc-shim"
#endif

#ifndef W_OK
#error "fcntl.h:W_OK macro is missing from libc-shim"
#endif

#ifndef X_OK
#error "fcntl.h:X_OK macro is missing from libc-shim"
#endif

#ifndef _ASM_GENERIC_INT_LL64_H
#error "fcntl.h:_ASM_GENERIC_INT_LL64_H macro is missing from libc-shim"
#endif

#ifndef _ASM_GENERIC_TYPES_H
#error "fcntl.h:_ASM_GENERIC_TYPES_H macro is missing from libc-shim"
#endif

#ifndef _ASM_X86_POSIX_TYPES_64_H
#error "fcntl.h:_ASM_X86_POSIX_TYPES_64_H macro is missing from libc-shim"
#endif

#ifndef _ATFILE_SOURCE
#error "fcntl.h:_ATFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _BITS_ENDIANNESS_H
#error "fcntl.h:_BITS_ENDIANNESS_H macro is missing from libc-shim"
#endif

#ifndef _BITS_ENDIAN_H
#error "fcntl.h:_BITS_ENDIAN_H macro is missing from libc-shim"
#endif

#ifndef _BITS_STAT_H
#error "fcntl.h:_BITS_STAT_H macro is missing from libc-shim"
#endif

#ifndef _BITS_STRUCT_STAT_H
#error "fcntl.h:_BITS_STRUCT_STAT_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TIME64_H
#error "fcntl.h:_BITS_TIME64_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TYPESIZES_H
#error "fcntl.h:_BITS_TYPESIZES_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TYPES_H
#error "fcntl.h:_BITS_TYPES_H macro is missing from libc-shim"
#endif

#ifndef _DEFAULT_SOURCE
#error "fcntl.h:_DEFAULT_SOURCE macro is missing from libc-shim"
#endif

#ifndef _DYNAMIC_STACK_SIZE_SOURCE
#error "fcntl.h:_DYNAMIC_STACK_SIZE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _FALLOC_H_
#error "fcntl.h:_FALLOC_H_ macro is missing from libc-shim"
#endif

#ifndef _FCNTL_H
#error "fcntl.h:_FCNTL_H macro is missing from libc-shim"
#endif

#ifndef _FEATURES_H
#error "fcntl.h:_FEATURES_H macro is missing from libc-shim"
#endif

#ifndef _ISOC11_SOURCE
#error "fcntl.h:_ISOC11_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC23_SOURCE
#error "fcntl.h:_ISOC23_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC2Y_SOURCE
#error "fcntl.h:_ISOC2Y_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC95_SOURCE
#error "fcntl.h:_ISOC95_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC99_SOURCE
#error "fcntl.h:_ISOC99_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE64_SOURCE
#error "fcntl.h:_LARGEFILE64_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE_SOURCE
#error "fcntl.h:_LARGEFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LINUX_OPENAT2_H
#error "fcntl.h:_LINUX_OPENAT2_H macro is missing from libc-shim"
#endif

#ifndef _LINUX_POSIX_TYPES_H
#error "fcntl.h:_LINUX_POSIX_TYPES_H macro is missing from libc-shim"
#endif

#ifndef _LINUX_STDDEF_H
#error "fcntl.h:_LINUX_STDDEF_H macro is missing from libc-shim"
#endif

#ifndef _LINUX_TYPES_H
#error "fcntl.h:_LINUX_TYPES_H macro is missing from libc-shim"
#endif

#ifndef _POSIX_C_SOURCE
#error "fcntl.h:_POSIX_C_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_SOURCE
#error "fcntl.h:_POSIX_SOURCE macro is missing from libc-shim"
#endif

#ifndef _SIZE_T
#error "fcntl.h:_SIZE_T macro is missing from libc-shim"
#endif

#ifndef _STATBUF_ST_BLKSIZE
#error "fcntl.h:_STATBUF_ST_BLKSIZE macro is missing from libc-shim"
#endif

#ifndef _STATBUF_ST_NSEC
#error "fcntl.h:_STATBUF_ST_NSEC macro is missing from libc-shim"
#endif

#ifndef _STATBUF_ST_RDEV
#error "fcntl.h:_STATBUF_ST_RDEV macro is missing from libc-shim"
#endif

#ifndef _STDC_PREDEF_H
#error "fcntl.h:_STDC_PREDEF_H macro is missing from libc-shim"
#endif

#ifndef _STRUCT_TIMESPEC
#error "fcntl.h:_STRUCT_TIMESPEC macro is missing from libc-shim"
#endif

#ifndef _SYS_CDEFS_H
#error "fcntl.h:_SYS_CDEFS_H macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE
#error "fcntl.h:_XOPEN_SOURCE macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE_EXTENDED
#error "fcntl.h:_XOPEN_SOURCE_EXTENDED macro is missing from libc-shim"
#endif

#ifndef st_atime
#error "fcntl.h:st_atime macro is missing from libc-shim"
#endif

#ifndef st_ctime
#error "fcntl.h:st_ctime macro is missing from libc-shim"
#endif

#ifndef st_mtime
#error "fcntl.h:st_mtime macro is missing from libc-shim"
#endif

int main(void) { return 0; }
