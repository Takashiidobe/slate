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
// IR-NEXT:     fn %[[VALUE_loops:[0-9]+]] @loops(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// IR-NEXT:             let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// IR-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// IR-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE1]]));
// IR-NEXT:         else
// IR-NEXT:             {
// IR-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// IR-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE3]]));
// IR-NEXT:             }
// IR-NEXT:         while %[[VALUE4:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// IR-NEXT:             {
// IR-NEXT:                 if eq<i32>(read<i32>(%[[VALUE_x]]), const<i32>(2))
// IR-NEXT:                     break %[[VALUE4]];
// IR-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// IR-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE6]]));
// IR-NEXT:                 continue %[[VALUE4]];
// IR-NEXT:             }
// IR-NEXT:         do %[[VALUE7:[0-9]+]]
// IR-NEXT:             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// IR-NEXT:             let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// IR-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE9]]));
// IR-NEXT:         while lt<i32>(read<i32>(%[[VALUE_x]]), const<i32>(3));
// IR-NEXT:         for %[[VALUE10:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:                 let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(2);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// IR-NEXT:                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE12]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// IR-NEXT:                         continue %[[VALUE10]];
// IR-NEXT:                     break %[[VALUE10]];
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE13:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:             condition: omitted
// IR-NEXT:             increment: omitted
// IR-NEXT:             body:
// IR-NEXT:                 ;
// IR-NEXT:         return conditional<i32>(ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0)), read<i32>(%[[VALUE_x]]), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
