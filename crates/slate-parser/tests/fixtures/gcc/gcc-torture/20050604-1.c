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
// DEFAULT-NEXT:     type @type0 v4hi = vector<u16, 4>;
// DEFAULT-NEXT:     type @type1 v4sf = vector<f32, 4>;
// DEFAULT-NEXT:     type @type2 = union {
// DEFAULT-NEXT:         field0 v: vector<u16, 4>;
// DEFAULT-NEXT:         field1 s: array<i16, 4>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type3 = union {
// DEFAULT-NEXT:         field0 v: vector<f32, 4>;
// DEFAULT-NEXT:         field1 f: array<f32, 4>;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     global %4 u: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 v: @type3 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 i: u32 [storage=automatic];
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%8, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%8), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: u32 [synthetic] = read<u32>(%8);
// DEFAULT-NEXT:                 let %15: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%14), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%8, read<u32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %16: vector<u16, 4> [synthetic] = read<vector<u16, 4>>(field0(%4));
// DEFAULT-NEXT:                 let %17: vector<u16, 4> [synthetic] = add<vector<u16, 4>, elementwise=true, overflow=wrap>(read<vector<u16, 4>>(%16), read<vector<u16, 4>>(compound_literal %11 [storage=automatic] = aggregate<vector<u16, 4>, zero_fill=true>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(12))), index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(const<i32>(32768))))));
// DEFAULT-NEXT:                 write<vector<u16, 4>>(field0(%4), read<vector<u16, 4>>(%17));
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%8, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%8), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %18: u32 [synthetic] = read<u32>(%8);
// DEFAULT-NEXT:                 let %19: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%18), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%8, read<u32>(%19));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %20: vector<f32, 4> [synthetic] = read<vector<f32, 4>>(field0(%6));
// DEFAULT-NEXT:                 let %21: vector<f32, 4> [synthetic] = add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%20), read<vector<f32, 4>>(compound_literal %13 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=true>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(18.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(20.0)), index2 = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(22)))));
// DEFAULT-NEXT:                 write<vector<f32, 4>>(field0(%6), read<vector<f32, 4>>(%21));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(4)>(field1(%4)), const<i32>(0))))), const<i32>(24)), ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(4)>(field1(%4)), const<i32>(1))))), const<i32>(0))), ne<i16>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(4)>(field1(%4)), const<i32>(2)))), const<i16>(0))), ne<i16>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(4)>(field1(%4)), const<i32>(3)))), const<i16>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(4)>(field1(%6)), const<i32>(0))))), const<f64>(36.0)), ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(4)>(field1(%6)), const<i32>(1))))), const<f64>(40.0))), ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(4)>(field1(%6)), const<i32>(2))))), const<f64>(44.0))), ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(4)>(field1(%6)), const<i32>(3))))), const<f64>(0.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
