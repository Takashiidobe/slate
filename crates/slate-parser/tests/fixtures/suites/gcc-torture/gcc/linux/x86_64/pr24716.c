/* PR24716, scalar evolution returning the wrong result
   for pdest.  */

int Link[] = {-1};
int W[]    = {2};

extern void abort(void);

int f(int k, int p) {
  int pdest, j, D1361;
  j     = 0;
  pdest = 0;
  for (;;) {
    if (pdest > 2)
      do
        j--, pdest++;
      while (j > 2);

    if (j == 1)
      break;

    while (pdest > p)
      if (j == p)
        pdest++;

    do {
      D1361 = W[k];
      do
        if (D1361 != 0)
          pdest = 1, W[k] = D1361 = 0;
      while (p < 1);
    } while (k > 0);

    do {
      p = 0;
      k = Link[k];
      while (p < j)
        if (k != -1)
          pdest++, p++;
    } while (k != -1);
    j = 1;
  }

  /* The correct return value should be pdest (1 in the call from main).
     DOM3 is mistaken and propagates a 0 here.  */
  return pdest;
}

int main() {
  if (!f(0, 2))
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
// DEFAULT-NEXT:     global %[[VALUE_Link:[0-9]+]] Link: array<i32, 1> [storage=static] = aggregate<array<i32, 1>, zero_fill=false>(index0 = neg<i32, overflow=ub>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_W:[0-9]+]] W: array<i32, 1> [storage=static] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_k:[0-9]+]] k: i32, %[[VALUE_p:[0-9]+]] p: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_pdest:[0-9]+]] pdest: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_D1361:[0-9]+]] D1361: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_pdest]], const<i32>(0));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if gt<i32>(read<i32>(%[[VALUE_pdest]]), const<i32>(2))
// DEFAULT-NEXT:                         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:                             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_pdest]]);
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_pdest]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                         while gt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(2));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%[[VALUE_j]]), const<i32>(1))
// DEFAULT-NEXT:                         break %[[VALUE0]];
// DEFAULT-NEXT:                     while %[[VALUE6:[0-9]+]] gt<i32>(read<i32>(%[[VALUE_pdest]]), read<i32>(%[[VALUE_p]]))
// DEFAULT-NEXT:                         if eq<i32>(read<i32>(%[[VALUE_j]]), read<i32>(%[[VALUE_p]]))
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_pdest]]);
// DEFAULT-NEXT:                             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_pdest]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                     do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_D1361]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_W]]), read<i32>(%[[VALUE_k]])))));
// DEFAULT-NEXT:                             do %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE_D1361]]), const<i32>(0))
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_pdest]], const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_D1361]], const<i32>(0));
// DEFAULT-NEXT:                                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_W]]), read<i32>(%[[VALUE_k]]))), const<i32>(0));
// DEFAULT-NEXT:                             while lt<i32>(read<i32>(%[[VALUE_p]]), const<i32>(1));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     while gt<i32>(read<i32>(%[[VALUE_k]]), const<i32>(0));
// DEFAULT-NEXT:                     do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_p]], const<i32>(0));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_k]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_Link]]), read<i32>(%[[VALUE_k]])))));
// DEFAULT-NEXT:                             while %[[VALUE12:[0-9]+]] lt<i32>(read<i32>(%[[VALUE_p]]), read<i32>(%[[VALUE_j]]))
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE_k]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:                                     let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_pdest]]);
// DEFAULT-NEXT:                                     let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_pdest]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:                                     let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_p]]);
// DEFAULT-NEXT:                                     let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE15]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_p]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     while ne<i32>(read<i32>(%[[VALUE_k]]), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_j]], const<i32>(1));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_pdest]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_f]], const<i32>(0), const<i32>(2)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
