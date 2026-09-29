// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

struct packed { char bytes[3]; } packed;
long aligned;

_Bool c11_word = __c11_atomic_is_lock_free(4);
_Bool c11_odd = __atomic_always_lock_free(3, 0);
_Bool always_word = __atomic_always_lock_free(8, 0);
_Bool always_wide = __atomic_always_lock_free(16, 0);
_Bool always_byte = __atomic_always_lock_free(1, &packed);
_Bool always_underaligned = __atomic_always_lock_free(4, &packed);
_Bool always_aligned = __atomic_always_lock_free(8, &aligned);
_Bool always_address = __atomic_always_lock_free(8, (long *)0x8);

int runtime(unsigned long size) {
    int a = __c11_atomic_is_lock_free(16);
    a += __atomic_is_lock_free(4, &packed);
    a += __atomic_is_lock_free(size, 0);
    a += __atomic_is_lock_free(8, &aligned);
    return a;
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
// IR-NEXT:     type @type[[TYPE_packed:[0-9]+]] packed = struct {
// IR-NEXT:         field0 bytes: array<i8, 3>;
// IR-NEXT:     } [size=3, align=1, offsets=[0]];
// IR-NEXT:     global %[[VALUE_packed:[0-9]+]] packed: @type[[TYPE_packed]] [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_aligned:[0-9]+]] aligned: i64 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_c11_word:[0-9]+]] c11_word: bool [storage=static] = const<bool>(true) [linkage=external];
// IR-NEXT:     global %[[VALUE_c11_odd:[0-9]+]] c11_odd: bool [storage=static] = const<bool>(false) [linkage=external];
// IR-NEXT:     global %[[VALUE_always_word:[0-9]+]] always_word: bool [storage=static] = const<bool>(true) [linkage=external];
// IR-NEXT:     global %[[VALUE_always_wide:[0-9]+]] always_wide: bool [storage=static] = const<bool>(false) [linkage=external];
// IR-NEXT:     global %[[VALUE_always_byte:[0-9]+]] always_byte: bool [storage=static] = const<bool>(true) [linkage=external];
// IR-NEXT:     global %[[VALUE_always_underaligned:[0-9]+]] always_underaligned: bool [storage=static] = const<bool>(false) [linkage=external];
// IR-NEXT:     global %[[VALUE_always_aligned:[0-9]+]] always_aligned: bool [storage=static] = const<bool>(true) [linkage=external];
// IR-NEXT:     global %[[VALUE_always_address:[0-9]+]] always_address: bool [storage=static] = const<bool>(true) [linkage=external];
// IR-NEXT:     fn %[[VALUE___atomic_is_lock_free:[0-9]+]] @__atomic_is_lock_free(%[[VALUE0:[0-9]+]] <unnamed>: u64, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const volatile void>) -> bool [linkage=external];
// IR-NEXT:     fn %[[VALUE_runtime:[0-9]+]] @runtime(%[[VALUE_size:[0-9]+]] size: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = from_bool<i32, reason=assign>(call<bool, signature=fn(u64, ptr<const volatile void>) -> bool>(%[[VALUE___atomic_is_lock_free]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))), null<ptr<const volatile void>>));
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), from_bool<i32, reason=promotion>(call<bool, signature=fn(u64, ptr<const volatile void>) -> bool>(%[[VALUE___atomic_is_lock_free]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), pointer_cast<ptr<const volatile void>, reason=arg>(addr_of<ptr<@type[[TYPE_packed]]>>(%[[VALUE_packed]])))));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE3]]));
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), from_bool<i32, reason=promotion>(call<bool, signature=fn(u64, ptr<const volatile void>) -> bool>(%[[VALUE___atomic_is_lock_free]], read<u64>(%[[VALUE_size]]), null<ptr<const volatile void>>)));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE5]]));
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), from_bool<i32, reason=promotion>(const<bool>(true)));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE7]]));
// IR-NEXT:         return read<i32>(%[[VALUE_a]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
