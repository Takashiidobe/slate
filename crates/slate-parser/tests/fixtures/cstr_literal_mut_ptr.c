#include <stdio.h>

char *mutable_pick(int i) {
  if (i == 0) {
    return "mut";
  }
  return "other";
}

const char *const_pick(int i) {
  if (i == 0) {
    return "const";
  }
  return "other";
}

unsigned char *bytes_pick(void) { return (unsigned char *)"bytes"; }

int main(void) {
  printf("%s %s %s\n", mutable_pick(0), const_pick(0), (char *)bytes_pick());
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
// DEFAULT-NEXT:     global %8 .str8: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([109, 117, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([111, 116, 104, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 111, 110, 115, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([111, 116, 104, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %12 .str12: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([98, 121, 116, 101, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 115, 32, 37, 115, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%7 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @mutable_pick(%2 i: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return array_decay<ptr<i8>, length=Some(4)>(%8);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return array_decay<ptr<i8>, length=Some(6)>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @const_pick(%4 i: i32) -> ptr<const i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(6)>(%10));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(6)>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bytes_pick() -> ptr<u8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<u8>, reason=explicit>(array_decay<ptr<i8>, length=Some(6)>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%13)), call<ptr<i8>, signature=fn(i32) -> ptr<i8>>(%1, const<i32>(0)), call<ptr<const i8>, signature=fn(i32) -> ptr<const i8>>(%3, const<i32>(0)), pointer_cast<ptr<i8>, reason=explicit>(call<ptr<u8>, signature=fn() -> ptr<u8>>(%5)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
