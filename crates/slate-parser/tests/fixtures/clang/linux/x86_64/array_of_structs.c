#include <stdio.h>

struct Point {
  int x;
  int y;
};

int main(void) {
  struct Point ps[2];
  ps[0].x = 1;
  ps[0].y = 2;
  ps[1].x = 3;
  ps[1].y = 4;
  printf("%d\n", ps[0].x + ps[1].y);

  struct Point init[2] = {{10, 20}, {30, 40}};
  printf("%d\n", init[0].y + init[1].x);
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
// DEFAULT-NEXT:     type @type[[TYPE_Point:[0-9]+]] Point = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_ps:[0-9]+]] ps: array<@type[[TYPE_Point]], 2> [storage=automatic] [align=16];
// DEFAULT-NEXT:         write<i32>(field0(deref(ptr_offset<ptr<@type[[TYPE_Point]]>, subtract=false, element=@type[[TYPE_Point]], overflow=ub>(array_decay<ptr<@type[[TYPE_Point]]>, length=Some(2)>(%[[VALUE_ps]]), const<i32>(0)))), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field1(deref(ptr_offset<ptr<@type[[TYPE_Point]]>, subtract=false, element=@type[[TYPE_Point]], overflow=ub>(array_decay<ptr<@type[[TYPE_Point]]>, length=Some(2)>(%[[VALUE_ps]]), const<i32>(0)))), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(field0(deref(ptr_offset<ptr<@type[[TYPE_Point]]>, subtract=false, element=@type[[TYPE_Point]], overflow=ub>(array_decay<ptr<@type[[TYPE_Point]]>, length=Some(2)>(%[[VALUE_ps]]), const<i32>(1)))), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(field1(deref(ptr_offset<ptr<@type[[TYPE_Point]]>, subtract=false, element=@type[[TYPE_Point]], overflow=ub>(array_decay<ptr<@type[[TYPE_Point]]>, length=Some(2)>(%[[VALUE_ps]]), const<i32>(1)))), const<i32>(4));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), add<i32, overflow=ub>(read<i32>(field0(deref(ptr_offset<ptr<@type[[TYPE_Point]]>, subtract=false, element=@type[[TYPE_Point]], overflow=ub>(array_decay<ptr<@type[[TYPE_Point]]>, length=Some(2)>(%[[VALUE_ps]]), const<i32>(0))))), read<i32>(field1(deref(ptr_offset<ptr<@type[[TYPE_Point]]>, subtract=false, element=@type[[TYPE_Point]], overflow=ub>(array_decay<ptr<@type[[TYPE_Point]]>, length=Some(2)>(%[[VALUE_ps]]), const<i32>(1)))))));
// DEFAULT-NEXT:         let %[[VALUE_init:[0-9]+]] init: array<@type[[TYPE_Point]], 2> [storage=automatic] [align=16] = aggregate<array<@type[[TYPE_Point]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE_Point]], zero_fill=false>(field0 = const<i32>(10), field1 = const<i32>(20)), index1 = aggregate<@type[[TYPE_Point]], zero_fill=false>(field0 = const<i32>(30), field1 = const<i32>(40)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), add<i32, overflow=ub>(read<i32>(field1(deref(ptr_offset<ptr<@type[[TYPE_Point]]>, subtract=false, element=@type[[TYPE_Point]], overflow=ub>(array_decay<ptr<@type[[TYPE_Point]]>, length=Some(2)>(%[[VALUE_init]]), const<i32>(0))))), read<i32>(field0(deref(ptr_offset<ptr<@type[[TYPE_Point]]>, subtract=false, element=@type[[TYPE_Point]], overflow=ub>(array_decay<ptr<@type[[TYPE_Point]]>, length=Some(2)>(%[[VALUE_init]]), const<i32>(1)))))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
