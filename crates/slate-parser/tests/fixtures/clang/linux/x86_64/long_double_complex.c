#include <complex.h>
#include <stdio.h>

void abort(void);

static void print_lc(const char *name, long double complex z) {
  printf("%s=%Lax%Lai\n", name, creall(z), cimagl(z));
}

static long double complex mix_complex(long double complex a,
                                       long double complex b) {
  long double complex c = (a + b) / 2.0L;
  return c * 3.0L;
}

static void check_arithmetic(void) {
  long double complex a = CMPLXL(1.0L, 2.0L);
  long double complex b = CMPLXL(3.0L, -1.0L);

  print_lc("add", a + b);
  print_lc("sub", a - b);
  print_lc("mul", a * b);
  print_lc("div", a / b);
  print_lc("neg", -a);

  long double complex c  = a;
  c                     += b;
  print_lc("add_assign", c);
  c -= b;
  print_lc("sub_assign", c);
  c *= b;
  print_lc("mul_assign", c);
  c /= b;
  print_lc("div_assign", c);

  if (!(a == a))
    abort();
  if (a == b)
    abort();
  if (!(a != b))
    abort();

  print_lc("mix", mix_complex(a, b));

  __real__ a = 9.0L;
  __imag__ a = 8.0L;
  print_lc("real_imag_assign", a);
  printf("real_field=%La imag_field=%La\n", __real__ a, __imag__ a);
}

static void check_casts(void) {
  long double         ld = 5.0L;
  long double complex z  = ld;
  print_lc("real_to_complex", z);

  long double back = (long double)z;
  if (back != 5.0L)
    abort();

  long double complex nonzero_imag = CMPLXL(3.0L, 4.0L);
  long double         real_part    = (long double)nonzero_imag;
  if (real_part != 3.0L)
    abort();

  double complex zd = (double complex)nonzero_imag;
  print_lc("to_double_complex", (long double complex)zd);

  float complex zf = (float complex)nonzero_imag;
  print_lc("to_float_complex", (long double complex)zf);

  double              dd          = 6.0;
  double complex      from_double = dd;
  long double complex widened     = (long double complex)from_double;
  print_lc("from_double_complex", widened);

  int                 i32   = 7;
  long double complex fromi = i32;
  print_lc("from_int", fromi);
}

static void check_stdlib_functions(void) {
  long double complex z = CMPLXL(3.0L, 4.0L);

  printf("cabs=%La\n", cabsl(z));
  printf("carg=%La\n", cargl(z));
  print_lc("conj", conjl(z));
  print_lc("cproj", cprojl(z));
  print_lc("csqrt", csqrtl(z));
  print_lc("cexp", cexpl(CMPLXL(0.0L, 0.0L)));
  print_lc("clog", clogl(CMPLXL(1.0L, 0.0L)));
  print_lc("cpow", cpowl(CMPLXL(2.0L, 0.0L), CMPLXL(3.0L, 0.0L)));

  print_lc("csin", csinl(CMPLXL(0.0L, 0.0L)));
  print_lc("ccos", ccosl(CMPLXL(0.0L, 0.0L)));
  print_lc("ctan", ctanl(CMPLXL(0.0L, 0.0L)));
  print_lc("casin", casinl(CMPLXL(0.0L, 0.0L)));
  print_lc("cacos", cacosl(CMPLXL(1.0L, 0.0L)));
  print_lc("catan", catanl(CMPLXL(0.0L, 0.0L)));

  print_lc("csinh", csinhl(CMPLXL(0.0L, 0.0L)));
  print_lc("ccosh", ccoshl(CMPLXL(0.0L, 0.0L)));
  print_lc("ctanh", ctanhl(CMPLXL(0.0L, 0.0L)));
  print_lc("casinh", casinhl(CMPLXL(0.0L, 0.0L)));
  print_lc("cacosh", cacoshl(CMPLXL(1.0L, 0.0L)));
  print_lc("catanh", catanhl(CMPLXL(0.0L, 0.0L)));

  printf("creal=%La cimag=%La\n", creall(z), cimagl(z));
}

int main(void) {
  check_arithmetic();
  check_casts();
  check_stdlib_functions();
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 115, 61, 37, 76, 97, 120, 37, 76, 97, 105, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 100, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([115, 117, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([109, 117, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([100, 105, 118, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([110, 101, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([97, 100, 100, 95, 97, 115, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([115, 117, 98, 95, 97, 115, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([109, 117, 108, 95, 97, 115, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([100, 105, 118, 95, 97, 115, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([109, 105, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([114, 101, 97, 108, 95, 105, 109, 97, 103, 95, 97, 115, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 31> [storage=static] = code_units<array<i8, 31>>([114, 101, 97, 108, 95, 102, 105, 101, 108, 100, 61, 37, 76, 97, 32, 105, 109, 97, 103, 95, 102, 105, 101, 108, 100, 61, 37, 76, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([114, 101, 97, 108, 95, 116, 111, 95, 99, 111, 109, 112, 108, 101, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([116, 111, 95, 100, 111, 117, 98, 108, 101, 95, 99, 111, 109, 112, 108, 101, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([116, 111, 95, 102, 108, 111, 97, 116, 95, 99, 111, 109, 112, 108, 101, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 20> [storage=static] = code_units<array<i8, 20>>([102, 114, 111, 109, 95, 100, 111, 117, 98, 108, 101, 95, 99, 111, 109, 112, 108, 101, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([102, 114, 111, 109, 95, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([99, 97, 98, 115, 61, 37, 76, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([99, 97, 114, 103, 61, 37, 76, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 111, 110, 106, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 112, 114, 111, 106, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 115, 113, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 101, 120, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 108, 111, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 112, 111, 119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 115, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_28:[0-9]+]] .str[[VALUE_str_28]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 99, 111, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_29:[0-9]+]] .str[[VALUE_str_29]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 116, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_30:[0-9]+]] .str[[VALUE_str_30]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 97, 115, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_31:[0-9]+]] .str[[VALUE_str_31]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 97, 99, 111, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_32:[0-9]+]] .str[[VALUE_str_32]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 97, 116, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_33:[0-9]+]] .str[[VALUE_str_33]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 115, 105, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_34:[0-9]+]] .str[[VALUE_str_34]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 99, 111, 115, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_35:[0-9]+]] .str[[VALUE_str_35]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 116, 97, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_36:[0-9]+]] .str[[VALUE_str_36]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([99, 97, 115, 105, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_37:[0-9]+]] .str[[VALUE_str_37]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([99, 97, 99, 111, 115, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_38:[0-9]+]] .str[[VALUE_str_38]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([99, 97, 116, 97, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_39:[0-9]+]] .str[[VALUE_str_39]]: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([99, 114, 101, 97, 108, 61, 37, 76, 97, 32, 99, 105, 109, 97, 103, 61, 37, 76, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_cacosl:[0-9]+]] @cacosl(%[[VALUE___z:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_casinl:[0-9]+]] @casinl(%[[VALUE___z_2:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_catanl:[0-9]+]] @catanl(%[[VALUE___z_3:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_ccosl:[0-9]+]] @ccosl(%[[VALUE___z_4:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_csinl:[0-9]+]] @csinl(%[[VALUE___z_5:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_ctanl:[0-9]+]] @ctanl(%[[VALUE___z_6:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_cacoshl:[0-9]+]] @cacoshl(%[[VALUE___z_7:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_casinhl:[0-9]+]] @casinhl(%[[VALUE___z_8:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_catanhl:[0-9]+]] @catanhl(%[[VALUE___z_9:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_ccoshl:[0-9]+]] @ccoshl(%[[VALUE___z_10:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_csinhl:[0-9]+]] @csinhl(%[[VALUE___z_11:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_ctanhl:[0-9]+]] @ctanhl(%[[VALUE___z_12:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_cexpl:[0-9]+]] @cexpl(%[[VALUE___z_13:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_clogl:[0-9]+]] @clogl(%[[VALUE___z_14:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_cpowl:[0-9]+]] @cpowl(%[[VALUE___x:[0-9]+]] __x: complex<f80>, %[[VALUE___y:[0-9]+]] __y: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_csqrtl:[0-9]+]] @csqrtl(%[[VALUE___z_15:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_cabsl:[0-9]+]] @cabsl(%[[VALUE___z_16:[0-9]+]] __z: complex<f80>) -> f80 [linkage=external] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_cargl:[0-9]+]] @cargl(%[[VALUE___z_17:[0-9]+]] __z: complex<f80>) -> f80 [linkage=external] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_conjl:[0-9]+]] @conjl(%[[VALUE___z_18:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [memory=none] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_cprojl:[0-9]+]] @cprojl(%[[VALUE___z_19:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [memory=none] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE_cimagl:[0-9]+]] @cimagl(%[[VALUE___z_20:[0-9]+]] __z: complex<f80>) -> f80 [linkage=external] [memory=none] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_creall:[0-9]+]] @creall(%[[VALUE___z_21:[0-9]+]] __z: complex<f80>) -> f80 [linkage=external] [memory=none] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_print_lc:[0-9]+]] @print_lc(%[[VALUE_name:[0-9]+]] name: ptr<const i8>, %[[VALUE_z:[0-9]+]] z: complex<f80>) -> void [linkage=internal] [abi=sysv64(scalar, byval<align=16>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str]])), read<ptr<const i8>>(%[[VALUE_name]]), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_creall]], read<complex<f80>>(%[[VALUE_z]])), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cimagl]], read<complex<f80>>(%[[VALUE_z]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_mix_complex:[0-9]+]] @mix_complex(%[[VALUE_a:[0-9]+]] a: complex<f80>, %[[VALUE_b:[0-9]+]] b: complex<f80>) -> complex<f80> [linkage=internal] [abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: complex<f80> [storage=automatic] = div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_a]]), read<complex<f80>>(%[[VALUE_b]])), const<f80>(2));
// DEFAULT-NEXT:         return mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_c]]), const<f80>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check_arithmetic:[0-9]+]] @check_arithmetic() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(1), index1 = const<f80>(2));
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(3), index1 = neg<f80>(const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_a_2]]), read<complex<f80>>(%[[VALUE_b_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]])), sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_a_2]]), read<complex<f80>>(%[[VALUE_b_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_4]])), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_a_2]]), read<complex<f80>>(%[[VALUE_b_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_5]])), div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_a_2]]), read<complex<f80>>(%[[VALUE_b_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_6]])), neg<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_a_2]])));
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: complex<f80> [storage=automatic] = read<complex<f80>>(%[[VALUE_a_2]]);
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_c_2]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: complex<f80> [synthetic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE0]]), read<complex<f80>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c_2]], read<complex<f80>>(%[[VALUE1]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_7]])), read<complex<f80>>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_c_2]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: complex<f80> [synthetic] = sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE2]]), read<complex<f80>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c_2]], read<complex<f80>>(%[[VALUE3]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_8]])), read<complex<f80>>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_c_2]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: complex<f80> [synthetic] = mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE4]]), read<complex<f80>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c_2]], read<complex<f80>>(%[[VALUE5]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_9]])), read<complex<f80>>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_c_2]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: complex<f80> [synthetic] = div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE6]]), read<complex<f80>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c_2]], read<complex<f80>>(%[[VALUE7]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_10]])), read<complex<f80>>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:         if not<bool>(eq<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_a_2]]), read<complex<f80>>(%[[VALUE_a_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_a_2]]), read<complex<f80>>(%[[VALUE_b_2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_a_2]]), read<complex<f80>>(%[[VALUE_b_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_11]])), call<complex<f80>, signature=fn(complex<f80>, complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_mix_complex]], read<complex<f80>>(%[[VALUE_a_2]]), read<complex<f80>>(%[[VALUE_b_2]])));
// DEFAULT-NEXT:         write<f80>(real(%[[VALUE_a_2]]), const<f80>(9));
// DEFAULT-NEXT:         write<f80>(imag(%[[VALUE_a_2]]), const<f80>(8));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str_12]])), read<complex<f80>>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(%[[VALUE_str_13]])), read<f80>(real(%[[VALUE_a_2]])), read<f80>(imag(%[[VALUE_a_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check_casts:[0-9]+]] @check_casts() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ld:[0-9]+]] ld: f80 [storage=automatic] = const<f80>(5);
// DEFAULT-NEXT:         let %[[VALUE_z_2:[0-9]+]] z: complex<f80> [storage=automatic] = real_to_complex<complex<f80>, reason=assign>(read<f80>(%[[VALUE_ld]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_14]])), read<complex<f80>>(%[[VALUE_z_2]]));
// DEFAULT-NEXT:         let %[[VALUE_back:[0-9]+]] back: f80 [storage=automatic] = complex_to_real<f80, reason=explicit>(read<complex<f80>>(%[[VALUE_z_2]]));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE_back]]), const<f80>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_nonzero_imag:[0-9]+]] nonzero_imag: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(3), index1 = const<f80>(4));
// DEFAULT-NEXT:         let %[[VALUE_real_part:[0-9]+]] real_part: f80 [storage=automatic] = complex_to_real<f80, reason=explicit>(read<complex<f80>>(%[[VALUE_nonzero_imag]]));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE_real_part]]), const<f80>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_zd:[0-9]+]] zd: complex<f64> [storage=automatic] = complex_convert<complex<f64>, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<complex<f80>>(%[[VALUE_nonzero_imag]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_15]])), complex_convert<complex<f80>, reason=explicit>(read<complex<f64>>(%[[VALUE_zd]])));
// DEFAULT-NEXT:         let %[[VALUE_zf:[0-9]+]] zf: complex<f32> [storage=automatic] = complex_convert<complex<f32>, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<complex<f80>>(%[[VALUE_nonzero_imag]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str_16]])), complex_convert<complex<f80>, reason=explicit>(read<complex<f32>>(%[[VALUE_zf]])));
// DEFAULT-NEXT:         let %[[VALUE_dd:[0-9]+]] dd: f64 [storage=automatic] = const<f64>(6.0);
// DEFAULT-NEXT:         let %[[VALUE_from_double:[0-9]+]] from_double: complex<f64> [storage=automatic] = real_to_complex<complex<f64>, reason=assign>(read<f64>(%[[VALUE_dd]]));
// DEFAULT-NEXT:         let %[[VALUE_widened:[0-9]+]] widened: complex<f80> [storage=automatic] = complex_convert<complex<f80>, reason=explicit>(read<complex<f64>>(%[[VALUE_from_double]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(20)>(%[[VALUE_str_17]])), read<complex<f80>>(%[[VALUE_widened]]));
// DEFAULT-NEXT:         let %[[VALUE_i32:[0-9]+]] i32: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         let %[[VALUE_fromi:[0-9]+]] fromi: complex<f80> [storage=automatic] = real_to_complex<complex<f80>, reason=assign>(int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE_i32]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_18]])), read<complex<f80>>(%[[VALUE_fromi]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check_stdlib_functions:[0-9]+]] @check_stdlib_functions() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_z_3:[0-9]+]] z: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(3), index1 = const<f80>(4));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_19]])), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cabsl]], read<complex<f80>>(%[[VALUE_z_3]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_20]])), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cargl]], read<complex<f80>>(%[[VALUE_z_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_21]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_conjl]], read<complex<f80>>(%[[VALUE_z_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_22]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_cprojl]], read<complex<f80>>(%[[VALUE_z_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_23]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_csqrtl]], read<complex<f80>>(%[[VALUE_z_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_24]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_cexpl]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_25]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_clogl]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(1), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_26]])), call<complex<f80>, signature=fn(complex<f80>, complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_cpowl]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(2), index1 = const<f80>(0)), aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(3), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_27]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_csinl]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_28]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_ccosl]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_29]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_ctanl]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_30]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_casinl]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_31]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_cacosl]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(1), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_32]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_catanl]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_33]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_csinhl]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_34]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_ccoshl]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_35]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_ctanhl]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_36]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_casinhl]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_37]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_cacoshl]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(1), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%[[VALUE_print_lc]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_38]])), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_catanhl]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(21)>(%[[VALUE_str_39]])), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_creall]], read<complex<f80>>(%[[VALUE_z_3]])), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cimagl]], read<complex<f80>>(%[[VALUE_z_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_check_arithmetic]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_check_casts]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_check_stdlib_functions]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
