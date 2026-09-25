/* Copyright (C) 2004 Free Software Foundation.

   Check that constant folding and RTL simplification of -(x >> y) doesn't
   break anything and produces the expected results.

   Written by Roger Sayle, 11th March 2004.  */

extern void abort(void);

#define INT_BITS (sizeof(int) * 8)

int test1(int x) { return -(x >> (INT_BITS - 1)); }

int test2(unsigned int x) { return -((int)(x >> (INT_BITS - 1))); }

int test3(int x) {
  int y;
  y = INT_BITS - 1;
  return -(x >> y);
}

int test4(unsigned int x) {
  int y;
  y = INT_BITS - 1;
  return -((int)(x >> y));
}

int main() {
  if (test1(0) != 0)
    abort();
  if (test1(1) != 0)
    abort();
  if (test1(-1) != 1)
    abort();

  if (test2(0) != 0)
    abort();
  if (test2(1) != 0)
    abort();
  if (test2((unsigned int)-1) != -1)
    abort();

  if (test3(0) != 0)
    abort();
  if (test3(1) != 0)
    abort();
  if (test3(-1) != 1)
    abort();

  if (test4(0) != 0)
    abort();
  if (test4(1) != 0)
    abort();
  if (test4((unsigned int)-1) != -1)
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
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%2), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @test2(%4 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%4), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test3(%6 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 y: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%7, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%6), read<i32>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test4(%9 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 y: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%10, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%9), read<i32>(%10))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, neg<i32, overflow=ub>(const<i32>(1))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%3, reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%3, reinterpret<u32, reason=arg, fits=always>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%3, reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, neg<i32, overflow=ub>(const<i32>(1))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%8, reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%8, reinterpret<u32, reason=arg, fits=always>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%8, reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
