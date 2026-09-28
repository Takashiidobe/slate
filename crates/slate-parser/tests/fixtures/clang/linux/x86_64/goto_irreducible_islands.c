#include <stdio.h>

int main() {
  volatile int pick_a = 1;
  volatile int pick_b = 0;
  int          x      = 0;
  int          y      = 0;

  if (pick_a)
    goto a2;
a1:
  x = x + 1;
  if (x < 3)
    goto a2;
  goto second;
a2:
  x = x + 2;
  if (x < 4)
    goto a1;

second:
  if (pick_b)
    goto b2;
b1:
  y = y + 3;
  if (y < 9)
    goto b2;
  goto done;
b2:
  y = y + 5;
  if (y < 11)
    goto b1;

done:
  printf("%d %d\n", x, y);
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
// DEFAULT-NEXT:     global %13 .str13: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%12 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 pick_a: volatile i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %9 pick_b: volatile i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %10 x: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %11 y: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%8), const<i32>(0))
// DEFAULT-NEXT:             goto %3;
// DEFAULT-NEXT:         label %2 a1:
// DEFAULT-NEXT:             write<i32>(%10, add<i32, overflow=ub>(read<i32>(%10), const<i32>(1)));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%10), const<i32>(3))
// DEFAULT-NEXT:             goto %3;
// DEFAULT-NEXT:         goto %4;
// DEFAULT-NEXT:         label %3 a2:
// DEFAULT-NEXT:             write<i32>(%10, add<i32, overflow=ub>(read<i32>(%10), const<i32>(2)));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%10), const<i32>(4))
// DEFAULT-NEXT:             goto %2;
// DEFAULT-NEXT:         label %4 second:
// DEFAULT-NEXT:             if ne<i32>(read<i32, volatile>(%9), const<i32>(0))
// DEFAULT-NEXT:                 goto %6;
// DEFAULT-NEXT:         label %5 b1:
// DEFAULT-NEXT:             write<i32>(%11, add<i32, overflow=ub>(read<i32>(%11), const<i32>(3)));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%11), const<i32>(9))
// DEFAULT-NEXT:             goto %6;
// DEFAULT-NEXT:         goto %7;
// DEFAULT-NEXT:         label %6 b2:
// DEFAULT-NEXT:             write<i32>(%11, add<i32, overflow=ub>(read<i32>(%11), const<i32>(5)));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%11), const<i32>(11))
// DEFAULT-NEXT:             goto %5;
// DEFAULT-NEXT:         label %7 done:
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%13)), read<i32>(%10), read<i32>(%11));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
