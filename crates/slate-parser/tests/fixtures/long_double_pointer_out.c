#include <stdio.h>

int main(void) {
  long double parsed;
  int         matched = sscanf("0x1.0000000000000002p+0", "%La", &parsed);

  printf("%d %La\n", matched, parsed);
  return matched != 1 || parsed != 0x1.0000000000000002p0L;
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
// DEFAULT-NEXT:     global %10 .str10: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([48, 120, 49, 46, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 50, 112, 43, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 76, 97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %12 .str12: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([37, 100, 32, 37, 76, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%5 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @sscanf(%6 __s: ptr<const i8> [restrict], %7 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external] [asm_name="__isoc23_sscanf"];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 parsed: f80 [storage=automatic];
// DEFAULT-NEXT:         let %4 matched: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i8>, ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%10)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%11)), addr_of<ptr<f80>>(%3));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%12)), read<i32>(%4), read<f80>(%3));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(ne<i32>(read<i32>(%4), const<i32>(1)), ne<f80, exceptions=ignore>(read<f80>(%3), const<f80>(1.00000000000000000011))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
