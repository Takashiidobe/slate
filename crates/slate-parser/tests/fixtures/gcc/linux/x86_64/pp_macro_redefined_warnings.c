// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-WARNING DEFAULT

#define SAME 1 + 2
#define SAME 1    +   2
#define SPACED 1+2
#define SPACED 1 + 2
#define CHANGED 1
#define CHANGED 2
#define RENAMED(a) a
#define RENAMED(b) b
#define SHAPE 1
#define SHAPE() 1
#define TWICE
#define TWICE
#define REDEFINED_AFTER_UNDEF 1
#undef REDEFINED_AFTER_UNDEF
#define REDEFINED_AFTER_UNDEF 2
#define __TIME__ "now"
#define __TIME__ "later"
#undef __DATE__
#define __LINE__ 1
#undef __COUNTER__
#define __STDC_EXTRA 1
#define __STDC_EXTRA 1
#undef __STDC_EXTRA
#define __STDC_FORMAT_MACROS 1
#define __STDC_FORMAT_MACROS 1
#undef __STDC_HOSTED__
#define __GNUC__ 99
#undef __x86_64__
int x;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ 'SPACED' redefined
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_macro_redefined_warnings.c:5:9]
// DEFAULT: 4 │ #define SPACED 1+2
// DEFAULT: 5 │ #define SPACED 1 + 2
// DEFAULT: ·         ──────
// DEFAULT: 6 │ #define CHANGED 1
// DEFAULT: ╰────
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ 'CHANGED' redefined
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_macro_redefined_warnings.c:7:9]
// DEFAULT: 6 │ #define CHANGED 1
// DEFAULT: 7 │ #define CHANGED 2
// DEFAULT: ·         ───────
// DEFAULT: 8 │ #define RENAMED(a) a
// DEFAULT: ╰────
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ 'RENAMED' redefined
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_macro_redefined_warnings.c:9:9]
// DEFAULT: 8 │ #define RENAMED(a) a
// DEFAULT: 9 │ #define RENAMED(b) b
// DEFAULT: ·         ───────
// DEFAULT: 10 │ #define SHAPE 1
// DEFAULT: ╰────
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ 'SHAPE' redefined
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_macro_redefined_warnings.c:11:9]
// DEFAULT: 10 │ #define SHAPE 1
// DEFAULT: 11 │ #define SHAPE() 1
// DEFAULT: ·         ─────
// DEFAULT: 12 │ #define TWICE
// DEFAULT: ╰────
// DEFAULT: -Wbuiltin-macro-redefined
// DEFAULT: ⚠ '__TIME__' redefined
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_macro_redefined_warnings.c:17:9]
// DEFAULT: 16 │ #define REDEFINED_AFTER_UNDEF 2
// DEFAULT: 17 │ #define __TIME__ "now"
// DEFAULT: ·         ────────
// DEFAULT: 18 │ #define __TIME__ "later"
// DEFAULT: ╰────
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ '__TIME__' redefined
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_macro_redefined_warnings.c:18:9]
// DEFAULT: 17 │ #define __TIME__ "now"
// DEFAULT: 18 │ #define __TIME__ "later"
// DEFAULT: ·         ────────
// DEFAULT: 19 │ #undef __DATE__
// DEFAULT: ╰────
// DEFAULT: -Wbuiltin-macro-redefined
// DEFAULT: ⚠ undefining '__DATE__'
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_macro_redefined_warnings.c:19:8]
// DEFAULT: 18 │ #define __TIME__ "later"
// DEFAULT: 19 │ #undef __DATE__
// DEFAULT: ·        ────────
// DEFAULT: 20 │ #define __LINE__ 1
// DEFAULT: ╰────
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ '__LINE__' redefined
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_macro_redefined_warnings.c:20:9]
// DEFAULT: 19 │ #undef __DATE__
// DEFAULT: 20 │ #define __LINE__ 1
// DEFAULT: ·         ────────
// DEFAULT: 21 │ #undef __COUNTER__
// DEFAULT: ╰────
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ undefining '__COUNTER__'
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_macro_redefined_warnings.c:21:8]
// DEFAULT: 20 │ #define __LINE__ 1
// DEFAULT: 21 │ #undef __COUNTER__
// DEFAULT: ·        ───────────
// DEFAULT: 22 │ #define __STDC_EXTRA 1
// DEFAULT: ╰────
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ '__STDC_EXTRA' redefined
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_macro_redefined_warnings.c:23:9]
// DEFAULT: 22 │ #define __STDC_EXTRA 1
// DEFAULT: 23 │ #define __STDC_EXTRA 1
// DEFAULT: ·         ────────────
// DEFAULT: 24 │ #undef __STDC_EXTRA
// DEFAULT: ╰────
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ undefining '__STDC_EXTRA'
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_macro_redefined_warnings.c:24:8]
// DEFAULT: 23 │ #define __STDC_EXTRA 1
// DEFAULT: 24 │ #undef __STDC_EXTRA
// DEFAULT: ·        ────────────
// DEFAULT: 25 │ #define __STDC_FORMAT_MACROS 1
// DEFAULT: ╰────
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ undefining '__STDC_HOSTED__'
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_macro_redefined_warnings.c:27:8]
// DEFAULT: 26 │ #define __STDC_FORMAT_MACROS 1
// DEFAULT: 27 │ #undef __STDC_HOSTED__
// DEFAULT: ·        ───────────────
// DEFAULT: 28 │ #define __GNUC__ 99
// DEFAULT: ╰────
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ '__GNUC__' redefined
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_macro_redefined_warnings.c:28:9]
// DEFAULT: 27 │ #undef __STDC_HOSTED__
// DEFAULT: 28 │ #define __GNUC__ 99
// DEFAULT: ·         ────────
// DEFAULT: 29 │ #undef __x86_64__
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
// IR-DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] [linkage=external];
// IR-DEFAULT-NEXT: }
// SLATE-FILECHECK-END IR-DEFAULT
