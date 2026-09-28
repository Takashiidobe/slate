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
// DEFAULT-NEXT:     type @type0 FlexUnion = union {
// DEFAULT-NEXT:         field0 value: i32;
// DEFAULT-NEXT:         field1 data: array<i8, incomplete>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type1 OnlyFlex = struct {
// DEFAULT-NEXT:         field0 data: array<i8, incomplete>;
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type2 Castable = union {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 f: f32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type3 Forward = enum : u32 {
// DEFAULT-NEXT:         %0 FORWARD_A = const<i32>(0);
// DEFAULT-NEXT:         %1 FORWARD_B = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type4 Point = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type5 Sized = struct {
// DEFAULT-NEXT:         field0 n: i32;
// DEFAULT-NEXT:         field1 data: array<i32, incomplete>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type6 NestedOuter = struct {
// DEFAULT-NEXT:         field0 inner: @type5;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %10 sized: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = const<i32>(3), field1 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(10), index1 = const<i32>(20), index2 = const<i32>(30))) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([37, 122, 117, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%19 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 range_values: array<i32, 10> [storage=automatic] [align=16] = aggregate<array<i32, 10>, zero_fill=true>(index2..=5 = const<i32>(9));
// DEFAULT-NEXT:         let %14 old_index: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=true>(index1 = const<i32>(11));
// DEFAULT-NEXT:         let %15 p: @type4 [storage=automatic] = aggregate<@type4, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2));
// DEFAULT-NEXT:         let %16 five: i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         let %17 c: @type2 [storage=automatic] = copy<@type2, reason=assign>(aggregate<@type2, zero_fill=false>(field0 = read<i32>(%16)));
// DEFAULT-NEXT:         let %18 f: @type3 [storage=automatic] = int_to_enum<@type3, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%20)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%13), const<i32>(3)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%14), const<i32>(1)))), add<i32, overflow=ub>(read<i32>(field0(%15)), read<i32>(field1(%15))), read<i32>(field0(%17)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%21)), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type3>(%18))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%22)), const<u64>(4), const<u64>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%23)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(field1(%10)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(field1(%10)), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(field1(%10)), const<i32>(2)))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%24)), const<u64>(4));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
