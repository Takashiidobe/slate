/* PR tree-optimization/58791 */
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

__attribute__((noinline, noclone)) int foo(int x, int y) {
  _Bool a = x != 0;
  _Bool b = y != 2;
  _Bool c = a & b;
  _Bool d = !c;
  int   ret;
  if (c) {
    if (y < 3 || y > 4)
      ret = 1;
    else
      ret = 0;
  } else
    ret = 0;
  asm volatile(
      NOP
      :
      :
      : "memory"); /* { dg-final { gdb-test pr58791-1.c:25 "c & 1" "1" } } */
  asm volatile(
      NOP
      :
      :
      : "memory"); /* { dg-final { gdb-test pr58791-1.c:25 "d & 1" "0" } } */
  return ret;
}

int
main() {
  foo(1, 3);
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: i32, %2 y: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 a: bool [storage=automatic] = ne<i32>(read<i32>(%1), const<i32>(0));
// DEFAULT-NEXT:         let %4 b: bool [storage=automatic] = ne<i32>(read<i32>(%2), const<i32>(2));
// DEFAULT-NEXT:         let %5 c: bool [storage=automatic] = ne<i32, reason=assign>(and<i32>(from_bool<i32, reason=promotion>(read<bool>(%3)), from_bool<i32, reason=promotion>(read<bool>(%4))), const<i32>(0));
// DEFAULT-NEXT:         let %6 d: bool [storage=automatic] = not<bool>(read<bool>(%5));
// DEFAULT-NEXT:         let %7 ret: i32 [storage=automatic];
// DEFAULT-NEXT:         if read<bool>(%5)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(lt<i32>(read<i32>(%2), const<i32>(3)), gt<i32>(read<i32>(%2), const<i32>(4)))
// DEFAULT-NEXT:                     write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32) -> i32>(%0, const<i32>(1), const<i32>(3));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
