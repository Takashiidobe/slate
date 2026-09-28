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
// IR-NEXT:     type @type0 Pair = struct {
// IR-NEXT:         field0 x: i16;
// IR-NEXT:         field1 y: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     fn %0 @inc(%1 s: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return truncate<i16, reason=return, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(1)));
// IR-NEXT:     }
// IR-NEXT:     fn %3 @consume(%21 x: i16, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %4 @decisions(%5 s: i16, %6 wide: u64, %7 f: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %8 byte: u8 [storage=automatic] = truncate<u8, reason=assign, fits=unknown>(read<u64>(%6));
// IR-NEXT:         let %9 truth: bool [storage=automatic] = ne<f32, reason=assign, exceptions=ignore>(read<f32>(%7), const<f32>(0.0));
// IR-NEXT:         write<i16>(%5, reinterpret<i16, reason=assign, fits=unknown>(widen<u16, reason=assign>(read<u8>(%8))));
// IR-NEXT:         call<i32, signature=fn(i16, ...) -> i32>(%3, reinterpret<i16, reason=arg, fits=unknown>(truncate<u16, reason=arg, fits=unknown>(read<u64>(%6))), widen<i32, reason=vararg>(read<i16>(%5)), float_widen<f64, reason=vararg>(read<f32>(%7)), from_bool<i32, reason=vararg>(read<bool>(%9)));
// IR-NEXT:         let %22: i32 [synthetic];
// IR-NEXT:         if read<bool>(%9)
// IR-NEXT:             read<i16>(%5);
// IR-NEXT:             write<i32>(%22, add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%5)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%8)))));
// IR-NEXT:         else
// IR-NEXT:             write<i32>(%22, neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%5))));
// IR-NEXT:         return read<i32>(%22);
// IR-NEXT:     }
// IR-NEXT:     fn %10 @sequencing(%11 p: ptr<i32>, %12 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %23: i32 [synthetic] = read<i32>(%12);
// IR-NEXT:         let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// IR-NEXT:         write<i32>(%12, read<i32>(%24));
// IR-NEXT:         let %25: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%11), read<i32>(%23));
// IR-NEXT:         let %26: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%25)));
// IR-NEXT:         let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(2));
// IR-NEXT:         write<i32>(deref(read<ptr<i32>>(%25)), read<i32>(%27));
// IR-NEXT:         write<i32>(%12, const<i32>(0));
// IR-NEXT:         let %28: bool [synthetic];
// IR-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%11), read<i32>(%12)))), const<i32>(0))
// IR-NEXT:             write<i32>(%12, const<i32>(3));
// IR-NEXT:             write<bool>(%28, ne<i32>(const<i32>(3), const<i32>(0)));
// IR-NEXT:         else
// IR-NEXT:             write<bool>(%28, const<bool>(false));
// IR-NEXT:         let %29: bool [synthetic];
// IR-NEXT:         if read<bool>(%28)
// IR-NEXT:             write<bool>(%29, const<bool>(true));
// IR-NEXT:         else
// IR-NEXT:             let %30: i32 [synthetic];
// IR-NEXT:             if ne<i32>(read<i32>(%12), const<i32>(0))
// IR-NEXT:                 let %31: i32 [synthetic] = read<i32>(%12);
// IR-NEXT:                 let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), const<i32>(1));
// IR-NEXT:                 write<i32>(%12, read<i32>(%32));
// IR-NEXT:                 write<i32>(%30, read<i32>(%32));
// IR-NEXT:             else
// IR-NEXT:                 let %33: i32 [synthetic] = read<i32>(%12);
// IR-NEXT:                 let %34: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// IR-NEXT:                 write<i32>(%12, read<i32>(%34));
// IR-NEXT:                 write<i32>(%30, read<i32>(%33));
// IR-NEXT:             write<bool>(%29, ne<i32>(read<i32>(%30), const<i32>(0)));
// IR-NEXT:         return from_bool<i32, reason=return>(read<bool>(%29));
// IR-NEXT:     }
// IR-NEXT:     fn %14 @fields(%15 p: ptr<@type0>, %16 q: @type0) -> i32 [linkage=external] [abi=sysv64(scalar, native_c) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         write<i16>(field0(deref(read<ptr<@type0>>(%15))), truncate<i16, reason=assign, fits=unknown>(read<i32>(field1(%16))));
// IR-NEXT:         return add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(field0(%16))), read<i32>(field1(deref(read<ptr<@type0>>(%15)))));
// IR-NEXT:     }
// IR-NEXT:     fn %17 @pointers(%18 p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %19 a: array<i32, 3> [storage=automatic];
// IR-NEXT:         let %20 q: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// IR-NEXT:         write<ptr<i32>>(%20, array_decay<ptr<i32>, length=Some(3)>(%19));
// IR-NEXT:         return conditional<i32>(ne<ptr<i32>>(read<ptr<i32>>(%18), null<ptr<i32>>), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%20), const<i32>(1)))), from_bool<i32, reason=promotion>(not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%18), null<ptr<i32>>))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
