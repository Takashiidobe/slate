/* PR tree-optimization/95731 */

__attribute__((noipa)) int foo(int x, int y, int z, int w, long long u,
                               long long v) {
  return x >= 0 && y >= 0 && z < 0 && u < 0 && w >= 0 && v < 0;
}

__attribute__((noipa)) int bar(int x, int y, int z, int w, long long u,
                               long long v) {
  return u >= 0 && x >= 0 && y >= 0 && v < 0 && z >= 0 && w >= 0;
}

__attribute__((noipa)) int baz(int x, int y, int z, int w, long long u,
                               long long v) {
  return x >= 0 || u < 0 || y >= 0 || v < 0 || z >= 0 || w >= 0;
}

int main() {
  int i;
  for (i = 0; i < 64; i++) {
    int a =
        foo((i & 1) ? -123 : 456, (i & 2) ? -123 : 456, (i & 4) ? -123 : 456,
            (i & 8) ? -123 : 456, (i & 16) ? -123 : 456, (i & 32) ? -123 : 456);
    int b =
        bar((i & 1) ? -123 : 456, (i & 2) ? -123 : 456, (i & 4) ? -123 : 456,
            (i & 8) ? -123 : 456, (i & 16) ? -123 : 456, (i & 32) ? -123 : 456);
    int c =
        baz((i & 1) ? -123 : 456, (i & 2) ? -123 : 456, (i & 4) ? -123 : 456,
            (i & 8) ? -123 : 456, (i & 16) ? -123 : 456, (i & 32) ? -123 : 456);
    if (a != (i == 52) || b != (i == 32) || c != (i != 15))
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32, %[[VALUE_z:[0-9]+]] z: i32, %[[VALUE_w:[0-9]+]] w: i32, %[[VALUE_u:[0-9]+]] u: i64, %[[VALUE_v:[0-9]+]] v: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(ge<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0)), ge<i32>(read<i32>(%[[VALUE_y]]), const<i32>(0))), lt<i32>(read<i32>(%[[VALUE_z]]), const<i32>(0))), lt<i64>(read<i64>(%[[VALUE_u]]), widen<i64, reason=usual_arith>(const<i32>(0)))), ge<i32>(read<i32>(%[[VALUE_w]]), const<i32>(0))), lt<i64>(read<i64>(%[[VALUE_v]]), widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: i32, %[[VALUE_y_2:[0-9]+]] y: i32, %[[VALUE_z_2:[0-9]+]] z: i32, %[[VALUE_w_2:[0-9]+]] w: i32, %[[VALUE_u_2:[0-9]+]] u: i64, %[[VALUE_v_2:[0-9]+]] v: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(ge<i64>(read<i64>(%[[VALUE_u_2]]), widen<i64, reason=usual_arith>(const<i32>(0))), ge<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(0))), ge<i32>(read<i32>(%[[VALUE_y_2]]), const<i32>(0))), lt<i64>(read<i64>(%[[VALUE_v_2]]), widen<i64, reason=usual_arith>(const<i32>(0)))), ge<i32>(read<i32>(%[[VALUE_z_2]]), const<i32>(0))), ge<i32>(read<i32>(%[[VALUE_w_2]]), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_3:[0-9]+]] x: i32, %[[VALUE_y_3:[0-9]+]] y: i32, %[[VALUE_z_3:[0-9]+]] z: i32, %[[VALUE_w_3:[0-9]+]] w: i32, %[[VALUE_u_3:[0-9]+]] u: i64, %[[VALUE_v_3:[0-9]+]] v: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ge<i32>(read<i32>(%[[VALUE_x_3]]), const<i32>(0)), lt<i64>(read<i64>(%[[VALUE_u_3]]), widen<i64, reason=usual_arith>(const<i32>(0)))), ge<i32>(read<i32>(%[[VALUE_y_3]]), const<i32>(0))), lt<i64>(read<i64>(%[[VALUE_v_3]]), widen<i64, reason=usual_arith>(const<i32>(0)))), ge<i32>(read<i32>(%[[VALUE_z_3]]), const<i32>(0))), ge<i32>(read<i32>(%[[VALUE_w_3]]), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(64))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = call<i32, signature=fn(i32, i32, i32, i32, i64, i64) -> i32>(%[[VALUE_foo]], conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456)), conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456)), conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456)), conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(8)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456)), widen<i64, reason=arg>(conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(16)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456))), widen<i64, reason=arg>(conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(32)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456))));
// DEFAULT-NEXT:                     let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = call<i32, signature=fn(i32, i32, i32, i32, i64, i64) -> i32>(%[[VALUE_bar]], conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456)), conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456)), conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456)), conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(8)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456)), widen<i64, reason=arg>(conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(16)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456))), widen<i64, reason=arg>(conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(32)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456))));
// DEFAULT-NEXT:                     let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic] = call<i32, signature=fn(i32, i32, i32, i32, i64, i64) -> i32>(%[[VALUE_baz]], conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456)), conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456)), conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456)), conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(8)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456)), widen<i64, reason=arg>(conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(16)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456))), widen<i64, reason=arg>(conditional<i32>(ne<i32>(and<i32>(read<i32>(%[[VALUE_i]]), const<i32>(32)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(123)), const<i32>(456))));
// DEFAULT-NEXT:                     if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_a]]), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(52)))), ne<i32>(read<i32>(%[[VALUE_b]]), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(32))))), ne<i32>(read<i32>(%[[VALUE_c]]), from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(15)))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
