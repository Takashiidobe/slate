void abort(void);
void exit(int);
long f(short a, short b) { return (long)a / b; }
int  main(void) {
  if (f(-32768, -1) != 32768L)
    abort();
  else
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
// DEFAULT-NEXT:     fn %1 @exit(%6 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @f(%3 a: i16, %4 b: i16) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<i64, by_zero=ub, min_by_neg_one=ub>(widen<i64, reason=explicit>(read<i16>(%3)), widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(read<i16>(%4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i16, i16) -> i64>(%2, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(32768))), truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), const<i64>(32768))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
