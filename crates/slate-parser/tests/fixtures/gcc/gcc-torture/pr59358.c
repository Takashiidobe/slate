/* PR tree-optimization/59358 */

__attribute__((noinline, noclone)) int foo(int *x, int y) {
  int z = *x;
  if (y > z && y <= 16)
    while (y > z)
      z *= 2;
  return z;
}

int main() {
  int i;
  for (i = 1; i < 17; i++) {
    int j = foo(&i, 16);
    int k;
    if (i >= 8 && i <= 15)
      k = 16 + (i - 8) * 2;
    else if (i >= 4 && i <= 7)
      k = 16 + (i - 4) * 4;
    else if (i == 3)
      k = 24;
    else
      k = 16;
    if (j != k)
      __builtin_abort();
    j = foo(&i, 7);
    if (i >= 7)
      k = i;
    else if (i >= 4)
      k = 8 + (i - 4) * 2;
    else if (i == 3)
      k = 12;
    else
      k = 8;
    if (j != k)
      __builtin_abort();
  }
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: ptr<i32>, %2 y: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 z: i32 [storage=automatic] = read<i32>(deref(read<ptr<i32>>(%1)));
// DEFAULT-NEXT:         if logical_and<bool>(gt<i32>(read<i32>(%2), read<i32>(%3)), le<i32>(read<i32>(%2), const<i32>(16)))
// DEFAULT-NEXT:             while %8 gt<i32>(read<i32>(%2), read<i32>(%3))
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%10), const<i32>(2));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%11));
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(1));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), const<i32>(17))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %6 j: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>, i32) -> i32>(%0, addr_of<ptr<i32>>(%5), const<i32>(16));
// DEFAULT-NEXT:                     let %7 k: i32 [storage=automatic];
// DEFAULT-NEXT:                     if logical_and<bool>(ge<i32>(read<i32>(%5), const<i32>(8)), le<i32>(read<i32>(%5), const<i32>(15)))
// DEFAULT-NEXT:                         write<i32>(%7, add<i32, overflow=ub>(const<i32>(16), mul<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%5), const<i32>(8)), const<i32>(2))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if logical_and<bool>(ge<i32>(read<i32>(%5), const<i32>(4)), le<i32>(read<i32>(%5), const<i32>(7)))
// DEFAULT-NEXT:                             write<i32>(%7, add<i32, overflow=ub>(const<i32>(16), mul<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%5), const<i32>(4)), const<i32>(4))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(%5), const<i32>(3))
// DEFAULT-NEXT:                                 write<i32>(%7, const<i32>(24));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<i32>(%7, const<i32>(16));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%6), read<i32>(%7))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     write<i32>(%6, call<i32, signature=fn(ptr<i32>, i32) -> i32>(%0, addr_of<ptr<i32>>(%5), const<i32>(7)));
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<i32>, i32) -> i32>(%0, addr_of<ptr<i32>>(%5), const<i32>(7));
// DEFAULT-NEXT:                     if ge<i32>(read<i32>(%5), const<i32>(7))
// DEFAULT-NEXT:                         write<i32>(%7, read<i32>(%5));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if ge<i32>(read<i32>(%5), const<i32>(4))
// DEFAULT-NEXT:                             write<i32>(%7, add<i32, overflow=ub>(const<i32>(8), mul<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%5), const<i32>(4)), const<i32>(2))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(%5), const<i32>(3))
// DEFAULT-NEXT:                                 write<i32>(%7, const<i32>(12));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<i32>(%7, const<i32>(8));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%6), read<i32>(%7))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
