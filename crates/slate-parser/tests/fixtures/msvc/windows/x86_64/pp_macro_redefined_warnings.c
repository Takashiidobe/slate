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
#define __TIME__ "now"
#undef __DATE__
int x;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ 'SPACED': macro redefinition
// DEFAULT: ╭─[tests/fixtures/msvc/windows/x86_64/pp_macro_redefined_warnings.c:5:9]
// DEFAULT: 4 │ #define SPACED 1+2
// DEFAULT: 5 │ #define SPACED 1 + 2
// DEFAULT: ·         ──────
// DEFAULT: 6 │ #define CHANGED 1
// DEFAULT: ╰────
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ 'CHANGED': macro redefinition
// DEFAULT: ╭─[tests/fixtures/msvc/windows/x86_64/pp_macro_redefined_warnings.c:7:9]
// DEFAULT: 6 │ #define CHANGED 1
// DEFAULT: 7 │ #define CHANGED 2
// DEFAULT: ·         ───────
// DEFAULT: 8 │ #define RENAMED(a) a
// DEFAULT: ╰────
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ 'SHAPE': macro redefinition
// DEFAULT: ╭─[tests/fixtures/msvc/windows/x86_64/pp_macro_redefined_warnings.c:11:9]
// DEFAULT: 10 │ #define SHAPE 1
// DEFAULT: 11 │ #define SHAPE() 1
// DEFAULT: ·         ─────
// DEFAULT: 12 │ #define __TIME__ "now"
// DEFAULT: ╰────
// DEFAULT: -Wbuiltin-macro-redefined
// DEFAULT: ⚠ macro name '__TIME__' is reserved, '#define' ignored
// DEFAULT: ╭─[tests/fixtures/msvc/windows/x86_64/pp_macro_redefined_warnings.c:12:9]
// DEFAULT: 11 │ #define SHAPE() 1
// DEFAULT: 12 │ #define __TIME__ "now"
// DEFAULT: ·         ────────
// DEFAULT: 13 │ #undef __DATE__
// DEFAULT: ╰────
// DEFAULT: -Wbuiltin-macro-redefined
// DEFAULT: ⚠ macro name '__DATE__' is reserved, '#undef' ignored
// DEFAULT: ╭─[tests/fixtures/msvc/windows/x86_64/pp_macro_redefined_warnings.c:13:8]
// DEFAULT: 12 │ #define __TIME__ "now"
// DEFAULT: 13 │ #undef __DATE__
// DEFAULT: ·        ────────
// DEFAULT: 14 │ int x;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN IR-DEFAULT
// IR-DEFAULT: module {
// IR-DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-DEFAULT-NEXT:         endian = little;
// IR-DEFAULT-NEXT:         pointer [size=8, align=8];
// IR-DEFAULT-NEXT:         stack_alignment = 16;
// IR-DEFAULT-NEXT:         long_double = f64;
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
// IR-DEFAULT-NEXT:         storage f128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage d32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage d64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage d128 [size=16, align=16];
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] [linkage=external];
// IR-DEFAULT-NEXT: }
// SLATE-FILECHECK-END IR-DEFAULT
