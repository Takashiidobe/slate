#include <stdio.h>

int main(void) {
  double _Complex a    = __builtin_complex(1.0, 2.0);
  double _Complex b    = __builtin_complex(3.0, 4.0);
  double _Complex sum  = a + b;
  double _Complex diff = a - b;
  double _Complex prod = a * b;
  double _Complex quot = a / b;
  printf("%d\n", (int)__real__ sum);
  printf("%d\n", (int)__imag__ sum);
  printf("%d\n", (int)__real__ diff);
  printf("%d\n", (int)__imag__ diff);
  printf("%d\n", (int)__real__ prod);
  printf("%d\n", (int)__imag__ prod);
  printf("%d\n", (int)(100.0 * __real__ quot));
  printf("%d\n", (int)(100.0 * __imag__ quot));
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
// DEFAULT-NEXT:     global %10 .str10: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %12 .str12: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%9 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 a: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(1.0), index1 = const<f64>(2.0));
// DEFAULT-NEXT:         let %4 b: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(3.0), index1 = const<f64>(4.0));
// DEFAULT-NEXT:         let %5 sum: complex<f64> [storage=automatic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%3), read<complex<f64>>(%4));
// DEFAULT-NEXT:         let %6 diff: complex<f64> [storage=automatic] = sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%3), read<complex<f64>>(%4));
// DEFAULT-NEXT:         let %7 prod: complex<f64> [storage=automatic] = mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%3), read<complex<f64>>(%4));
// DEFAULT-NEXT:         let %8 quot: complex<f64> [storage=automatic] = div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%3), read<complex<f64>>(%4));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%10)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(real(%5))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%11)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(imag(%5))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%12)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(real(%6))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%13)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(imag(%6))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%14)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(real(%7))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%15)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(imag(%7))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%16)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(100.0), read<f64>(real(%8)))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%17)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(100.0), read<f64>(imag(%8)))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
