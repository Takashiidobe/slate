/* { dg-do run } */

#include <stdlib.h>

struct P {
  long      v;
  struct P *n;
};

struct F {
  long     x;
  struct P fam[];
};

int __attribute__((noipa)) f(struct F *f, int i) {
  struct P *p = f->fam;
  asm("" : "+r"(f) : "r"(p));
  p->v = 0;
  p->n = 0;
  return f->fam->n != 0;
}

int
main() {
  struct F *m = malloc(sizeof(long) + 2 * sizeof(struct P));
  m->fam[0].n = &m->fam[1];
  if (f(m, 0))
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 P = struct {
// DEFAULT-NEXT:         field0 v: i64;
// DEFAULT-NEXT:         field1 n: ptr<@type1>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type2 F = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:         field1 fam: array<@type1, incomplete>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %1 @malloc(%11 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @f(%6 f: ptr<@type2>, %7 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 p: ptr<@type1> [storage=automatic] = array_decay<ptr<@type1>, length=None>(field1(deref(read<ptr<@type2>>(%6))));
// DEFAULT-NEXT:         asm "" [dialect=att] {
// DEFAULT-NEXT:             out 0 "+r" place<ptr<@type2>>(%6);
// DEFAULT-NEXT:             in 1 "r" read<ptr<@type1>>(%8);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i64>(field0(deref(read<ptr<@type1>>(%8))), widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<ptr<@type1>>(field1(deref(read<ptr<@type1>>(%8))), null<ptr<@type1>>);
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<ptr<@type1>>(read<ptr<@type1>>(field1(deref(array_decay<ptr<@type1>, length=None>(field1(deref(read<ptr<@type2>>(%6))))))), null<ptr<@type1>>));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 m: ptr<@type2> [storage=automatic] = pointer_cast<ptr<@type2>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%1, add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(16)))));
// DEFAULT-NEXT:         write<ptr<@type1>>(field1(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=None>(field1(deref(read<ptr<@type2>>(%10)))), const<i32>(0)))), addr_of<ptr<@type1>>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=None>(field1(deref(read<ptr<@type2>>(%10)))), const<i32>(1)))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type2>, i32) -> i32>(%5, read<ptr<@type2>>(%10), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
