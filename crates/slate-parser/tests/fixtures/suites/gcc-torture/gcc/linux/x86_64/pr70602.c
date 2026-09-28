/* PR tree-optimization/70602 */
/* { dg-require-effective-target int32plus } */

struct __attribute__((packed)) S {
  int s : 1;
  int t : 20;
};

int a, b, c;

int main() {
  for (; a < 1; a++) {
    struct S e[] = {{0, 9}, {0, 9}, {0, 9}, {0, 0}, {0, 9}, {0, 9}, {0, 9},
                    {0, 0}, {0, 9}, {0, 9}, {0, 9}, {0, 0}, {0, 9}, {0, 9},
                    {0, 9}, {0, 0}, {0, 9}, {0, 9}, {0, 9}, {0, 0}, {0, 9}};
    b            = b || e[0].s;
    c            = e[0].t;
  }
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 s: i32 : 1;
// DEFAULT-NEXT:         field1 t: i32 : 20;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0, 0], bit_offsets=[Some(0), Some(1)], bit_units=[(0, 3)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     global %1 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %7: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %8: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%7), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%8));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %5 e: array<@type0, 21> [storage=automatic] [align=16] = aggregate<array<@type0, 21>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(9)), index1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(9)), index2 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(9)), index3 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0)), index4 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(9)), index5 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(9)), index6 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(9)), index7 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0)), index8 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(9)), index9 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(9)), index10 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(9)), index11 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0)), index12 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(9)), index13 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(9)), index14 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(9)), index15 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0)), index16 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(9)), index17 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(9)), index18 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(9)), index19 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0)), index20 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(9)));
// DEFAULT-NEXT:                     write<i32>(%2, from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(read<i32>(%2), const<i32>(0)), ne<i32>(read<i32>(bitfield0<unit=0, bytes=0..3, bits=0..1>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(21)>(%5), const<i32>(0))))), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%3, read<i32>(bitfield1<unit=0, bytes=0..3, bits=1..21>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(21)>(%5), const<i32>(0))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
