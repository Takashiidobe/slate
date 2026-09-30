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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return xor<i32>(read<i32>(%[[VALUE_x]]), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1u:[0-9]+]] @test1u(%[[VALUE_x_2:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return xor<u32>(read<u32>(%[[VALUE_x_2]]), reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2u:[0-9]+]] @test2u(%[[VALUE_x_3:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(read<u32>(%[[VALUE_x_3]]), reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3u:[0-9]+]] @test3u(%[[VALUE_x_4:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(read<u32>(%[[VALUE_x_4]]), reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_x_5:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         return xor<i32>(read<i32>(%[[VALUE_x_5]]), read<i32>(%[[VALUE_y]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4u:[0-9]+]] @test4u(%[[VALUE_x_6:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:         return xor<u32>(read<u32>(%[[VALUE_x_6]]), read<u32>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5u:[0-9]+]] @test5u(%[[VALUE_x_7:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_3:[0-9]+]] y: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(read<u32>(%[[VALUE_x_7]]), read<u32>(%[[VALUE_y_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6u:[0-9]+]] @test6u(%[[VALUE_x_8:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_4:[0-9]+]] y: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(read<u32>(%[[VALUE_x_8]]), read<u32>(%[[VALUE_y_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test1]], read<i32>(%[[VALUE_a]])), read<i32>(%[[VALUE_b]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test4]], read<i32>(%[[VALUE_a]])), read<i32>(%[[VALUE_b]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testu:[0-9]+]] @testu(%[[VALUE_a_2:[0-9]+]] a: u32, %[[VALUE_b_2:[0-9]+]] b: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_test1u]], read<u32>(%[[VALUE_a_2]])), read<u32>(%[[VALUE_b_2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_test2u]], read<u32>(%[[VALUE_a_2]])), read<u32>(%[[VALUE_b_2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_test3u]], read<u32>(%[[VALUE_a_2]])), read<u32>(%[[VALUE_b_2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_test4u]], read<u32>(%[[VALUE_a_2]])), read<u32>(%[[VALUE_b_2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_test5u]], read<u32>(%[[VALUE_a_2]])), read<u32>(%[[VALUE_b_2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_test6u]], read<u32>(%[[VALUE_a_2]])), read<u32>(%[[VALUE_b_2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_test]], const<i32>(0), reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2147483648)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_test]], reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2147483648)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_test]], const<i32>(305419896), reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2452903544)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_test]], reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2452903544)), const<i32>(305419896));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_test]], const<i32>(2147483647), reinterpret<i32, reason=arg, fits=unknown>(const<u32>(4294967295)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_test]], reinterpret<i32, reason=arg, fits=unknown>(const<u32>(4294967295)), const<i32>(2147483647));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%[[VALUE_testu]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), const<u32>(2147483648));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%[[VALUE_testu]], const<u32>(2147483648), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%[[VALUE_testu]], reinterpret<u32, reason=arg, fits=always>(const<i32>(305419896)), const<u32>(2452903544));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%[[VALUE_testu]], const<u32>(2452903544), reinterpret<u32, reason=arg, fits=always>(const<i32>(305419896)));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%[[VALUE_testu]], reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647)), const<u32>(4294967295));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%[[VALUE_testu]], const<u32>(4294967295), reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
