struct user_aligned { char c; int i __attribute__((aligned(8))); };
#pragma pack(4)
struct pragma_four { char c; double d; };
#pragma pack()
struct reset { char c; double d; };
struct bits { char c; int x : 4; int y : 20; };

int user_aligned_size = sizeof(struct user_aligned);
int user_aligned_align = _Alignof(struct user_aligned);
int pragma_four_size = sizeof(struct pragma_four);
int reset_size = sizeof(struct reset);
int reset_align = _Alignof(struct reset);
int bits_align = _Alignof(struct bits);

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES BARE
// SLATE-FILECHECK-PREFIX-ARGS BARE -fpack-struct
// SLATE-FILECHECK-DEFINES TWO
// SLATE-FILECHECK-PREFIX-ARGS TWO -fpack-struct=2
// SLATE-FILECHECK-DEFINES BOTH
// SLATE-FILECHECK-PREFIX-ARGS BOTH -fpack-struct -fpack-struct=2
// SLATE-FILECHECK-DEFINES NEGATED
// SLATE-FILECHECK-PREFIX-ARGS NEGATED -fpack-struct -fno-pack-struct

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
// DEFAULT-NEXT:     type @type[[TYPE_user_aligned:[0-9]+]] user_aligned = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 i: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_pragma_four:[0-9]+]] pragma_four = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_reset:[0-9]+]] reset = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_bits:[0-9]+]] bits = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 x: i32 : 4;
// DEFAULT-NEXT:         field2 y: i32 : 20;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 1, 1], bit_offsets=[None, Some(8), Some(12)], bit_units=[(1, 3)], field_units=[None, Some(0), Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_user_aligned_size:[0-9]+]] user_aligned_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(16))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_user_aligned_align:[0-9]+]] user_aligned_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(8))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pragma_four_size:[0-9]+]] pragma_four_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(12))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_reset_size:[0-9]+]] reset_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(16))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_reset_align:[0-9]+]] reset_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(8))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_bits_align:[0-9]+]] bits_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN BARE
// BARE: module {
// BARE-NEXT:     target "x86_64-unknown-linux-gnu" {
// BARE-NEXT:         endian = little;
// BARE-NEXT:         pointer [size=8, align=8];
// BARE-NEXT:         stack_alignment = 16;
// BARE-NEXT:         long_double = f80;
// BARE-NEXT:         storage bool [size=1, align=1];
// BARE-NEXT:         storage i8, u8 [size=1, align=1];
// BARE-NEXT:         storage i16, u16 [size=2, align=2];
// BARE-NEXT:         storage i32, u32 [size=4, align=4];
// BARE-NEXT:         storage i64, u64 [size=8, align=8];
// BARE-NEXT:         storage i128, u128 [size=16, align=16];
// BARE-NEXT:         storage bf16 [size=2, align=2];
// BARE-NEXT:         storage f16 [size=2, align=2];
// BARE-NEXT:         storage f32 [size=4, align=4];
// BARE-NEXT:         storage f64 [size=8, align=8];
// BARE-NEXT:         storage f80 [size=16, align=16];
// BARE-NEXT:         storage f128 [size=16, align=16];
// BARE-NEXT:         storage d32 [size=4, align=4];
// BARE-NEXT:         storage d64 [size=8, align=8];
// BARE-NEXT:         storage d128 [size=16, align=16];
// BARE-NEXT:     }
// BARE-NEXT:     type @type[[TYPE_user_aligned:[0-9]+]] user_aligned = struct {
// BARE-NEXT:         field0 c: i8;
// BARE-NEXT:         field1 i: i32;
// BARE-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// BARE-NEXT:     type @type[[TYPE_pragma_four:[0-9]+]] pragma_four = struct {
// BARE-NEXT:         field0 c: i8;
// BARE-NEXT:         field1 d: f64;
// BARE-NEXT:     } [size=9, align=1, offsets=[0, 1]];
// BARE-NEXT:     type @type[[TYPE_reset:[0-9]+]] reset = struct {
// BARE-NEXT:         field0 c: i8;
// BARE-NEXT:         field1 d: f64;
// BARE-NEXT:     } [size=9, align=1, offsets=[0, 1]];
// BARE-NEXT:     type @type[[TYPE_bits:[0-9]+]] bits = struct {
// BARE-NEXT:         field0 c: i8;
// BARE-NEXT:         field1 x: i32 : 4;
// BARE-NEXT:         field2 y: i32 : 20;
// BARE-NEXT:     } [size=4, align=1, offsets=[0, 1, 1], bit_offsets=[None, Some(8), Some(12)], bit_units=[(1, 3)], field_units=[None, Some(0), Some(0)]];
// BARE-NEXT:     global %[[VALUE_user_aligned_size:[0-9]+]] user_aligned_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(16))) [linkage=external];
// BARE-NEXT:     global %[[VALUE_user_aligned_align:[0-9]+]] user_aligned_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(8))) [linkage=external];
// BARE-NEXT:     global %[[VALUE_pragma_four_size:[0-9]+]] pragma_four_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(9))) [linkage=external];
// BARE-NEXT:     global %[[VALUE_reset_size:[0-9]+]] reset_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(9))) [linkage=external];
// BARE-NEXT:     global %[[VALUE_reset_align:[0-9]+]] reset_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(1))) [linkage=external];
// BARE-NEXT:     global %[[VALUE_bits_align:[0-9]+]] bits_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(1))) [linkage=external];
// BARE-NEXT: }
// SLATE-FILECHECK-END BARE
// SLATE-FILECHECK-BEGIN TWO
// TWO: module {
// TWO-NEXT:     target "x86_64-unknown-linux-gnu" {
// TWO-NEXT:         endian = little;
// TWO-NEXT:         pointer [size=8, align=8];
// TWO-NEXT:         stack_alignment = 16;
// TWO-NEXT:         long_double = f80;
// TWO-NEXT:         storage bool [size=1, align=1];
// TWO-NEXT:         storage i8, u8 [size=1, align=1];
// TWO-NEXT:         storage i16, u16 [size=2, align=2];
// TWO-NEXT:         storage i32, u32 [size=4, align=4];
// TWO-NEXT:         storage i64, u64 [size=8, align=8];
// TWO-NEXT:         storage i128, u128 [size=16, align=16];
// TWO-NEXT:         storage bf16 [size=2, align=2];
// TWO-NEXT:         storage f16 [size=2, align=2];
// TWO-NEXT:         storage f32 [size=4, align=4];
// TWO-NEXT:         storage f64 [size=8, align=8];
// TWO-NEXT:         storage f80 [size=16, align=16];
// TWO-NEXT:         storage f128 [size=16, align=16];
// TWO-NEXT:         storage d32 [size=4, align=4];
// TWO-NEXT:         storage d64 [size=8, align=8];
// TWO-NEXT:         storage d128 [size=16, align=16];
// TWO-NEXT:     }
// TWO-NEXT:     type @type[[TYPE_user_aligned:[0-9]+]] user_aligned = struct {
// TWO-NEXT:         field0 c: i8;
// TWO-NEXT:         field1 i: i32;
// TWO-NEXT:     } [size=6, align=2, offsets=[0, 2]];
// TWO-NEXT:     type @type[[TYPE_pragma_four:[0-9]+]] pragma_four = struct {
// TWO-NEXT:         field0 c: i8;
// TWO-NEXT:         field1 d: f64;
// TWO-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// TWO-NEXT:     type @type[[TYPE_reset:[0-9]+]] reset = struct {
// TWO-NEXT:         field0 c: i8;
// TWO-NEXT:         field1 d: f64;
// TWO-NEXT:     } [size=10, align=2, offsets=[0, 2]];
// TWO-NEXT:     type @type[[TYPE_bits:[0-9]+]] bits = struct {
// TWO-NEXT:         field0 c: i8;
// TWO-NEXT:         field1 x: i32 : 4;
// TWO-NEXT:         field2 y: i32 : 20;
// TWO-NEXT:     } [size=4, align=2, offsets=[0, 1, 1], bit_offsets=[None, Some(8), Some(12)], bit_units=[(1, 3)], field_units=[None, Some(0), Some(0)]];
// TWO-NEXT:     global %[[VALUE_user_aligned_size:[0-9]+]] user_aligned_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(6))) [linkage=external];
// TWO-NEXT:     global %[[VALUE_user_aligned_align:[0-9]+]] user_aligned_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(2))) [linkage=external];
// TWO-NEXT:     global %[[VALUE_pragma_four_size:[0-9]+]] pragma_four_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(12))) [linkage=external];
// TWO-NEXT:     global %[[VALUE_reset_size:[0-9]+]] reset_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(10))) [linkage=external];
// TWO-NEXT:     global %[[VALUE_reset_align:[0-9]+]] reset_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(2))) [linkage=external];
// TWO-NEXT:     global %[[VALUE_bits_align:[0-9]+]] bits_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(2))) [linkage=external];
// TWO-NEXT: }
// SLATE-FILECHECK-END TWO
// SLATE-FILECHECK-BEGIN BOTH
// BOTH: module {
// BOTH-NEXT:     target "x86_64-unknown-linux-gnu" {
// BOTH-NEXT:         endian = little;
// BOTH-NEXT:         pointer [size=8, align=8];
// BOTH-NEXT:         stack_alignment = 16;
// BOTH-NEXT:         long_double = f80;
// BOTH-NEXT:         storage bool [size=1, align=1];
// BOTH-NEXT:         storage i8, u8 [size=1, align=1];
// BOTH-NEXT:         storage i16, u16 [size=2, align=2];
// BOTH-NEXT:         storage i32, u32 [size=4, align=4];
// BOTH-NEXT:         storage i64, u64 [size=8, align=8];
// BOTH-NEXT:         storage i128, u128 [size=16, align=16];
// BOTH-NEXT:         storage bf16 [size=2, align=2];
// BOTH-NEXT:         storage f16 [size=2, align=2];
// BOTH-NEXT:         storage f32 [size=4, align=4];
// BOTH-NEXT:         storage f64 [size=8, align=8];
// BOTH-NEXT:         storage f80 [size=16, align=16];
// BOTH-NEXT:         storage f128 [size=16, align=16];
// BOTH-NEXT:         storage d32 [size=4, align=4];
// BOTH-NEXT:         storage d64 [size=8, align=8];
// BOTH-NEXT:         storage d128 [size=16, align=16];
// BOTH-NEXT:     }
// BOTH-NEXT:     type @type[[TYPE_user_aligned:[0-9]+]] user_aligned = struct {
// BOTH-NEXT:         field0 c: i8;
// BOTH-NEXT:         field1 i: i32;
// BOTH-NEXT:     } [size=6, align=2, offsets=[0, 2]];
// BOTH-NEXT:     type @type[[TYPE_pragma_four:[0-9]+]] pragma_four = struct {
// BOTH-NEXT:         field0 c: i8;
// BOTH-NEXT:         field1 d: f64;
// BOTH-NEXT:     } [size=9, align=1, offsets=[0, 1]];
// BOTH-NEXT:     type @type[[TYPE_reset:[0-9]+]] reset = struct {
// BOTH-NEXT:         field0 c: i8;
// BOTH-NEXT:         field1 d: f64;
// BOTH-NEXT:     } [size=9, align=1, offsets=[0, 1]];
// BOTH-NEXT:     type @type[[TYPE_bits:[0-9]+]] bits = struct {
// BOTH-NEXT:         field0 c: i8;
// BOTH-NEXT:         field1 x: i32 : 4;
// BOTH-NEXT:         field2 y: i32 : 20;
// BOTH-NEXT:     } [size=4, align=2, offsets=[0, 1, 1], bit_offsets=[None, Some(8), Some(12)], bit_units=[(1, 3)], field_units=[None, Some(0), Some(0)]];
// BOTH-NEXT:     global %[[VALUE_user_aligned_size:[0-9]+]] user_aligned_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(6))) [linkage=external];
// BOTH-NEXT:     global %[[VALUE_user_aligned_align:[0-9]+]] user_aligned_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(2))) [linkage=external];
// BOTH-NEXT:     global %[[VALUE_pragma_four_size:[0-9]+]] pragma_four_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(9))) [linkage=external];
// BOTH-NEXT:     global %[[VALUE_reset_size:[0-9]+]] reset_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(9))) [linkage=external];
// BOTH-NEXT:     global %[[VALUE_reset_align:[0-9]+]] reset_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(1))) [linkage=external];
// BOTH-NEXT:     global %[[VALUE_bits_align:[0-9]+]] bits_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(2))) [linkage=external];
// BOTH-NEXT: }
// SLATE-FILECHECK-END BOTH
// SLATE-FILECHECK-BEGIN NEGATED
// NEGATED: module {
// NEGATED-NEXT:     target "x86_64-unknown-linux-gnu" {
// NEGATED-NEXT:         endian = little;
// NEGATED-NEXT:         pointer [size=8, align=8];
// NEGATED-NEXT:         stack_alignment = 16;
// NEGATED-NEXT:         long_double = f80;
// NEGATED-NEXT:         storage bool [size=1, align=1];
// NEGATED-NEXT:         storage i8, u8 [size=1, align=1];
// NEGATED-NEXT:         storage i16, u16 [size=2, align=2];
// NEGATED-NEXT:         storage i32, u32 [size=4, align=4];
// NEGATED-NEXT:         storage i64, u64 [size=8, align=8];
// NEGATED-NEXT:         storage i128, u128 [size=16, align=16];
// NEGATED-NEXT:         storage bf16 [size=2, align=2];
// NEGATED-NEXT:         storage f16 [size=2, align=2];
// NEGATED-NEXT:         storage f32 [size=4, align=4];
// NEGATED-NEXT:         storage f64 [size=8, align=8];
// NEGATED-NEXT:         storage f80 [size=16, align=16];
// NEGATED-NEXT:         storage f128 [size=16, align=16];
// NEGATED-NEXT:         storage d32 [size=4, align=4];
// NEGATED-NEXT:         storage d64 [size=8, align=8];
// NEGATED-NEXT:         storage d128 [size=16, align=16];
// NEGATED-NEXT:     }
// NEGATED-NEXT:     type @type[[TYPE_user_aligned:[0-9]+]] user_aligned = struct {
// NEGATED-NEXT:         field0 c: i8;
// NEGATED-NEXT:         field1 i: i32;
// NEGATED-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// NEGATED-NEXT:     type @type[[TYPE_pragma_four:[0-9]+]] pragma_four = struct {
// NEGATED-NEXT:         field0 c: i8;
// NEGATED-NEXT:         field1 d: f64;
// NEGATED-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// NEGATED-NEXT:     type @type[[TYPE_reset:[0-9]+]] reset = struct {
// NEGATED-NEXT:         field0 c: i8;
// NEGATED-NEXT:         field1 d: f64;
// NEGATED-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// NEGATED-NEXT:     type @type[[TYPE_bits:[0-9]+]] bits = struct {
// NEGATED-NEXT:         field0 c: i8;
// NEGATED-NEXT:         field1 x: i32 : 4;
// NEGATED-NEXT:         field2 y: i32 : 20;
// NEGATED-NEXT:     } [size=4, align=4, offsets=[0, 1, 1], bit_offsets=[None, Some(8), Some(12)], bit_units=[(1, 3)], field_units=[None, Some(0), Some(0)]];
// NEGATED-NEXT:     global %[[VALUE_user_aligned_size:[0-9]+]] user_aligned_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(16))) [linkage=external];
// NEGATED-NEXT:     global %[[VALUE_user_aligned_align:[0-9]+]] user_aligned_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(8))) [linkage=external];
// NEGATED-NEXT:     global %[[VALUE_pragma_four_size:[0-9]+]] pragma_four_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(12))) [linkage=external];
// NEGATED-NEXT:     global %[[VALUE_reset_size:[0-9]+]] reset_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(16))) [linkage=external];
// NEGATED-NEXT:     global %[[VALUE_reset_align:[0-9]+]] reset_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(8))) [linkage=external];
// NEGATED-NEXT:     global %[[VALUE_bits_align:[0-9]+]] bits_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// NEGATED-NEXT: }
// SLATE-FILECHECK-END NEGATED
