#include <complex.h>
#include <stdio.h>

int main(void) {
  volatile double real      = 0.5;
  volatile double imaginary = -0.25;
  double complex  a         = __builtin_complex(real, imaginary);
  double complex  b         = __builtin_complex(real, -imaginary);
  double complex  product;
  double complex  quotient;

  {
#pragma STDC CX_LIMITED_RANGE ON
    product = a * b;
  }

  {
#pragma STDC CX_LIMITED_RANGE OFF
    quotient = a / b;
  }

  printf("%.4f %.4f %.4f %.4f\n", creal(product), cimag(product),
         creal(quotient), cimag(quotient));
  return creal(product) == 0.3125 && cimag(product) == 0.0 &&
                 creal(quotient) == 0.6 && cimag(quotient) == -0.8
             ? 0
             : 1;
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
// DEFAULT-NEXT:     global %16 .str16: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([37, 46, 52, 102, 32, 37, 46, 52, 102, 32, 37, 46, 52, 102, 32, 37, 46, 52, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @cimag(%13 __z: complex<f64>) -> f64 [linkage=external] [memory=none] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %3 @creal(%14 __z: complex<f64>) -> f64 [linkage=external] [memory=none] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %5 @printf(%15 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 real: volatile f64 [storage=automatic] = const<f64>(0.5);
// DEFAULT-NEXT:         let %8 imaginary: volatile f64 [storage=automatic] = neg<f64>(const<f64>(0.25));
// DEFAULT-NEXT:         let %9 a: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64, volatile>(%7), index1 = read<f64, volatile>(%8));
// DEFAULT-NEXT:         let %10 b: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = read<f64, volatile>(%7), index1 = neg<f64>(read<f64, volatile>(%8)));
// DEFAULT-NEXT:         let %11 product: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %12 quotient: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<complex<f64>>(%11, mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%9), read<complex<f64>>(%10)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<complex<f64>>(%12, div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%9), read<complex<f64>>(%10)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%5, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(21)>(%16)), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%3, read<complex<f64>>(%11)), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%1, read<complex<f64>>(%11)), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%3, read<complex<f64>>(%12)), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%1, read<complex<f64>>(%12)));
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(logical_and<bool>(logical_and<bool>(eq<f64, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%3, read<complex<f64>>(%11)), const<f64>(0.3125)), eq<f64, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%1, read<complex<f64>>(%11)), const<f64>(0.0))), eq<f64, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%3, read<complex<f64>>(%12)), const<f64>(0.6))), eq<f64, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%1, read<complex<f64>>(%12)), neg<f64>(const<f64>(0.8)))), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
