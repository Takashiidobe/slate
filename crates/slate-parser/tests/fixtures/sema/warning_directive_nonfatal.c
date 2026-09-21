// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu99

#warning directives stay non-fatal
int after_warning = 7;

// SLATE-FILECHECK-BEGIN WARN
// WARN: ⚠ #warning directives stay non-fatal
// WARN: ╭─[tests/fixtures/sema/warning_directive_nonfatal.c:2:1]
// WARN: 1 │
// WARN: 2 │ #warning directives stay non-fatal
// WARN: · ──────────────────────────────────
// WARN: 3 │ int after_warning = 7;
// WARN: ╰────
// SLATE-FILECHECK-END WARN
