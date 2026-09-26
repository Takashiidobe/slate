/* { dg-additional-options "-fwrapv" } */

#include <limits.h>

extern void abort();

int test2(int x) { return ~(x + INT_MIN); }

int test3(int x) { return ~(x - INT_MIN); }

int test5(int x) {
  int y = INT_MIN;
  return ~(x + y);
}

int test6(int x) {
  int y = INT_MIN;
  return ~(x - y);
}

void test(int a, int b) {
  if (test2(a) != b)
    abort();
  if (test3(a) != b)
    abort();
  if (test5(a) != b)
    abort();
  if (test6(a) != b)
    abort();
}

int main() {
#if INT_MAX == 2147483647
  test(0x00000000, 0x7fffffff);
  test(0x80000000, 0xffffffff);
  test(0x12345678, 0x6dcba987);
  test(0x92345678, 0xedcba987);
  test(0x7fffffff, 0x00000000);
  test(0xffffffff, 0x80000000);
#endif

#if INT_MAX == 32767
  test(0x0000, 0x7fff);
  test(0x8000, 0xffff);
  test(0x1234, 0x6dcb);
  test(0x9234, 0xedcb);
  test(0x7fff, 0x0000);
  test(0xffff, 0x8000);
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
// DEFAULT-NEXT:     fn %1 @test2(%2 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return not<i32>(add<i32, overflow=ub>(read<i32>(%2), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @test3(%4 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return not<i32>(sub<i32, overflow=ub>(read<i32>(%4), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test5(%6 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 y: i32 [storage=automatic] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         return not<i32>(add<i32, overflow=ub>(read<i32>(%6), read<i32>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test6(%9 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 y: i32 [storage=automatic] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         return not<i32>(sub<i32, overflow=ub>(read<i32>(%9), read<i32>(%10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test(%12 a: i32, %13 b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%12)), read<i32>(%13))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%3, read<i32>(%12)), read<i32>(%13))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, read<i32>(%12)), read<i32>(%13))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%8, read<i32>(%12)), read<i32>(%13))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%11, const<i32>(0), const<i32>(2147483647));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%11, reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2147483648)), reinterpret<i32, reason=arg, fits=unknown>(const<u32>(4294967295)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%11, const<i32>(305419896), const<i32>(1842063751));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%11, reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2452903544)), reinterpret<i32, reason=arg, fits=unknown>(const<u32>(3989547399)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%11, const<i32>(2147483647), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%11, reinterpret<i32, reason=arg, fits=unknown>(const<u32>(4294967295)), reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2147483648)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
