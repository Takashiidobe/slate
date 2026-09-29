#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

struct FixedPair {
  int16_t  left;
  uint32_t right;
  size_t   count;
};

static uint32_t global_u32  = 4000000000u;
static size_t   global_size = 7;

static int32_t add_i32(int32_t a, int16_t b) {
  int32_t sum = a + b;
  return sum;
}


static uint64_t widen_u32(uint32_t value) {
  uint64_t wide = value + global_size;
  return wide;
}

static int use_fixed_types(void) {
  int8_t           small        = -5;
  uint8_t          byte         = 250;
  int16_t          short_value  = 1200;
  uint16_t         ushort_value = 65000;
  int32_t          signed_value = add_i32(100000, short_value);
  uint64_t         wide         = widen_u32(global_u32);
  struct FixedPair pair;
  pair.left  = short_value;
  pair.right = global_u32;
  pair.count = global_size + 3;
  return small + byte + pair.left + signed_value + ushort_value + pair.count +
         wide;
}

int main(void) {
  printf("%d\n", add_i32(10, 20));
  printf("%lu\n", widen_u32(5));
  printf("%d\n", use_fixed_types());
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___int8_t:[0-9]+]] __int8_t = i8;
// DEFAULT-NEXT:     type @type[[TYPE___uint8_t:[0-9]+]] __uint8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE___int16_t:[0-9]+]] __int16_t = i16;
// DEFAULT-NEXT:     type @type[[TYPE___uint16_t:[0-9]+]] __uint16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE___int32_t:[0-9]+]] __int32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE___uint32_t:[0-9]+]] __uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___uint64_t:[0-9]+]] __uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_int8_t:[0-9]+]] int8_t = i8;
// DEFAULT-NEXT:     type @type[[TYPE_int16_t:[0-9]+]] int16_t = i16;
// DEFAULT-NEXT:     type @type[[TYPE_int32_t:[0-9]+]] int32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_uint8_t:[0-9]+]] uint8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE_uint16_t:[0-9]+]] uint16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE_uint32_t:[0-9]+]] uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_uint64_t:[0-9]+]] uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_FixedPair:[0-9]+]] FixedPair = struct {
// DEFAULT-NEXT:         field0 left: i16;
// DEFAULT-NEXT:         field1 right: u32;
// DEFAULT-NEXT:         field2 count: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     global %[[VALUE_global_u32:[0-9]+]] global_u32: u32 [storage=static] = const<u32>(4000000000) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_global_size:[0-9]+]] global_size: u64 [storage=static] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(7))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_add_i32:[0-9]+]] @add_i32(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i16) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_sum:[0-9]+]] sum: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), widen<i32, reason=promotion>(read<i16>(%[[VALUE_b]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_sum]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_widen_u32:[0-9]+]] @widen_u32(%[[VALUE_value:[0-9]+]] value: u32) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_wide:[0-9]+]] wide: u64 [storage=automatic] = add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_value]])), read<u64>(%[[VALUE_global_size]]));
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_wide]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use_fixed_types:[0-9]+]] @use_fixed_types() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_small:[0-9]+]] small: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(5)));
// DEFAULT-NEXT:         let %[[VALUE_byte:[0-9]+]] byte: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(250)));
// DEFAULT-NEXT:         let %[[VALUE_short_value:[0-9]+]] short_value: i16 [storage=automatic] = truncate<i16, reason=assign, fits=always>(const<i32>(1200));
// DEFAULT-NEXT:         let %[[VALUE_ushort_value:[0-9]+]] ushort_value: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(const<i32>(65000)));
// DEFAULT-NEXT:         let %[[VALUE_signed_value:[0-9]+]] signed_value: i32 [storage=automatic] = call<i32, signature=fn(i32, i16) -> i32>(%[[VALUE_add_i32]], const<i32>(100000), read<i16>(%[[VALUE_short_value]]));
// DEFAULT-NEXT:         let %[[VALUE_wide_2:[0-9]+]] wide: u64 [storage=automatic] = call<u64, signature=fn(u32) -> u64>(%[[VALUE_widen_u32]], read<u32>(%[[VALUE_global_u32]]));
// DEFAULT-NEXT:         let %[[VALUE_pair:[0-9]+]] pair: @type[[TYPE_FixedPair]] [storage=automatic];
// DEFAULT-NEXT:         write<i16>(field0(%[[VALUE_pair]]), read<i16>(%[[VALUE_short_value]]));
// DEFAULT-NEXT:         write<u32>(field1(%[[VALUE_pair]]), read<u32>(%[[VALUE_global_u32]]));
// DEFAULT-NEXT:         write<u64>(field2(%[[VALUE_pair]]), add<u64, overflow=wrap>(read<u64>(%[[VALUE_global_size]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_small]])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_byte]])))), widen<i32, reason=promotion>(read<i16>(field0(%[[VALUE_pair]])))), read<i32>(%[[VALUE_signed_value]])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_ushort_value]])))))), read<u64>(field2(%[[VALUE_pair]]))), read<u64>(%[[VALUE_wide_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), call<i32, signature=fn(i32, i16) -> i32>(%[[VALUE_add_i32]], const<i32>(10), truncate<i16, reason=arg, fits=always>(const<i32>(20))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_2]])), call<u64, signature=fn(u32) -> u64>(%[[VALUE_widen_u32]], reinterpret<u32, reason=arg, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]])), call<i32, signature=fn() -> i32>(%[[VALUE_use_fixed_types]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
