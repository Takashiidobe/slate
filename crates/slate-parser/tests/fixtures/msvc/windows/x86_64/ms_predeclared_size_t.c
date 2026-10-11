size_t length(const char *s);
typedef unsigned long long size_t;
_Static_assert(sizeof(size_t) == sizeof(void *), "pointer width");
_Static_assert(_Generic((size_t)0, unsigned long long: 1, default: 0), "size type");
size_t twice(size_t n) {
    typedef int size_t;
    size_t local = (size_t)n;
    return local * 2;
}
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_size_t_2:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_size_t_3:[0-9]+]] size_t = i32;
// DEFAULT-NEXT:     fn %[[VALUE_length:[0-9]+]] @length(%[[VALUE_s:[0-9]+]] s: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_twice:[0-9]+]] @twice(%[[VALUE_n:[0-9]+]] n: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_local:[0-9]+]] local: i32 [storage=automatic] = reinterpret<i32>(truncate<u32>(read<u64>(%[[VALUE_n]])));
// DEFAULT-NEXT:         return reinterpret<u64>(widen<i64>(mul<i32>(read<i32>(%[[VALUE_local]]), const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
