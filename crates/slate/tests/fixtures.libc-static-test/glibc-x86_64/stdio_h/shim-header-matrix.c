#include <stdio.h>

extern int slate_oracle_fscanf(struct _IO_FILE *restrict, const char *restrict, ...);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fscanf), __typeof__(fscanf)),
    "stdio.h:fscanf declaration differs from oracle");

static __typeof__(fscanf) *const slate_reference_fscanf = &fscanf;

#ifndef AT_RENAME_EXCHANGE
#error "stdio.h:AT_RENAME_EXCHANGE macro is missing from libc-shim"
#endif

#ifndef AT_RENAME_NOREPLACE
#error "stdio.h:AT_RENAME_NOREPLACE macro is missing from libc-shim"
#endif

#ifndef AT_RENAME_WHITEOUT
#error "stdio.h:AT_RENAME_WHITEOUT macro is missing from libc-shim"
#endif

#ifndef BUFSIZ
#error "stdio.h:BUFSIZ macro is missing from libc-shim"
#endif

#ifndef EOF
#error "stdio.h:EOF macro is missing from libc-shim"
#endif

#ifndef FILENAME_MAX
#error "stdio.h:FILENAME_MAX macro is missing from libc-shim"
#endif

#ifndef FOPEN_MAX
#error "stdio.h:FOPEN_MAX macro is missing from libc-shim"
#endif

#ifndef L_ctermid
#error "stdio.h:L_ctermid macro is missing from libc-shim"
#endif

#ifndef L_cuserid
#error "stdio.h:L_cuserid macro is missing from libc-shim"
#endif

#ifndef L_tmpnam
#error "stdio.h:L_tmpnam macro is missing from libc-shim"
#endif

#ifndef P_tmpdir
#error "stdio.h:P_tmpdir macro is missing from libc-shim"
#endif

#ifndef RENAME_EXCHANGE
#error "stdio.h:RENAME_EXCHANGE macro is missing from libc-shim"
#endif

#ifndef RENAME_NOREPLACE
#error "stdio.h:RENAME_NOREPLACE macro is missing from libc-shim"
#endif

#ifndef RENAME_WHITEOUT
#error "stdio.h:RENAME_WHITEOUT macro is missing from libc-shim"
#endif

#ifndef SEEK_CUR
#error "stdio.h:SEEK_CUR macro is missing from libc-shim"
#endif

#ifndef SEEK_DATA
#error "stdio.h:SEEK_DATA macro is missing from libc-shim"
#endif

#ifndef SEEK_END
#error "stdio.h:SEEK_END macro is missing from libc-shim"
#endif

#ifndef SEEK_HOLE
#error "stdio.h:SEEK_HOLE macro is missing from libc-shim"
#endif

#ifndef SEEK_SET
#error "stdio.h:SEEK_SET macro is missing from libc-shim"
#endif

#ifndef TMP_MAX
#error "stdio.h:TMP_MAX macro is missing from libc-shim"
#endif

#ifndef stderr
#error "stdio.h:stderr macro is missing from libc-shim"
#endif

#ifndef stdin
#error "stdio.h:stdin macro is missing from libc-shim"
#endif

#ifndef stdout
#error "stdio.h:stdout macro is missing from libc-shim"
#endif

int main(void) { return 0; }
