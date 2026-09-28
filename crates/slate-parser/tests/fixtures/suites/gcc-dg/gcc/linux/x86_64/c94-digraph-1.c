/* Test for recognition of digraphs: should be recognized in C94 and C99
   mode, but not in C90 mode.  Also check correct stringizing.
*/
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do run } */
/* { dg-options "-std=iso9899:199409 -pedantic-errors" } */

#define str(x) xstr(x)
#define xstr(x) #x
#define foo(p, q) str(p %:%: q)

extern void abort (void);
extern int strcmp (const char *, const char *);

int
main (void)
{
  const char *t = foo (1, 2);
  const char *u = str (<:);
  if (strcmp (t, "12") || strcmp (u, "<:"))
    abort ();
  else
    return 0;
}

// SLATE-FILECHECK-STD DEFAULT iso9899:199409
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
// DEFAULT-NEXT:     global %7 .str7: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([49, 32, 37, 58, 37, 58, 32, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([60, 58, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([49, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([60, 58, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @strcmp(%5 <unnamed>: ptr<const i8>, %6 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 t: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(9)>(%7));
// DEFAULT-NEXT:         let %4 u: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%8));
// DEFAULT-NEXT:         let %11: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%1, read<ptr<const i8>>(%3), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%9))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%11, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%11, ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%1, read<ptr<const i8>>(%4), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%10))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%11)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
