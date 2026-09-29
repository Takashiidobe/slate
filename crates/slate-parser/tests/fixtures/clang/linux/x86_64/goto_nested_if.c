#include <stdio.h>

int main() {
  int a = 3;
  int b = -1;
  int r;
  if (a > 0) {
    if (b > 0)
      goto both_pos;
    else
      goto a_pos;
  } else {
    goto other;
  }
both_pos:
  r = 1;
  goto done;
a_pos:
  r = 2;
  goto done;
other:
  r = 3;
  goto done;
done:
  printf("%d\n", r);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if gt<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))
// DEFAULT-NEXT:                     goto %[[VALUE_both_pos:[0-9]+]];
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     goto %[[VALUE_a_pos:[0-9]+]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 goto %[[VALUE_other:[0-9]+]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         label %[[VALUE_both_pos]] both_pos:
// DEFAULT-NEXT:             write<i32>(%[[VALUE_r]], const<i32>(1));
// DEFAULT-NEXT:         goto %[[VALUE_done:[0-9]+]];
// DEFAULT-NEXT:         label %[[VALUE_a_pos]] a_pos:
// DEFAULT-NEXT:             write<i32>(%[[VALUE_r]], const<i32>(2));
// DEFAULT-NEXT:         goto %[[VALUE_done]];
// DEFAULT-NEXT:         label %[[VALUE_other]] other:
// DEFAULT-NEXT:             write<i32>(%[[VALUE_r]], const<i32>(3));
// DEFAULT-NEXT:         goto %[[VALUE_done]];
// DEFAULT-NEXT:         label %[[VALUE_done]] done:
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), read<i32>(%[[VALUE_r]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
