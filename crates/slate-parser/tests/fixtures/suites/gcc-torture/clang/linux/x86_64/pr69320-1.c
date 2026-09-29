#include <stdlib.h>
int         a, b, d, f;
char        c;
static int *e = &d;
int         main() {
  int g = -1L;
  *e    = g;
  c     = 4;
  for (; c >= 14; c++)
    *e = 1;
  f      = a == 0;
  *e    ^= f;
  int h  = ~d;
  if (d)
    b = h;
  if (h)
    exit(0);
  abort();
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_d]]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE___status:[0-9]+]] __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: i32 [storage=automatic] = truncate<i32, reason=assign, fits=unknown>(neg<i64, overflow=ub>(const<i64>(1)));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_e]])), read<i32>(%[[VALUE_g]]));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_c]], truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ge<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_c]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE1]])), const<i32>(1)));
// DEFAULT-NEXT:                 write<i8>(%[[VALUE_c]], read<i8>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE_e]])), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_f]], from_bool<i32, reason=assign>(eq<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_e]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE3]])));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = xor<i32>(read<i32>(%[[VALUE4]]), read<i32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE3]])), read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: i32 [storage=automatic] = not<i32>(read<i32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE_h]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_h]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
