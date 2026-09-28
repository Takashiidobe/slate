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
// IR-NEXT:     type @type0 packed = struct {
// IR-NEXT:         field0 bytes: array<i8, 3>;
// IR-NEXT:     } [size=3, align=1, offsets=[0]];
// IR-NEXT:     global %1 packed: @type0 [storage=static] [linkage=external];
// IR-NEXT:     global %2 aligned: i64 [storage=static] [linkage=external];
// IR-NEXT:     global %3 c11_word: bool [storage=static] = const<bool>(true) [linkage=external];
// IR-NEXT:     global %4 c11_odd: bool [storage=static] = const<bool>(false) [linkage=external];
// IR-NEXT:     global %5 always_word: bool [storage=static] = const<bool>(true) [linkage=external];
// IR-NEXT:     global %6 always_wide: bool [storage=static] = const<bool>(false) [linkage=external];
// IR-NEXT:     global %7 always_byte: bool [storage=static] = const<bool>(true) [linkage=external];
// IR-NEXT:     global %8 always_underaligned: bool [storage=static] = const<bool>(false) [linkage=external];
// IR-NEXT:     global %9 always_aligned: bool [storage=static] = const<bool>(true) [linkage=external];
// IR-NEXT:     global %10 always_address: bool [storage=static] = const<bool>(true) [linkage=external];
// IR-NEXT:     fn %16 @__atomic_is_lock_free(%14 <unnamed>: u64, %15 <unnamed>: ptr<const volatile void>) -> bool [linkage=external];
// IR-NEXT:     fn %11 @runtime(%12 size: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %13 a: i32 [storage=automatic] = from_bool<i32, reason=assign>(call<bool, signature=fn(u64, ptr<const volatile void>) -> bool>(%16, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))), null<ptr<const volatile void>>));
// IR-NEXT:         let %17: i32 [synthetic] = read<i32>(%13);
// IR-NEXT:         let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), from_bool<i32, reason=promotion>(call<bool, signature=fn(u64, ptr<const volatile void>) -> bool>(%16, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), pointer_cast<ptr<const volatile void>, reason=arg>(addr_of<ptr<@type0>>(%1)))));
// IR-NEXT:         write<i32>(%13, read<i32>(%18));
// IR-NEXT:         let %19: i32 [synthetic] = read<i32>(%13);
// IR-NEXT:         let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), from_bool<i32, reason=promotion>(call<bool, signature=fn(u64, ptr<const volatile void>) -> bool>(%16, read<u64>(%12), null<ptr<const volatile void>>)));
// IR-NEXT:         write<i32>(%13, read<i32>(%20));
// IR-NEXT:         let %21: i32 [synthetic] = read<i32>(%13);
// IR-NEXT:         let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), from_bool<i32, reason=promotion>(const<bool>(true)));
// IR-NEXT:         write<i32>(%13, read<i32>(%22));
// IR-NEXT:         return read<i32>(%13);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
