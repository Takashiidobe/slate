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
// C89: ╭─[tests/fixtures/clang/linux/i686/integer_literal_extension_warnings.c:1:1]
// C89: 1 │ long long c99_compat = 2147483648;
// C89: · ──────────────────────────────────
// C89: 2 │ long long long_long_extension = 4294967296;
// C89: ╰────
// C89: -Wc99-compat
// C89: ⚠ integer literal is too large to be represented in type 'long',
// C89: │ interpreting as 'unsigned long' per C89; this literal will have type 'long
// C89: │ long' in C99 onwards
// C89: ╭─[tests/fixtures/clang/linux/i686/integer_literal_extension_warnings.c:1:24]
// C89: 1 │ long long c99_compat = 2147483648;
// C89: ·                        ──────────
// C89: 2 │ long long long_long_extension = 4294967296;
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/clang/linux/i686/integer_literal_extension_warnings.c:2:1]
// C89: 1 │ long long c99_compat = 2147483648;
// C89: 2 │ long long long_long_extension = 4294967296;
// C89: · ───────────────────────────────────────────
// C89: 3 │ long long implicitly_unsigned = 18446744073709551615;
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/clang/linux/i686/integer_literal_extension_warnings.c:2:33]
// C89: 1 │ long long c99_compat = 2147483648;
// C89: 2 │ long long long_long_extension = 4294967296;
// C89: ·                                 ──────────
// C89: 3 │ long long implicitly_unsigned = 18446744073709551615;
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/clang/linux/i686/integer_literal_extension_warnings.c:3:1]
// C89: 2 │ long long long_long_extension = 4294967296;
// C89: 3 │ long long implicitly_unsigned = 18446744073709551615;
// C89: · ─────────────────────────────────────────────────────
// C89: 4 │
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/clang/linux/i686/integer_literal_extension_warnings.c:3:33]
// C89: 2 │ long long long_long_extension = 4294967296;
// C89: 3 │ long long implicitly_unsigned = 18446744073709551615;
// C89: ·                                 ────────────────────
// C89: 4 │
// C89: ╰────
// C89: -Wimplicitly-unsigned-literal
// C89: ⚠ integer literal is too large to be represented in a signed integer type,
// C89: │ interpreting as unsigned
// C89: ╭─[tests/fixtures/clang/linux/i686/integer_literal_extension_warnings.c:3:33]
// C89: 2 │ long long long_long_extension = 4294967296;
// C89: 3 │ long long implicitly_unsigned = 18446744073709551615;
// C89: ·                                 ────────────────────
// C89: 4 │
// C89: ╰────
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN IR-C89
// IR-C89: module {
// IR-C89-NEXT:     target "i686-unknown-linux-gnu" {
// IR-C89-NEXT:         endian = little;
// IR-C89-NEXT:         pointer [size=4, align=4];
// IR-C89-NEXT:         stack_alignment = 16;
// IR-C89-NEXT:         long_double = f80;
// IR-C89-NEXT:         storage bool [size=1, align=1];
// IR-C89-NEXT:         storage i8, u8 [size=1, align=1];
// IR-C89-NEXT:         storage i16, u16 [size=2, align=2];
// IR-C89-NEXT:         storage i32, u32 [size=4, align=4];
// IR-C89-NEXT:         storage i64, u64 [size=8, align=4];
// IR-C89-NEXT:         storage i128, u128 [size=16, align=16];
// IR-C89-NEXT:         storage bf16 [size=2, align=2];
// IR-C89-NEXT:         storage f16 [size=2, align=2];
// IR-C89-NEXT:         storage f32 [size=4, align=4];
// IR-C89-NEXT:         storage f64 [size=8, align=4];
// IR-C89-NEXT:         storage f80 [size=12, align=4];
// IR-C89-NEXT:         storage f128 [size=16, align=16];
// IR-C89-NEXT:         storage d32 [size=4, align=4];
// IR-C89-NEXT:         storage d64 [size=8, align=8];
// IR-C89-NEXT:         storage d128 [size=16, align=16];
// IR-C89-NEXT:     }
// IR-C89-NEXT:     global %0 c99_compat: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(const<u32>(2147483648))) [linkage=external];
// IR-C89-NEXT:     global %1 long_long_extension: i64 [storage=static] = const<i64>(4294967296) [linkage=external];
// IR-C89-NEXT:     global %2 implicitly_unsigned: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709551615)) [linkage=external];
// IR-C89-NEXT: }
// SLATE-FILECHECK-END IR-C89
