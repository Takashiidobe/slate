// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

enum Mode { Idle, Busy };

struct Inner { int x; int y; };

union Value { int integer; float real; };

struct Outer {
    struct Tagged { int declared_only; };
    struct Inner inner;
    union Value value;
    struct { int promoted; union { int left; float right; }; };
};

struct Flags {
    unsigned low : 3;
    unsigned high : 5;
    signed narrow : 4;
    unsigned : 0;
    unsigned long wide : 40;
    _Bool present : 1;
    enum Mode mode : 2;
    int plain;
};

int read_members(struct Outer *o) {
    return o->inner.x + o->value.integer + o->promoted + o->left;
}

void write_members(struct Outer *o, int v) {
    o->inner.y = v;
    o->value.integer = v;
    o->right = 1.0f;
}

int read_bits(struct Flags *f) {
    return f->low + f->high + f->narrow + f->plain;
}

unsigned long wide_bits(struct Flags *f) {
    f->wide = 5;
    f->wide += 1;
    return f->wide;
}

int bit_types(struct Flags *f) {
    return _Generic(f->low, unsigned: 1, int: 2)
         + _Generic(f->present, _Bool: 4, int: 8)
         + (int)sizeof(f->plain)
         + (int)_Alignof(f->plain);
}

void update_bits(struct Flags *f, unsigned v) {
    f->low = v;
    f->high += 1;
    f->narrow++;
    f->present = 1;
    f->mode = Busy;
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
// IR-NEXT:     type @type0 Mode = enum : i32 {
// IR-NEXT:         %0 Idle = const<i32>(0);
// IR-NEXT:         %1 Busy = const<i32>(1);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type1 Inner = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:         field1 y: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type2 Value = union {
// IR-NEXT:         field0 integer: i32;
// IR-NEXT:         field1 real: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type3 Outer = struct {
// IR-NEXT:         field0 inner: @type1;
// IR-NEXT:         field1 value: @type2;
// IR-NEXT:         field2 <anonymous>: @type5;
// IR-NEXT:     } [size=20, align=4, offsets=[0, 8, 12]];
// IR-NEXT:     type @type4 Tagged = struct {
// IR-NEXT:         field0 declared_only: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type5 = struct {
// IR-NEXT:         field0 promoted: i32;
// IR-NEXT:         field1 <anonymous>: @type6;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type6 = union {
// IR-NEXT:         field0 left: i32;
// IR-NEXT:         field1 right: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type7 Flags = struct {
// IR-NEXT:         field0 low: u32 : 3;
// IR-NEXT:         field1 high: u32 : 5;
// IR-NEXT:         field2 narrow: i32 : 4;
// IR-NEXT:         field3 <anonymous>: u32 : 0;
// IR-NEXT:         field4 wide: u64 : 40;
// IR-NEXT:         field5 present: bool : 1;
// IR-NEXT:         field6 mode: @type0 : 2;
// IR-NEXT:         field7 plain: i32;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 0, 1, 4, 8, 13, 13, 16], bit_offsets=[Some(0), Some(3), Some(8), Some(32), Some(64), Some(104), Some(105), None], bit_units=[(0, 2), (8, 6)], field_units=[Some(0), Some(0), Some(0), None, Some(1), Some(1), Some(1), None]];
// IR-NEXT:     fn %10 @read_members(%11 o: ptr<@type3>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field0(field0(deref(read<ptr<@type3>>(%11))))), read<i32>(field0(field1(deref(read<ptr<@type3>>(%11)))))), read<i32>(field0(field2(deref(read<ptr<@type3>>(%11)))))), read<i32>(field0(field1(field2(deref(read<ptr<@type3>>(%11)))))));
// IR-NEXT:     }
// IR-NEXT:     fn %12 @write_members(%13 o: ptr<@type3>, %14 v: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         write<i32>(field1(field0(deref(read<ptr<@type3>>(%13)))), read<i32>(%14));
// IR-NEXT:         write<i32>(field0(field1(deref(read<ptr<@type3>>(%13)))), read<i32>(%14));
// IR-NEXT:         write<f32>(field1(field1(field2(deref(read<ptr<@type3>>(%13))))), const<f32>(1.0));
// IR-NEXT:     }
// IR-NEXT:     fn %15 @read_bits(%16 f: ptr<@type7>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..2, bits=0..3>(deref(read<ptr<@type7>>(%16))))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..2, bits=3..8>(deref(read<ptr<@type7>>(%16)))))), read<i32>(bitfield2<unit=0, bytes=0..2, bits=8..12>(deref(read<ptr<@type7>>(%16))))), read<i32>(field7(deref(read<ptr<@type7>>(%16)))));
// IR-NEXT:     }
// IR-NEXT:     fn %17 @wide_bits(%18 f: ptr<@type7>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         write<u64>(bitfield4<unit=1, bytes=8..14, bits=0..40>(deref(read<ptr<@type7>>(%18))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(5))));
// IR-NEXT:         let %24: ptr<@type7> [synthetic] = read<ptr<@type7>>(%18);
// IR-NEXT:         let %25: u64 [synthetic] = read<u64>(bitfield4<unit=1, bytes=8..14, bits=0..40>(deref(read<ptr<@type7>>(%24))));
// IR-NEXT:         let %26: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%25), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// IR-NEXT:         write<u64>(bitfield4<unit=1, bytes=8..14, bits=0..40>(deref(read<ptr<@type7>>(%24))), read<u64>(%26));
// IR-NEXT:         return read<u64>(bitfield4<unit=1, bytes=8..14, bits=0..40>(deref(read<ptr<@type7>>(%18))));
// IR-NEXT:     }
// IR-NEXT:     fn %19 @bit_types(%20 f: ptr<@type7>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(1), const<i32>(4)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(4)))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(4))));
// IR-NEXT:     }
// IR-NEXT:     fn %21 @update_bits(%22 f: ptr<@type7>, %23 v: u32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..2, bits=0..3>(deref(read<ptr<@type7>>(%22))), read<u32>(%23));
// IR-NEXT:         let %27: ptr<@type7> [synthetic] = read<ptr<@type7>>(%22);
// IR-NEXT:         let %28: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..2, bits=3..8>(deref(read<ptr<@type7>>(%27))));
// IR-NEXT:         let %29: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%28)), const<i32>(1)));
// IR-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..2, bits=3..8>(deref(read<ptr<@type7>>(%27))), read<u32>(%29));
// IR-NEXT:         let %30: ptr<@type7> [synthetic] = read<ptr<@type7>>(%22);
// IR-NEXT:         let %31: i32 [synthetic] = read<i32>(bitfield2<unit=0, bytes=0..2, bits=8..12>(deref(read<ptr<@type7>>(%30))));
// IR-NEXT:         let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), const<i32>(1));
// IR-NEXT:         write<i32>(bitfield2<unit=0, bytes=0..2, bits=8..12>(deref(read<ptr<@type7>>(%30))), read<i32>(%32));
// IR-NEXT:         write<bool>(bitfield5<unit=1, bytes=8..14, bits=40..41>(deref(read<ptr<@type7>>(%22))), ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// IR-NEXT:         write<@type0>(bitfield6<unit=1, bytes=8..14, bits=41..43>(deref(read<ptr<@type7>>(%22))), int_to_enum<@type0, reason=assign>(const<i32>(1)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
