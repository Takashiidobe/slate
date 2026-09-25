// SLATE-FILECHECK-DEFINES DEFAULT

/* Test that prefix attributes after a comma only apply to a single
   declared object or function.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk>.  */

__attribute__((noreturn)) void d0 (void), __attribute__((format(printf, 1, 2))) d1 (const char *, ...), d2 (void);

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
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
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %0 @d0() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @d1(%3 <unnamed>: ptr<const i8>, ...) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @d2() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
