extern __attribute__((__noreturn__)) void exit(int);
extern __attribute__((__noreturn__)) void abort(void);
__attribute__((__noinline__)) int         add512(int a, int *b) {
  int c = a + 512;
  if (c != 0)
    *b = a;
  return c;
}

__attribute__((__noinline__)) int add513(int a, int *b) {
  int c = a + 513;
  if (c == 0)
    *b = a;
  return c;
}

int main(void) {
  int b0 = -1;
  int b1 = -1;
  if (add512(-512, &b0) != 0 || b0 != -1 || add513(-513, &b1) != 0 ||
      b1 != -513)
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
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_add512:[0-9]+]] @add512(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: ptr<i32>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), const<i32>(512));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(deref(read<ptr<i32>>(%[[VALUE_b]])), read<i32>(%[[VALUE_a]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_add513:[0-9]+]] @add513(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: ptr<i32>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_a_2]]), const<i32>(513));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_c_2]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(deref(read<ptr<i32>>(%[[VALUE_b_2]])), read<i32>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_c_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_b0:[0-9]+]] b0: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_b1:[0-9]+]] b1: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(call<i32, signature=fn(i32, ptr<i32>) -> i32>(%[[VALUE_add512]], neg<i32, overflow=ub>(const<i32>(512)), addr_of<ptr<i32>>(%[[VALUE_b0]])), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_b0]]), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<i32>(call<i32, signature=fn(i32, ptr<i32>) -> i32>(%[[VALUE_add513]], neg<i32, overflow=ub>(const<i32>(513)), addr_of<ptr<i32>>(%[[VALUE_b1]])), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%[[VALUE1]]), ne<i32>(read<i32>(%[[VALUE_b1]]), neg<i32, overflow=ub>(const<i32>(513))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
