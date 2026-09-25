/* PR tree-optimization/58791 */
/* { dg-do run } */
/* { dg-options "-g -ffast-math" } */

#if defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#define NOP "nop 0"
#elif defined(__MMIX__)
#define NOP "swym 0"
#elif defined(__or1k__)
#define NOP "l.nop"
#else
#define NOP "nop"
#endif

__attribute__((noinline, noclone)) double foo(float a, float b, float c,
                                              float d, float l, double u) {
  float  e = a * d;
  float  f = d * e;
  double g = (double)f;
  double h = (double)b;
  double i =
      g *
      h; /* { dg-final { gdb-test pr58791-4.c:32 "i" "486" { target { { i?86-*-* x86_64-*-* } && lp64 } } } } */
  double i2 =
      i +
      1.0; /* { dg-final { gdb-test pr58791-4.c:32 "i2" "487" { target { { i?86-*-* x86_64-*-* } && lp64 } } } } */
  double j = i * 3.25;
  double k = h + j;
  float  m = l * 8.75;
  double n = (double)m;
  double o = (double)a;
  double p = n * o;
  double q = h * p;
  double r = q * 2.5;
  double s = k - r;
  double t = (double)c;
  double v = o * u;
  double w = o * v;
  double x = h * w;
  double y = h * x;
  double z = y * 8.5;
  asm volatile(NOP : : : "memory");
  asm volatile(NOP : : : "memory");
  return s - z;
}

int
main() {
  foo(3.0f, 2.0f, -1.0f, 9.0f, 1.0f, 2.0);
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
// DEFAULT-NEXT:     fn %0 @foo(%1 a: f32, %2 b: f32, %3 c: f32, %4 d: f32, %5 l: f32, %6 u: f64) -> f64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 e: f32 [storage=automatic] = mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%1), read<f32>(%4));
// DEFAULT-NEXT:         let %8 f: f32 [storage=automatic] = mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%4), read<f32>(%7));
// DEFAULT-NEXT:         let %9 g: f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f32>(%8));
// DEFAULT-NEXT:         let %10 h: f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f32>(%2));
// DEFAULT-NEXT:         let %11 i: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%9), read<f64>(%10));
// DEFAULT-NEXT:         let %12 i2: f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%11), const<f64>(1.0));
// DEFAULT-NEXT:         let %13 j: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%11), const<f64>(3.25));
// DEFAULT-NEXT:         let %14 k: f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%10), read<f64>(%13));
// DEFAULT-NEXT:         let %15 m: f32 [storage=automatic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(mul<f64, rounding=nearest_even, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(%5)), const<f64>(8.75)));
// DEFAULT-NEXT:         let %16 n: f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f32>(%15));
// DEFAULT-NEXT:         let %17 o: f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f32>(%1));
// DEFAULT-NEXT:         let %18 p: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%16), read<f64>(%17));
// DEFAULT-NEXT:         let %19 q: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%10), read<f64>(%18));
// DEFAULT-NEXT:         let %20 r: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%19), const<f64>(2.5));
// DEFAULT-NEXT:         let %21 s: f64 [storage=automatic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%14), read<f64>(%20));
// DEFAULT-NEXT:         let %22 t: f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f32>(%3));
// DEFAULT-NEXT:         let %23 v: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%17), read<f64>(%6));
// DEFAULT-NEXT:         let %24 w: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%17), read<f64>(%23));
// DEFAULT-NEXT:         let %25 x: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%10), read<f64>(%24));
// DEFAULT-NEXT:         let %26 y: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%10), read<f64>(%25));
// DEFAULT-NEXT:         let %27 z: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%26), const<f64>(8.5));
// DEFAULT-NEXT:         asm volatile "nop" {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "nop" {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%21), read<f64>(%27));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<f64, signature=fn(f32, f32, f32, f32, f32, f64) -> f64>(%0, const<f32>(3.0), const<f32>(2.0), neg<f32>(const<f32>(1.0)), const<f32>(9.0), const<f32>(1.0), const<f64>(2.0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
