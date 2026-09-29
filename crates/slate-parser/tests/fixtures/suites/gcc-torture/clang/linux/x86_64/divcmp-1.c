extern void abort(void);

int test1(int x) { return x / 10 == 2; }

int test1u(unsigned int x) { return x / 10U == 2; }

int test2(int x) { return x / 10 == 0; }

int test2u(unsigned int x) { return x / 10U == 0; }

int test3(int x) { return x / 10 != 2; }

int test3u(unsigned int x) { return x / 10U != 2; }

int test4(int x) { return x / 10 != 0; }

int test4u(unsigned int x) { return x / 10U != 0; }

int test5(int x) { return x / 10 < 2; }

int test5u(unsigned int x) { return x / 10U < 2; }

int test6(int x) { return x / 10 < 0; }

int test7(int x) { return x / 10 <= 2; }

int test7u(unsigned int x) { return x / 10U <= 2; }

int test8(int x) { return x / 10 <= 0; }

int test8u(unsigned int x) { return x / 10U <= 0; }

int test9(int x) { return x / 10 > 2; }

int test9u(unsigned int x) { return x / 10U > 2; }

int test10(int x) { return x / 10 > 0; }

int test10u(unsigned int x) { return x / 10U > 0; }

int test11(int x) { return x / 10 >= 2; }

int test11u(unsigned int x) { return x / 10U >= 2; }

int test12(int x) { return x / 10 >= 0; }

int main() {
  if (test1(19) != 0)
    abort();
  if (test1(20) != 1)
    abort();
  if (test1(29) != 1)
    abort();
  if (test1(30) != 0)
    abort();

  if (test1u(19) != 0)
    abort();
  if (test1u(20) != 1)
    abort();
  if (test1u(29) != 1)
    abort();
  if (test1u(30) != 0)
    abort();

  if (test2(0) != 1)
    abort();
  if (test2(9) != 1)
    abort();
  if (test2(10) != 0)
    abort();
  if (test2(-1) != 1)
    abort();
  if (test2(-9) != 1)
    abort();
  if (test2(-10) != 0)
    abort();

  if (test2u(0) != 1)
    abort();
  if (test2u(9) != 1)
    abort();
  if (test2u(10) != 0)
    abort();
  if (test2u(-1) != 0)
    abort();
  if (test2u(-9) != 0)
    abort();
  if (test2u(-10) != 0)
    abort();

  if (test3(19) != 1)
    abort();
  if (test3(20) != 0)
    abort();
  if (test3(29) != 0)
    abort();
  if (test3(30) != 1)
    abort();

  if (test3u(19) != 1)
    abort();
  if (test3u(20) != 0)
    abort();
  if (test3u(29) != 0)
    abort();
  if (test3u(30) != 1)
    abort();

  if (test4(0) != 0)
    abort();
  if (test4(9) != 0)
    abort();
  if (test4(10) != 1)
    abort();
  if (test4(-1) != 0)
    abort();
  if (test4(-9) != 0)
    abort();
  if (test4(-10) != 1)
    abort();

  if (test4u(0) != 0)
    abort();
  if (test4u(9) != 0)
    abort();
  if (test4u(10) != 1)
    abort();
  if (test4u(-1) != 1)
    abort();
  if (test4u(-9) != 1)
    abort();
  if (test4u(-10) != 1)
    abort();

  if (test5(19) != 1)
    abort();
  if (test5(20) != 0)
    abort();
  if (test5(29) != 0)
    abort();
  if (test5(30) != 0)
    abort();

  if (test5u(19) != 1)
    abort();
  if (test5u(20) != 0)
    abort();
  if (test5u(29) != 0)
    abort();
  if (test5u(30) != 0)
    abort();

  if (test6(0) != 0)
    abort();
  if (test6(9) != 0)
    abort();
  if (test6(10) != 0)
    abort();
  if (test6(-1) != 0)
    abort();
  if (test6(-9) != 0)
    abort();
  if (test6(-10) != 1)
    abort();

  if (test7(19) != 1)
    abort();
  if (test7(20) != 1)
    abort();
  if (test7(29) != 1)
    abort();
  if (test7(30) != 0)
    abort();

  if (test7u(19) != 1)
    abort();
  if (test7u(20) != 1)
    abort();
  if (test7u(29) != 1)
    abort();
  if (test7u(30) != 0)
    abort();

  if (test8(0) != 1)
    abort();
  if (test8(9) != 1)
    abort();
  if (test8(10) != 0)
    abort();
  if (test8(-1) != 1)
    abort();
  if (test8(-9) != 1)
    abort();
  if (test8(-10) != 1)
    abort();

  if (test8u(0) != 1)
    abort();
  if (test8u(9) != 1)
    abort();
  if (test8u(10) != 0)
    abort();
  if (test8u(-1) != 0)
    abort();
  if (test8u(-9) != 0)
    abort();
  if (test8u(-10) != 0)
    abort();

  if (test9(19) != 0)
    abort();
  if (test9(20) != 0)
    abort();
  if (test9(29) != 0)
    abort();
  if (test9(30) != 1)
    abort();

  if (test9u(19) != 0)
    abort();
  if (test9u(20) != 0)
    abort();
  if (test9u(29) != 0)
    abort();
  if (test9u(30) != 1)
    abort();

  if (test10(0) != 0)
    abort();
  if (test10(9) != 0)
    abort();
  if (test10(10) != 1)
    abort();
  if (test10(-1) != 0)
    abort();
  if (test10(-9) != 0)
    abort();
  if (test10(-10) != 0)
    abort();

  if (test10u(0) != 0)
    abort();
  if (test10u(9) != 0)
    abort();
  if (test10u(10) != 1)
    abort();
  if (test10u(-1) != 1)
    abort();
  if (test10u(-9) != 1)
    abort();
  if (test10u(-10) != 1)
    abort();

  if (test11(19) != 0)
    abort();
  if (test11(20) != 1)
    abort();
  if (test11(29) != 1)
    abort();
  if (test11(30) != 1)
    abort();

  if (test11u(19) != 0)
    abort();
  if (test11u(20) != 1)
    abort();
  if (test11u(29) != 1)
    abort();
  if (test11u(30) != 1)
    abort();

  if (test12(0) != 1)
    abort();
  if (test12(9) != 1)
    abort();
  if (test12(10) != 1)
    abort();
  if (test12(-1) != 1)
    abort();
  if (test12(-9) != 1)
    abort();
  if (test12(-10) != 0)
    abort();

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
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x]]), const<i32>(10)), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1u:[0-9]+]] @test1u(%[[VALUE_x_2:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<u32>(div<u32, by_zero=ub>(read<u32>(%[[VALUE_x_2]]), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_x_3:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x_3]]), const<i32>(10)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2u:[0-9]+]] @test2u(%[[VALUE_x_4:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<u32>(div<u32, by_zero=ub>(read<u32>(%[[VALUE_x_4]]), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_x_5:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x_5]]), const<i32>(10)), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3u:[0-9]+]] @test3u(%[[VALUE_x_6:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<u32>(div<u32, by_zero=ub>(read<u32>(%[[VALUE_x_6]]), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_x_7:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x_7]]), const<i32>(10)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4u:[0-9]+]] @test4u(%[[VALUE_x_8:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<u32>(div<u32, by_zero=ub>(read<u32>(%[[VALUE_x_8]]), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5:[0-9]+]] @test5(%[[VALUE_x_9:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x_9]]), const<i32>(10)), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5u:[0-9]+]] @test5u(%[[VALUE_x_10:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<u32>(div<u32, by_zero=ub>(read<u32>(%[[VALUE_x_10]]), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6:[0-9]+]] @test6(%[[VALUE_x_11:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x_11]]), const<i32>(10)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test7:[0-9]+]] @test7(%[[VALUE_x_12:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x_12]]), const<i32>(10)), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test7u:[0-9]+]] @test7u(%[[VALUE_x_13:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<u32>(div<u32, by_zero=ub>(read<u32>(%[[VALUE_x_13]]), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test8:[0-9]+]] @test8(%[[VALUE_x_14:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x_14]]), const<i32>(10)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test8u:[0-9]+]] @test8u(%[[VALUE_x_15:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<u32>(div<u32, by_zero=ub>(read<u32>(%[[VALUE_x_15]]), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test9:[0-9]+]] @test9(%[[VALUE_x_16:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x_16]]), const<i32>(10)), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test9u:[0-9]+]] @test9u(%[[VALUE_x_17:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<u32>(div<u32, by_zero=ub>(read<u32>(%[[VALUE_x_17]]), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test10:[0-9]+]] @test10(%[[VALUE_x_18:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x_18]]), const<i32>(10)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test10u:[0-9]+]] @test10u(%[[VALUE_x_19:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<u32>(div<u32, by_zero=ub>(read<u32>(%[[VALUE_x_19]]), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test11:[0-9]+]] @test11(%[[VALUE_x_20:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x_20]]), const<i32>(10)), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test11u:[0-9]+]] @test11u(%[[VALUE_x_21:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<u32>(div<u32, by_zero=ub>(read<u32>(%[[VALUE_x_21]]), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test12:[0-9]+]] @test12(%[[VALUE_x_22:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x_22]]), const<i32>(10)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test1]], const<i32>(19)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test1]], const<i32>(20)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test1]], const<i32>(29)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test1]], const<i32>(30)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test1u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(19))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test1u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(20))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test1u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(29))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test1u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(30))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test2]], const<i32>(0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test2]], const<i32>(9)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test2]], const<i32>(10)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test2]], neg<i32, overflow=ub>(const<i32>(1))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test2]], neg<i32, overflow=ub>(const<i32>(9))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test2]], neg<i32, overflow=ub>(const<i32>(10))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test2u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test2u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(9))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test2u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(10))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test2u]], reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test2u]], reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(9)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test2u]], reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(10)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test3]], const<i32>(19)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test3]], const<i32>(20)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test3]], const<i32>(29)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test3]], const<i32>(30)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test3u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(19))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test3u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(20))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test3u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(29))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test3u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(30))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test4]], const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test4]], const<i32>(9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test4]], const<i32>(10)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test4]], neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test4]], neg<i32, overflow=ub>(const<i32>(9))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test4]], neg<i32, overflow=ub>(const<i32>(10))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test4u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test4u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(9))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test4u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(10))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test4u]], reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test4u]], reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(9)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test4u]], reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(10)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test5]], const<i32>(19)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test5]], const<i32>(20)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test5]], const<i32>(29)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test5]], const<i32>(30)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test5u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(19))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test5u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(20))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test5u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(29))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test5u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(30))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test6]], const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test6]], const<i32>(9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test6]], const<i32>(10)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test6]], neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test6]], neg<i32, overflow=ub>(const<i32>(9))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test6]], neg<i32, overflow=ub>(const<i32>(10))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test7]], const<i32>(19)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test7]], const<i32>(20)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test7]], const<i32>(29)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test7]], const<i32>(30)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test7u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(19))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test7u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(20))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test7u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(29))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test7u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(30))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test8]], const<i32>(0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test8]], const<i32>(9)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test8]], const<i32>(10)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test8]], neg<i32, overflow=ub>(const<i32>(1))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test8]], neg<i32, overflow=ub>(const<i32>(9))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test8]], neg<i32, overflow=ub>(const<i32>(10))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test8u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test8u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(9))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test8u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(10))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test8u]], reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test8u]], reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(9)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test8u]], reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(10)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test9]], const<i32>(19)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test9]], const<i32>(20)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test9]], const<i32>(29)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test9]], const<i32>(30)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test9u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(19))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test9u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(20))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test9u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(29))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test9u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(30))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test10]], const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test10]], const<i32>(9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test10]], const<i32>(10)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test10]], neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test10]], neg<i32, overflow=ub>(const<i32>(9))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test10]], neg<i32, overflow=ub>(const<i32>(10))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test10u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test10u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(9))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test10u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(10))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test10u]], reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test10u]], reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(9)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test10u]], reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(10)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test11]], const<i32>(19)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test11]], const<i32>(20)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test11]], const<i32>(29)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test11]], const<i32>(30)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test11u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(19))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test11u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(20))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test11u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(29))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_test11u]], reinterpret<u32, reason=arg, fits=always>(const<i32>(30))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test12]], const<i32>(0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test12]], const<i32>(9)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test12]], const<i32>(10)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test12]], neg<i32, overflow=ub>(const<i32>(1))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test12]], neg<i32, overflow=ub>(const<i32>(9))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_test12]], neg<i32, overflow=ub>(const<i32>(10))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
