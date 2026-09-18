long long promoted = 2147483648;

// SLATE-FILECHECK-ARGS -Werror=c99-compat --target=i686-unknown-linux-gnu
// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-ERROR C89

// SLATE-FILECHECK-BEGIN C89
// C89: Error:   × semantic analysis failed
// C89: Error: -Wc99-compat
// C89: × integer literal is too large to be represented in type 'long',
// C89: ╭─[tests/fixtures/error/integer_literal_werror_c99_compat.c:1:22]
// C89: 1 │ long long promoted = 2147483648;
// C89: ·                      ──────────
// C89: 2 │
// C89: ╰────
// SLATE-FILECHECK-END C89
