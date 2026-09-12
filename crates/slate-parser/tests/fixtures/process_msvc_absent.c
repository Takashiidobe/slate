#include <stdlib.h>
#include <wchar.h>

extern int _beginthread;
extern int _cwait;
extern int _execv;
extern int _get_initial_narrow_environment;
extern int _getpid;
extern int _spawnv;
extern int _wexecv;
extern int _wspawnv;

int main(void) { return 0; }



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
