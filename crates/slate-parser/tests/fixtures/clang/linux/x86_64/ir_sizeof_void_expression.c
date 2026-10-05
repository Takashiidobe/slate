// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu11

void nothing(void);

int void_sized[sizeof(*(void *)0)];

unsigned long dereferenced_size(void *p) {
  return sizeof(*p);
}

unsigned long dereferenced_alignment(void *p) {
  return _Alignof(*p) + __alignof__(*p);
}

unsigned long void_call_size(void) {
  return sizeof(nothing());
}

unsigned long conditional_size(int c, void *p, int *q) {
  return sizeof(*(c ? p : q));
}

unsigned long is_constexpr_size(long n) {
  return sizeof(*(8 ? ((void *)(n * 0l)) : (int *)8));
}

int chosen_by_void_size(void *p) {
  return __builtin_choose_expr(sizeof(*p) == 1, 1, 2);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %[[VALUE_void_sized:[0-9]+]] void_sized: array<i32, 1> [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_nothing:[0-9]+]] @nothing() -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_dereferenced_size:[0-9]+]] @dereferenced_size(%[[VALUE_p:[0-9]+]] p: ptr<void>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_dereferenced_alignment:[0-9]+]] @dereferenced_alignment(%[[VALUE_p_2:[0-9]+]] p: ptr<void>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<u64, overflow=wrap>(const<u64>(1), const<u64>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_void_call_size:[0-9]+]] @void_call_size() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_conditional_size:[0-9]+]] @conditional_size(%[[VALUE_c:[0-9]+]] c: i32, %[[VALUE_p_3:[0-9]+]] p: ptr<void>, %[[VALUE_q:[0-9]+]] q: ptr<i32>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_is_constexpr_size:[0-9]+]] @is_constexpr_size(%[[VALUE_n:[0-9]+]] n: i64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_chosen_by_void_size:[0-9]+]] @chosen_by_void_size(%[[VALUE_p_4:[0-9]+]] p: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
