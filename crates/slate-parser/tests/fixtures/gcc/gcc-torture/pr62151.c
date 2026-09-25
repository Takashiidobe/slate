/* PR rtl-optimization/62151 */

int   a, c, d, e, f, g, h, i;
short b;

int fn1() {
  b = 0;
  for (;;) {
    int j[2];
    j[f] = 0;
    if (h)
      d = 0;
    else {
      for (; f; f++)
        ;
      for (a = 0; a < 1; a++)
        for (;;) {
          i = b & ((b ^ 1) & 83647) ? b : b - 1;
          g = 1 ? i : 0;
          e = j[0];
          if (c)
            break;
          return 0;
        }
    }
  }
}

int main() {
  fn1();
  if (g != -1)
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
// DEFAULT-NEXT:     global %2 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 h: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 b: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %9 @fn1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i16>(%8, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %10 j: array<i32, 2> [storage=automatic];
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%10), read<i32>(%4))), const<i32>(0));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:                         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             for %13
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                 condition: ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %16: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                                     let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%4, read<i32>(%17));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     ;
// DEFAULT-NEXT:                             for %14
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%0, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%0), const<i32>(1))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %18: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                                     let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%0, read<i32>(%19));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     for %15
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                         condition: omitted
// DEFAULT-NEXT:                                         increment: omitted
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 write<i32>(%7, conditional<i32>(ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i16>(%8)), and<i32>(xor<i32>(widen<i32, reason=promotion>(read<i16>(%8)), const<i32>(1)), const<i32>(83647))), const<i32>(0)), widen<i32, reason=promotion>(read<i16>(%8)), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%8)), const<i32>(1))));
// DEFAULT-NEXT:                                                 write<i32>(%5, conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), read<i32>(%7), const<i32>(0)));
// DEFAULT-NEXT:                                                 write<i32>(%3, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%10), const<i32>(0)))));
// DEFAULT-NEXT:                                                 if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:                                                     break %15;
// DEFAULT-NEXT:                                                 return const<i32>(0);
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%9);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
