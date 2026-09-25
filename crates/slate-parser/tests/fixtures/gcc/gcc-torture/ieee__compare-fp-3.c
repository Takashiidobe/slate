/* { dg-do run }
   { dg-additional-options "-fno-trapping-math" } */

/* Copyright (C) 2004 Free Software Foundation.

   Test for composite comparison always true/false optimization.

   Written by Paolo Bonzini, 26th May 2004.  */

extern void link_error0();
extern void link_error1();

void test1(float x, float y) {
  if ((x == y) && (x != y))
    link_error0();
}

void test2(float x, float y) {
  if ((x < y) && (x > y))
    link_error0();
}

void test3(float x, float y) {
  if ((x < y) && (y < x))
    link_error0();
}

void test4(float x, float y) {
  if ((x == y) || (x != y)) {
  } else
    link_error1();
}

void test5(float x, float y) {
  if (__builtin_isunordered(x, y) || (x >= y) || (x < y)) {
  } else
    link_error1();
}

void test6(float x, float y) {
  if (__builtin_isunordered(y, x) || (x <= y) || (y < x)) {
  } else
    link_error1();
}

void test7(float x, float y) {
  if (__builtin_isunordered(x, y) || !__builtin_isunordered(x, y)) {
  } else
    link_error1();
}

void all_tests(float x, float y) {
  test1(x, y);
  test2(x, y);
  test3(x, y);
  test4(x, y);
  test5(x, y);
  test6(x, y);
  test7(x, y);
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
// DEFAULT-NEXT:     fn %2 @test1(%3 x: f32, %4 y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(eq<f32, exceptions=ignore>(read<f32>(%3), read<f32>(%4)), ne<f32, exceptions=ignore>(read<f32>(%3), read<f32>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test2(%6 x: f32, %7 y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(lt<f32, exceptions=ignore>(read<f32>(%6), read<f32>(%7)), gt<f32, exceptions=ignore>(read<f32>(%6), read<f32>(%7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test3(%9 x: f32, %10 y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(lt<f32, exceptions=ignore>(read<f32>(%9), read<f32>(%10)), lt<f32, exceptions=ignore>(read<f32>(%10), read<f32>(%9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test4(%12 x: f32, %13 y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(eq<f32, exceptions=ignore>(read<f32>(%12), read<f32>(%13)), ne<f32, exceptions=ignore>(read<f32>(%12), read<f32>(%13)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test5(%15 x: f32, %16 y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%15))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%16)))), const<i32>(0)), ge<f32, exceptions=ignore>(read<f32>(%15), read<f32>(%16))), lt<f32, exceptions=ignore>(read<f32>(%15), read<f32>(%16)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test6(%18 x: f32, %19 y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%19))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%18)))), const<i32>(0)), le<f32, exceptions=ignore>(read<f32>(%18), read<f32>(%19))), lt<f32, exceptions=ignore>(read<f32>(%19), read<f32>(%18)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @test7(%21 x: f32, %22 y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%21))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%22)))), const<i32>(0)), not<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%21))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%22)))), const<i32>(0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @all_tests(%24 x: f32, %25 y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%2, read<f32>(%24), read<f32>(%25));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%5, read<f32>(%24), read<f32>(%25));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%8, read<f32>(%24), read<f32>(%25));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%11, read<f32>(%24), read<f32>(%25));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%14, read<f32>(%24), read<f32>(%25));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%17, read<f32>(%24), read<f32>(%25));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%20, read<f32>(%24), read<f32>(%25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%23, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%23, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%23, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(4)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
