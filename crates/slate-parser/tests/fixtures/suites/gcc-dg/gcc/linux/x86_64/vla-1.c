/* { dg-do compile } */
/* { dg-options "-g -O3 -fdump-tree-optimized -fvar-tracking-assignments -fno-selective-scheduling -fno-selective-scheduling2 -fno-ipa-vrp --param ipa-cp-eval-threshold=1" } */

static int __attribute__((noinline))
f1 (int i)
{
  char a[i + 1];
  char b[i + 2];
  b[1] = 3;
  a[0] = 5;
  return a[0] + b[1];
}

int
main ()
{
  volatile int j;
  int x = 5;
  asm volatile ("" : "+r" (x));
  j = f1 (x);
  asm volatile ("" : "+r" (x));
  return 0;
}

/* One debug source bind is generated for the parameter, and one to describe the
   sizes of a and b.  */
/* { dg-final { scan-tree-dump-times " s=> i" 2 "optimized" } } */

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
// DEFAULT-NEXT:     fn %0 @f1(%1 i: i32) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%1), const<i32>(1))));
// DEFAULT-NEXT:         let %2 a: vla<i8, %7> [storage=automatic];
// DEFAULT-NEXT:         let %8: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%1), const<i32>(2))));
// DEFAULT-NEXT:         let %3 b: vla<i8, %8> [storage=automatic];
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(%3), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(%2), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(5)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(%2), const<i32>(0))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(%3), const<i32>(1))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 j: volatile i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 x: i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 32 place<i32>(%6);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32, volatile>(%5, call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%6)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%6));
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 32 place<i32>(%6);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
