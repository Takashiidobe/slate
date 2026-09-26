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
// DEFAULT-NEXT:     type @type0 V2SI = vector<i32, 2>;
// DEFAULT-NEXT:     type @type1 V2USI = vector<u32, 2>;
// DEFAULT-NEXT:     type @type2 V2HI = vector<i16, 2>;
// DEFAULT-NEXT:     type @type3 V2UHI = vector<u32, 1>;
// DEFAULT-NEXT:     type @type4 = union {
// DEFAULT-NEXT:         field0 x: vector<i32, 2>;
// DEFAULT-NEXT:         field1 y: array<i32, 2>;
// DEFAULT-NEXT:         field2 z: vector<u32, 2>;
// DEFAULT-NEXT:         field3 l: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0, 0]];
// DEFAULT-NEXT:     type @type5 = union {
// DEFAULT-NEXT:         field0 x: vector<i32, 2>;
// DEFAULT-NEXT:         field1 y: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @test1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(vector_bit_cast<i64, reason=explicit>(vector_bit_cast<vector<i32, 2>, reason=explicit>(const<i64>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test2(%7 x: vector<i32, 2>) -> i32 [linkage=external] [abi=sysv64(coerce<f64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(vector_bit_cast<i64, reason=explicit>(read<vector<i32, 2>>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test3() -> vector<i32, 2> [linkage=external] [abi=sysv64() -> coerce<f64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return vector_bit_cast<vector<i32, 2>, reason=explicit>(widen<i64, reason=explicit>(vector_bit_cast<i32, reason=explicit>(vector_bit_cast<vector<i16, 2>, reason=explicit>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test4(%10 x: vector<i16, 2>) -> vector<i32, 2> [linkage=external] [abi=sysv64(coerce<i32>) -> coerce<f64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return vector_bit_cast<vector<i32, 2>, reason=explicit>(widen<i64, reason=explicit>(vector_bit_cast<i32, reason=explicit>(read<vector<i16, 2>>(%10))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test5(%12 x: vector<u32, 2>) -> vector<i32, 2> [linkage=external] [abi=sysv64(coerce<f64>) -> coerce<f64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return vector_bit_cast<vector<i32, 2>, reason=explicit>(read<vector<u32, 2>>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u64>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), ne<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), ne<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %14 x: vector<i32, 2> [storage=automatic] = aggregate<vector<i32, 2>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(2));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(vector<i32, 2>) -> i32, abi=sysv64(coerce<f64>) -> scalar>(%6, read<vector<i32, 2>>(%14)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %16 u: @type4 [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%16), call<vector<i32, 2>, signature=fn() -> vector<i32, 2>, abi=sysv64() -> coerce<f64>>(%8));
// DEFAULT-NEXT:         call<vector<i32, 2>, signature=fn() -> vector<i32, 2>, abi=sysv64() -> coerce<f64>>(%8);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%16)), const<i32>(0)))), const<i32>(0)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%16)), const<i32>(1)))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %17 y: vector<i16, 2> [storage=automatic] = aggregate<vector<i16, 2>, zero_fill=false>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(4)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         let %19 v: @type5 [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%19), call<vector<i32, 2>, signature=fn(vector<i16, 2>) -> vector<i32, 2>, abi=sysv64(coerce<i32>) -> coerce<f64>>(%9, read<vector<i16, 2>>(%17)));
// DEFAULT-NEXT:         call<vector<i32, 2>, signature=fn(vector<i16, 2>) -> vector<i32, 2>, abi=sysv64(coerce<i32>) -> coerce<f64>>(%9, read<vector<i16, 2>>(%17));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(field1(%19)), widen<i64, reason=usual_arith>(const<i32>(262148)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %20 z: vector<u32, 2> [storage=automatic] = aggregate<vector<u32, 2>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(6)), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(6)));
// DEFAULT-NEXT:         write<vector<i32, 2>>(field0(%16), call<vector<i32, 2>, signature=fn(vector<u32, 2>) -> vector<i32, 2>, abi=sysv64(coerce<f64>) -> coerce<f64>>(%11, read<vector<u32, 2>>(%20)));
// DEFAULT-NEXT:         call<vector<i32, 2>, signature=fn(vector<u32, 2>) -> vector<i32, 2>, abi=sysv64(coerce<f64>) -> coerce<f64>>(%11, read<vector<u32, 2>>(%20));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%16)), const<i32>(0)))), const<i32>(6)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%16)), const<i32>(1)))), const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
