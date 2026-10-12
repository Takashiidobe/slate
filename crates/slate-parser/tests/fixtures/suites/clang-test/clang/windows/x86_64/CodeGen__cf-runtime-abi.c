






const __NSConstantString *s = __builtin___CFStringMakeConstantString("");

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE___NSConstantString_tag:[0-9]+]] __NSConstantString_tag = struct {
// DEFAULT-NEXT:         field0 isa: ptr<const i32>;
// DEFAULT-NEXT:         field1 flags: i32;
// DEFAULT-NEXT:         field2 str: ptr<const i8>;
// DEFAULT-NEXT:         field3 length: i32;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE___NSConstantString:[0-9]+]] __NSConstantString = @type[[TYPE___NSConstantString_tag]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     extern %[[VALUE___CFConstantStringClassReference:[0-9]+]] __CFConstantStringClassReference: array<i32, incomplete> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE__unnamed_cfstring_5:[0-9]+]] _unnamed_cfstring_5: @type[[TYPE___NSConstantString_tag]] [storage=static] = aggregate<@type[[TYPE___NSConstantString_tag]], zero_fill=false>(field0 = array_decay<ptr<const i32>, length=None>(%[[VALUE___CFConstantStringClassReference]]), field1 = const<i32>(1992), field2 = array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]]), field3 = const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: ptr<const @type[[TYPE___NSConstantString_tag]]> [storage=static] = addr_of<ptr<const @type[[TYPE___NSConstantString_tag]]>>(%[[VALUE__unnamed_cfstring_5]]) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
