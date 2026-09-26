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
// DEFAULT-NEXT:     global %0 Link: array<i32, 1> [storage=static] = aggregate<array<i32, 1>, zero_fill=false>(index0 = neg<i32, overflow=ub>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %1 W: array<i32, 1> [storage=static] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @f(%4 k: i32, %5 p: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 pdest: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 D1361: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if gt<i32>(read<i32>(%6), const<i32>(2))
// DEFAULT-NEXT:                         do %11
// DEFAULT-NEXT:                             let %17: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                             let %18: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%7, read<i32>(%18));
// DEFAULT-NEXT:                             let %19: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                             let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%6, read<i32>(%20));
// DEFAULT-NEXT:                         while gt<i32>(read<i32>(%7), const<i32>(2));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%7), const<i32>(1))
// DEFAULT-NEXT:                         break %10;
// DEFAULT-NEXT:                     while %12 gt<i32>(read<i32>(%6), read<i32>(%5))
// DEFAULT-NEXT:                         if eq<i32>(read<i32>(%7), read<i32>(%5))
// DEFAULT-NEXT:                             let %21: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                             let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%6, read<i32>(%22));
// DEFAULT-NEXT:                     do %13
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%8, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(%4)))));
// DEFAULT-NEXT:                             do %14
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:                                     write<i32>(%6, const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:                                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(%4))), const<i32>(0));
// DEFAULT-NEXT:                             while lt<i32>(read<i32>(%5), const<i32>(1));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     while gt<i32>(read<i32>(%4), const<i32>(0));
// DEFAULT-NEXT:                     do %15
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:                             write<i32>(%4, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%0), read<i32>(%4)))));
// DEFAULT-NEXT:                             while %16 lt<i32>(read<i32>(%5), read<i32>(%7))
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%4), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:                                     let %23: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                                     let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%6, read<i32>(%24));
// DEFAULT-NEXT:                                     let %25: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                                     let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%5, read<i32>(%26));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     while ne<i32>(read<i32>(%4), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                     write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%3, const<i32>(0), const<i32>(2)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
