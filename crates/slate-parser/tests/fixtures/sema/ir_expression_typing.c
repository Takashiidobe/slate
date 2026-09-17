// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

short inc(short s) { return s + 1; }
extern int consume(short x, ...);
int decisions(short s, unsigned long wide, float f) {
    unsigned char byte = wide;
    _Bool truth = f;
    s = byte;
    consume(wide, s, f, truth);
    return truth ? (s, s + byte) : -s;
}
int sequencing(int *p, int i) {
    p[i++] += 2;
    return (i = 0, p[i] && (i = 3)) || (i ? ++i : i--);
}
struct Pair { short x; int y; };
int fields(struct Pair *p, struct Pair q) {
    p->x = q.y;
    return q.x + p->y;
}
int pointers(int *p) {
    int a[3];
    int *q = 0;
    q = a;
    return p != 0 ? q[1] : !p;
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
// IR-NEXT:     type @type0 = fn(i16) -> i16;
// IR-NEXT:     type @type1 = fn(i16, ...) -> i32;
// IR-NEXT:     type @type2 = fn(i16, u64, f32) -> i32;
// IR-NEXT:     type @type3 = ptr<i32>;
// IR-NEXT:     type @type4 = fn(@type3, i32) -> i32;
// IR-NEXT:     type @type5 = ptr<i32>;
// IR-NEXT:     type @type6 Pair = struct {
// IR-NEXT:         field0 x: i16;
// IR-NEXT:         field1 y: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type7 = ptr<@type6>;
// IR-NEXT:     type @type8 = fn(@type7, @type6) -> i32;
// IR-NEXT:     type @type9 = ptr<@type6>;
// IR-NEXT:     type @type10 = ptr<i32>;
// IR-NEXT:     type @type11 = fn(@type10) -> i32;
// IR-NEXT:     type @type12 = ptr<i32>;
// IR-NEXT:     type @type13 = array<i32, 3>;
// IR-NEXT:     type @type14 = ptr<i32>;
// IR-NEXT:     fn %0 @inc(%1 s: i16) -> i16 [linkage=external] {
// IR-NEXT:         return truncate<i16, reason=return, fits=unknown>(add<i32, overflow=undefined>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(1)));
// IR-NEXT:     }
// IR-NEXT:     fn %2 @consume(%20 x: i16, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %3 @decisions(%4 s: i16, %5 wide: u64, %6 f: f32) -> i32 [linkage=external] {
// IR-NEXT:         let %7 byte: u8 [storage=automatic] = truncate<u8, reason=assign, fits=unknown>(read<u64>(%5));
// IR-NEXT:         let %8 truth: bool [storage=automatic] = ne<f32, reason=assign, exceptions=ignore>(read<f32>(%6), const<f32>(0.0));
// IR-NEXT:         store<i16>(%4, reinterpret<i16, reason=assign, fits=unknown>(widen<u16, reason=assign>(read<u8>(%7))));
// IR-NEXT:         call<i32>(%2, reinterpret<i16, reason=arg, fits=unknown>(truncate<u16, reason=arg, fits=unknown>(read<u64>(%5))), widen<i32, reason=vararg>(read<i16>(%4)), float_widen<f64, reason=vararg>(read<f32>(%6)), from_bool<i32, reason=vararg>(read<bool>(%8)));
// IR-NEXT:         return conditional<i32>(read<bool>(%8), sequence<i32>(read<i16>(%4), add<i32, overflow=undefined>(widen<i32, reason=promotion>(read<i16>(%4)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%7))))), neg<i32, overflow=undefined>(widen<i32, reason=promotion>(read<i16>(%4))));
// IR-NEXT:     }
// IR-NEXT:     fn %9 @sequencing(%10 p: @type5, %11 i: i32) -> i32 [linkage=external] {
// IR-NEXT:         update<i32, result=new>(index(read<@type5>(%10), update<i32, result=old>(%11, add<i32, overflow=undefined>(old<i32>, const<i32>(1)))), add<i32, overflow=undefined>(old<i32>, const<i32>(2)));
// IR-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(sequence<bool>(store<i32>(%11, const<i32>(0)), logical_and<bool>(ne<i32>(read<i32>(index(read<@type5>(%10), read<i32>(%11))), const<i32>(0)), ne<i32>(store<i32>(%11, const<i32>(3)), const<i32>(0)))), ne<i32>(conditional<i32>(ne<i32>(read<i32>(%11), const<i32>(0)), update<i32, result=new>(%11, add<i32, overflow=undefined>(old<i32>, const<i32>(1))), update<i32, result=old>(%11, sub<i32, overflow=undefined>(old<i32>, const<i32>(1)))), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %13 @fields(%14 p: @type9, %15 q: @type6) -> i32 [linkage=external] {
// IR-NEXT:         store<i16>(field0(deref(read<@type9>(%14))), truncate<i16, reason=assign, fits=unknown>(read<i32>(field1(%15))));
// IR-NEXT:         return add<i32, overflow=undefined>(widen<i32, reason=promotion>(read<i16>(field0(%15))), read<i32>(field1(deref(read<@type9>(%14)))));
// IR-NEXT:     }
// IR-NEXT:     fn %16 @pointers(%17 p: @type12) -> i32 [linkage=external] {
// IR-NEXT:         let %18 a: @type13 [storage=automatic];
// IR-NEXT:         let %19 q: @type14 [storage=automatic] = null<@type14>;
// IR-NEXT:         store<@type14>(%19, pointer_cast<@type14, reason=assign>(array_decay<@type3, length=Some(3)>(%18)));
// IR-NEXT:         return conditional<i32>(ne<@type12>(read<@type12>(%17), null<@type12>), read<i32>(index(read<@type14>(%19), const<i32>(1))), from_bool<i32, reason=promotion>(not<bool>(ne<@type12>(read<@type12>(%17), null<@type12>))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
