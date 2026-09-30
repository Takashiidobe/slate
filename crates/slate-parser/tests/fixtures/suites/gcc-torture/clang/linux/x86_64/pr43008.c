int i;
struct X {
  int *p;
};
struct X *__attribute__((malloc)) my_alloc(void) {
  struct X *p = __builtin_malloc(sizeof(struct X));
  p->p        = &i;
  return p;
}
extern void abort(void);
int         main() {
  struct X *p, *q;
  p       = my_alloc();
  q       = my_alloc();
  *(p->p) = 1;
  *(q->p) = 0;
  if (*(p->p) != 0)
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
// DEFAULT-NEXT:     type @type[[TYPE_X:[0-9]+]] X = struct {
// DEFAULT-NEXT:         field0 p: ptr<i32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_malloc:[0-9]+]] @__builtin_malloc(%[[VALUE0:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_my_alloc:[0-9]+]] @my_alloc() -> ptr<@type[[TYPE_X]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_X]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_X]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], const<u64>(8)));
// DEFAULT-NEXT:         write<ptr<i32>>(field0(deref(read<ptr<@type[[TYPE_X]]>>(%[[VALUE_p]]))), addr_of<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_X]]>>(%[[VALUE_p]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_X]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<@type[[TYPE_X]]> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_X]]>>(%[[VALUE_p_2]], call<ptr<@type[[TYPE_X]]>, signature=fn() -> ptr<@type[[TYPE_X]]>>(%[[VALUE_my_alloc]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_X]]>>(%[[VALUE_q]], call<ptr<@type[[TYPE_X]]>, signature=fn() -> ptr<@type[[TYPE_X]]>>(%[[VALUE_my_alloc]]));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(field0(deref(read<ptr<@type[[TYPE_X]]>>(%[[VALUE_p_2]]))))), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(field0(deref(read<ptr<@type[[TYPE_X]]>>(%[[VALUE_q]]))))), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(read<ptr<i32>>(field0(deref(read<ptr<@type[[TYPE_X]]>>(%[[VALUE_p_2]])))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
