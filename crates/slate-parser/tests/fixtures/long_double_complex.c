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
// DEFAULT-NEXT:     global %75 .str75: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 115, 61, 37, 76, 97, 120, 37, 76, 97, 105, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %76 .str76: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 100, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %77 .str77: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([115, 117, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %78 .str78: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([109, 117, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %79 .str79: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([100, 105, 118, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %80 .str80: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([110, 101, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %81 .str81: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([97, 100, 100, 95, 97, 115, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %82 .str82: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([115, 117, 98, 95, 97, 115, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %83 .str83: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([109, 117, 108, 95, 97, 115, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %84 .str84: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([100, 105, 118, 95, 97, 115, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %85 .str85: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([109, 105, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %86 .str86: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([114, 101, 97, 108, 95, 105, 109, 97, 103, 95, 97, 115, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %87 .str87: array<i8, 31> [storage=static] = code_units<array<i8, 31>>([114, 101, 97, 108, 95, 102, 105, 101, 108, 100, 61, 37, 76, 97, 32, 105, 109, 97, 103, 95, 102, 105, 101, 108, 100, 61, 37, 76, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %88 .str88: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([114, 101, 97, 108, 95, 116, 111, 95, 99, 111, 109, 112, 108, 101, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %89 .str89: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([116, 111, 95, 100, 111, 117, 98, 108, 101, 95, 99, 111, 109, 112, 108, 101, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %90 .str90: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([116, 111, 95, 102, 108, 111, 97, 116, 95, 99, 111, 109, 112, 108, 101, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %91 .str91: array<i8, 20> [storage=static] = code_units<array<i8, 20>>([102, 114, 111, 109, 95, 100, 111, 117, 98, 108, 101, 95, 99, 111, 109, 112, 108, 101, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %92 .str92: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([102, 114, 111, 109, 95, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %93 .str93: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([99, 97, 98, 115, 61, 37, 76, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %94 .str94: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([99, 97, 114, 103, 61, 37, 76, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %95 .str95: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 111, 110, 106, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %96 .str96: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 112, 114, 111, 106, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %97 .str97: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 115, 113, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %98 .str98: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 101, 120, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %99 .str99: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 108, 111, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %100 .str100: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 112, 111, 119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %101 .str101: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 115, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %102 .str102: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 99, 111, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %103 .str103: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 116, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %104 .str104: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 97, 115, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %105 .str105: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 97, 99, 111, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %106 .str106: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 97, 116, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %107 .str107: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 115, 105, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %108 .str108: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 99, 111, 115, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %109 .str109: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 116, 97, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %110 .str110: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([99, 97, 115, 105, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %111 .str111: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([99, 97, 99, 111, 115, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %112 .str112: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([99, 97, 116, 97, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %113 .str113: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([99, 114, 101, 97, 108, 61, 37, 76, 97, 32, 99, 105, 109, 97, 103, 61, 37, 76, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @cacosl(%51 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %1 @casinl(%52 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %2 @catanl(%53 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %3 @ccosl(%54 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %4 @csinl(%55 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %5 @ctanl(%56 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %6 @cacoshl(%57 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %7 @casinhl(%58 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %8 @catanhl(%59 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %9 @ccoshl(%60 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %10 @csinhl(%61 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %11 @ctanhl(%62 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %12 @cexpl(%63 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %13 @clogl(%64 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %14 @cpowl(%65 __x: complex<f80>, %66 __y: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %15 @csqrtl(%67 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %16 @cabsl(%68 __z: complex<f80>) -> f80 [linkage=external] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %17 @cargl(%69 __z: complex<f80>) -> f80 [linkage=external] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %18 @conjl(%70 __z: complex<f80>) -> complex<f80> [linkage=external] [memory=none] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %19 @cprojl(%71 __z: complex<f80>) -> complex<f80> [linkage=external] [memory=none] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %20 @cimagl(%72 __z: complex<f80>) -> f80 [linkage=external] [memory=none] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %21 @creall(%73 __z: complex<f80>) -> f80 [linkage=external] [memory=none] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %22 @printf(%74 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %23 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %24 @print_lc(%25 name: ptr<const i8>, %26 z: complex<f80>) -> void [linkage=internal] [abi=sysv64(scalar, byval<align=16>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%75)), read<ptr<const i8>>(%25), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%26)), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%20, read<complex<f80>>(%26)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @mix_complex(%28 a: complex<f80>, %29 b: complex<f80>) -> complex<f80> [linkage=internal] [abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %30 c: complex<f80> [storage=automatic] = div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%28), read<complex<f80>>(%29)), const<f80>(2));
// DEFAULT-NEXT:         return mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%30), const<f80>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @check_arithmetic() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %32 a: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(1), index1 = const<f80>(2));
// DEFAULT-NEXT:         let %33 b: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(3), index1 = neg<f80>(const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%76)), add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%32), read<complex<f80>>(%33)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%77)), sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%32), read<complex<f80>>(%33)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%78)), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%32), read<complex<f80>>(%33)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%79)), div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%32), read<complex<f80>>(%33)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%80)), neg<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%32)));
// DEFAULT-NEXT:         let %34 c: complex<f80> [storage=automatic] = read<complex<f80>>(%32);
// DEFAULT-NEXT:         let %114: complex<f80> [synthetic] = read<complex<f80>>(%34);
// DEFAULT-NEXT:         let %115: complex<f80> [synthetic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%114), read<complex<f80>>(%33));
// DEFAULT-NEXT:         write<complex<f80>>(%34, read<complex<f80>>(%115));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%81)), read<complex<f80>>(%34));
// DEFAULT-NEXT:         let %116: complex<f80> [synthetic] = read<complex<f80>>(%34);
// DEFAULT-NEXT:         let %117: complex<f80> [synthetic] = sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%116), read<complex<f80>>(%33));
// DEFAULT-NEXT:         write<complex<f80>>(%34, read<complex<f80>>(%117));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%82)), read<complex<f80>>(%34));
// DEFAULT-NEXT:         let %118: complex<f80> [synthetic] = read<complex<f80>>(%34);
// DEFAULT-NEXT:         let %119: complex<f80> [synthetic] = mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%118), read<complex<f80>>(%33));
// DEFAULT-NEXT:         write<complex<f80>>(%34, read<complex<f80>>(%119));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%83)), read<complex<f80>>(%34));
// DEFAULT-NEXT:         let %120: complex<f80> [synthetic] = read<complex<f80>>(%34);
// DEFAULT-NEXT:         let %121: complex<f80> [synthetic] = div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%120), read<complex<f80>>(%33));
// DEFAULT-NEXT:         write<complex<f80>>(%34, read<complex<f80>>(%121));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%84)), read<complex<f80>>(%34));
// DEFAULT-NEXT:         if not<bool>(eq<complex<f80>, exceptions=ignore>(read<complex<f80>>(%32), read<complex<f80>>(%32)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:         if eq<complex<f80>, exceptions=ignore>(read<complex<f80>>(%32), read<complex<f80>>(%33))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:         if not<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%32), read<complex<f80>>(%33)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%85)), call<complex<f80>, signature=fn(complex<f80>, complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>>(%27, read<complex<f80>>(%32), read<complex<f80>>(%33)));
// DEFAULT-NEXT:         write<f80>(real(%32), const<f80>(9));
// DEFAULT-NEXT:         write<f80>(imag(%32), const<f80>(8));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%86)), read<complex<f80>>(%32));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(%87)), read<f80>(real(%32)), read<f80>(imag(%32)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @check_casts() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %36 ld: f80 [storage=automatic] = const<f80>(5);
// DEFAULT-NEXT:         let %37 z: complex<f80> [storage=automatic] = real_to_complex<complex<f80>, reason=assign>(read<f80>(%36));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%88)), read<complex<f80>>(%37));
// DEFAULT-NEXT:         let %38 back: f80 [storage=automatic] = complex_to_real<f80, reason=explicit>(read<complex<f80>>(%37));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(%38), const<f80>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:         let %39 nonzero_imag: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(3), index1 = const<f80>(4));
// DEFAULT-NEXT:         let %40 real_part: f80 [storage=automatic] = complex_to_real<f80, reason=explicit>(read<complex<f80>>(%39));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(%40), const<f80>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:         let %41 zd: complex<f64> [storage=automatic] = complex_convert<complex<f64>, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<complex<f80>>(%39));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%89)), complex_convert<complex<f80>, reason=explicit>(read<complex<f64>>(%41)));
// DEFAULT-NEXT:         let %42 zf: complex<f32> [storage=automatic] = complex_convert<complex<f32>, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<complex<f80>>(%39));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%90)), complex_convert<complex<f80>, reason=explicit>(read<complex<f32>>(%42)));
// DEFAULT-NEXT:         let %43 dd: f64 [storage=automatic] = const<f64>(6.0);
// DEFAULT-NEXT:         let %44 from_double: complex<f64> [storage=automatic] = real_to_complex<complex<f64>, reason=assign>(read<f64>(%43));
// DEFAULT-NEXT:         let %45 widened: complex<f80> [storage=automatic] = complex_convert<complex<f80>, reason=explicit>(read<complex<f64>>(%44));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(20)>(%91)), read<complex<f80>>(%45));
// DEFAULT-NEXT:         let %46 i32: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         let %47 fromi: complex<f80> [storage=automatic] = real_to_complex<complex<f80>, reason=assign>(int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%46)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%92)), read<complex<f80>>(%47));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @check_stdlib_functions() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %49 z: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(3), index1 = const<f80>(4));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%93)), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%16, read<complex<f80>>(%49)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%94)), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%17, read<complex<f80>>(%49)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%95)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%18, read<complex<f80>>(%49)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%96)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%19, read<complex<f80>>(%49)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%97)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%15, read<complex<f80>>(%49)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%98)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%12, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%99)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%13, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(1), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%100)), call<complex<f80>, signature=fn(complex<f80>, complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>>(%14, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(2), index1 = const<f80>(0)), aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(3), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%101)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%4, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%102)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%3, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%103)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%5, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%104)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%1, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%105)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%0, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(1), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%106)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%2, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%107)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%10, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%108)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%9, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%109)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%11, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%110)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%7, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%111)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%6, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(1), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%112)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%8, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(21)>(%113)), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%49)), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%20, read<complex<f80>>(%49)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%31);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%35);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%48);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
