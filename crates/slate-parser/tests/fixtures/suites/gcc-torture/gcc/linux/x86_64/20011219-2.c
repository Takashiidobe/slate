// SLATE-FILECHECK-DEFINES DEFAULT

/* This testcase failed on Alpha at -O2 when simplifying conditional
   expressions.  */

struct S {
  unsigned long a;
  double b, c;
};

extern double bar (double, double);

int
foo (unsigned long x, unsigned int y, struct S *z)
{
  unsigned int a = z->a;
  int b = y / z->a > 1 ? y / z->a : 1;

  a = y / b < z->a ? y / b : z->a;
  z->c = z->b * bar ((double) a, (double) x);
  return 0;
}

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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: u64;
// DEFAULT-NEXT:         field1 b: f64;
// DEFAULT-NEXT:         field2 c: f64;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE0:[0-9]+]] <unnamed>: f64, %[[VALUE1:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: u64, %[[VALUE_y:[0-9]+]] y: u32, %[[VALUE_z:[0-9]+]] z: ptr<@type[[TYPE_S]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(read<u64>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_z]])))));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(conditional<u64>(gt<u64>(div<u64, by_zero=ub>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_y]])), read<u64>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_z]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), div<u64, by_zero=ub>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_y]])), read<u64>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_z]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a]], truncate<u32, reason=assign, fits=unknown>(conditional<u64>(lt<u64>(widen<u64, reason=usual_arith>(div<u32, by_zero=ub>(read<u32>(%[[VALUE_y]]), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_b]])))), read<u64>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_z]]))))), widen<u64, reason=usual_arith>(div<u32, by_zero=ub>(read<u32>(%[[VALUE_y]]), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_b]])))), read<u64>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_z]])))))));
// DEFAULT-NEXT:         write<f64>(field2(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_z]]))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_z]])))), call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_bar]], int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(read<u32>(%[[VALUE_a]])), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(read<u64>(%[[VALUE_x]])))));
// DEFAULT-NEXT:         mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_z]])))), call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_bar]], int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(read<u32>(%[[VALUE_a]])), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(read<u64>(%[[VALUE_x]]))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
