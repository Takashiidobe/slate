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
// DEFAULT-NEXT:         let %5 p: ptr<ptr<array<i32, 1>>> [storage=automatic];
// DEFAULT-NEXT:         let %31: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:         let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%32));
// DEFAULT-NEXT:         let %16: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %17: ptr<vla<i32, %16>> [synthetic] = pointer_cast<ptr<vla<i32, %16>>, reason=explicit>(array_decay<ptr<i32>, length=Some(1)>(%2));
// DEFAULT-NEXT:         write<ptr<ptr<array<i32, 1>>>>(%5, pointer_cast<ptr<ptr<array<i32, 1>>>, reason=assign>(addr_of<ptr<ptr<vla<i32, %16>>>>(compound_literal %18 [storage=automatic] = pointer_cast<ptr<vla<i32, %16>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%2)))));
// DEFAULT-NEXT:         if ne<ptr<array<i32, 1>>>(read<ptr<array<i32, 1>>>(deref(read<ptr<ptr<array<i32, 1>>>>(%5))), addr_of<ptr<array<i32, 1>>>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %33: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:         let %34: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(%34));
// DEFAULT-NEXT:         let %19: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%7)));
// DEFAULT-NEXT:         let %20: ptr<vla<i32, %19>> [synthetic] = pointer_cast<ptr<vla<i32, %19>>, reason=explicit>(array_decay<ptr<i32>, length=Some(1)>(%2));
// DEFAULT-NEXT:         write<ptr<vla<i32, %19>>>(compound_literal %21 [storage=automatic] = pointer_cast<ptr<vla<i32, %19>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%2)), null<ptr<vla<i32, %19>>>);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%7), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @f3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %35: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%36));
// DEFAULT-NEXT:         let %22: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%9)));
// DEFAULT-NEXT:         let %23: ptr<vla<i32, %22>> [synthetic] = pointer_cast<ptr<vla<i32, %22>>, reason=explicit>(array_decay<ptr<i32>, length=Some(1)>(%2));
// DEFAULT-NEXT:         let %37: ptr<vla<i32, %22>> [synthetic] = read<ptr<vla<i32, %22>>>(compound_literal %24 [storage=automatic] = pointer_cast<ptr<vla<i32, %22>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%2)));
// DEFAULT-NEXT:         let %38: ptr<vla<i32, %22>> [synthetic] = ptr_offset<ptr<vla<i32, %22>>, subtract=false, element=vla<i32, %22>, overflow=ub>(read<ptr<vla<i32, %22>>>(%37), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<vla<i32, %22>>>(compound_literal %24 [storage=automatic] = pointer_cast<ptr<vla<i32, %22>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%2)), read<ptr<vla<i32, %22>>>(%38));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%9), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @f4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %39: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %40: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%39), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%40));
// DEFAULT-NEXT:         let %25: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%11)));
// DEFAULT-NEXT:         let %26: ptr<vla<i32, %25>> [synthetic] = pointer_cast<ptr<vla<i32, %25>>, reason=explicit>(array_decay<ptr<i32>, length=Some(1)>(%2));
// DEFAULT-NEXT:         let %41: ptr<vla<i32, %25>> [synthetic] = read<ptr<vla<i32, %25>>>(compound_literal %27 [storage=automatic] = pointer_cast<ptr<vla<i32, %25>>, reason=assign>(ptr_offset<ptr<array<i32, 1>>, subtract=false, element=array<i32, 1>, overflow=ub>(addr_of<ptr<array<i32, 1>>>(%2), const<i32>(1))));
// DEFAULT-NEXT:         let %42: ptr<vla<i32, %25>> [synthetic] = ptr_offset<ptr<vla<i32, %25>>, subtract=true, element=vla<i32, %25>, overflow=ub>(read<ptr<vla<i32, %25>>>(%41), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<vla<i32, %25>>>(compound_literal %27 [storage=automatic] = pointer_cast<ptr<vla<i32, %25>>, reason=assign>(ptr_offset<ptr<array<i32, 1>>, subtract=false, element=array<i32, 1>, overflow=ub>(addr_of<ptr<array<i32, 1>>>(%2), const<i32>(1))), read<ptr<vla<i32, %25>>>(%42));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%11), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @f5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %43: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %44: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%43), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%44));
// DEFAULT-NEXT:         let %28: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%13)));
// DEFAULT-NEXT:         let %29: ptr<vla<i32, %28>> [synthetic] = pointer_cast<ptr<vla<i32, %28>>, reason=explicit>(array_decay<ptr<i32>, length=Some(1)>(%2));
// DEFAULT-NEXT:         let %45: ptr<vla<i32, %28>> [synthetic] = read<ptr<vla<i32, %28>>>(compound_literal %30 [storage=automatic] = pointer_cast<ptr<vla<i32, %28>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%2)));
// DEFAULT-NEXT:         let %46: ptr<vla<i32, %28>> [synthetic] = ptr_offset<ptr<vla<i32, %28>>, subtract=false, element=vla<i32, %28>, overflow=ub>(read<ptr<vla<i32, %28>>>(%45), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<vla<i32, %28>>>(compound_literal %30 [storage=automatic] = pointer_cast<ptr<vla<i32, %28>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%2)), read<ptr<vla<i32, %28>>>(%46));
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
