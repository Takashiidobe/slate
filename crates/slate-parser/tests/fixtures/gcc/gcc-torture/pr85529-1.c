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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %3 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 g: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %7 s: volatile @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %8 @foo(%9 i: i8, %10 j: i32) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(conditional<i32>(lt<i32>(widen<i32, reason=promotion>(read<i8>(%9)), const<i32>(0)), widen<i32, reason=promotion>(read<i8>(%9)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(%9)), read<i32>(%10))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 k: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(83)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%3), const<i32>(0)))
// DEFAULT-NEXT:             goto %12;
// DEFAULT-NEXT:         write<i8>(%13, from_bool<i8, reason=assign>(logical_or<bool>(ne<i32>(read<i32>(%4), const<i32>(0)), ne<i32>(read<i32>(%5), const<i32>(0)))));
// DEFAULT-NEXT:         label %12 L:
// DEFAULT-NEXT:             for %14
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                 condition: lt<i32>(read<i32>(%1), const<i32>(1))
// DEFAULT-NEXT:                 increment: {
// DEFAULT-NEXT:                     let %16: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                     let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%1, read<i32>(%17));
// DEFAULT-NEXT:                     yield void;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     let %18: bool [synthetic];
// DEFAULT-NEXT:                     if lt<i32>(widen<i32, reason=promotion>(read<i8>(%13)), widen<i32, reason=promotion>(call<i8, signature=fn(i8, i32) -> i8>(%8, read<i8>(%13), const<i32>(2))))
// DEFAULT-NEXT:                         write<i8>(%13, truncate<i8, reason=assign, fits=unknown>(read<i32>(%6)));
// DEFAULT-NEXT:                         write<i32>(%2, widen<i32, reason=assign>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%6))));
// DEFAULT-NEXT:                         write<bool>(%18, ne<i32>(widen<i32, reason=assign>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%6))), const<i32>(0)));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%18, const<bool>(false));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
