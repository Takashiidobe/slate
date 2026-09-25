/* PR middle-end/17894 */

extern void abort(void);

int test1(int x) { return x / -10 == 2; }

int test2(int x) { return x / -10 == 0; }

int test3(int x) { return x / -10 != 2; }

int test4(int x) { return x / -10 != 0; }

int test5(int x) { return x / -10 < 2; }

int test6(int x) { return x / -10 < 0; }

int test7(int x) { return x / -10 <= 2; }

int test8(int x) { return x / -10 <= 0; }

int test9(int x) { return x / -10 > 2; }

int test10(int x) { return x / -10 > 0; }

int test11(int x) { return x / -10 >= 2; }

int test12(int x) { return x / -10 >= 0; }

int main() {
  if (test1(-30) != 0)
    abort();
  if (test1(-29) != 1)
    abort();
  if (test1(-20) != 1)
    abort();
  if (test1(-19) != 0)
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

  if (test3(-30) != 1)
    abort();
  if (test3(-29) != 0)
    abort();
  if (test3(-20) != 0)
    abort();
  if (test3(-19) != 1)
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

  if (test5(-30) != 0)
    abort();
  if (test5(-29) != 0)
    abort();
  if (test5(-20) != 0)
    abort();
  if (test5(-19) != 1)
    abort();

  if (test6(0) != 0)
    abort();
  if (test6(9) != 0)
    abort();
  if (test6(10) != 1)
    abort();
  if (test6(-1) != 0)
    abort();
  if (test6(-9) != 0)
    abort();
  if (test6(-10) != 0)
    abort();

  if (test7(-30) != 0)
    abort();
  if (test7(-29) != 1)
    abort();
  if (test7(-20) != 1)
    abort();
  if (test7(-19) != 1)
    abort();

  if (test8(0) != 1)
    abort();
  if (test8(9) != 1)
    abort();
  if (test8(10) != 1)
    abort();
  if (test8(-1) != 1)
    abort();
  if (test8(-9) != 1)
    abort();
  if (test8(-10) != 0)
    abort();

  if (test9(-30) != 1)
    abort();
  if (test9(-29) != 0)
    abort();
  if (test9(-20) != 0)
    abort();
  if (test9(-19) != 0)
    abort();

  if (test10(0) != 0)
    abort();
  if (test10(9) != 0)
    abort();
  if (test10(10) != 0)
    abort();
  if (test10(-1) != 0)
    abort();
  if (test10(-9) != 0)
    abort();
  if (test10(-10) != 1)
    abort();

  if (test11(-30) != 1)
    abort();
  if (test11(-29) != 1)
    abort();
  if (test11(-20) != 1)
    abort();
  if (test11(-19) != 0)
    abort();

  if (test12(0) != 1)
    abort();
  if (test12(9) != 1)
    abort();
  if (test12(10) != 0)
    abort();
  if (test12(-1) != 1)
    abort();
  if (test12(-9) != 1)
    abort();
  if (test12(-10) != 1)
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
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%2), neg<i32, overflow=ub>(const<i32>(10))), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @test2(%4 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%4), neg<i32, overflow=ub>(const<i32>(10))), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test3(%6 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%6), neg<i32, overflow=ub>(const<i32>(10))), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test4(%8 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%8), neg<i32, overflow=ub>(const<i32>(10))), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test5(%10 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%10), neg<i32, overflow=ub>(const<i32>(10))), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test6(%12 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%12), neg<i32, overflow=ub>(const<i32>(10))), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test7(%14 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%14), neg<i32, overflow=ub>(const<i32>(10))), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test8(%16 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%16), neg<i32, overflow=ub>(const<i32>(10))), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test9(%18 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%18), neg<i32, overflow=ub>(const<i32>(10))), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test10(%20 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%20), neg<i32, overflow=ub>(const<i32>(10))), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test11(%22 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%22), neg<i32, overflow=ub>(const<i32>(10))), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @test12(%24 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%24), neg<i32, overflow=ub>(const<i32>(10))), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, neg<i32, overflow=ub>(const<i32>(30))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, neg<i32, overflow=ub>(const<i32>(29))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, neg<i32, overflow=ub>(const<i32>(20))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, neg<i32, overflow=ub>(const<i32>(19))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%3, const<i32>(0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%3, const<i32>(9)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%3, const<i32>(10)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%3, neg<i32, overflow=ub>(const<i32>(1))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%3, neg<i32, overflow=ub>(const<i32>(9))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%3, neg<i32, overflow=ub>(const<i32>(10))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, neg<i32, overflow=ub>(const<i32>(30))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, neg<i32, overflow=ub>(const<i32>(29))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, neg<i32, overflow=ub>(const<i32>(20))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, neg<i32, overflow=ub>(const<i32>(19))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%7, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%7, const<i32>(9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%7, const<i32>(10)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%7, neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%7, neg<i32, overflow=ub>(const<i32>(9))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%7, neg<i32, overflow=ub>(const<i32>(10))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%9, neg<i32, overflow=ub>(const<i32>(30))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%9, neg<i32, overflow=ub>(const<i32>(29))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%9, neg<i32, overflow=ub>(const<i32>(20))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%9, neg<i32, overflow=ub>(const<i32>(19))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%11, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%11, const<i32>(9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%11, const<i32>(10)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%11, neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%11, neg<i32, overflow=ub>(const<i32>(9))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%11, neg<i32, overflow=ub>(const<i32>(10))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%13, neg<i32, overflow=ub>(const<i32>(30))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%13, neg<i32, overflow=ub>(const<i32>(29))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%13, neg<i32, overflow=ub>(const<i32>(20))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%13, neg<i32, overflow=ub>(const<i32>(19))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%15, const<i32>(0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%15, const<i32>(9)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%15, const<i32>(10)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%15, neg<i32, overflow=ub>(const<i32>(1))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%15, neg<i32, overflow=ub>(const<i32>(9))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%15, neg<i32, overflow=ub>(const<i32>(10))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%17, neg<i32, overflow=ub>(const<i32>(30))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%17, neg<i32, overflow=ub>(const<i32>(29))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%17, neg<i32, overflow=ub>(const<i32>(20))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%17, neg<i32, overflow=ub>(const<i32>(19))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%19, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%19, const<i32>(9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%19, const<i32>(10)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%19, neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%19, neg<i32, overflow=ub>(const<i32>(9))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%19, neg<i32, overflow=ub>(const<i32>(10))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%21, neg<i32, overflow=ub>(const<i32>(30))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%21, neg<i32, overflow=ub>(const<i32>(29))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%21, neg<i32, overflow=ub>(const<i32>(20))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%21, neg<i32, overflow=ub>(const<i32>(19))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%23, const<i32>(0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%23, const<i32>(9)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%23, const<i32>(10)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%23, neg<i32, overflow=ub>(const<i32>(1))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%23, neg<i32, overflow=ub>(const<i32>(9))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%23, neg<i32, overflow=ub>(const<i32>(10))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
