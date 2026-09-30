/* PR tree-optimization/36008 */

extern void abort(void);

int g[48][3][3];

void __attribute__((noinline)) bar(int x[3][3], int y[3][3]) {
  static int i;
  if (x != g[i + 8] || y != g[i++])
    abort();
}

static inline void __attribute__((always_inline)) foo(int x[][3][3]) {
  int i;
  for (i = 0; i < 8; i++) {
    int k = i + 8;
    bar(x[k], x[k - 8]);
  }
}

int main() {
  foo(g);
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
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: array<array<array<i32, 3>, 3>, 48> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: ptr<array<i32, 3>> [array=3], %[[VALUE_y:[0-9]+]] y: ptr<array<i32, 3>> [array=3]) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>>(%[[VALUE_x]]), array_decay<ptr<array<i32, 3>>, length=Some(3)>(deref(ptr_offset<ptr<array<array<i32, 3>, 3>>, subtract=false, element=array<array<i32, 3>, 3>, overflow=ub>(array_decay<ptr<array<array<i32, 3>, 3>>, length=Some(48)>(%[[VALUE_g]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(8))))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>>(%[[VALUE_y]]), array_decay<ptr<array<i32, 3>>, length=Some(3)>(deref(ptr_offset<ptr<array<array<i32, 3>, 3>>, subtract=false, element=array<array<i32, 3>, 3>, overflow=ub>(array_decay<ptr<array<array<i32, 3>, 3>>, length=Some(48)>(%[[VALUE_g]]), read<i32>(%[[VALUE1]]))))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x_2:[0-9]+]] x: ptr<array<array<i32, 3>, 3>>) -> void [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i_2]]), const<i32>(8));
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<array<i32, 3>>, ptr<array<i32, 3>>) -> void>(%[[VALUE_bar]], array_decay<ptr<array<i32, 3>>, length=Some(3)>(deref(ptr_offset<ptr<array<array<i32, 3>, 3>>, subtract=false, element=array<array<i32, 3>, 3>, overflow=ub>(read<ptr<array<array<i32, 3>, 3>>>(%[[VALUE_x_2]]), read<i32>(%[[VALUE_k]])))), array_decay<ptr<array<i32, 3>>, length=Some(3)>(deref(ptr_offset<ptr<array<array<i32, 3>, 3>>, subtract=false, element=array<array<i32, 3>, 3>, overflow=ub>(read<ptr<array<array<i32, 3>, 3>>>(%[[VALUE_x_2]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_k]]), const<i32>(8))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<array<array<i32, 3>, 3>>) -> void>(%[[VALUE_foo]], array_decay<ptr<array<array<i32, 3>, 3>>, length=Some(48)>(%[[VALUE_g]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
