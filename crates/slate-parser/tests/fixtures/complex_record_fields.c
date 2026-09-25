typedef __complex__ float complex_float;

struct complex_fields {
  _Complex signed char    c8;
  _Complex unsigned short u16;
  complex_float           f32;
  _Complex double         f64;
};

union complex_union {
  _Complex double    value;
  unsigned long long words[2];
};

struct nested_fields {
  struct {
    unsigned short significand[4], exponent, padding[3];
  } values;
};

int main(void) {
  struct complex_fields fields  = {0};
  union complex_union   overlay = {0};

  fields.c8     = 1 + 2i;
  fields.u16    = 3 + 4i;
  fields.f32    = 5.0f + 6.0fi;
  fields.f64    = 7.0 + 8.0i;
  overlay.value = fields.f64;

  int failed = __real__ fields.c8 != 1 || __imag__ fields.c8 != 2 ||
               __real__ fields.u16 != 3 || __imag__ fields.u16 != 4 ||
               __real__ fields.f32 != 5.0f || __imag__ fields.f32 != 6.0f ||
               __real__ overlay.value != 7.0 || __imag__ overlay.value != 8.0;
  return failed;
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
// DEFAULT-NEXT:     type @type0 complex_float = complex<f32>;
// DEFAULT-NEXT:     type @type1 complex_fields = struct {
// DEFAULT-NEXT:         field0 c8: complex<i8>;
// DEFAULT-NEXT:         field1 u16: complex<u16>;
// DEFAULT-NEXT:         field2 f32: complex<f32>;
// DEFAULT-NEXT:         field3 f64: complex<f64>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 2, 8, 16]];
// DEFAULT-NEXT:     type @type2 complex_union = union {
// DEFAULT-NEXT:         field0 value: complex<f64>;
// DEFAULT-NEXT:         field1 words: array<u64, 2>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type3 nested_fields = struct {
// DEFAULT-NEXT:         field0 values: @type4;
// DEFAULT-NEXT:     } [size=16, align=2, offsets=[0]];
// DEFAULT-NEXT:     type @type4 = struct {
// DEFAULT-NEXT:         field0 significand: array<u16, 4>;
// DEFAULT-NEXT:         field1 exponent: u16;
// DEFAULT-NEXT:         field2 padding: array<u16, 3>;
// DEFAULT-NEXT:     } [size=16, align=2, offsets=[0, 8, 10]];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 fields: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>(field0 = real_to_complex<complex<i8>, reason=assign>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %7 overlay: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = real_to_complex<complex<f64>, reason=assign>(int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         write<complex<i8>>(field0(%6), complex_convert<complex<i8>, reason=assign, fits=unknown>(add<complex<i32>, complex=true, overflow=ub>(const<i32>(1), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(2)))));
// DEFAULT-NEXT:         write<complex<u16>>(field1(%6), complex_convert<complex<u16>, reason=assign, fits=unknown>(add<complex<i32>, complex=true, overflow=ub>(const<i32>(3), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(4)))));
// DEFAULT-NEXT:         write<complex<f32>>(field2(%6), add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f32>(5.0), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(6.0))));
// DEFAULT-NEXT:         write<complex<f64>>(field3(%6), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(7.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(8.0))));
// DEFAULT-NEXT:         write<complex<f64>>(field0(%7), read<complex<f64>>(field3(%6)));
// DEFAULT-NEXT:         let %8 failed: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(real(field0(%6)))), const<i32>(1)), ne<i32>(widen<i32, reason=promotion>(read<i8>(imag(field0(%6)))), const<i32>(2))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(real(field1(%6))))), const<i32>(3))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(imag(field1(%6))))), const<i32>(4))), ne<f32, exceptions=ignore>(read<f32>(real(field2(%6))), const<f32>(5.0))), ne<f32, exceptions=ignore>(read<f32>(imag(field2(%6))), const<f32>(6.0))), ne<f64, exceptions=ignore>(read<f64>(real(field0(%7))), const<f64>(7.0))), ne<f64, exceptions=ignore>(read<f64>(imag(field0(%7))), const<f64>(8.0))));
// DEFAULT-NEXT:         return read<i32>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
