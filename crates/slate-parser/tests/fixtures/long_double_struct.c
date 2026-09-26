#include <stdio.h>

struct box {
  int         tag;
  long double value;
};

static int sum_box(struct box b) { return (int)(b.value + (long double)b.tag); }

int main(void) {
  struct box b;
  b.tag   = 3;
  b.value = 4.5L;
  printf("%d\n", sum_box(b));
  b.value = b.value * 2.0L;
  printf("%d\n", (int)b.value);
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
// DEFAULT-NEXT:     type @type0 box = struct {
// DEFAULT-NEXT:         field0 tag: i32;
// DEFAULT-NEXT:         field1 value: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%6 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @sum_box(%3 b: @type0) -> i32 [linkage=internal] [abi=sysv64(byval<align=16>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(field1(%3)), int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(field0(%3)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 b: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%5), const<i32>(3));
// DEFAULT-NEXT:         write<f80>(field1(%5), const<f80>(4.5));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%7)), call<i32, signature=fn(@type0) -> i32, abi=sysv64(byval<align=16>) -> scalar>(%2, copy<@type0, reason=arg>(read<@type0>(%5))));
// DEFAULT-NEXT:         write<f80>(field1(%5), mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(field1(%5)), const<f80>(2)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%8)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f80>(field1(%5))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
