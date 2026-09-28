/* { dg-require-alias "" } */
int        a = 1;
extern int b __attribute__((alias("a")));
int        c = 1;
extern int d __attribute__((alias("c")));
int        main(int argc) {
  int *p;
  int *q;
  if (argc)
    p = &a, q = &b;
  else
    p = &c, q = &d;
  *p = 1;
  *q = 2;
  if (*p == 1)
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external] [alias="a"];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %3 d: i32 [storage=static] [linkage=external] [alias="c"];
// DEFAULT-NEXT:     fn %8 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main(%5 argc: i32) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 p: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %7 q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             write<ptr<i32>>(%6, addr_of<ptr<i32>>(%0));
// DEFAULT-NEXT:             write<ptr<i32>>(%7, addr_of<ptr<i32>>(%1));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<ptr<i32>>(%6, addr_of<ptr<i32>>(%2));
// DEFAULT-NEXT:             write<ptr<i32>>(%7, addr_of<ptr<i32>>(%3));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%6)), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%7)), const<i32>(2));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(deref(read<ptr<i32>>(%6))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
