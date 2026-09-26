#include <limits.h>

extern void abort();

int test1(int x) { return x ^ INT_MIN; }

unsigned int test1u(unsigned int x) { return x ^ (unsigned int)INT_MIN; }

unsigned int test2u(unsigned int x) { return x + (unsigned int)INT_MIN; }

unsigned int test3u(unsigned int x) { return x - (unsigned int)INT_MIN; }

int test4(int x) {
  int y = INT_MIN;
  return x ^ y;
}

unsigned int test4u(unsigned int x) {
  unsigned int y = (unsigned int)INT_MIN;
  return x ^ y;
}

unsigned int test5u(unsigned int x) {
  unsigned int y = (unsigned int)INT_MIN;
  return x + y;
}

unsigned int test6u(unsigned int x) {
  unsigned int y = (unsigned int)INT_MIN;
  return x - y;
}

void test(int a, int b) {
  if (test1(a) != b)
    abort();
  if (test4(a) != b)
    abort();
}

void testu(unsigned int a, unsigned int b) {
  if (test1u(a) != b)
    abort();
  if (test2u(a) != b)
    abort();
  if (test3u(a) != b)
    abort();
  if (test4u(a) != b)
    abort();
  if (test5u(a) != b)
    abort();
  if (test6u(a) != b)
    abort();
}

int main() {
#if INT_MAX == 2147483647
  test(0x00000000, 0x80000000);
  test(0x80000000, 0x00000000);
  test(0x12345678, 0x92345678);
  test(0x92345678, 0x12345678);
  test(0x7fffffff, 0xffffffff);
  test(0xffffffff, 0x7fffffff);

  testu(0x00000000, 0x80000000);
  testu(0x80000000, 0x00000000);
  testu(0x12345678, 0x92345678);
  testu(0x92345678, 0x12345678);
  testu(0x7fffffff, 0xffffffff);
  testu(0xffffffff, 0x7fffffff);
#endif

#if INT_MAX == 32767
  test(0x0000, 0x8000);
  test(0x8000, 0x0000);
  test(0x1234, 0x9234);
  test(0x9234, 0x1234);
  test(0x7fff, 0xffff);
  test(0xffff, 0x7fff);

  testu(0x0000, 0x8000);
  testu(0x8000, 0x0000);
  testu(0x1234, 0x9234);
  testu(0x9234, 0x1234);
  testu(0x7fff, 0xffff);
  testu(0xffff, 0x7fff);
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
// DEFAULT-NEXT:     fn %1 @test1(%2 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return xor<i32>(read<i32>(%2), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @test1u(%4 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return xor<u32>(read<u32>(%4), reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test2u(%6 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(read<u32>(%6), reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test3u(%8 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(read<u32>(%8), reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test4(%10 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 y: i32 [storage=automatic] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         return xor<i32>(read<i32>(%10), read<i32>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test4u(%13 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 y: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:         return xor<u32>(read<u32>(%13), read<u32>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test5u(%16 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17 y: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(read<u32>(%16), read<u32>(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test6u(%19 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %20 y: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(read<u32>(%19), read<u32>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test(%22 a: i32, %23 b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%22)), read<i32>(%23))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%9, read<i32>(%22)), read<i32>(%23))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @testu(%25 a: u32, %26 b: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%3, read<u32>(%25)), read<u32>(%26))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%5, read<u32>(%25)), read<u32>(%26))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%7, read<u32>(%25)), read<u32>(%26))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%12, read<u32>(%25)), read<u32>(%26))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%15, read<u32>(%25)), read<u32>(%26))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%18, read<u32>(%25)), read<u32>(%26))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%21, const<i32>(0), reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2147483648)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%21, reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2147483648)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%21, const<i32>(305419896), reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2452903544)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%21, reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2452903544)), const<i32>(305419896));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%21, const<i32>(2147483647), reinterpret<i32, reason=arg, fits=unknown>(const<u32>(4294967295)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%21, reinterpret<i32, reason=arg, fits=unknown>(const<u32>(4294967295)), const<i32>(2147483647));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%24, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), const<u32>(2147483648));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%24, const<u32>(2147483648), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%24, reinterpret<u32, reason=arg, fits=always>(const<i32>(305419896)), const<u32>(2452903544));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%24, const<u32>(2452903544), reinterpret<u32, reason=arg, fits=always>(const<i32>(305419896)));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%24, reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647)), const<u32>(4294967295));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%24, const<u32>(4294967295), reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
