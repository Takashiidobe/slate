/* Test null pointer constants: typedefs for void should be OK but not
   qualified void.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1990 -pedantic-errors" } */

typedef void V;
int *p;
long *q;
int j;
void (*fp)(void);

void
f (void)
{
  /* (V *)0 is a null pointer constant, so the assignment should be
     diagnosed.  */
  q = (j ? p : (V *)0); /* { dg-error "5:assignment to 'long int \\*' from incompatible pointer type 'int \\*'" } */
  q = (j ? p : (void *)0); /* { dg-error "5:assignment to 'long int \\*' from incompatible pointer type 'int \\*'" } */
  /* And this conversion should be valid.  */
  (void (*)(void))(V *)0;
  (void (*)(void))(void *)0;
  /* Pointers to qualified void are not valid null pointer
     constants.  */
  fp = (const void *)0; /* { dg-error "6:ISO C forbids assignment between function pointer and 'void \\*'" } */
  fp = (void *)0;
  fp = (V *)0;
  fp = 0;
  fp == 0;
  0 == fp;
  fp == (void *)0;
  (void *)0 == fp;
  fp == (V *)0;
  (V *)0 == fp;
  fp == (V *)1; /* { dg-error "6:ISO C forbids comparison of 'void \\*' with function pointer" } */
  (V *)1 == fp; /* { dg-error "10:ISO C forbids comparison of 'void \\*' with function pointer" } */
  fp == (const void *)0; /* { dg-error "6:ISO C forbids comparison of 'void \\*' with function pointer" } */
  (const void *)0 == fp; /* { dg-error "19:ISO C forbids comparison of 'void \\*' with function pointer" } */
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1990
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
// DEFAULT-NEXT:     type @type0 V = void;
// DEFAULT-NEXT:     global %1 p: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 q: ptr<i64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 fp: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %5 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i64>>(%2, pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<i32>>(ne<i32>(read<i32>(%3), const<i32>(0)), read<ptr<i32>>(%1), null<ptr<i32>>)));
// DEFAULT-NEXT:         write<ptr<i64>>(%2, pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<i32>>(ne<i32>(read<i32>(%3), const<i32>(0)), read<ptr<i32>>(%1), null<ptr<i32>>)));
// DEFAULT-NEXT:         null<ptr<fn() -> void>>;
// DEFAULT-NEXT:         null<ptr<fn() -> void>>;
// DEFAULT-NEXT:         write<ptr<fn() -> void>>(%4, pointer_cast<ptr<fn() -> void>, reason=assign>(null<ptr<const void>>));
// DEFAULT-NEXT:         write<ptr<fn() -> void>>(%4, null<ptr<fn() -> void>>);
// DEFAULT-NEXT:         write<ptr<fn() -> void>>(%4, null<ptr<fn() -> void>>);
// DEFAULT-NEXT:         write<ptr<fn() -> void>>(%4, null<ptr<fn() -> void>>);
// DEFAULT-NEXT:         eq<ptr<fn() -> void>>(read<ptr<fn() -> void>>(%4), null<ptr<fn() -> void>>);
// DEFAULT-NEXT:         eq<ptr<fn() -> void>>(null<ptr<fn() -> void>>, read<ptr<fn() -> void>>(%4));
// DEFAULT-NEXT:         eq<ptr<fn() -> void>>(read<ptr<fn() -> void>>(%4), null<ptr<fn() -> void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(null<ptr<void>>, pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<fn() -> void>>(%4)));
// DEFAULT-NEXT:         eq<ptr<fn() -> void>>(read<ptr<fn() -> void>>(%4), null<ptr<fn() -> void>>);
// DEFAULT-NEXT:         eq<ptr<void>>(null<ptr<void>>, pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<fn() -> void>>(%4)));
// DEFAULT-NEXT:         eq<ptr<fn() -> void>>(read<ptr<fn() -> void>>(%4), pointer_cast<ptr<fn() -> void>, reason=usual_arith>(int_to_ptr<ptr<void>, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:         eq<ptr<void>>(int_to_ptr<ptr<void>, reason=explicit>(const<i32>(1)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<fn() -> void>>(%4)));
// DEFAULT-NEXT:         eq<ptr<fn() -> void>>(read<ptr<fn() -> void>>(%4), null<ptr<fn() -> void>>);
// DEFAULT-NEXT:         eq<ptr<const void>>(null<ptr<const void>>, pointer_cast<ptr<const void>, reason=usual_arith>(read<ptr<fn() -> void>>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
