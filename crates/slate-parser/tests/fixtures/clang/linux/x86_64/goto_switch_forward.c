#include <stdio.h>

int main() {
  int x = 2;
  int r = 0;
  switch (x) {
  case 1:
    goto one;
  case 2:
    goto two;
  default:
    goto other;
  }
one:
  r = 10;
  goto done;
two:
  r = 20;
  goto done;
other:
  r = 30;
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
// DEFAULT-NEXT:     global %11 .str11: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%9 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 x: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %8 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         switch %10 read<i32>(%7)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %10 const<i32>(1):
// DEFAULT-NEXT:                     goto %3;
// DEFAULT-NEXT:                 case %10 const<i32>(2):
// DEFAULT-NEXT:                     goto %4;
// DEFAULT-NEXT:                 default %10:
// DEFAULT-NEXT:                     goto %5;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         label %3 one:
// DEFAULT-NEXT:             write<i32>(%8, const<i32>(10));
// DEFAULT-NEXT:         goto %6;
// DEFAULT-NEXT:         label %4 two:
// DEFAULT-NEXT:             write<i32>(%8, const<i32>(20));
// DEFAULT-NEXT:         goto %6;
// DEFAULT-NEXT:         label %5 other:
// DEFAULT-NEXT:             write<i32>(%8, const<i32>(30));
// DEFAULT-NEXT:         goto %6;
// DEFAULT-NEXT:         label %6 done:
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%11)), read<i32>(%8));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
