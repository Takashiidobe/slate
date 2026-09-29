#include <stdio.h>

int main(void) {
  int elifdef_value  = 0;
  int elifndef_value = 0;
  int inactive_value = 0;

#if 0
  elifdef_value = 1;
#elifdef __STDC__
  elifdef_value = 23;
#elifndef __STDC__
  elifdef_value = 2;
#else
  elifdef_value = 3;
#endif

#if 0
  elifndef_value = 1;
#elifdef C23_MISSING
  elifndef_value = 2;
#elifndef C23_MISSING
  elifndef_value = 29;
#else
  elifndef_value = 3;
#endif

#ifdef __STDC__
  inactive_value = 31;
#elifdef __STDC__
  inactive_value = 2;
#elifndef C23_MISSING
  inactive_value = 3;
#else
  inactive_value = 4;
#endif

  printf("%d %d %d\n", elifdef_value, elifndef_value, inactive_value);
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
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_elifdef_value:[0-9]+]] elifdef_value: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_elifndef_value:[0-9]+]] elifndef_value: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_inactive_value:[0-9]+]] inactive_value: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_elifdef_value]], const<i32>(23));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_elifndef_value]], const<i32>(29));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_inactive_value]], const<i32>(31));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])), read<i32>(%[[VALUE_elifdef_value]]), read<i32>(%[[VALUE_elifndef_value]]), read<i32>(%[[VALUE_inactive_value]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
