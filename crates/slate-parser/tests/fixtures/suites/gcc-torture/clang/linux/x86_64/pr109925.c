/* PR tree-optimization/109925 */

int a, c, f;

int main() {
  int g[2];
  for (c = 0; c < 2; c++) {
    {
      char h[20], *b = h;
      int  d = 48, e = 0;
      while (d && e < 5)
        b[e++] = d /= 10;
      f = e;
    }
    g[f - 2 + c] = 0;
  }
  for (;;) {
    for (; a <= 4; a++)
      if (g[0])
        break;
    break;
  }
  if (a != 5)
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %13 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 g: array<i32, 2> [storage=automatic];
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %5 h: array<i8, 20> [storage=automatic] [align=16];
// DEFAULT-NEXT:                         let %6 b: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(20)>(%5);
// DEFAULT-NEXT:                         let %7 d: i32 [storage=automatic] = const<i32>(48);
// DEFAULT-NEXT:                         let %8 e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         while %10 logical_and<bool>(ne<i32>(read<i32>(%7), const<i32>(0)), lt<i32>(read<i32>(%8), const<i32>(5)))
// DEFAULT-NEXT:                             let %16: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                             let %17: i32 [synthetic] = div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%16), const<i32>(10));
// DEFAULT-NEXT:                             write<i32>(%7, read<i32>(%17));
// DEFAULT-NEXT:                             let %18: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                             let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%8, read<i32>(%19));
// DEFAULT-NEXT:                             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%6), read<i32>(%18))), truncate<i8, reason=assign, fits=unknown>(read<i32>(%17)));
// DEFAULT-NEXT:                         write<i32>(%2, read<i32>(%8));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%4), add<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%2), const<i32>(2)), read<i32>(%1)))), const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %12
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: le<i32>(read<i32>(%0), const<i32>(4))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %20: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%21));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%4), const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:                                 break %12;
// DEFAULT-NEXT:                     break %11;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%0), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
