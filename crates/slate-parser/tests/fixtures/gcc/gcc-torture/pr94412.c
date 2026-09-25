/* PR middle-end/94412 */

typedef unsigned V __attribute__((__vector_size__(sizeof(unsigned) * 2)));

void foo(V *v, V *w) { *w = -*v / 11; }

void bar(V *v, V *w) { *w = -18 / -*v; }

int main() {
  V a = (V){1, 0};
  V b = (V){3, __INT_MAX__};
  V c, d;
  foo(&a, &c);
  bar(&b, &d);
  if (c[0] != -1U / 11 || c[1] != 0 || d[0] != 0 || d[1] != -18U / -__INT_MAX__)
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
// DEFAULT-NEXT:     type @type0 V = vector<u32, 2>;
// DEFAULT-NEXT:     fn %1 @foo(%2 v: ptr<vector<u32, 2>>, %3 w: ptr<vector<u32, 2>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<vector<u32, 2>>(deref(read<ptr<vector<u32, 2>>>(%3)), div<vector<u32, 2>, elementwise=true, by_zero=ub>(neg<vector<u32, 2>, elementwise=true, overflow=wrap>(read<vector<u32, 2>>(deref(read<ptr<vector<u32, 2>>>(%2)))), vector_splat<vector<u32, 2>, reason=usual_arith>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(11)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar(%5 v: ptr<vector<u32, 2>>, %6 w: ptr<vector<u32, 2>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<vector<u32, 2>>(deref(read<ptr<vector<u32, 2>>>(%6)), div<vector<u32, 2>, elementwise=true, by_zero=ub>(vector_splat<vector<u32, 2>, reason=usual_arith>(reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(18)))), neg<vector<u32, 2>, elementwise=true, overflow=wrap>(read<vector<u32, 2>>(deref(read<ptr<vector<u32, 2>>>(%5))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 a: vector<u32, 2> [storage=automatic] = read<vector<u32, 2>>(compound_literal %12 [storage=automatic] = aggregate<vector<u32, 2>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %9 b: vector<u32, 2> [storage=automatic] = read<vector<u32, 2>>(compound_literal %13 [storage=automatic] = aggregate<vector<u32, 2>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(3)), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2147483647))));
// DEFAULT-NEXT:         let %10 c: vector<u32, 2> [storage=automatic];
// DEFAULT-NEXT:         let %11 d: vector<u32, 2> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<vector<u32, 2>>, ptr<vector<u32, 2>>) -> void>(%1, addr_of<ptr<vector<u32, 2>>>(%8), addr_of<ptr<vector<u32, 2>>>(%10));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<vector<u32, 2>>, ptr<vector<u32, 2>>) -> void>(%4, addr_of<ptr<vector<u32, 2>>>(%9), addr_of<ptr<vector<u32, 2>>>(%11));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(read<u32>(lane(%10, const<i32>(0))), div<u32, by_zero=ub>(neg<u32, overflow=wrap>(const<u32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(11)))), ne<u32>(read<u32>(lane(%10, const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(read<u32>(lane(%11, const<i32>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(read<u32>(lane(%11, const<i32>(1))), div<u32, by_zero=ub>(neg<u32, overflow=wrap>(const<u32>(18)), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(2147483647))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
