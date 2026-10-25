// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir /X
#include <stdio.h>

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × header not found in search path: <stdio.h>
// DEFAULT:   ╰─▶ header not found in search path: <stdio.h>
// DEFAULT:    ╭─[tests/fixtures/error/msvc/windows/x86_64/no_standard_includes.c:1:10]
// DEFAULT:  1 │ #include <stdio.h>
// DEFAULT:    ·          ─────────
// DEFAULT:    ╰────
// SLATE-FILECHECK-END DEFAULT
