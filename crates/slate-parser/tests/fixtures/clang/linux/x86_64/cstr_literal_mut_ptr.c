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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([109, 117, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([111, 116, 104, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([99, 111, 110, 115, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([111, 116, 104, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([98, 121, 116, 101, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 115, 32, 37, 115, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mutable_pick:[0-9]+]] @mutable_pick(%[[VALUE_i:[0-9]+]] i: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_const_pick:[0-9]+]] @const_pick(%[[VALUE_i_2:[0-9]+]] i: i32) -> ptr<const i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_3]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bytes_pick:[0-9]+]] @bytes_pick() -> ptr<u8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<u8>, reason=explicit>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_6]])), call<ptr<i8>, signature=fn(i32) -> ptr<i8>>(%[[VALUE_mutable_pick]], const<i32>(0)), call<ptr<const i8>, signature=fn(i32) -> ptr<const i8>>(%[[VALUE_const_pick]], const<i32>(0)), pointer_cast<ptr<i8>, reason=explicit>(call<ptr<u8>, signature=fn() -> ptr<u8>>(%[[VALUE_bytes_pick]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
