/* Test __builtin_complex semantics.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */
/* { dg-require-effective-target inf } */
/* { dg-add-options ieee } */
/* { dg-skip-if "double support is incomplete" { "avr-*-*" } } */

extern void exit(int);
extern void abort(void);

#define COMPARE_BODY(A, B, TYPE, COPYSIGN)                                     \
  do {                                                                         \
    TYPE s1 = COPYSIGN((TYPE)1.0, A);                                          \
    TYPE s2 = COPYSIGN((TYPE)1.0, B);                                          \
    if (s1 != s2)                                                              \
      abort();                                                                 \
    if ((__builtin_isnan(A) != 0) != (__builtin_isnan(B) != 0))                \
      abort();                                                                 \
    if ((A != B) != (__builtin_isnan(A) != 0))                                 \
      abort();                                                                 \
  } while (0)

void comparef(float a, float b) {
  COMPARE_BODY(a, b, float, __builtin_copysignf);
}

void compare(double a, double b) {
  COMPARE_BODY(a, b, double, __builtin_copysign);
}

void comparel(long double a, long double b) {
  COMPARE_BODY(a, b, long double, __builtin_copysignl);
}

void comparecf(_Complex float a, float r, float i) {
  comparef(__real__ a, r);
  comparef(__imag__ a, i);
}

void comparec(_Complex double a, double r, double i) {
  compare(__real__ a, r);
  compare(__imag__ a, i);
}

void comparecl(_Complex long double a, long double r, long double i) {
  comparel(__real__ a, r);
  comparel(__imag__ a, i);
}

#define VERIFY(A, B, TYPE, COMPARE)                                            \
  do {                                                                         \
    TYPE                 a  = A;                                               \
    TYPE                 b  = B;                                               \
    _Complex TYPE        cr = __builtin_complex(a, b);                         \
    static _Complex TYPE cs = __builtin_complex(A, B);                         \
    COMPARE(cr, A, B);                                                         \
    COMPARE(cs, A, B);                                                         \
  } while (0)

#define ALL_CHECKS(PZ, NZ, NAN, INF, TYPE, COMPARE)                            \
  do {                                                                         \
    VERIFY(PZ, PZ, TYPE, COMPARE);                                             \
    VERIFY(PZ, NZ, TYPE, COMPARE);                                             \
    VERIFY(PZ, NAN, TYPE, COMPARE);                                            \
    VERIFY(PZ, INF, TYPE, COMPARE);                                            \
    VERIFY(NZ, PZ, TYPE, COMPARE);                                             \
    VERIFY(NZ, NZ, TYPE, COMPARE);                                             \
    VERIFY(NZ, NAN, TYPE, COMPARE);                                            \
    VERIFY(NZ, INF, TYPE, COMPARE);                                            \
    VERIFY(NAN, PZ, TYPE, COMPARE);                                            \
    VERIFY(NAN, NZ, TYPE, COMPARE);                                            \
    VERIFY(NAN, NAN, TYPE, COMPARE);                                           \
    VERIFY(NAN, INF, TYPE, COMPARE);                                           \
    VERIFY(INF, PZ, TYPE, COMPARE);                                            \
    VERIFY(INF, NZ, TYPE, COMPARE);                                            \
    VERIFY(INF, NAN, TYPE, COMPARE);                                           \
    VERIFY(INF, INF, TYPE, COMPARE);                                           \
  } while (0)

void check_float(void) {
  ALL_CHECKS(0.0f, -0.0f, __builtin_nanf(""), __builtin_inff(), float,
             comparecf);
}

void check_double(void) {
  ALL_CHECKS(0.0, -0.0, __builtin_nan(""), __builtin_inf(), double, comparec);
}

void check_long_double(void) {
  ALL_CHECKS(0.0l, -0.0l, __builtin_nanl(""), __builtin_infl(), long double,
             comparecl);
}

int main(void) {
  check_float();
  check_double();
  check_long_double();
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
// DEFAULT-NEXT:     global %33 cs: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %37 cs: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0))) [linkage=internal];
// DEFAULT-NEXT:     global %244 .str244: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %245 .str245: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 cs: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%245)))) [linkage=internal];
// DEFAULT-NEXT:     global %246 .str246: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %247 .str247: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %45 cs: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = call<f32, signature=fn() -> f32>(%249)) [linkage=internal];
// DEFAULT-NEXT:     global %49 cs: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = neg<f32>(const<f32>(0.0)), index1 = const<f32>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %53 cs: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = neg<f32>(const<f32>(0.0)), index1 = neg<f32>(const<f32>(0.0))) [linkage=internal];
// DEFAULT-NEXT:     global %253 .str253: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %254 .str254: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %57 cs: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = neg<f32>(const<f32>(0.0)), index1 = call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%254)))) [linkage=internal];
// DEFAULT-NEXT:     global %255 .str255: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %256 .str256: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %61 cs: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = neg<f32>(const<f32>(0.0)), index1 = call<f32, signature=fn() -> f32>(%249)) [linkage=internal];
// DEFAULT-NEXT:     global %259 .str259: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %260 .str260: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %65 cs: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%260))), index1 = const<f32>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %261 .str261: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %262 .str262: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %264 .str264: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %265 .str265: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %69 cs: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%265))), index1 = neg<f32>(const<f32>(0.0))) [linkage=internal];
// DEFAULT-NEXT:     global %266 .str266: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %267 .str267: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %269 .str269: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %270 .str270: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %271 .str271: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %272 .str272: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %73 cs: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%271))), index1 = call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%272)))) [linkage=internal];
// DEFAULT-NEXT:     global %273 .str273: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %274 .str274: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %275 .str275: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %276 .str276: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %278 .str278: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %279 .str279: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %77 cs: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%279))), index1 = call<f32, signature=fn() -> f32>(%249)) [linkage=internal];
// DEFAULT-NEXT:     global %280 .str280: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %281 .str281: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %81 cs: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = call<f32, signature=fn() -> f32>(%249), index1 = const<f32>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %85 cs: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = call<f32, signature=fn() -> f32>(%249), index1 = neg<f32>(const<f32>(0.0))) [linkage=internal];
// DEFAULT-NEXT:     global %285 .str285: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %286 .str286: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %89 cs: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = call<f32, signature=fn() -> f32>(%249), index1 = call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%286)))) [linkage=internal];
// DEFAULT-NEXT:     global %287 .str287: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %288 .str288: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %93 cs: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = call<f32, signature=fn() -> f32>(%249), index1 = call<f32, signature=fn() -> f32>(%249)) [linkage=internal];
// DEFAULT-NEXT:     global %98 cs: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %102 cs: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = neg<f64>(const<f64>(0.0))) [linkage=internal];
// DEFAULT-NEXT:     global %296 .str296: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %297 .str297: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %106 cs: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%297)))) [linkage=internal];
// DEFAULT-NEXT:     global %298 .str298: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %299 .str299: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %110 cs: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = call<f64, signature=fn() -> f64>(%301)) [linkage=internal];
// DEFAULT-NEXT:     global %114 cs: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = neg<f64>(const<f64>(0.0)), index1 = const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %118 cs: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = neg<f64>(const<f64>(0.0)), index1 = neg<f64>(const<f64>(0.0))) [linkage=internal];
// DEFAULT-NEXT:     global %305 .str305: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %306 .str306: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %122 cs: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = neg<f64>(const<f64>(0.0)), index1 = call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%306)))) [linkage=internal];
// DEFAULT-NEXT:     global %307 .str307: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %308 .str308: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %126 cs: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = neg<f64>(const<f64>(0.0)), index1 = call<f64, signature=fn() -> f64>(%301)) [linkage=internal];
// DEFAULT-NEXT:     global %311 .str311: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %312 .str312: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %130 cs: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%312))), index1 = const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %313 .str313: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %314 .str314: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %316 .str316: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %317 .str317: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %134 cs: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%317))), index1 = neg<f64>(const<f64>(0.0))) [linkage=internal];
// DEFAULT-NEXT:     global %318 .str318: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %319 .str319: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %321 .str321: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %322 .str322: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %323 .str323: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %324 .str324: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %138 cs: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%323))), index1 = call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%324)))) [linkage=internal];
// DEFAULT-NEXT:     global %325 .str325: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %326 .str326: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %327 .str327: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %328 .str328: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %330 .str330: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %331 .str331: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %142 cs: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%331))), index1 = call<f64, signature=fn() -> f64>(%301)) [linkage=internal];
// DEFAULT-NEXT:     global %332 .str332: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %333 .str333: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %146 cs: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = call<f64, signature=fn() -> f64>(%301), index1 = const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %150 cs: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = call<f64, signature=fn() -> f64>(%301), index1 = neg<f64>(const<f64>(0.0))) [linkage=internal];
// DEFAULT-NEXT:     global %337 .str337: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %338 .str338: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %154 cs: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = call<f64, signature=fn() -> f64>(%301), index1 = call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%338)))) [linkage=internal];
// DEFAULT-NEXT:     global %339 .str339: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %340 .str340: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %158 cs: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = call<f64, signature=fn() -> f64>(%301), index1 = call<f64, signature=fn() -> f64>(%301)) [linkage=internal];
// DEFAULT-NEXT:     global %163 cs: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %167 cs: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = neg<f80>(const<f80>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %348 .str348: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %349 .str349: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %171 cs: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%349)))) [linkage=internal];
// DEFAULT-NEXT:     global %350 .str350: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %351 .str351: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %175 cs: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = call<f80, signature=fn() -> f80>(%353)) [linkage=internal];
// DEFAULT-NEXT:     global %179 cs: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = neg<f80>(const<f80>(0)), index1 = const<f80>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %183 cs: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = neg<f80>(const<f80>(0)), index1 = neg<f80>(const<f80>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %357 .str357: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %358 .str358: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %187 cs: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = neg<f80>(const<f80>(0)), index1 = call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%358)))) [linkage=internal];
// DEFAULT-NEXT:     global %359 .str359: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %360 .str360: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %191 cs: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = neg<f80>(const<f80>(0)), index1 = call<f80, signature=fn() -> f80>(%353)) [linkage=internal];
// DEFAULT-NEXT:     global %363 .str363: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %364 .str364: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %195 cs: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%364))), index1 = const<f80>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %365 .str365: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %366 .str366: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %368 .str368: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %369 .str369: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %199 cs: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%369))), index1 = neg<f80>(const<f80>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %370 .str370: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %371 .str371: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %373 .str373: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %374 .str374: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %375 .str375: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %376 .str376: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %203 cs: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%375))), index1 = call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%376)))) [linkage=internal];
// DEFAULT-NEXT:     global %377 .str377: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %378 .str378: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %379 .str379: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %380 .str380: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %382 .str382: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %383 .str383: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %207 cs: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%383))), index1 = call<f80, signature=fn() -> f80>(%353)) [linkage=internal];
// DEFAULT-NEXT:     global %384 .str384: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %385 .str385: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %211 cs: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = call<f80, signature=fn() -> f80>(%353), index1 = const<f80>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %215 cs: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = call<f80, signature=fn() -> f80>(%353), index1 = neg<f80>(const<f80>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %389 .str389: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %390 .str390: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %219 cs: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = call<f80, signature=fn() -> f80>(%353), index1 = call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%390)))) [linkage=internal];
// DEFAULT-NEXT:     global %391 .str391: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %392 .str392: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %223 cs: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = call<f80, signature=fn() -> f80>(%353), index1 = call<f80, signature=fn() -> f80>(%353)) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @exit(%225 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %229 @__builtin_copysignf(%227 <unnamed>: f32, %228 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %2 @comparef(%3 a: f32, %4 b: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %226
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %5 s1: f32 [storage=automatic] = call<f32, signature=fn(f32, f32) -> f32>(%229, float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), read<f32>(%3));
// DEFAULT-NEXT:                 let %6 s2: f32 [storage=automatic] = call<f32, signature=fn(f32, f32) -> f32>(%229, float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), read<f32>(%4));
// DEFAULT-NEXT:                 if ne<f32, exceptions=ignore>(read<f32>(%5), read<f32>(%6))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 if ne<i32>(from_bool<i32, reason=promotion>(ne<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%3))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%4))), const<i32>(0))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 if ne<i32>(from_bool<i32, reason=promotion>(ne<f32, exceptions=ignore>(read<f32>(%3), read<f32>(%4))), from_bool<i32, reason=promotion>(ne<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%3))), const<i32>(0))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %233 @__builtin_copysign(%231 <unnamed>: f64, %232 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %7 @compare(%8 a: f64, %9 b: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %230
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %10 s1: f64 [storage=automatic] = call<f64, signature=fn(f64, f64) -> f64>(%233, const<f64>(1.0), read<f64>(%8));
// DEFAULT-NEXT:                 let %11 s2: f64 [storage=automatic] = call<f64, signature=fn(f64, f64) -> f64>(%233, const<f64>(1.0), read<f64>(%9));
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(read<f64>(%10), read<f64>(%11))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 if ne<i32>(from_bool<i32, reason=promotion>(ne<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f64>(%8))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f64>(%9))), const<i32>(0))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 if ne<i32>(from_bool<i32, reason=promotion>(ne<f64, exceptions=ignore>(read<f64>(%8), read<f64>(%9))), from_bool<i32, reason=promotion>(ne<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f64>(%8))), const<i32>(0))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %237 @__builtin_copysignl(%235 <unnamed>: f80, %236 <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %12 @comparel(%13 a: f80, %14 b: f80) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %234
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %15 s1: f80 [storage=automatic] = call<f80, signature=fn(f80, f80) -> f80>(%237, float_widen<f80, reason=explicit>(const<f64>(1.0)), read<f80>(%13));
// DEFAULT-NEXT:                 let %16 s2: f80 [storage=automatic] = call<f80, signature=fn(f80, f80) -> f80>(%237, float_widen<f80, reason=explicit>(const<f64>(1.0)), read<f80>(%14));
// DEFAULT-NEXT:                 if ne<f80, exceptions=ignore>(read<f80>(%15), read<f80>(%16))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 if ne<i32>(from_bool<i32, reason=promotion>(ne<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f80>(%13))), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f80>(%14))), const<i32>(0))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 if ne<i32>(from_bool<i32, reason=promotion>(ne<f80, exceptions=ignore>(read<f80>(%13), read<f80>(%14))), from_bool<i32, reason=promotion>(ne<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f80>(%13))), const<i32>(0))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @comparecf(%18 a: complex<f32>, %19 r: f32, %20 i: f32) -> void [linkage=external] [abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%2, read<f32>(real(%18)), read<f32>(%19));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%2, read<f32>(imag(%18)), read<f32>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @comparec(%22 a: complex<f64>, %23 r: f64, %24 i: f64) -> void [linkage=external] [abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%7, read<f64>(real(%22)), read<f64>(%23));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%7, read<f64>(imag(%22)), read<f64>(%24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @comparecl(%26 a: complex<f80>, %27 r: f80, %28 i: f80) -> void [linkage=external] [abi=sysv64(byval<align=16>, scalar, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(f80, f80) -> void>(%12, read<f80>(real(%26)), read<f80>(%27));
// DEFAULT-NEXT:         call<void, signature=fn(f80, f80) -> void>(%12, read<f80>(imag(%26)), read<f80>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %243 @__builtin_nanf(%242 <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %249 @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %29 @check_float() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %238
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %239
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %30 a: f32 [storage=automatic] = const<f32>(0.0);
// DEFAULT-NEXT:                         let %31 b: f32 [storage=automatic] = const<f32>(0.0);
// DEFAULT-NEXT:                         let %32 cr: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%30), index1 = read<f32>(%31));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%32), const<f32>(0.0), const<f32>(0.0));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%33), const<f32>(0.0), const<f32>(0.0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %240
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %34 a: f32 [storage=automatic] = const<f32>(0.0);
// DEFAULT-NEXT:                         let %35 b: f32 [storage=automatic] = neg<f32>(const<f32>(0.0));
// DEFAULT-NEXT:                         let %36 cr: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%34), index1 = read<f32>(%35));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%36), const<f32>(0.0), neg<f32>(const<f32>(0.0)));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%37), const<f32>(0.0), neg<f32>(const<f32>(0.0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %241
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %38 a: f32 [storage=automatic] = const<f32>(0.0);
// DEFAULT-NEXT:                         let %39 b: f32 [storage=automatic] = call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%244)));
// DEFAULT-NEXT:                         let %40 cr: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%38), index1 = read<f32>(%39));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%40), const<f32>(0.0), call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%246))));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%41), const<f32>(0.0), call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%247))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %248
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %42 a: f32 [storage=automatic] = const<f32>(0.0);
// DEFAULT-NEXT:                         let %43 b: f32 [storage=automatic] = call<f32, signature=fn() -> f32>(%249);
// DEFAULT-NEXT:                         let %44 cr: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%42), index1 = read<f32>(%43));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%44), const<f32>(0.0), call<f32, signature=fn() -> f32>(%249));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%45), const<f32>(0.0), call<f32, signature=fn() -> f32>(%249));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %250
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %46 a: f32 [storage=automatic] = neg<f32>(const<f32>(0.0));
// DEFAULT-NEXT:                         let %47 b: f32 [storage=automatic] = const<f32>(0.0);
// DEFAULT-NEXT:                         let %48 cr: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%46), index1 = read<f32>(%47));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%48), neg<f32>(const<f32>(0.0)), const<f32>(0.0));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%49), neg<f32>(const<f32>(0.0)), const<f32>(0.0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %251
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %50 a: f32 [storage=automatic] = neg<f32>(const<f32>(0.0));
// DEFAULT-NEXT:                         let %51 b: f32 [storage=automatic] = neg<f32>(const<f32>(0.0));
// DEFAULT-NEXT:                         let %52 cr: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%50), index1 = read<f32>(%51));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%52), neg<f32>(const<f32>(0.0)), neg<f32>(const<f32>(0.0)));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%53), neg<f32>(const<f32>(0.0)), neg<f32>(const<f32>(0.0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %252
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %54 a: f32 [storage=automatic] = neg<f32>(const<f32>(0.0));
// DEFAULT-NEXT:                         let %55 b: f32 [storage=automatic] = call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%253)));
// DEFAULT-NEXT:                         let %56 cr: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%54), index1 = read<f32>(%55));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%56), neg<f32>(const<f32>(0.0)), call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%255))));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%57), neg<f32>(const<f32>(0.0)), call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%256))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %257
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %58 a: f32 [storage=automatic] = neg<f32>(const<f32>(0.0));
// DEFAULT-NEXT:                         let %59 b: f32 [storage=automatic] = call<f32, signature=fn() -> f32>(%249);
// DEFAULT-NEXT:                         let %60 cr: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%58), index1 = read<f32>(%59));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%60), neg<f32>(const<f32>(0.0)), call<f32, signature=fn() -> f32>(%249));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%61), neg<f32>(const<f32>(0.0)), call<f32, signature=fn() -> f32>(%249));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %258
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %62 a: f32 [storage=automatic] = call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%259)));
// DEFAULT-NEXT:                         let %63 b: f32 [storage=automatic] = const<f32>(0.0);
// DEFAULT-NEXT:                         let %64 cr: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%62), index1 = read<f32>(%63));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%64), call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%261))), const<f32>(0.0));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%65), call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%262))), const<f32>(0.0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %263
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %66 a: f32 [storage=automatic] = call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%264)));
// DEFAULT-NEXT:                         let %67 b: f32 [storage=automatic] = neg<f32>(const<f32>(0.0));
// DEFAULT-NEXT:                         let %68 cr: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%66), index1 = read<f32>(%67));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%68), call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%266))), neg<f32>(const<f32>(0.0)));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%69), call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%267))), neg<f32>(const<f32>(0.0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %268
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %70 a: f32 [storage=automatic] = call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%269)));
// DEFAULT-NEXT:                         let %71 b: f32 [storage=automatic] = call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%270)));
// DEFAULT-NEXT:                         let %72 cr: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%70), index1 = read<f32>(%71));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%72), call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%273))), call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%274))));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%73), call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%275))), call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%276))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %277
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %74 a: f32 [storage=automatic] = call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%278)));
// DEFAULT-NEXT:                         let %75 b: f32 [storage=automatic] = call<f32, signature=fn() -> f32>(%249);
// DEFAULT-NEXT:                         let %76 cr: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%74), index1 = read<f32>(%75));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%76), call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%280))), call<f32, signature=fn() -> f32>(%249));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%77), call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%281))), call<f32, signature=fn() -> f32>(%249));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %282
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %78 a: f32 [storage=automatic] = call<f32, signature=fn() -> f32>(%249);
// DEFAULT-NEXT:                         let %79 b: f32 [storage=automatic] = const<f32>(0.0);
// DEFAULT-NEXT:                         let %80 cr: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%78), index1 = read<f32>(%79));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%80), call<f32, signature=fn() -> f32>(%249), const<f32>(0.0));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%81), call<f32, signature=fn() -> f32>(%249), const<f32>(0.0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %283
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %82 a: f32 [storage=automatic] = call<f32, signature=fn() -> f32>(%249);
// DEFAULT-NEXT:                         let %83 b: f32 [storage=automatic] = neg<f32>(const<f32>(0.0));
// DEFAULT-NEXT:                         let %84 cr: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%82), index1 = read<f32>(%83));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%84), call<f32, signature=fn() -> f32>(%249), neg<f32>(const<f32>(0.0)));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%85), call<f32, signature=fn() -> f32>(%249), neg<f32>(const<f32>(0.0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %284
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %86 a: f32 [storage=automatic] = call<f32, signature=fn() -> f32>(%249);
// DEFAULT-NEXT:                         let %87 b: f32 [storage=automatic] = call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%285)));
// DEFAULT-NEXT:                         let %88 cr: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%86), index1 = read<f32>(%87));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%88), call<f32, signature=fn() -> f32>(%249), call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%287))));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%89), call<f32, signature=fn() -> f32>(%249), call<f32, signature=fn(ptr<const i8>) -> f32>(%243, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%288))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %289
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %90 a: f32 [storage=automatic] = call<f32, signature=fn() -> f32>(%249);
// DEFAULT-NEXT:                         let %91 b: f32 [storage=automatic] = call<f32, signature=fn() -> f32>(%249);
// DEFAULT-NEXT:                         let %92 cr: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%90), index1 = read<f32>(%91));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%92), call<f32, signature=fn() -> f32>(%249), call<f32, signature=fn() -> f32>(%249));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%17, read<complex<f32>>(%93), call<f32, signature=fn() -> f32>(%249), call<f32, signature=fn() -> f32>(%249));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %295 @__builtin_nan(%294 <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %301 @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %94 @check_double() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %290
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %291
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %95 a: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:                         let %96 b: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:                         let %97 cr: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64>(%95), index1 = read<f64>(%96));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%97), const<f64>(0.0), const<f64>(0.0));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%98), const<f64>(0.0), const<f64>(0.0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %292
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %99 a: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:                         let %100 b: f64 [storage=automatic] = neg<f64>(const<f64>(0.0));
// DEFAULT-NEXT:                         let %101 cr: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64>(%99), index1 = read<f64>(%100));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%101), const<f64>(0.0), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%102), const<f64>(0.0), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %293
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %103 a: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:                         let %104 b: f64 [storage=automatic] = call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%296)));
// DEFAULT-NEXT:                         let %105 cr: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64>(%103), index1 = read<f64>(%104));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%105), const<f64>(0.0), call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%298))));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%106), const<f64>(0.0), call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%299))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %300
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %107 a: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:                         let %108 b: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(%301);
// DEFAULT-NEXT:                         let %109 cr: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64>(%107), index1 = read<f64>(%108));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%109), const<f64>(0.0), call<f64, signature=fn() -> f64>(%301));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%110), const<f64>(0.0), call<f64, signature=fn() -> f64>(%301));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %302
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %111 a: f64 [storage=automatic] = neg<f64>(const<f64>(0.0));
// DEFAULT-NEXT:                         let %112 b: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:                         let %113 cr: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64>(%111), index1 = read<f64>(%112));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%113), neg<f64>(const<f64>(0.0)), const<f64>(0.0));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%114), neg<f64>(const<f64>(0.0)), const<f64>(0.0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %303
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %115 a: f64 [storage=automatic] = neg<f64>(const<f64>(0.0));
// DEFAULT-NEXT:                         let %116 b: f64 [storage=automatic] = neg<f64>(const<f64>(0.0));
// DEFAULT-NEXT:                         let %117 cr: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64>(%115), index1 = read<f64>(%116));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%117), neg<f64>(const<f64>(0.0)), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%118), neg<f64>(const<f64>(0.0)), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %304
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %119 a: f64 [storage=automatic] = neg<f64>(const<f64>(0.0));
// DEFAULT-NEXT:                         let %120 b: f64 [storage=automatic] = call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%305)));
// DEFAULT-NEXT:                         let %121 cr: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64>(%119), index1 = read<f64>(%120));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%121), neg<f64>(const<f64>(0.0)), call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%307))));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%122), neg<f64>(const<f64>(0.0)), call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%308))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %309
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %123 a: f64 [storage=automatic] = neg<f64>(const<f64>(0.0));
// DEFAULT-NEXT:                         let %124 b: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(%301);
// DEFAULT-NEXT:                         let %125 cr: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64>(%123), index1 = read<f64>(%124));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%125), neg<f64>(const<f64>(0.0)), call<f64, signature=fn() -> f64>(%301));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%126), neg<f64>(const<f64>(0.0)), call<f64, signature=fn() -> f64>(%301));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %310
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %127 a: f64 [storage=automatic] = call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%311)));
// DEFAULT-NEXT:                         let %128 b: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:                         let %129 cr: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64>(%127), index1 = read<f64>(%128));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%129), call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%313))), const<f64>(0.0));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%130), call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%314))), const<f64>(0.0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %315
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %131 a: f64 [storage=automatic] = call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%316)));
// DEFAULT-NEXT:                         let %132 b: f64 [storage=automatic] = neg<f64>(const<f64>(0.0));
// DEFAULT-NEXT:                         let %133 cr: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64>(%131), index1 = read<f64>(%132));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%133), call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%318))), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%134), call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%319))), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %320
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %135 a: f64 [storage=automatic] = call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%321)));
// DEFAULT-NEXT:                         let %136 b: f64 [storage=automatic] = call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%322)));
// DEFAULT-NEXT:                         let %137 cr: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64>(%135), index1 = read<f64>(%136));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%137), call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%325))), call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%326))));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%138), call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%327))), call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%328))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %329
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %139 a: f64 [storage=automatic] = call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%330)));
// DEFAULT-NEXT:                         let %140 b: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(%301);
// DEFAULT-NEXT:                         let %141 cr: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64>(%139), index1 = read<f64>(%140));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%141), call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%332))), call<f64, signature=fn() -> f64>(%301));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%142), call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%333))), call<f64, signature=fn() -> f64>(%301));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %334
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %143 a: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(%301);
// DEFAULT-NEXT:                         let %144 b: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:                         let %145 cr: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64>(%143), index1 = read<f64>(%144));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%145), call<f64, signature=fn() -> f64>(%301), const<f64>(0.0));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%146), call<f64, signature=fn() -> f64>(%301), const<f64>(0.0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %335
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %147 a: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(%301);
// DEFAULT-NEXT:                         let %148 b: f64 [storage=automatic] = neg<f64>(const<f64>(0.0));
// DEFAULT-NEXT:                         let %149 cr: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64>(%147), index1 = read<f64>(%148));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%149), call<f64, signature=fn() -> f64>(%301), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%150), call<f64, signature=fn() -> f64>(%301), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %336
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %151 a: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(%301);
// DEFAULT-NEXT:                         let %152 b: f64 [storage=automatic] = call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%337)));
// DEFAULT-NEXT:                         let %153 cr: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64>(%151), index1 = read<f64>(%152));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%153), call<f64, signature=fn() -> f64>(%301), call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%339))));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%154), call<f64, signature=fn() -> f64>(%301), call<f64, signature=fn(ptr<const i8>) -> f64>(%295, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%340))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %341
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %155 a: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(%301);
// DEFAULT-NEXT:                         let %156 b: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(%301);
// DEFAULT-NEXT:                         let %157 cr: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64>(%155), index1 = read<f64>(%156));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%157), call<f64, signature=fn() -> f64>(%301), call<f64, signature=fn() -> f64>(%301));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%21, read<complex<f64>>(%158), call<f64, signature=fn() -> f64>(%301), call<f64, signature=fn() -> f64>(%301));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %347 @__builtin_nanl(%346 <unnamed>: ptr<const i8>) -> f80 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %353 @__builtin_infl() -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %159 @check_long_double() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %342
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %343
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %160 a: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:                         let %161 b: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:                         let %162 cr: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = read<f80>(%160), index1 = read<f80>(%161));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%162), const<f80>(0), const<f80>(0));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%163), const<f80>(0), const<f80>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %344
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %164 a: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:                         let %165 b: f80 [storage=automatic] = neg<f80>(const<f80>(0));
// DEFAULT-NEXT:                         let %166 cr: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = read<f80>(%164), index1 = read<f80>(%165));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%166), const<f80>(0), neg<f80>(const<f80>(0)));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%167), const<f80>(0), neg<f80>(const<f80>(0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %345
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %168 a: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:                         let %169 b: f80 [storage=automatic] = call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%348)));
// DEFAULT-NEXT:                         let %170 cr: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = read<f80>(%168), index1 = read<f80>(%169));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%170), const<f80>(0), call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%350))));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%171), const<f80>(0), call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%351))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %352
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %172 a: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:                         let %173 b: f80 [storage=automatic] = call<f80, signature=fn() -> f80>(%353);
// DEFAULT-NEXT:                         let %174 cr: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = read<f80>(%172), index1 = read<f80>(%173));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%174), const<f80>(0), call<f80, signature=fn() -> f80>(%353));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%175), const<f80>(0), call<f80, signature=fn() -> f80>(%353));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %354
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %176 a: f80 [storage=automatic] = neg<f80>(const<f80>(0));
// DEFAULT-NEXT:                         let %177 b: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:                         let %178 cr: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = read<f80>(%176), index1 = read<f80>(%177));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%178), neg<f80>(const<f80>(0)), const<f80>(0));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%179), neg<f80>(const<f80>(0)), const<f80>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %355
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %180 a: f80 [storage=automatic] = neg<f80>(const<f80>(0));
// DEFAULT-NEXT:                         let %181 b: f80 [storage=automatic] = neg<f80>(const<f80>(0));
// DEFAULT-NEXT:                         let %182 cr: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = read<f80>(%180), index1 = read<f80>(%181));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%182), neg<f80>(const<f80>(0)), neg<f80>(const<f80>(0)));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%183), neg<f80>(const<f80>(0)), neg<f80>(const<f80>(0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %356
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %184 a: f80 [storage=automatic] = neg<f80>(const<f80>(0));
// DEFAULT-NEXT:                         let %185 b: f80 [storage=automatic] = call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%357)));
// DEFAULT-NEXT:                         let %186 cr: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = read<f80>(%184), index1 = read<f80>(%185));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%186), neg<f80>(const<f80>(0)), call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%359))));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%187), neg<f80>(const<f80>(0)), call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%360))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %361
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %188 a: f80 [storage=automatic] = neg<f80>(const<f80>(0));
// DEFAULT-NEXT:                         let %189 b: f80 [storage=automatic] = call<f80, signature=fn() -> f80>(%353);
// DEFAULT-NEXT:                         let %190 cr: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = read<f80>(%188), index1 = read<f80>(%189));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%190), neg<f80>(const<f80>(0)), call<f80, signature=fn() -> f80>(%353));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%191), neg<f80>(const<f80>(0)), call<f80, signature=fn() -> f80>(%353));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %362
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %192 a: f80 [storage=automatic] = call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%363)));
// DEFAULT-NEXT:                         let %193 b: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:                         let %194 cr: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = read<f80>(%192), index1 = read<f80>(%193));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%194), call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%365))), const<f80>(0));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%195), call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%366))), const<f80>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %367
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %196 a: f80 [storage=automatic] = call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%368)));
// DEFAULT-NEXT:                         let %197 b: f80 [storage=automatic] = neg<f80>(const<f80>(0));
// DEFAULT-NEXT:                         let %198 cr: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = read<f80>(%196), index1 = read<f80>(%197));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%198), call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%370))), neg<f80>(const<f80>(0)));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%199), call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%371))), neg<f80>(const<f80>(0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %372
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %200 a: f80 [storage=automatic] = call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%373)));
// DEFAULT-NEXT:                         let %201 b: f80 [storage=automatic] = call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%374)));
// DEFAULT-NEXT:                         let %202 cr: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = read<f80>(%200), index1 = read<f80>(%201));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%202), call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%377))), call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%378))));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%203), call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%379))), call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%380))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %381
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %204 a: f80 [storage=automatic] = call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%382)));
// DEFAULT-NEXT:                         let %205 b: f80 [storage=automatic] = call<f80, signature=fn() -> f80>(%353);
// DEFAULT-NEXT:                         let %206 cr: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = read<f80>(%204), index1 = read<f80>(%205));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%206), call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%384))), call<f80, signature=fn() -> f80>(%353));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%207), call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%385))), call<f80, signature=fn() -> f80>(%353));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %386
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %208 a: f80 [storage=automatic] = call<f80, signature=fn() -> f80>(%353);
// DEFAULT-NEXT:                         let %209 b: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:                         let %210 cr: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = read<f80>(%208), index1 = read<f80>(%209));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%210), call<f80, signature=fn() -> f80>(%353), const<f80>(0));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%211), call<f80, signature=fn() -> f80>(%353), const<f80>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %387
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %212 a: f80 [storage=automatic] = call<f80, signature=fn() -> f80>(%353);
// DEFAULT-NEXT:                         let %213 b: f80 [storage=automatic] = neg<f80>(const<f80>(0));
// DEFAULT-NEXT:                         let %214 cr: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = read<f80>(%212), index1 = read<f80>(%213));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%214), call<f80, signature=fn() -> f80>(%353), neg<f80>(const<f80>(0)));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%215), call<f80, signature=fn() -> f80>(%353), neg<f80>(const<f80>(0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %388
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %216 a: f80 [storage=automatic] = call<f80, signature=fn() -> f80>(%353);
// DEFAULT-NEXT:                         let %217 b: f80 [storage=automatic] = call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%389)));
// DEFAULT-NEXT:                         let %218 cr: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = read<f80>(%216), index1 = read<f80>(%217));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%218), call<f80, signature=fn() -> f80>(%353), call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%391))));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%219), call<f80, signature=fn() -> f80>(%353), call<f80, signature=fn(ptr<const i8>) -> f80>(%347, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%392))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %393
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %220 a: f80 [storage=automatic] = call<f80, signature=fn() -> f80>(%353);
// DEFAULT-NEXT:                         let %221 b: f80 [storage=automatic] = call<f80, signature=fn() -> f80>(%353);
// DEFAULT-NEXT:                         let %222 cr: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = read<f80>(%220), index1 = read<f80>(%221));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%222), call<f80, signature=fn() -> f80>(%353), call<f80, signature=fn() -> f80>(%353));
// DEFAULT-NEXT:                         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%25, read<complex<f80>>(%223), call<f80, signature=fn() -> f80>(%353), call<f80, signature=fn() -> f80>(%353));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %224 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%29);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%94);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%159);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
