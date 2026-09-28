/* PR debug/36728 */
/* { dg-do run } */
/* { dg-options "-g" } */

#if defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#define NOP "nop 0"
#elif defined(__MMIX__)
#define NOP "swym 0"
#elif defined(__or1k__)
#define NOP "l.nop"
#else
#define NOP "nop"
#endif

int __attribute__((noinline)) foo(int arg1, int arg2, int arg3, int arg4,
                                  int arg5, int arg6, int arg7) {
  char                            *x = __builtin_alloca(arg7);
  int __attribute__((aligned(32))) y;

  y = 2;
  asm(NOP : "=m"(y) : "m"(y));
  x[0] = 25;
  asm volatile(NOP : "=m"(x[0]) : "m"(x[0]));
  return y;
}

/* On s390(x) r2 and r3 are (depending on the optimization level) used
   when adjusting the addresses in order to meet the alignment
   requirements above.  They usually hold the function arguments arg1
   and arg2.  So it is expected that these values are unavailable in
   some of these tests.  */

/* { dg-final { gdb-test 14 "arg1" "1" { target { ! "s390*-*-*" } } } } */
/* { dg-final { gdb-test 14 "arg2" "2" { target { ! "s390*-*-*" } } } } */
/* { dg-final { gdb-test 14 "arg3" "3" } } */
/* { dg-final { gdb-test 14 "arg4" "4" } } */
/* { dg-final { gdb-test 14 "arg5" "5" } } */
/* { dg-final { gdb-test 14 "arg6" "6" } } */
/* { dg-final { gdb-test 14 "arg7" "30" } } */
/* { dg-final { gdb-test 14 "y" "2" { xfail { aarch64*-*-* && { any-opts "-O3" } } } } } */
/* { dg-final { gdb-test 16 "arg1" "1" { target { ! "s390*-*-*" } } } } */
/* { dg-final { gdb-test 16 "arg2" "2" { target { ! "s390*-*-*" } } } } */
/* { dg-final { gdb-test 16 "arg3" "3" } } */
/* { dg-final { gdb-test 16 "arg4" "4" } } */
/* { dg-final { gdb-test 16 "arg5" "5" } } */
/* { dg-final { gdb-test 16 "arg6" "6" } } */
/* { dg-final { gdb-test 16 "arg7" "30" } } */
/* { dg-final { gdb-test 16 "*x" "(char) 25" } } */
/* { dg-final { gdb-test 16 "y" "2" } } */

int
main() {
  int l = 0;
  asm volatile("" : "=r"(l) : "0"(l));
  foo(l + 1, l + 2, l + 3, l + 4, l + 5, l + 6, l + 30);
  asm volatile("" ::"r"(l));
  return 0;
}



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
// DEFAULT-NEXT:     fn %13 @__builtin_alloca(%12 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %0 @foo(%1 arg1: i32, %2 arg2: i32, %3 arg3: i32, %4 arg4: i32, %5 arg5: i32, %6 arg6: i32, %7 arg7: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 x: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%13, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%7)))));
// DEFAULT-NEXT:         let %9 y: i32 [storage=automatic] [align=32];
// DEFAULT-NEXT:         write<i32>(%9, const<i32>(2));
// DEFAULT-NEXT:         asm "nop" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             lateout 0 "m" [mem] width 32 place<i32>(%9);
// DEFAULT-NEXT:             in 1 "m" [mem] width 32 place<i32>(%9);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%8), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(25)));
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             lateout 0 "m" [mem] width 8 place<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%8), const<i32>(0))));
// DEFAULT-NEXT:             in 1 "m" [mem] width 8 place<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%8), const<i32>(0))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 l: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 32 place<i32>(%11) from read<i32>(%11);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32, i32, i32, i32, i32) -> i32>(%0, add<i32, overflow=ub>(read<i32>(%11), const<i32>(1)), add<i32, overflow=ub>(read<i32>(%11), const<i32>(2)), add<i32, overflow=ub>(read<i32>(%11), const<i32>(3)), add<i32, overflow=ub>(read<i32>(%11), const<i32>(4)), add<i32, overflow=ub>(read<i32>(%11), const<i32>(5)), add<i32, overflow=ub>(read<i32>(%11), const<i32>(6)), add<i32, overflow=ub>(read<i32>(%11), const<i32>(30)));
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             in 0 "r" [reg] width 32 read<i32>(%11);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
