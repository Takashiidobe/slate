#include <sysroot_only.h>

// SLATE-FILECHECK-ARGS -nostdlibinc --sysroot=tests/fixtures/inputs/include_search/sysroot
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × header not found in search path: <sysroot_only.h>
// PARSE: ╰─▶ header not found in search path: <sysroot_only.h>
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/nostdlibinc_sysroot.c:1:10]
// PARSE: 1 │ #include <sysroot_only.h>
// PARSE: ·          ────────────────
// PARSE: 2 │
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
