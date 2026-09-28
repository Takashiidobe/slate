
typedef __SIZE_TYPE__ size_t;
typedef __WCHAR_TYPE__ wchar_t;

const wchar_t *wmemchr_test(const wchar_t *s, const wchar_t c, size_t n) {


  //
  return __builtin_wmemchr(s, c, n);
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 wchar_t = u16;
// DEFAULT-NEXT:     fn %9 @__builtin_wmemchr(%6 <unnamed>: ptr<const u16>, %7 <unnamed>: u16, %8 <unnamed>: u64) -> ptr<u16> [linkage=external];
// DEFAULT-NEXT:     fn %2 @wmemchr_test(%3 s: ptr<const u16>, %4 c: u16 [const], %5 n: u64) -> ptr<const u16> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<const u16>, reason=return>(call<ptr<u16>, signature=fn(ptr<const u16>, u16, u64) -> ptr<u16>>(%9, read<ptr<const u16>>(%3), read<u16>(%4), read<u64>(%5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
