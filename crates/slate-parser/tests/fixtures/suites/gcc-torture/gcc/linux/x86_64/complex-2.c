void abort(void);
void exit(int);

__complex__ double f(__complex__ double x, __complex__ double y) {
  x += y;
  return x;
}

__complex__ double ag = 1.0 + 1.0i;
__complex__ double bg = -2.0 + 2.0i;

int main(void) {
  __complex__ double a, b, c;

  a = ag;
  b = -2.0 + 2.0i;
  c = f(a, b);

  if (a != 1.0 + 1.0i)
    abort();
  if (b != -2.0 + 2.0i)
    abort();
  if (c != -1.0 + 3.0i)
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
// DEFAULT-NEXT:     global %[[VALUE_ag:[0-9]+]] ag: complex<f64> [storage=static] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_bg:[0-9]+]] bg: complex<f64> [storage=static] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(neg<f64>(const<f64>(2.0)), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_x:[0-9]+]] x: complex<f64>, %[[VALUE_y:[0-9]+]] y: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: complex<f64> [synthetic] = read<complex<f64>>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: complex<f64> [synthetic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE1]]), read<complex<f64>>(%[[VALUE_y]]));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_x]], read<complex<f64>>(%[[VALUE2]]));
// DEFAULT-NEXT:         return read<complex<f64>>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_a]], read<complex<f64>>(%[[VALUE_ag]]));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_b]], add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f64>(const<f64>(2.0)), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_c]], call<complex<f64>, signature=fn(complex<f64>, complex<f64>) -> complex<f64>, abi=sysv64(native_c, native_c) -> native_c>(%[[VALUE_f]], read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]])));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>, complex<f64>) -> complex<f64>, abi=sysv64(native_c, native_c) -> native_c>(%[[VALUE_f]], read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%[[VALUE_a]]), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%[[VALUE_b]]), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f64>(const<f64>(2.0)), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%[[VALUE_c]]), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f64>(const<f64>(1.0)), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(3.0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
