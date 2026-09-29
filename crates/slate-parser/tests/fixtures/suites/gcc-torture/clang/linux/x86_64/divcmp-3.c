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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_x:[0-9]+]] x: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_x]])), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1u:[0-9]+]] @test1u(%[[VALUE_x_2:[0-9]+]] x: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_x_2]]))), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_x_3:[0-9]+]] x: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_x_3]])), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2u:[0-9]+]] @test2u(%[[VALUE_x_4:[0-9]+]] x: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_x_4]]))), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_x_5:[0-9]+]] x: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_x_5]])), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3u:[0-9]+]] @test3u(%[[VALUE_x_6:[0-9]+]] x: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_x_6]]))), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_x_7:[0-9]+]] x: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_x_7]])), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4u:[0-9]+]] @test4u(%[[VALUE_x_8:[0-9]+]] x: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_x_8]]))), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5:[0-9]+]] @test5(%[[VALUE_x_9:[0-9]+]] x: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_x_9]])), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5u:[0-9]+]] @test5u(%[[VALUE_x_10:[0-9]+]] x: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_x_10]]))), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6:[0-9]+]] @test6(%[[VALUE_x_11:[0-9]+]] x: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_x_11]])), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6u:[0-9]+]] @test6u(%[[VALUE_x_12:[0-9]+]] x: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_x_12]]))), const<i32>(100)), const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], neg<i32, overflow=ub>(const<i32>(128)));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_c]]), const<i32>(256))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i8) -> i32>(%[[VALUE_test1]], truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_c]]))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u8) -> i32>(%[[VALUE_test1u]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_c]])))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i8) -> i32>(%[[VALUE_test2]], truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_c]]))), const<i32>(1))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u8) -> i32>(%[[VALUE_test2u]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_c]])))), const<i32>(1))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i8) -> i32>(%[[VALUE_test3]], truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_c]]))), const<i32>(1))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u8) -> i32>(%[[VALUE_test3u]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_c]])))), const<i32>(1))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i8) -> i32>(%[[VALUE_test4]], truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_c]]))), const<i32>(1))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u8) -> i32>(%[[VALUE_test4u]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_c]])))), const<i32>(1))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i8) -> i32>(%[[VALUE_test5]], truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_c]]))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u8) -> i32>(%[[VALUE_test5u]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_c]])))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i8) -> i32>(%[[VALUE_test6]], truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_c]]))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u8) -> i32>(%[[VALUE_test6u]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_c]])))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
