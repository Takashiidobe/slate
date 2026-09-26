/* PR tree-optimization/33136 */

extern void abort(void);

struct S {
  struct S *a;
  int       b;
};

int main(void) {
  struct S *s = (struct S *)0, **p, *n;
  for (p = &s; *p; p = &(*p)->a)
    ;
  n    = (struct S *)__builtin_alloca(sizeof(*n));
  n->a = *p;
  n->b = 1;
  *p   = n;

  if (!s)
    abort();
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: ptr<@type0>;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 s: ptr<@type0> [storage=automatic] = null<ptr<@type0>>;
// DEFAULT-NEXT:         let %4 p: ptr<ptr<@type0>> [storage=automatic];
// DEFAULT-NEXT:         let %5 n: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<ptr<ptr<@type0>>>(%4, addr_of<ptr<ptr<@type0>>>(%3));
// DEFAULT-NEXT:             condition: ne<ptr<@type0>>(read<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%4))), null<ptr<@type0>>)
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<ptr<ptr<@type0>>>(%4, addr_of<ptr<ptr<@type0>>>(field0(deref(read<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%4)))))));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         write<ptr<@type0>>(%5, pointer_cast<ptr<@type0>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(__builtin_alloca, const<u64>(16))));
// DEFAULT-NEXT:         pointer_cast<ptr<@type0>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(__builtin_alloca, const<u64>(16)));
// DEFAULT-NEXT:         write<ptr<@type0>>(field0(deref(read<ptr<@type0>>(%5))), read<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%4))));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type0>>(%5))), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%4)), read<ptr<@type0>>(%5));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type0>>(read<ptr<@type0>>(%3), null<ptr<@type0>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
