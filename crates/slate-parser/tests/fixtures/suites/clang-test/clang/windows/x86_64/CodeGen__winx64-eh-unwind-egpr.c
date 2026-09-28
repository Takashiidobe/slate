// Per-function check should NOT fire when the function is nounwind (no unwind
// info is emitted) — this run compiles successfully without -fexceptions.

void g(void);
void f(void) { g(); }

// EGPR on Windows x64 with no explicit unwind mode should auto-promote to V3.

// EGPR on Windows x64 with explicit V3 should still emit V3.

// EGPR on Windows x64 with explicit V1 + exceptions should produce an error
// (the function needs unwind info).

// EGPR on Windows x64 with explicit V2 + exceptions should produce an error.

// EGPR on Windows x64 with explicit V1 and no exceptions: the function is
// nounwind, so no error and no module flag (V1 is the default).

// Without EGPR on Windows x64, default should be V1 (no flag emitted).

// EGPR on non-Windows should not emit the flag.

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %0 @g() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
