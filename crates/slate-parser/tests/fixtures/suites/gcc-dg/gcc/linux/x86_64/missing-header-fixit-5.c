
/* Forget to include any standard headers, all for built-in functions.
   Rely on -Wimplicit-function-declaration for fixit hints, not on
   -Wbuiltin-declaration-mismatch (which misses abs, isdigit, putchar).  */

/* { dg-options "-fpermissive -fdiagnostics-show-caret -fdiagnostics-show-line-numbers -Wimplicit-function-declaration -Wno-builtin-declaration-mismatch" } */

int
foo (char *m, int i)
{
  if (isdigit (m[0])) /* { dg-warning "implicit declaration of function" } */
  /* { dg-begin-multiline-output "" }
   11 |   if (isdigit (m[0]))
      |       ^~~~~~~
     { dg-end-multiline-output "" } */
  /* { dg-begin-multiline-output "" }
  +++ |+#include <ctype.h>
    1 | 
     { dg-end-multiline-output "" } */
    {
      return abs (i); /* { dg-warning "implicit declaration of function" } */
  /* { dg-begin-multiline-output "" }
   21 |       return abs (i);
      |              ^~~
     { dg-end-multiline-output "" } */
  /* { dg-begin-multiline-output "" }
  +++ |+#include <stdlib.h>
    1 | 
     { dg-end-multiline-output "" } */
    }
  else
    putchar (m[0]); /* { dg-warning "implicit declaration of function" } */
  /* { dg-begin-multiline-output "" }
   32 |     putchar (m[0]);
      |     ^~~~~~~
     { dg-end-multiline-output "" } */
  /* { dg-begin-multiline-output "" }
  +++ |+#include <stdio.h>
    1 | 
     { dg-end-multiline-output "" } */
  return i;
}

// SLATE-FILECHECK-ARGS -fpermissive
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %[[VALUE_isdigit:[0-9]+]] @isdigit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_abs:[0-9]+]] @abs(%[[VALUE1:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_putchar:[0-9]+]] @putchar(%[[VALUE2:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_m:[0-9]+]] m: ptr<i8>, %[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_isdigit]], widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_m]]), const<i32>(0)))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return call<i32, signature=fn(i32) -> i32>(%[[VALUE_abs]], read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(i32) -> i32>(%[[VALUE_putchar]], widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_m]]), const<i32>(0))))));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
