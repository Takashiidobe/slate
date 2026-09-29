/* Test for typeof evaluation: should be at the appropriate point in
   the containing expression rather than just adding a statement.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

extern void exit (int);
extern void abort (void);

void *p;

void
f1 (void)
{
  int i = 0, j = -1, k = -1;
  /* typeof applied to expression with cast.  */
  (j = ++i), (void)(typeof ((int (*)[(k = ++i)])p))p;
  if (j != 1 || k != 2 || i != 2)
    abort ();
}

void
f2 (void)
{
  int i = 0, j = -1, k = -1;
  /* typeof applied to type.  */
  (j = ++i), (void)(typeof (int (*)[(k = ++i)]))p;
  if (j != 1 || k != 2 || i != 2)
    abort ();
}

void
f3 (void)
{
  int i = 0, j = -1, k = -1;
  void *q;
  /* typeof applied to expression with cast that is used.  */
  (j = ++i), (void)((typeof (1 + (int (*)[(k = ++i)])p))p);
  if (j != 1 || k != 2 || i != 2)
    abort ();
}

int
main (void)
{
  f1 ();
  f2 ();
  f3 ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     global %2 p: ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%17 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @f1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %5 j: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %6 k: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %23: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:         let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%24));
// DEFAULT-NEXT:         write<i32>(%5, read<i32>(%24));
// DEFAULT-NEXT:         let %25: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:         let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%26));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%26));
// DEFAULT-NEXT:         let %18: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%26)));
// DEFAULT-NEXT:         let %19: ptr<vla<i32, %18>> [synthetic] = pointer_cast<ptr<vla<i32, %18>>, reason=explicit>(read<ptr<void>>(%2));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%5), const<i32>(1)), ne<i32>(read<i32>(%6), const<i32>(2))), ne<i32>(read<i32>(%4), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @f2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %9 j: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %10 k: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %27: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%28));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%28));
// DEFAULT-NEXT:         let %29: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%30));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%30));
// DEFAULT-NEXT:         let %20: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%30)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%9), const<i32>(1)), ne<i32>(read<i32>(%10), const<i32>(2))), ne<i32>(read<i32>(%8), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @f3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %13 j: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %14 k: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %15 q: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %31: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:         let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%12, read<i32>(%32));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%32));
// DEFAULT-NEXT:         let %33: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:         let %34: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%12, read<i32>(%34));
// DEFAULT-NEXT:         write<i32>(%14, read<i32>(%34));
// DEFAULT-NEXT:         let %21: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%34)));
// DEFAULT-NEXT:         let %22: ptr<vla<i32, %21>> [synthetic] = ptr_offset<ptr<vla<i32, %21>>, subtract=false, element=vla<i32, %21>, overflow=ub>(pointer_cast<ptr<vla<i32, %21>>, reason=explicit>(read<ptr<void>>(%2)), const<i32>(1));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%13), const<i32>(1)), ne<i32>(read<i32>(%14), const<i32>(2))), ne<i32>(read<i32>(%12), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
