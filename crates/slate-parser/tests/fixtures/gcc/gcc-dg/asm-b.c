/* { dg-do run  { target powerpc*-*-* i?86-*-* x86_64-*-* } } */
/* { dg-options "-O1" } */
/* Test to make sure that inline-asm causes the tree optimizers to get the
   V_MAY_DEFs and clobber memory.  */
/* Test from Jakub Jelinek, modified by Andrew Pinski to work on all powerpc targets.  */
extern void abort (void);

unsigned short v = 0x0300;

void
foo (unsigned short *p)
{
  *p = v;
}

int
bar (void)
{
  unsigned short x;
  volatile unsigned short *z;
  foo (&x);
  const unsigned int y = x;
  z = &x;
#if defined (__powerpc__) || defined (__PPC__) || defined (__ppc__) || defined (_POWER) || defined (__ppc64__) || defined (__ppc)
  __asm __volatile ("sthbrx %1,0,%2" : "=m" (*z) : "r" (y), "r" (z));
#elif defined __i386__ || defined __x86_64__
  __asm __volatile ("movb %b1,1(%2)\n\tmovb %h1,(%2)"
		    : "=m" (*z) : "Q" (y), "R" (z));
#endif
  return (x & 1) == 0;
}

int
main (void)
{
  if (bar ())
    abort ();
  return 0;
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     global %1 v: u16 [storage=static] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(768))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @foo(%3 p: ptr<u16>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u16>(deref(read<ptr<u16>>(%3)), read<u16>(%1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 x: u16 [storage=automatic];
// DEFAULT-NEXT:         let %6 z: ptr<volatile u16> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u16>) -> void>(%2, addr_of<ptr<u16>>(%5));
// DEFAULT-NEXT:         let %7 y: u32 [storage=automatic] [const] = widen<u32, reason=assign>(read<u16>(%5));
// DEFAULT-NEXT:         write<ptr<volatile u16>>(%6, pointer_cast<ptr<volatile u16>, reason=assign>(addr_of<ptr<u16>>(%5)));
// DEFAULT-NEXT:         asm volatile "movb %b1,1(%2)\\n\\tmovb %h1,(%2)" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             template: "movb " %b1(8) ",1(" %2 ")\\n\\tmovb " %h1(high8) ",(" %2 ")";
// DEFAULT-NEXT:             lateout 0 "m" [mem] width 16 place<u16, volatile>(deref(read<ptr<volatile u16>>(%6)));
// DEFAULT-NEXT:             in 1 "Q" [reg_abcd] width 32 read<u32>(%7);
// DEFAULT-NEXT:             in 2 "R" [reg_legacy] width 64 read<ptr<volatile u16>>(%6);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%5))), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%4), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
