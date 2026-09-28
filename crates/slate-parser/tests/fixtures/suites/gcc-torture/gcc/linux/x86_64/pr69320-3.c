#include <stdlib.h>

static int a[40] = {7, 5, 3, 3, 0, 0, 3};
short      b;
int        c = 5;
int        main() {
  b = 0;
  for (; b <= 3; b++)
    if (a[b + 6] ^ (0 || c))
      ;
    else
      break;
  if (b != 4)
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
// DEFAULT-NEXT:     global %3 a: array<i32, 40> [storage=static] [align=16] = aggregate<array<i32, 40>, zero_fill=true>(index0 = const<i32>(7), index1 = const<i32>(5), index2 = const<i32>(3), index3 = const<i32>(3), index4 = const<i32>(0), index5 = const<i32>(0), index6 = const<i32>(3)) [linkage=internal];
// DEFAULT-NEXT:     global %4 b: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 c: i32 [storage=static] = const<i32>(5) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @exit(%7 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i16>(%4, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: le<i32>(widen<i32, reason=promotion>(read<i16>(%4)), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %9: i16 [synthetic] = read<i16>(%4);
// DEFAULT-NEXT:                 let %10: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%9)), const<i32>(1)));
// DEFAULT-NEXT:                 write<i16>(%4, read<i16>(%10));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(xor<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(40)>(%3), add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%4)), const<i32>(6))))), from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(const<i32>(0), const<i32>(0)), ne<i32>(read<i32>(%5), const<i32>(0))))), const<i32>(0))
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     break %8;
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%4)), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
