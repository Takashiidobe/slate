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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_le:[0-9]+]] le: f64 [const], %[[VALUE_ri:[0-9]+]] ri: f64 [const]) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_val:[0-9]+]] val: f64 [storage=automatic] = div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_ri]]), read<f64>(%[[VALUE_le]])), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_ri]]), add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_le]]), const<f64>(1.0))));
// DEFAULT-NEXT:         return read<f64>(%[[VALUE_val]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_retval:[0-9]+]] retval: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%[[VALUE_retval]], call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test]], const<f64>(1.0), const<f64>(2.0)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test]], const<f64>(1.0), const<f64>(2.0));
// DEFAULT-NEXT:         if logical_or<bool>(lt<f64, exceptions=observable>(read<f64>(%[[VALUE_retval]]), const<f64>(0.24)), gt<f64, exceptions=observable>(read<f64>(%[[VALUE_retval]]), const<f64>(0.26)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
