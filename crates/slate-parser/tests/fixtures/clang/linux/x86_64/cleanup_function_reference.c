void release(int *p);
void release_void(void *p);
void release_const(const int *p);
int release_variadic(int *p, ...);

int cleanup_targets(void) {
  int plain __attribute__((cleanup(release))) = 1;
  int erased __attribute__((cleanup(release_void))) = 2;
  volatile int dropped __attribute__((cleanup(release_const))) = 3;
  int variadic __attribute__((cleanup(release_variadic))) = 4;
  return plain + erased + dropped + variadic;
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
// DEFAULT-NEXT:     fn %[[VALUE_release:[0-9]+]] @release(%[[VALUE_p:[0-9]+]] p: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_release_void:[0-9]+]] @release_void(%[[VALUE_p_2:[0-9]+]] p: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_release_const:[0-9]+]] @release_const(%[[VALUE_p_3:[0-9]+]] p: ptr<const i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_release_variadic:[0-9]+]] @release_variadic(%[[VALUE_p_4:[0-9]+]] p: ptr<i32>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cleanup_targets:[0-9]+]] @cleanup_targets() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_plain:[0-9]+]] plain: i32 [storage=automatic] [cleanup=%[[VALUE_release]]] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE_erased:[0-9]+]] erased: i32 [storage=automatic] [cleanup=%[[VALUE_release_void]]] = const<i32>(2);
// DEFAULT-NEXT:         let %[[VALUE_dropped:[0-9]+]] dropped: volatile i32 [storage=automatic] [cleanup=%[[VALUE_release_const]]] = const<i32>(3);
// DEFAULT-NEXT:         let %[[VALUE_variadic:[0-9]+]] variadic: i32 [storage=automatic] [cleanup=%[[VALUE_release_variadic]]] = const<i32>(4);
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_plain]]), read<i32>(%[[VALUE_erased]])), read<i32, volatile>(%[[VALUE_dropped]])), read<i32>(%[[VALUE_variadic]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
