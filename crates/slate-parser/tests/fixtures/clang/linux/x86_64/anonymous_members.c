#include <stdio.h>

struct container {
  int prefix;
  union {
    int   integer;
    float real;
  };
  struct {
    int x;
    int y;
  };
};

int main(void) {
  struct container value = {0};
  value.prefix           = 3;
  value.integer          = 31;
  value.x                = 37;
  value.y                = 41;
  printf("%d %d %d %d %zu\n", value.prefix, value.integer, value.x, value.y,
         sizeof(value));
  value.real = 2.5f;
  printf("%d\n", (int)value.real);
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
// DEFAULT-NEXT:     type @type0 container = struct {
// DEFAULT-NEXT:         field0 prefix: i32;
// DEFAULT-NEXT:         field1 <anonymous>: @type1;
// DEFAULT-NEXT:         field2 <anonymous>: @type2;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type1 = union {
// DEFAULT-NEXT:         field0 integer: i32;
// DEFAULT-NEXT:         field1 real: f32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%6 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 value: @type0 [storage=automatic] = aggregate<@type0, zero_fill=true>(field0 = const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field0(%5), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(field0(field1(%5)), const<i32>(31));
// DEFAULT-NEXT:         write<i32>(field0(field2(%5)), const<i32>(37));
// DEFAULT-NEXT:         write<i32>(field1(field2(%5)), const<i32>(41));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%7)), read<i32>(field0(%5)), read<i32>(field0(field1(%5))), read<i32>(field0(field2(%5))), read<i32>(field1(field2(%5))), const<u64>(16));
// DEFAULT-NEXT:         write<f32>(field1(field1(%5)), const<f32>(2.5));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%8)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(field1(field1(%5)))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
