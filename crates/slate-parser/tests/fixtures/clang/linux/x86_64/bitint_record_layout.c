#include <stdio.h>

union BitIntOrArray {
  _BitInt(65) bits;
  char bytes[20];
};

struct NestedBitInt {
  char tag;
  struct {
    int prefix;
    _BitInt(65) value;
  } inner;
  short tail;
};

static struct NestedBitInt values[2] = {
    {.tag = 1, .inner = {.prefix = 2, .value = 333}, .tail = 4},
    {.tag = 5, .inner = {.prefix = 6, .value = 777}, .tail = 8},
};

int main(void) {
  union BitIntOrArray item = {.bytes = {'a', 'b', 'c'}};
  printf("%zu %zu %d %d %lld %d %c%c%c\n", sizeof(item), sizeof(values[0]),
         values[1].tag, values[1].inner.prefix,
         (long long)values[1].inner.value, values[1].tail, item.bytes[0],
         item.bytes[1], item.bytes[2]);
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
// DEFAULT-NEXT:     type @type[[TYPE_BitIntOrArray:[0-9]+]] BitIntOrArray = union {
// DEFAULT-NEXT:         field0 bits: i65b;
// DEFAULT-NEXT:         field1 bytes: array<i8, 20>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_NestedBitInt:[0-9]+]] NestedBitInt = struct {
// DEFAULT-NEXT:         field0 tag: i8;
// DEFAULT-NEXT:         field1 inner: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:         field2 tail: i16;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 8, 32]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = struct {
// DEFAULT-NEXT:         field0 prefix: i32;
// DEFAULT-NEXT:         field1 value: i65b;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_values:[0-9]+]] values: array<@type[[TYPE_NestedBitInt]], 2> [storage=static] [align=16] = aggregate<array<@type[[TYPE_NestedBitInt]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE_NestedBitInt]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field1 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(2), field1 = widen<i65b, reason=assign>(const<i32>(333))), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(4))), index1 = aggregate<@type[[TYPE_NestedBitInt]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), field1 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(6), field1 = widen<i65b, reason=assign>(const<i32>(777))), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(8)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([37, 122, 117, 32, 37, 122, 117, 32, 37, 100, 32, 37, 100, 32, 37, 108, 108, 100, 32, 37, 100, 32, 37, 99, 37, 99, 37, 99, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_item:[0-9]+]] item: @type[[TYPE_BitIntOrArray]] [storage=automatic] = aggregate<@type[[TYPE_BitIntOrArray]], zero_fill=false>(field1 = aggregate<array<i8, 20>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(97)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(98)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(99))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%[[VALUE_str]])), const<u64>(24), const<u64>(40), widen<i32, reason=vararg>(read<i8>(field0(deref(ptr_offset<ptr<@type[[TYPE_NestedBitInt]]>, subtract=false, element=@type[[TYPE_NestedBitInt]], overflow=ub>(array_decay<ptr<@type[[TYPE_NestedBitInt]]>, length=Some(2)>(%[[VALUE_values]]), const<i32>(1)))))), read<i32>(field0(field1(deref(ptr_offset<ptr<@type[[TYPE_NestedBitInt]]>, subtract=false, element=@type[[TYPE_NestedBitInt]], overflow=ub>(array_decay<ptr<@type[[TYPE_NestedBitInt]]>, length=Some(2)>(%[[VALUE_values]]), const<i32>(1)))))), truncate<i64, reason=explicit, fits=unknown>(read<i65b>(field1(field1(deref(ptr_offset<ptr<@type[[TYPE_NestedBitInt]]>, subtract=false, element=@type[[TYPE_NestedBitInt]], overflow=ub>(array_decay<ptr<@type[[TYPE_NestedBitInt]]>, length=Some(2)>(%[[VALUE_values]]), const<i32>(1))))))), widen<i32, reason=vararg>(read<i16>(field2(deref(ptr_offset<ptr<@type[[TYPE_NestedBitInt]]>, subtract=false, element=@type[[TYPE_NestedBitInt]], overflow=ub>(array_decay<ptr<@type[[TYPE_NestedBitInt]]>, length=Some(2)>(%[[VALUE_values]]), const<i32>(1)))))), widen<i32, reason=vararg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(20)>(field1(%[[VALUE_item]])), const<i32>(0))))), widen<i32, reason=vararg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(20)>(field1(%[[VALUE_item]])), const<i32>(1))))), widen<i32, reason=vararg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(20)>(field1(%[[VALUE_item]])), const<i32>(2))))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
