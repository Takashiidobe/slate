int g,
    *h;

struct point {
    int x,
        y;
    unsigned flags : 3;
};

void f(int a,
       char *b) {
    int c = 1,
        d;
    for (int i = 0,
             j = 1; i < j; i++) {
        c += i;
    }
    (void)a;
    (void)b;
    (void)d;
}

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 point = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:         field2 flags: u32 : 3;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8], bit_offsets=[None, None, Some(64)], bit_units=[(8, 1)], field_units=[None, None, Some(0)]];
// DEFAULT-NEXT:     global %0 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 h: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @f(%4 a: i32, %5 b: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 c: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %7 d: i32 [storage=automatic];
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %8 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %9 j: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), read<i32>(%9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%12));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %13: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                     let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), read<i32>(%8));
// DEFAULT-NEXT:                     write<i32>(%6, read<i32>(%14));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         read<i32>(%4);
// DEFAULT-NEXT:         read<ptr<i8>>(%5);
// DEFAULT-NEXT:         read<i32>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
