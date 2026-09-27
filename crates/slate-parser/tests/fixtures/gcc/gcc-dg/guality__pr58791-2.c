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

__attribute__((noinline, noclone)) int foo(unsigned char c) {
  int   ret;
  _Bool a, b, d, e, f;

  a = c == 34;
  b = c == 32;
  d = a | b;
  f = !d;
  if (d)
    ret = 1;
  else {
    e   = c <= 31;
    ret = e;
  }

  asm volatile(
      NOP
      :
      :
      : "memory"); /* { dg-final { gdb-test pr58791-2.c:27 "d & 1" "1" } } */
  asm volatile(
      NOP
      :
      :
      : "memory"); /* { dg-final { gdb-test pr58791-2.c:27 "f & 1" "0" } } */
  return ret;
}

int
main() {
  foo(32);
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
// DEFAULT-NEXT:     fn %0 @foo(%1 c: u8) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 ret: i32 [storage=automatic];
// DEFAULT-NEXT:         let %3 a: bool [storage=automatic];
// DEFAULT-NEXT:         let %4 b: bool [storage=automatic];
// DEFAULT-NEXT:         let %5 d: bool [storage=automatic];
// DEFAULT-NEXT:         let %6 e: bool [storage=automatic];
// DEFAULT-NEXT:         let %7 f: bool [storage=automatic];
// DEFAULT-NEXT:         write<bool>(%3, eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1))), const<i32>(34)));
// DEFAULT-NEXT:         write<bool>(%4, eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1))), const<i32>(32)));
// DEFAULT-NEXT:         write<bool>(%5, ne<i32, reason=assign>(or<i32>(from_bool<i32, reason=promotion>(read<bool>(%3)), from_bool<i32, reason=promotion>(read<bool>(%4))), const<i32>(0)));
// DEFAULT-NEXT:         write<bool>(%7, not<bool>(read<bool>(%5)));
// DEFAULT-NEXT:         if read<bool>(%5)
// DEFAULT-NEXT:             write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<bool>(%6, le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1))), const<i32>(31)));
// DEFAULT-NEXT:                 write<i32>(%2, from_bool<i32, reason=assign>(read<bool>(%6)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(u8) -> i32>(%0, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(32))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
