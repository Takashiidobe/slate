/* PR rtl-optimization/57860 */

extern void abort(void);
int         a, *b = &a, c, d, e, *f = &e, g, *h = &d, k[1] = {1};

int foo(int p) {
  for (;; g++) {
    for (; c; c--)
      ;
    *f = *h = p > ((0x1FFFFFFFFLL ^ a) & *b);
    if (k[g])
      return 0;
  }
}

int main() {
  foo(1);
  if (d != 1)
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
// DEFAULT-NEXT:     global %1 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%1) [linkage=external];
// DEFAULT-NEXT:     global %3 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 f: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%5) [linkage=external];
// DEFAULT-NEXT:     global %7 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 h: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%4) [linkage=external];
// DEFAULT-NEXT:     global %9 k: array<i32, 1> [storage=static] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %10 @foo(%11 p: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%16));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %14
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %17: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                             let %18: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%3, read<i32>(%18));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             ;
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%8)), from_bool<i32, reason=assign>(gt<i64>(widen<i64, reason=usual_arith>(read<i32>(%11)), and<i64>(xor<i64>(const<i64>(8589934591), widen<i64, reason=usual_arith>(read<i32>(%1))), widen<i64, reason=usual_arith>(read<i32>(deref(read<ptr<i32>>(%2))))))));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%6)), from_bool<i32, reason=assign>(gt<i64>(widen<i64, reason=usual_arith>(read<i32>(%11)), and<i64>(xor<i64>(const<i64>(8589934591), widen<i64, reason=usual_arith>(read<i32>(%1))), widen<i64, reason=usual_arith>(read<i32>(deref(read<ptr<i32>>(%2))))))));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%9), read<i32>(%7)))), const<i32>(0))
// DEFAULT-NEXT:                         return const<i32>(0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%10, const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
