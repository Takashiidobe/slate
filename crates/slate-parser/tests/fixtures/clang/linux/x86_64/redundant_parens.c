#include <stdio.h>

int classify(int a, int b) {
  int r = 0;
  while (a < b) {
    a = a + 1;
    r = r + 1;
  }
  if (a == b) {
    r = r + 10;
  }
  int t = (a > b) ? (a - b) : (b - a);
  int m = (a & b) + (a << 1);
  return r + t + m;
}

int main() {
  printf("%d\n", classify(2, 5));
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
// DEFAULT-NEXT:     global %10 .str10: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%8 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @classify(%2 a: i32, %3 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %9 lt<i32>(read<i32>(%2), read<i32>(%3))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%2, add<i32, overflow=ub>(read<i32>(%2), const<i32>(1)));
// DEFAULT-NEXT:                 write<i32>(%4, add<i32, overflow=ub>(read<i32>(%4), const<i32>(1)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%2), read<i32>(%3))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%4, add<i32, overflow=ub>(read<i32>(%4), const<i32>(10)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %5 t: i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%2), read<i32>(%3)), sub<i32, overflow=ub>(read<i32>(%2), read<i32>(%3)), sub<i32, overflow=ub>(read<i32>(%3), read<i32>(%2)));
// DEFAULT-NEXT:         let %6 m: i32 [storage=automatic] = add<i32, overflow=ub>(and<i32>(read<i32>(%2), read<i32>(%3)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%2), const<i32>(1)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%4), read<i32>(%5)), read<i32>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%10)), call<i32, signature=fn(i32, i32) -> i32>(%1, const<i32>(2), const<i32>(5)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
