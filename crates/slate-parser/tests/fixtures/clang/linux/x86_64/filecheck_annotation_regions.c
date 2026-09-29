#include <stdio.h>

int main(void) {
  int value;
  int lowered_value;
  int lowered_absence;
  lowered_value   = 6 * 7;
  lowered_absence = 5 + 6;
  value           = 40 + 2;
  printf("%d %d %d\n", value, lowered_value, lowered_absence);
  puts("_v9 anon_4 anon_struct_i32");
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([95, 118, 57, 32, 97, 110, 111, 110, 95, 52, 32, 97, 110, 111, 110, 95, 115, 116, 114, 117, 99, 116, 95, 105, 51, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_puts:[0-9]+]] @puts(%[[VALUE___s:[0-9]+]] __s: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_value:[0-9]+]] value: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_lowered_value:[0-9]+]] lowered_value: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_lowered_absence:[0-9]+]] lowered_absence: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_lowered_value]], mul<i32, overflow=ub>(const<i32>(6), const<i32>(7)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_lowered_absence]], add<i32, overflow=ub>(const<i32>(5), const<i32>(6)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_value]], add<i32, overflow=ub>(const<i32>(40), const<i32>(2)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])), read<i32>(%[[VALUE_value]]), read<i32>(%[[VALUE_lowered_value]]), read<i32>(%[[VALUE_lowered_absence]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_puts]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_2]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
