/* { dg-require-effective-target stdint_types } */
#include <stdint.h>

void abort(void);

typedef int32_t __attribute__((vector_size(8))) v2si;
int64_t                                         s64;

static inline int64_t __ev_convert_s64(v2si a) { return (int64_t)a; }

int main() {
  union {
    int64_t ll;
    int32_t i[2];
  } endianness_test;
  endianness_test.ll    = 1;
  int32_t little_endian = endianness_test.i[0];
  s64                   = __ev_convert_s64((v2si){1, 0xffffffff});
  if (s64 != (little_endian ? 0xffffffff00000001LL : 0x1ffffffffLL))
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
// DEFAULT-NEXT:     type @type[[TYPE___int32_t:[0-9]+]] __int32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE___int64_t:[0-9]+]] __int64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_int32_t:[0-9]+]] int32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_int64_t:[0-9]+]] int64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_v2si:[0-9]+]] v2si = vector<i32, 2>;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 ll: i64;
// DEFAULT-NEXT:         field1 i: array<i32, 2>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_s64:[0-9]+]] s64: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___ev_convert_s64:[0-9]+]] @__ev_convert_s64(%[[VALUE_a:[0-9]+]] a: vector<i32, 2>) -> i64 [linkage=internal] [inline=hint] [definition=emitted] [abi=sysv64(coerce<f64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return vector_bit_cast<i64, reason=explicit>(read<vector<i32, 2>>(%[[VALUE_a]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_endianness_test:[0-9]+]] endianness_test: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<i64>(field0(%[[VALUE_endianness_test]]), widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_little_endian:[0-9]+]] little_endian: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%[[VALUE_endianness_test]])), const<i32>(0))));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_s64]], call<i64, signature=fn(vector<i32, 2>) -> i64, abi=sysv64(coerce<f64>) -> scalar>(%[[VALUE___ev_convert_s64]], read<vector<i32, 2>>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<vector<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = reinterpret<i32, reason=assign, fits=unknown>(const<u32>(4294967295))))));
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_s64]])), conditional<u64>(ne<i32>(read<i32>(%[[VALUE_little_endian]]), const<i32>(0)), const<u64>(18446744069414584321), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(8589934591))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
