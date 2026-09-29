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
// DEFAULT-NEXT:     fn %[[VALUE_link_error0:[0-9]+]] @link_error0() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_link_error1:[0-9]+]] @link_error1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_x:[0-9]+]] x: f32, %[[VALUE_y:[0-9]+]] y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(eq<f32, exceptions=observable>(read<f32>(%[[VALUE_x]]), read<f32>(%[[VALUE_y]])), ne<f32, exceptions=observable>(read<f32>(%[[VALUE_x]]), read<f32>(%[[VALUE_y]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error0]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_x_2:[0-9]+]] x: f32, %[[VALUE_y_2:[0-9]+]] y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_2]]), read<f32>(%[[VALUE_y_2]])), gt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_2]]), read<f32>(%[[VALUE_y_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error0]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_x_3:[0-9]+]] x: f32, %[[VALUE_y_3:[0-9]+]] y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_3]]), read<f32>(%[[VALUE_y_3]])), lt<f32, exceptions=observable>(read<f32>(%[[VALUE_y_3]]), read<f32>(%[[VALUE_x_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error0]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_x_4:[0-9]+]] x: f32, %[[VALUE_y_4:[0-9]+]] y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(eq<f32, exceptions=observable>(read<f32>(%[[VALUE_x_4]]), read<f32>(%[[VALUE_y_4]])), ne<f32, exceptions=observable>(read<f32>(%[[VALUE_x_4]]), read<f32>(%[[VALUE_y_4]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5:[0-9]+]] @test5(%[[VALUE_x_5:[0-9]+]] x: f32, %[[VALUE_y_5:[0-9]+]] y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x_5]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y_5]])))), const<i32>(0)), ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_5]]), read<f32>(%[[VALUE_y_5]]))), lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_5]]), read<f32>(%[[VALUE_y_5]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6:[0-9]+]] @test6(%[[VALUE_x_6:[0-9]+]] x: f32, %[[VALUE_y_6:[0-9]+]] y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y_6]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x_6]])))), const<i32>(0)), le<f32, exceptions=observable>(read<f32>(%[[VALUE_x_6]]), read<f32>(%[[VALUE_y_6]]))), lt<f32, exceptions=observable>(read<f32>(%[[VALUE_y_6]]), read<f32>(%[[VALUE_x_6]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test7:[0-9]+]] @test7(%[[VALUE_x_7:[0-9]+]] x: f32, %[[VALUE_y_7:[0-9]+]] y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x_7]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y_7]])))), const<i32>(0)), not<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x_7]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y_7]])))), const<i32>(0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_all_tests:[0-9]+]] @all_tests(%[[VALUE_x_8:[0-9]+]] x: f32, %[[VALUE_y_8:[0-9]+]] y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_test1]], read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]]));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_test2]], read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]]));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_test3]], read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]]));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_test4]], read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]]));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_test5]], read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]]));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_test6]], read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]]));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_test7]], read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_all_tests]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_all_tests]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_all_tests]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
