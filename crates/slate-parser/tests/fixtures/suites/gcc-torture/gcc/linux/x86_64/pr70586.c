/* PR tree-optimization/70586 */

int   a, e, f;
short b, c, d;

int foo(int x, int y) { return (y == 0 || (x && y == 1)) ? x : x % y; }

static short bar(void) {
  int i = foo(c, f);
  f     = foo(d, 2);
  int g = foo(b, c);
  int h = foo(g > 0, c);
  c     = (3 >= h ^ 7) <= foo(i, c);
  if (foo(e, 1))
    return a;
  return 0;
}

int main() {
  bar();
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
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_or<bool>(eq<i32>(read<i32>(%[[VALUE_y]]), const<i32>(0)), logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0)), eq<i32>(read<i32>(%[[VALUE_y]]), const<i32>(1)))), read<i32>(%[[VALUE_x]]), rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_y]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> i16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_foo]], widen<i32, reason=arg>(read<i16>(%[[VALUE_c]])), read<i32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_f]], call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_foo]], widen<i32, reason=arg>(read<i16>(%[[VALUE_d]])), const<i32>(2)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_foo]], widen<i32, reason=arg>(read<i16>(%[[VALUE_d]])), const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: i32 [storage=automatic] = call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_foo]], widen<i32, reason=arg>(read<i16>(%[[VALUE_b]])), widen<i32, reason=arg>(read<i16>(%[[VALUE_c]])));
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: i32 [storage=automatic] = call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_foo]], from_bool<i32, reason=arg>(gt<i32>(read<i32>(%[[VALUE_g]]), const<i32>(0))), widen<i32, reason=arg>(read<i16>(%[[VALUE_c]])));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_c]], from_bool<i16, reason=assign>(le<i32>(xor<i32>(from_bool<i32, reason=promotion>(ge<i32>(const<i32>(3), read<i32>(%[[VALUE_h]]))), const<i32>(7)), call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_foo]], read<i32>(%[[VALUE_i]]), widen<i32, reason=arg>(read<i16>(%[[VALUE_c]]))))));
// DEFAULT-NEXT:         from_bool<i16, reason=assign>(le<i32>(xor<i32>(from_bool<i32, reason=promotion>(ge<i32>(const<i32>(3), read<i32>(%[[VALUE_h]]))), const<i32>(7)), call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_foo]], read<i32>(%[[VALUE_i]]), widen<i32, reason=arg>(read<i16>(%[[VALUE_c]])))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_foo]], read<i32>(%[[VALUE_e]]), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             return truncate<i16, reason=return, fits=unknown>(read<i32>(%[[VALUE_a]]));
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i16, signature=fn() -> i16>(%[[VALUE_bar]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
