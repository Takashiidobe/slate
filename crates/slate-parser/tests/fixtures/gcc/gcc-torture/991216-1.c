void abort(void);
void exit(int);

#define VALUE 0x123456789abcdefLL
#define AFTER 0x55

void test1(int a, long long value, int after) {
  if (a != 1 || value != VALUE || after != AFTER)
    abort();
}

void test2(int a, int b, long long value, int after) {
  if (a != 1 || b != 2 || value != VALUE || after != AFTER)
    abort();
}

void test3(int a, int b, int c, long long value, int after) {
  if (a != 1 || b != 2 || c != 3 || value != VALUE || after != AFTER)
    abort();
}

void test4(int a, int b, int c, int d, long long value, int after) {
  if (a != 1 || b != 2 || c != 3 || d != 4 || value != VALUE || after != AFTER)
    abort();
}

void test5(int a, int b, int c, int d, int e, long long value, int after) {
  if (a != 1 || b != 2 || c != 3 || d != 4 || e != 5 || value != VALUE ||
      after != AFTER)
    abort();
}

void test6(int a, int b, int c, int d, int e, int f, long long value,
           int after) {
  if (a != 1 || b != 2 || c != 3 || d != 4 || e != 5 || f != 6 ||
      value != VALUE || after != AFTER)
    abort();
}

void test7(int a, int b, int c, int d, int e, int f, int g, long long value,
           int after) {
  if (a != 1 || b != 2 || c != 3 || d != 4 || e != 5 || f != 6 || g != 7 ||
      value != VALUE || after != AFTER)
    abort();
}

void test8(int a, int b, int c, int d, int e, int f, int g, int h,
           long long value, int after) {
  if (a != 1 || b != 2 || c != 3 || d != 4 || e != 5 || f != 6 || g != 7 ||
      h != 8 || value != VALUE || after != AFTER)
    abort();
}

int main() {
  test1(1, VALUE, AFTER);
  test2(1, 2, VALUE, AFTER);
  test3(1, 2, 3, VALUE, AFTER);
  test4(1, 2, 3, 4, VALUE, AFTER);
  test5(1, 2, 3, 4, 5, VALUE, AFTER);
  test6(1, 2, 3, 4, 5, 6, VALUE, AFTER);
  test7(1, 2, 3, 4, 5, 6, 7, VALUE, AFTER);
  test8(1, 2, 3, 4, 5, 6, 7, 8, VALUE, AFTER);
  exit(0);
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
// DEFAULT-NEXT:     fn %1 @exit(%63 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @test1(%3 a: i32, %4 value: i64, %5 after: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%3), const<i32>(1)), ne<i64>(read<i64>(%4), const<i64>(81985529216486895))), ne<i32>(read<i32>(%5), const<i32>(85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test2(%7 a: i32, %8 b: i32, %9 value: i64, %10 after: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%7), const<i32>(1)), ne<i32>(read<i32>(%8), const<i32>(2))), ne<i64>(read<i64>(%9), const<i64>(81985529216486895))), ne<i32>(read<i32>(%10), const<i32>(85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test3(%12 a: i32, %13 b: i32, %14 c: i32, %15 value: i64, %16 after: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%12), const<i32>(1)), ne<i32>(read<i32>(%13), const<i32>(2))), ne<i32>(read<i32>(%14), const<i32>(3))), ne<i64>(read<i64>(%15), const<i64>(81985529216486895))), ne<i32>(read<i32>(%16), const<i32>(85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test4(%18 a: i32, %19 b: i32, %20 c: i32, %21 d: i32, %22 value: i64, %23 after: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%18), const<i32>(1)), ne<i32>(read<i32>(%19), const<i32>(2))), ne<i32>(read<i32>(%20), const<i32>(3))), ne<i32>(read<i32>(%21), const<i32>(4))), ne<i64>(read<i64>(%22), const<i64>(81985529216486895))), ne<i32>(read<i32>(%23), const<i32>(85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @test5(%25 a: i32, %26 b: i32, %27 c: i32, %28 d: i32, %29 e: i32, %30 value: i64, %31 after: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%25), const<i32>(1)), ne<i32>(read<i32>(%26), const<i32>(2))), ne<i32>(read<i32>(%27), const<i32>(3))), ne<i32>(read<i32>(%28), const<i32>(4))), ne<i32>(read<i32>(%29), const<i32>(5))), ne<i64>(read<i64>(%30), const<i64>(81985529216486895))), ne<i32>(read<i32>(%31), const<i32>(85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @test6(%33 a: i32, %34 b: i32, %35 c: i32, %36 d: i32, %37 e: i32, %38 f: i32, %39 value: i64, %40 after: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%33), const<i32>(1)), ne<i32>(read<i32>(%34), const<i32>(2))), ne<i32>(read<i32>(%35), const<i32>(3))), ne<i32>(read<i32>(%36), const<i32>(4))), ne<i32>(read<i32>(%37), const<i32>(5))), ne<i32>(read<i32>(%38), const<i32>(6))), ne<i64>(read<i64>(%39), const<i64>(81985529216486895))), ne<i32>(read<i32>(%40), const<i32>(85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @test7(%42 a: i32, %43 b: i32, %44 c: i32, %45 d: i32, %46 e: i32, %47 f: i32, %48 g: i32, %49 value: i64, %50 after: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%42), const<i32>(1)), ne<i32>(read<i32>(%43), const<i32>(2))), ne<i32>(read<i32>(%44), const<i32>(3))), ne<i32>(read<i32>(%45), const<i32>(4))), ne<i32>(read<i32>(%46), const<i32>(5))), ne<i32>(read<i32>(%47), const<i32>(6))), ne<i32>(read<i32>(%48), const<i32>(7))), ne<i64>(read<i64>(%49), const<i64>(81985529216486895))), ne<i32>(read<i32>(%50), const<i32>(85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %51 @test8(%52 a: i32, %53 b: i32, %54 c: i32, %55 d: i32, %56 e: i32, %57 f: i32, %58 g: i32, %59 h: i32, %60 value: i64, %61 after: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%52), const<i32>(1)), ne<i32>(read<i32>(%53), const<i32>(2))), ne<i32>(read<i32>(%54), const<i32>(3))), ne<i32>(read<i32>(%55), const<i32>(4))), ne<i32>(read<i32>(%56), const<i32>(5))), ne<i32>(read<i32>(%57), const<i32>(6))), ne<i32>(read<i32>(%58), const<i32>(7))), ne<i32>(read<i32>(%59), const<i32>(8))), ne<i64>(read<i64>(%60), const<i64>(81985529216486895))), ne<i32>(read<i32>(%61), const<i32>(85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %62 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i64, i32) -> void>(%2, const<i32>(1), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i64, i32) -> void>(%6, const<i32>(1), const<i32>(2), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i64, i32) -> void>(%11, const<i32>(1), const<i32>(2), const<i32>(3), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i64, i32) -> void>(%17, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i64, i32) -> void>(%24, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i64, i32) -> void>(%32, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i64, i32) -> void>(%41, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i64, i32) -> void>(%51, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
