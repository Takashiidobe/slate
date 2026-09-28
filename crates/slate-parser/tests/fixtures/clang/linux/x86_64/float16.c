#include <stdarg.h>
#include <stdio.h>

static _Float16 add16(_Float16 a, _Float16 b) { return a + b; }

static _Float16 mul16(_Float16 a, _Float16 b) { return a * b; }

static _Float16 sum_variadic(int n, ...) {
  va_list ap;
  va_start(ap, n);
  _Float16 total = (_Float16)0;
  for (int i = 0; i < n; i++) {
    total = total + va_arg(ap, _Float16);
  }
  va_end(ap);
  return total;
}

int main(void) {
  _Float16 a = 3.0f16;
  _Float16 b = 4.0f16;
  printf("%d\n", (int)add16(a, b));
  printf("%d\n", (int)mul16(a, b));
  printf("%d\n", (int)sum_variadic(3, (_Float16)1.0f16, (_Float16)2.0f16,
                                   (_Float16)3.0f16));
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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     type @type2 va_list = va_list;
// DEFAULT-NEXT:     global %20 .str20: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @printf(%18 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @add16(%5 a: f16, %6 b: f16) -> f16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<f16, rounding=nearest_even, exceptions=ignore, contract=on>(read<f16>(%5), read<f16>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @mul16(%8 a: f16, %9 b: f16) -> f16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f16, rounding=nearest_even, exceptions=ignore, contract=on>(read<f16>(%8), read<f16>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @sum_variadic(%11 n: i32, ...) -> f16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%12);
// DEFAULT-NEXT:         let %13 total: f16 [storage=automatic] = int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         for %19
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %14 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%14), read<i32>(%11))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %23: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%24));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f16>(%13, add<f16, rounding=nearest_even, exceptions=ignore, contract=on>(read<f16>(%13), va_arg<f16>(%12)));
// DEFAULT-NEXT:                     add<f16, rounding=nearest_even, exceptions=ignore, contract=on>(read<f16>(%13), va_arg<f16>(%12));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%12);
// DEFAULT-NEXT:         return read<f16>(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %16 a: f16 [storage=automatic] = const<f16>(3);
// DEFAULT-NEXT:         let %17 b: f16 [storage=automatic] = const<f16>(4);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%20)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f16, signature=fn(f16, f16) -> f16>(%4, read<f16>(%16), read<f16>(%17))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%21)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f16, signature=fn(f16, f16) -> f16>(%7, read<f16>(%16), read<f16>(%17))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%22)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f16, signature=fn(i32, ...) -> f16>(%10, const<i32>(3), const<f16>(1), const<f16>(2), const<f16>(3))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
