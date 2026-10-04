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

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES FLAG
// SLATE-FILECHECK-PREFIX-ARGS FLAG -fms-anonymous-structs
// SLATE-FILECHECK-DEFINES EXT
// SLATE-FILECHECK-PREFIX-ARGS EXT -fno-ms-anonymous-structs -fms-extensions
// SLATE-FILECHECK-DEFINES COMPAT
// SLATE-FILECHECK-PREFIX-ARGS COMPAT -fno-ms-anonymous-structs -fms-compatibility

// SLATE-FILECHECK-BEGIN FLAG
// FLAG: module {
// FLAG-NEXT:     target "x86_64-unknown-linux-gnu" {
// FLAG-NEXT:         endian = little;
// FLAG-NEXT:         pointer [size=8, align=8];
// FLAG-NEXT:         stack_alignment = 16;
// FLAG-NEXT:         long_double = f80;
// FLAG-NEXT:         storage bool [size=1, align=1];
// FLAG-NEXT:         storage i8, u8 [size=1, align=1];
// FLAG-NEXT:         storage i16, u16 [size=2, align=2];
// FLAG-NEXT:         storage i32, u32 [size=4, align=4];
// FLAG-NEXT:         storage i64, u64 [size=8, align=8];
// FLAG-NEXT:         storage i128, u128 [size=16, align=16];
// FLAG-NEXT:         storage bf16 [size=2, align=2];
// FLAG-NEXT:         storage f16 [size=2, align=2];
// FLAG-NEXT:         storage f32 [size=4, align=4];
// FLAG-NEXT:         storage f64 [size=8, align=8];
// FLAG-NEXT:         storage f80 [size=16, align=16];
// FLAG-NEXT:         storage f128 [size=16, align=16];
// FLAG-NEXT:         storage d32 [size=4, align=4];
// FLAG-NEXT:         storage d64 [size=8, align=8];
// FLAG-NEXT:         storage d128 [size=16, align=16];
// FLAG-NEXT:     }
// FLAG-NEXT:     type @type[[TYPE_inner:[0-9]+]] inner = struct {
// FLAG-NEXT:         field0 a: i32;
// FLAG-NEXT:     } [size=4, align=4, offsets=[0]];
// FLAG-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// FLAG-NEXT:         field0 t: i32;
// FLAG-NEXT:     } [size=4, align=4, offsets=[0]];
// FLAG-NEXT:     type @type[[TYPE_tagless:[0-9]+]] tagless = @type[[TYPE0]];
// FLAG-NEXT:     type @type[[TYPE1:[0-9]+]] = union {
// FLAG-NEXT:         field0 u: i32;
// FLAG-NEXT:         field1 f: f32;
// FLAG-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// FLAG-NEXT:     type @type[[TYPE_either:[0-9]+]] either = @type[[TYPE1]];
// FLAG-NEXT:     type @type[[TYPE_outer:[0-9]+]] outer = struct {
// FLAG-NEXT:         field0 <anonymous>: @type[[TYPE_inner]];
// FLAG-NEXT:         field1 <anonymous>: @type[[TYPE0]];
// FLAG-NEXT:         field2 <anonymous>: @type[[TYPE1]];
// FLAG-NEXT:         field3 <anonymous>: @type[[TYPE_nested:[0-9]+]];
// FLAG-NEXT:         field4 b: i32;
// FLAG-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// FLAG-NEXT:     type @type[[TYPE_nested]] nested = struct {
// FLAG-NEXT:         field0 c: i32;
// FLAG-NEXT:     } [size=4, align=4, offsets=[0]];
// FLAG-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_o:[0-9]+]] o: ptr<@type[[TYPE_outer]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// FLAG-NEXT:         return add<i32>(add<i32>(add<i32>(add<i32>(read<i32>(field0(field0(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]]))))), read<i32>(field0(field1(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]])))))), read<i32>(field0(field2(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]])))))), read<i32>(field0(field3(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]])))))), read<i32>(field4(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]])))));
// FLAG-NEXT:     }
// FLAG-NEXT: }
// SLATE-FILECHECK-END FLAG
// SLATE-FILECHECK-BEGIN EXT
// EXT: module {
// EXT-NEXT:     target "x86_64-unknown-linux-gnu" {
// EXT-NEXT:         endian = little;
// EXT-NEXT:         pointer [size=8, align=8];
// EXT-NEXT:         stack_alignment = 16;
// EXT-NEXT:         long_double = f80;
// EXT-NEXT:         storage bool [size=1, align=1];
// EXT-NEXT:         storage i8, u8 [size=1, align=1];
// EXT-NEXT:         storage i16, u16 [size=2, align=2];
// EXT-NEXT:         storage i32, u32 [size=4, align=4];
// EXT-NEXT:         storage i64, u64 [size=8, align=8];
// EXT-NEXT:         storage i128, u128 [size=16, align=16];
// EXT-NEXT:         storage bf16 [size=2, align=2];
// EXT-NEXT:         storage f16 [size=2, align=2];
// EXT-NEXT:         storage f32 [size=4, align=4];
// EXT-NEXT:         storage f64 [size=8, align=8];
// EXT-NEXT:         storage f80 [size=16, align=16];
// EXT-NEXT:         storage f128 [size=16, align=16];
// EXT-NEXT:         storage d32 [size=4, align=4];
// EXT-NEXT:         storage d64 [size=8, align=8];
// EXT-NEXT:         storage d128 [size=16, align=16];
// EXT-NEXT:     }
// EXT-NEXT:     type @type[[TYPE_inner:[0-9]+]] inner = struct {
// EXT-NEXT:         field0 a: i32;
// EXT-NEXT:     } [size=4, align=4, offsets=[0]];
// EXT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// EXT-NEXT:         field0 t: i32;
// EXT-NEXT:     } [size=4, align=4, offsets=[0]];
// EXT-NEXT:     type @type[[TYPE_tagless:[0-9]+]] tagless = @type[[TYPE0]];
// EXT-NEXT:     type @type[[TYPE1:[0-9]+]] = union {
// EXT-NEXT:         field0 u: i32;
// EXT-NEXT:         field1 f: f32;
// EXT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// EXT-NEXT:     type @type[[TYPE_either:[0-9]+]] either = @type[[TYPE1]];
// EXT-NEXT:     type @type[[TYPE_outer:[0-9]+]] outer = struct {
// EXT-NEXT:         field0 <anonymous>: @type[[TYPE_inner]];
// EXT-NEXT:         field1 <anonymous>: @type[[TYPE0]];
// EXT-NEXT:         field2 <anonymous>: @type[[TYPE1]];
// EXT-NEXT:         field3 <anonymous>: @type[[TYPE_nested:[0-9]+]];
// EXT-NEXT:         field4 b: i32;
// EXT-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// EXT-NEXT:     type @type[[TYPE_nested]] nested = struct {
// EXT-NEXT:         field0 c: i32;
// EXT-NEXT:     } [size=4, align=4, offsets=[0]];
// EXT-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_o:[0-9]+]] o: ptr<@type[[TYPE_outer]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// EXT-NEXT:         return add<i32>(add<i32>(add<i32>(add<i32>(read<i32>(field0(field0(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]]))))), read<i32>(field0(field1(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]])))))), read<i32>(field0(field2(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]])))))), read<i32>(field0(field3(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]])))))), read<i32>(field4(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]])))));
// EXT-NEXT:     }
// EXT-NEXT: }
// SLATE-FILECHECK-END EXT
// SLATE-FILECHECK-BEGIN COMPAT
// COMPAT: module {
// COMPAT-NEXT:     target "x86_64-unknown-linux-gnu" {
// COMPAT-NEXT:         endian = little;
// COMPAT-NEXT:         pointer [size=8, align=8];
// COMPAT-NEXT:         stack_alignment = 16;
// COMPAT-NEXT:         long_double = f80;
// COMPAT-NEXT:         storage bool [size=1, align=1];
// COMPAT-NEXT:         storage i8, u8 [size=1, align=1];
// COMPAT-NEXT:         storage i16, u16 [size=2, align=2];
// COMPAT-NEXT:         storage i32, u32 [size=4, align=4];
// COMPAT-NEXT:         storage i64, u64 [size=8, align=8];
// COMPAT-NEXT:         storage i128, u128 [size=16, align=16];
// COMPAT-NEXT:         storage bf16 [size=2, align=2];
// COMPAT-NEXT:         storage f16 [size=2, align=2];
// COMPAT-NEXT:         storage f32 [size=4, align=4];
// COMPAT-NEXT:         storage f64 [size=8, align=8];
// COMPAT-NEXT:         storage f80 [size=16, align=16];
// COMPAT-NEXT:         storage f128 [size=16, align=16];
// COMPAT-NEXT:         storage d32 [size=4, align=4];
// COMPAT-NEXT:         storage d64 [size=8, align=8];
// COMPAT-NEXT:         storage d128 [size=16, align=16];
// COMPAT-NEXT:     }
// COMPAT-NEXT:     type @type[[TYPE_inner:[0-9]+]] inner = struct {
// COMPAT-NEXT:         field0 a: i32;
// COMPAT-NEXT:     } [size=4, align=4, offsets=[0]];
// COMPAT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// COMPAT-NEXT:         field0 t: i32;
// COMPAT-NEXT:     } [size=4, align=4, offsets=[0]];
// COMPAT-NEXT:     type @type[[TYPE_tagless:[0-9]+]] tagless = @type[[TYPE0]];
// COMPAT-NEXT:     type @type[[TYPE1:[0-9]+]] = union {
// COMPAT-NEXT:         field0 u: i32;
// COMPAT-NEXT:         field1 f: f32;
// COMPAT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// COMPAT-NEXT:     type @type[[TYPE_either:[0-9]+]] either = @type[[TYPE1]];
// COMPAT-NEXT:     type @type[[TYPE_outer:[0-9]+]] outer = struct {
// COMPAT-NEXT:         field0 <anonymous>: @type[[TYPE_inner]];
// COMPAT-NEXT:         field1 <anonymous>: @type[[TYPE0]];
// COMPAT-NEXT:         field2 <anonymous>: @type[[TYPE1]];
// COMPAT-NEXT:         field3 <anonymous>: @type[[TYPE_nested:[0-9]+]];
// COMPAT-NEXT:         field4 b: i32;
// COMPAT-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// COMPAT-NEXT:     type @type[[TYPE_nested]] nested = struct {
// COMPAT-NEXT:         field0 c: i32;
// COMPAT-NEXT:     } [size=4, align=4, offsets=[0]];
// COMPAT-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_o:[0-9]+]] o: ptr<@type[[TYPE_outer]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// COMPAT-NEXT:         return add<i32>(add<i32>(add<i32>(add<i32>(read<i32>(field0(field0(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]]))))), read<i32>(field0(field1(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]])))))), read<i32>(field0(field2(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]])))))), read<i32>(field0(field3(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]])))))), read<i32>(field4(deref(read<ptr<@type[[TYPE_outer]]>>(%[[VALUE_o]])))));
// COMPAT-NEXT:     }
// COMPAT-NEXT: }
// SLATE-FILECHECK-END COMPAT
