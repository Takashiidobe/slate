/* PR c/54391 - transparent_union typedef'ing inconsistent
   { dg-do compile }
   { dg-options "-Wall" } */

typedef union m30_u m30_t;

union __attribute__((transparent_union)) m30_u {
  int u;
};

double make_double (m30_t);

double f (void)
{
  int bar = 17;
  return make_double (bar);
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
// DEFAULT-NEXT:     type @type[[TYPE_m30_u:[0-9]+]] m30_u = union {
// DEFAULT-NEXT:         field0 u: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_m30_t:[0-9]+]] m30_t = @type[[TYPE_m30_u]];
// DEFAULT-NEXT:     fn %[[VALUE_make_double:[0-9]+]] @make_double(%[[VALUE0:[0-9]+]] <unnamed>: @type[[TYPE_m30_u]]) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_bar:[0-9]+]] bar: i32 [storage=automatic] = const<i32>(17);
// DEFAULT-NEXT:         return call<f64, signature=fn(@type[[TYPE_m30_u]]) -> f64>(%[[VALUE_make_double]], aggregate<@type[[TYPE_m30_u]], zero_fill=false>(field0 = read<i32>(%[[VALUE_bar]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
