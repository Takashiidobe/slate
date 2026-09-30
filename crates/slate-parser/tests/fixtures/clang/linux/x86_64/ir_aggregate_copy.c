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
// IR-NEXT:     type @type[[TYPE_Pair:[0-9]+]] Pair = struct {
// IR-NEXT:         field0 first: i32;
// IR-NEXT:         field1 second: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_Item:[0-9]+]] Item = union {
// IR-NEXT:         field0 integer: i32;
// IR-NEXT:         field1 real: f64;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE_Pair_2:[0-9]+]] Pair = @type[[TYPE_Pair]];
// IR-NEXT:     type @type[[TYPE_Item_2:[0-9]+]] Item = @type[[TYPE_Item]];
// IR-NEXT:     fn %[[VALUE_take_pair:[0-9]+]] @take_pair(%[[VALUE_value:[0-9]+]] value: @type[[TYPE_Pair]]) -> void [linkage=external] [abi=sysv64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_take_item:[0-9]+]] @take_item(%[[VALUE_value_2:[0-9]+]] value: @type[[TYPE_Item]]) -> void [linkage=external] [abi=sysv64(native_c) -> void];
// IR-NEXT:     fn %[[VALUE_copy_pair:[0-9]+]] @copy_pair(%[[VALUE_source:[0-9]+]] source: @type[[TYPE_Pair]]) -> @type[[TYPE_Pair]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_initialized:[0-9]+]] initialized: @type[[TYPE_Pair]] [storage=automatic] = copy<@type[[TYPE_Pair]], reason=assign>(read<@type[[TYPE_Pair]]>(%[[VALUE_source]]));
// IR-NEXT:         let %[[VALUE_assigned:[0-9]+]] assigned: @type[[TYPE_Pair]] [storage=automatic];
// IR-NEXT:         write<@type[[TYPE_Pair]]>(%[[VALUE_assigned]], copy<@type[[TYPE_Pair]], reason=assign>(read<@type[[TYPE_Pair]]>(%[[VALUE_initialized]])));
// IR-NEXT:         call<void, signature=fn(@type[[TYPE_Pair]]) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_take_pair]], copy<@type[[TYPE_Pair]], reason=arg>(read<@type[[TYPE_Pair]]>(%[[VALUE_assigned]])));
// IR-NEXT:         return copy<@type[[TYPE_Pair]], reason=return>(read<@type[[TYPE_Pair]]>(%[[VALUE_assigned]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_copy_item:[0-9]+]] @copy_item(%[[VALUE_source_2:[0-9]+]] source: @type[[TYPE_Item]]) -> @type[[TYPE_Item]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_initialized_2:[0-9]+]] initialized: @type[[TYPE_Item]] [storage=automatic] = copy<@type[[TYPE_Item]], reason=assign>(read<@type[[TYPE_Item]]>(%[[VALUE_source_2]]));
// IR-NEXT:         let %[[VALUE_assigned_2:[0-9]+]] assigned: @type[[TYPE_Item]] [storage=automatic];
// IR-NEXT:         write<@type[[TYPE_Item]]>(%[[VALUE_assigned_2]], copy<@type[[TYPE_Item]], reason=assign>(read<@type[[TYPE_Item]]>(%[[VALUE_initialized_2]])));
// IR-NEXT:         call<void, signature=fn(@type[[TYPE_Item]]) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_take_item]], copy<@type[[TYPE_Item]], reason=arg>(read<@type[[TYPE_Item]]>(%[[VALUE_assigned_2]])));
// IR-NEXT:         return copy<@type[[TYPE_Item]], reason=return>(read<@type[[TYPE_Item]]>(%[[VALUE_assigned_2]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
