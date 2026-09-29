#include <stdio.h>

#define TYPE_NAME(x)                                                           \
  _Generic((x), int: "int", double: "double", default: "other")

int main(void) {
  int a = 3, b = 4;
  int chosen_true  = __builtin_choose_expr(1, a, b);
  int chosen_false = __builtin_choose_expr(0, a, b);

  int same_type = __builtin_types_compatible_p(int, int);
  int diff_type = __builtin_types_compatible_p(int, float);

  int         i      = 7;
  double      d      = 2.5;
  const char *i_name = TYPE_NAME(i);
  const char *d_name = TYPE_NAME(d);

  printf("%d %d %d %d %s %s\n", chosen_true, chosen_false, same_type, diff_type,
         i_name, d_name);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([100, 111, 117, 98, 108, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 115, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = const<i32>(4);
// DEFAULT-NEXT:         let %[[VALUE_chosen_true:[0-9]+]] chosen_true: i32 [storage=automatic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE_chosen_false:[0-9]+]] chosen_false: i32 [storage=automatic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE_same_type:[0-9]+]] same_type: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE_diff_type:[0-9]+]] diff_type: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: f64 [storage=automatic] = const<f64>(2.5);
// DEFAULT-NEXT:         let %[[VALUE_i_name:[0-9]+]] i_name: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]]));
// DEFAULT-NEXT:         let %[[VALUE_d_name:[0-9]+]] d_name: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_2]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str_3]])), read<i32>(%[[VALUE_chosen_true]]), read<i32>(%[[VALUE_chosen_false]]), read<i32>(%[[VALUE_same_type]]), read<i32>(%[[VALUE_diff_type]]), read<ptr<const i8>>(%[[VALUE_i_name]]), read<ptr<const i8>>(%[[VALUE_d_name]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
