#line 1 "assert_runtime_true.c"
#include <assert.h>
#include <stdio.h>

int main(int argc, char **argv) {
  printf("before\n");
  assert(argc == 1);
  printf("after\n");
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([98, 101, 102, 111, 114, 101, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([97, 114, 103, 99, 32, 61, 61, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([97, 115, 115, 101, 114, 116, 95, 114, 117, 110, 116, 105, 109, 101, 95, 116, 114, 117, 101, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([105, 110, 116, 32, 109, 97, 105, 110, 40, 105, 110, 116, 44, 32, 99, 104, 97, 114, 32, 42, 42, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([97, 102, 116, 101, 114, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___assert_fail:[0-9]+]] @__assert_fail(%[[VALUE___assertion:[0-9]+]] __assertion: ptr<const i8>, %[[VALUE___file:[0-9]+]] __file: ptr<const i8>, %[[VALUE___line:[0-9]+]] __line: u32, %[[VALUE___function:[0-9]+]] __function: ptr<const i8>) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___assert_single_arg:[0-9]+]] @__assert_single_arg(%[[VALUE0:[0-9]+]] <unnamed>: bool) -> bool [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str]])));
// DEFAULT-NEXT:         const<u64>(1);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             if eq<i32>(read<i32>(%[[VALUE_argc]]), const<i32>(1))
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>, ptr<const i8>, u32, ptr<const i8>) -> void>(%[[VALUE___assert_fail]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_2]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str_3]])), reinterpret<u32, reason=arg, fits=always>(const<i32>(6)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_4]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_5]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
