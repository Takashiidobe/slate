#if STYLE == 1
typedef
struct
Payload
{
  int
  value
  ;
  int
  (*callback)
  (int)
  ;
}
Payload
;
enum
Mode
:
unsigned
int
{
  FIRST = 1,
  LAST = FIRST + 2
}
;
int
read_value
(
  Payload *p
)
{
  if (p)
  {
    return p->callback(p->value);
  }
  return LAST;
}
int
after
;
#elif STYLE == 2
#define RECORD(name, fields) typedef struct name { fields } name;
#define ENUM(name, ...) enum name : unsigned int { __VA_ARGS__ };
#define FUNCTION(name, args, body) int name args body
RECORD(Payload, int value; int (*callback)(int);)
ENUM(Mode, FIRST = 1, LAST = FIRST + 2)
FUNCTION(read_value, (Payload *p), { if (p) { return p->callback(p->value); } return LAST; })
int after;
#else
typedef struct Payload { int value; int (*callback)(int); } Payload; enum Mode : unsigned int { FIRST = 1, LAST = FIRST + 2 }; int read_value(Payload *p) { if (p) { return p->callback(p->value); } return LAST; } int after;
#endif

// SLATE-FILECHECK-DEFINES COMPACT STYLE=0
// SLATE-FILECHECK-DEFINES SPLIT STYLE=1
// SLATE-FILECHECK-DEFINES MACRO STYLE=2

// SLATE-FILECHECK-BEGIN COMPACT
// COMPACT: module {
// COMPACT-NEXT:     target "x86_64-unknown-linux-gnu" {
// COMPACT-NEXT:         endian = little;
// COMPACT-NEXT:         pointer [size=8, align=8];
// COMPACT-NEXT:         stack_alignment = 16;
// COMPACT-NEXT:         long_double = f80;
// COMPACT-NEXT:         storage bool [size=1, align=1];
// COMPACT-NEXT:         storage i8, u8 [size=1, align=1];
// COMPACT-NEXT:         storage i16, u16 [size=2, align=2];
// COMPACT-NEXT:         storage i32, u32 [size=4, align=4];
// COMPACT-NEXT:         storage i64, u64 [size=8, align=8];
// COMPACT-NEXT:         storage i128, u128 [size=16, align=16];
// COMPACT-NEXT:         storage bf16 [size=2, align=2];
// COMPACT-NEXT:         storage f16 [size=2, align=2];
// COMPACT-NEXT:         storage f32 [size=4, align=4];
// COMPACT-NEXT:         storage f64 [size=8, align=8];
// COMPACT-NEXT:         storage f80 [size=16, align=16];
// COMPACT-NEXT:         storage f128 [size=16, align=16];
// COMPACT-NEXT:         storage d32 [size=4, align=4];
// COMPACT-NEXT:         storage d64 [size=8, align=8];
// COMPACT-NEXT:         storage d128 [size=16, align=16];
// COMPACT-NEXT:     }
// COMPACT-NEXT:     type @type[[TYPE_Payload:[0-9]+]] Payload = struct {
// COMPACT-NEXT:         field0 value: i32;
// COMPACT-NEXT:         field1 callback: ptr<fn(i32) -> i32>;
// COMPACT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// COMPACT-NEXT:     type @type[[TYPE_Payload_2:[0-9]+]] Payload = @type[[TYPE_Payload]];
// COMPACT-NEXT:     type @type[[TYPE_Mode:[0-9]+]] Mode = enum : u32 {
// COMPACT-NEXT:         %[[VALUE_FIRST:[0-9]+]] FIRST = const<@type[[TYPE_Mode]]>(1);
// COMPACT-NEXT:         %[[VALUE_LAST:[0-9]+]] LAST = const<@type[[TYPE_Mode]]>(3);
// COMPACT-NEXT:     } [size=4, align=4];
// COMPACT-NEXT:     global %[[VALUE_after:[0-9]+]] after: i32 [storage=static] [linkage=external];
// COMPACT-NEXT:     fn %[[VALUE_read_value:[0-9]+]] @read_value(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_Payload]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// COMPACT-NEXT:         if ne<ptr<@type[[TYPE_Payload]]>>(read<ptr<@type[[TYPE_Payload]]>>(%[[VALUE_p]]), null<ptr<@type[[TYPE_Payload]]>>)
// COMPACT-NEXT:             {
// COMPACT-NEXT:                 return call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(field1(deref(read<ptr<@type[[TYPE_Payload]]>>(%[[VALUE_p]])))), read<i32>(field0(deref(read<ptr<@type[[TYPE_Payload]]>>(%[[VALUE_p]])))));
// COMPACT-NEXT:             }
// COMPACT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(enum_to_int<u32, reason=promotion>(const<@type[[TYPE_Mode]]>(3)));
// COMPACT-NEXT:     }
// COMPACT-NEXT: }
// SLATE-FILECHECK-END COMPACT
// SLATE-FILECHECK-BEGIN SPLIT
// SPLIT: module {
// SPLIT-NEXT:     target "x86_64-unknown-linux-gnu" {
// SPLIT-NEXT:         endian = little;
// SPLIT-NEXT:         pointer [size=8, align=8];
// SPLIT-NEXT:         stack_alignment = 16;
// SPLIT-NEXT:         long_double = f80;
// SPLIT-NEXT:         storage bool [size=1, align=1];
// SPLIT-NEXT:         storage i8, u8 [size=1, align=1];
// SPLIT-NEXT:         storage i16, u16 [size=2, align=2];
// SPLIT-NEXT:         storage i32, u32 [size=4, align=4];
// SPLIT-NEXT:         storage i64, u64 [size=8, align=8];
// SPLIT-NEXT:         storage i128, u128 [size=16, align=16];
// SPLIT-NEXT:         storage bf16 [size=2, align=2];
// SPLIT-NEXT:         storage f16 [size=2, align=2];
// SPLIT-NEXT:         storage f32 [size=4, align=4];
// SPLIT-NEXT:         storage f64 [size=8, align=8];
// SPLIT-NEXT:         storage f80 [size=16, align=16];
// SPLIT-NEXT:         storage f128 [size=16, align=16];
// SPLIT-NEXT:         storage d32 [size=4, align=4];
// SPLIT-NEXT:         storage d64 [size=8, align=8];
// SPLIT-NEXT:         storage d128 [size=16, align=16];
// SPLIT-NEXT:     }
// SPLIT-NEXT:     type @type[[TYPE_Payload:[0-9]+]] Payload = struct {
// SPLIT-NEXT:         field0 value: i32;
// SPLIT-NEXT:         field1 callback: ptr<fn(i32) -> i32>;
// SPLIT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// SPLIT-NEXT:     type @type[[TYPE_Payload_2:[0-9]+]] Payload = @type[[TYPE_Payload]];
// SPLIT-NEXT:     type @type[[TYPE_Mode:[0-9]+]] Mode = enum : u32 {
// SPLIT-NEXT:         %[[VALUE_FIRST:[0-9]+]] FIRST = const<@type[[TYPE_Mode]]>(1);
// SPLIT-NEXT:         %[[VALUE_LAST:[0-9]+]] LAST = const<@type[[TYPE_Mode]]>(3);
// SPLIT-NEXT:     } [size=4, align=4];
// SPLIT-NEXT:     global %[[VALUE_after:[0-9]+]] after: i32 [storage=static] [linkage=external];
// SPLIT-NEXT:     fn %[[VALUE_read_value:[0-9]+]] @read_value(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_Payload]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// SPLIT-NEXT:         if ne<ptr<@type[[TYPE_Payload]]>>(read<ptr<@type[[TYPE_Payload]]>>(%[[VALUE_p]]), null<ptr<@type[[TYPE_Payload]]>>)
// SPLIT-NEXT:             {
// SPLIT-NEXT:                 return call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(field1(deref(read<ptr<@type[[TYPE_Payload]]>>(%[[VALUE_p]])))), read<i32>(field0(deref(read<ptr<@type[[TYPE_Payload]]>>(%[[VALUE_p]])))));
// SPLIT-NEXT:             }
// SPLIT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(enum_to_int<u32, reason=promotion>(const<@type[[TYPE_Mode]]>(3)));
// SPLIT-NEXT:     }
// SPLIT-NEXT: }
// SLATE-FILECHECK-END SPLIT
// SLATE-FILECHECK-BEGIN MACRO
// MACRO: module {
// MACRO-NEXT:     target "x86_64-unknown-linux-gnu" {
// MACRO-NEXT:         endian = little;
// MACRO-NEXT:         pointer [size=8, align=8];
// MACRO-NEXT:         stack_alignment = 16;
// MACRO-NEXT:         long_double = f80;
// MACRO-NEXT:         storage bool [size=1, align=1];
// MACRO-NEXT:         storage i8, u8 [size=1, align=1];
// MACRO-NEXT:         storage i16, u16 [size=2, align=2];
// MACRO-NEXT:         storage i32, u32 [size=4, align=4];
// MACRO-NEXT:         storage i64, u64 [size=8, align=8];
// MACRO-NEXT:         storage i128, u128 [size=16, align=16];
// MACRO-NEXT:         storage bf16 [size=2, align=2];
// MACRO-NEXT:         storage f16 [size=2, align=2];
// MACRO-NEXT:         storage f32 [size=4, align=4];
// MACRO-NEXT:         storage f64 [size=8, align=8];
// MACRO-NEXT:         storage f80 [size=16, align=16];
// MACRO-NEXT:         storage f128 [size=16, align=16];
// MACRO-NEXT:         storage d32 [size=4, align=4];
// MACRO-NEXT:         storage d64 [size=8, align=8];
// MACRO-NEXT:         storage d128 [size=16, align=16];
// MACRO-NEXT:     }
// MACRO-NEXT:     type @type[[TYPE_Payload:[0-9]+]] Payload = struct {
// MACRO-NEXT:         field0 value: i32;
// MACRO-NEXT:         field1 callback: ptr<fn(i32) -> i32>;
// MACRO-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// MACRO-NEXT:     type @type[[TYPE_Payload_2:[0-9]+]] Payload = @type[[TYPE_Payload]];
// MACRO-NEXT:     type @type[[TYPE_Mode:[0-9]+]] Mode = enum : u32 {
// MACRO-NEXT:         %[[VALUE_FIRST:[0-9]+]] FIRST = const<@type[[TYPE_Mode]]>(1);
// MACRO-NEXT:         %[[VALUE_LAST:[0-9]+]] LAST = const<@type[[TYPE_Mode]]>(3);
// MACRO-NEXT:     } [size=4, align=4];
// MACRO-NEXT:     global %[[VALUE_after:[0-9]+]] after: i32 [storage=static] [linkage=external];
// MACRO-NEXT:     fn %[[VALUE_read_value:[0-9]+]] @read_value(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_Payload]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// MACRO-NEXT:         if ne<ptr<@type[[TYPE_Payload]]>>(read<ptr<@type[[TYPE_Payload]]>>(%[[VALUE_p]]), null<ptr<@type[[TYPE_Payload]]>>)
// MACRO-NEXT:             {
// MACRO-NEXT:                 return call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(field1(deref(read<ptr<@type[[TYPE_Payload]]>>(%[[VALUE_p]])))), read<i32>(field0(deref(read<ptr<@type[[TYPE_Payload]]>>(%[[VALUE_p]])))));
// MACRO-NEXT:             }
// MACRO-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(enum_to_int<u32, reason=promotion>(const<@type[[TYPE_Mode]]>(3)));
// MACRO-NEXT:     }
// MACRO-NEXT: }
// SLATE-FILECHECK-END MACRO
