// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu11

enum cache_type { CACHE_DATA = 1, CACHE_INST, CACHE_UNIFIED } __attribute__((__packed__));
enum __attribute__((packed)) leading { LEADING_A = 200 };
enum signed_byte { SIGNED_LOW = -128, SIGNED_HIGH = 127 } __attribute__((packed));
enum signed_short { SHORT_LOW = -1, SHORT_HIGH = 200 } __attribute__((packed));
enum wide { WIDE_A = 0x10000 } __attribute__((packed));
typedef enum { TYPEDEF_A } __attribute__((packed)) packed_t;
enum late { LATE_A };
enum late __attribute__((packed));

_Static_assert(sizeof(enum cache_type) == 1, "trailing");
_Static_assert(sizeof(enum leading) == 1 && (enum leading)-1 > 0, "leading");
_Static_assert(sizeof(enum signed_byte) == 1 && (enum signed_byte)-1 < 0, "signed byte");
_Static_assert(sizeof(enum signed_short) == 2, "signed short");
_Static_assert(sizeof(enum wide) == 4, "wide");
_Static_assert(sizeof(packed_t) == 1, "typedef");
_Static_assert(sizeof(enum late) == 4, "late");
_Static_assert(sizeof(CACHE_DATA) == 4, "enumerators stay int");

struct descriptor {
  unsigned char level;
  enum cache_type type;
  enum signed_short code;
};

_Static_assert(sizeof(struct descriptor) == 4, "member layout");

enum cache_type cache_type_of(struct descriptor *d) {
  return d->type;
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
// IR-NEXT:     type @type[[TYPE_cache_type:[0-9]+]] cache_type = enum : u8 {
// IR-NEXT:         %[[VALUE_CACHE_DATA:[0-9]+]] CACHE_DATA = const<i32>(1);
// IR-NEXT:         %[[VALUE_CACHE_INST:[0-9]+]] CACHE_INST = const<i32>(2);
// IR-NEXT:         %[[VALUE_CACHE_UNIFIED:[0-9]+]] CACHE_UNIFIED = const<i32>(3);
// IR-NEXT:     } [size=1, align=1];
// IR-NEXT:     type @type[[TYPE_leading:[0-9]+]] leading = enum : u8 {
// IR-NEXT:         %[[VALUE_CACHE_DATA]] LEADING_A = const<i32>(200);
// IR-NEXT:     } [size=1, align=1];
// IR-NEXT:     type @type[[TYPE_signed_byte:[0-9]+]] signed_byte = enum : i8 {
// IR-NEXT:         %[[VALUE_CACHE_DATA]] SIGNED_LOW = const<i32>(-128);
// IR-NEXT:         %[[VALUE_CACHE_INST]] SIGNED_HIGH = const<i32>(127);
// IR-NEXT:     } [size=1, align=1];
// IR-NEXT:     type @type[[TYPE_signed_short:[0-9]+]] signed_short = enum : i16 {
// IR-NEXT:         %[[VALUE_CACHE_DATA]] SHORT_LOW = const<i32>(-1);
// IR-NEXT:         %[[VALUE_CACHE_INST]] SHORT_HIGH = const<i32>(200);
// IR-NEXT:     } [size=2, align=2];
// IR-NEXT:     type @type[[TYPE_wide:[0-9]+]] wide = enum : u32 {
// IR-NEXT:         %[[VALUE_CACHE_DATA]] WIDE_A = const<i32>(65536);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u8 {
// IR-NEXT:         %[[VALUE_CACHE_DATA]] TYPEDEF_A = const<i32>(0);
// IR-NEXT:     } [size=1, align=1];
// IR-NEXT:     type @type[[TYPE_packed_t:[0-9]+]] packed_t = @type[[TYPE0]];
// IR-NEXT:     type @type[[TYPE_late:[0-9]+]] late = enum : u32 {
// IR-NEXT:         %[[VALUE_CACHE_DATA]] LATE_A = const<i32>(0);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type[[TYPE_descriptor:[0-9]+]] descriptor = struct {
// IR-NEXT:         field0 level: u8;
// IR-NEXT:         field1 type: @type[[TYPE_cache_type]];
// IR-NEXT:         field2 code: @type[[TYPE_signed_short]];
// IR-NEXT:     } [size=4, align=2, offsets=[0, 1, 2]];
// IR-NEXT:     fn %[[VALUE_cache_type_of:[0-9]+]] @cache_type_of(%[[VALUE_d:[0-9]+]] d: ptr<@type[[TYPE_descriptor]]>) -> @type[[TYPE_cache_type]] [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<@type[[TYPE_cache_type]]>(field1(deref(read<ptr<@type[[TYPE_descriptor]]>>(%[[VALUE_d]]))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
