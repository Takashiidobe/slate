// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

int next(void);

char *up_pointer(char *p) { return __builtin_align_up(p++, 16); }

const int *down_pointer(const int *p) { return __builtin_align_down(p, 8); }

unsigned char up_integer(unsigned char value) {
  return __builtin_align_up(value, next());
}

long down_integer(long value) { return __builtin_align_down(value, 4); }

_Bool aligned_pointer(void *p) { return __builtin_is_aligned(p, 32); }

int aligned_integer(int value) { return __builtin_is_aligned(value, 2) + sizeof(__builtin_is_aligned(value, 2)); }

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
// IR-NEXT:     fn %[[VALUE_next:[0-9]+]] @next() -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_up_pointer:[0-9]+]] @up_pointer(%[[VALUE_p:[0-9]+]] p: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: u64 [synthetic] = sub<u64>(reinterpret<u64>(widen<i64>(const<i32>(16))), const<u64>(1));
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p]]);
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false>(read<ptr<i8>>(%[[VALUE1]]), const<i32>(1));
// IR-NEXT:         write<ptr<i8>>(%[[VALUE_p]], read<ptr<i8>>(%[[VALUE2]]));
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE1]]);
// IR-NEXT:         return pointer_cast<ptr<i8>>(ptr_offset<ptr<u8>, subtract=false>(pointer_cast<ptr<u8>>(read<ptr<i8>>(%[[VALUE3]])), sub<u64>(and<u64>(add<u64>(ptr_to_int<u64>(read<ptr<i8>>(%[[VALUE3]])), read<u64>(%[[VALUE0]])), not<u64>(read<u64>(%[[VALUE0]]))), ptr_to_int<u64>(read<ptr<i8>>(%[[VALUE3]])))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_down_pointer:[0-9]+]] @down_pointer(%[[VALUE_p_2:[0-9]+]] p: ptr<const i32>) -> ptr<const i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: u64 [synthetic] = sub<u64>(reinterpret<u64>(widen<i64>(const<i32>(8))), const<u64>(1));
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: ptr<const i32> [synthetic] = read<ptr<const i32>>(%[[VALUE_p_2]]);
// IR-NEXT:         return pointer_cast<ptr<const i32>>(ptr_offset<ptr<const u8>, subtract=true>(pointer_cast<ptr<const u8>>(read<ptr<const i32>>(%[[VALUE5]])), and<u64>(ptr_to_int<u64>(read<ptr<const i32>>(%[[VALUE5]])), read<u64>(%[[VALUE4]]))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_up_integer:[0-9]+]] @up_integer(%[[VALUE_value:[0-9]+]] value: u8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: u8 [synthetic] = sub<u8>(reinterpret<u8>(truncate<i8>(call<i32>(%[[VALUE_next]]))), const<u8>(1));
// IR-NEXT:         return and<u8>(add<u8>(read<u8>(%[[VALUE_value]]), read<u8>(%[[VALUE6]])), not<u8>(read<u8>(%[[VALUE6]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_down_integer:[0-9]+]] @down_integer(%[[VALUE_value_2:[0-9]+]] value: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: i64 [synthetic] = sub<i64>(widen<i64>(const<i32>(4)), const<i64>(1));
// IR-NEXT:         return and<i64>(read<i64>(%[[VALUE_value_2]]), not<i64>(read<i64>(%[[VALUE7]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_aligned_pointer:[0-9]+]] @aligned_pointer(%[[VALUE_p_3:[0-9]+]] p: ptr<void>) -> bool [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: u64 [synthetic] = sub<u64>(reinterpret<u64>(widen<i64>(const<i32>(32))), const<u64>(1));
// IR-NEXT:         let %[[VALUE9:[0-9]+]]: ptr<void> [synthetic] = read<ptr<void>>(%[[VALUE_p_3]]);
// IR-NEXT:         return eq<u64>(and<u64>(ptr_to_int<u64>(read<ptr<void>>(%[[VALUE9]])), read<u64>(%[[VALUE8]])), const<u64>(0));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_aligned_integer:[0-9]+]] @aligned_integer(%[[VALUE_value_3:[0-9]+]] value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = sub<i32>(const<i32>(2), const<i32>(1));
// IR-NEXT:         return reinterpret<i32>(truncate<u32>(add<u64>(reinterpret<u64>(widen<i64>(from_bool<i32>(eq<i32>(and<i32>(read<i32>(%[[VALUE_value_3]]), read<i32>(%[[VALUE10]])), const<i32>(0))))), const<u64>(1))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
