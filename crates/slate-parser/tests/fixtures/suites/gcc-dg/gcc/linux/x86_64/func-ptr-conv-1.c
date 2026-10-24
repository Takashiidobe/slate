/* Conversions between function and object pointers are not permitted
   in any version of ISO C, even with casts, except for the special
   case of converting a null pointer constant to function pointer
   type.  Likewise, comparisons between function and object pointers
   are not permitted.  PR c/11234.  */
/* Origin: Joseph Myers <jsm@polyomino.org.uk> */
/* { dg-do compile } */
/* { dg-options "-fpermissive -pedantic" } */

void f(void);

void *v1 = f; /* { dg-warning "12:pointer" "bad conversion" } */
void *v2 = &f; /* { dg-warning "12:pointer" "bad conversion" } */
void *v3 = (void *)f; /* { dg-warning "12:pointer" "bad conversion" } */
void *v4 = (void *)&f; /* { dg-warning "12:pointer" "bad conversion" } */
void *v5;
char *c1 = f; /* { dg-warning "12:pointer" "bad conversion" } */
char *c2 = &f; /* { dg-warning "12:pointer" "bad conversion" } */
char *c3 = (char *)f; /* { dg-warning "12:pointer" "bad conversion" } */
char *c4 = (char *)&f; /* { dg-warning "12:pointer" "bad conversion" } */
char *c5;
void (*fp)(void);
int a;

void
g(void)
{
  v5 = f; /* { dg-warning "6:pointer" "bad conversion" } */
  v5 = &f; /* { dg-warning "6:pointer" "bad conversion" } */
  v5 = (void *)f; /* { dg-warning "8:pointer" "bad conversion" } */
  v5 = (void *)&f; /* { dg-warning "8:pointer" "bad conversion" } */
  c5 = f; /* { dg-warning "6:pointer" "bad conversion" } */
  c5 = &f; /* { dg-warning "6:pointer" "bad conversion" } */
  c5 = (char *)f; /* { dg-warning "8:pointer" "bad conversion" } */
  c5 = (char *)&f; /* { dg-warning "8:pointer" "bad conversion" } */
  fp = v5; /* { dg-warning "6:pointer" "bad conversion" } */
  fp = c5; /* { dg-warning "6:pointer" "bad conversion" } */
  fp = (void (*)(void))v5; /* { dg-warning "8:pointer" "bad conversion" } */
  fp = (void (*)(void))c5; /* { dg-warning "8:pointer" "bad conversion" } */
  (a ? f : v3); /* { dg-warning "10:pointer" "bad conversion" } */
  (a ? v2 : fp); /* { dg-warning "11:pointer" "bad conversion" } */
  /* The following are OK.  */
  fp = 0;
  fp = (void *)0;
  fp = 0L;
  fp = (void (*)(void))0;
  fp = (void (*)(void))(void *)0;
  (a ? f : 0);
  (a ? f : (void *)0);
  (a ? (void *)0 : fp);
  (a ? 0 : fp);
}

/* The following are OK.  */
void (*fp2)(void) = 0;
void (*fp3)(void) = (void *)0;

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
// DEFAULT-NEXT:     global %[[VALUE_v1:[0-9]+]] v1: ptr<void> [storage=static] = pointer_cast<ptr<void>, reason=assign>(function_decay<ptr<fn() -> void>>(%[[VALUE_f:[0-9]+]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v2:[0-9]+]] v2: ptr<void> [storage=static] = pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<fn() -> void>>(%[[VALUE_f]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v3:[0-9]+]] v3: ptr<void> [storage=static] = pointer_cast<ptr<void>, reason=explicit>(function_decay<ptr<fn() -> void>>(%[[VALUE_f]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v4:[0-9]+]] v4: ptr<void> [storage=static] = pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<fn() -> void>>(%[[VALUE_f]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v5:[0-9]+]] v5: ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c1:[0-9]+]] c1: ptr<i8> [storage=static] = pointer_cast<ptr<i8>, reason=assign>(function_decay<ptr<fn() -> void>>(%[[VALUE_f]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c2:[0-9]+]] c2: ptr<i8> [storage=static] = pointer_cast<ptr<i8>, reason=assign>(addr_of<ptr<fn() -> void>>(%[[VALUE_f]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c3:[0-9]+]] c3: ptr<i8> [storage=static] = pointer_cast<ptr<i8>, reason=explicit>(function_decay<ptr<fn() -> void>>(%[[VALUE_f]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c4:[0-9]+]] c4: ptr<i8> [storage=static] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<fn() -> void>>(%[[VALUE_f]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c5:[0-9]+]] c5: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_fp:[0-9]+]] fp: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_fp2:[0-9]+]] fp2: ptr<fn() -> void> [storage=static] = null<ptr<fn() -> void>> [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_fp3:[0-9]+]] fp3: ptr<fn() -> void> [storage=static] = null<ptr<fn() -> void>> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f]] @f() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_v5]], pointer_cast<ptr<void>, reason=assign>(function_decay<ptr<fn() -> void>>(%[[VALUE_f]])));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_v5]], pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<fn() -> void>>(%[[VALUE_f]])));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_v5]], pointer_cast<ptr<void>, reason=explicit>(function_decay<ptr<fn() -> void>>(%[[VALUE_f]])));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_v5]], pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<fn() -> void>>(%[[VALUE_f]])));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_c5]], pointer_cast<ptr<i8>, reason=assign>(function_decay<ptr<fn() -> void>>(%[[VALUE_f]])));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_c5]], pointer_cast<ptr<i8>, reason=assign>(addr_of<ptr<fn() -> void>>(%[[VALUE_f]])));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_c5]], pointer_cast<ptr<i8>, reason=explicit>(function_decay<ptr<fn() -> void>>(%[[VALUE_f]])));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_c5]], pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<fn() -> void>>(%[[VALUE_f]])));
// DEFAULT-NEXT:         write<ptr<fn() -> void>>(%[[VALUE_fp]], pointer_cast<ptr<fn() -> void>, reason=assign>(read<ptr<void>>(%[[VALUE_v5]])));
// DEFAULT-NEXT:         write<ptr<fn() -> void>>(%[[VALUE_fp]], pointer_cast<ptr<fn() -> void>, reason=assign>(read<ptr<i8>>(%[[VALUE_c5]])));
// DEFAULT-NEXT:         write<ptr<fn() -> void>>(%[[VALUE_fp]], pointer_cast<ptr<fn() -> void>, reason=explicit>(read<ptr<void>>(%[[VALUE_v5]])));
// DEFAULT-NEXT:         write<ptr<fn() -> void>>(%[[VALUE_fp]], pointer_cast<ptr<fn() -> void>, reason=explicit>(read<ptr<i8>>(%[[VALUE_c5]])));
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(function_decay<ptr<fn() -> void>>(%[[VALUE_f]])), read<ptr<void>>(%[[VALUE_v3]]));
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)), read<ptr<void>>(%[[VALUE_v2]]), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<fn() -> void>>(%[[VALUE_fp]])));
// DEFAULT-NEXT:         write<ptr<fn() -> void>>(%[[VALUE_fp]], null<ptr<fn() -> void>>);
// DEFAULT-NEXT:         write<ptr<fn() -> void>>(%[[VALUE_fp]], null<ptr<fn() -> void>>);
// DEFAULT-NEXT:         write<ptr<fn() -> void>>(%[[VALUE_fp]], null<ptr<fn() -> void>>);
// DEFAULT-NEXT:         write<ptr<fn() -> void>>(%[[VALUE_fp]], null<ptr<fn() -> void>>);
// DEFAULT-NEXT:         write<ptr<fn() -> void>>(%[[VALUE_fp]], null<ptr<fn() -> void>>);
// DEFAULT-NEXT:         conditional<ptr<fn() -> void>>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)), function_decay<ptr<fn() -> void>>(%[[VALUE_f]]), null<ptr<fn() -> void>>);
// DEFAULT-NEXT:         conditional<ptr<fn() -> void>>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)), function_decay<ptr<fn() -> void>>(%[[VALUE_f]]), null<ptr<fn() -> void>>);
// DEFAULT-NEXT:         conditional<ptr<fn() -> void>>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)), null<ptr<fn() -> void>>, read<ptr<fn() -> void>>(%[[VALUE_fp]]));
// DEFAULT-NEXT:         conditional<ptr<fn() -> void>>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)), null<ptr<fn() -> void>>, read<ptr<fn() -> void>>(%[[VALUE_fp]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
