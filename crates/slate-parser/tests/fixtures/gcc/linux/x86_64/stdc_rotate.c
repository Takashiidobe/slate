typedef unsigned char byte;
unsigned char left8(byte value, signed char count) {
    return __builtin_stdc_rotate_left(value, count);
}
unsigned short right16(unsigned short value, unsigned long long count) {
    return __builtin_stdc_rotate_right(value, count);
}
unsigned int left32(unsigned int value, int count) {
    return __builtin_stdc_rotate_left(value, count);
}
unsigned long long right64(unsigned long long value, unsigned short count) {
    return __builtin_stdc_rotate_right(value, count);
}
unsigned _BitInt(5) left5(unsigned _BitInt(5) value, unsigned _BitInt(9) count) {
    return __builtin_stdc_rotate_left(value, count);
}
unsigned _BitInt(129) right129(unsigned _BitInt(129) value, unsigned __int128 count) {
    return __builtin_stdc_rotate_right(value, count);
}
unsigned int effects(unsigned int *value, int *count) {
    return __builtin_stdc_rotate_left((*value)++, (*count)++);
}
_Static_assert(_Generic(__builtin_stdc_rotate_left((byte)1, 1), byte: 1, default: 0), "no promotion");
_Static_assert(__builtin_stdc_rotate_left((byte)129, 1) == 3, "left");
_Static_assert(__builtin_stdc_rotate_right((byte)3, 1) == 129, "right");
_Static_assert(__builtin_stdc_rotate_left((unsigned short)0x1234, 0) == 0x1234, "zero");
_Static_assert(__builtin_stdc_rotate_right((unsigned short)0x1234, 32) == 0x1234, "multiple");
_Static_assert(__builtin_stdc_rotate_left(42u, 33) == 84, "large count");

_Static_assert(__builtin_stdc_rotate_right((unsigned _BitInt(129))3, 1) == (((unsigned _BitInt(129))1 << 128) | 1), "wide");
_Static_assert(__builtin_stdc_rotate_left((unsigned _BitInt(1))1, 1000) == 1, "one bit");
int bound[__builtin_stdc_rotate_left((byte)129, 1)];

// SLATE-FILECHECK-STD C11 c11
// SLATE-FILECHECK-DEFINES C11
// SLATE-FILECHECK-STD C2Y c2y
// SLATE-FILECHECK-DEFINES C2Y

// SLATE-FILECHECK-BEGIN C11
// C11: module {
// C11-NEXT:     target "x86_64-unknown-linux-gnu" {
// C11-NEXT:         endian = little;
// C11-NEXT:         pointer [size=8, align=8];
// C11-NEXT:         stack_alignment = 16;
// C11-NEXT:         long_double = f80;
// C11-NEXT:         storage bool [size=1, align=1];
// C11-NEXT:         storage i8, u8 [size=1, align=1];
// C11-NEXT:         storage i16, u16 [size=2, align=2];
// C11-NEXT:         storage i32, u32 [size=4, align=4];
// C11-NEXT:         storage i64, u64 [size=8, align=8];
// C11-NEXT:         storage i128, u128 [size=16, align=16];
// C11-NEXT:         storage bf16 [size=2, align=2];
// C11-NEXT:         storage f16 [size=2, align=2];
// C11-NEXT:         storage f32 [size=4, align=4];
// C11-NEXT:         storage f64 [size=8, align=8];
// C11-NEXT:         storage f80 [size=16, align=16];
// C11-NEXT:         storage f128 [size=16, align=16];
// C11-NEXT:         storage d32 [size=4, align=4];
// C11-NEXT:         storage d64 [size=8, align=8];
// C11-NEXT:         storage d128 [size=16, align=16];
// C11-NEXT:     }
// C11-NEXT:     type @type[[TYPE_byte:[0-9]+]] byte = u8;
// C11-NEXT:     global %[[VALUE_bound:[0-9]+]] bound: array<i32, 3> [storage=static] [linkage=external];
// C11-NEXT:     fn %[[VALUE___builtin_stdc_rotate_left:[0-9]+]] @__builtin_stdc_rotate_left(%[[VALUE0:[0-9]+]] <unnamed>: u8, %[[VALUE1:[0-9]+]] <unnamed>: i8) -> u8 [linkage=external] [memory=none];
// C11-NEXT:     fn %[[VALUE_left8:[0-9]+]] @left8(%[[VALUE_value:[0-9]+]] value: u8, %[[VALUE_count:[0-9]+]] count: i8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// C11-NEXT:         return call<u8, signature=fn(u8, i8) -> u8>(%[[VALUE___builtin_stdc_rotate_left]], read<u8>(%[[VALUE_value]]), read<i8>(%[[VALUE_count]]));
// C11-NEXT:     }
// C11-NEXT:     fn %[[VALUE___builtin_stdc_rotate_right:[0-9]+]] @__builtin_stdc_rotate_right(%[[VALUE2:[0-9]+]] <unnamed>: u16, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> u16 [linkage=external] [memory=none];
// C11-NEXT:     fn %[[VALUE_right16:[0-9]+]] @right16(%[[VALUE_value_2:[0-9]+]] value: u16, %[[VALUE_count_2:[0-9]+]] count: u64) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// C11-NEXT:         return call<u16, signature=fn(u16, u64) -> u16>(%[[VALUE___builtin_stdc_rotate_right]], read<u16>(%[[VALUE_value_2]]), read<u64>(%[[VALUE_count_2]]));
// C11-NEXT:     }
// C11-NEXT:     fn %[[VALUE___builtin_stdc_rotate_left_2:[0-9]+]] @__builtin_stdc_rotate_left(%[[VALUE4:[0-9]+]] <unnamed>: u32, %[[VALUE5:[0-9]+]] <unnamed>: i32) -> u32 [linkage=external] [memory=none];
// C11-NEXT:     fn %[[VALUE_left32:[0-9]+]] @left32(%[[VALUE_value_3:[0-9]+]] value: u32, %[[VALUE_count_3:[0-9]+]] count: i32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// C11-NEXT:         return call<u32, signature=fn(u32, i32) -> u32>(%[[VALUE___builtin_stdc_rotate_left_2]], read<u32>(%[[VALUE_value_3]]), read<i32>(%[[VALUE_count_3]]));
// C11-NEXT:     }
// C11-NEXT:     fn %[[VALUE___builtin_stdc_rotate_right_2:[0-9]+]] @__builtin_stdc_rotate_right(%[[VALUE6:[0-9]+]] <unnamed>: u64, %[[VALUE7:[0-9]+]] <unnamed>: u16) -> u64 [linkage=external] [memory=none];
// C11-NEXT:     fn %[[VALUE_right64:[0-9]+]] @right64(%[[VALUE_value_4:[0-9]+]] value: u64, %[[VALUE_count_4:[0-9]+]] count: u16) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// C11-NEXT:         return call<u64, signature=fn(u64, u16) -> u64>(%[[VALUE___builtin_stdc_rotate_right_2]], read<u64>(%[[VALUE_value_4]]), read<u16>(%[[VALUE_count_4]]));
// C11-NEXT:     }
// C11-NEXT:     fn %[[VALUE___builtin_stdc_rotate_left_3:[0-9]+]] @__builtin_stdc_rotate_left(%[[VALUE8:[0-9]+]] <unnamed>: u5b, %[[VALUE9:[0-9]+]] <unnamed>: u9b) -> u5b [linkage=external] [memory=none];
// C11-NEXT:     fn %[[VALUE_left5:[0-9]+]] @left5(%[[VALUE_value_5:[0-9]+]] value: u5b, %[[VALUE_count_5:[0-9]+]] count: u9b) -> u5b [linkage=external] [fallthrough=ub_if_used] {
// C11-NEXT:         return call<u5b, signature=fn(u5b, u9b) -> u5b>(%[[VALUE___builtin_stdc_rotate_left_3]], read<u5b>(%[[VALUE_value_5]]), read<u9b>(%[[VALUE_count_5]]));
// C11-NEXT:     }
// C11-NEXT:     fn %[[VALUE___builtin_stdc_rotate_right_3:[0-9]+]] @__builtin_stdc_rotate_right(%[[VALUE10:[0-9]+]] <unnamed>: u129b, %[[VALUE11:[0-9]+]] <unnamed>: u128) -> u129b [linkage=external] [memory=none];
// C11-NEXT:     fn %[[VALUE_right129:[0-9]+]] @right129(%[[VALUE_value_6:[0-9]+]] value: u129b, %[[VALUE_count_6:[0-9]+]] count: u128) -> u129b [linkage=external] [fallthrough=ub_if_used] {
// C11-NEXT:         return call<u129b, signature=fn(u129b, u128) -> u129b>(%[[VALUE___builtin_stdc_rotate_right_3]], read<u129b>(%[[VALUE_value_6]]), read<u128>(%[[VALUE_count_6]]));
// C11-NEXT:     }
// C11-NEXT:     fn %[[VALUE_effects:[0-9]+]] @effects(%[[VALUE_value_7:[0-9]+]] value: ptr<u32>, %[[VALUE_count_7:[0-9]+]] count: ptr<i32>) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// C11-NEXT:         let %[[VALUE12:[0-9]+]]: ptr<u32> [synthetic] = read<ptr<u32>>(%[[VALUE_value_7]]);
// C11-NEXT:         let %[[VALUE13:[0-9]+]]: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%[[VALUE12]])));
// C11-NEXT:         let %[[VALUE14:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE13]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// C11-NEXT:         write<u32>(deref(read<ptr<u32>>(%[[VALUE12]])), read<u32>(%[[VALUE14]]));
// C11-NEXT:         let %[[VALUE15:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_count_7]]);
// C11-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE15]])));
// C11-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// C11-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE15]])), read<i32>(%[[VALUE17]]));
// C11-NEXT:         return call<u32, signature=fn(u32, i32) -> u32>(%[[VALUE___builtin_stdc_rotate_left_2]], read<u32>(%[[VALUE13]]), read<i32>(%[[VALUE16]]));
// C11-NEXT:     }
// C11-NEXT: }
// SLATE-FILECHECK-END C11
// SLATE-FILECHECK-BEGIN C2Y
// C2Y: module {
// C2Y-NEXT:     target "x86_64-unknown-linux-gnu" {
// C2Y-NEXT:         endian = little;
// C2Y-NEXT:         pointer [size=8, align=8];
// C2Y-NEXT:         stack_alignment = 16;
// C2Y-NEXT:         long_double = f80;
// C2Y-NEXT:         storage bool [size=1, align=1];
// C2Y-NEXT:         storage i8, u8 [size=1, align=1];
// C2Y-NEXT:         storage i16, u16 [size=2, align=2];
// C2Y-NEXT:         storage i32, u32 [size=4, align=4];
// C2Y-NEXT:         storage i64, u64 [size=8, align=8];
// C2Y-NEXT:         storage i128, u128 [size=16, align=16];
// C2Y-NEXT:         storage bf16 [size=2, align=2];
// C2Y-NEXT:         storage f16 [size=2, align=2];
// C2Y-NEXT:         storage f32 [size=4, align=4];
// C2Y-NEXT:         storage f64 [size=8, align=8];
// C2Y-NEXT:         storage f80 [size=16, align=16];
// C2Y-NEXT:         storage f128 [size=16, align=16];
// C2Y-NEXT:         storage d32 [size=4, align=4];
// C2Y-NEXT:         storage d64 [size=8, align=8];
// C2Y-NEXT:         storage d128 [size=16, align=16];
// C2Y-NEXT:     }
// C2Y-NEXT:     type @type[[TYPE_byte:[0-9]+]] byte = u8;
// C2Y-NEXT:     global %[[VALUE_bound:[0-9]+]] bound: array<i32, 3> [storage=static] [linkage=external];
// C2Y-NEXT:     fn %[[VALUE___builtin_stdc_rotate_left:[0-9]+]] @__builtin_stdc_rotate_left(%[[VALUE0:[0-9]+]] <unnamed>: u8, %[[VALUE1:[0-9]+]] <unnamed>: i8) -> u8 [linkage=external] [memory=none];
// C2Y-NEXT:     fn %[[VALUE_left8:[0-9]+]] @left8(%[[VALUE_value:[0-9]+]] value: u8, %[[VALUE_count:[0-9]+]] count: i8) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// C2Y-NEXT:         return call<u8, signature=fn(u8, i8) -> u8>(%[[VALUE___builtin_stdc_rotate_left]], read<u8>(%[[VALUE_value]]), read<i8>(%[[VALUE_count]]));
// C2Y-NEXT:     }
// C2Y-NEXT:     fn %[[VALUE___builtin_stdc_rotate_right:[0-9]+]] @__builtin_stdc_rotate_right(%[[VALUE2:[0-9]+]] <unnamed>: u16, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> u16 [linkage=external] [memory=none];
// C2Y-NEXT:     fn %[[VALUE_right16:[0-9]+]] @right16(%[[VALUE_value_2:[0-9]+]] value: u16, %[[VALUE_count_2:[0-9]+]] count: u64) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// C2Y-NEXT:         return call<u16, signature=fn(u16, u64) -> u16>(%[[VALUE___builtin_stdc_rotate_right]], read<u16>(%[[VALUE_value_2]]), read<u64>(%[[VALUE_count_2]]));
// C2Y-NEXT:     }
// C2Y-NEXT:     fn %[[VALUE___builtin_stdc_rotate_left_2:[0-9]+]] @__builtin_stdc_rotate_left(%[[VALUE4:[0-9]+]] <unnamed>: u32, %[[VALUE5:[0-9]+]] <unnamed>: i32) -> u32 [linkage=external] [memory=none];
// C2Y-NEXT:     fn %[[VALUE_left32:[0-9]+]] @left32(%[[VALUE_value_3:[0-9]+]] value: u32, %[[VALUE_count_3:[0-9]+]] count: i32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// C2Y-NEXT:         return call<u32, signature=fn(u32, i32) -> u32>(%[[VALUE___builtin_stdc_rotate_left_2]], read<u32>(%[[VALUE_value_3]]), read<i32>(%[[VALUE_count_3]]));
// C2Y-NEXT:     }
// C2Y-NEXT:     fn %[[VALUE___builtin_stdc_rotate_right_2:[0-9]+]] @__builtin_stdc_rotate_right(%[[VALUE6:[0-9]+]] <unnamed>: u64, %[[VALUE7:[0-9]+]] <unnamed>: u16) -> u64 [linkage=external] [memory=none];
// C2Y-NEXT:     fn %[[VALUE_right64:[0-9]+]] @right64(%[[VALUE_value_4:[0-9]+]] value: u64, %[[VALUE_count_4:[0-9]+]] count: u16) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// C2Y-NEXT:         return call<u64, signature=fn(u64, u16) -> u64>(%[[VALUE___builtin_stdc_rotate_right_2]], read<u64>(%[[VALUE_value_4]]), read<u16>(%[[VALUE_count_4]]));
// C2Y-NEXT:     }
// C2Y-NEXT:     fn %[[VALUE___builtin_stdc_rotate_left_3:[0-9]+]] @__builtin_stdc_rotate_left(%[[VALUE8:[0-9]+]] <unnamed>: u5b, %[[VALUE9:[0-9]+]] <unnamed>: u9b) -> u5b [linkage=external] [memory=none];
// C2Y-NEXT:     fn %[[VALUE_left5:[0-9]+]] @left5(%[[VALUE_value_5:[0-9]+]] value: u5b, %[[VALUE_count_5:[0-9]+]] count: u9b) -> u5b [linkage=external] [fallthrough=ub_if_used] {
// C2Y-NEXT:         return call<u5b, signature=fn(u5b, u9b) -> u5b>(%[[VALUE___builtin_stdc_rotate_left_3]], read<u5b>(%[[VALUE_value_5]]), read<u9b>(%[[VALUE_count_5]]));
// C2Y-NEXT:     }
// C2Y-NEXT:     fn %[[VALUE___builtin_stdc_rotate_right_3:[0-9]+]] @__builtin_stdc_rotate_right(%[[VALUE10:[0-9]+]] <unnamed>: u129b, %[[VALUE11:[0-9]+]] <unnamed>: u128) -> u129b [linkage=external] [memory=none];
// C2Y-NEXT:     fn %[[VALUE_right129:[0-9]+]] @right129(%[[VALUE_value_6:[0-9]+]] value: u129b, %[[VALUE_count_6:[0-9]+]] count: u128) -> u129b [linkage=external] [fallthrough=ub_if_used] {
// C2Y-NEXT:         return call<u129b, signature=fn(u129b, u128) -> u129b>(%[[VALUE___builtin_stdc_rotate_right_3]], read<u129b>(%[[VALUE_value_6]]), read<u128>(%[[VALUE_count_6]]));
// C2Y-NEXT:     }
// C2Y-NEXT:     fn %[[VALUE_effects:[0-9]+]] @effects(%[[VALUE_value_7:[0-9]+]] value: ptr<u32>, %[[VALUE_count_7:[0-9]+]] count: ptr<i32>) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// C2Y-NEXT:         let %[[VALUE12:[0-9]+]]: ptr<u32> [synthetic] = read<ptr<u32>>(%[[VALUE_value_7]]);
// C2Y-NEXT:         let %[[VALUE13:[0-9]+]]: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%[[VALUE12]])));
// C2Y-NEXT:         let %[[VALUE14:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE13]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// C2Y-NEXT:         write<u32>(deref(read<ptr<u32>>(%[[VALUE12]])), read<u32>(%[[VALUE14]]));
// C2Y-NEXT:         let %[[VALUE15:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_count_7]]);
// C2Y-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE15]])));
// C2Y-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// C2Y-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE15]])), read<i32>(%[[VALUE17]]));
// C2Y-NEXT:         return call<u32, signature=fn(u32, i32) -> u32>(%[[VALUE___builtin_stdc_rotate_left_2]], read<u32>(%[[VALUE13]]), read<i32>(%[[VALUE16]]));
// C2Y-NEXT:     }
// C2Y-NEXT: }
// SLATE-FILECHECK-END C2Y
