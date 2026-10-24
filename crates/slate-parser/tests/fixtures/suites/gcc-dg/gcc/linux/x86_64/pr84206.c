/* { dg-do compile { target powerpc*-*-* ia64-*-* i?86-*-* x86_64-*-* } } */
/* { dg-options "-fpermissive -O2 -fselective-scheduling -fsel-sched-pipelining -fsel-sched-pipelining-outer-loops -fno-forward-propagate -fno-tree-fre -w" } */

long long unsigned int ao;
int hk;

void
b8 (void)
{
  int *w9;

 c8:
  ao = 0;
  w9 = &ao;
  for (;;)
    for (hk = 0; hk < 1; ++hk)
      for (ao = 0; ao < 4; ++ao)
        {
          int f4 = (ao != 0) ? *w9 : hk;

          if (f4 != 0)
            goto c8;
        }
}

// SLATE-FILECHECK-ARGS -fpermissive
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %[[VALUE_ao:[0-9]+]] ao: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_hk:[0-9]+]] hk: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_b8:[0-9]+]] @b8() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_w9:[0-9]+]] w9: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         label %[[VALUE_c8:[0-9]+]] c8:
// DEFAULT-NEXT:             write<u64>(%[[VALUE_ao]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_w9]], pointer_cast<ptr<i32>, reason=assign>(addr_of<ptr<u64>>(%[[VALUE_ao]])));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_hk]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_hk]]), const<i32>(1))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_hk]]);
// DEFAULT-NEXT:                         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_hk]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<u64>(%[[VALUE_ao]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                             condition: lt<u64>(read<u64>(%[[VALUE_ao]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %[[VALUE5:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_ao]]);
// DEFAULT-NEXT:                                 let %[[VALUE6:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE5]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 write<u64>(%[[VALUE_ao]], read<u64>(%[[VALUE6]]));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %[[VALUE_f4:[0-9]+]] f4: i32 [storage=automatic] = conditional<i32>(ne<u64>(read<u64>(%[[VALUE_ao]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), read<i32>(deref(read<ptr<i32>>(%[[VALUE_w9]]))), read<i32>(%[[VALUE_hk]]));
// DEFAULT-NEXT:                                     if ne<i32>(read<i32>(%[[VALUE_f4]]), const<i32>(0))
// DEFAULT-NEXT:                                         goto %[[VALUE_c8]];
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
