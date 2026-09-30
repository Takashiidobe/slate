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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_a]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_e]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_d]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: array<i32, 1> [storage=static] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_g]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_g]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             ;
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: i32 [synthetic] = from_bool<i32, reason=assign>(gt<i64>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_p]])), and<i64>(xor<i64>(const<i64>(8589934591), widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_a]]))), widen<i64, reason=usual_arith>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_b]])))))));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%[[VALUE_h]])), read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%[[VALUE_f]])), read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_k]]), read<i32>(%[[VALUE_g]])))), const<i32>(0))
// DEFAULT-NEXT:                         return const<i32>(0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
