/* PR target/91472 */
/* Reported by John Paul Adrian Glaubitz <glaubitz@physik.fu-berlin.de> */
/* { dg-require-effective-target double64plus } */

#if __SIZEOF_INT__ >= 4
typedef unsigned int gmp_uint_least32_t;
#else
typedef __UINT_LEAST32_TYPE__ gmp_uint_least32_t;
#endif

union ieee_double_extract {
  struct {
    gmp_uint_least32_t sig  : 1;
    gmp_uint_least32_t exp  : 11;
    gmp_uint_least32_t manh : 20;
    gmp_uint_least32_t manl : 32;
  } s;
  double d;
};

double __attribute__((noipa)) tests_infinity_d(void) {
  union ieee_double_extract x;
  x.s.exp  = 2047;
  x.s.manl = 0;
  x.s.manh = 0;
  x.s.sig  = 0;
  return x.d;
}

int main(void) {
  double x = tests_infinity_d();
  if (x == 0.0)
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type0 gmp_uint_least32_t = u32;
// DEFAULT-NEXT:     type @type1 ieee_double_extract = union {
// DEFAULT-NEXT:         field0 s: @type2;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 sig: u32 : 1;
// DEFAULT-NEXT:         field1 exp: u32 : 11;
// DEFAULT-NEXT:         field2 manh: u32 : 20;
// DEFAULT-NEXT:         field3 manl: u32 : 32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0, 1, 4], bit_offsets=[Some(0), Some(1), Some(12), Some(32)], bit_units=[(0, 8)], field_units=[Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %3 @tests_infinity_d() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 x: @type1 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..8, bits=1..12>(field0(%4)), reinterpret<u32, reason=assign, fits=always>(const<i32>(2047)));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..8, bits=32..64>(field0(%4)), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..8, bits=12..32>(field0(%4)), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..8, bits=0..1>(field0(%4)), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return read<f64>(field1(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 x: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(%3);
// DEFAULT-NEXT:         if eq<f64, exceptions=ignore>(read<f64>(%6), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
