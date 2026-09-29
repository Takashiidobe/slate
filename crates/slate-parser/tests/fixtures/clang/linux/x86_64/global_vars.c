#include <stdio.h>

int counter = 4;
int zeroed;
int numbers[4] = {1, 2};

struct Pair {
  int left;
  int right;
};

struct Pair pair = {3, 5};

static int adjust(int by) {
  counter    += by;
  zeroed     += counter;
  numbers[2]  = zeroed - numbers[0];
  pair.right += numbers[1];
  return pair.left + pair.right;
}

int main(void) {
  printf("%d\n", adjust(6));
  printf("%d %d %d\n", counter, zeroed, numbers[2]);
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
// DEFAULT-NEXT:     type @type[[TYPE_Pair:[0-9]+]] Pair = struct {
// DEFAULT-NEXT:         field0 left: i32;
// DEFAULT-NEXT:         field1 right: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_counter:[0-9]+]] counter: i32 [storage=static] = const<i32>(4) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_zeroed:[0-9]+]] zeroed: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_numbers:[0-9]+]] numbers: array<i32, 4> [storage=static] [align=16] = aggregate<array<i32, 4>, zero_fill=true>(index0 = const<i32>(1), index1 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pair:[0-9]+]] pair: @type[[TYPE_Pair]] [storage=static] = aggregate<@type[[TYPE_Pair]], zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(5)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_adjust:[0-9]+]] @adjust(%[[VALUE_by:[0-9]+]] by: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_counter]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), read<i32>(%[[VALUE_by]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_counter]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_zeroed]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), read<i32>(%[[VALUE_counter]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_zeroed]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_numbers]]), const<i32>(2))), sub<i32, overflow=ub>(read<i32>(%[[VALUE_zeroed]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_numbers]]), const<i32>(0))))));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(field1(%[[VALUE_pair]]));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_numbers]]), const<i32>(1)))));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_pair]]), read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(field0(%[[VALUE_pair]])), read<i32>(field1(%[[VALUE_pair]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_adjust]], const<i32>(6)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_2]])), read<i32>(%[[VALUE_counter]]), read<i32>(%[[VALUE_zeroed]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_numbers]]), const<i32>(2)))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
