/* PR rtl-optimization/45912 */

extern void abort(void);

static void *__attribute__((noinline, noclone))
get_addr_base_and_unit_offset(void *base, long long *i) {
  *i = 0;
  return base;
}

static void *__attribute__((noinline, noclone))
build_int_cst(void *base, long long offset) {
  if (offset != 4)
    abort();

  return base;
}

static void *__attribute__((noinline, noclone))
build_ref_for_offset(void *base, long long offset) {
  long long base_offset;
  base = get_addr_base_and_unit_offset(base, &base_offset);
  return build_int_cst(base, base_offset + offset / 8);
}

int main(void) {
  void *ret = build_ref_for_offset((void *)0, 32);
  if (ret != (void *)0)
    abort();
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @get_addr_base_and_unit_offset(%2 base: ptr<void>, %3 i: ptr<i64>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%3)), widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         return read<ptr<void>>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @build_int_cst(%5 base: ptr<void>, %6 offset: i64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%6), widen<i64, reason=usual_arith>(const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return read<ptr<void>>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @build_ref_for_offset(%8 base: ptr<void>, %9 offset: i64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 base_offset: i64 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<void>>(%8, call<ptr<void>, signature=fn(ptr<void>, ptr<i64>) -> ptr<void>>(%1, read<ptr<void>>(%8), addr_of<ptr<i64>>(%10)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<i64>) -> ptr<void>>(%1, read<ptr<void>>(%8), addr_of<ptr<i64>>(%10));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(ptr<void>, i64) -> ptr<void>>(%4, read<ptr<void>>(%8), add<i64, overflow=ub>(read<i64>(%10), div<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%9), widen<i64, reason=usual_arith>(const<i32>(8)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 ret: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, i64) -> ptr<void>>(%7, null<ptr<void>>, widen<i64, reason=arg>(const<i32>(32)));
// DEFAULT-NEXT:         if ne<ptr<void>>(read<ptr<void>>(%12), null<ptr<void>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
