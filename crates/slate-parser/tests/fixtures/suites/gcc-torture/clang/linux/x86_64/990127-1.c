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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_pa:[0-9]+]] pa: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_pb:[0-9]+]] pb: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_pc:[0-9]+]] pc: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ppa:[0-9]+]] ppa: ptr<ptr<i32>> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ppb:[0-9]+]] ppb: ptr<ptr<i32>> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ppc:[0-9]+]] ppc: ptr<ptr<i32>> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], const<i32>(10));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], const<i32>(20));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c]], const<i32>(30));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_pa]], addr_of<ptr<i32>>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_pb]], addr_of<ptr<i32>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_pc]], addr_of<ptr<i32>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%[[VALUE_ppa]], addr_of<ptr<ptr<i32>>>(%[[VALUE_pa]]));
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%[[VALUE_ppb]], addr_of<ptr<ptr<i32>>>(%[[VALUE_pb]]));
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%[[VALUE_ppc]], addr_of<ptr<ptr<i32>>>(%[[VALUE_pc]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_y]], const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_z]], const<i32>(0));
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_pa]]), addr_of<ptr<i32>>(%[[VALUE_a]]))
// DEFAULT-NEXT:                         write<ptr<i32>>(%[[VALUE_pa]], addr_of<ptr<i32>>(%[[VALUE_b]]));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<ptr<i32>>(%[[VALUE_pa]], addr_of<ptr<i32>>(%[[VALUE_a]]));
// DEFAULT-NEXT:                     while %[[VALUE4:[0-9]+]] {
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_pa]]);
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE5]])));
// DEFAULT-NEXT:                         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(deref(read<ptr<i32>>(%[[VALUE5]])), read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                         yield ne<i32>(read<i32>(%[[VALUE6]]), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:                             let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_pa]]))), const<i32>(3))
// DEFAULT-NEXT:                                 break %[[VALUE4]];
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<ptr<i32>>(%[[VALUE_pa]], addr_of<ptr<i32>>(%[[VALUE_b]]));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:                     let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                     write<ptr<i32>>(%[[VALUE_pa]], addr_of<ptr<i32>>(%[[VALUE_b]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_pa]]))), neg<i32, overflow=ub>(const<i32>(5))), ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_pb]]))), neg<i32, overflow=ub>(const<i32>(5)))), ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(43)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
