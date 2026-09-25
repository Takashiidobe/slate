/* { dg-require-effective-target int32plus } */

/* PR tree-optimization/78170.
   Check that sign-extended store to a bitfield
   doesn't overwrite other fields.  */

int a, b, d;

struct S0 {
  int f0;
  int f1;
  int f2;
  int f3;
  int f4;
  int f5 : 15;
  int f6 : 17;
  int f7 : 2;
  int f8 : 30;
} c;

void fn1() {
  d = b = 1;
  for (; b; b = a) {
    struct S0 e = {0, 0, 0, 0, 0, 0, 1, 0, 1};
    c           = e;
    c.f6        = -1;
  }
}

int main() {
  fn1();
  if (c.f7 != 0)
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
// DEFAULT-NEXT:     type @type0 S0 = struct {
// DEFAULT-NEXT:         field0 f0: i32;
// DEFAULT-NEXT:         field1 f1: i32;
// DEFAULT-NEXT:         field2 f2: i32;
// DEFAULT-NEXT:         field3 f3: i32;
// DEFAULT-NEXT:         field4 f4: i32;
// DEFAULT-NEXT:         field5 f5: i32 : 15;
// DEFAULT-NEXT:         field6 f6: i32 : 17;
// DEFAULT-NEXT:         field7 f7: i32 : 2;
// DEFAULT-NEXT:         field8 f8: i32 : 30;
// DEFAULT-NEXT:     } [size=28, align=4, offsets=[0, 4, 8, 12, 16, 20, 21, 24, 24], bit_offsets=[None, None, None, None, None, Some(160), Some(175), Some(192), Some(194)], bit_units=[(20, 8)], field_units=[None, None, None, None, None, Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 c: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %5 @fn1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%0));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %6 e: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0), field2 = const<i32>(0), field3 = const<i32>(0), field4 = const<i32>(0), field5 = const<i32>(0), field6 = const<i32>(1), field7 = const<i32>(0), field8 = const<i32>(1));
// DEFAULT-NEXT:                     write<@type0>(%4, copy<@type0, reason=assign>(read<@type0>(%6)));
// DEFAULT-NEXT:                     write<i32>(bitfield6<unit=0, bytes=20..28, bits=15..32>(%4), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(bitfield7<unit=0, bytes=20..28, bits=32..34>(%4)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
