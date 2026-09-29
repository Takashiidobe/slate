#include <stdio.h>

typedef enum { E_OK = 0, E_FAIL = 1 } Status;

int main(void) {
  struct CaseData {
    int    input;
    Status expected;
  };
  struct CaseData cases[2] = {{1, E_OK}, {2, E_FAIL}};
  Status          actual   = E_OK;
  for (int i = 0; i < 2; i++) {
    if (actual != cases[i].expected) {
      printf("mismatch %d\n", i);
    } else {
      printf("match %d\n", i);
    }
  }
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_E_OK:[0-9]+]] E_OK = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_E_FAIL:[0-9]+]] E_FAIL = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_Status:[0-9]+]] Status = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_CaseData:[0-9]+]] CaseData = struct {
// DEFAULT-NEXT:         field0 input: i32;
// DEFAULT-NEXT:         field1 expected: @type[[TYPE0]];
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([109, 105, 115, 109, 97, 116, 99, 104, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([109, 97, 116, 99, 104, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_E_FAIL]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_cases:[0-9]+]] cases: array<@type[[TYPE_CaseData]], 2> [storage=automatic] [align=16] = aggregate<array<@type[[TYPE_CaseData]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE_CaseData]], zero_fill=false>(field0 = const<i32>(1), field1 = int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0)))), index1 = aggregate<@type[[TYPE_CaseData]], zero_fill=false>(field0 = const<i32>(2), field1 = int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE_actual:[0-9]+]] actual: @type[[TYPE0]] [storage=automatic] = int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE0]]>(%[[VALUE_actual]])), enum_to_int<u32, reason=promotion>(read<@type[[TYPE0]]>(field1(deref(ptr_offset<ptr<@type[[TYPE_CaseData]]>, subtract=false, element=@type[[TYPE_CaseData]], overflow=ub>(array_decay<ptr<@type[[TYPE_CaseData]]>, length=Some(2)>(%[[VALUE_cases]]), read<i32>(%[[VALUE_i]])))))))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_E_FAIL]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str]])), read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_E_FAIL]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_2]])), read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
