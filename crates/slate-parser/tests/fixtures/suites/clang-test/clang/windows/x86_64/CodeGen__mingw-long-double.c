
struct {
  char c;
  long double ldb;
} agggregate_LD = {};

long double dataLD = 1.0L;

long double _Complex dataLDC = {1.0L, 1.0L};

long double TestLD(long double x) {
  return x * x;
}

long double _Complex TestLDC(long double _Complex x) {
  return x * x;
}


void VarArgLD(int a, ...) {
  __builtin_va_list ap;
  __builtin_va_start(ap, a);
  long double LD = __builtin_va_arg(ap, long double);
  __builtin_va_end(ap);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 ldb: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %1 agggregate_LD: @type0 [storage=static] = aggregate<@type0, zero_fill=true>() [linkage=external];
// DEFAULT-NEXT:     global %2 dataLD: f64 [storage=static] = const<f64>(1.0) [linkage=external];
// DEFAULT-NEXT:     global %3 dataLDC: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(1.0), index1 = const<f64>(1.0)) [linkage=external];
// DEFAULT-NEXT:     fn %4 @TestLD(%5 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%5), read<f64>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @TestLDC(%7 x: complex<f64>) -> complex<f64> [linkage=external] [abi=win64(byref<align=8>) -> sret<align=8>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%7), read<complex<f64>>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @VarArgLD(%9 a: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10 ap: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_start(%10);
// DEFAULT-NEXT:         let %11 LD: f64 [storage=automatic] = va_arg<f64>(%10);
// DEFAULT-NEXT:         va_end(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
