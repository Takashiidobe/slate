/* PR middle-end/108789 */

int add(unsigned *r, const unsigned *a, const unsigned *b) {
  return __builtin_add_overflow(*a, *b, r);
}

int mul(unsigned *r, const unsigned *a, const unsigned *b) {
  return __builtin_mul_overflow(*a, *b, r);
}

int main() {
  unsigned x;

  /* 1073741824U + 1073741824U should not overflow.  */
  x = (__INT_MAX__ + 1U) / 2;
  if (add(&x, &x, &x))
    __builtin_abort();

  /* 256U * 256U should not overflow */
  x = 1U << (sizeof(int) * __CHAR_BIT__ / 4);
  if (mul(&x, &x, &x))
    __builtin_abort();

  /* 2147483648U + 2147483648U should overflow */
  x = __INT_MAX__ + 1U;
  if (!add(&x, &x, &x))
    __builtin_abort();

  /* 65536U * 65536U should overflow */
  x = 1U << (sizeof(int) * __CHAR_BIT__ / 2);
  if (!mul(&x, &x, &x))
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_r:[0-9]+]] r: ptr<u32>, %[[VALUE_a:[0-9]+]] a: ptr<const u32>, %[[VALUE_b:[0-9]+]] b: ptr<const u32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(overflow_add<bool>(read<u32>(deref(read<ptr<const u32>>(%[[VALUE_a]]))), read<u32>(deref(read<ptr<const u32>>(%[[VALUE_b]]))), deref(read<ptr<u32>>(%[[VALUE_r]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_mul:[0-9]+]] @mul(%[[VALUE_r_2:[0-9]+]] r: ptr<u32>, %[[VALUE_a_2:[0-9]+]] a: ptr<const u32>, %[[VALUE_b_2:[0-9]+]] b: ptr<const u32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(overflow_mul<bool>(read<u32>(deref(read<ptr<const u32>>(%[[VALUE_a_2]]))), read<u32>(deref(read<ptr<const u32>>(%[[VALUE_b_2]]))), deref(read<ptr<u32>>(%[[VALUE_r_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%[[VALUE_x]], div<u32, by_zero=ub>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<u32>, ptr<const u32>, ptr<const u32>) -> i32>(%[[VALUE_add]], addr_of<ptr<u32>>(%[[VALUE_x]]), pointer_cast<ptr<const u32>, reason=arg>(addr_of<ptr<u32>>(%[[VALUE_x]])), pointer_cast<ptr<const u32>, reason=arg>(addr_of<ptr<u32>>(%[[VALUE_x]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_x]], shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), div<u64, by_zero=ub>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<u32>, ptr<const u32>, ptr<const u32>) -> i32>(%[[VALUE_mul]], addr_of<ptr<u32>>(%[[VALUE_x]]), pointer_cast<ptr<const u32>, reason=arg>(addr_of<ptr<u32>>(%[[VALUE_x]])), pointer_cast<ptr<const u32>, reason=arg>(addr_of<ptr<u32>>(%[[VALUE_x]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_x]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<u32>, ptr<const u32>, ptr<const u32>) -> i32>(%[[VALUE_add]], addr_of<ptr<u32>>(%[[VALUE_x]]), pointer_cast<ptr<const u32>, reason=arg>(addr_of<ptr<u32>>(%[[VALUE_x]])), pointer_cast<ptr<const u32>, reason=arg>(addr_of<ptr<u32>>(%[[VALUE_x]]))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_x]], shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), div<u64, by_zero=ub>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<u32>, ptr<const u32>, ptr<const u32>) -> i32>(%[[VALUE_mul]], addr_of<ptr<u32>>(%[[VALUE_x]]), pointer_cast<ptr<const u32>, reason=arg>(addr_of<ptr<u32>>(%[[VALUE_x]])), pointer_cast<ptr<const u32>, reason=arg>(addr_of<ptr<u32>>(%[[VALUE_x]]))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
