#define SAME(T, U) _Generic((T)0, U: 1, default: 0)
_Static_assert(SAME(__int8, char), "__int8 is plain char");
_Static_assert(SAME(signed __int8, signed char), "signed __int8");
_Static_assert(SAME(unsigned _int8, unsigned char), "_int8 alias");
_Static_assert(SAME(__int16, short), "__int16");
_Static_assert(SAME(unsigned __int32, unsigned int), "__int32");
_Static_assert(SAME(__int64, long long), "__int64");
_Static_assert(SAME(__int64 unsigned int, unsigned long long), "__int64 width");
_Static_assert(SAME(long __int64, long long), "long __int64");

typedef unsigned __int64 uintptr_like;
_int64 const volatile counter;
__int32 sum(__int16 a, unsigned __int8 b) { return a + b; }
unsigned __int64 widen(__int32 value) { return (unsigned __int64)value; }
// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-STD DEFAULT c17
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc
// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     type @type0 uintptr_like = u64;
// DEFAULT-NEXT:     global %1 counter: volatile i64 [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     fn %2 @sum(%3 a: i16, %4 b: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%3)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @widen(%6 value: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(read<i32>(%6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
