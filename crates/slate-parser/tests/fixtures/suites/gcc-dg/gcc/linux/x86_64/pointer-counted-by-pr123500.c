/* PR c/123500 */
/* { dg-do compile } */
/* { dg-options "-Wbad-function-cast" } */

#include <stdint.h>
struct buffer {
  uint8_t * ptr __attribute__((counted_by(len)));
  int len;
};

uintptr_t foo(struct buffer * b) {
  return (uintptr_t)b->ptr;
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE___uint8_t:[0-9]+]] __uint8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE_uint8_t:[0-9]+]] uint8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE_uintptr_t:[0-9]+]] uintptr_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_buffer:[0-9]+]] buffer = struct {
// DEFAULT-NEXT:         field0 ptr: ptr<u8>;
// DEFAULT-NEXT:         field1 len: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_b:[0-9]+]] b: ptr<@type[[TYPE_buffer]]>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ptr_to_int<u64, reason=explicit>(read<ptr<u8>>(field0(deref(read<ptr<@type[[TYPE_buffer]]>>(%[[VALUE_b]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
