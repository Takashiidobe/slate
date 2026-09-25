/* When comparisons of bit-fields to unsigned constants got shortened,
   the shortened signed constant was wrongly marked as overflowing,
   leading to a later integer_zerop failure and misoptimization.

   Related to bug tree-optimization/16437 but shows the problem on
   32-bit systems.  */
/* Origin: Joseph Myers <jsm@polyomino.org.uk> */

/* { dg-require-effective-target int32plus } */

extern void abort(void);

struct s {
  int a : 12, b : 20;
};

struct s x = {-123, -456};

int main(void) {
  if (x.a != -123U || x.b != -456U)
    abort();
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 a: i32 : 12;
// DEFAULT-NEXT:         field1 b: i32 : 20;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 1], bit_offsets=[Some(0), Some(12)], bit_units=[(0, 4)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     global %2 x: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(123)), field1 = neg<i32, overflow=ub>(const<i32>(456))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%2))), neg<u32, overflow=wrap>(const<u32>(123))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=12..32>(%2))), neg<u32, overflow=wrap>(const<u32>(456))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
