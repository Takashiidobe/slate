// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

int switches(short x) {
    while (x) {
        switch (x) {
        case 0: case 1: x++;
        case 2 ... 4: continue;
        default: break;
        }
        break;
    }
    switch (x) {
    case 1 ? 2 : 3: do { x--; case 5: x--; } while (x);
    default: switch (x) { case 2: break; default: break; }
    }
    return x;
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
// IR-NEXT:     fn %0 @switches(%1 x: i16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         while %2 ne<i16>(read<i16>(%1), const<i16>(0))
// IR-NEXT:             {
// IR-NEXT:                 switch %3 widen<i32, reason=promotion>(read<i16>(%1))
// IR-NEXT:                     {
// IR-NEXT:                         case %3 const<i32>(0):
// IR-NEXT:                             case %3 const<i32>(1):
// IR-NEXT:                                 let %7: i16 [synthetic] = read<i16>(%1);
// IR-NEXT:                                 let %8: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%7)), const<i32>(1)));
// IR-NEXT:                                 write<i16>(%1, read<i16>(%8));
// IR-NEXT:                         case %3 const<i32>(2) ... const<i32>(4):
// IR-NEXT:                             continue %2;
// IR-NEXT:                         default %3:
// IR-NEXT:                             break %3;
// IR-NEXT:                     }
// IR-NEXT:                 break %2;
// IR-NEXT:             }
// IR-NEXT:         switch %4 widen<i32, reason=promotion>(read<i16>(%1))
// IR-NEXT:             {
// IR-NEXT:                 case %4 const<i32>(2):
// IR-NEXT:                     do %5
// IR-NEXT:                         {
// IR-NEXT:                             let %9: i16 [synthetic] = read<i16>(%1);
// IR-NEXT:                             let %10: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%9)), const<i32>(1)));
// IR-NEXT:                             write<i16>(%1, read<i16>(%10));
// IR-NEXT:                             case %4 const<i32>(5):
// IR-NEXT:                                 let %11: i16 [synthetic] = read<i16>(%1);
// IR-NEXT:                                 let %12: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%11)), const<i32>(1)));
// IR-NEXT:                                 write<i16>(%1, read<i16>(%12));
// IR-NEXT:                         }
// IR-NEXT:                     while ne<i16>(read<i16>(%1), const<i16>(0));
// IR-NEXT:                 default %4:
// IR-NEXT:                     switch %6 widen<i32, reason=promotion>(read<i16>(%1))
// IR-NEXT:                         {
// IR-NEXT:                             case %6 const<i32>(2):
// IR-NEXT:                                 break %6;
// IR-NEXT:                             default %6:
// IR-NEXT:                                 break %6;
// IR-NEXT:                         }
// IR-NEXT:             }
// IR-NEXT:         return widen<i32, reason=return>(read<i16>(%1));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
