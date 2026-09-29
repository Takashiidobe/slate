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
// IR-NEXT:     fn %[[VALUE_switches:[0-9]+]] @switches(%[[VALUE_x:[0-9]+]] x: i16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         while %[[VALUE0:[0-9]+]] ne<i16>(read<i16>(%[[VALUE_x]]), const<i16>(0))
// IR-NEXT:             {
// IR-NEXT:                 switch %[[VALUE1:[0-9]+]] widen<i32, reason=promotion>(read<i16>(%[[VALUE_x]]))
// IR-NEXT:                     {
// IR-NEXT:                         case %[[VALUE1]] const<i32>(0):
// IR-NEXT:                             case %[[VALUE1]] const<i32>(1):
// IR-NEXT:                                 let %[[VALUE2:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_x]]);
// IR-NEXT:                                 let %[[VALUE3:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE2]])), const<i32>(1)));
// IR-NEXT:                                 write<i16>(%[[VALUE_x]], read<i16>(%[[VALUE3]]));
// IR-NEXT:                         case %[[VALUE1]] const<i32>(2) ... const<i32>(4):
// IR-NEXT:                             continue %[[VALUE0]];
// IR-NEXT:                         default %[[VALUE1]]:
// IR-NEXT:                             break %[[VALUE1]];
// IR-NEXT:                     }
// IR-NEXT:                 break %[[VALUE0]];
// IR-NEXT:             }
// IR-NEXT:         switch %[[VALUE4:[0-9]+]] widen<i32, reason=promotion>(read<i16>(%[[VALUE_x]]))
// IR-NEXT:             {
// IR-NEXT:                 case %[[VALUE4]] const<i32>(2):
// IR-NEXT:                     do %[[VALUE5:[0-9]+]]
// IR-NEXT:                         {
// IR-NEXT:                             let %[[VALUE6:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_x]]);
// IR-NEXT:                             let %[[VALUE7:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE6]])), const<i32>(1)));
// IR-NEXT:                             write<i16>(%[[VALUE_x]], read<i16>(%[[VALUE7]]));
// IR-NEXT:                             case %[[VALUE4]] const<i32>(5):
// IR-NEXT:                                 let %[[VALUE8:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_x]]);
// IR-NEXT:                                 let %[[VALUE9:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE8]])), const<i32>(1)));
// IR-NEXT:                                 write<i16>(%[[VALUE_x]], read<i16>(%[[VALUE9]]));
// IR-NEXT:                         }
// IR-NEXT:                     while ne<i16>(read<i16>(%[[VALUE_x]]), const<i16>(0));
// IR-NEXT:                 default %[[VALUE4]]:
// IR-NEXT:                     switch %[[VALUE10:[0-9]+]] widen<i32, reason=promotion>(read<i16>(%[[VALUE_x]]))
// IR-NEXT:                         {
// IR-NEXT:                             case %[[VALUE10]] const<i32>(2):
// IR-NEXT:                                 break %[[VALUE10]];
// IR-NEXT:                             default %[[VALUE10]]:
// IR-NEXT:                                 break %[[VALUE10]];
// IR-NEXT:                         }
// IR-NEXT:             }
// IR-NEXT:         return widen<i32, reason=return>(read<i16>(%[[VALUE_x]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
