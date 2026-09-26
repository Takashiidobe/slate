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
// DEFAULT-NEXT:     global %2 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 c: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 e: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%4) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%11 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 g: i32 [storage=automatic] = truncate<i32, reason=assign, fits=unknown>(neg<i64, overflow=ub>(const<i64>(1)));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%7)), read<i32>(%9));
// DEFAULT-NEXT:         write<i8>(%6, truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ge<i32>(widen<i32, reason=promotion>(read<i8>(%6)), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %13: i8 [synthetic] = read<i8>(%6);
// DEFAULT-NEXT:                 let %14: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%13)), const<i32>(1)));
// DEFAULT-NEXT:                 write<i8>(%6, read<i8>(%14));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%7)), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%5, from_bool<i32, reason=assign>(eq<i32>(read<i32>(%2), const<i32>(0))));
// DEFAULT-NEXT:         let %15: ptr<i32> [synthetic] = read<ptr<i32>>(%7);
// DEFAULT-NEXT:         let %16: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%15)));
// DEFAULT-NEXT:         let %17: i32 [synthetic] = xor<i32>(read<i32>(%16), read<i32>(%5));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%15)), read<i32>(%17));
// DEFAULT-NEXT:         let %10 h: i32 [storage=automatic] = not<i32>(read<i32>(%4));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%3, read<i32>(%10));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
