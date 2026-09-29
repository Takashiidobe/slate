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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_pick_a:[0-9]+]] pick_a: volatile i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE_pick_b:[0-9]+]] pick_b: volatile i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%[[VALUE_pick_a]]), const<i32>(0))
// DEFAULT-NEXT:             goto %[[VALUE_a2:[0-9]+]];
// DEFAULT-NEXT:         label %[[VALUE_a1:[0-9]+]] a1:
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x]], add<i32, overflow=ub>(read<i32>(%[[VALUE_x]]), const<i32>(1)));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_x]]), const<i32>(3))
// DEFAULT-NEXT:             goto %[[VALUE_a2]];
// DEFAULT-NEXT:         goto %[[VALUE_second:[0-9]+]];
// DEFAULT-NEXT:         label %[[VALUE_a2]] a2:
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x]], add<i32, overflow=ub>(read<i32>(%[[VALUE_x]]), const<i32>(2)));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_x]]), const<i32>(4))
// DEFAULT-NEXT:             goto %[[VALUE_a1]];
// DEFAULT-NEXT:         label %[[VALUE_second]] second:
// DEFAULT-NEXT:             if ne<i32>(read<i32, volatile>(%[[VALUE_pick_b]]), const<i32>(0))
// DEFAULT-NEXT:                 goto %[[VALUE_b2:[0-9]+]];
// DEFAULT-NEXT:         label %[[VALUE_b1:[0-9]+]] b1:
// DEFAULT-NEXT:             write<i32>(%[[VALUE_y]], add<i32, overflow=ub>(read<i32>(%[[VALUE_y]]), const<i32>(3)));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_y]]), const<i32>(9))
// DEFAULT-NEXT:             goto %[[VALUE_b2]];
// DEFAULT-NEXT:         goto %[[VALUE_done:[0-9]+]];
// DEFAULT-NEXT:         label %[[VALUE_b2]] b2:
// DEFAULT-NEXT:             write<i32>(%[[VALUE_y]], add<i32, overflow=ub>(read<i32>(%[[VALUE_y]]), const<i32>(5)));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_y]]), const<i32>(11))
// DEFAULT-NEXT:             goto %[[VALUE_b1]];
// DEFAULT-NEXT:         label %[[VALUE_done]] done:
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]])), read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_y]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
