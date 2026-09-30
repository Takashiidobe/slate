/* { dg-require-stack-size "0x12000" } */

void abort(void);
void exit(int);

#if __INT_MAX__ < 32768
int main() { exit(0); }
#else
int a[2] = {2, 3};

static int __attribute__((noinline)) bar(int x, void *b) {
  a[0]++;
  return x;
}

static int __attribute__((noinline)) foo(int x) {
  char buf[0x10000];
  int  y = a[0];
  a[1]   = y;
  x      = bar(x, buf);
  y      = bar(y, buf);
  return x + y;
}

int main() {
  if (foo(100) != 102)
    abort();
  exit(0);
}
#endif



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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(3)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_b:[0-9]+]] b: ptr<void>) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE1]])));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE1]])), read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: array<i8, 65536> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_a]]), const<i32>(1))), read<i32>(%[[VALUE_y]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x_2]], call<i32, signature=fn(i32, ptr<void>) -> i32>(%[[VALUE_bar]], read<i32>(%[[VALUE_x_2]]), pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(65536)>(%[[VALUE_buf]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_y]], call<i32, signature=fn(i32, ptr<void>) -> i32>(%[[VALUE_bar]], read<i32>(%[[VALUE_y]]), pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(65536)>(%[[VALUE_buf]]))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_x_2]]), read<i32>(%[[VALUE_y]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(100)), const<i32>(102))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
