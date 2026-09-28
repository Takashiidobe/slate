#include <stdio.h>

struct Byte {
  signed char value;
};

struct Byte make_byte(void) {
  struct Byte result = {7};
  return result;
}

int initialize_chars(void) {
  char text[4] = "abc";
  return 5;
}

int main(void) {
  struct Byte byte = make_byte();
  printf("%d\n", byte.value + initialize_chars());
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
// DEFAULT-NEXT:     type @type0 Byte = struct {
// DEFAULT-NEXT:         field0 value: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%8 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @make_byte() -> @type0 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 result: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @initialize_chars() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 text: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([97, 98, 99, 0]);
// DEFAULT-NEXT:         return const<i32>(5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 byte: @type0 [storage=automatic] = copy<@type0, reason=assign>(call<@type0, signature=fn() -> @type0, abi=sysv64() -> native_c>(%2));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%9)), add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(field0(%7))), call<i32, signature=fn() -> i32>(%4)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
