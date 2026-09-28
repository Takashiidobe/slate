/* Regression test for a cpplib macro-expansion bug where
   `@' becomes `@@' when stringified.  */

/* { dg-do run } */

#include <string.h>
#include <stdlib.h>

#define STR(x) #x

char *a = STR(@foo), *b = "@foo";

int
main(void)
{
  if (strcmp (a, b))
    abort ();
  return 0;
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     global %7 .str7: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([64, 102, 111, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %2 a: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(5)>(%7) [linkage=external];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([64, 102, 111, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %3 b: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(5)>(%8) [linkage=external];
// DEFAULT-NEXT:     fn %0 @strcmp(%5 __s1: ptr<const i8>, %6 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%2)), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%3))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
