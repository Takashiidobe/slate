#include <stdio.h>

struct Conditional {
  enum { IN_THEN, IN_ELIF = 4, IN_ELSE } ctx;
};

int main(void) {
  struct Conditional conditional = {IN_THEN};
  conditional.ctx                = IN_ELSE;
  printf("%d %d\n", (int)conditional.ctx, IN_ELIF);
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
// DEFAULT-NEXT:     type @type0 Conditional = struct {
// DEFAULT-NEXT:         field0 ctx: @type1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 = enum : u32 {
// DEFAULT-NEXT:         %0 IN_THEN = const<i32>(0);
// DEFAULT-NEXT:         %1 IN_ELIF = const<i32>(4);
// DEFAULT-NEXT:         %2 IN_ELSE = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%8 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 conditional: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = int_to_enum<@type1, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<@type1>(field0(%7), int_to_enum<@type1, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%9)), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type1>(field0(%7)))), const<i32>(4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
