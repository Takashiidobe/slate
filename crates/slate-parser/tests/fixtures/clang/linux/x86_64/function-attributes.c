__attribute__((visibility("hidden"))) int hidden_data;
extern int weak_data __attribute__((weak, used, section(".data")));

int declared(int *p) __attribute__((nonnull(1), noinline));
int parameterized(int p __attribute__((unused)));

__attribute__((noinline)) int definition() __attribute__((pure)) {
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
// DEFAULT-NEXT:     global %[[VALUE_hidden_data:[0-9]+]] hidden_data: i32 [storage=static] [linkage=external] [visibility=hidden];
// DEFAULT-NEXT:     extern %[[VALUE_weak_data:[0-9]+]] weak_data: i32 [storage=static] [linkage=external] [weak] [section=".data"] [used];
// DEFAULT-NEXT:     fn %[[VALUE_declared:[0-9]+]] @declared(%[[VALUE_p:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [inline=never];
// DEFAULT-NEXT:     fn %[[VALUE_parameterized:[0-9]+]] @parameterized(%[[VALUE_p_2:[0-9]+]] p: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_definition:[0-9]+]] @definition() -> i32 [linkage=external] [inline=never] [definition=emitted] [memory=read] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
