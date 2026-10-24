/* { dg-do compile { target powerpc*-*-* ia64-*-* i?86-*-* x86_64-*-* } } */
/* { dg-options "-std=gnu99 -fpermissive -O2 -fselective-scheduling -fno-if-conversion -fno-tree-dse -w" } */
long long int uo;
unsigned int vt;

void
r5 (long long int h8, long long int pu)
{
  short int wj;
  long long int *mh = h8;

  for (wj = 0; wj < 3; ++wj)
    {
      int oq;
      long long int ns, xf;

      h8 += 2;
      oq = !!h8 && !!wj;
      ++uo;
      vt ^= oq + uo;
      ns = !!uo && !!vt;
      xf = (h8 != 0) ? mh : 1;
      pu += ns < xf;
    }

  for (pu = 0; pu < 1; ++pu)
    {
      int *sc;

      sc = (int *)&pu;
      *sc = 0;
    }
}

// SLATE-FILECHECK-ARGS -fpermissive
// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     global %[[VALUE_uo:[0-9]+]] uo: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_vt:[0-9]+]] vt: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_r5:[0-9]+]] @r5(%[[VALUE_h8:[0-9]+]] h8: i64, %[[VALUE_pu:[0-9]+]] pu: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_wj:[0-9]+]] wj: i16 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mh:[0-9]+]] mh: ptr<i64> [storage=automatic] = int_to_ptr<ptr<i64>, reason=assign>(read<i64>(%[[VALUE_h8]]));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i16>(%[[VALUE_wj]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_wj]])), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_wj]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE1]])), const<i32>(1)));
// DEFAULT-NEXT:                 write<i16>(%[[VALUE_wj]], read<i16>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_oq:[0-9]+]] oq: i32 [storage=automatic];
// DEFAULT-NEXT:                     let %[[VALUE_ns:[0-9]+]] ns: i64 [storage=automatic];
// DEFAULT-NEXT:                     let %[[VALUE_xf:[0-9]+]] xf: i64 [storage=automatic];
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_h8]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE3]]), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_h8]], read<i64>(%[[VALUE4]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_oq]], from_bool<i32, reason=assign>(logical_and<bool>(not<bool>(not<bool>(ne<i64>(read<i64>(%[[VALUE_h8]]), const<i64>(0)))), not<bool>(not<bool>(ne<i16>(read<i16>(%[[VALUE_wj]]), const<i16>(0)))))));
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_uo]]);
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE5]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_uo]], read<i64>(%[[VALUE6]]));
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_vt]]);
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(truncate<i32, reason=assign, fits=unknown>(xor<i64>(reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE7]]))), add<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_oq]])), read<i64>(%[[VALUE_uo]])))));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_vt]], read<u32>(%[[VALUE8]]));
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_ns]], from_bool<i64, reason=assign>(logical_and<bool>(not<bool>(not<bool>(ne<i64>(read<i64>(%[[VALUE_uo]]), const<i64>(0)))), not<bool>(not<bool>(ne<u32>(read<u32>(%[[VALUE_vt]]), const<u32>(0)))))));
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_xf]], ptr_to_int<i64, reason=assign>(conditional<ptr<i64>>(ne<i64>(read<i64>(%[[VALUE_h8]]), widen<i64, reason=usual_arith>(const<i32>(0))), read<ptr<i64>>(%[[VALUE_mh]]), int_to_ptr<ptr<i64>, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                     let %[[VALUE9:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_pu]]);
// DEFAULT-NEXT:                     let %[[VALUE10:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE9]]), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(lt<i64>(read<i64>(%[[VALUE_ns]]), read<i64>(%[[VALUE_xf]])))));
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_pu]], read<i64>(%[[VALUE10]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_pu]], widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<i64>(read<i64>(%[[VALUE_pu]]), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_pu]]);
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE12]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_pu]], read<i64>(%[[VALUE13]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_sc:[0-9]+]] sc: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:                     write<ptr<i32>>(%[[VALUE_sc]], pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<i64>>(%[[VALUE_pu]])));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%[[VALUE_sc]])), const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
