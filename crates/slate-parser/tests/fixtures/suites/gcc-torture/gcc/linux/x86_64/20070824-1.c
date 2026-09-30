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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: ptr<@type[[TYPE_S]]>;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_alloca:[0-9]+]] @__builtin_alloca(%[[VALUE0:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: ptr<@type[[TYPE_S]]> [storage=automatic] = null<ptr<@type[[TYPE_S]]>>;
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<ptr<@type[[TYPE_S]]>> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: ptr<@type[[TYPE_S]]> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE_S]]>>>(%[[VALUE_p]], addr_of<ptr<ptr<@type[[TYPE_S]]>>>(%[[VALUE_s]]));
// DEFAULT-NEXT:             condition: ne<ptr<@type[[TYPE_S]]>>(read<ptr<@type[[TYPE_S]]>>(deref(read<ptr<ptr<@type[[TYPE_S]]>>>(%[[VALUE_p]]))), null<ptr<@type[[TYPE_S]]>>)
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE_S]]>>>(%[[VALUE_p]], addr_of<ptr<ptr<@type[[TYPE_S]]>>>(field0(deref(read<ptr<@type[[TYPE_S]]>>(deref(read<ptr<ptr<@type[[TYPE_S]]>>>(%[[VALUE_p]])))))));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_n]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], const<u64>(16))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_n]]))), read<ptr<@type[[TYPE_S]]>>(deref(read<ptr<ptr<@type[[TYPE_S]]>>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_n]]))), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(deref(read<ptr<ptr<@type[[TYPE_S]]>>>(%[[VALUE_p]])), read<ptr<@type[[TYPE_S]]>>(%[[VALUE_n]]));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type[[TYPE_S]]>>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]), null<ptr<@type[[TYPE_S]]>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
