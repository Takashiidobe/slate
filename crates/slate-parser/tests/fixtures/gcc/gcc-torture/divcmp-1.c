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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @test1(%2 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%2), const<i32>(10)), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @test1u(%4 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<u32>(div<u32, by_zero=ub>(read<u32>(%4), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test2(%6 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%6), const<i32>(10)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test2u(%8 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<u32>(div<u32, by_zero=ub>(read<u32>(%8), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test3(%10 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%10), const<i32>(10)), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test3u(%12 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<u32>(div<u32, by_zero=ub>(read<u32>(%12), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test4(%14 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%14), const<i32>(10)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test4u(%16 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<u32>(div<u32, by_zero=ub>(read<u32>(%16), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test5(%18 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%18), const<i32>(10)), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test5u(%20 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<u32>(div<u32, by_zero=ub>(read<u32>(%20), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test6(%22 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%22), const<i32>(10)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @test7(%24 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%24), const<i32>(10)), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @test7u(%26 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<u32>(div<u32, by_zero=ub>(read<u32>(%26), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @test8(%28 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%28), const<i32>(10)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @test8u(%30 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<u32>(div<u32, by_zero=ub>(read<u32>(%30), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @test9(%32 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%32), const<i32>(10)), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @test9u(%34 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<u32>(div<u32, by_zero=ub>(read<u32>(%34), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @test10(%36 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%36), const<i32>(10)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @test10u(%38 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<u32>(div<u32, by_zero=ub>(read<u32>(%38), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @test11(%40 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%40), const<i32>(10)), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @test11u(%42 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<u32>(div<u32, by_zero=ub>(read<u32>(%42), const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @test12(%44 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%44), const<i32>(10)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, const<i32>(19)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, const<i32>(20)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, const<i32>(29)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, const<i32>(30)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%3, reinterpret<u32, reason=arg, fits=always>(const<i32>(19))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%3, reinterpret<u32, reason=arg, fits=always>(const<i32>(20))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%3, reinterpret<u32, reason=arg, fits=always>(const<i32>(29))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%3, reinterpret<u32, reason=arg, fits=always>(const<i32>(30))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, const<i32>(0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, const<i32>(9)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, const<i32>(10)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, neg<i32, overflow=ub>(const<i32>(1))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, neg<i32, overflow=ub>(const<i32>(9))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, neg<i32, overflow=ub>(const<i32>(10))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%7, reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%7, reinterpret<u32, reason=arg, fits=always>(const<i32>(9))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%7, reinterpret<u32, reason=arg, fits=always>(const<i32>(10))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%7, reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%7, reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(9)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%7, reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(10)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%9, const<i32>(19)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%9, const<i32>(20)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%9, const<i32>(29)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%9, const<i32>(30)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%11, reinterpret<u32, reason=arg, fits=always>(const<i32>(19))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%11, reinterpret<u32, reason=arg, fits=always>(const<i32>(20))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%11, reinterpret<u32, reason=arg, fits=always>(const<i32>(29))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%11, reinterpret<u32, reason=arg, fits=always>(const<i32>(30))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%13, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%13, const<i32>(9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%13, const<i32>(10)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%13, neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%13, neg<i32, overflow=ub>(const<i32>(9))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%13, neg<i32, overflow=ub>(const<i32>(10))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%15, reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%15, reinterpret<u32, reason=arg, fits=always>(const<i32>(9))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%15, reinterpret<u32, reason=arg, fits=always>(const<i32>(10))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%15, reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%15, reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(9)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%15, reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(10)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%17, const<i32>(19)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%17, const<i32>(20)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%17, const<i32>(29)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%17, const<i32>(30)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%19, reinterpret<u32, reason=arg, fits=always>(const<i32>(19))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%19, reinterpret<u32, reason=arg, fits=always>(const<i32>(20))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%19, reinterpret<u32, reason=arg, fits=always>(const<i32>(29))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%19, reinterpret<u32, reason=arg, fits=always>(const<i32>(30))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%21, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%21, const<i32>(9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%21, const<i32>(10)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%21, neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%21, neg<i32, overflow=ub>(const<i32>(9))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%21, neg<i32, overflow=ub>(const<i32>(10))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%23, const<i32>(19)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%23, const<i32>(20)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%23, const<i32>(29)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%23, const<i32>(30)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%25, reinterpret<u32, reason=arg, fits=always>(const<i32>(19))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%25, reinterpret<u32, reason=arg, fits=always>(const<i32>(20))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%25, reinterpret<u32, reason=arg, fits=always>(const<i32>(29))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%25, reinterpret<u32, reason=arg, fits=always>(const<i32>(30))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%27, const<i32>(0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%27, const<i32>(9)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%27, const<i32>(10)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%27, neg<i32, overflow=ub>(const<i32>(1))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%27, neg<i32, overflow=ub>(const<i32>(9))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%27, neg<i32, overflow=ub>(const<i32>(10))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%29, reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%29, reinterpret<u32, reason=arg, fits=always>(const<i32>(9))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%29, reinterpret<u32, reason=arg, fits=always>(const<i32>(10))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%29, reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%29, reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(9)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%29, reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(10)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%31, const<i32>(19)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%31, const<i32>(20)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%31, const<i32>(29)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%31, const<i32>(30)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%33, reinterpret<u32, reason=arg, fits=always>(const<i32>(19))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%33, reinterpret<u32, reason=arg, fits=always>(const<i32>(20))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%33, reinterpret<u32, reason=arg, fits=always>(const<i32>(29))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%33, reinterpret<u32, reason=arg, fits=always>(const<i32>(30))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%35, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%35, const<i32>(9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%35, const<i32>(10)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%35, neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%35, neg<i32, overflow=ub>(const<i32>(9))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%35, neg<i32, overflow=ub>(const<i32>(10))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%37, reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%37, reinterpret<u32, reason=arg, fits=always>(const<i32>(9))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%37, reinterpret<u32, reason=arg, fits=always>(const<i32>(10))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%37, reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%37, reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(9)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%37, reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(10)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%39, const<i32>(19)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%39, const<i32>(20)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%39, const<i32>(29)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%39, const<i32>(30)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%41, reinterpret<u32, reason=arg, fits=always>(const<i32>(19))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%41, reinterpret<u32, reason=arg, fits=always>(const<i32>(20))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%41, reinterpret<u32, reason=arg, fits=always>(const<i32>(29))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%41, reinterpret<u32, reason=arg, fits=always>(const<i32>(30))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%43, const<i32>(0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%43, const<i32>(9)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%43, const<i32>(10)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%43, neg<i32, overflow=ub>(const<i32>(1))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%43, neg<i32, overflow=ub>(const<i32>(9))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%43, neg<i32, overflow=ub>(const<i32>(10))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
