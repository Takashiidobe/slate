/* PR rtl-optimization/97421 */
/* { dg-additional-options "-fmodulo-sched -fno-dce -fno-strict-aliasing" } */

static int a, b, c;
int       *d = &c;
int      **e = &d;
int     ***f = &e;
int        main() {
  int h;
  for (a = 2; a; a--)
    for (h = 0; h <= 2; h++)
      for (b = 0; b <= 2; b++)
        ***f = 6;

  if (b != 3)
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %3 d: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%2) [linkage=external];
// DEFAULT-NEXT:     global %4 e: ptr<ptr<i32>> [storage=static] = addr_of<ptr<ptr<i32>>>(%3) [linkage=external];
// DEFAULT-NEXT:     global %5 f: ptr<ptr<ptr<i32>>> [storage=static] = addr_of<ptr<ptr<ptr<i32>>>>(%4) [linkage=external];
// DEFAULT-NEXT:     fn %11 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 h: i32 [storage=automatic];
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%0, const<i32>(2));
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%0, read<i32>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %9
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:                     condition: le<i32>(read<i32>(%7), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %14: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                         let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%7, read<i32>(%15));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %10
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(0));
// DEFAULT-NEXT:                             condition: le<i32>(read<i32>(%1), const<i32>(2))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %16: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%1, read<i32>(%17));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 write<i32>(deref(read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(deref(read<ptr<ptr<ptr<i32>>>>(%5)))))), const<i32>(6));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
