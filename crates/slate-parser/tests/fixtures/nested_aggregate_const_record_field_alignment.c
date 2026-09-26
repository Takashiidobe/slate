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
// DEFAULT-NEXT:     type @type0 fnptr = ptr<fn(i32) -> i32>;
// DEFAULT-NEXT:     type @type1 Inner = struct {
// DEFAULT-NEXT:         field0 scanners: array<ptr<fn(i32) -> i32>, 3>;
// DEFAULT-NEXT:         field1 tag: i32;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 24]];
// DEFAULT-NEXT:     type @type2 Outer = struct {
// DEFAULT-NEXT:         field0 inner: @type1;
// DEFAULT-NEXT:         field1 bytes: array<u8, 8>;
// DEFAULT-NEXT:         field2 trailing1: ptr<fn(i32) -> i32>;
// DEFAULT-NEXT:         field3 trailing2: ptr<fn(i32) -> i32>;
// DEFAULT-NEXT:         field4 trailing3: i32;
// DEFAULT-NEXT:     } [size=64, align=8, offsets=[0, 32, 40, 48, 56]];
// DEFAULT-NEXT:     global %14 g: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = aggregate<@type1, zero_fill=false>(field0 = aggregate<array<ptr<fn(i32) -> i32>, 3>, zero_fill=false>(index0 = function_decay<ptr<fn(i32) -> i32>>(%2), index1 = function_decay<ptr<fn(i32) -> i32>>(%4), index2 = function_decay<ptr<fn(i32) -> i32>>(%6)), field1 = const<i32>(42)), field1 = aggregate<array<u8, 8>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(4))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(6))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8)))), field2 = function_decay<ptr<fn(i32) -> i32>>(%8), field3 = function_decay<ptr<fn(i32) -> i32>>(%10), field4 = const<i32>(99)) [linkage=external];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%17 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @add1(%3 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%3), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @add2(%5 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%5), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @add3(%7 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%7), const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @mul5(%9 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(read<i32>(%9), const<i32>(5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @mul7(%11 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(read<i32>(%11), const<i32>(7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%18)), call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(deref(ptr_offset<ptr<ptr<fn(i32) -> i32>>, subtract=false, element=ptr<fn(i32) -> i32>, overflow=ub>(array_decay<ptr<ptr<fn(i32) -> i32>>, length=Some(3)>(field0(field0(%14))), const<i32>(0)))), const<i32>(10)), call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(deref(ptr_offset<ptr<ptr<fn(i32) -> i32>>, subtract=false, element=ptr<fn(i32) -> i32>, overflow=ub>(array_decay<ptr<ptr<fn(i32) -> i32>>, length=Some(3)>(field0(field0(%14))), const<i32>(1)))), const<i32>(10)), call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(deref(ptr_offset<ptr<ptr<fn(i32) -> i32>>, subtract=false, element=ptr<fn(i32) -> i32>, overflow=ub>(array_decay<ptr<ptr<fn(i32) -> i32>>, length=Some(3)>(field0(field0(%14))), const<i32>(2)))), const<i32>(10)), read<i32>(field1(field0(%14))));
// DEFAULT-NEXT:         for %19
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %16 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%16), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %23: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%24));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%20)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%14)), read<i32>(%16)))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%21)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%22)), call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(field2(%14)), const<i32>(10)), call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(field3(%14)), const<i32>(10)), read<i32>(field4(%14)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
