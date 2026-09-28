/* PR 3177 */
/* Produced a SIGILL on ia64 with sibcall from F to G.  We hadn't
   widened the register window to allow for the fourth outgoing
   argument as an "in" register.  */

float g(void *a, void *b, int e, int c, float d) { return d; }

float f(void *a, void *b, int c, float d) { return g(a, b, 0, c, d); }

int main() {
  f(0, 0, 1, 1);
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     fn %0 @g(%1 a: ptr<void>, %2 b: ptr<void>, %3 e: i32, %4 c: i32, %5 d: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<f32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f(%7 a: ptr<void>, %8 b: ptr<void>, %9 c: i32, %10 d: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(ptr<void>, ptr<void>, i32, i32, f32) -> f32>(%0, read<ptr<void>>(%7), read<ptr<void>>(%8), const<i32>(0), read<i32>(%9), read<f32>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<f32, signature=fn(ptr<void>, ptr<void>, i32, f32) -> f32>(%6, null<ptr<void>>, null<ptr<void>>, const<i32>(1), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
