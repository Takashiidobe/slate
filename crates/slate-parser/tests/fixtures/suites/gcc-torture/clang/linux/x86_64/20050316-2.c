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
// DEFAULT-NEXT:     type @type[[TYPE_V2SI:[0-9]+]] V2SI = vector<i32, 2>;
// DEFAULT-NEXT:     type @type[[TYPE_V2USI:[0-9]+]] V2USI = vector<u32, 2>;
// DEFAULT-NEXT:     type @type[[TYPE_V2SF:[0-9]+]] V2SF = vector<f32, 2>;
// DEFAULT-NEXT:     type @type[[TYPE_V2HI:[0-9]+]] V2HI = vector<i16, 2>;
// DEFAULT-NEXT:     type @type[[TYPE_V2UHI:[0-9]+]] V2UHI = vector<u32, 1>;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 l: i64;
// DEFAULT-NEXT:         field1 f: array<f32, 2>;
// DEFAULT-NEXT:         field2 i: array<i32, 2>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_x:[0-9]+]] x: vector<f32, 2>) -> i64 [linkage=external] [abi=sysv64(coerce<f64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return vector_bit_cast<i64, reason=explicit>(vector_bit_cast<vector<i32, 2>, reason=explicit>(read<vector<f32, 2>>(%[[VALUE_x]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_x_2:[0-9]+]] x: vector<f32, 2>) -> i64 [linkage=external] [abi=sysv64(coerce<f64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return vector_bit_cast<i64, reason=explicit>(read<vector<f32, 2>>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_x_3:[0-9]+]] x: vector<i32, 2>) -> i64 [linkage=external] [abi=sysv64(coerce<f64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return vector_bit_cast<i64, reason=explicit>(vector_bit_cast<vector<f32, 2>, reason=explicit>(read<vector<i32, 2>>(%[[VALUE_x_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u64>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), ne<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), ne<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_x_4:[0-9]+]] x: vector<f32, 2> [storage=automatic] = aggregate<vector<f32, 2>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)));
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<i64>(field0(%[[VALUE_u]]), call<i64, signature=fn(vector<f32, 2>) -> i64, abi=sysv64(coerce<f64>) -> scalar>(%[[VALUE_test1]], read<vector<f32, 2>>(%[[VALUE_x_4]])));
// DEFAULT-NEXT:         call<i64, signature=fn(vector<f32, 2>) -> i64, abi=sysv64(coerce<f64>) -> scalar>(%[[VALUE_test1]], read<vector<f32, 2>>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(field1(%[[VALUE_u]])), const<i32>(0))))), const<f64>(2.0)), ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(field1(%[[VALUE_u]])), const<i32>(1))))), const<f64>(2.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: vector<f32, 2> [storage=automatic] = aggregate<vector<f32, 2>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(6.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(6.0)));
// DEFAULT-NEXT:         write<i64>(field0(%[[VALUE_u]]), call<i64, signature=fn(vector<f32, 2>) -> i64, abi=sysv64(coerce<f64>) -> scalar>(%[[VALUE_test2]], read<vector<f32, 2>>(%[[VALUE_y]])));
// DEFAULT-NEXT:         call<i64, signature=fn(vector<f32, 2>) -> i64, abi=sysv64(coerce<f64>) -> scalar>(%[[VALUE_test2]], read<vector<f32, 2>>(%[[VALUE_y]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(field1(%[[VALUE_u]])), const<i32>(0))))), const<f64>(6.0)), ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(field1(%[[VALUE_u]])), const<i32>(1))))), const<f64>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: vector<i32, 2> [storage=automatic] = aggregate<vector<i32, 2>, zero_fill=false>(index0 = const<i32>(4), index1 = const<i32>(4));
// DEFAULT-NEXT:         write<i64>(field0(%[[VALUE_u]]), call<i64, signature=fn(vector<i32, 2>) -> i64, abi=sysv64(coerce<f64>) -> scalar>(%[[VALUE_test3]], read<vector<i32, 2>>(%[[VALUE_z]])));
// DEFAULT-NEXT:         call<i64, signature=fn(vector<i32, 2>) -> i64, abi=sysv64(coerce<f64>) -> scalar>(%[[VALUE_test3]], read<vector<i32, 2>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field2(%[[VALUE_u]])), const<i32>(0)))), const<i32>(4)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field2(%[[VALUE_u]])), const<i32>(1)))), const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
