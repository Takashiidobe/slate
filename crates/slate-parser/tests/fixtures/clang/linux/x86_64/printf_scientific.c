#include <stdio.h>

int main(void) {
  double pos   = 1234.5678;
  double neg   = -1234.5678;
  double zero  = 0.0;
  double big   = 1e300;
  double small = 1e-300;
  printf("%e %.2e %10.2e %+e\n", pos, pos, pos, pos);
  printf("%E %.2E\n", pos, pos);
  printf("%-10.2e|\n", pos);
  printf("%e %+e\n", neg, neg);
  printf("%.0e\n", pos);
  printf("%e\n", zero);
  printf("%e %E\n", big, small);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 20> [storage=static] = code_units<array<i8, 20>>([37, 101, 32, 37, 46, 50, 101, 32, 37, 49, 48, 46, 50, 101, 32, 37, 43, 101, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([37, 69, 32, 37, 46, 50, 69, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 45, 49, 48, 46, 50, 101, 124, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([37, 101, 32, 37, 43, 101, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 46, 48, 101, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 101, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 101, 32, 37, 69, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_pos:[0-9]+]] pos: f64 [storage=automatic] = const<f64>(1234.5678);
// DEFAULT-NEXT:         let %[[VALUE_neg:[0-9]+]] neg: f64 [storage=automatic] = neg<f64>(const<f64>(1234.5678));
// DEFAULT-NEXT:         let %[[VALUE_zero:[0-9]+]] zero: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         let %[[VALUE_big:[0-9]+]] big: f64 [storage=automatic] = const<f64>(1e300);
// DEFAULT-NEXT:         let %[[VALUE_small:[0-9]+]] small: f64 [storage=automatic] = const<f64>(1e-300);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(20)>(%[[VALUE_str]])), read<f64>(%[[VALUE_pos]]), read<f64>(%[[VALUE_pos]]), read<f64>(%[[VALUE_pos]]), read<f64>(%[[VALUE_pos]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_2]])), read<f64>(%[[VALUE_pos]]), read<f64>(%[[VALUE_pos]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_3]])), read<f64>(%[[VALUE_pos]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_4]])), read<f64>(%[[VALUE_neg]]), read<f64>(%[[VALUE_neg]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_5]])), read<f64>(%[[VALUE_pos]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_6]])), read<f64>(%[[VALUE_zero]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_7]])), read<f64>(%[[VALUE_big]]), read<f64>(%[[VALUE_small]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
