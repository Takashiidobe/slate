/* PR middle-end/29272 */

extern void abort(void);

struct S {
  struct S *s;
} s;
struct T {
  struct T *t;
} t;

static inline void foo(void *s) {
  struct T *p = s;
  __builtin_memcpy(&p->t, &t.t, sizeof(t.t));
}

void *__attribute__((noinline)) bar(void *p, struct S *q) {
  q->s = &s;
  foo(p);
  return q->s;
}

int main(void) {
  t.t = &t;
  if (bar(&s, &s) != (void *)&t)
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
// DEFAULT-NEXT:         field0 s: ptr<@type0>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 T = struct {
// DEFAULT-NEXT:         field0 t: ptr<@type1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %2 s: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 t: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %15 @__builtin_memcpy(%12 <unnamed>: ptr<void>, %13 <unnamed>: ptr<const void>, %14 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %5 @foo(%6 s: ptr<void>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 p: ptr<@type1> [storage=automatic] = pointer_cast<ptr<@type1>, reason=assign>(read<ptr<void>>(%6));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%15, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<@type1>>>(field0(deref(read<ptr<@type1>>(%7))))), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<ptr<@type1>>>(field0(%4))), const<u64>(8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @bar(%9 p: ptr<void>, %10 q: ptr<@type0>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<@type0>>(field0(deref(read<ptr<@type0>>(%10))), addr_of<ptr<@type0>>(%2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%5, read<ptr<void>>(%9));
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(read<ptr<@type0>>(field0(deref(read<ptr<@type0>>(%10)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<@type1>>(field0(%4), addr_of<ptr<@type1>>(%4));
// DEFAULT-NEXT:         if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, ptr<@type0>) -> ptr<void>>(%8, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type0>>(%2)), addr_of<ptr<@type0>>(%2)), pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<@type1>>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
