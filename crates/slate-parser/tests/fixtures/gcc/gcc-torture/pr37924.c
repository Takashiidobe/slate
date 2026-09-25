/* PR c/37924 */

extern void abort(void);

signed char   a;
unsigned char b;

int test1(void) {
  int c = -1;
  return ((unsigned int)(a ^ c)) >> 9;
}

int test2(void) {
  int c = -1;
  return ((unsigned int)(b ^ c)) >> 9;
}

int main(void) {
  a = 0;
  if (test1() != (-1U >> 9))
    abort();
  a = 0x40;
  if (test1() != (-1U >> 9))
    abort();
  a = 0x80;
  if (test1() != (a < 0) ? 0 : (-1U >> 9))
    abort();
  a = 0xff;
  if (test1() != (a < 0) ? 0 : (-1U >> 9))
    abort();
  b = 0;
  if (test2() != (-1U >> 9))
    abort();
  b = 0x40;
  if (test2() != (-1U >> 9))
    abort();
  b = 0x80;
  if (test2() != (-1U >> 9))
    abort();
  b = 0xff;
  if (test2() != (-1U >> 9))
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
// DEFAULT-NEXT:     global %1 a: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @test1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 c: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit, fits=unknown>(xor<i32>(widen<i32, reason=promotion>(read<i8>(%1)), read<i32>(%4))), const<i32>(9)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 c: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%2))), read<i32>(%6))), const<i32>(9)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn() -> i32>(%3)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(neg<u32, overflow=wrap>(const<u32>(1)), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=always>(const<i32>(64)));
// DEFAULT-NEXT:         if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn() -> i32>(%3)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(neg<u32, overflow=wrap>(const<u32>(1)), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=unknown>(const<i32>(128)));
// DEFAULT-NEXT:         if ne<u32>(conditional<u32>(ne<i32>(call<i32, signature=fn() -> i32>(%3), from_bool<i32, reason=promotion>(lt<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(neg<u32, overflow=wrap>(const<u32>(1)), const<i32>(9))), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=unknown>(const<i32>(255)));
// DEFAULT-NEXT:         if ne<u32>(conditional<u32>(ne<i32>(call<i32, signature=fn() -> i32>(%3), from_bool<i32, reason=promotion>(lt<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(neg<u32, overflow=wrap>(const<u32>(1)), const<i32>(9))), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u8>(%2, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn() -> i32>(%5)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(neg<u32, overflow=wrap>(const<u32>(1)), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u8>(%2, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(64))));
// DEFAULT-NEXT:         if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn() -> i32>(%5)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(neg<u32, overflow=wrap>(const<u32>(1)), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u8>(%2, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(128))));
// DEFAULT-NEXT:         if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn() -> i32>(%5)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(neg<u32, overflow=wrap>(const<u32>(1)), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u8>(%2, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(255))));
// DEFAULT-NEXT:         if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn() -> i32>(%5)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(neg<u32, overflow=wrap>(const<u32>(1)), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
