/* PR regression/21897 */
/* This testcase generates MMX instructions together with x87 instructions.
   Currently, there is no "emms" generated to switch between register sets,
   so the testcase fails for targets where MMX insns are enabled.  */
/* { dg-options "-mno-mmx" { target { x86_64-*-* i?86-*-* } } } */

extern void abort(void);

typedef unsigned short v4hi __attribute__((vector_size(8)));
typedef float          v4sf __attribute__((vector_size(16)));

union {
  v4hi  v;
  short s[4];
} u;

union {
  v4sf  v;
  float f[4];
} v;

void foo(void) {
  unsigned int i;
  for (i = 0; i < 2; i++)
    u.v += (v4hi){12, 32768};
  for (i = 0; i < 2; i++)
    v.v += (v4sf){18.0, 20.0, 22};
}

int main(void) {
  foo();
  if (u.s[0] != 24 || u.s[1] != 0 || u.s[2] || u.s[3])
    abort();
  if (v.f[0] != 36.0 || v.f[1] != 40.0 || v.f[2] != 44.0 || v.f[3] != 0.0)
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
// DEFAULT-NEXT:     type @type[[TYPE_v4hi:[0-9]+]] v4hi = vector<u16, 4>;
// DEFAULT-NEXT:     type @type[[TYPE_v4sf:[0-9]+]] v4sf = vector<f32, 4>;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 v: vector<u16, 4>;
// DEFAULT-NEXT:         field1 s: array<i16, 4>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 v: vector<f32, 4>;
// DEFAULT-NEXT:         field1 f: array<f32, 4>;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: @type[[TYPE1]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: u32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%[[VALUE_i]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE1]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i]], read<u32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: vector<u16, 4> [synthetic] = read<vector<u16, 4>>(field0(%[[VALUE_u]]));
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: vector<u16, 4> [synthetic] = add<vector<u16, 4>, elementwise=true, overflow=wrap>(read<vector<u16, 4>>(%[[VALUE3]]), read<vector<u16, 4>>(compound_literal %[[VALUE5:[0-9]+]] [storage=automatic] = aggregate<vector<u16, 4>, zero_fill=true>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(12))), index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(const<i32>(32768))))));
// DEFAULT-NEXT:                 write<vector<u16, 4>>(field0(%[[VALUE_u]]), read<vector<u16, 4>>(%[[VALUE4]]));
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%[[VALUE_i]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE7]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i]], read<u32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: vector<f32, 4> [synthetic] = read<vector<f32, 4>>(field0(%[[VALUE_v]]));
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: vector<f32, 4> [synthetic] = add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE9]]), read<vector<f32, 4>>(compound_literal %[[VALUE11:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=true>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(18.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(20.0)), index2 = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(22)))));
// DEFAULT-NEXT:                 write<vector<f32, 4>>(field0(%[[VALUE_v]]), read<vector<f32, 4>>(%[[VALUE10]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(4)>(field1(%[[VALUE_u]])), const<i32>(0))))), const<i32>(24)), ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(4)>(field1(%[[VALUE_u]])), const<i32>(1))))), const<i32>(0))), ne<i16>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(4)>(field1(%[[VALUE_u]])), const<i32>(2)))), const<i16>(0))), ne<i16>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(4)>(field1(%[[VALUE_u]])), const<i32>(3)))), const<i16>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>,
// DEFAULT-SAME: length=Some(4)>(field1(%[[VALUE_v]])), const<i32>(0))))), const<f64>(36.0)), ne<f64, exceptions=ignore>(float_widen<f64,
// DEFAULT-SAME: reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(4)>(field1(%[[VALUE_v]])),
// DEFAULT-SAME: const<i32>(1))))), const<f64>(40.0))), ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>,
// DEFAULT-SAME: length=Some(4)>(field1(%[[VALUE_v]])), const<i32>(2))))), const<f64>(44.0))), ne<f64, exceptions=ignore>(float_widen<f64,
// DEFAULT-SAME: reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(4)>(field1(%[[VALUE_v]])),
// DEFAULT-SAME: const<i32>(3))))), const<f64>(0.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
