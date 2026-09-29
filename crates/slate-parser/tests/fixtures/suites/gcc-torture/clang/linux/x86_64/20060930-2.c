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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 s: ptr<@type[[TYPE_S]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = struct {
// DEFAULT-NEXT:         field0 t: ptr<@type[[TYPE_T]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_t:[0-9]+]] t: @type[[TYPE_T]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_s_2:[0-9]+]] s: ptr<void>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_T]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_T]]>, reason=assign>(read<ptr<void>>(%[[VALUE_s_2]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<@type[[TYPE_T]]>>>(field0(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p]]))))), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<ptr<@type[[TYPE_T]]>>>(field0(%[[VALUE_t]]))), const<u64>(8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_p_2:[0-9]+]] p: ptr<void>, %[[VALUE_q:[0-9]+]] q: ptr<@type[[TYPE_S]]>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_q]]))), addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_foo]], read<ptr<void>>(%[[VALUE_p_2]]));
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(read<ptr<@type[[TYPE_S]]>>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_q]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_T]]>>(field0(%[[VALUE_t]]), addr_of<ptr<@type[[TYPE_T]]>>(%[[VALUE_t]]));
// DEFAULT-NEXT:         if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, ptr<@type[[TYPE_S]]>) -> ptr<void>>(%[[VALUE_bar]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]])), addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]])), pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<@type[[TYPE_T]]>>(%[[VALUE_t]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
