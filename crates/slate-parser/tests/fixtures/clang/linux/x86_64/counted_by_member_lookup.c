struct outer_counter {
    int count;
    union {
        struct {
            int tag;
            int tail[] __attribute__((counted_by(count)));
        };
        int raw;
    };
};

struct nested_counter {
    struct {
        int count;
    };
    void *bytes __attribute__((sized_by(count)));
    int *items __attribute__((counted_by_or_null(count)));
    void *maybe __attribute__((sized_by_or_null(count)));
};

int first(struct outer_counter *o, struct nested_counter *n) { return o->count + n->count; }

// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

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
// IR-NEXT:     type @type[[TYPE_outer_counter:[0-9]+]] outer_counter = struct {
// IR-NEXT:         field0 count: i32;
// IR-NEXT:         field1 <anonymous>: @type[[TYPE0:[0-9]+]];
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE0]] = union {
// IR-NEXT:         field0 <anonymous>: @type[[TYPE1:[0-9]+]];
// IR-NEXT:         field1 raw: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE1]] = struct {
// IR-NEXT:         field0 tag: i32;
// IR-NEXT:         field1 tail: array<i32, incomplete>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_nested_counter:[0-9]+]] nested_counter = struct {
// IR-NEXT:         field0 <anonymous>: @type[[TYPE2:[0-9]+]];
// IR-NEXT:         field1 bytes: ptr<void>;
// IR-NEXT:         field2 items: ptr<i32>;
// IR-NEXT:         field3 maybe: ptr<void>;
// IR-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// IR-NEXT:     type @type[[TYPE2]] = struct {
// IR-NEXT:         field0 count: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     fn %[[VALUE_first:[0-9]+]] @first(%[[VALUE_o:[0-9]+]] o: ptr<@type[[TYPE_outer_counter]]>, %[[VALUE_n:[0-9]+]] n: ptr<@type[[TYPE_nested_counter]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_outer_counter]]>>(%[[VALUE_o]])))), read<i32>(field0(field0(deref(read<ptr<@type[[TYPE_nested_counter]]>>(%[[VALUE_n]]))))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
