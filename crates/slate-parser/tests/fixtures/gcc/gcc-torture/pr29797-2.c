extern void abort(void);

unsigned long bar(void) { return 32768; }

int main() {
  unsigned long nStyle = bar();
  if (nStyle & 32768)
    nStyle |= 65536;
  if (nStyle != (32768 | 65536))
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
// DEFAULT-NEXT:     fn %1 @bar() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(const<i32>(32768)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 nStyle: u64 [storage=automatic] = call<u64, signature=fn() -> u64>(%1);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(32768)))), const<u64>(0))
// DEFAULT-NEXT:             let %4: u64 [synthetic] = read<u64>(%3);
// DEFAULT-NEXT:             let %5: u64 [synthetic] = or<u64>(read<u64>(%4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(65536))));
// DEFAULT-NEXT:             write<u64>(%3, read<u64>(%5));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(or<i32>(const<i32>(32768), const<i32>(65536)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
