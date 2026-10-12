// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir

const __NSConstantString *file_scope = __builtin___CFStringMakeConstantString("hi");

const void *local(void) { return __builtin___CFStringMakeConstantString("abc"); }

const void *again(void) { return __builtin___CFStringMakeConstantString("abc"); }

unsigned long size(void) { return sizeof(__NSConstantString); }

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
// DEFAULT-NEXT:     type @type[[TYPE___NSConstantString_tag:[0-9]+]] __NSConstantString_tag = struct {
// DEFAULT-NEXT:         field0 isa: ptr<const i32>;
// DEFAULT-NEXT:         field1 flags: i32;
// DEFAULT-NEXT:         field2 str: ptr<const i8>;
// DEFAULT-NEXT:         field3 length: i64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE___NSConstantString:[0-9]+]] __NSConstantString = @type[[TYPE___NSConstantString_tag]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([104, 105, 0]) [linkage=internal];
// DEFAULT-NEXT:     extern %[[VALUE___CFConstantStringClassReference:[0-9]+]] __CFConstantStringClassReference: array<i32, incomplete> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE__unnamed_cfstring_8:[0-9]+]] _unnamed_cfstring_8: @type[[TYPE___NSConstantString_tag]] [storage=static] = aggregate<@type[[TYPE___NSConstantString_tag]], zero_fill=false>(field0 = array_decay<ptr<const i32>, length=None>(%[[VALUE___CFConstantStringClassReference]]), field1 = const<i32>(1992), field2 = array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]]), field3 = const<i64>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_file_scope:[0-9]+]] file_scope: ptr<const @type[[TYPE___NSConstantString_tag]]> [storage=static] = addr_of<ptr<const @type[[TYPE___NSConstantString_tag]]>>(%[[VALUE__unnamed_cfstring_8]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE__unnamed_cfstring_10:[0-9]+]] _unnamed_cfstring_10: @type[[TYPE___NSConstantString_tag]] [storage=static] = aggregate<@type[[TYPE___NSConstantString_tag]], zero_fill=false>(field0 = array_decay<ptr<const i32>, length=None>(%[[VALUE___CFConstantStringClassReference]]), field1 = const<i32>(1992), field2 = array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]]), field3 = const<i64>(3)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE__unnamed_cfstring_12:[0-9]+]] _unnamed_cfstring_12: @type[[TYPE___NSConstantString_tag]] [storage=static] = aggregate<@type[[TYPE___NSConstantString_tag]], zero_fill=false>(field0 = array_decay<ptr<const i32>, length=None>(%[[VALUE___CFConstantStringClassReference]]), field1 = const<i32>(1992), field2 = array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]]), field3 = const<i64>(3)) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_local:[0-9]+]] @local() -> ptr<const void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<const void>, reason=return>(addr_of<ptr<const @type[[TYPE___NSConstantString_tag]]>>(%[[VALUE__unnamed_cfstring_10]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_again:[0-9]+]] @again() -> ptr<const void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<const void>, reason=return>(addr_of<ptr<const @type[[TYPE___NSConstantString_tag]]>>(%[[VALUE__unnamed_cfstring_12]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_size:[0-9]+]] @size() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<u64>(32);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
