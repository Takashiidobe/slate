/* PR middle-end/16790.  */

extern void abort();

static void test1(unsigned int u1) {
  unsigned int y_final_1;
  signed short y_middle;
  unsigned int y_final_2;

  y_final_1 = (unsigned int)((signed short)(u1 * 2) * 3);
  y_middle  = (signed short)(u1 * 2);
  y_final_2 = (unsigned int)(y_middle * 3);

  if (y_final_1 != y_final_2)
    abort();
}

static void test2(unsigned int u1) {
  unsigned int y_final_1;
  signed short y_middle;
  unsigned int y_final_2;

  y_final_1 = (unsigned int)((signed short)(u1 << 1) * 3);
  y_middle  = (signed short)(u1 << 1);
  y_final_2 = (unsigned int)(y_middle * 3);

  if (y_final_1 != y_final_2)
    abort();
}

int main() {
  test1(0x4000U);
  test2(0x4000U);
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
// DEFAULT-NEXT:     fn %1 @test1(%2 u1: u32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 y_final_1: u32 [storage=automatic];
// DEFAULT-NEXT:         let %4 y_middle: i16 [storage=automatic];
// DEFAULT-NEXT:         let %5 y_final_2: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%3, reinterpret<u32, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(mul<u32, overflow=wrap>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))))), const<i32>(3))));
// DEFAULT-NEXT:         write<i16>(%4, reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(mul<u32, overflow=wrap>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))))));
// DEFAULT-NEXT:         write<u32>(%5, reinterpret<u32, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%4)), const<i32>(3))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%3), read<u32>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test2(%7 u1: u32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 y_final_1: u32 [storage=automatic];
// DEFAULT-NEXT:         let %9 y_middle: i16 [storage=automatic];
// DEFAULT-NEXT:         let %10 y_final_2: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%8, reinterpret<u32, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%7), const<i32>(1))))), const<i32>(3))));
// DEFAULT-NEXT:         write<i16>(%9, reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%7), const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%10, reinterpret<u32, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%9)), const<i32>(3))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%8), read<u32>(%10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%1, const<u32>(16384));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%6, const<u32>(16384));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
