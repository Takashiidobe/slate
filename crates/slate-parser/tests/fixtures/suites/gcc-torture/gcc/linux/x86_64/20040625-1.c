/* From PR target/16176 */
void abort(void);
void exit(int);

struct __attribute__((packed)) s {
  struct s *next;
};

struct s *__attribute__((noinline)) maybe_next(struct s *s, int t) {
  if (t)
    s = s->next;
  return s;
}

int main() {
  struct s s1, s2;

  s1.next = &s2;
  if (maybe_next(&s1, 1) != &s2)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 next: ptr<@type[[TYPE_s]]>;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_maybe_next:[0-9]+]] @maybe_next(%[[VALUE_s:[0-9]+]] s: ptr<@type[[TYPE_s]]>, %[[VALUE_t:[0-9]+]] t: i32) -> ptr<@type[[TYPE_s]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_s]]>>(%[[VALUE_s]], read<ptr<@type[[TYPE_s]]>>(field0(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_s]])))));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_s]]>>(%[[VALUE_s]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s1:[0-9]+]] s1: @type[[TYPE_s]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_s2:[0-9]+]] s2: @type[[TYPE_s]] [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_s]]>>(field0(%[[VALUE_s1]]), addr_of<ptr<@type[[TYPE_s]]>>(%[[VALUE_s2]]));
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_s]]>>(call<ptr<@type[[TYPE_s]]>, signature=fn(ptr<@type[[TYPE_s]]>, i32) -> ptr<@type[[TYPE_s]]>>(%[[VALUE_maybe_next]], addr_of<ptr<@type[[TYPE_s]]>>(%[[VALUE_s1]]), const<i32>(1)), addr_of<ptr<@type[[TYPE_s]]>>(%[[VALUE_s2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
