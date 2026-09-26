/* PR rtl-optimization/28970 */
/* Origin: Peter Bergner <bergner@vnet.ibm.com> */

extern void abort(void);

int tar(long i) {
  if (i != 36863)
    abort();

  return -1;
}

void bug(int q, long bcount) {
  int j     = 0;
  int outgo = 0;

  while (j != -1) {
    outgo++;
    if (outgo > q - 1)
      outgo = q - 1;
    j = tar(outgo * bcount);
  }
}

int main(void) {
  bug(5, 36863);
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
// DEFAULT-NEXT:     fn %1 @tar(%2 i: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(36863)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bug(%4 q: i32, %5 bcount: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %7 outgo: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %9 ne<i32>(read<i32>(%6), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%11));
// DEFAULT-NEXT:                 if gt<i32>(read<i32>(%7), sub<i32, overflow=ub>(read<i32>(%4), const<i32>(1)))
// DEFAULT-NEXT:                     write<i32>(%7, sub<i32, overflow=ub>(read<i32>(%4), const<i32>(1)));
// DEFAULT-NEXT:                 write<i32>(%6, call<i32, signature=fn(i64) -> i32>(%1, mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%7)), read<i64>(%5))));
// DEFAULT-NEXT:                 call<i32, signature=fn(i64) -> i32>(%1, mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%7)), read<i64>(%5)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i64) -> void>(%3, const<i32>(5), widen<i64, reason=arg>(const<i32>(36863)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
