#include <stdio.h>

static double pick(double *p) {
  *p = *p + 5.0;
  return *p;
}

int main(void) {
  double _Complex z = __builtin_complex(1.0, 2.0);
  __real__ z        = 7.0;
  __imag__ z        = 11.0;
  double r          = pick(&__real__ z);
  double i          = pick(&__imag__ z);
  printf("%d\n", (int)__real__ z);
  printf("%d\n", (int)__imag__ z);
  printf("%d\n", (int)r);
  printf("%d\n", (int)i);
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
// DEFAULT-NEXT:     global %8 .str8: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%7 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @pick(%2 p: ptr<f64>) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<f64>(deref(read<ptr<f64>>(%2)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(read<ptr<f64>>(%2))), const<f64>(5.0)));
// DEFAULT-NEXT:         return read<f64>(deref(read<ptr<f64>>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 z: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(1.0), index1 = const<f64>(2.0));
// DEFAULT-NEXT:         write<f64>(real(%4), const<f64>(7.0));
// DEFAULT-NEXT:         write<f64>(imag(%4), const<f64>(11.0));
// DEFAULT-NEXT:         let %5 r: f64 [storage=automatic] = call<f64, signature=fn(ptr<f64>) -> f64>(%1, addr_of<ptr<f64>>(real(%4)));
// DEFAULT-NEXT:         let %6 i: f64 [storage=automatic] = call<f64, signature=fn(ptr<f64>) -> f64>(%1, addr_of<ptr<f64>>(imag(%4)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%8)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(real(%4))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%9)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(imag(%4))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%10)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(%5)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%11)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(%6)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
