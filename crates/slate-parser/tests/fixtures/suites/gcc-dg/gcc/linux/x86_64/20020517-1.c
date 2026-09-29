/* This testcase caused ICE in do_SUBST on IA-32, because 0xf6 constant
   was not sign-extended for QImode.  */
/* { dg-do run } */
/* { dg-options "-O2" } */
/* { dg-options "-O2 -mtune=i686" { target { { i?86-*-* x86_64-*-* } && ia32 } } } */

#include <limits.h>

void abort (void);
void exit (int);

void foo (void)
{
  int i;
  char *p;

  p = (char *) &i;
  *p = -10;
  if (* (unsigned char *) p != 0x100 - 10)
    abort ();
}

int main (void)
{
  if (UCHAR_MAX == 255)
    foo ();
  exit (0);
}

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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_i]])));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_p]])), truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(10))));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(pointer_cast<ptr<u8>, reason=explicit>(read<ptr<i8>>(%[[VALUE_p]])))))), sub<i32, overflow=ub>(const<i32>(256), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if eq<i32>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(255))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
