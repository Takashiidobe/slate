/* PR rtl-optimization/16104 */
/* { dg-require-effective-target int32plus } */
/* { dg-options "-Wno-psabi" } */

extern void abort(void);

typedef int          V2SI __attribute__((vector_size(8)));
typedef unsigned int V2USI __attribute__((vector_size(8)));
typedef short        V2HI __attribute__((vector_size(4)));
typedef unsigned int V2UHI __attribute__((vector_size(4)));

int test1(void) { return (long long)(V2SI)0LL; }

int test2(V2SI x) { return (long long)x; }

V2SI test3(void) { return (V2SI)(long long)(int)(V2HI)0; }

V2SI test4(V2HI x) { return (V2SI)(long long)(int)x; }

V2SI test5(V2USI x) { return (V2SI)x; }

int main(void) {
  if (sizeof(short) != 2 || sizeof(int) != 4 || sizeof(long long) != 8)
    return 0;

  if (test1() != 0)
    abort();

  V2SI x = {2, 2};
  if (test2(x) != 2)
    abort();

  union {
    V2SI      x;
    int       y[2];
    V2USI     z;
    long long l;
  } u;
  u.x = test3();
  if (u.y[0] != 0 || u.y[1] != 0)
    abort();

  V2HI y = {4, 4};
  union {
    V2SI      x;
    long long y;
  } v;
  v.x = test4(y);
  if (v.y != 0x40004)
    abort();

  V2USI z = {6, 6};
  u.x     = test5(z);
  if (u.y[0] != 6 || u.y[1] != 6)
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
// DEFAULT-NEXT:     type @type[[TYPE_V2HI:[0-9]+]] V2HI = vector<i16, 2>;
// DEFAULT-NEXT:     type @type[[TYPE_V2UHI:[0-9]+]] V2UHI = vector<u32, 1>;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 x: vector<i32, 2>;
// DEFAULT-NEXT:         field1 y: array<i32, 2>;
// DEFAULT-NEXT:         field2 z: vector<u32, 2>;
// DEFAULT-NEXT:         field3 l: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 x: vector<i32, 2>;
// DEFAULT-NEXT:         field1 y: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(vector_bit_cast<i64, reason=explicit>(vector_bit_cast<vector<i32, 2>, reason=explicit>(const<i64>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_x:[0-9]+]] x: vector<i32, 2>) -> i32 [linkage=external] [abi=sysv64(coerce<f64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(vector_bit_cast<i64, reason=explicit>(read<vector<i32, 2>>(%[[VALUE_x]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3() -> vector<i32, 2> [linkage=external] [abi=sysv64() -> coerce<f64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return vector_bit_cast<vector<i32, 2>, reason=explicit>(widen<i64, reason=explicit>(vector_bit_cast<i32, reason=explicit>(vector_bit_cast<vector<i16, 2>, reason=explicit>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_x_2:[0-9]+]] x: vector<i16, 2>) -> vector<i32, 2> [linkage=external] [abi=sysv64(coerce<i32>) -> coerce<f64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return vector_bit_cast<vector<i32, 2>, reason=explicit>(widen<i64, reason=explicit>(vector_bit_cast<i32, reason=explicit>(read<vector<i16, 2>>(%[[VALUE_x_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5:[0-9]+]] @test5(%[[VALUE_x_3:[0-9]+]] x: vector<u32, 2>) -> vector<i32, 2> [linkage=external] [abi=sysv64(coerce<f64>) -> coerce<f64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return vector_bit_cast<vector<i32, 2>, reason=explicit>(read<vector<u32, 2>>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u64>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), ne<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), ne<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_test1]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_x_4:[0-9]+]] x: vector<i32, 2> [storage=automatic] = aggregate<vector<i32, 2>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(2));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(vector<i32, 2>) -> i32, abi=sysv64(coerce<f64>) -> scalar>(%[[VALUE_test2]], read<vector<i32, 2>>(%[[VALUE_x_4]])), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%[[VALUE_u]]), call<vector<i32, 2>, signature=fn() -> vector<i32, 2>, abi=sysv64() -> coerce<f64>>(%[[VALUE_test3]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%[[VALUE_u]])), const<i32>(0)))), const<i32>(0)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%[[VALUE_u]])), const<i32>(1)))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: vector<i16, 2> [storage=automatic] = aggregate<vector<i16, 2>, zero_fill=false>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(4)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%[[VALUE_v]]), call<vector<i32, 2>, signature=fn(vector<i16, 2>) -> vector<i32, 2>, abi=sysv64(coerce<i32>) -> coerce<f64>>(%[[VALUE_test4]], read<vector<i16, 2>>(%[[VALUE_y]])));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(field1(%[[VALUE_v]])), widen<i64, reason=usual_arith>(const<i32>(262148)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: vector<u32, 2> [storage=automatic] = aggregate<vector<u32, 2>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(6)), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(6)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%[[VALUE_u]]), call<vector<i32, 2>, signature=fn(vector<u32, 2>) -> vector<i32, 2>, abi=sysv64(coerce<f64>) -> coerce<f64>>(%[[VALUE_test5]], read<vector<u32, 2>>(%[[VALUE_z]])));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%[[VALUE_u]])), const<i32>(0)))), const<i32>(6)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%[[VALUE_u]])), const<i32>(1)))), const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
