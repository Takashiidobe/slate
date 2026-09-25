int bar(int foo) { return (int)(((unsigned long long)(long long)foo) / 8); }
extern void abort(void);
int         main() {
  if (sizeof(long long) > sizeof(int) && bar(-1) != -1)
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
// DEFAULT-NEXT:     fn %0 @bar(%1 foo: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(div<u64, by_zero=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(read<i32>(%1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4: bool [synthetic];
// DEFAULT-NEXT:         if gt<u64>(const<u64>(8), const<u64>(4))
// DEFAULT-NEXT:             write<bool>(%4, ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, neg<i32, overflow=ub>(const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%4, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%4)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
