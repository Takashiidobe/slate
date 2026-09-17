// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

int loops(int x) {
    if (x) x--; else { x++; }
    while (x) { if (x == 2) break; x--; continue; }
    do x++; while (x < 3);
    for (int i = 0, j = 2; i < j; i++) { if (x) continue; break; }
    for (;;) ;
    return x ? x : 1;
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
// IR-NEXT:     fn %0 @loops(%1 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// IR-NEXT:             let %8: i32 [synthetic] = read<i32>(%1);
// IR-NEXT:             let %9: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// IR-NEXT:             write<i32>(%1, read<i32>(%9));
// IR-NEXT:         else
// IR-NEXT:             {
// IR-NEXT:                 let %10: i32 [synthetic] = read<i32>(%1);
// IR-NEXT:                 let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// IR-NEXT:                 write<i32>(%1, read<i32>(%11));
// IR-NEXT:             }
// IR-NEXT:         while %4 ne<i32>(read<i32>(%1), const<i32>(0))
// IR-NEXT:             {
// IR-NEXT:                 if eq<i32>(read<i32>(%1), const<i32>(2))
// IR-NEXT:                     break %4;
// IR-NEXT:                 let %12: i32 [synthetic] = read<i32>(%1);
// IR-NEXT:                 let %13: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// IR-NEXT:                 write<i32>(%1, read<i32>(%13));
// IR-NEXT:                 continue %4;
// IR-NEXT:             }
// IR-NEXT:         do %5
// IR-NEXT:             let %14: i32 [synthetic] = read<i32>(%1);
// IR-NEXT:             let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// IR-NEXT:             write<i32>(%1, read<i32>(%15));
// IR-NEXT:         while lt<i32>(read<i32>(%1), const<i32>(3));
// IR-NEXT:         for %6
// IR-NEXT:             init:
// IR-NEXT:                 let %2 i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:                 let %3 j: i32 [storage=automatic] = const<i32>(2);
// IR-NEXT:             condition: lt<i32>(read<i32>(%2), read<i32>(%3))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %16: i32 [synthetic] = read<i32>(%2);
// IR-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// IR-NEXT:                 write<i32>(%2, read<i32>(%17));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     if ne<i32>(read<i32>(%1), const<i32>(0))
// IR-NEXT:                         continue %6;
// IR-NEXT:                     break %6;
// IR-NEXT:                 }
// IR-NEXT:         for %7
// IR-NEXT:             init:
// IR-NEXT:             condition: omitted
// IR-NEXT:             increment: omitted
// IR-NEXT:             body:
// IR-NEXT:                 ;
// IR-NEXT:         return conditional<i32>(ne<i32>(read<i32>(%1), const<i32>(0)), read<i32>(%1), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
