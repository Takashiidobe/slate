/* PR rtl-optimization/85925 */
/* { dg-require-effective-target int32plus } */
/* Testcase by <sudi@gcc.gnu.org> */

int          a, c, d;
volatile int b;
int         *e = &d;

union U1 {
  unsigned f0;
  unsigned f1 : 15;
};
volatile union U1 u = {0x4030201};

int main(void) {
  for (c = 0; c <= 1; c++) {
    union U1 f = {0x4030201};
    if (c == 1)
      b;
    *e = f.f1;
  }

  if (d != u.f1)
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
// DEFAULT-NEXT:     type @type[[TYPE_U1:[0-9]+]] U1 = union {
// DEFAULT-NEXT:         field0 f0: u32;
// DEFAULT-NEXT:         field1 f1: u32 : 15;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[None, Some(0)], bit_units=[(0, 2)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_d]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: volatile @type[[TYPE_U1]] [storage=static] = aggregate<@type[[TYPE_U1]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(67305985))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], const<i32>(0));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_c]]), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_f:[0-9]+]] f: @type[[TYPE_U1]] [storage=automatic] = aggregate<@type[[TYPE_U1]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(67305985)));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%[[VALUE_c]]), const<i32>(1))
// DEFAULT-NEXT:                         read<i32, volatile>(%[[VALUE_b]]);
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%[[VALUE_e]])), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..2, bits=0..15>(%[[VALUE_f]]))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_d]]), reinterpret<i32, reason=promotion, fits=unknown>(read<u32, volatile>(bitfield1<unit=0, bytes=0..2, bits=0..15>(%[[VALUE_u]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
