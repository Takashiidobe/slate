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
// DEFAULT-NEXT:     type @type0 f = struct {
// DEFAULT-NEXT:         field0 t: i32 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT:     fn %1 @g(%2 a: ptr<@type0>, %3 t: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(deref(read<ptr<@type0>>(%2))), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(deref(read<ptr<@type0>>(%2))), const<i32>(0));
// DEFAULT-NEXT:         let %4 t1: i32 [storage=automatic] = read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(deref(read<ptr<@type0>>(%2))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         return read<i32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<@type0>, i32) -> i32>(%1, addr_of<ptr<@type0>>(%6), const<i32>(1)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
