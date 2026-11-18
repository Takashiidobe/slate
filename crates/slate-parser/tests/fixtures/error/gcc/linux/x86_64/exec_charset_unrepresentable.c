// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir -fexec-charset=IBM1047

const char text[] = "€";

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT:   × converting to execution character set: Invalid or incomplete multibyte or
// DEFAULT:    ╭─[tests/fixtures/error/gcc/linux/x86_64/exec_charset_unrepresentable.c:2:21]
// DEFAULT:  1 │
// DEFAULT:  2 │ const char text[] = "€";
// DEFAULT:    ·                     ───
// DEFAULT:  3 │
// DEFAULT:    ╰────
// SLATE-FILECHECK-END DEFAULT
