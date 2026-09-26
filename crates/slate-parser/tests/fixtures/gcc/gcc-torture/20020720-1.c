/* Copyright (C) 2002  Free Software Foundation.

   Ensure that fabs(x) < 0.0 optimization is working.

   Written by Roger Sayle, 20th July 2002.  */

extern void   abort(void);
extern double fabs(double);
extern void   link_error(void);

void foo(double x) {
  double p, q;

  p = fabs(x);
  q = 0.0;
  if (p < q)
    link_error();
}

int main() {
  foo(1.0);
  return 0;
}

#ifndef __OPTIMIZE__
void link_error() { abort(); }
#endif


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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @fabs(%8 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @link_error() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @foo(%4 x: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 p: f64 [storage=automatic];
// DEFAULT-NEXT:         let %6 q: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%5, call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%4)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%4));
// DEFAULT-NEXT:         write<f64>(%6, const<f64>(0.0));
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(read<f64>(%5), read<f64>(%6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(%3, const<f64>(1.0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
