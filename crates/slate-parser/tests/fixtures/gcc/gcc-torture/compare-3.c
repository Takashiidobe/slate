/* Copyright (C) 2002 Free Software Foundation.

   Test for composite comparison always true/false optimization.

   Written by Roger Sayle, 7th June 2002.  */

extern void link_error0();
extern void link_error1();

void test1(int x, int y) {
  if ((x == y) && (x != y))
    link_error0();
}

void test2(int x, int y) {
  if ((x < y) && (x > y))
    link_error0();
}

void test3(int x, int y) {
  if ((x < y) && (y < x))
    link_error0();
}

void test4(int x, int y) {
  if ((x == y) || (x != y)) {
  } else
    link_error1();
}

void test5(int x, int y) {
  if ((x >= y) || (x < y)) {
  } else
    link_error1();
}

void test6(int x, int y) {
  if ((x <= y) || (y < x)) {
  } else
    link_error1();
}

void all_tests(int x, int y) {
  test1(x, y);
  test2(x, y);
  test3(x, y);
  test4(x, y);
  test5(x, y);
  test6(x, y);
}

int main() {
  all_tests(0, 0);
  all_tests(1, 2);
  all_tests(4, 3);

  return 0;
}

#ifndef __OPTIMIZE__
void link_error0() {}
void link_error1() {}
#endif /* ! __OPTIMIZE__ */


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
// DEFAULT-NEXT:     fn %0 @link_error0() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1 @link_error1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @test1(%3 x: i32, %4 y: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(eq<i32>(read<i32>(%3), read<i32>(%4)), ne<i32>(read<i32>(%3), read<i32>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test2(%6 x: i32, %7 y: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(lt<i32>(read<i32>(%6), read<i32>(%7)), gt<i32>(read<i32>(%6), read<i32>(%7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test3(%9 x: i32, %10 y: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(lt<i32>(read<i32>(%9), read<i32>(%10)), lt<i32>(read<i32>(%10), read<i32>(%9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test4(%12 x: i32, %13 y: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(eq<i32>(read<i32>(%12), read<i32>(%13)), ne<i32>(read<i32>(%12), read<i32>(%13)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test5(%15 x: i32, %16 y: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ge<i32>(read<i32>(%15), read<i32>(%16)), lt<i32>(read<i32>(%15), read<i32>(%16)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test6(%18 x: i32, %19 y: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(le<i32>(read<i32>(%18), read<i32>(%19)), lt<i32>(read<i32>(%19), read<i32>(%18)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @all_tests(%21 x: i32, %22 y: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%2, read<i32>(%21), read<i32>(%22));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%5, read<i32>(%21), read<i32>(%22));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%8, read<i32>(%21), read<i32>(%22));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%11, read<i32>(%21), read<i32>(%22));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%14, read<i32>(%21), read<i32>(%22));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%17, read<i32>(%21), read<i32>(%22));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%20, const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%20, const<i32>(1), const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%20, const<i32>(4), const<i32>(3));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
