/* PR debug/45003 */
/* { dg-do run { target { { i?86-*-* x86_64-*-* } && lp64 } } } */
/* { dg-options "-g" } */

int __attribute__((noinline)) foo(unsigned short *p) {
  int a = *p;
  asm volatile("nop");
  asm volatile("nop" : : "D"(a)); /* { dg-final { gdb-test . "a" "0x8078" } } */
  return 0;
}

int __attribute__((noinline)) bar(short *p) {
  unsigned int a = *p;
  asm volatile("nop");
  asm volatile("nop"
               :
               : "D"(a)); /* { dg-final { gdb-test . "a" "0xffff8078" } } */
  return 0;
}

int
main() {
  unsigned short us = 0x8078;
  foo(&us);
  short s = -32648;
  bar(&s);
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
// DEFAULT-NEXT:     fn %0 @foo(%1 p: ptr<u16>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 a: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u16>(deref(read<ptr<u16>>(%1)))));
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att];
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             in 0 "D" [{di}] read<i32>(%2);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar(%4 p: ptr<i16>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=unknown>(widen<i32, reason=assign>(read<i16>(deref(read<ptr<i16>>(%4)))));
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att];
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             in 0 "D" [{di}] read<u32>(%5);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 us: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(const<i32>(32888)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<u16>) -> i32>(%0, addr_of<ptr<u16>>(%7));
// DEFAULT-NEXT:         let %8 s: i16 [storage=automatic] = truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(32648)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i16>) -> i32>(%3, addr_of<ptr<i16>>(%8));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
