// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

extern int target;
static int __attribute__((weakref("target"))) reference = 0;

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR: × weakref declaration cannot be a definition
// IR: ╭─[tests/fixtures/error/gcc/linux/x86_64/weakref_initialized_target.c:3:47]
// IR: 2 │ extern int target;
// IR: 3 │ static int __attribute__((weakref("target"))) reference = 0;
// IR: ·                                               ─────────────
// IR: ╰────
// SLATE-FILECHECK-END IR
