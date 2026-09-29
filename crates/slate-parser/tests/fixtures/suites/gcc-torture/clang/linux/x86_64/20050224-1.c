/* Origin: Mikael Pettersson <mikpe@csd.uu.se> and the Linux kernel.  */

extern void   abort(void);
unsigned long a = 0xc0000000, b = 0xd0000000;
unsigned long c = 0xc01bb958, d = 0xc0264000;
unsigned long e = 0xc0288000, f = 0xc02d4378;

void foo(int x, int y, int z) {
  if (x != 245 || y != 36 || z != 444)
    abort();
}

int main(void) {
  unsigned long g;
  int           h = 0, i = 0, j = 0;

  if (sizeof(unsigned long) < 4)
    return 0;

  for (g = a; g < b; g += 0x1000)
    if (g < c)
      h++;
    else if (g >= d && g < e)
      j++;
    else if (g < f)
      i++;
  foo(i, j, h);
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: u64 [storage=static] = widen<u64, reason=assign>(const<u32>(3221225472)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: u64 [storage=static] = widen<u64, reason=assign>(const<u32>(3489660928)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: u64 [storage=static] = widen<u64, reason=assign>(const<u32>(3223042392)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: u64 [storage=static] = widen<u64, reason=assign>(const<u32>(3223732224)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: u64 [storage=static] = widen<u64, reason=assign>(const<u32>(3223879680)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: u64 [storage=static] = widen<u64, reason=assign>(const<u32>(3224191864)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32, %[[VALUE_z:[0-9]+]] z: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(245)), ne<i32>(read<i32>(%[[VALUE_y]]), const<i32>(36))), ne<i32>(read<i32>(%[[VALUE_z]]), const<i32>(444)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if lt<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_g]], read<u64>(%[[VALUE_a]]));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%[[VALUE_g]]), read<u64>(%[[VALUE_b]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_g]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE1]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4096))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_g]], read<u64>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if lt<u64>(read<u64>(%[[VALUE_g]]), read<u64>(%[[VALUE_c]]))
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_h]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_h]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if logical_and<bool>(ge<u64>(read<u64>(%[[VALUE_g]]), read<u64>(%[[VALUE_d]])), lt<u64>(read<u64>(%[[VALUE_g]]), read<u64>(%[[VALUE_e]])))
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<u64>(read<u64>(%[[VALUE_g]]), read<u64>(%[[VALUE_f]]))
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_foo]], read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j]]), read<i32>(%[[VALUE_h]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
