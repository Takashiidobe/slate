/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-optimized" } */

/* Indirectly unused result */
void test_unused_indirect (void *d, const void *s, __SIZE_TYPE__ n) {
  void *a = __builtin_mempcpy (d, s, n);
  void *b = a;
}

/* Simple used result (in statement) */
void *test_used_simple (void *d, const void *s, __SIZE_TYPE__ n) {
  return __builtin_mempcpy (d, s, n);
}

/* More complicated used result (in expression) */
__SIZE_TYPE__ test_used_in_expr (char *d, const char *s, __SIZE_TYPE__ n) {
  return (char *)__builtin_mempcpy (d, s, n) - d;
}

/* Unused in all paths */
void *test_unused_indirect2 (void *d, const void *s, __SIZE_TYPE__ n) {
  void *a = __builtin_mempcpy (d, s, n);
  if (n > 20) {
	return (void *)20;
  }
  return (void *)7;
}

/* Used in at least one path */
void *test_maybe_used (void *d, const void *s, __SIZE_TYPE__ n) {
  void *a = __builtin_mempcpy (d, s, n);
  if (n > 20) {
    return a;
  }
  return (void *)0;
}

/* { dg-final { scan-tree-dump-times "__builtin_memcpy" 2 "optimized" } } */
/* { dg-final { scan-tree-dump-times "__builtin_mempcpy" 3 "optimized" } } */

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %27 @__builtin_mempcpy(%24 <unnamed>: ptr<void>, %25 <unnamed>: ptr<const void>, %26 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %0 @test_unused_indirect(%1 d: ptr<void>, %2 s: ptr<const void>, %3 n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 a: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%27, read<ptr<void>>(%1), read<ptr<const void>>(%2), read<u64>(%3));
// DEFAULT-NEXT:         let %5 b: ptr<void> [storage=automatic] = read<ptr<void>>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_used_simple(%7 d: ptr<void>, %8 s: ptr<const void>, %9 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%27, read<ptr<void>>(%7), read<ptr<const void>>(%8), read<u64>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test_used_in_expr(%11 d: ptr<i8>, %12 s: ptr<const i8>, %13 n: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%27, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%11)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%12)), read<u64>(%13))), read<ptr<i8>>(%11)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test_unused_indirect2(%15 d: ptr<void>, %16 s: ptr<const void>, %17 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %18 a: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%27, read<ptr<void>>(%15), read<ptr<const void>>(%16), read<u64>(%17));
// DEFAULT-NEXT:         if gt<u64>(read<u64>(%17), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(20))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return int_to_ptr<ptr<void>, reason=explicit>(const<i32>(20));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return int_to_ptr<ptr<void>, reason=explicit>(const<i32>(7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test_maybe_used(%20 d: ptr<void>, %21 s: ptr<const void>, %22 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 a: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%27, read<ptr<void>>(%20), read<ptr<const void>>(%21), read<u64>(%22));
// DEFAULT-NEXT:         if gt<u64>(read<u64>(%22), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(20))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return read<ptr<void>>(%23);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
