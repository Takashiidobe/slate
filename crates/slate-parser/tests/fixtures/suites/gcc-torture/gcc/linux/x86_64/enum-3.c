/* The composite type of int and an enum compatible with int might be
   either of the two types, but it isn't an unsigned type.  */
/* Origin: Joseph Myers <jsm@polyomino.org.uk> */

#include <limits.h>

#include <stdio.h>

extern void abort(void);
extern void exit(int);

enum e { a = INT_MIN };

int    *p;
enum e *q;
int     main(void) {
  enum e x = a;
  q        = &x;
  if (*(1 ? q : p) > 0)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     type @type[[TYPE_e:[0-9]+]] e = enum : i32 {
// DEFAULT-NEXT:         %[[VALUE_a:[0-9]+]] a = const<i32>(-2147483648);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: ptr<@type[[TYPE_e]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_a]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: @type[[TYPE_e]] [storage=automatic] = int_to_enum<@type[[TYPE_e]], reason=assign>(const<i32>(-2147483648));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_e]]>>(%[[VALUE_q]], addr_of<ptr<@type[[TYPE_e]]>>(%[[VALUE_x]]));
// DEFAULT-NEXT:         if gt<i32>(enum_to_int<i32, reason=promotion>(read<@type[[TYPE_e]]>(deref(conditional<ptr<@type[[TYPE_e]]>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type[[TYPE_e]]>>(%[[VALUE_q]]), pointer_cast<ptr<@type[[TYPE_e]]>, reason=usual_arith>(read<ptr<i32>>(%[[VALUE_p]])))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_a]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
