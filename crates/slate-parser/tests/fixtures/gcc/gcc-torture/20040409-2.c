#include <limits.h>

extern void abort();

int test1(int x) { return (x ^ INT_MIN) ^ 0x1234; }

unsigned int test1u(unsigned int x) {
  return (x ^ (unsigned int)INT_MIN) ^ 0x1234;
}

int test2(int x) { return (x ^ 0x1234) ^ INT_MIN; }

unsigned int test2u(unsigned int x) {
  return (x ^ 0x1234) ^ (unsigned int)INT_MIN;
}

unsigned int test3u(unsigned int x) {
  return (x + (unsigned int)INT_MIN) ^ 0x1234;
}

unsigned int test4u(unsigned int x) {
  return (x ^ 0x1234) + (unsigned int)INT_MIN;
}

unsigned int test5u(unsigned int x) {
  return (x - (unsigned int)INT_MIN) ^ 0x1234;
}

unsigned int test6u(unsigned int x) {
  return (x ^ 0x1234) - (unsigned int)INT_MIN;
}

int test7(int x) {
  int y = INT_MIN;
  int z = 0x1234;
  return (x ^ y) ^ z;
}

unsigned int test7u(unsigned int x) {
  unsigned int y = (unsigned int)INT_MIN;
  unsigned int z = 0x1234;
  return (x ^ y) ^ z;
}

int test8(int x) {
  int y = 0x1234;
  int z = INT_MIN;
  return (x ^ y) ^ z;
}

unsigned int test8u(unsigned int x) {
  unsigned int y = 0x1234;
  unsigned int z = (unsigned int)INT_MIN;
  return (x ^ y) ^ z;
}

unsigned int test9u(unsigned int x) {
  unsigned int y = (unsigned int)INT_MIN;
  unsigned int z = 0x1234;
  return (x + y) ^ z;
}

unsigned int test10u(unsigned int x) {
  unsigned int y = 0x1234;
  unsigned int z = (unsigned int)INT_MIN;
  return (x ^ y) + z;
}

unsigned int test11u(unsigned int x) {
  unsigned int y = (unsigned int)INT_MIN;
  unsigned int z = 0x1234;
  return (x - y) ^ z;
}

unsigned int test12u(unsigned int x) {
  unsigned int y = 0x1234;
  unsigned int z = (unsigned int)INT_MIN;
  return (x ^ y) - z;
}

void test(int a, int b) {
  if (test1(a) != b)
    abort();
  if (test2(a) != b)
    abort();
  if (test7(a) != b)
    abort();
  if (test8(a) != b)
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
  if (test7u(a) != b)
    abort();
  if (test8u(a) != b)
    abort();
  if (test9u(a) != b)
    abort();
  if (test10u(a) != b)
    abort();
  if (test11u(a) != b)
    abort();
  if (test12u(a) != b)
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

  testu(0x00000000, 0x80001234);
  testu(0x00001234, 0x80000000);
  testu(0x80000000, 0x00001234);
  testu(0x80001234, 0x00000000);
  testu(0x7fffffff, 0xffffedcb);
  testu(0xffffffff, 0x7fffedcb);
#endif

#if INT_MAX == 32767
  test(0x0000, 0x9234);
  test(0x1234, 0x8000);
  test(0x8000, 0x1234);
  test(0x9234, 0x0000);
  test(0x7fff, 0xedcb);
  test(0xffff, 0x6dcb);

  testu(0x0000, 0x9234);
  testu(0x8000, 0x1234);
  testu(0x1234, 0x8000);
  testu(0x9234, 0x0000);
  testu(0x7fff, 0xedcb);
  testu(0xffff, 0x6dcb);
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
// DEFAULT-NEXT:         return xor<i32>(xor<i32>(read<i32>(%2), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), const<i32>(4660));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @test1u(%4 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return xor<u32>(xor<u32>(read<u32>(%4), reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4660)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test2(%6 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return xor<i32>(xor<i32>(read<i32>(%6), const<i32>(4660)), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test2u(%8 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return xor<u32>(xor<u32>(read<u32>(%8), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4660))), reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test3u(%10 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return xor<u32>(add<u32, overflow=wrap>(read<u32>(%10), reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4660)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test4u(%12 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(xor<u32>(read<u32>(%12), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4660))), reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test5u(%14 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return xor<u32>(sub<u32, overflow=wrap>(read<u32>(%14), reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4660)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test6u(%16 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(xor<u32>(read<u32>(%16), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4660))), reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test7(%18 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %19 y: i32 [storage=automatic] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         let %20 z: i32 [storage=automatic] = const<i32>(4660);
// DEFAULT-NEXT:         return xor<i32>(xor<i32>(read<i32>(%18), read<i32>(%19)), read<i32>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test7u(%22 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 y: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:         let %24 z: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(4660));
// DEFAULT-NEXT:         return xor<u32>(xor<u32>(read<u32>(%22), read<u32>(%23)), read<u32>(%24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @test8(%26 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %27 y: i32 [storage=automatic] = const<i32>(4660);
// DEFAULT-NEXT:         let %28 z: i32 [storage=automatic] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         return xor<i32>(xor<i32>(read<i32>(%26), read<i32>(%27)), read<i32>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @test8u(%30 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %31 y: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(4660));
// DEFAULT-NEXT:         let %32 z: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:         return xor<u32>(xor<u32>(read<u32>(%30), read<u32>(%31)), read<u32>(%32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @test9u(%34 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %35 y: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:         let %36 z: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(4660));
// DEFAULT-NEXT:         return xor<u32>(add<u32, overflow=wrap>(read<u32>(%34), read<u32>(%35)), read<u32>(%36));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @test10u(%38 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %39 y: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(4660));
// DEFAULT-NEXT:         let %40 z: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(xor<u32>(read<u32>(%38), read<u32>(%39)), read<u32>(%40));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @test11u(%42 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %43 y: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:         let %44 z: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(4660));
// DEFAULT-NEXT:         return xor<u32>(sub<u32, overflow=wrap>(read<u32>(%42), read<u32>(%43)), read<u32>(%44));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @test12u(%46 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %47 y: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(4660));
// DEFAULT-NEXT:         let %48 z: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(xor<u32>(read<u32>(%46), read<u32>(%47)), read<u32>(%48));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @test(%50 a: i32, %51 b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%50)), read<i32>(%51))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, read<i32>(%50)), read<i32>(%51))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%17, read<i32>(%50)), read<i32>(%51))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%25, read<i32>(%50)), read<i32>(%51))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @testu(%53 a: u32, %54 b: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%3, read<u32>(%53)), read<u32>(%54))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%7, read<u32>(%53)), read<u32>(%54))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%9, read<u32>(%53)), read<u32>(%54))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%11, read<u32>(%53)), read<u32>(%54))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%13, read<u32>(%53)), read<u32>(%54))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%15, read<u32>(%53)), read<u32>(%54))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%21, read<u32>(%53)), read<u32>(%54))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%29, read<u32>(%53)), read<u32>(%54))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%33, read<u32>(%53)), read<u32>(%54))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%37, read<u32>(%53)), read<u32>(%54))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%41, read<u32>(%53)), read<u32>(%54))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%45, read<u32>(%53)), read<u32>(%54))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %55 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%49, const<i32>(0), reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2147488308)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%49, const<i32>(4660), reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2147483648)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%49, reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2147483648)), const<i32>(4660));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%49, reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2147488308)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%49, const<i32>(2147483647), reinterpret<i32, reason=arg, fits=unknown>(const<u32>(4294962635)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%49, reinterpret<i32, reason=arg, fits=unknown>(const<u32>(4294967295)), const<i32>(2147478987));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%52, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), const<u32>(2147488308));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%52, reinterpret<u32, reason=arg, fits=always>(const<i32>(4660)), const<u32>(2147483648));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%52, const<u32>(2147483648), reinterpret<u32, reason=arg, fits=always>(const<i32>(4660)));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%52, const<u32>(2147488308), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%52, reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647)), const<u32>(4294962635));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%52, const<u32>(4294967295), reinterpret<u32, reason=arg, fits=always>(const<i32>(2147478987)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
