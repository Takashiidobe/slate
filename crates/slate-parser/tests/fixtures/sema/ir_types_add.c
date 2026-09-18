// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include
// SLATE-FILECHECK-ARGS --dump-ir-types --show-metadata

#include <stdio.h>

int add(int a, int b) {
  int c = a + b;
  return c;
}

int main(void) {
  printf("%d\n", add(2, 3));
}

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
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1 @printf(%0 <unnamed>: ptr<const i8> [restrict] [c="const char *restrict"] [c_restrict="true"], ...) -> i32 [linkage=external] [c="int"];
// DEFAULT-NEXT:     fn %4 @add(%2 a: i32 [c="int"], %3 b: i32 [c="int"]) -> i32 [linkage=external] [c="int"];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [c="int"];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
