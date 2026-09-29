/* PR tree-optimization/85529 */

struct S {
  int a;
};

int               b, c = 1, d, e, f;
static int        g;
volatile struct S s;

signed char foo(signed char i, int j) { return i < 0 ? i : i << j; }

int main() {
  signed char k = -83;
  if (!d)
    goto L;
  k = e || f;
L:
  for (; b < 1; b++)
    s.a != (k < foo(k, 2) && (c = k = g));
  if (c != 1)
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: volatile @type[[TYPE_S]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_i:[0-9]+]] i: i8, %[[VALUE_j:[0-9]+]] j: i32) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(conditional<i32>(lt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_i]])), const<i32>(0)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_i]])), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_i]])), read<i32>(%[[VALUE_j]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(83)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0)))
// DEFAULT-NEXT:             goto %[[VALUE_L:[0-9]+]];
// DEFAULT-NEXT:         write<i8>(%[[VALUE_k]], from_bool<i8, reason=assign>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_e]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_f]]), const<i32>(0)))));
// DEFAULT-NEXT:         label %[[VALUE_L]] L:
// DEFAULT-NEXT:             for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                 condition: lt<i32>(read<i32>(%[[VALUE_b]]), const<i32>(1))
// DEFAULT-NEXT:                 increment: {
// DEFAULT-NEXT:                     let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                     yield void;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                     if lt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_k]])), widen<i32, reason=promotion>(call<i8, signature=fn(i8, i32) -> i8>(%[[VALUE_foo]], read<i8>(%[[VALUE_k]]), const<i32>(2))))
// DEFAULT-NEXT:                         write<i8>(%[[VALUE_k]], truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_g]])));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_c]], widen<i32, reason=assign>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_g]]))));
// DEFAULT-NEXT:                         write<bool>(%[[VALUE3]], ne<i32>(widen<i32, reason=assign>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_g]]))), const<i32>(0)));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%[[VALUE3]], const<bool>(false));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
