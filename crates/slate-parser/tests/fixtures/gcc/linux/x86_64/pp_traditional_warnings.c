// SLATE-FILECHECK-ARGS -std=gnu99 -Wtraditional -Wno-deprecated -Wno-long-long
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-WARNING DEFAULT

#assert x(x)

 #define X X

#if 0
 #define SKIPPED 1
#elif 1
#endif

#define f(X) X
int f;

#if 0U
#endif

#if +1
#endif

#if 0 && +1
#endif

#define ALERT "\a"
char *x = "\x0";
char *y = ALERT;
char *z = "\u0F43";
int ok = 1;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: -Wtraditional
// DEFAULT: ⚠ suggest hiding '#assert' from traditional C with an indented '#'
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_traditional_warnings.c:2:2]
// DEFAULT: 1 │
// DEFAULT: 2 │ #assert x(x)
// DEFAULT: ·  ──────
// DEFAULT: 3 │
// DEFAULT: ╰────
// DEFAULT: -Wtraditional
// DEFAULT: ⚠ traditional C ignores '#define' with the '#' indented
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_traditional_warnings.c:4:3]
// DEFAULT: 3 │
// DEFAULT: 4 │  #define X X
// DEFAULT: ·   ──────
// DEFAULT: 5 │
// DEFAULT: ╰────
// DEFAULT: -Wtraditional
// DEFAULT: ⚠ traditional C ignores '#define' with the '#' indented
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_traditional_warnings.c:7:3]
// DEFAULT: 6 │ #if 0
// DEFAULT: 7 │  #define SKIPPED 1
// DEFAULT: ·   ──────
// DEFAULT: 8 │ #elif 1
// DEFAULT: ╰────
// DEFAULT: -Wtraditional
// DEFAULT: ⚠ suggest not using '#elif' in traditional C
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_traditional_warnings.c:8:2]
// DEFAULT: 7 │  #define SKIPPED 1
// DEFAULT: 8 │ #elif 1
// DEFAULT: ·  ────
// DEFAULT: 9 │ #endif
// DEFAULT: ╰────
// DEFAULT: -Wtraditional
// DEFAULT: ⚠ function-like macro 'f' must be used with arguments in traditional C
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_traditional_warnings.c:12:5]
// DEFAULT: 11 │ #define f(X) X
// DEFAULT: 12 │ int f;
// DEFAULT: ·     ─
// DEFAULT: 13 │
// DEFAULT: ╰────
// DEFAULT: -Wtraditional
// DEFAULT: ⚠ traditional C rejects the 'U' suffix
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_traditional_warnings.c:14:5]
// DEFAULT: 13 │
// DEFAULT: 14 │ #if 0U
// DEFAULT: ·     ──
// DEFAULT: 15 │ #endif
// DEFAULT: ╰────
// DEFAULT: -Wtraditional
// DEFAULT: ⚠ traditional C rejects the unary plus operator
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_traditional_warnings.c:17:5]
// DEFAULT: 16 │
// DEFAULT: 17 │ #if +1
// DEFAULT: ·     ──
// DEFAULT: 18 │ #endif
// DEFAULT: ╰────
// DEFAULT: -Wtraditional
// DEFAULT: ⚠ the meaning of '\x' is different in traditional C
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_traditional_warnings.c:24:12]
// DEFAULT: 23 │ #define ALERT "\a"
// DEFAULT: 24 │ char *x = "\x0";
// DEFAULT: ·            ──
// DEFAULT: 25 │ char *y = ALERT;
// DEFAULT: ╰────
// DEFAULT: -Wtraditional
// DEFAULT: ⚠ the meaning of '\a' is different in traditional C
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_traditional_warnings.c:25:11]
// DEFAULT: 24 │ char *x = "\x0";
// DEFAULT: 25 │ char *y = ALERT;
// DEFAULT: ·           ─────
// DEFAULT: 26 │ char *z = "\u0F43";
// DEFAULT: ╰────
// DEFAULT: -Wtraditional
// DEFAULT: ⚠ the meaning of '\u' is different in traditional C
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_traditional_warnings.c:26:12]
// DEFAULT: 25 │ char *y = ALERT;
// DEFAULT: 26 │ char *z = "\u0F43";
// DEFAULT: ·            ──
// DEFAULT: 27 │ int ok = 1;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN IR-DEFAULT
// IR-DEFAULT: module {
// IR-DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-DEFAULT-NEXT:         endian = little;
// IR-DEFAULT-NEXT:         pointer [size=8, align=8];
// IR-DEFAULT-NEXT:         stack_alignment = 16;
// IR-DEFAULT-NEXT:         long_double = f80;
// IR-DEFAULT-NEXT:         storage bool [size=1, align=1];
// IR-DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// IR-DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage f16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage f32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage f64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage f80 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage f128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage d32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage d64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage d128 [size=16, align=16];
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i32 [storage=static] [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([0, 0]) [linkage=internal];
// IR-DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]]) [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([7, 0]) [linkage=internal];
// IR-DEFAULT-NEXT:     global %[[VALUE_y:[0-9]+]] y: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_2]]) [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([224, 189, 131, 0]) [linkage=internal];
// IR-DEFAULT-NEXT:     global %[[VALUE_z:[0-9]+]] z: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]]) [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_ok:[0-9]+]] ok: i32 [storage=static] = const<i32>(1) [linkage=external];
// IR-DEFAULT-NEXT: }
// SLATE-FILECHECK-END IR-DEFAULT
