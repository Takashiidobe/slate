/* PR tree-optimization/120677 */
/* { dg-do run { target int32plus } } */

unsigned a;
int      b, e;

int foo(int d) {
  switch (d) {
  case 0:
  case 2:
    return 0;
  default:
    return 1;
  }
}

int main() {
  for (b = 8; b; b--)
    if (a & 1)
      a = a >> 1 ^ 20000000;
    else
      a >>= 1;
  e = foo(0);
  if (e || a)
    __builtin_abort();
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
// DEFAULT-NEXT:     global %0 a: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 d: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %6 read<i32>(%4)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %6 const<i32>(0):
// DEFAULT-NEXT:                     case %6 const<i32>(2):
// DEFAULT-NEXT:                         return const<i32>(0);
// DEFAULT-NEXT:                 default %6:
// DEFAULT-NEXT:                     return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %7
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1, const<i32>(8));
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %8: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%9));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(and<u32>(read<u32>(%0), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), const<u32>(0))
// DEFAULT-NEXT:                     write<u32>(%0, xor<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%0), const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(20000000))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     let %10: u32 [synthetic] = read<u32>(%0);
// DEFAULT-NEXT:                     let %11: u32 [synthetic] = shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%10), const<i32>(1));
// DEFAULT-NEXT:                     write<u32>(%0, read<u32>(%11));
// DEFAULT-NEXT:         write<i32>(%2, call<i32, signature=fn(i32) -> i32>(%3, const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%3, const<i32>(0));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%2), const<i32>(0)), ne<u32>(read<u32>(%0), const<u32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
