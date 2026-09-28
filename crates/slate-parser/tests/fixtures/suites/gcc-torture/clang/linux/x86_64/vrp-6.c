/* { dg-require-effective-target int32plus } */
#include <limits.h>

extern void exit(int);
extern void abort();

void test01(unsigned int a, unsigned int b) {
  if (a < 5)
    abort();
  if (b < 5)
    abort();
  if (a - b != 5)
    abort();
}

void test02(unsigned int a, unsigned int b) {
  if (a >= 12)
    if (b > 15)
      if (a - b < UINT_MAX - 15U)
        abort();
}

int main(int argc, char *argv[]) {
  unsigned x = 0x80000000;
  test01(x + 5, x);
  test02(14, 16);
  exit(0);
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
// DEFAULT-NEXT:     fn %0 @exit(%12 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @test01(%3 a: u32, %4 b: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if lt<u32>(read<u32>(%3), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if lt<u32>(read<u32>(%4), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<u32>(sub<u32, overflow=wrap>(read<u32>(%3), read<u32>(%4)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test02(%6 a: u32, %7 b: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u32>(read<u32>(%6), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(12)))
// DEFAULT-NEXT:             if gt<u32>(read<u32>(%7), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15)))
// DEFAULT-NEXT:                 if lt<u32>(sub<u32, overflow=wrap>(read<u32>(%6), read<u32>(%7)), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1)), const<u32>(15)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main(%9 argc: i32, %10 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 x: u32 [storage=automatic] = const<u32>(2147483648);
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%2, add<u32, overflow=wrap>(read<u32>(%11), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5))), read<u32>(%11));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%5, reinterpret<u32, reason=arg, fits=always>(const<i32>(14)), reinterpret<u32, reason=arg, fits=always>(const<i32>(16)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
