;
int x;;;
int f(void) { ;; return 0; };

// SLATE-FILECHECK-DEFINES STRICT
// SLATE-FILECHECK-PREFIX-ARGS STRICT /Za /W4
// SLATE-FILECHECK-WARNING STRICT
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT /W4
// SLATE-FILECHECK-WARNING DEFAULT
// SLATE-FILECHECK-DEFINES LOW
// SLATE-FILECHECK-PREFIX-ARGS LOW /Za /W3
// SLATE-FILECHECK-WARNING LOW

// SLATE-FILECHECK-BEGIN STRICT
// STRICT: -Wextra-top-level-semi
// STRICT:   ⚠ empty statement at global scope
// STRICT:    ╭─[tests/fixtures/msvc/windows/x86_64/za_extra_file_scope_semi.c:1:1]
// STRICT:  1 │ ;
// STRICT:    · ─
// STRICT:  2 │ int x;;;
// STRICT:    ╰────
// STRICT: -Wextra-top-level-semi
// STRICT:   ⚠ empty statement at global scope
// STRICT:    ╭─[tests/fixtures/msvc/windows/x86_64/za_extra_file_scope_semi.c:2:7]
// STRICT:  1 │ ;
// STRICT:  2 │ int x;;;
// STRICT:    ·       ─
// STRICT:  3 │ int f(void) { ;; return 0; };
// STRICT:    ╰────
// STRICT: -Wextra-top-level-semi
// STRICT:   ⚠ empty statement at global scope
// STRICT:    ╭─[tests/fixtures/msvc/windows/x86_64/za_extra_file_scope_semi.c:2:8]
// STRICT:  1 │ ;
// STRICT:  2 │ int x;;;
// STRICT:    ·        ─
// STRICT:  3 │ int f(void) { ;; return 0; };
// STRICT:    ╰────
// STRICT: -Wextra-top-level-semi
// STRICT:   ⚠ empty statement at global scope
// STRICT:    ╭─[tests/fixtures/msvc/windows/x86_64/za_extra_file_scope_semi.c:3:29]
// STRICT:  2 │ int x;;;
// STRICT:  3 │ int f(void) { ;; return 0; };
// STRICT:    ·                             ─
// STRICT:  4 │
// STRICT:    ╰────
// SLATE-FILECHECK-END STRICT
// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN LOW
// SLATE-FILECHECK-END LOW
// SLATE-FILECHECK-BEGIN IR-STRICT
// IR-STRICT: module {
// IR-STRICT-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-STRICT-NEXT:         endian = little;
// IR-STRICT-NEXT:         pointer [size=8, align=8];
// IR-STRICT-NEXT:         stack_alignment = 16;
// IR-STRICT-NEXT:         long_double = f64;
// IR-STRICT-NEXT:         storage bool [size=1, align=1];
// IR-STRICT-NEXT:         storage i8, u8 [size=1, align=1];
// IR-STRICT-NEXT:         storage i16, u16 [size=2, align=2];
// IR-STRICT-NEXT:         storage i32, u32 [size=4, align=4];
// IR-STRICT-NEXT:         storage i64, u64 [size=8, align=8];
// IR-STRICT-NEXT:         storage i128, u128 [size=16, align=16];
// IR-STRICT-NEXT:         storage bf16 [size=2, align=2];
// IR-STRICT-NEXT:         storage f16 [size=2, align=2];
// IR-STRICT-NEXT:         storage f32 [size=4, align=4];
// IR-STRICT-NEXT:         storage f64 [size=8, align=8];
// IR-STRICT-NEXT:         storage f128 [size=16, align=16];
// IR-STRICT-NEXT:         storage d32 [size=4, align=4];
// IR-STRICT-NEXT:         storage d64 [size=8, align=8];
// IR-STRICT-NEXT:         storage d128 [size=16, align=16];
// IR-STRICT-NEXT:     }
// IR-STRICT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] [linkage=external];
// IR-STRICT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-STRICT-NEXT:         ;
// IR-STRICT-NEXT:         ;
// IR-STRICT-NEXT:         return const<i32>(0);
// IR-STRICT-NEXT:     }
// IR-STRICT-NEXT: }
// SLATE-FILECHECK-END IR-STRICT
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
// IR-DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-DEFAULT-NEXT:         ;
// IR-DEFAULT-NEXT:         ;
// IR-DEFAULT-NEXT:         return const<i32>(0);
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT: }
// SLATE-FILECHECK-END IR-DEFAULT
// SLATE-FILECHECK-BEGIN IR-LOW
// IR-LOW: module {
// IR-LOW-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-LOW-NEXT:         endian = little;
// IR-LOW-NEXT:         pointer [size=8, align=8];
// IR-LOW-NEXT:         stack_alignment = 16;
// IR-LOW-NEXT:         long_double = f64;
// IR-LOW-NEXT:         storage bool [size=1, align=1];
// IR-LOW-NEXT:         storage i8, u8 [size=1, align=1];
// IR-LOW-NEXT:         storage i16, u16 [size=2, align=2];
// IR-LOW-NEXT:         storage i32, u32 [size=4, align=4];
// IR-LOW-NEXT:         storage i64, u64 [size=8, align=8];
// IR-LOW-NEXT:         storage i128, u128 [size=16, align=16];
// IR-LOW-NEXT:         storage bf16 [size=2, align=2];
// IR-LOW-NEXT:         storage f16 [size=2, align=2];
// IR-LOW-NEXT:         storage f32 [size=4, align=4];
// IR-LOW-NEXT:         storage f64 [size=8, align=8];
// IR-LOW-NEXT:         storage f128 [size=16, align=16];
// IR-LOW-NEXT:         storage d32 [size=4, align=4];
// IR-LOW-NEXT:         storage d64 [size=8, align=8];
// IR-LOW-NEXT:         storage d128 [size=16, align=16];
// IR-LOW-NEXT:     }
// IR-LOW-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] [linkage=external];
// IR-LOW-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-LOW-NEXT:         ;
// IR-LOW-NEXT:         ;
// IR-LOW-NEXT:         return const<i32>(0);
// IR-LOW-NEXT:     }
// IR-LOW-NEXT: }
// SLATE-FILECHECK-END IR-LOW
