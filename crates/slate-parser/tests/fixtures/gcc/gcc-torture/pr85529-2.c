/* PR tree-optimization/85529 */

__attribute__((noipa)) int foo(int x) {
  x &= 63;
  x -= 50;
  x |= 1;
  if (x < 0)
    return 1;
  int y = x >> 2;
  if (x >= y)
    return 1;
  return 0;
}

int main() {
  int i;
  for (i = 0; i < 63; i++)
    if (foo(i) != 1)
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %7: i32 [synthetic] = and<i32>(read<i32>(%6), const<i32>(63));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%7));
// DEFAULT-NEXT:         let %8: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %9: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%8), const<i32>(50));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%9));
// DEFAULT-NEXT:         let %10: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %11: i32 [synthetic] = or<i32>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%11));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         let %2 y: i32 [storage=automatic] = shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%1), const<i32>(2));
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%1), read<i32>(%2))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %5
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), const<i32>(63))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%4)), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
