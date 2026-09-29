#include <stdio.h>

static int int_score(int value) { return value + 10; }
static int long_score(long value) { return (int)value + 20; }
static int pointer_score(const int *value) { return *value + 30; }

#define SCORE(value)                                                           \
  _Generic((value),                                                            \
      int: int_score,                                                          \
      long: long_score,                                                        \
      const int *: pointer_score)(value)

int main(void) {
  const int value   = 7;
  int       array[] = {5, 6};
  int       first   = SCORE(value);
  int       second  = SCORE(8L);
  int       third   = SCORE((const int *)array);
  printf("%d %d %d\n", first, second, third);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_int_score:[0-9]+]] @int_score(%[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_value]]), const<i32>(10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_long_score:[0-9]+]] @long_score(%[[VALUE_value_2:[0-9]+]] value: i64) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(truncate<i32, reason=explicit, fits=unknown>(read<i64>(%[[VALUE_value_2]])), const<i32>(20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pointer_score:[0-9]+]] @pointer_score(%[[VALUE_value_3:[0-9]+]] value: ptr<const i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(read<ptr<const i32>>(%[[VALUE_value_3]]))), const<i32>(30));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_value_4:[0-9]+]] value: i32 [storage=automatic] [const] = const<i32>(7);
// DEFAULT-NEXT:         let %[[VALUE_array:[0-9]+]] array: array<i32, 2> [storage=automatic] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(5), index1 = const<i32>(6));
// DEFAULT-NEXT:         let %[[VALUE_first:[0-9]+]] first: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%[[VALUE_int_score]], read<i32>(%[[VALUE_value_4]]));
// DEFAULT-NEXT:         let %[[VALUE_second:[0-9]+]] second: i32 [storage=automatic] = call<i32, signature=fn(i64) -> i32>(%[[VALUE_long_score]], const<i64>(8));
// DEFAULT-NEXT:         let %[[VALUE_third:[0-9]+]] third: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i32>) -> i32>(%[[VALUE_pointer_score]], pointer_cast<ptr<const i32>, reason=explicit>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_array]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])), read<i32>(%[[VALUE_first]]), read<i32>(%[[VALUE_second]]), read<i32>(%[[VALUE_third]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
