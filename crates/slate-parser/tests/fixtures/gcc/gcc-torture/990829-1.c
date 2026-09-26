void abort(void);
void exit(int);

double test(const double le, const double ri) {
  double val = (ri - le) / (ri * (le + 1.0));
  return val;
}

int main() {
  double retval;

  retval = test(1.0, 2.0);
  if (retval < 0.24 || retval > 0.26)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @test(%3 le: f64 [const], %4 ri: f64 [const]) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 val: f64 [storage=automatic] = div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%4), read<f64>(%3)), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%4), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%3), const<f64>(1.0))));
// DEFAULT-NEXT:         return read<f64>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 retval: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%7, call<f64, signature=fn(f64, f64) -> f64>(%2, const<f64>(1.0), const<f64>(2.0)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%2, const<f64>(1.0), const<f64>(2.0));
// DEFAULT-NEXT:         if logical_or<bool>(lt<f64, exceptions=ignore>(read<f64>(%7), const<f64>(0.24)), gt<f64, exceptions=ignore>(read<f64>(%7), const<f64>(0.26)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
