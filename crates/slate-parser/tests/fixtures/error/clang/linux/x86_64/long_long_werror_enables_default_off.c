long long x;

// SLATE-FILECHECK-ARGS -Werror=long-long
// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-ERROR C89

// SLATE-FILECHECK-BEGIN C89
// C89: Error:   × semantic analysis failed
// C89: Error: -Wlong-long
// C89: × 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/error/clang/linux/x86_64/long_long_werror_enables_default_off.c:1:1]
// C89: 1 │ long long x;
// C89: · ────────────
// C89: 2 │
// C89: ╰────
// SLATE-FILECHECK-END C89
