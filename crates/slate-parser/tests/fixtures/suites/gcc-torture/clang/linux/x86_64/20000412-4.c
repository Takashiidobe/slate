void abort(void);
void exit(int);

void f(int i, int j, int radius, int width, int N) {
  const int diff = i - radius;
  const int lowk = (diff > 0 ? diff : 0);
  int       k;

  for (k = lowk; k <= 2; k++) {
    int idx = ((k - i + radius) * width - j + radius);
    if (idx < 0)
      abort();
  }

  for (k = lowk; k <= 2; k++)
    ;
}

int main(int argc, char **argv) {
  int exc_rad = 2;
  int N       = 8;
  int i;
  for (i = 1; i < 4; i++)
    f(i, 1, exc_rad, 2 * exc_rad + 1, N);
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_j:[0-9]+]] j: i32, %[[VALUE_radius:[0-9]+]] radius: i32, %[[VALUE_width:[0-9]+]] width: i32, %[[VALUE_N:[0-9]+]] N: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_diff:[0-9]+]] diff: i32 [storage=automatic] [const] = sub<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_radius]]));
// DEFAULT-NEXT:         let %[[VALUE_lowk:[0-9]+]] lowk: i32 [storage=automatic] [const] = conditional<i32>(gt<i32>(read<i32>(%[[VALUE_diff]]), const<i32>(0)), read<i32>(%[[VALUE_diff]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE_lowk]]));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_k]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_idx:[0-9]+]] idx: i32 [storage=automatic] = add<i32, overflow=ub>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_k]]), read<i32>(%[[VALUE_i]])), read<i32>(%[[VALUE_radius]])), read<i32>(%[[VALUE_width]])), read<i32>(%[[VALUE_j]])), read<i32>(%[[VALUE_radius]]));
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(%[[VALUE_idx]]), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE_lowk]]));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_k]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_exc_rad:[0-9]+]] exc_rad: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %[[VALUE_N_2:[0-9]+]] N: i32 [storage=automatic] = const<i32>(8);
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(1));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32, i32, i32, i32) -> void>(%[[VALUE_f]], read<i32>(%[[VALUE_i_2]]), const<i32>(1), read<i32>(%[[VALUE_exc_rad]]), add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(2), read<i32>(%[[VALUE_exc_rad]])), const<i32>(1)), read<i32>(%[[VALUE_N_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
