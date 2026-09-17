// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
struct Pair { int first; int second; };
union Item { int integer; double real; };
typedef struct Pair Pair;
typedef union Item Item;
void take_pair(struct Pair value);
void take_item(union Item value);

struct Pair copy_pair(struct Pair source) {
    Pair initialized = source;
    Pair assigned;
    assigned = initialized;
    take_pair(assigned);
    return assigned;
}

union Item copy_item(union Item source) {
    Item initialized = source;
    Item assigned;
    assigned = initialized;
    take_item(assigned);
    return assigned;
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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 Pair = struct {
// IR-NEXT:         field0 first: i32;
// IR-NEXT:         field1 second: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type1 Item = union {
// IR-NEXT:         field0 integer: i32;
// IR-NEXT:         field1 real: f64;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// IR-NEXT:     type @type2 Pair = @type0;
// IR-NEXT:     type @type3 Item = @type1;
// IR-NEXT:     fn %4 @take_pair(%14 value: @type0) -> void [linkage=external];
// IR-NEXT:     fn %5 @take_item(%15 value: @type1) -> void [linkage=external];
// IR-NEXT:     fn %6 @copy_pair(%7 source: @type0) -> @type0 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %8 initialized: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(%7));
// IR-NEXT:         let %9 assigned: @type0 [storage=automatic];
// IR-NEXT:         write<@type0>(%9, copy<@type0, reason=assign>(read<@type0>(%8)));
// IR-NEXT:         call<void, signature=fn(@type0) -> void>(%4, copy<@type0, reason=arg>(read<@type0>(%9)));
// IR-NEXT:         return copy<@type0, reason=return>(read<@type0>(%9));
// IR-NEXT:     }
// IR-NEXT:     fn %10 @copy_item(%11 source: @type1) -> @type1 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %12 initialized: @type1 [storage=automatic] = copy<@type1, reason=assign>(read<@type1>(%11));
// IR-NEXT:         let %13 assigned: @type1 [storage=automatic];
// IR-NEXT:         write<@type1>(%13, copy<@type1, reason=assign>(read<@type1>(%12)));
// IR-NEXT:         call<void, signature=fn(@type1) -> void>(%5, copy<@type1, reason=arg>(read<@type1>(%13)));
// IR-NEXT:         return copy<@type1, reason=return>(read<@type1>(%13));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
