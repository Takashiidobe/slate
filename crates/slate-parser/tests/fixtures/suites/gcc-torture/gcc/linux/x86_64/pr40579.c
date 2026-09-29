extern void                            abort(void);
static char *__attribute__((noinline)) itos(int num) { return (char *)0; }
static void __attribute__((noinline))  foo(int i, const char *x) {
  if (i >= 4)
    abort();
}
int main() {
  int x = -__INT_MAX__ + 3;
  int i;

  for (i = 0; i < 4; ++i) {
    char *p;
    --x;
    p = itos(x);
    foo(i, p);
  }

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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_itos:[0-9]+]] @itos(%[[VALUE_num:[0-9]+]] num: i32) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<i8>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_x:[0-9]+]] x: ptr<const i8>) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: i32 [storage=automatic] = add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], call<ptr<i8>, signature=fn(i32) -> ptr<i8>>(%[[VALUE_itos]], read<i32>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:                     call<ptr<i8>, signature=fn(i32) -> ptr<i8>>(%[[VALUE_itos]], read<i32>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:                     call<void, signature=fn(i32, ptr<const i8>) -> void>(%[[VALUE_foo]], read<i32>(%[[VALUE_i_2]]), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_p]])));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
