void abort(void);
void exit(int);

float __complex__ p(float __complex__ a, float __complex__ b) { return a + b; }

float __complex__ x = 1.0 + 14.0 * (1.0fi);
float __complex__ y = 7.0 + 5.0 * (1.0fi);
float __complex__ w = 8.0 + 19.0 * (1.0fi);
float __complex__ z;

int main(void) {

  z = p(x, y);
  y = p(x, 1.0f / z);
  if (z != w)
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
// DEFAULT-NEXT:     global %5 x: complex<f32> [storage=static] = complex_convert<complex<f32>, reason=assign, rounding=nearest_even, exceptions=ignore>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(14.0), complex_convert<complex<f64>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)))))) [linkage=external];
// DEFAULT-NEXT:     global %6 y: complex<f32> [storage=static] = complex_convert<complex<f32>, reason=assign, rounding=nearest_even, exceptions=ignore>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(7.0), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(5.0), complex_convert<complex<f64>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)))))) [linkage=external];
// DEFAULT-NEXT:     global %7 w: complex<f32> [storage=static] = complex_convert<complex<f32>, reason=assign, rounding=nearest_even, exceptions=ignore>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(8.0), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(19.0), complex_convert<complex<f64>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)))))) [linkage=external];
// DEFAULT-NEXT:     global %8 z: complex<f32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%10 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @p(%3 a: complex<f32>, %4 b: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(coerce<pair<f32>>, coerce<pair<f32>>) -> coerce<pair<f32>>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore>(read<complex<f32>>(%3), read<complex<f32>>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<complex<f32>>(%8, call<complex<f32>, signature=fn(complex<f32>, complex<f32>) -> complex<f32>, abi=sysv64(coerce<pair<f32>>, coerce<pair<f32>>) -> coerce<pair<f32>>>(%2, read<complex<f32>>(%5), read<complex<f32>>(%6)));
// DEFAULT-NEXT:         call<complex<f32>, signature=fn(complex<f32>, complex<f32>) -> complex<f32>, abi=sysv64(coerce<pair<f32>>, coerce<pair<f32>>) -> coerce<pair<f32>>>(%2, read<complex<f32>>(%5), read<complex<f32>>(%6));
// DEFAULT-NEXT:         write<complex<f32>>(%6, call<complex<f32>, signature=fn(complex<f32>, complex<f32>) -> complex<f32>, abi=sysv64(coerce<pair<f32>>, coerce<pair<f32>>) -> coerce<pair<f32>>>(%2, read<complex<f32>>(%5), div<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f32>(1.0), read<complex<f32>>(%8))));
// DEFAULT-NEXT:         call<complex<f32>, signature=fn(complex<f32>, complex<f32>) -> complex<f32>, abi=sysv64(coerce<pair<f32>>, coerce<pair<f32>>) -> coerce<pair<f32>>>(%2, read<complex<f32>>(%5), div<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f32>(1.0), read<complex<f32>>(%8)));
// DEFAULT-NEXT:         if ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%8), read<complex<f32>>(%7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
