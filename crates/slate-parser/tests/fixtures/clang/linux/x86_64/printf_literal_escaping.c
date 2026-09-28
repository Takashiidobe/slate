#include <stdio.h>

int main(void) {
  int      d = 5;
  unsigned h = 0xAB;
  printf("{%d} %% \"quoted\" back\\slash %s|%c|%x\n", d, "hi", 'X', h);
  printf("}}%%{{%d}}\n", d);
  printf("%%%%%d%%%%\n", d);
  printf("{{}}%s{{}}\n", "mid");
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
// DEFAULT-NEXT:     global %6 .str6: array<i8, 38> [storage=static] = code_units<array<i8, 38>>([123, 37, 100, 125, 32, 37, 37, 32, 34, 113, 117, 111, 116, 101, 100, 34, 32, 98, 97, 99, 107, 92, 115, 108, 97, 115, 104, 32, 37, 115, 124, 37, 99, 124, 37, 120, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([104, 105, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([125, 125, 37, 37, 123, 123, 37, 100, 125, 125, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([37, 37, 37, 37, 37, 100, 37, 37, 37, 37, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([123, 123, 125, 125, 37, 115, 123, 123, 125, 125, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([109, 105, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%5 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 d: i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         let %4 h: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(171));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(38)>(%6)), read<i32>(%3), array_decay<ptr<i8>, length=Some(3)>(%7), const<i32>(88), read<u32>(%4));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%8)), read<i32>(%3));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%9)), read<i32>(%3));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%10)), array_decay<ptr<i8>, length=Some(4)>(%11));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
