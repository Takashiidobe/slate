/* PR tree-optimization/102622 */
/* Wrong code introduced due to phi-opt
   introducing undefined signed interger overflow
   with one bit signed integer negation. */

struct f {
  signed t : 1;
};
int g(struct f *a, int t) __attribute__((noipa));
int g(struct f *a, int t) {
  if (t)
    a->t = -1;
  else
    a->t = 0;
  int t1 = a->t;
  if (t1)
    return 1;
  return t1;
}

int main(void) {
  struct f a;
  if (!g(&a, 1))
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
// DEFAULT-NEXT:     type @type[[TYPE_f:[0-9]+]] f = struct {
// DEFAULT-NEXT:         field0 t: i32 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_f]]>, %[[VALUE_t:[0-9]+]] t: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(deref(read<ptr<@type[[TYPE_f]]>>(%[[VALUE_a]]))), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(deref(read<ptr<@type[[TYPE_f]]>>(%[[VALUE_a]]))), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_t1:[0-9]+]] t1: i32 [storage=automatic] = read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(deref(read<ptr<@type[[TYPE_f]]>>(%[[VALUE_a]]))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_t1]]), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_t1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: @type[[TYPE_f]] [storage=automatic];
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_f]]>, i32) -> i32>(%[[VALUE_g]], addr_of<ptr<@type[[TYPE_f]]>>(%[[VALUE_a_2]]), const<i32>(1)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
