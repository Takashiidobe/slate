#include <stdio.h>

typedef int (*fnptr)(int);

int add1(int x) { return x + 1; }
int add2(int x) { return x + 2; }
int add3(int x) { return x + 3; }
int mul5(int x) { return x * 5; }
int mul7(int x) { return x * 7; }

struct Inner {
  fnptr scanners[3];
  int   tag;
};

struct Outer {
  struct Inner  inner;
  unsigned char bytes[8];
  fnptr         trailing1;
  fnptr         trailing2;
  int           trailing3;
};

struct Outer g = {
    {{add1, add2, add3}, 42}, {1, 2, 3, 4, 5, 6, 7, 8}, mul5, mul7, 99,
};

int main(void) {
  printf("%d %d %d %d\n", g.inner.scanners[0](10), g.inner.scanners[1](10),
         g.inner.scanners[2](10), g.inner.tag);
  for (int i = 0; i < 8; i++) {
    printf("%d ", g.bytes[i]);
  }
  printf("\n");
  printf("%d %d %d\n", g.trailing1(10), g.trailing2(10), g.trailing3);
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
// DEFAULT-NEXT:     type @type[[TYPE_fnptr:[0-9]+]] fnptr = ptr<fn(i32) -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE_Inner:[0-9]+]] Inner = struct {
// DEFAULT-NEXT:         field0 scanners: array<ptr<fn(i32) -> i32>, 3>;
// DEFAULT-NEXT:         field1 tag: i32;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_Outer:[0-9]+]] Outer = struct {
// DEFAULT-NEXT:         field0 inner: @type[[TYPE_Inner]];
// DEFAULT-NEXT:         field1 bytes: array<u8, 8>;
// DEFAULT-NEXT:         field2 trailing1: ptr<fn(i32) -> i32>;
// DEFAULT-NEXT:         field3 trailing2: ptr<fn(i32) -> i32>;
// DEFAULT-NEXT:         field4 trailing3: i32;
// DEFAULT-NEXT:     } [size=64, align=8, offsets=[0, 32, 40, 48, 56]];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: @type[[TYPE_Outer]] [storage=static] = aggregate<@type[[TYPE_Outer]], zero_fill=false>(field0 = aggregate<@type[[TYPE_Inner]], zero_fill=false>(field0 = aggregate<array<ptr<fn(i32) -> i32>, 3>, zero_fill=false>(index0 = function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_add1:[0-9]+]]), index1 = function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_add2:[0-9]+]]), index2 = function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_add3:[0-9]+]])), field1 = const<i32>(42)), field1 = aggregate<array<u8, 8>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(4))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(6))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8)))), field2 = function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_mul5:[0-9]+]]), field3 = function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_mul7:[0-9]+]]), field4 = const<i32>(99)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_add1]] @add1(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_x]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_add2]] @add2(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_x_2]]), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_add3]] @add3(%[[VALUE_x_3:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_x_3]]), const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_mul5]] @mul5(%[[VALUE_x_4:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(read<i32>(%[[VALUE_x_4]]), const<i32>(5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_mul7]] @mul7(%[[VALUE_x_5:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(read<i32>(%[[VALUE_x_5]]), const<i32>(7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str]])), call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(deref(ptr_offset<ptr<ptr<fn(i32) -> i32>>, subtract=false, element=ptr<fn(i32) -> i32>, overflow=ub>(array_decay<ptr<ptr<fn(i32) -> i32>>, length=Some(3)>(field0(field0(%[[VALUE_g]]))), const<i32>(0)))), const<i32>(10)), call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(deref(ptr_offset<ptr<ptr<fn(i32) -> i32>>, subtract=false, element=ptr<fn(i32) -> i32>, overflow=ub>(array_decay<ptr<ptr<fn(i32) -> i32>>, length=Some(3)>(field0(field0(%[[VALUE_g]]))), const<i32>(1)))), const<i32>(10)), call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(deref(ptr_offset<ptr<ptr<fn(i32) -> i32>>, subtract=false, element=ptr<fn(i32) -> i32>, overflow=ub>(array_decay<ptr<ptr<fn(i32) -> i32>>, length=Some(3)>(field0(field0(%[[VALUE_g]]))), const<i32>(2)))), const<i32>(10)), read<i32>(field1(field0(%[[VALUE_g]]))));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%[[VALUE_g]])), read<i32>(%[[VALUE_i]])))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_3]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_4]])), call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(field2(%[[VALUE_g]])), const<i32>(10)), call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(field3(%[[VALUE_g]])), const<i32>(10)), read<i32>(field4(%[[VALUE_g]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
