#include <stdio.h>

int main(void) {
  struct {
    int         code;
    const char *message;
    double      confidence;
  } error_log[] = {
      {404, "Not Found", 0.99},
      {500, "Internal Server Error", 0.85},
      {200, "OK", 1.00},
  };

  printf("%d\n", error_log[0].code);
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 code: i32;
// DEFAULT-NEXT:         field1 message: ptr<const i8>;
// DEFAULT-NEXT:         field2 confidence: f64;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     global %6 .str6: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([78, 111, 116, 32, 70, 111, 117, 110, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([73, 110, 116, 101, 114, 110, 97, 108, 32, 83, 101, 114, 118, 101, 114, 32, 69, 114, 114, 111, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([79, 75, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%5 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 error_log: array<@type0, 3> [storage=automatic] [align=16] = aggregate<array<@type0, 3>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(404), field1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%6)), field2 = const<f64>(0.99)), index1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(500), field1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(22)>(%7)), field2 = const<f64>(0.85)), index2 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(200), field1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%8)), field2 = const<f64>(1.0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%9)), read<i32>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(%4), const<i32>(0))))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
