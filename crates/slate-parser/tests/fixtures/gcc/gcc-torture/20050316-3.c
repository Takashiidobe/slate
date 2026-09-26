/* { dg-options "-Wno-psabi" } */
extern void abort(void);

typedef int          V2SI __attribute__((vector_size(8)));
typedef unsigned int V2USI __attribute__((vector_size(8)));
typedef short        V2HI __attribute__((vector_size(4)));
typedef unsigned int V2UHI __attribute__((vector_size(4)));

V2USI
test1(V2SI x) { return (V2USI)(V2SI)(long long)x; }

long long test2(V2SI x) { return (long long)(V2USI)(V2SI)(long long)x; }

int main(void) {
  if (sizeof(short) != 2 || sizeof(int) != 4 || sizeof(long long) != 8)
    return 0;

  union {
    V2SI      x;
    int       y[2];
    V2USI     z;
    long long l;
  } u;
  V2SI a = {-3, -3};
  u.z    = test1(a);
  if (u.y[0] != -3 || u.y[1] != -3)
    abort();

  u.l = test2(a);
  if (u.y[0] != -3 || u.y[1] != -3)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @test1(%6 x: vector<i32, 2>) -> vector<u32, 2> [linkage=external] [abi=sysv64(coerce<f64>) -> coerce<f64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return vector_bit_cast<vector<u32, 2>, reason=explicit>(vector_bit_cast<vector<i32, 2>, reason=explicit>(vector_bit_cast<i64, reason=explicit>(read<vector<i32, 2>>(%6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test2(%8 x: vector<i32, 2>) -> i64 [linkage=external] [abi=sysv64(coerce<f64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return vector_bit_cast<i64, reason=explicit>(vector_bit_cast<vector<u32, 2>, reason=explicit>(vector_bit_cast<vector<i32, 2>, reason=explicit>(vector_bit_cast<i64, reason=explicit>(read<vector<i32, 2>>(%8)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u64>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), ne<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), ne<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         let %11 u: @type4 [storage=automatic];
// DEFAULT-NEXT:         let %12 a: vector<i32, 2> [storage=automatic] = aggregate<vector<i32, 2>, zero_fill=false>(index0 = neg<i32, overflow=ub>(const<i32>(3)), index1 = neg<i32, overflow=ub>(const<i32>(3)));
// DEFAULT-NEXT:         write<vector<u32, 2>>(field2(%11), call<vector<u32, 2>, signature=fn(vector<i32, 2>) -> vector<u32, 2>, abi=sysv64(coerce<f64>) -> coerce<f64>>(%5, read<vector<i32, 2>>(%12)));
// DEFAULT-NEXT:         call<vector<u32, 2>, signature=fn(vector<i32, 2>) -> vector<u32, 2>, abi=sysv64(coerce<f64>) -> coerce<f64>>(%5, read<vector<i32, 2>>(%12));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%11)), const<i32>(0)))), neg<i32, overflow=ub>(const<i32>(3))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%11)), const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(3))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i64>(field3(%11), call<i64, signature=fn(vector<i32, 2>) -> i64, abi=sysv64(coerce<f64>) -> scalar>(%7, read<vector<i32, 2>>(%12)));
// DEFAULT-NEXT:         call<i64, signature=fn(vector<i32, 2>) -> i64, abi=sysv64(coerce<f64>) -> scalar>(%7, read<vector<i32, 2>>(%12));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%11)), const<i32>(0)))), neg<i32, overflow=ub>(const<i32>(3))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%11)), const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(3))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
