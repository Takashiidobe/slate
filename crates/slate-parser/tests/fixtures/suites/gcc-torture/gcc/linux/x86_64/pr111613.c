#include <stdio.h>
#include <stdlib.h>

struct bitfield {
  unsigned int field1 : 1;
  unsigned int field2 : 1;
  unsigned int field3 : 1;
};

__attribute__((noinline)) static void
set_field1_and_field2(struct bitfield *b) {
  b->field1 = 1;
  b->field2 = 1;
}

__attribute__((noinline)) static struct bitfield *new_bitfield(void) {
  struct bitfield *b = (struct bitfield *)malloc(sizeof(*b));
  b->field3          = 1;
  set_field1_and_field2(b);
  return b;
}

int main(void) {
  struct bitfield *b = new_bitfield();
  if (b->field3 != 1)
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_bitfield:[0-9]+]] bitfield = struct {
// DEFAULT-NEXT:         field0 field1: u32 : 1;
// DEFAULT-NEXT:         field1 field2: u32 : 1;
// DEFAULT-NEXT:         field2 field3: u32 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0], bit_offsets=[Some(0), Some(1), Some(2)], bit_units=[(0, 1)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_set_field1_and_field2:[0-9]+]] @set_field1_and_field2(%[[VALUE_b:[0-9]+]] b: ptr<@type[[TYPE_bitfield]]>) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(deref(read<ptr<@type[[TYPE_bitfield]]>>(%[[VALUE_b]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..1, bits=1..2>(deref(read<ptr<@type[[TYPE_bitfield]]>>(%[[VALUE_b]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_new_bitfield:[0-9]+]] @new_bitfield() -> ptr<@type[[TYPE_bitfield]]> [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: ptr<@type[[TYPE_bitfield]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_bitfield]]>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], const<u64>(4)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..1, bits=2..3>(deref(read<ptr<@type[[TYPE_bitfield]]>>(%[[VALUE_b_2]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_bitfield]]>) -> void>(%[[VALUE_set_field1_and_field2]], read<ptr<@type[[TYPE_bitfield]]>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_bitfield]]>>(%[[VALUE_b_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_b_3:[0-9]+]] b: ptr<@type[[TYPE_bitfield]]> [storage=automatic] = call<ptr<@type[[TYPE_bitfield]]>, signature=fn() -> ptr<@type[[TYPE_bitfield]]>>(%[[VALUE_new_bitfield]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..1, bits=2..3>(deref(read<ptr<@type[[TYPE_bitfield]]>>(%[[VALUE_b_3]]))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
