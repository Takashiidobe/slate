/* { dg-do run } */

/* Regression test for stringizing and token pasting.
   We got internal escape markers in the strings.  */

#include <string.h>
#include <stdlib.h>

#define S(x) _S(x)
#define _S(x) #x

#define I 1
static const char s1[] = S(I.1);
static const char t1[] = "1.1";

#define f h
#define h(a) a+f
static const char s2[] = S( f(1)(2) );
static const char t2[] = "1+h(2)";

#undef I
#undef f
#undef h

int
main(void)
{
  if (strcmp (s1, t1))
    abort ();

  if (strcmp (s2, t2))
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
// DEFAULT-NEXT:     global %[[VALUE_s1:[0-9]+]] s1: array<i8, 4> [storage=static] [const] = code_units<array<i8, 4>>([49, 46, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_t1:[0-9]+]] t1: array<i8, 4> [storage=static] [const] = code_units<array<i8, 4>>([49, 46, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s2:[0-9]+]] s2: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([49, 43, 102, 40, 50, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_t2:[0-9]+]] t2: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([49, 43, 104, 40, 50, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE___s1:[0-9]+]] __s1: ptr<const i8>, %[[VALUE___s2:[0-9]+]] __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_s1]]), array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_t1]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], array_decay<ptr<const i8>, length=Some(7)>(%[[VALUE_s2]]), array_decay<ptr<const i8>, length=Some(7)>(%[[VALUE_t2]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
