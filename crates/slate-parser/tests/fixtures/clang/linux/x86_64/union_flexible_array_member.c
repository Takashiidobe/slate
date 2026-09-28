union Tail { int i; char tail[]; };
union Lead { char lead[]; int i; };
union Only { char only[]; };
union Two { int i; char a[]; char b[]; };
union Wide { char c; double d[]; };
union Packed { char c; double d[]; } __attribute__((packed));

unsigned long sizes[] = {
  sizeof(union Tail), sizeof(union Lead), sizeof(union Only),
  sizeof(union Two), sizeof(union Wide), sizeof(union Packed),
};
unsigned long aligns[] = { _Alignof(union Tail), _Alignof(union Wide) };

union Tail scalar = { 5 };
union Tail short_tail = { .tail = { 1, 2 } };
union Tail long_tail = { .tail = { 1, 2, 3, 4, 5, 6 } };

int read_tail(union Tail *p) { return p->tail[1]; }

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES IR

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
// IR-NEXT:     type @type0 Tail = union {
// IR-NEXT:         field0 i: i32;
// IR-NEXT:         field1 tail: array<i8, incomplete>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type1 Lead = union {
// IR-NEXT:         field0 lead: array<i8, incomplete>;
// IR-NEXT:         field1 i: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type2 Only = union {
// IR-NEXT:         field0 only: array<i8, incomplete>;
// IR-NEXT:     } [size=0, align=1, offsets=[0]];
// IR-NEXT:     type @type3 Two = union {
// IR-NEXT:         field0 i: i32;
// IR-NEXT:         field1 a: array<i8, incomplete>;
// IR-NEXT:         field2 b: array<i8, incomplete>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0, 0]];
// IR-NEXT:     type @type4 Wide = union {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 d: array<f64, incomplete>;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// IR-NEXT:     type @type5 Packed = union {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 d: array<f64, incomplete>;
// IR-NEXT:     } [size=1, align=1, offsets=[0, 0]];
// IR-NEXT:     global %6 sizes: array<u64, 6> [storage=static] [align=16] = aggregate<array<u64, 6>, zero_fill=false>(index0 = const<u64>(4), index1 = const<u64>(4), index2 = const<u64>(0), index3 = const<u64>(4), index4 = const<u64>(8), index5 = const<u64>(1)) [linkage=external];
// IR-NEXT:     global %7 aligns: array<u64, 2> [storage=static] [align=16] = aggregate<array<u64, 2>, zero_fill=false>(index0 = const<u64>(4), index1 = const<u64>(8)) [linkage=external];
// IR-NEXT:     global %8 scalar: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(5)) [linkage=external];
// IR-NEXT:     global %9 short_tail: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field1 = aggregate<array<i8, 2>, zero_fill=false>(index0 = truncate<i8>(const<i32>(1)), index1 = truncate<i8>(const<i32>(2)))) [linkage=external];
// IR-NEXT:     global %10 long_tail: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field1 = aggregate<array<i8, 6>, zero_fill=false>(index0 = truncate<i8>(const<i32>(1)), index1 = truncate<i8>(const<i32>(2)), index2 = truncate<i8>(const<i32>(3)), index3 = truncate<i8>(const<i32>(4)), index4 = truncate<i8>(const<i32>(5)), index5 = truncate<i8>(const<i32>(6)))) [linkage=external];
// IR-NEXT:     fn %11 @read_tail(%12 p: ptr<@type0>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return widen<i32>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false>(array_decay<ptr<i8>, length=None>(field1(deref(read<ptr<@type0>>(%12)))), const<i32>(1)))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
