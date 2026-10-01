/* PR tree-optimization/92618 */

typedef long long __m128i
    __attribute__((__vector_size__(2 * sizeof(long long)), __may_alias__));

double             a[4];
unsigned long long b[4];

__attribute__((noipa)) __m128i bar(void) {
  static int cnt;
  cnt += 2;
  return (__m128i){cnt, cnt + 1};
}

#if __SIZEOF_LONG_LONG__ == __SIZEOF_DOUBLE__
typedef double __m128d
    __attribute__((__vector_size__(2 * sizeof(double)), __may_alias__));

__attribute__((noipa)) __m128i qux(void) {
  static double cnt;
  cnt += 2.0;
  return (__m128i)(__m128d){cnt, cnt + 1.0};
}
#endif

void foo(unsigned long long *x) {
  __m128i c         = bar();
  __m128i d         = bar();
  *(__m128i *)&b[0] = c;
  *(__m128i *)&b[2] = d;
  *x                = b[0] + b[1] + b[2] + b[3];
}

void baz(double *x) {
#if __SIZEOF_LONG_LONG__ == __SIZEOF_DOUBLE__
  __m128i c         = qux();
  __m128i d         = qux();
  *(__m128i *)&a[0] = c;
  *(__m128i *)&a[2] = d;
  *x                = a[0] + a[1] + a[2] + a[3];
#endif
}

int main() {
  unsigned long long c = 0;
  foo(&c);
  if (c != 2 + 3 + 4 + 5)
    __builtin_abort();
#if __SIZEOF_LONG_LONG__ == __SIZEOF_DOUBLE__
  double d = 0.0;
  baz(&d);
  if (d != 2.0 + 3.0 + 4.0 + 5.0)
    __builtin_abort();
#endif
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
// DEFAULT-NEXT:     type @type[[TYPE___m128i:[0-9]+]] __m128i = vector<i64, 2>;
// DEFAULT-NEXT:     type @type[[TYPE___m128d:[0-9]+]] __m128d = vector<f64, 2>;
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<f64, 4> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: array<u64, 4> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cnt:[0-9]+]] cnt: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_cnt_2:[0-9]+]] cnt: f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> vector<i64, 2> [linkage=external] [abi=sysv64() -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_cnt]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_cnt]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         return read<vector<i64, 2>>(compound_literal %[[VALUE2:[0-9]+]] [storage=automatic] = aggregate<vector<i64, 2>, zero_fill=false>(index0 = widen<i64, reason=assign>(read<i32>(%[[VALUE_cnt]])), index1 = widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%[[VALUE_cnt]]), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_qux:[0-9]+]] @qux() -> vector<i64, 2> [linkage=external] [abi=sysv64() -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: f64 [synthetic] = read<f64>(%[[VALUE_cnt_2]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE3]]), const<f64>(2.0));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_cnt_2]], read<f64>(%[[VALUE4]]));
// DEFAULT-NEXT:         return vector_bit_cast<vector<i64, 2>, reason=explicit>(read<vector<f64, 2>>(compound_literal %[[VALUE5:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = read<f64>(%[[VALUE_cnt_2]]), index1 = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_cnt_2]]), const<f64>(1.0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<u64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: vector<i64, 2> [storage=automatic] = call<vector<i64, 2>, signature=fn() -> vector<i64, 2>, abi=sysv64() -> direct>(%[[VALUE_bar]]);
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: vector<i64, 2> [storage=automatic] = call<vector<i64, 2>, signature=fn() -> vector<i64, 2>, abi=sysv64() -> direct>(%[[VALUE_bar]]);
// DEFAULT-NEXT:         write<vector<i64, 2>>(deref(pointer_cast<ptr<vector<i64, 2>>, reason=explicit>(addr_of<ptr<u64>>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(4)>(%[[VALUE_b]]), const<i32>(0)))))), read<vector<i64, 2>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         write<vector<i64, 2>>(deref(pointer_cast<ptr<vector<i64, 2>>, reason=explicit>(addr_of<ptr<u64>>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(4)>(%[[VALUE_b]]), const<i32>(2)))))), read<vector<i64, 2>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%[[VALUE_x]])), add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(4)>(%[[VALUE_b]]), const<i32>(0)))), read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(4)>(%[[VALUE_b]]), const<i32>(1))))), read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(4)>(%[[VALUE_b]]), const<i32>(2))))), read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(4)>(%[[VALUE_b]]), const<i32>(3))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_2:[0-9]+]] x: ptr<f64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: vector<i64, 2> [storage=automatic] = call<vector<i64, 2>, signature=fn() -> vector<i64, 2>, abi=sysv64() -> direct>(%[[VALUE_qux]]);
// DEFAULT-NEXT:         let %[[VALUE_d_2:[0-9]+]] d: vector<i64, 2> [storage=automatic] = call<vector<i64, 2>, signature=fn() -> vector<i64, 2>, abi=sysv64() -> direct>(%[[VALUE_qux]]);
// DEFAULT-NEXT:         write<vector<i64, 2>>(deref(pointer_cast<ptr<vector<i64, 2>>, reason=explicit>(addr_of<ptr<f64>>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%[[VALUE_a]]), const<i32>(0)))))), read<vector<i64, 2>>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:         write<vector<i64, 2>>(deref(pointer_cast<ptr<vector<i64, 2>>, reason=explicit>(addr_of<ptr<f64>>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%[[VALUE_a]]), const<i32>(2)))))), read<vector<i64, 2>>(%[[VALUE_d_2]]));
// DEFAULT-NEXT:         write<f64>(deref(read<ptr<f64>>(%[[VALUE_x_2]])), add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%[[VALUE_a]]), const<i32>(0)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%[[VALUE_a]]), const<i32>(1))))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%[[VALUE_a]]), const<i32>(2))))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%[[VALUE_a]]), const<i32>(3))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_c_3:[0-9]+]] c: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u64>) -> void>(%[[VALUE_foo]], addr_of<ptr<u64>>(%[[VALUE_c_3]]));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_c_3]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(2), const<i32>(3)), const<i32>(4)), const<i32>(5)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_d_3:[0-9]+]] d: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f64>) -> void>(%[[VALUE_baz]], addr_of<ptr<f64>>(%[[VALUE_d_3]]));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%[[VALUE_d_3]]), add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(2.0), const<f64>(3.0)), const<f64>(4.0)), const<f64>(5.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
