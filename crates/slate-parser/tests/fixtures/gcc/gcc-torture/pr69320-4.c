#include <stdlib.h>

int   a;
char  b, d;
short c;
short fn1(int p1, int p2) { return p2 >= 2 ? p1 : p1 > p2; }

int main() {
  int *e = &a, *f = &a;
  b = 1;
  for (; b <= 9; b++) {
    c  = *e != 5 || d;
    *f = fn1(c || b, a);
  }
  if ((long long)a != 1)
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
// DEFAULT-NEXT:     global %2 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 b: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 c: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%12 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @fn1(%7 p1: i32, %8 p2: i32) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(conditional<i32>(ge<i32>(read<i32>(%8), const<i32>(2)), read<i32>(%7), from_bool<i32, reason=promotion>(gt<i32>(read<i32>(%7), read<i32>(%8)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 e: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%2);
// DEFAULT-NEXT:         let %11 f: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%2);
// DEFAULT-NEXT:         write<i8>(%3, truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: le<i32>(widen<i32, reason=promotion>(read<i8>(%3)), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:                 let %15: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%14)), const<i32>(1)));
// DEFAULT-NEXT:                 write<i8>(%3, read<i8>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i16>(%5, from_bool<i16, reason=assign>(logical_or<bool>(ne<i32>(read<i32>(deref(read<ptr<i32>>(%10))), const<i32>(5)), ne<i8>(read<i8>(%4), const<i8>(0)))));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%11)), widen<i32, reason=assign>(call<i16, signature=fn(i32, i32) -> i16>(%6, from_bool<i32, reason=arg>(logical_or<bool>(ne<i16>(read<i16>(%5), const<i16>(0)), ne<i8>(read<i8>(%3), const<i8>(0)))), read<i32>(%2))));
// DEFAULT-NEXT:                     widen<i32, reason=assign>(call<i16, signature=fn(i32, i32) -> i16>(%6, from_bool<i32, reason=arg>(logical_or<bool>(ne<i16>(read<i16>(%5), const<i16>(0)), ne<i8>(read<i8>(%3), const<i8>(0)))), read<i32>(%2)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i64>(widen<i64, reason=explicit>(read<i32>(%2)), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
