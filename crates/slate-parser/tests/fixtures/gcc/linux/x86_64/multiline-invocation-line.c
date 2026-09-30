// __LINE__ in a macro body spanning lines: clang reports the invocation's
// closing paren, gcc the macro name, msvc how far the source has been read
#define F(a, b) a, b, __LINE__
#define G F
#define L __LINE__
#define A(e) { #e, __LINE__ }
struct site { const char *text; int line; };
int v[] = { F(1,
  2
  ) };
int w[] = { G
(1,
2), F(L,
3
), F(__LINE__,
F(4,5
)) };
struct site a = A(F(1,
2
)
);
int z = L;

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
// DEFAULT-NEXT:     type @type[[TYPE_site:[0-9]+]] site = struct {
// DEFAULT-NEXT:         field0 text: ptr<const i8>;
// DEFAULT-NEXT:         field1 line: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: array<i32, 3> [storage=static] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(8)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_w:[0-9]+]] w: array<i32, 11> [storage=static] [align=16] = aggregate<array<i32, 11>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(11), index3 = const<i32>(13), index4 = const<i32>(3), index5 = const<i32>(13), index6 = const<i32>(15), index7 = const<i32>(4), index8 = const<i32>(5), index9 = const<i32>(16), index10 = const<i32>(15)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([70, 40, 49, 44, 32, 50, 32, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: @type[[TYPE_site]] [storage=static] = aggregate<@type[[TYPE_site]], zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str]])), field1 = const<i32>(18)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_z:[0-9]+]] z: i32 [storage=static] = const<i32>(22) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
