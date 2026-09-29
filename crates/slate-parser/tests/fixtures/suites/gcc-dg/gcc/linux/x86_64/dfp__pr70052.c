/* { dg-do compile } */
/* { dg-options "-O1" } */

typedef struct
{
  _Decimal128 td0;
  _Decimal128 td1;
} TDx2_t;


TDx2_t
D256_add_finite (void)
{
  _Decimal128 z, zz;
  TDx2_t result = {0.DL, 0.DL};

  if (zz == 0.DL)
  {
    result.td0 = z;
    return result;
  }

  return result;
}

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 td0: d128;
// DEFAULT-NEXT:         field1 td1: d128;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_TDx2_t:[0-9]+]] TDx2_t = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_D256_add_finite:[0-9]+]] @D256_add_finite() -> @type[[TYPE0]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: d128 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_zz:[0-9]+]] zz: d128 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: @type[[TYPE0]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<d128>(0.), field1 = const<d128>(0.));
// DEFAULT-NEXT:         if eq<d128, exceptions=observable>(read<d128>(%[[VALUE_zz]]), const<d128>(0.))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<d128>(field0(%[[VALUE_result]]), read<d128>(%[[VALUE_z]]));
// DEFAULT-NEXT:                 return copy<@type[[TYPE0]], reason=return>(read<@type[[TYPE0]]>(%[[VALUE_result]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return copy<@type[[TYPE0]], reason=return>(read<@type[[TYPE0]]>(%[[VALUE_result]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
