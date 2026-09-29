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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_c]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: ptr<ptr<i32>> [storage=static] = addr_of<ptr<ptr<i32>>>(%[[VALUE_d]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: ptr<ptr<ptr<i32>>> [storage=static] = addr_of<ptr<ptr<ptr<i32>>>>(%[[VALUE_e]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_a]], const<i32>(2));
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_h]], const<i32>(0));
// DEFAULT-NEXT:                     condition: le<i32>(read<i32>(%[[VALUE_h]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_h]]);
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_h]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_b]], const<i32>(0));
// DEFAULT-NEXT:                             condition: le<i32>(read<i32>(%[[VALUE_b]]), const<i32>(2))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:                                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 write<i32>(deref(read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(deref(read<ptr<ptr<ptr<i32>>>>(%[[VALUE_f]])))))), const<i32>(6));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
