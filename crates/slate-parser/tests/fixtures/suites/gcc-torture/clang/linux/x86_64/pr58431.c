char         a, h;
int          b, d, e, g, j, k;
volatile int c;
short        i;

int main() {
  int m;

  m = i ^= 1;
  for (b = 0; b < 1; b++) {
    char o = m;
    g      = k;
    j      = j || c;
    if (a != o)
      for (; d < 1; d++)
        ;
    else {
      char *p = &h;
      *p      = 1;
      for (; e; e++)
        ;
    }
  }

  if (h != 0)
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
// DEFAULT-NEXT:     global %0 a: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 h: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 k: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 c: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 i: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %17 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 m: i32 [storage=automatic];
// DEFAULT-NEXT:         let %18: i16 [synthetic] = read<i16>(%9);
// DEFAULT-NEXT:         let %19: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(xor<i32>(widen<i32, reason=promotion>(read<i16>(%18)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%9, read<i16>(%19));
// DEFAULT-NEXT:         write<i32>(%11, widen<i32, reason=assign>(read<i16>(%19)));
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%2), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%21));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %12 o: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(read<i32>(%11));
// DEFAULT-NEXT:                     write<i32>(%5, read<i32>(%7));
// DEFAULT-NEXT:                     write<i32>(%6, from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(read<i32>(%6), const<i32>(0)), ne<i32>(read<i32, volatile>(%8), const<i32>(0)))));
// DEFAULT-NEXT:                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(%0)), widen<i32, reason=promotion>(read<i8>(%12)))
// DEFAULT-NEXT:                         for %15
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                             condition: lt<i32>(read<i32>(%3), const<i32>(1))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %22: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                                 let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%3, read<i32>(%23));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 ;
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %13 p: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%1);
// DEFAULT-NEXT:                             write<i8>(deref(read<ptr<i8>>(%13)), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                             for %16
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                 condition: ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %24: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                                     let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%4, read<i32>(%25));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     ;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
