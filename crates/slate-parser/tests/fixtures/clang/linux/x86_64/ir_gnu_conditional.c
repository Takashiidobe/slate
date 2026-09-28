// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu99

extern int side(void);

int call_once(void) { return side() ?: 7; }

long pointer_default(int *p, long q) { return (long)(p ?: (int *)0) + q; }

double promoted(int n) { return n ?: 1.5; }

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
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %0 @side() -> i32 [linkage=external];
// IR-NEXT:     fn %1 @call_once() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %7: i32 [synthetic] = call<i32, signature=fn() -> i32>(%0);
// IR-NEXT:         return conditional<i32>(ne<i32>(read<i32>(%7), const<i32>(0)), read<i32>(%7), const<i32>(7));
// IR-NEXT:     }
// IR-NEXT:     fn %2 @pointer_default(%3 p: ptr<i32>, %4 q: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %8: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// IR-NEXT:         return add<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(conditional<ptr<i32>>(ne<ptr<i32>>(read<ptr<i32>>(%8), null<ptr<i32>>), read<ptr<i32>>(%8), null<ptr<i32>>)), read<i64>(%4));
// IR-NEXT:     }
// IR-NEXT:     fn %5 @promoted(%6 n: i32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %9: i32 [synthetic] = read<i32>(%6);
// IR-NEXT:         return conditional<f64>(ne<i32>(read<i32>(%9), const<i32>(0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%9)), const<f64>(1.5));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
