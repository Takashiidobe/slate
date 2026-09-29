/* PR tree-optimization/59358 */

__attribute__((noinline, noclone)) int foo(int *x, int y) {
  int z = *x;
  if (y > z && y <= 16)
    while (y > z)
      z *= 2;
  return z;
}

int main() {
  int i;
  for (i = 1; i < 17; i++) {
    int j = foo(&i, 16);
    int k;
    if (i >= 8 && i <= 15)
      k = 16 + (i - 8) * 2;
    else if (i >= 4 && i <= 7)
      k = 16 + (i - 4) * 4;
    else if (i == 3)
      k = 24;
    else
      k = 16;
    if (j != k)
      __builtin_abort();
    j = foo(&i, 7);
    if (i >= 7)
      k = i;
    else if (i >= 4)
      k = 8 + (i - 4) * 2;
    else if (i == 3)
      k = 12;
    else
      k = 8;
    if (j != k)
      __builtin_abort();
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<i32>, %[[VALUE_y:[0-9]+]] y: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: i32 [storage=automatic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE_x]])));
// DEFAULT-NEXT:         if logical_and<bool>(gt<i32>(read<i32>(%[[VALUE_y]]), read<i32>(%[[VALUE_z]])), le<i32>(read<i32>(%[[VALUE_y]]), const<i32>(16)))
// DEFAULT-NEXT:             while %[[VALUE0:[0-9]+]] gt<i32>(read<i32>(%[[VALUE_y]]), read<i32>(%[[VALUE_z]]))
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_z]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(2));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_z]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_z]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(1));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(17))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>, i32) -> i32>(%[[VALUE_foo]], addr_of<ptr<i32>>(%[[VALUE_i]]), const<i32>(16));
// DEFAULT-NEXT:                     let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic];
// DEFAULT-NEXT:                     if logical_and<bool>(ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(8)), le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(15)))
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_k]], add<i32, overflow=ub>(const<i32>(16), mul<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(8)), const<i32>(2))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if logical_and<bool>(ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4)), le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(7)))
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_k]], add<i32, overflow=ub>(const<i32>(16), mul<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(4)), const<i32>(4))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(3))
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_k]], const<i32>(24));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_k]], const<i32>(16));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_j]]), read<i32>(%[[VALUE_k]]))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_j]], call<i32, signature=fn(ptr<i32>, i32) -> i32>(%[[VALUE_foo]], addr_of<ptr<i32>>(%[[VALUE_i]]), const<i32>(7)));
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<i32>, i32) -> i32>(%[[VALUE_foo]], addr_of<ptr<i32>>(%[[VALUE_i]]), const<i32>(7));
// DEFAULT-NEXT:                     if ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(7))
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4))
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_k]], add<i32, overflow=ub>(const<i32>(8), mul<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(4)), const<i32>(2))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(3))
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_k]], const<i32>(12));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_k]], const<i32>(8));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_j]]), read<i32>(%[[VALUE_k]]))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
