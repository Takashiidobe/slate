/* This testcase generates MMX instructions together with x87 instructions.
   Currently, there is no "emms" generated to switch between register sets,
   so the testcase fails for targets where MMX insns are enabled.  */
/* { dg-options "-mno-mmx -Wno-psabi" { target { x86_64-*-* i?86-*-* } } } */

extern void abort(void);

typedef int          V2SI __attribute__((vector_size(8)));
typedef unsigned int V2USI __attribute__((vector_size(8)));
typedef float        V2SF __attribute__((vector_size(8)));
typedef short        V2HI __attribute__((vector_size(4)));
typedef unsigned int V2UHI __attribute__((vector_size(4)));

long long test1(V2SF x) { return (long long)(V2SI)x; }

long long test2(V2SF x) { return (long long)x; }

long long test3(V2SI x) { return (long long)(V2SF)x; }

int main(void) {
  if (sizeof(short) != 2 || sizeof(int) != 4 || sizeof(long long) != 8)
    return 0;

  V2SF x = {2.0, 2.0};
  union {
    long long l;
    float     f[2];
    int       i[2];
  } u;
  u.l = test1(x);
  if (u.f[0] != 2.0 || u.f[1] != 2.0)
    abort();

  V2SF y = {6.0, 6.0};
  u.l    = test2(y);
  if (u.f[0] != 6.0 || u.f[1] != 6.0)
    abort();

  V2SI z = {4, 4};
  u.l    = test3(z);
  if (u.i[0] != 4 || u.i[1] != 4)
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
// DEFAULT-NEXT:     type @type0 V2SI = vector<i32, 2>;
// DEFAULT-NEXT:     type @type1 V2USI = vector<u32, 2>;
// DEFAULT-NEXT:     type @type2 V2SF = vector<f32, 2>;
// DEFAULT-NEXT:     type @type3 V2HI = vector<i16, 2>;
// DEFAULT-NEXT:     type @type4 V2UHI = vector<u32, 1>;
// DEFAULT-NEXT:     type @type5 = union {
// DEFAULT-NEXT:         field0 l: i64;
// DEFAULT-NEXT:         field1 f: array<f32, 2>;
// DEFAULT-NEXT:         field2 i: array<i32, 2>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @test1(%7 x: vector<f32, 2>) -> i64 [linkage=external] [abi=sysv64(coerce<f64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return vector_bit_cast<i64, reason=explicit>(vector_bit_cast<vector<i32, 2>, reason=explicit>(read<vector<f32, 2>>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test2(%9 x: vector<f32, 2>) -> i64 [linkage=external] [abi=sysv64(coerce<f64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return vector_bit_cast<i64, reason=explicit>(read<vector<f32, 2>>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test3(%11 x: vector<i32, 2>) -> i64 [linkage=external] [abi=sysv64(coerce<f64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return vector_bit_cast<i64, reason=explicit>(vector_bit_cast<vector<f32, 2>, reason=explicit>(read<vector<i32, 2>>(%11)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u64>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), ne<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), ne<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         let %13 x: vector<f32, 2> [storage=automatic] = aggregate<vector<f32, 2>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)));
// DEFAULT-NEXT:         let %15 u: @type5 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(field0(%15), call<i64, signature=fn(vector<f32, 2>) -> i64, abi=sysv64(coerce<f64>) -> scalar>(%6, read<vector<f32, 2>>(%13)));
// DEFAULT-NEXT:         call<i64, signature=fn(vector<f32, 2>) -> i64, abi=sysv64(coerce<f64>) -> scalar>(%6, read<vector<f32, 2>>(%13));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f64, exceptions=observable>(float_widen<f64, reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(field1(%15)), const<i32>(0))))), const<f64>(2.0)), ne<f64, exceptions=observable>(float_widen<f64, reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(field1(%15)), const<i32>(1))))), const<f64>(2.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %16 y: vector<f32, 2> [storage=automatic] = aggregate<vector<f32, 2>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(6.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(6.0)));
// DEFAULT-NEXT:         write<i64>(field0(%15), call<i64, signature=fn(vector<f32, 2>) -> i64, abi=sysv64(coerce<f64>) -> scalar>(%8, read<vector<f32, 2>>(%16)));
// DEFAULT-NEXT:         call<i64, signature=fn(vector<f32, 2>) -> i64, abi=sysv64(coerce<f64>) -> scalar>(%8, read<vector<f32, 2>>(%16));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f64, exceptions=observable>(float_widen<f64, reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(field1(%15)), const<i32>(0))))), const<f64>(6.0)), ne<f64, exceptions=observable>(float_widen<f64, reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(field1(%15)), const<i32>(1))))), const<f64>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %17 z: vector<i32, 2> [storage=automatic] = aggregate<vector<i32, 2>, zero_fill=false>(index0 = const<i32>(4), index1 = const<i32>(4));
// DEFAULT-NEXT:         write<i64>(field0(%15), call<i64, signature=fn(vector<i32, 2>) -> i64, abi=sysv64(coerce<f64>) -> scalar>(%10, read<vector<i32, 2>>(%17)));
// DEFAULT-NEXT:         call<i64, signature=fn(vector<i32, 2>) -> i64, abi=sysv64(coerce<f64>) -> scalar>(%10, read<vector<i32, 2>>(%17));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field2(%15)), const<i32>(0)))), const<i32>(4)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field2(%15)), const<i32>(1)))), const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
