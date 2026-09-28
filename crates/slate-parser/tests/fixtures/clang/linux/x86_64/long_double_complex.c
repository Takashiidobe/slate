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
// DEFAULT-NEXT:     global %99 .str99: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 115, 61, 37, 76, 97, 120, 37, 76, 97, 105, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %100 .str100: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 100, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %101 .str101: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([115, 117, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %102 .str102: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([109, 117, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %103 .str103: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([100, 105, 118, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %104 .str104: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([110, 101, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %105 .str105: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([97, 100, 100, 95, 97, 115, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %106 .str106: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([115, 117, 98, 95, 97, 115, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %107 .str107: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([109, 117, 108, 95, 97, 115, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %108 .str108: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([100, 105, 118, 95, 97, 115, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %109 .str109: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([109, 105, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %110 .str110: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([114, 101, 97, 108, 95, 105, 109, 97, 103, 95, 97, 115, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %111 .str111: array<i8, 31> [storage=static] = code_units<array<i8, 31>>([114, 101, 97, 108, 95, 102, 105, 101, 108, 100, 61, 37, 76, 97, 32, 105, 109, 97, 103, 95, 102, 105, 101, 108, 100, 61, 37, 76, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %112 .str112: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([114, 101, 97, 108, 95, 116, 111, 95, 99, 111, 109, 112, 108, 101, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %113 .str113: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([116, 111, 95, 100, 111, 117, 98, 108, 101, 95, 99, 111, 109, 112, 108, 101, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %114 .str114: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([116, 111, 95, 102, 108, 111, 97, 116, 95, 99, 111, 109, 112, 108, 101, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %115 .str115: array<i8, 20> [storage=static] = code_units<array<i8, 20>>([102, 114, 111, 109, 95, 100, 111, 117, 98, 108, 101, 95, 99, 111, 109, 112, 108, 101, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %116 .str116: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([102, 114, 111, 109, 95, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %117 .str117: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([99, 97, 98, 115, 61, 37, 76, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %118 .str118: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([99, 97, 114, 103, 61, 37, 76, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %119 .str119: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 111, 110, 106, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %120 .str120: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 112, 114, 111, 106, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %121 .str121: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 115, 113, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %122 .str122: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 101, 120, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %123 .str123: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 108, 111, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %124 .str124: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 112, 111, 119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %125 .str125: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 115, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %126 .str126: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 99, 111, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %127 .str127: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 116, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %128 .str128: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 97, 115, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %129 .str129: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 97, 99, 111, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %130 .str130: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 97, 116, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %131 .str131: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 115, 105, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %132 .str132: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 99, 111, 115, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %133 .str133: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 116, 97, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %134 .str134: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([99, 97, 115, 105, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %135 .str135: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([99, 97, 99, 111, 115, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %136 .str136: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([99, 97, 116, 97, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %137 .str137: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([99, 114, 101, 97, 108, 61, 37, 76, 97, 32, 99, 105, 109, 97, 103, 61, 37, 76, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @cacosl(%75 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %3 @casinl(%76 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %5 @catanl(%77 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %7 @ccosl(%78 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %9 @csinl(%79 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %11 @ctanl(%80 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %13 @cacoshl(%81 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %15 @casinhl(%82 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %17 @catanhl(%83 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %19 @ccoshl(%84 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %21 @csinhl(%85 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %23 @ctanhl(%86 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %25 @cexpl(%87 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %27 @clogl(%88 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %30 @cpowl(%89 __x: complex<f80>, %90 __y: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %32 @csqrtl(%91 __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %34 @cabsl(%92 __z: complex<f80>) -> f80 [linkage=external] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %36 @cargl(%93 __z: complex<f80>) -> f80 [linkage=external] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %38 @conjl(%94 __z: complex<f80>) -> complex<f80> [linkage=external] [memory=none] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %40 @cprojl(%95 __z: complex<f80>) -> complex<f80> [linkage=external] [memory=none] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %42 @cimagl(%96 __z: complex<f80>) -> f80 [linkage=external] [memory=none] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %44 @creall(%97 __z: complex<f80>) -> f80 [linkage=external] [memory=none] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %46 @printf(%98 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %47 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %48 @print_lc(%49 name: ptr<const i8>, %50 z: complex<f80>) -> void [linkage=internal] [abi=sysv64(scalar, byval<align=16>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%46, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%99)), read<ptr<const i8>>(%49), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%44, read<complex<f80>>(%50)), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%42, read<complex<f80>>(%50)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %51 @mix_complex(%52 a: complex<f80>, %53 b: complex<f80>) -> complex<f80> [linkage=internal] [abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %54 c: complex<f80> [storage=automatic] = div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%52), read<complex<f80>>(%53)), const<f80>(2));
// DEFAULT-NEXT:         return mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%54), const<f80>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %55 @check_arithmetic() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %56 a: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(1), index1 = const<f80>(2));
// DEFAULT-NEXT:         let %57 b: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(3), index1 = neg<f80>(const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%100)), add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%56), read<complex<f80>>(%57)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%101)), sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%56), read<complex<f80>>(%57)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%102)), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%56), read<complex<f80>>(%57)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%103)), div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%56), read<complex<f80>>(%57)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%104)), neg<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%56)));
// DEFAULT-NEXT:         let %58 c: complex<f80> [storage=automatic] = read<complex<f80>>(%56);
// DEFAULT-NEXT:         let %138: complex<f80> [synthetic] = read<complex<f80>>(%58);
// DEFAULT-NEXT:         let %139: complex<f80> [synthetic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%138), read<complex<f80>>(%57));
// DEFAULT-NEXT:         write<complex<f80>>(%58, read<complex<f80>>(%139));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%105)), read<complex<f80>>(%58));
// DEFAULT-NEXT:         let %140: complex<f80> [synthetic] = read<complex<f80>>(%58);
// DEFAULT-NEXT:         let %141: complex<f80> [synthetic] = sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%140), read<complex<f80>>(%57));
// DEFAULT-NEXT:         write<complex<f80>>(%58, read<complex<f80>>(%141));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%106)), read<complex<f80>>(%58));
// DEFAULT-NEXT:         let %142: complex<f80> [synthetic] = read<complex<f80>>(%58);
// DEFAULT-NEXT:         let %143: complex<f80> [synthetic] = mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%142), read<complex<f80>>(%57));
// DEFAULT-NEXT:         write<complex<f80>>(%58, read<complex<f80>>(%143));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%107)), read<complex<f80>>(%58));
// DEFAULT-NEXT:         let %144: complex<f80> [synthetic] = read<complex<f80>>(%58);
// DEFAULT-NEXT:         let %145: complex<f80> [synthetic] = div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%144), read<complex<f80>>(%57));
// DEFAULT-NEXT:         write<complex<f80>>(%58, read<complex<f80>>(%145));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%108)), read<complex<f80>>(%58));
// DEFAULT-NEXT:         if not<bool>(eq<complex<f80>, exceptions=ignore>(read<complex<f80>>(%56), read<complex<f80>>(%56)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%47);
// DEFAULT-NEXT:         if eq<complex<f80>, exceptions=ignore>(read<complex<f80>>(%56), read<complex<f80>>(%57))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%47);
// DEFAULT-NEXT:         if not<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%56), read<complex<f80>>(%57)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%47);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%109)), call<complex<f80>, signature=fn(complex<f80>, complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>>(%51, read<complex<f80>>(%56), read<complex<f80>>(%57)));
// DEFAULT-NEXT:         write<f80>(real(%56), const<f80>(9));
// DEFAULT-NEXT:         write<f80>(imag(%56), const<f80>(8));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%110)), read<complex<f80>>(%56));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%46, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(%111)), read<f80>(real(%56)), read<f80>(imag(%56)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %59 @check_casts() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %60 ld: f80 [storage=automatic] = const<f80>(5);
// DEFAULT-NEXT:         let %61 z: complex<f80> [storage=automatic] = real_to_complex<complex<f80>, reason=assign>(read<f80>(%60));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%112)), read<complex<f80>>(%61));
// DEFAULT-NEXT:         let %62 back: f80 [storage=automatic] = complex_to_real<f80, reason=explicit>(read<complex<f80>>(%61));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(%62), const<f80>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%47);
// DEFAULT-NEXT:         let %63 nonzero_imag: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(3), index1 = const<f80>(4));
// DEFAULT-NEXT:         let %64 real_part: f80 [storage=automatic] = complex_to_real<f80, reason=explicit>(read<complex<f80>>(%63));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(%64), const<f80>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%47);
// DEFAULT-NEXT:         let %65 zd: complex<f64> [storage=automatic] = complex_convert<complex<f64>, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<complex<f80>>(%63));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%113)), complex_convert<complex<f80>, reason=explicit>(read<complex<f64>>(%65)));
// DEFAULT-NEXT:         let %66 zf: complex<f32> [storage=automatic] = complex_convert<complex<f32>, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<complex<f80>>(%63));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%114)), complex_convert<complex<f80>, reason=explicit>(read<complex<f32>>(%66)));
// DEFAULT-NEXT:         let %67 dd: f64 [storage=automatic] = const<f64>(6.0);
// DEFAULT-NEXT:         let %68 from_double: complex<f64> [storage=automatic] = real_to_complex<complex<f64>, reason=assign>(read<f64>(%67));
// DEFAULT-NEXT:         let %69 widened: complex<f80> [storage=automatic] = complex_convert<complex<f80>, reason=explicit>(read<complex<f64>>(%68));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(20)>(%115)), read<complex<f80>>(%69));
// DEFAULT-NEXT:         let %70 i32: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         let %71 fromi: complex<f80> [storage=automatic] = real_to_complex<complex<f80>, reason=assign>(int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%70)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%116)), read<complex<f80>>(%71));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %72 @check_stdlib_functions() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %73 z: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(3), index1 = const<f80>(4));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%46, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%117)), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%34, read<complex<f80>>(%73)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%46, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%118)), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%36, read<complex<f80>>(%73)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%119)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%38, read<complex<f80>>(%73)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%120)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%40, read<complex<f80>>(%73)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%121)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%32, read<complex<f80>>(%73)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%122)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%25, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%123)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%27, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(1), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%124)), call<complex<f80>, signature=fn(complex<f80>, complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>>(%30, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(2), index1 = const<f80>(0)), aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(3), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%125)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%9, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%126)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%7, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%127)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%11, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%128)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%3, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%129)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%1, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(1), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%130)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%5, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%131)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%21, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%132)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%19, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%133)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%23, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%134)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%15, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%135)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%13, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(1), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%136)), call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%17, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%46, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(21)>(%137)), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%44, read<complex<f80>>(%73)), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%42, read<complex<f80>>(%73)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %74 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%55);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%59);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%72);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
