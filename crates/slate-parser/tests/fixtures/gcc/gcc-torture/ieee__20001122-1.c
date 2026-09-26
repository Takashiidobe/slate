/* { dg-do run } */
void abort(void);
void exit(int);

volatile double a, *p;

int main() {
  double          c, d;
  volatile double b;

  d = 1.0;
  p = &b;
  do {
    c = d;
    d = c * 0.5;
    b = 1 + d;
  } while (b != 1.0);

  a = 1.0 + c;
  if (a == 1.0)
    abort();

  exit(0);
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
// DEFAULT-NEXT:     global %2 a: volatile f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 p: ptr<volatile f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 c: f64 [storage=automatic];
// DEFAULT-NEXT:         let %6 d: f64 [storage=automatic];
// DEFAULT-NEXT:         let %7 b: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%6, const<f64>(1.0));
// DEFAULT-NEXT:         write<ptr<volatile f64>>(%3, addr_of<ptr<volatile f64>>(%7));
// DEFAULT-NEXT:         do %9
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64>(%5, read<f64>(%6));
// DEFAULT-NEXT:                 write<f64>(%6, mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%5), const<f64>(0.5)));
// DEFAULT-NEXT:                 write<f64, volatile>(%7, add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%6)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<f64, exceptions=ignore>(read<f64, volatile>(%7), const<f64>(1.0));
// DEFAULT-NEXT:         write<f64, volatile>(%2, add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), read<f64>(%5)));
// DEFAULT-NEXT:         if eq<f64, exceptions=ignore>(read<f64, volatile>(%2), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
