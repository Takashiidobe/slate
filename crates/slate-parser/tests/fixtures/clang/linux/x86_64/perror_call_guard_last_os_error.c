#include <stdio.h>

int main(void) {
  int rc = remove("slate_perror_call_guard_missing.tmp");
  if (rc < 0) {
    perror("remove failed");
    return 1;
  }
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
// DEFAULT-NEXT:     global %6 .str6: array<i8, 36> [storage=static] = code_units<array<i8, 36>>([115, 108, 97, 116, 101, 95, 112, 101, 114, 114, 111, 114, 95, 99, 97, 108, 108, 95, 103, 117, 97, 114, 100, 95, 109, 105, 115, 115, 105, 110, 103, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([114, 101, 109, 111, 118, 101, 32, 102, 97, 105, 108, 101, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @remove(%4 __filename: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @perror(%5 __s: ptr<const i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 rc: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i8>) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(36)>(%6)));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%7)));
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
