#define MAPPER(FN, ctx...) FN(first, 1, ##ctx) FN(second, 2, ##ctx)
#define ENUM_FN(name, value) ID_##name = value,
#define COUNT_FN(name, value, base) + (value + base)
#define FIRST(head, rest...) head
#define LIST(items...) { items }
#define NAME(x, rest...) #rest

enum ids {
  MAPPER(ENUM_FN)
  ID_MAX
};

int total = 0 MAPPER(COUNT_FN, 10);
int head = FIRST(3);
int list[] = LIST(1, 2, 3);
int empty[] = LIST();
char named[] = NAME(0, a, b);


// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     type @type[[TYPE_ids:[0-9]+]] ids = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_ID_first:[0-9]+]] ID_first = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_ID_second:[0-9]+]] ID_second = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_ID_MAX:[0-9]+]] ID_MAX = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_total:[0-9]+]] total: i32 [storage=static] = add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(0), add<i32, overflow=ub>(const<i32>(1), const<i32>(10))), add<i32, overflow=ub>(const<i32>(2), const<i32>(10))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_head:[0-9]+]] head: i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_list:[0-9]+]] list: array<i32, 3> [storage=static] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_empty:[0-9]+]] empty: array<i32, 0> [storage=static] = aggregate<array<i32, 0>, zero_fill=false>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_named:[0-9]+]] named: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 44, 32, 98, 0]) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
