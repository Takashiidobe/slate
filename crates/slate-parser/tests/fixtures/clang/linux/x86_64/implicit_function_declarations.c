int size = sizeof(at_file_scope(1));

int first(void) {
  return later(1, 2) + at_file_scope(3);
}

int second(void) {
  {
    return later(4, 5);
  }
}

int nested(void) {
  return (const) 7 + sizeof(volatile *);
}

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-PREFIX-ARGS C89 --dump-ir -Werror
// SLATE-FILECHECK-DEFINES GNU17
// SLATE-FILECHECK-STD GNU17 gnu17
// SLATE-FILECHECK-PREFIX-ARGS GNU17 --dump-ir -Wno-implicit

// SLATE-FILECHECK-BEGIN C89
// C89: module {
// C89-NEXT:     target "x86_64-unknown-linux-gnu" {
// C89-NEXT:         endian = little;
// C89-NEXT:         pointer [size=8, align=8];
// C89-NEXT:         stack_alignment = 16;
// C89-NEXT:         long_double = f80;
// C89-NEXT:         storage bool [size=1, align=1];
// C89-NEXT:         storage i8, u8 [size=1, align=1];
// C89-NEXT:         storage i16, u16 [size=2, align=2];
// C89-NEXT:         storage i32, u32 [size=4, align=4];
// C89-NEXT:         storage i64, u64 [size=8, align=8];
// C89-NEXT:         storage i128, u128 [size=16, align=16];
// C89-NEXT:         storage bf16 [size=2, align=2];
// C89-NEXT:         storage f16 [size=2, align=2];
// C89-NEXT:         storage f32 [size=4, align=4];
// C89-NEXT:         storage f64 [size=8, align=8];
// C89-NEXT:         storage f80 [size=16, align=16];
// C89-NEXT:         storage f128 [size=16, align=16];
// C89-NEXT:         storage d32 [size=4, align=4];
// C89-NEXT:         storage d64 [size=8, align=8];
// C89-NEXT:         storage d128 [size=16, align=16];
// C89-NEXT:     }
// C89-NEXT:     global %[[VALUE_size:[0-9]+]] size: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(size_of<u64, i32>(4) [c="__size_t"])) [linkage=external];
// C89-NEXT:     fn %[[VALUE_at_file_scope:[0-9]+]] @at_file_scope(unprototyped) -> i32 [linkage=external];
// C89-NEXT:     fn %[[VALUE_later:[0-9]+]] @later(unprototyped) -> i32 [linkage=external];
// C89-NEXT:     fn %[[VALUE_first:[0-9]+]] @first() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C89-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_later]], const<i32>(1), const<i32>(2)), call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_at_file_scope]], const<i32>(3)));
// C89-NEXT:     }
// C89-NEXT:     fn %[[VALUE_second:[0-9]+]] @second() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C89-NEXT:         {
// C89-NEXT:             return call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_later]], const<i32>(4), const<i32>(5));
// C89-NEXT:         }
// C89-NEXT:     }
// C89-NEXT:     fn %[[VALUE_nested:[0-9]+]] @nested() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C89-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))) [c="__size_t"], size_of<u64, ptr<volatile i32>>(8) [c="__size_t"]) [c="__size_t"]));
// C89-NEXT:     }
// C89-NEXT: }
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN GNU17
// GNU17: module {
// GNU17-NEXT:     target "x86_64-unknown-linux-gnu" {
// GNU17-NEXT:         endian = little;
// GNU17-NEXT:         pointer [size=8, align=8];
// GNU17-NEXT:         stack_alignment = 16;
// GNU17-NEXT:         long_double = f80;
// GNU17-NEXT:         storage bool [size=1, align=1];
// GNU17-NEXT:         storage i8, u8 [size=1, align=1];
// GNU17-NEXT:         storage i16, u16 [size=2, align=2];
// GNU17-NEXT:         storage i32, u32 [size=4, align=4];
// GNU17-NEXT:         storage i64, u64 [size=8, align=8];
// GNU17-NEXT:         storage i128, u128 [size=16, align=16];
// GNU17-NEXT:         storage bf16 [size=2, align=2];
// GNU17-NEXT:         storage f16 [size=2, align=2];
// GNU17-NEXT:         storage f32 [size=4, align=4];
// GNU17-NEXT:         storage f64 [size=8, align=8];
// GNU17-NEXT:         storage f80 [size=16, align=16];
// GNU17-NEXT:         storage f128 [size=16, align=16];
// GNU17-NEXT:         storage d32 [size=4, align=4];
// GNU17-NEXT:         storage d64 [size=8, align=8];
// GNU17-NEXT:         storage d128 [size=16, align=16];
// GNU17-NEXT:     }
// GNU17-NEXT:     global %[[VALUE_size:[0-9]+]] size: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(size_of<u64, i32>(4) [c="__size_t"])) [linkage=external];
// GNU17-NEXT:     fn %[[VALUE_at_file_scope:[0-9]+]] @at_file_scope(unprototyped) -> i32 [linkage=external];
// GNU17-NEXT:     fn %[[VALUE_later:[0-9]+]] @later(unprototyped) -> i32 [linkage=external];
// GNU17-NEXT:     fn %[[VALUE_first:[0-9]+]] @first() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// GNU17-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_later]], const<i32>(1), const<i32>(2)), call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_at_file_scope]], const<i32>(3)));
// GNU17-NEXT:     }
// GNU17-NEXT:     fn %[[VALUE_second:[0-9]+]] @second() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// GNU17-NEXT:         {
// GNU17-NEXT:             return call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_later]], const<i32>(4), const<i32>(5));
// GNU17-NEXT:         }
// GNU17-NEXT:     }
// GNU17-NEXT:     fn %[[VALUE_nested:[0-9]+]] @nested() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// GNU17-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))) [c="__size_t"], size_of<u64, ptr<volatile i32>>(8) [c="__size_t"]) [c="__size_t"]));
// GNU17-NEXT:     }
// GNU17-NEXT: }
// SLATE-FILECHECK-END GNU17
