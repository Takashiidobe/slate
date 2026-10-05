// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu23

enum byte { BYTE_A } __attribute__((__mode__(__byte__)));
enum __attribute__((mode(HI))) leading { LEADING_A = 200 };
enum negative { NEGATIVE_A = -1 } __attribute__((mode(QI)));
enum word { WORD_A } __attribute__((__mode__(__word__)));
enum packed_mode { PACKED_A } __attribute__((packed, mode(SI)));
enum fixed : int { FIXED_A } __attribute__((mode(QI)));
typedef enum { TYPEDEF_A } __attribute__((mode(HI))) mode_t;

_Static_assert(sizeof(enum byte) == 1 && (enum byte)-1 > 0, "byte is unsigned");
_Static_assert(sizeof(enum leading) == 2 && (enum leading)-1 > 0, "leading");
_Static_assert(sizeof(enum negative) == 1 && (enum negative)-1 < 0, "negative");
_Static_assert(sizeof(enum word) == 8, "word");
_Static_assert(sizeof(enum packed_mode) == 4, "mode beats packed");
_Static_assert(sizeof(enum fixed) == 4, "fixed type ignores mode");
_Static_assert(sizeof(mode_t) == 2, "typedef");

enum leading next(enum leading value) {
  return value + 1;
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
// IR-NEXT:     type @type[[TYPE_byte:[0-9]+]] byte = enum : u8 {
// IR-NEXT:         %[[VALUE_BYTE_A:[0-9]+]] BYTE_A = const<i32>(0);
// IR-NEXT:     } [size=1, align=1];
// IR-NEXT:     type @type[[TYPE_leading:[0-9]+]] leading = enum : u16 {
// IR-NEXT:         %[[VALUE_BYTE_A]] LEADING_A = const<i32>(200);
// IR-NEXT:     } [size=2, align=2];
// IR-NEXT:     type @type[[TYPE_negative:[0-9]+]] negative = enum : i8 {
// IR-NEXT:         %[[VALUE_BYTE_A]] NEGATIVE_A = const<i32>(-1);
// IR-NEXT:     } [size=1, align=1];
// IR-NEXT:     type @type[[TYPE_word:[0-9]+]] word = enum : u64 {
// IR-NEXT:         %[[VALUE_BYTE_A]] WORD_A = const<i32>(0);
// IR-NEXT:     } [size=8, align=8];
// IR-NEXT:     type @type[[TYPE_packed_mode:[0-9]+]] packed_mode = enum : u32 {
// IR-NEXT:         %[[VALUE_BYTE_A]] PACKED_A = const<i32>(0);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type[[TYPE_fixed:[0-9]+]] fixed = enum : i32 {
// IR-NEXT:         %[[VALUE_BYTE_A]] FIXED_A = const<@type[[TYPE_fixed]]>(0);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u16 {
// IR-NEXT:         %[[VALUE_BYTE_A]] TYPEDEF_A = const<i32>(0);
// IR-NEXT:     } [size=2, align=2];
// IR-NEXT:     type @type[[TYPE_mode_t:[0-9]+]] mode_t = @type[[TYPE0]];
// IR-NEXT:     fn %[[VALUE_next:[0-9]+]] @next(%[[VALUE_value:[0-9]+]] value: @type[[TYPE_leading]]) -> @type[[TYPE_leading]] [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return int_to_enum<@type[[TYPE_leading]], reason=return>(reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(enum_to_int<u16, reason=promotion>(read<@type[[TYPE_leading]]>(%[[VALUE_value]])))), const<i32>(1)))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
