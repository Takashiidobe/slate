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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_mempcpy:[0-9]+]] @__builtin_mempcpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_unused_indirect:[0-9]+]] @test_unused_indirect(%[[VALUE_d:[0-9]+]] d: ptr<void>, %[[VALUE_s:[0-9]+]] s: ptr<const void>, %[[VALUE_n:[0-9]+]] n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_mempcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), read<u64>(%[[VALUE_n]]));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: ptr<void> [storage=automatic] = read<ptr<void>>(%[[VALUE_a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_used_simple:[0-9]+]] @test_used_simple(%[[VALUE_d_2:[0-9]+]] d: ptr<void>, %[[VALUE_s_2:[0-9]+]] s: ptr<const void>, %[[VALUE_n_2:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_mempcpy]], read<ptr<void>>(%[[VALUE_d_2]]), read<ptr<const void>>(%[[VALUE_s_2]]), read<u64>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_used_in_expr:[0-9]+]] @test_used_in_expr(%[[VALUE_d_3:[0-9]+]] d: ptr<i8>, %[[VALUE_s_3:[0-9]+]] s: ptr<const i8>, %[[VALUE_n_3:[0-9]+]] n: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_mempcpy]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_d_3]])), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%[[VALUE_s_3]])), read<u64>(%[[VALUE_n_3]]))), read<ptr<i8>>(%[[VALUE_d_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_unused_indirect2:[0-9]+]] @test_unused_indirect2(%[[VALUE_d_4:[0-9]+]] d: ptr<void>, %[[VALUE_s_4:[0-9]+]] s: ptr<const void>, %[[VALUE_n_4:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_mempcpy]], read<ptr<void>>(%[[VALUE_d_4]]), read<ptr<const void>>(%[[VALUE_s_4]]), read<u64>(%[[VALUE_n_4]]));
// DEFAULT-NEXT:         if gt<u64>(read<u64>(%[[VALUE_n_4]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(20))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return int_to_ptr<ptr<void>, reason=explicit>(const<i32>(20));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return int_to_ptr<ptr<void>, reason=explicit>(const<i32>(7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_maybe_used:[0-9]+]] @test_maybe_used(%[[VALUE_d_5:[0-9]+]] d: ptr<void>, %[[VALUE_s_5:[0-9]+]] s: ptr<const void>, %[[VALUE_n_5:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a_3:[0-9]+]] a: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_mempcpy]], read<ptr<void>>(%[[VALUE_d_5]]), read<ptr<const void>>(%[[VALUE_s_5]]), read<u64>(%[[VALUE_n_5]]));
// DEFAULT-NEXT:         if gt<u64>(read<u64>(%[[VALUE_n_5]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(20))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return read<ptr<void>>(%[[VALUE_a_3]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
