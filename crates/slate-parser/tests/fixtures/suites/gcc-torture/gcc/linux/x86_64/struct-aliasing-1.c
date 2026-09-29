struct S {
  float f;
};
int __attribute__((noinline)) foo(int *r, struct S *p) {
  int *q = (int *)&p->f;
  int  i = *q;
  *r     = 0;
  return i + *q;
}
extern void abort(void);
int         main() {
  int i = 1;
  if (foo(&i, (struct S *)&i) != 1)
    abort();
  return (0);
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
// DEFAULT-NEXT:         field0 f: f32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_r:[0-9]+]] r: ptr<i32>, %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<f32>>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]])))));
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE_q]])));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_r]])), const<i32>(0));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), read<i32>(deref(read<ptr<i32>>(%[[VALUE_q]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<i32>, ptr<@type[[TYPE_S]]>) -> i32>(%[[VALUE_foo]], addr_of<ptr<i32>>(%[[VALUE_i_2]]), pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_i_2]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
