void exit(int);

int a, b, c, d = 1, e;

static signed char foo() {
  int f, g = a;

  for (f = 1; f < 3; f++)
    for (; b < 1; b++) {
      if (d)
        for (c = 0; c < 4; c++)
          for (f = 0; f < 3; f++) {
            for (e = 0; e < 1; e++)
              a = g;
            if (f)
              break;
          }
      else if (f)
        continue;
      return 0;
    }
  return 0;
}

int main() {
  foo();
  exit(0);
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
// DEFAULT-NEXT:     global %1 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %5 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%10 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @foo() -> i8 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 f: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 g: i32 [storage=automatic] = read<i32>(%1);
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %12
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%2), const<i32>(1))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %18: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                         let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%2, read<i32>(%19));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:                                 for %13
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:                                     condition: lt<i32>(read<i32>(%3), const<i32>(4))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %20: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                                         let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%3, read<i32>(%21));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         for %14
// DEFAULT-NEXT:                                             init:
// DEFAULT-NEXT:                                                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:                                             condition: lt<i32>(read<i32>(%7), const<i32>(3))
// DEFAULT-NEXT:                                             increment: {
// DEFAULT-NEXT:                                                 let %22: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                                                 let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                                                 write<i32>(%7, read<i32>(%23));
// DEFAULT-NEXT:                                                 yield void;
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                             body:
// DEFAULT-NEXT:                                                 {
// DEFAULT-NEXT:                                                     for %15
// DEFAULT-NEXT:                                                         init:
// DEFAULT-NEXT:                                                             write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:                                                         condition: lt<i32>(read<i32>(%5), const<i32>(1))
// DEFAULT-NEXT:                                                         increment: {
// DEFAULT-NEXT:                                                             let %24: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                                                             let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                                                             write<i32>(%5, read<i32>(%25));
// DEFAULT-NEXT:                                                             yield void;
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         body:
// DEFAULT-NEXT:                                                             write<i32>(%1, read<i32>(%8));
// DEFAULT-NEXT:                                                     if ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:                                                         break %14;
// DEFAULT-NEXT:                                                 }
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:                                     continue %12;
// DEFAULT-NEXT:                             return truncate<i8, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i8, signature=fn() -> i8>(%6);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
