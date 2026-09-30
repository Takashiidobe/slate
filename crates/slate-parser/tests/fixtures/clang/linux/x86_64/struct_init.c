#include <stdio.h>

struct Triple {
  int x;
  int y;
  int z;
};

int main(void) {
  struct Triple full       = {1, 2, 3};
  struct Triple partial    = {4, 5};
  struct Triple designated = {.z = 9, .x = 7};
  printf("%d %d %d\n", full.x, full.y, full.z);
  printf("%d %d %d\n", partial.x, partial.y, partial.z);
  printf("%d %d %d\n", designated.x, designated.y, designated.z);

  struct Triple copy = full;
  copy.x             = 42;
  printf("%d %d\n", full.x, copy.x);
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
// DEFAULT-NEXT:     type @type[[TYPE_Triple:[0-9]+]] Triple = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:         field2 z: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_full:[0-9]+]] full: @type[[TYPE_Triple]] [storage=automatic] = aggregate<@type[[TYPE_Triple]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2), field2 = const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE_partial:[0-9]+]] partial: @type[[TYPE_Triple]] [storage=automatic] = aggregate<@type[[TYPE_Triple]], zero_fill=true>(field0 = const<i32>(4), field1 = const<i32>(5));
// DEFAULT-NEXT:         let %[[VALUE_designated:[0-9]+]] designated: @type[[TYPE_Triple]] [storage=automatic] = aggregate<@type[[TYPE_Triple]], zero_fill=true>(field0 = const<i32>(7), field2 = const<i32>(9));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])), read<i32>(field0(%[[VALUE_full]])), read<i32>(field1(%[[VALUE_full]])), read<i32>(field2(%[[VALUE_full]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_2]])), read<i32>(field0(%[[VALUE_partial]])), read<i32>(field1(%[[VALUE_partial]])), read<i32>(field2(%[[VALUE_partial]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_3]])), read<i32>(field0(%[[VALUE_designated]])), read<i32>(field1(%[[VALUE_designated]])), read<i32>(field2(%[[VALUE_designated]])));
// DEFAULT-NEXT:         let %[[VALUE_copy:[0-9]+]] copy: @type[[TYPE_Triple]] [storage=automatic] = copy<@type[[TYPE_Triple]], reason=assign>(read<@type[[TYPE_Triple]]>(%[[VALUE_full]]));
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_copy]]), const<i32>(42));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_4]])), read<i32>(field0(%[[VALUE_full]])), read<i32>(field0(%[[VALUE_copy]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
