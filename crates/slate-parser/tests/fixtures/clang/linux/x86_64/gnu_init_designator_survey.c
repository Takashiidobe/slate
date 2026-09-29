#include <stdio.h>

union FlexUnion {
  int  value;
  char data[];
};

struct OnlyFlex {
  char data[];
};

union Castable {
  int   i;
  float f;
};

enum Forward;
enum Forward { FORWARD_A, FORWARD_B };

struct Point {
  int x;
  int y;
};

struct Sized {
  int n;
  int data[];
};

static struct Sized sized = {3, {10, 20, 30}};

struct NestedOuter {
  struct Sized inner;
};

int main(void) {
  int            range_values[10] = {[2 ... 5] = 9};
  int            old_index[3]     = {[1] 11};
  struct Point   p                = {x : 1, y : 2};
  int            five             = 5;
  union Castable c                = (union Castable)five;
  enum Forward   f                = FORWARD_B;

  printf("%d %d %d %d\n", range_values[3], old_index[1], p.x + p.y, c.i);
  printf("%d\n", (int)f);
  printf("%zu %zu\n", sizeof(union FlexUnion), sizeof(struct OnlyFlex));
  printf("%d %d %d\n", sized.data[0], sized.data[1], sized.data[2]);
  printf("%zu\n", sizeof(struct NestedOuter));
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
// DEFAULT-NEXT:     type @type[[TYPE_FlexUnion:[0-9]+]] FlexUnion = union {
// DEFAULT-NEXT:         field0 value: i32;
// DEFAULT-NEXT:         field1 data: array<i8, incomplete>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_OnlyFlex:[0-9]+]] OnlyFlex = struct {
// DEFAULT-NEXT:         field0 data: array<i8, incomplete>;
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Castable:[0-9]+]] Castable = union {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 f: f32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_Forward:[0-9]+]] Forward = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_FORWARD_A:[0-9]+]] FORWARD_A = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_FORWARD_B:[0-9]+]] FORWARD_B = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_Point:[0-9]+]] Point = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_Sized:[0-9]+]] Sized = struct {
// DEFAULT-NEXT:         field0 n: i32;
// DEFAULT-NEXT:         field1 data: array<i32, incomplete>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_NestedOuter:[0-9]+]] NestedOuter = struct {
// DEFAULT-NEXT:         field0 inner: @type[[TYPE_Sized]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_sized:[0-9]+]] sized: @type[[TYPE_Sized]] [storage=static] = aggregate<@type[[TYPE_Sized]], zero_fill=false>(field0 = const<i32>(3), field1 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(10), index1 = const<i32>(20), index2 = const<i32>(30))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([37, 122, 117, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_FORWARD_B]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_range_values:[0-9]+]] range_values: array<i32, 10> [storage=automatic] [align=16] = aggregate<array<i32, 10>, zero_fill=true>(index2..=5 = const<i32>(9));
// DEFAULT-NEXT:         let %[[VALUE_old_index:[0-9]+]] old_index: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=true>(index1 = const<i32>(11));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: @type[[TYPE_Point]] [storage=automatic] = aggregate<@type[[TYPE_Point]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE_five:[0-9]+]] five: i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: @type[[TYPE_Castable]] [storage=automatic] = copy<@type[[TYPE_Castable]], reason=assign>(aggregate<@type[[TYPE_Castable]], zero_fill=false>(field0 = read<i32>(%[[VALUE_five]])));
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: @type[[TYPE_Forward]] [storage=automatic] = int_to_enum<@type[[TYPE_Forward]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_FORWARD_B]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str]])), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_range_values]]), const<i32>(3)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%[[VALUE_old_index]]), const<i32>(1)))), add<i32, overflow=ub>(read<i32>(field0(%[[VALUE_p]])), read<i32>(field1(%[[VALUE_p]]))), read<i32>(field0(%[[VALUE_c]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_FORWARD_B]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_Forward]]>(%[[VALUE_f]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_FORWARD_B]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_3]])), const<u64>(4), const<u64>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_FORWARD_B]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_4]])), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(field1(%[[VALUE_sized]])), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(field1(%[[VALUE_sized]])), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(field1(%[[VALUE_sized]])), const<i32>(2)))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_FORWARD_B]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_5]])), const<u64>(4));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
