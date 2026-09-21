// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

// MSVC accepts _Alignas on a parameter and reports __alignof(p) raised to the
// natural alignment, unlike clang, which reports the request as written.
unsigned long above_natural(_Alignas(32) int p) { return _Alignof(p); }

unsigned long below_natural(_Alignas(1) int p) { return _Alignof(p); }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %0 @above_natural(%1 p: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(32);
// IR-NEXT:     }
// IR-NEXT:     fn %2 @below_natural(%3 p: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(4);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
