/* { dg-do run { target i?86-*-* x86_64-*-* } } */
/* { dg-options "-g" } */

void __attribute__((noipa)) bar(void *p) {}

void __attribute__((noipa)) foo(int i) {
  void *p = __builtin_alloca(i);

  asm volatile("" : : : "ebx");

  bar(p); /* { dg-final { gdb-test . "i" "5" } } */
}

int main(void) {
  foo(5);
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
// DEFAULT-NEXT:     fn %0 @bar(%1 p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @__builtin_alloca(%6 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%3))));
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             clobbers: "ebx" as bx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%0, read<ptr<void>>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(5));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
