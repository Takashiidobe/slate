#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

extern int _ctime64_s;
extern int _dupenv_s;
extern int _get_errno;
extern int _itoa_s;
extern int _set_invalid_parameter_handler;
extern int fopen_s;
extern int strcpy_s;
extern int wcscpy_s;

int main(void) { return 0; }



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
