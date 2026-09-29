#include <stdio.h>

struct Slice {
  const char   *data;
  unsigned long length;
};

static struct Slice *make_slice(struct Slice *result, const char *data,
                                unsigned long length) {
  result->data   = data;
  result->length = length;
  return result;
}

#define SLICE(text) (*make_slice(&(struct Slice){}, text, sizeof(text) - 1))

int main(void) {
  struct Slice first  = SLICE("slate");
  struct Slice second = SLICE("translation");
  printf("%c %lu %c %lu\n", first.data[0], first.length, second.data[0],
         second.length);
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
// DEFAULT-NEXT:     type @type[[TYPE_Slice:[0-9]+]] Slice = struct {
// DEFAULT-NEXT:         field0 data: ptr<const i8>;
// DEFAULT-NEXT:         field1 length: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([116, 114, 97, 110, 115, 108, 97, 116, 105, 111, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([37, 99, 32, 37, 108, 117, 32, 37, 99, 32, 37, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_make_slice:[0-9]+]] @make_slice(%[[VALUE_result:[0-9]+]] result: ptr<@type[[TYPE_Slice]]>, %[[VALUE_data:[0-9]+]] data: ptr<const i8>, %[[VALUE_length:[0-9]+]] length: u64) -> ptr<@type[[TYPE_Slice]]> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<const i8>>(field0(deref(read<ptr<@type[[TYPE_Slice]]>>(%[[VALUE_result]]))), read<ptr<const i8>>(%[[VALUE_data]]));
// DEFAULT-NEXT:         write<u64>(field1(deref(read<ptr<@type[[TYPE_Slice]]>>(%[[VALUE_result]]))), read<u64>(%[[VALUE_length]]));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_Slice]]>>(%[[VALUE_result]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_first:[0-9]+]] first: @type[[TYPE_Slice]] [storage=automatic] = copy<@type[[TYPE_Slice]], reason=assign>(read<@type[[TYPE_Slice]]>(deref(call<ptr<@type[[TYPE_Slice]]>, signature=fn(ptr<@type[[TYPE_Slice]]>, ptr<const i8>, u64) -> ptr<@type[[TYPE_Slice]]>>(%[[VALUE_make_slice]], addr_of<ptr<@type[[TYPE_Slice]]>>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_Slice]], zero_fill=true>()), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str]])), sub<u64, overflow=wrap>(const<u64>(6), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))));
// DEFAULT-NEXT:         let %[[VALUE_second:[0-9]+]] second: @type[[TYPE_Slice]] [storage=automatic] = copy<@type[[TYPE_Slice]], reason=assign>(read<@type[[TYPE_Slice]]>(deref(call<ptr<@type[[TYPE_Slice]]>, signature=fn(ptr<@type[[TYPE_Slice]]>, ptr<const i8>, u64) -> ptr<@type[[TYPE_Slice]]>>(%[[VALUE_make_slice]], addr_of<ptr<@type[[TYPE_Slice]]>>(compound_literal %[[VALUE1:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_Slice]], zero_fill=true>()), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_2]])), sub<u64, overflow=wrap>(const<u64>(12), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%[[VALUE_str_3]])), widen<i32, reason=vararg>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(field0(%[[VALUE_first]])), const<i32>(0))))), read<u64>(field1(%[[VALUE_first]])), widen<i32, reason=vararg>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(field0(%[[VALUE_second]])), const<i32>(0))))), read<u64>(field1(%[[VALUE_second]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
