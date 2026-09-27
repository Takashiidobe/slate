/* PR debug/54970 */
/* PR debug/54971 */
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

int
main() {
  int a[] = {
      1, 2,
      3}; /* { dg-final { gdb-test .+4 "a\[0\]" "1" { xfail { no-opts "-O0" "-Og" } } } } */
  int *p = a + 2; /* { dg-final { gdb-test .+3 "a\[1\]" "2" } } */
  int *q = a + 1; /* { dg-final { gdb-test .+2 "a\[2\]" "3" } } */
  /* { dg-final { gdb-test .+1 "*p" "3" } } */
  asm volatile(NOP); /* { dg-final { gdb-test . "*q" "2" } } */
  *p +=
      10; /* { dg-final { gdb-test .+4 "a\[0\]" "1" { xfail { no-opts "-O0" "-Og" } } } } */
  /* { dg-final { gdb-test .+3 "a\[1\]" "2" } } */
  /* { dg-final { gdb-test .+2 "a\[2\]" "13" } } */
  /* { dg-final { gdb-test .+1 "*p" "13" } } */
  asm volatile(NOP); /* { dg-final { gdb-test . "*q" "2" } } */
  *q +=
      10; /* { dg-final { gdb-test .+4 "a\[0\]" "1" { xfail { no-opts "-O0" "-Og" } } } } */
  /* { dg-final { gdb-test .+3 "a\[1\]" "12" } } */
  /* { dg-final { gdb-test .+2 "a\[2\]" "13" } } */
  /* { dg-final { gdb-test .+1 "*p" "13" } } */
  asm volatile(NOP); /* { dg-final { gdb-test . "*q" "12" } } */
  __builtin_memcpy(&a, (int[3]){4, 5, 6}, sizeof(a));
  /* { dg-final { gdb-test .+4 "a\[0\]" "4" { xfail { no-opts "-O0" "-Og" } } } } */
  /* { dg-final { gdb-test .+3 "a\[1\]" "5" } } */
  /* { dg-final { gdb-test .+2 "a\[2\]" "6" } } */
  /* { dg-final { gdb-test .+1 "*p" "6" } } */
  asm volatile(NOP); /* { dg-final { gdb-test . "*q" "5" } } */
  *p +=
      20; /* { dg-final { gdb-test .+4 "a\[0\]" "4" { xfail { no-opts "-O0" "-Og" } } } } */
  /* { dg-final { gdb-test .+3 "a\[1\]" "5" } } */
  /* { dg-final { gdb-test .+2 "a\[2\]" "26" } } */
  /* { dg-final { gdb-test .+1 "*p" "26" } } */
  asm volatile(NOP); /* { dg-final { gdb-test . "*q" "5" } } */
  *q +=
      20; /* { dg-final { gdb-test .+8 "a\[0\]" "4" { xfail { no-opts "-O0" "-Og" } } } } */
  /* { dg-final { gdb-test .+7 "a\[1\]" "25" } } */
  /* { dg-final { gdb-test .+6 "a\[2\]" "26" } } */
  /* { dg-final { gdb-test .+5 "*p" "26" } } */
  /* { dg-final { gdb-test .+4 "p\[-1\]" "25" } } */
  /* { dg-final { gdb-test .+3 "p\[-2\]" "4" { xfail { no-opts "-O0" "-Og" } } } } */
  /* { dg-final { gdb-test .+2 "q\[-1\]" "4" { xfail { no-opts "-O0" "-Og" } } } } */
  /* { dg-final { gdb-test .+1 "q\[1\]" "26" } } */
  asm volatile(NOP); /* { dg-final { gdb-test . "*q" "25" } } */
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
// DEFAULT-NEXT:     fn %7 @__builtin_memcpy(%4 <unnamed>: ptr<void>, %5 <unnamed>: ptr<const void>, %6 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %0 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %1 a: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3));
// DEFAULT-NEXT:         let %2 p: ptr<i32> [storage=automatic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%1), const<i32>(2));
// DEFAULT-NEXT:         let %3 q: ptr<i32> [storage=automatic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%1), const<i32>(1));
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att];
// DEFAULT-NEXT:         let %9: ptr<i32> [synthetic] = read<ptr<i32>>(%2);
// DEFAULT-NEXT:         let %10: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%9)));
// DEFAULT-NEXT:         let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(10));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%9)), read<i32>(%11));
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att];
// DEFAULT-NEXT:         let %12: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:         let %13: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%12)));
// DEFAULT-NEXT:         let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(10));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%12)), read<i32>(%14));
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%7, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<array<i32, 3>>>(%1)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(3)>(compound_literal %8 [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(4), index1 = const<i32>(5), index2 = const<i32>(6)))), const<u64>(12));
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att];
// DEFAULT-NEXT:         let %15: ptr<i32> [synthetic] = read<ptr<i32>>(%2);
// DEFAULT-NEXT:         let %16: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%15)));
// DEFAULT-NEXT:         let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(20));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%15)), read<i32>(%17));
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att];
// DEFAULT-NEXT:         let %18: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:         let %19: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%18)));
// DEFAULT-NEXT:         let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(20));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%18)), read<i32>(%20));
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att];
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
