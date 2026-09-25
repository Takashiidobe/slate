extern void abort(void);

int test1(char x) { return x / 100 == 3; }

int test1u(unsigned char x) { return x / 100 == 3; }

int test2(char x) { return x / 100 != 3; }

int test2u(unsigned char x) { return x / 100 != 3; }

int test3(char x) { return x / 100 < 3; }

int test3u(unsigned char x) { return x / 100 < 3; }

int test4(char x) { return x / 100 <= 3; }

int test4u(unsigned char x) { return x / 100 <= 3; }

int test5(char x) { return x / 100 > 3; }

int test5u(unsigned char x) { return x / 100 > 3; }

int test6(char x) { return x / 100 >= 3; }

int test6u(unsigned char x) { return x / 100 >= 3; }

int main() {
  int c;

  for (c = -128; c < 256; c++) {
    if (test1(c) != 0)
      abort();
    if (test1u(c) != 0)
      abort();
    if (test2(c) != 1)
      abort();
    if (test2u(c) != 1)
      abort();
    if (test3(c) != 1)
      abort();
    if (test3u(c) != 1)
      abort();
    if (test4(c) != 1)
      abort();
    if (test4u(c) != 1)
      abort();
    if (test5(c) != 0)
      abort();
    if (test5u(c) != 0)
      abort();
    if (test6(c) != 0)
      abort();
    if (test6u(c) != 0)
      abort();
  }
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
// DEFAULT-NEXT:     fn %1 @test1(%2 x: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @test1u(%4 x: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%4))), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test2(%6 x: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%6)), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test2u(%8 x: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%8))), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test3(%10 x: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test3u(%12 x: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%12))), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test4(%14 x: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%14)), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test4u(%16 x: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%16))), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test5(%18 x: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%18)), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test5u(%20 x: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%20))), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test6(%22 x: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%22)), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @test6u(%24 x: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%24))), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %26 c: i32 [storage=automatic];
// DEFAULT-NEXT:         for %27
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%26, neg<i32, overflow=ub>(const<i32>(128)));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%26), const<i32>(256))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = read<i32>(%26);
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%26, read<i32>(%29));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i8) -> i32>(%1, truncate<i8, reason=arg, fits=unknown>(read<i32>(%26))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u8) -> i32>(%3, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%26)))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i8) -> i32>(%5, truncate<i8, reason=arg, fits=unknown>(read<i32>(%26))), const<i32>(1))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u8) -> i32>(%7, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%26)))), const<i32>(1))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i8) -> i32>(%9, truncate<i8, reason=arg, fits=unknown>(read<i32>(%26))), const<i32>(1))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u8) -> i32>(%11, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%26)))), const<i32>(1))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i8) -> i32>(%13, truncate<i8, reason=arg, fits=unknown>(read<i32>(%26))), const<i32>(1))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u8) -> i32>(%15, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%26)))), const<i32>(1))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i8) -> i32>(%17, truncate<i8, reason=arg, fits=unknown>(read<i32>(%26))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u8) -> i32>(%19, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%26)))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i8) -> i32>(%21, truncate<i8, reason=arg, fits=unknown>(read<i32>(%26))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u8) -> i32>(%23, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%26)))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
