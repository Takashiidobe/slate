long long c99_compat = 2147483648;
long long long_long_extension = 4294967296;
long long implicitly_unsigned = 18446744073709551615;

// SLATE-FILECHECK-ARGS -pedantic
// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-WARNING C89

// SLATE-FILECHECK-BEGIN C89
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/sema/i686-unknown-linux-gnu/integer_literal_extension_warnings.c:1:1]
// C89: 1 │ long long c99_compat = 2147483648;
// C89: · ──────────────────────────────────
// C89: 2 │ long long long_long_extension = 4294967296;
// C89: ╰────
// C89: -Wc99-compat
// C89: ⚠ integer literal is too large to be represented in type 'long',
// C89: │ interpreting as 'unsigned long' per C89; this literal will have type 'long
// C89: │ long' in C99 onwards
// C89: ╭─[tests/fixtures/sema/i686-unknown-linux-gnu/integer_literal_extension_warnings.c:1:24]
// C89: 1 │ long long c99_compat = 2147483648;
// C89: ·                        ──────────
// C89: 2 │ long long long_long_extension = 4294967296;
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/sema/i686-unknown-linux-gnu/integer_literal_extension_warnings.c:2:1]
// C89: 1 │ long long c99_compat = 2147483648;
// C89: 2 │ long long long_long_extension = 4294967296;
// C89: · ───────────────────────────────────────────
// C89: 3 │ long long implicitly_unsigned = 18446744073709551615;
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/sema/i686-unknown-linux-gnu/integer_literal_extension_warnings.c:2:33]
// C89: 1 │ long long c99_compat = 2147483648;
// C89: 2 │ long long long_long_extension = 4294967296;
// C89: ·                                 ──────────
// C89: 3 │ long long implicitly_unsigned = 18446744073709551615;
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/sema/i686-unknown-linux-gnu/integer_literal_extension_warnings.c:3:1]
// C89: 2 │ long long long_long_extension = 4294967296;
// C89: 3 │ long long implicitly_unsigned = 18446744073709551615;
// C89: · ─────────────────────────────────────────────────────
// C89: 4 │
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/sema/i686-unknown-linux-gnu/integer_literal_extension_warnings.c:3:33]
// C89: 2 │ long long long_long_extension = 4294967296;
// C89: 3 │ long long implicitly_unsigned = 18446744073709551615;
// C89: ·                                 ────────────────────
// C89: 4 │
// C89: ╰────
// C89: -Wimplicitly-unsigned-literal
// C89: ⚠ integer literal is too large to be represented in a signed integer type,
// C89: │ interpreting as unsigned
// C89: ╭─[tests/fixtures/sema/i686-unknown-linux-gnu/integer_literal_extension_warnings.c:3:33]
// C89: 2 │ long long long_long_extension = 4294967296;
// C89: 3 │ long long implicitly_unsigned = 18446744073709551615;
// C89: ·                                 ────────────────────
// C89: 4 │
// C89: ╰────
// SLATE-FILECHECK-END C89
