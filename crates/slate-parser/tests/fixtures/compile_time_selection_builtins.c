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
// DEFAULT-NEXT:     global %13 .str13: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([100, 111, 117, 98, 108, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 115, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%12 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 a: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %3 b: i32 [storage=automatic] = const<i32>(4);
// DEFAULT-NEXT:         let %4 chosen_true: i32 [storage=automatic] = read<i32>(%2);
// DEFAULT-NEXT:         let %5 chosen_false: i32 [storage=automatic] = read<i32>(%3);
// DEFAULT-NEXT:         let %6 same_type: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %7 diff_type: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         let %9 d: f64 [storage=automatic] = const<f64>(2.5);
// DEFAULT-NEXT:         let %10 i_name: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%13));
// DEFAULT-NEXT:         let %11 d_name: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%14));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%15)), read<i32>(%4), read<i32>(%5), read<i32>(%6), read<i32>(%7), read<ptr<const i8>>(%10), read<ptr<const i8>>(%11));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
