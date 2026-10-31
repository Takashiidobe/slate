// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
typedef int *_Nullable maybe_int;
typedef int *plain_int;

int parameter(const char *_Nonnull p);
int callback(int (*_Nonnull compare)(const void *_Nullable, const void *_Null_unspecified));
int *_Nullable returned(void);
int *_Nonnull const qualified(int *const _Nullable_result p);
int array_parameter(int a[_Nonnull]);
int sized_array_parameter(int a[_Nullable 3]);
int (*_Nullable function_pointer)(void);
int *_Nonnull _Nonnull repeated;
_Nonnull plain_int before_typedef;
maybe_int _Nullable after_typedef;
int **_Nullable *_Nonnull levels;

int use(int *_Nullable p) {
    int *_Nonnull local = (int *_Nonnull)p;
    return (int)sizeof(int *_Nullable) + (int)sizeof(_Nonnull plain_int) + *local;
}

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
// DEFAULT-NEXT:     type @type[[TYPE_maybe_int:[0-9]+]] maybe_int = ptr<i32>;
// DEFAULT-NEXT:     type @type[[TYPE_plain_int:[0-9]+]] plain_int = ptr<i32>;
// DEFAULT-NEXT:     global %[[VALUE_function_pointer:[0-9]+]] function_pointer: ptr<fn() -> i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_repeated:[0-9]+]] repeated: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_before_typedef:[0-9]+]] before_typedef: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_after_typedef:[0-9]+]] after_typedef: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_levels:[0-9]+]] levels: ptr<ptr<ptr<i32>>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_parameter:[0-9]+]] @parameter(%[[VALUE_p:[0-9]+]] p: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_callback:[0-9]+]] @callback(%[[VALUE_compare:[0-9]+]] compare: ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_returned:[0-9]+]] @returned() -> ptr<i32> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_qualified:[0-9]+]] @qualified(%[[VALUE_p_2:[0-9]+]] p: ptr<i32> [const]) -> ptr<i32> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_array_parameter:[0-9]+]] @array_parameter(%[[VALUE_a:[0-9]+]] a: ptr<i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sized_array_parameter:[0-9]+]] @sized_array_parameter(%[[VALUE_a_2:[0-9]+]] a: ptr<i32> [array=3]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE_p_3:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_local:[0-9]+]] local: ptr<i32> [storage=automatic] = read<ptr<i32>>(%[[VALUE_p_3]]);
// DEFAULT-NEXT:         return add<i32>(add<i32>(reinterpret<i32>(truncate<u32>(const<u64>(8))), reinterpret<i32>(truncate<u32>(const<u64>(8)))), read<i32>(deref(read<ptr<i32>>(%[[VALUE_local]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
