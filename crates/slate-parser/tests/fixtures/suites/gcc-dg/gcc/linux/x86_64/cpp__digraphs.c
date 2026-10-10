/* Copyright (C) 2000 Free Software Foundation, Inc.  */

/* { dg-do run } */
/* { dg-options "-std=c99" } */

/* Fully test the 6 digraphs under c99 assumptions.  Four are pasted,
   to check that digraph pasting works.  */

extern int strcmp (const char *, const char *);
extern void abort (void);
#if DEBUG
extern int puts (const char *);
#else
#define puts(X)
#endif
#define err(str) do { puts(str); abort(); } while (0)

%:define glue(x, y) x %:%: y	/* #define glue(x, y) x ## y. */
#ifndef glue
#error glue not defined!
#endif
%:define str(x) %:x		/* #define str(x) #x */

int main (int argc, char *argv<::>) /* argv[] */
glue (<, %) /* { */
             /* di_str[] = */
  const char di_str glue(<, :)glue(:, >) = str(%:%:<::><%%>%:);

  /* Check the glue macro actually pastes, and that the spelling of
     all digraphs is preserved.  */
  if (glue(str, cmp) (di_str, "%:%:<::><%%>%:"))
    err ("Digraph spelling not preserved!");

  return 0;
glue (%, >) /* } */

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([37, 58, 37, 58, 60, 58, 58, 62, 60, 37, 37, 62, 37, 58, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_di_str:[0-9]+]] di_str: array<i8, 15> [storage=automatic] [const] = code_units<array<i8, 15>>([37, 58, 37, 58, 60, 58, 58, 62, 60, 37, 37, 62, 37, 58, 0]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], array_decay<ptr<const i8>, length=Some(15)>(%[[VALUE_di_str]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%[[VALUE_str]]))), const<i32>(0))
// DEFAULT-NEXT:             do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
