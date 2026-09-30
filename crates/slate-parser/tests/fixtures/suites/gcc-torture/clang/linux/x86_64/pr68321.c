/* PR rtl-optimization/68321 */

int  e = 1, u = 5, t2, t5, i, k;
int  a[1], b, m;
char n, t;

int fn1(int p1) {
  int g[1];
  for (;;) {
    if (p1 / 3)
      for (; t5;)
        u || n;
    t2 = p1 & 4;
    if (b + 1)
      return 0;
    u = g[0];
  }
}

int main() {
  for (; e >= 0; e--) {
    char c;
    if (!m)
      c = t;
    fn1(c);
  }

  if (a[t2] != 0)
    __builtin_abort();

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
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: i32 [storage=static] = const<i32>(5) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_t2:[0-9]+]] t2: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_t5:[0-9]+]] t5: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_m:[0-9]+]] m: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n:[0-9]+]] n: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_t:[0-9]+]] t: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1(%[[VALUE_p1:[0-9]+]] p1: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_p1]]), const<i32>(3)), const<i32>(0))
// DEFAULT-NEXT:                         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                             condition: ne<i32>(read<i32>(%[[VALUE_t5]]), const<i32>(0))
// DEFAULT-NEXT:                             increment: omitted
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_u]]), const<i32>(0)), ne<i8>(read<i8>(%[[VALUE_n]]), const<i8>(0)));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_t2]], and<i32>(read<i32>(%[[VALUE_p1]]), const<i32>(4)));
// DEFAULT-NEXT:                     if ne<i32>(add<i32, overflow=ub>(read<i32>(%[[VALUE_b]]), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:                         return const<i32>(0);
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_u]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_g]]), const<i32>(0)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ge<i32>(read<i32>(%[[VALUE_e]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_c:[0-9]+]] c: i8 [storage=automatic];
// DEFAULT-NEXT:                     if not<bool>(ne<i32>(read<i32>(%[[VALUE_m]]), const<i32>(0)))
// DEFAULT-NEXT:                         write<i8>(%[[VALUE_c]], read<i8>(%[[VALUE_t]]));
// DEFAULT-NEXT:                     call<i32, signature=fn(i32) -> i32>(%[[VALUE_fn1]], widen<i32, reason=arg>(read<i8>(%[[VALUE_c]])));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(%[[VALUE_t2]])))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
