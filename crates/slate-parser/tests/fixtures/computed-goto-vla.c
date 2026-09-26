int computed_goto(int n) {
  void *labels[2] = { &&L0, &&L1 };
#ifdef USE_COMPUTED_GOTO
  goto *labels[n];
#else
  goto L1;
#endif
L0:
  return 0;
L1:
  return 1;
}

int vla_sum(int n, int arr[n]) {
#ifdef DOUBLE_LOCAL
  int local[n * 2];
#else
  int local[n];
#endif
  return local[0] + arr[0];
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES COMPUTED USE_COMPUTED_GOTO
// SLATE-FILECHECK-DEFINES DOUBLED DOUBLE_LOCAL

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
// DEFAULT-NEXT:     fn %0 @computed_goto(%3 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 labels: array<ptr<void>, 2> [storage=automatic] [align=16] = aggregate<array<ptr<void>, 2>, zero_fill=false>(index0 = label_addr<ptr<void>>(%1), index1 = label_addr<ptr<void>>(%2));
// DEFAULT-NEXT:         goto %2;
// DEFAULT-NEXT:         label %1 L0:
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         label %2 L1:
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @vla_sum(%6 n: i32, %7 arr: ptr<i32> [array=%9]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%6)));
// DEFAULT-NEXT:         let %10: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%6)));
// DEFAULT-NEXT:         let %8 local: vla<i32, %10> [storage=automatic];
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(%8), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%7), const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN COMPUTED
// COMPUTED: module {
// COMPUTED-NEXT:     target "x86_64-unknown-linux-gnu" {
// COMPUTED-NEXT:         endian = little;
// COMPUTED-NEXT:         pointer [size=8, align=8];
// COMPUTED-NEXT:         stack_alignment = 16;
// COMPUTED-NEXT:         long_double = f80;
// COMPUTED-NEXT:         storage bool [size=1, align=1];
// COMPUTED-NEXT:         storage i8, u8 [size=1, align=1];
// COMPUTED-NEXT:         storage i16, u16 [size=2, align=2];
// COMPUTED-NEXT:         storage i32, u32 [size=4, align=4];
// COMPUTED-NEXT:         storage i64, u64 [size=8, align=8];
// COMPUTED-NEXT:         storage i128, u128 [size=16, align=16];
// COMPUTED-NEXT:         storage bf16 [size=2, align=2];
// COMPUTED-NEXT:         storage f16 [size=2, align=2];
// COMPUTED-NEXT:         storage f32 [size=4, align=4];
// COMPUTED-NEXT:         storage f64 [size=8, align=8];
// COMPUTED-NEXT:         storage f80 [size=16, align=16];
// COMPUTED-NEXT:         storage f128 [size=16, align=16];
// COMPUTED-NEXT:         storage d32 [size=4, align=4];
// COMPUTED-NEXT:         storage d64 [size=8, align=8];
// COMPUTED-NEXT:         storage d128 [size=16, align=16];
// COMPUTED-NEXT:     }
// COMPUTED-NEXT:     fn %0 @computed_goto(%3 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// COMPUTED-NEXT:         let %4 labels: array<ptr<void>, 2> [storage=automatic] [align=16] = aggregate<array<ptr<void>, 2>, zero_fill=false>(index0 = label_addr<ptr<void>>(%1), index1 = label_addr<ptr<void>>(%2));
// COMPUTED-NEXT:         goto *read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(2)>(%4), read<i32>(%3))));
// COMPUTED-NEXT:         label %1 L0:
// COMPUTED-NEXT:             return const<i32>(0);
// COMPUTED-NEXT:         label %2 L1:
// COMPUTED-NEXT:             return const<i32>(1);
// COMPUTED-NEXT:     }
// COMPUTED-NEXT:     fn %5 @vla_sum(%6 n: i32, %7 arr: ptr<i32> [array=%9]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// COMPUTED-NEXT:         let %9: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%6)));
// COMPUTED-NEXT:         let %10: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%6)));
// COMPUTED-NEXT:         let %8 local: vla<i32, %10> [storage=automatic];
// COMPUTED-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(%8), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%7), const<i32>(0)))));
// COMPUTED-NEXT:     }
// COMPUTED-NEXT: }
// SLATE-FILECHECK-END COMPUTED
// SLATE-FILECHECK-BEGIN DOUBLED
// DOUBLED: module {
// DOUBLED-NEXT:     target "x86_64-unknown-linux-gnu" {
// DOUBLED-NEXT:         endian = little;
// DOUBLED-NEXT:         pointer [size=8, align=8];
// DOUBLED-NEXT:         stack_alignment = 16;
// DOUBLED-NEXT:         long_double = f80;
// DOUBLED-NEXT:         storage bool [size=1, align=1];
// DOUBLED-NEXT:         storage i8, u8 [size=1, align=1];
// DOUBLED-NEXT:         storage i16, u16 [size=2, align=2];
// DOUBLED-NEXT:         storage i32, u32 [size=4, align=4];
// DOUBLED-NEXT:         storage i64, u64 [size=8, align=8];
// DOUBLED-NEXT:         storage i128, u128 [size=16, align=16];
// DOUBLED-NEXT:         storage bf16 [size=2, align=2];
// DOUBLED-NEXT:         storage f16 [size=2, align=2];
// DOUBLED-NEXT:         storage f32 [size=4, align=4];
// DOUBLED-NEXT:         storage f64 [size=8, align=8];
// DOUBLED-NEXT:         storage f80 [size=16, align=16];
// DOUBLED-NEXT:         storage f128 [size=16, align=16];
// DOUBLED-NEXT:         storage d32 [size=4, align=4];
// DOUBLED-NEXT:         storage d64 [size=8, align=8];
// DOUBLED-NEXT:         storage d128 [size=16, align=16];
// DOUBLED-NEXT:     }
// DOUBLED-NEXT:     fn %0 @computed_goto(%3 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DOUBLED-NEXT:         let %4 labels: array<ptr<void>, 2> [storage=automatic] [align=16] = aggregate<array<ptr<void>, 2>, zero_fill=false>(index0 = label_addr<ptr<void>>(%1), index1 = label_addr<ptr<void>>(%2));
// DOUBLED-NEXT:         goto %2;
// DOUBLED-NEXT:         label %1 L0:
// DOUBLED-NEXT:             return const<i32>(0);
// DOUBLED-NEXT:         label %2 L1:
// DOUBLED-NEXT:             return const<i32>(1);
// DOUBLED-NEXT:     }
// DOUBLED-NEXT:     fn %5 @vla_sum(%6 n: i32, %7 arr: ptr<i32> [array=%9]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DOUBLED-NEXT:         let %9: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%6)));
// DOUBLED-NEXT:         let %10: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(mul<i32, overflow=ub>(read<i32>(%6), const<i32>(2))));
// DOUBLED-NEXT:         let %8 local: vla<i32, %10> [storage=automatic];
// DOUBLED-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(%8), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%7), const<i32>(0)))));
// DOUBLED-NEXT:     }
// DOUBLED-NEXT: }
// SLATE-FILECHECK-END DOUBLED
