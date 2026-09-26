extern void abort(void);
extern void exit(int);

int main(void) {
  int   a, b, c;
  int  *pa, *pb, *pc;
  int **ppa, **ppb, **ppc;
  int   i, j, k, x, y, z;

  a   = 10;
  b   = 20;
  c   = 30;
  pa  = &a;
  pb  = &b;
  pc  = &c;
  ppa = &pa;
  ppb = &pb;
  ppc = &pc;
  x   = 0;
  y   = 0;
  z   = 0;

  for (i = 0; i < 10; i++) {
    if (pa == &a)
      pa = &b;
    else
      pa = &a;
    while ((*pa)--) {
      x++;
      if ((*pa) < 3)
        break;
      else
        pa = &b;
    }
    x++;
    pa = &b;
  }

  if ((*pa) != -5 || (*pb) != -5 || x != 43)
    abort();

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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%18 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %4 b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %5 c: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 pa: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %7 pb: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %8 pc: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %9 ppa: ptr<ptr<i32>> [storage=automatic];
// DEFAULT-NEXT:         let %10 ppb: ptr<ptr<i32>> [storage=automatic];
// DEFAULT-NEXT:         let %11 ppc: ptr<ptr<i32>> [storage=automatic];
// DEFAULT-NEXT:         let %12 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %14 k: i32 [storage=automatic];
// DEFAULT-NEXT:         let %15 x: i32 [storage=automatic];
// DEFAULT-NEXT:         let %16 y: i32 [storage=automatic];
// DEFAULT-NEXT:         let %17 z: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(10));
// DEFAULT-NEXT:         write<i32>(%4, const<i32>(20));
// DEFAULT-NEXT:         write<i32>(%5, const<i32>(30));
// DEFAULT-NEXT:         write<ptr<i32>>(%6, addr_of<ptr<i32>>(%3));
// DEFAULT-NEXT:         write<ptr<i32>>(%7, addr_of<ptr<i32>>(%4));
// DEFAULT-NEXT:         write<ptr<i32>>(%8, addr_of<ptr<i32>>(%5));
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%9, addr_of<ptr<ptr<i32>>>(%6));
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%10, addr_of<ptr<ptr<i32>>>(%7));
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%11, addr_of<ptr<ptr<i32>>>(%8));
// DEFAULT-NEXT:         write<i32>(%15, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%17, const<i32>(0));
// DEFAULT-NEXT:         for %19
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%12, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%12), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%22));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if eq<ptr<i32>>(read<ptr<i32>>(%6), addr_of<ptr<i32>>(%3))
// DEFAULT-NEXT:                         write<ptr<i32>>(%6, addr_of<ptr<i32>>(%4));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<ptr<i32>>(%6, addr_of<ptr<i32>>(%3));
// DEFAULT-NEXT:                     while %20 {
// DEFAULT-NEXT:                         let %23: ptr<i32> [synthetic] = read<ptr<i32>>(%6);
// DEFAULT-NEXT:                         let %24: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%23)));
// DEFAULT-NEXT:                         let %25: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(deref(read<ptr<i32>>(%23)), read<i32>(%25));
// DEFAULT-NEXT:                         yield ne<i32>(read<i32>(%24), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %26: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                             let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%15, read<i32>(%27));
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(deref(read<ptr<i32>>(%6))), const<i32>(3))
// DEFAULT-NEXT:                                 break %20;
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<ptr<i32>>(%6, addr_of<ptr<i32>>(%4));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     let %28: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                     let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%15, read<i32>(%29));
// DEFAULT-NEXT:                     write<ptr<i32>>(%6, addr_of<ptr<i32>>(%4));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(read<ptr<i32>>(%6))), neg<i32, overflow=ub>(const<i32>(5))), ne<i32>(read<i32>(deref(read<ptr<i32>>(%7))), neg<i32, overflow=ub>(const<i32>(5)))), ne<i32>(read<i32>(%15), const<i32>(43)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
