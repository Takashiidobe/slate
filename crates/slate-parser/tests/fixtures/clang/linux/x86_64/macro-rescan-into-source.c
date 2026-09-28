// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS -std=c23

#define ZERO(x) 0
#define OPEN ZERO (
#define IDENTITY(x) x
#define ALIAS IDENTITY
#define ALIAS_OF_ALIAS ALIAS
#define PASTE(x) OPEN_##x
#define OPEN_a ZERO (

enum { open_paren_from_macro = OPEN 1) };
enum { name_from_macro = ALIAS (2) };
enum { name_through_two_macros = ALIAS_OF_ALIAS (3) };
enum { paste_then_rescan = PASTE(a) 4) };

static_assert(open_paren_from_macro == 0);
static_assert(name_from_macro == 2);
static_assert(name_through_two_macros == 3);
static_assert(paste_then_rescan == 0);

enum { first_of_two = ALIAS (10), second_of_two = ALIAS (20) };
static_assert(first_of_two == 10 && second_of_two == 20);

#define ARGS(a, b) a + b
#define SPLIT ARGS(1,
enum { arguments_split_across_expansion = SPLIT 2) };
static_assert(arguments_split_across_expansion == 3);

enum { recursive = 5 };
#define recursive recursive
static_assert(recursive == 5);
int blue_paint[] = { recursive, recursive };

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
// DEFAULT-NEXT:     type @type0 = enum : u32 {
// DEFAULT-NEXT:         %0 open_paren_from_macro = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 = enum : u32 {
// DEFAULT-NEXT:         %0 name_from_macro = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type2 = enum : u32 {
// DEFAULT-NEXT:         %0 name_through_two_macros = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type3 = enum : u32 {
// DEFAULT-NEXT:         %0 paste_then_rescan = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type4 = enum : u32 {
// DEFAULT-NEXT:         %0 first_of_two = const<i32>(10);
// DEFAULT-NEXT:         %1 second_of_two = const<i32>(20);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type5 = enum : u32 {
// DEFAULT-NEXT:         %0 arguments_split_across_expansion = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type6 = enum : u32 {
// DEFAULT-NEXT:         %0 recursive = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %15 blue_paint: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(5), index1 = const<i32>(5)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
