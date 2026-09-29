// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata
// SLATE-FILECHECK-STD DEFAULT c23

static_assert(_Generic(1L + 1u, unsigned long: 1, default: 0));
static_assert(_Generic(1L, long: 1, int: 0));
static_assert(_Generic(sizeof(int), unsigned long long: 1, default: 0));
static_assert(_Generic((unsigned short)1 + 0, int: 1, default: 0));

typeof(1L + 1u) mixed;
typeof(sizeof(int)) size;
typeof(1LL + 1u) wider;

unsigned long ranks(long a, unsigned b) {
    typeof(a + b) result = a + b;
    return result;
}
int selection(long a, unsigned b) {
    return _Generic(a + b, unsigned long: 1, default: 0);
}
long long difference(const int *a, int *b) { return a - b; }

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
// DEFAULT-NEXT:     global %[[VALUE_mixed:[0-9]+]] mixed: u32 [storage=static] [linkage=external] [c="typeof(1L + 1u)"] [c_canon="unsigned long"];
// DEFAULT-NEXT:     global %[[VALUE_size:[0-9]+]] size: u64 [storage=static] [linkage=external] [c="typeof(sizeof(...))"] [c_canon="unsigned long long"];
// DEFAULT-NEXT:     global %[[VALUE_wider:[0-9]+]] wider: i64 [storage=static] [linkage=external] [c="typeof(1LL + 1u)"] [c_canon="long long"];
// DEFAULT-NEXT:     fn %[[VALUE_ranks:[0-9]+]] @ranks(%[[VALUE_a:[0-9]+]] a: i32 [c="long"], %[[VALUE_b:[0-9]+]] b: u32 [c="unsigned int"]) -> u32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="unsigned long"] [c="unsigned long(long, unsigned int)"] {
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: u32 [storage=automatic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_a]])), read<u32>(%[[VALUE_b]])) [c="typeof(a + b)"] [c_canon="unsigned long"];
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_result]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_selection:[0-9]+]] @selection(%[[VALUE_a_2:[0-9]+]] a: i32 [c="long"], %[[VALUE_b_2:[0-9]+]] b: u32 [c="unsigned int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(long, unsigned int)"] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_difference:[0-9]+]] @difference(%[[VALUE_a_3:[0-9]+]] a: ptr<const i32> [c="const int *"], %[[VALUE_b_3:[0-9]+]] b: ptr<i32> [c="int *"]) -> i64 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="long long"] [c="long long(const int *, int *)"] {
// DEFAULT-NEXT:         return ptr_diff<i64, element=i32, same_array=required, overflow=ub>(read<ptr<const i32>>(%[[VALUE_a_3]]), read<ptr<i32>>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
