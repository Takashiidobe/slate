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
// DEFAULT-NEXT:     global %10 .str10: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%9 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 a: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %7 b: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %8 r: i32 [storage=automatic];
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if gt<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:                     goto %2;
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     goto %3;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 goto %4;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         label %2 both_pos:
// DEFAULT-NEXT:             write<i32>(%8, const<i32>(1));
// DEFAULT-NEXT:         goto %5;
// DEFAULT-NEXT:         label %3 a_pos:
// DEFAULT-NEXT:             write<i32>(%8, const<i32>(2));
// DEFAULT-NEXT:         goto %5;
// DEFAULT-NEXT:         label %4 other:
// DEFAULT-NEXT:             write<i32>(%8, const<i32>(3));
// DEFAULT-NEXT:         goto %5;
// DEFAULT-NEXT:         label %5 done:
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%10)), read<i32>(%8));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
