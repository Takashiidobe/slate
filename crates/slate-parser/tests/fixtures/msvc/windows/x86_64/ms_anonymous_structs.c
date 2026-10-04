struct inner {
    int a;
};

typedef struct {
    int t;
} tagless;

typedef union {
    int u;
    float f;
} either;

struct outer {
    struct inner;
    tagless;
    either;
    struct nested {
        int c;
    };
    int b;
};

_Static_assert(sizeof(struct outer) == 20, "embedded records are members");

int sum(struct outer *o) { return o->a + o->t + o->u + o->c + o->b; }

// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
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
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_inner:[0-9]+]] inner = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// IR-NEXT:         field0 t: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_tagless:[0-9]+]] tagless = @type[[TYPE0]];
// IR-NEXT:     type @type[[TYPE1:[0-9]+]] = union {
// IR-NEXT:         field0 u: i32;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE_either:[0-9]+]] either = @type[[TYPE1]];
// IR-NEXT:     type @type[[TYPE_outer:[0-9]+]] outer = struct {
// IR-NEXT:         field0 <anonymous>: @type[[TYPE_inner]];
// IR-NEXT:         field1 <anonymous>: @type[[TYPE0]];
// IR-NEXT:         field2 <anonymous>: @type[[TYPE1]];
// IR-NEXT:         field3 <anonymous>: @type[[TYPE_nested:[0-9]+]];
// IR-NEXT:         field4 b: i32;
// IR-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// IR-NEXT:     type @type[[TYPE_nested]] nested = struct {
// IR-NEXT:         field0 c: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_o:[0-9]+]] o: ptr<@type[[TYPE_outer]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32>(add<i32>(add<i32>(add<i32>(read<i32>(field0(field0(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]]))))), read<i32>(field0(field1(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]])))))), read<i32>(field0(field2(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]])))))), read<i32>(field0(field3(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]])))))), read<i32>(field4(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]])))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
