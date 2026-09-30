
typedef __SIZE_TYPE__ size_t;
typedef __WCHAR_TYPE__ wchar_t;

int wmemcmp_test(const wchar_t *s1, const wchar_t *s2, size_t n) {



  //
  return __builtin_wmemcmp(s1, s2, n);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_wchar_t:[0-9]+]] wchar_t = u16;
// DEFAULT-NEXT:     fn %[[VALUE___builtin_wmemcmp:[0-9]+]] @__builtin_wmemcmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const u16>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const u16>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_wmemcmp_test:[0-9]+]] @wmemcmp_test(%[[VALUE_s1:[0-9]+]] s1: ptr<const u16>, %[[VALUE_s2:[0-9]+]] s2: ptr<const u16>, %[[VALUE_n:[0-9]+]] n: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const u16>, ptr<const u16>, u64) -> i32>(%[[VALUE___builtin_wmemcmp]], read<ptr<const u16>>(%[[VALUE_s1]]), read<ptr<const u16>>(%[[VALUE_s2]]), read<u64>(%[[VALUE_n]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
