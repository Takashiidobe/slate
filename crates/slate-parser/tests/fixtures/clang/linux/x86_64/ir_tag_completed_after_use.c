typedef struct Named Alias;
struct Named { int a; };
Alias through_typedef;

struct Param *take(struct Param *p);
struct Param { long b; };
struct Param through_prototype;

typedef enum Color Shade;
enum Color { red, green };
Shade through_enum_typedef = green;

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir

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
// DEFAULT-NEXT:     type @type[[TYPE_Named:[0-9]+]] Named = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Alias:[0-9]+]] Alias = @type[[TYPE_Named]];
// DEFAULT-NEXT:     type @type[[TYPE_Param:[0-9]+]] Param = struct {
// DEFAULT-NEXT:         field0 b: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Color:[0-9]+]] Color = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_red:[0-9]+]] red = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_green:[0-9]+]] green = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_Shade:[0-9]+]] Shade = @type[[TYPE_Color]];
// DEFAULT-NEXT:     global %[[VALUE_through_typedef:[0-9]+]] through_typedef: @type[[TYPE_Named]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_through_prototype:[0-9]+]] through_prototype: @type[[TYPE_Param]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_through_enum_typedef:[0-9]+]] through_enum_typedef: @type[[TYPE_Color]] [storage=static] = int_to_enum<@type[[TYPE_Color]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_take:[0-9]+]] @take(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_Param]]>) -> ptr<@type[[TYPE_Param]]> [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
