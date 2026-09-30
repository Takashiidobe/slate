#include <sys/stat.h>

extern int slate_oracle_stat(const char *restrict, struct stat *restrict);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_stat), __typeof__(stat)),
    "sys/stat.h:stat declaration differs from oracle");

static __typeof__(stat) *const slate_reference_stat = &stat;

typedef unsigned long long slate_oracle_typedef_dev_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_dev_t, dev_t), "typedef dev_t differs from oracle");

#ifndef ACCESSPERMS
#error "sys/stat.h:ACCESSPERMS macro is missing from libc-shim"
#endif

#ifndef ALLPERMS
#error "sys/stat.h:ALLPERMS macro is missing from libc-shim"
#endif

#ifndef DEFFILEMODE
#error "sys/stat.h:DEFFILEMODE macro is missing from libc-shim"
#endif

#ifndef S_BLKSIZE
#error "sys/stat.h:S_BLKSIZE macro is missing from libc-shim"
#endif

#ifndef S_IEXEC
#error "sys/stat.h:S_IEXEC macro is missing from libc-shim"
#endif

#ifndef S_IFBLK
#error "sys/stat.h:S_IFBLK macro is missing from libc-shim"
#endif

#ifndef S_IFCHR
#error "sys/stat.h:S_IFCHR macro is missing from libc-shim"
#endif

#ifndef S_IFDIR
#error "sys/stat.h:S_IFDIR macro is missing from libc-shim"
#endif

#ifndef S_IFIFO
#error "sys/stat.h:S_IFIFO macro is missing from libc-shim"
#endif

#ifndef S_IFLNK
#error "sys/stat.h:S_IFLNK macro is missing from libc-shim"
#endif

#ifndef S_IFMT
#error "sys/stat.h:S_IFMT macro is missing from libc-shim"
#endif

#ifndef S_IFREG
#error "sys/stat.h:S_IFREG macro is missing from libc-shim"
#endif

#ifndef S_IFSOCK
#error "sys/stat.h:S_IFSOCK macro is missing from libc-shim"
#endif

#ifndef S_IREAD
#error "sys/stat.h:S_IREAD macro is missing from libc-shim"
#endif

#ifndef S_IRGRP
#error "sys/stat.h:S_IRGRP macro is missing from libc-shim"
#endif

#ifndef S_IROTH
#error "sys/stat.h:S_IROTH macro is missing from libc-shim"
#endif

#ifndef S_IRUSR
#error "sys/stat.h:S_IRUSR macro is missing from libc-shim"
#endif

#ifndef S_IRWXG
#error "sys/stat.h:S_IRWXG macro is missing from libc-shim"
#endif

#ifndef S_IRWXO
#error "sys/stat.h:S_IRWXO macro is missing from libc-shim"
#endif

#ifndef S_IRWXU
#error "sys/stat.h:S_IRWXU macro is missing from libc-shim"
#endif

#ifndef S_ISBLK
#error "sys/stat.h:S_ISBLK macro is missing from libc-shim"
#endif

#ifndef S_ISCHR
#error "sys/stat.h:S_ISCHR macro is missing from libc-shim"
#endif

#ifndef S_ISDIR
#error "sys/stat.h:S_ISDIR macro is missing from libc-shim"
#endif

#ifndef S_ISFIFO
#error "sys/stat.h:S_ISFIFO macro is missing from libc-shim"
#endif

#ifndef S_ISGID
#error "sys/stat.h:S_ISGID macro is missing from libc-shim"
#endif

#ifndef S_ISLNK
#error "sys/stat.h:S_ISLNK macro is missing from libc-shim"
#endif

#ifndef S_ISREG
#error "sys/stat.h:S_ISREG macro is missing from libc-shim"
#endif

#ifndef S_ISSOCK
#error "sys/stat.h:S_ISSOCK macro is missing from libc-shim"
#endif

#ifndef S_ISUID
#error "sys/stat.h:S_ISUID macro is missing from libc-shim"
#endif

#ifndef S_ISVTX
#error "sys/stat.h:S_ISVTX macro is missing from libc-shim"
#endif

#ifndef S_IWGRP
#error "sys/stat.h:S_IWGRP macro is missing from libc-shim"
#endif

#ifndef S_IWOTH
#error "sys/stat.h:S_IWOTH macro is missing from libc-shim"
#endif

#ifndef S_IWRITE
#error "sys/stat.h:S_IWRITE macro is missing from libc-shim"
#endif

#ifndef S_IWUSR
#error "sys/stat.h:S_IWUSR macro is missing from libc-shim"
#endif

#ifndef S_IXGRP
#error "sys/stat.h:S_IXGRP macro is missing from libc-shim"
#endif

#ifndef S_IXOTH
#error "sys/stat.h:S_IXOTH macro is missing from libc-shim"
#endif

#ifndef S_IXUSR
#error "sys/stat.h:S_IXUSR macro is missing from libc-shim"
#endif

#ifndef S_TYPEISMQ
#error "sys/stat.h:S_TYPEISMQ macro is missing from libc-shim"
#endif

#ifndef S_TYPEISSEM
#error "sys/stat.h:S_TYPEISSEM macro is missing from libc-shim"
#endif

#ifndef S_TYPEISSHM
#error "sys/stat.h:S_TYPEISSHM macro is missing from libc-shim"
#endif

#ifndef UTIME_NOW
#error "sys/stat.h:UTIME_NOW macro is missing from libc-shim"
#endif

#ifndef UTIME_OMIT
#error "sys/stat.h:UTIME_OMIT macro is missing from libc-shim"
#endif

int main(void) { return 0; }
