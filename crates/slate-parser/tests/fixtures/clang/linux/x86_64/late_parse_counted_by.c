struct buffer {
    int *items __attribute__((counted_by(count)));
    int count;
    int tail[] __attribute__((counted_by(count)));
};

int first(struct buffer *b) { return b->count > 0 ? b->tail[0] : 0; }

// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir -fexperimental-late-parse-attributes -fstrict-flex-arrays=3

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
// IR-NEXT:     type @type[[TYPE_buffer:[0-9]+]] buffer = struct {
// IR-NEXT:         field0 items: ptr<i32>;
// IR-NEXT:         field1 count: i32;
// IR-NEXT:         field2 tail: array<i32, incomplete>;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8, 12]];
// IR-NEXT:     fn %[[VALUE_first:[0-9]+]] @first(%[[VALUE_b:[0-9]+]] b: ptr<@type[[TYPE_buffer]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return conditional<i32>(gt<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_buffer]]>>(%[[VALUE_b]])))), const<i32>(0)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false>(array_decay<ptr<i32>, length=None>(field2(deref(read<ptr<@type[[TYPE_buffer]]>>(%[[VALUE_b]])))), const<i32>(0)))), const<i32>(0));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
