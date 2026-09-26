/* { dg-additional-options "-fwrapv" } */

#include <limits.h>

extern void abort();

int test3(int x) { return (x + INT_MIN) ^ 0x1234; }

int test4(int x) { return (x ^ 0x1234) + INT_MIN; }

int test5(int x) { return (x - INT_MIN) ^ 0x1234; }

int test6(int x) { return (x ^ 0x1234) - INT_MIN; }

int test9(int x) {
  int y = INT_MIN;
  int z = 0x1234;
  return (x + y) ^ z;
}

int test10(int x) {
  int y = 0x1234;
  int z = INT_MIN;
  return (x ^ y) + z;
}

int test11(int x) {
  int y = INT_MIN;
  int z = 0x1234;
  return (x - y) ^ z;
}

int test12(int x) {
  int y = 0x1234;
  int z = INT_MIN;
  return (x ^ y) - z;
}

void test(int a, int b) {
  if (test3(a) != b)
    abort();
  if (test4(a) != b)
    abort();
  if (test5(a) != b)
    abort();
  if (test6(a) != b)
    abort();
  if (test9(a) != b)
    abort();
  if (test10(a) != b)
    abort();
  if (test11(a) != b)
    abort();
  if (test12(a) != b)
    abort();
}

int main() {
#if INT_MAX == 2147483647
  test(0x00000000, 0x80001234);
  test(0x00001234, 0x80000000);
  test(0x80000000, 0x00001234);
  test(0x80001234, 0x00000000);
  test(0x7fffffff, 0xffffedcb);
  test(0xffffffff, 0x7fffedcb);
#endif

#if INT_MAX == 32767
  test(0x0000, 0x9234);
  test(0x1234, 0x8000);
  test(0x8000, 0x1234);
  test(0x9234, 0x0000);
  test(0x7fff, 0xedcb);
  test(0xffff, 0x6dcb);
#endif

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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @test3(%2 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return xor<i32>(add<i32, overflow=ub>(read<i32>(%2), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), const<i32>(4660));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @test4(%4 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(xor<i32>(read<i32>(%4), const<i32>(4660)), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test5(%6 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return xor<i32>(sub<i32, overflow=ub>(read<i32>(%6), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), const<i32>(4660));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test6(%8 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(xor<i32>(read<i32>(%8), const<i32>(4660)), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test9(%10 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 y: i32 [storage=automatic] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         let %12 z: i32 [storage=automatic] = const<i32>(4660);
// DEFAULT-NEXT:         return xor<i32>(add<i32, overflow=ub>(read<i32>(%10), read<i32>(%11)), read<i32>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test10(%14 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15 y: i32 [storage=automatic] = const<i32>(4660);
// DEFAULT-NEXT:         let %16 z: i32 [storage=automatic] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(xor<i32>(read<i32>(%14), read<i32>(%15)), read<i32>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test11(%18 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %19 y: i32 [storage=automatic] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         let %20 z: i32 [storage=automatic] = const<i32>(4660);
// DEFAULT-NEXT:         return xor<i32>(sub<i32, overflow=ub>(read<i32>(%18), read<i32>(%19)), read<i32>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test12(%22 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 y: i32 [storage=automatic] = const<i32>(4660);
// DEFAULT-NEXT:         let %24 z: i32 [storage=automatic] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(xor<i32>(read<i32>(%22), read<i32>(%23)), read<i32>(%24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @test(%26 a: i32, %27 b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%26)), read<i32>(%27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%3, read<i32>(%26)), read<i32>(%27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, read<i32>(%26)), read<i32>(%27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%7, read<i32>(%26)), read<i32>(%27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%9, read<i32>(%26)), read<i32>(%27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%13, read<i32>(%26)), read<i32>(%27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%17, read<i32>(%26)), read<i32>(%27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%21, read<i32>(%26)), read<i32>(%27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%25, const<i32>(0), reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2147488308)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%25, const<i32>(4660), reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2147483648)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%25, reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2147483648)), const<i32>(4660));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%25, reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2147488308)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%25, const<i32>(2147483647), reinterpret<i32, reason=arg, fits=unknown>(const<u32>(4294962635)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%25, reinterpret<i32, reason=arg, fits=unknown>(const<u32>(4294967295)), const<i32>(2147478987));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
