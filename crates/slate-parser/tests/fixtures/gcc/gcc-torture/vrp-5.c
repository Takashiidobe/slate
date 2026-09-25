/* { dg-require-effective-target int32plus } */
extern void exit(int);
extern void abort();

void test(unsigned int a, unsigned int b) {
  if (a < 5)
    abort();
  if (b < 5)
    abort();
  if (a + b != 0U)
    abort();
}

int main(int argc, char *argv[]) {
  unsigned int x = 0x80000000;
  test(x, x);
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
// DEFAULT-NEXT:     fn %0 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @test(%3 a: u32, %4 b: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if lt<u32>(read<u32>(%3), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if lt<u32>(read<u32>(%4), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<u32>(add<u32, overflow=wrap>(read<u32>(%3), read<u32>(%4)), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main(%6 argc: i32, %7 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 x: u32 [storage=automatic] = const<u32>(2147483648);
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%2, read<u32>(%8), read<u32>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
