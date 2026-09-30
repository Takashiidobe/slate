/* PR rtl-optimization/68249 */

int  a, b, c, g, k, l, m, n;
char h;

void fn1() {
  for (; k; k++) {
    m = b || c < 0 || c > 1 ?: c;
    g = l = n || m < 0 || (m > 1) > 1 >> m ?: 1 << m;
  }
  l = b + 1;
  for (; b < 1; b++)
    h = a + 1;
}

int main() {
  char j;
  for (; a < 1; a++) {
    fn1();
    if (h)
      j = h;
    if (j > c)
      g = 0;
  }

  if (h != 1)
    __builtin_abort();

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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_m:[0-9]+]] m: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n:[0-9]+]] n: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_k]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: bool [synthetic] = logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0)), lt<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0))), gt<i32>(read<i32>(%[[VALUE_c]]), const<i32>(1)));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_m]], conditional<i32>(ne<i32>(read<bool>(%[[VALUE3]]), const<i32>(0)), read<bool>(%[[VALUE3]]), read<i32>(%[[VALUE_c]])));
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: bool [synthetic] = logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0)), lt<i32>(read<i32>(%[[VALUE_m]]), const<i32>(0))), gt<i32>(from_bool<i32, reason=promotion>(gt<i32>(read<i32>(%[[VALUE_m]]), const<i32>(1))), shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(1), read<i32>(%[[VALUE_m]]))));
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = conditional<i32>(ne<i32>(read<bool>(%[[VALUE4]]), const<i32>(0)), read<bool>(%[[VALUE4]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<i32>(%[[VALUE_m]])));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_l]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_g]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<i32>(%[[VALUE_l]], add<i32, overflow=ub>(read<i32>(%[[VALUE_b]]), const<i32>(1)));
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_b]]), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(%[[VALUE_h]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i8 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_a]]), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_fn1]]);
// DEFAULT-NEXT:                     if ne<i8>(read<i8>(%[[VALUE_h]]), const<i8>(0))
// DEFAULT-NEXT:                         write<i8>(%[[VALUE_j]], read<i8>(%[[VALUE_h]]));
// DEFAULT-NEXT:                     if gt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_j]])), read<i32>(%[[VALUE_c]]))
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_g]], const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_h]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
