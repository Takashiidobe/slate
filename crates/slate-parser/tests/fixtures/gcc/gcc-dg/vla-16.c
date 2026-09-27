/* Test for modifying and taking addresses of compound literals whose
   variably modified types involve typeof.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

#include <stdarg.h>

extern void exit (int);
extern void abort (void);

int a[1];

void
f1 (void)
{
  int i = 0;
  int (**p)[1] = &(typeof (++i, (int (*)[i])a)){&a};
  if (*p != &a)
    abort ();
  if (i != 1)
    abort ();
}

void
f2 (void)
{
  int i = 0;
  (typeof (++i, (int (*)[i])a)){&a} = 0;
  if (i != 1)
    abort ();
}

void
f3 (void)
{
  int i = 0;
  (typeof (++i, (int (*)[i])a)){&a} += 1;
  if (i != 1)
    abort ();
}

void
f4 (void)
{
  int i = 0;
  --(typeof (++i, (int (*)[i])a)){&a + 1};
  if (i != 1)
    abort ();
}

void
f5 (void)
{
  int i = 0;
  (typeof (++i, (int (*)[i])a)){&a}++;
  if (i != 1)
    abort ();
}

int
main (void)
{
  f1 ();
  f2 ();
  f3 ();
  f4 ();
  f5 ();
  exit (0);
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     global %2 a: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%15 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @f1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %5 p: ptr<ptr<array<i32, 1>>> [storage=automatic] = pointer_cast<ptr<ptr<array<i32, 1>>>, reason=assign>(addr_of<ptr<ptr<vla<i32, %16>>>>(compound_literal %16 [storage=automatic] = pointer_cast<ptr<vla<i32, %16>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%2))));
// DEFAULT-NEXT:         if ne<ptr<array<i32, 1>>>(read<ptr<array<i32, 1>>>(deref(read<ptr<ptr<array<i32, 1>>>>(%5))), addr_of<ptr<array<i32, 1>>>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<ptr<vla<i32, %17>>>(compound_literal %17 [storage=automatic] = pointer_cast<ptr<vla<i32, %17>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%2)), null<ptr<vla<i32, %17>>>);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%7), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @f3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %21: ptr<vla<i32, %18>> [synthetic] = read<ptr<vla<i32, %18>>>(compound_literal %18 [storage=automatic] = pointer_cast<ptr<vla<i32, %18>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%2)));
// DEFAULT-NEXT:         let %22: ptr<vla<i32, %18>> [synthetic] = ptr_offset<ptr<vla<i32, %18>>, subtract=false, element=vla<i32, %18>, overflow=ub>(read<ptr<vla<i32, %18>>>(%21), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<vla<i32, %18>>>(compound_literal %18 [storage=automatic] = pointer_cast<ptr<vla<i32, %18>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%2)), read<ptr<vla<i32, %18>>>(%22));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%9), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @f4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %23: ptr<vla<i32, %19>> [synthetic] = read<ptr<vla<i32, %19>>>(compound_literal %19 [storage=automatic] = pointer_cast<ptr<vla<i32, %19>>, reason=assign>(ptr_offset<ptr<array<i32, 1>>, subtract=false, element=array<i32, 1>, overflow=ub>(addr_of<ptr<array<i32, 1>>>(%2), const<i32>(1))));
// DEFAULT-NEXT:         let %24: ptr<vla<i32, %19>> [synthetic] = ptr_offset<ptr<vla<i32, %19>>, subtract=true, element=vla<i32, %19>, overflow=ub>(read<ptr<vla<i32, %19>>>(%23), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<vla<i32, %19>>>(compound_literal %19 [storage=automatic] = pointer_cast<ptr<vla<i32, %19>>, reason=assign>(ptr_offset<ptr<array<i32, 1>>, subtract=false, element=array<i32, 1>, overflow=ub>(addr_of<ptr<array<i32, 1>>>(%2), const<i32>(1))), read<ptr<vla<i32, %19>>>(%24));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%11), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @f5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %25: ptr<vla<i32, %20>> [synthetic] = read<ptr<vla<i32, %20>>>(compound_literal %20 [storage=automatic] = pointer_cast<ptr<vla<i32, %20>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%2)));
// DEFAULT-NEXT:         let %26: ptr<vla<i32, %20>> [synthetic] = ptr_offset<ptr<vla<i32, %20>>, subtract=false, element=vla<i32, %20>, overflow=ub>(read<ptr<vla<i32, %20>>>(%25), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<vla<i32, %20>>>(compound_literal %20 [storage=automatic] = pointer_cast<ptr<vla<i32, %20>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%2)), read<ptr<vla<i32, %20>>>(%26));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%13), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
