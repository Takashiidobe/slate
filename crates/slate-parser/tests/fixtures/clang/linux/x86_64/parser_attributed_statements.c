// SLATE-FILECHECK-DEFINES AST
// SLATE-FILECHECK-STD AST gnu23

int attributed(int x) {
    [[vendor::hint]] if (x) [[vendor::hint]] return 1; else return 2;
    [[vendor::hint]] while (x) { x--; }
    [[vendor::hint]] for (; x; x--) ;
    [[vendor::hint]] goto done;
    [[vendor::hint]] done: x++;
    switch (x) {
    [[vendor::hint]] case 1: x++;
    [[fallthrough]];
    default: break;
    }
    [[maybe_unused]] int local = x;
    return local;
}

// SLATE-FILECHECK-BEGIN AST
// AST: module {
// AST-NEXT:     target "x86_64-unknown-linux-gnu" {
// AST-NEXT:         endian = little;
// AST-NEXT:         pointer [size=8, align=8];
// AST-NEXT:         stack_alignment = 16;
// AST-NEXT:         long_double = f80;
// AST-NEXT:         storage bool [size=1, align=1];
// AST-NEXT:         storage i8, u8 [size=1, align=1];
// AST-NEXT:         storage i16, u16 [size=2, align=2];
// AST-NEXT:         storage i32, u32 [size=4, align=4];
// AST-NEXT:         storage i64, u64 [size=8, align=8];
// AST-NEXT:         storage i128, u128 [size=16, align=16];
// AST-NEXT:         storage bf16 [size=2, align=2];
// AST-NEXT:         storage f16 [size=2, align=2];
// AST-NEXT:         storage f32 [size=4, align=4];
// AST-NEXT:         storage f64 [size=8, align=8];
// AST-NEXT:         storage f80 [size=16, align=16];
// AST-NEXT:         storage f128 [size=16, align=16];
// AST-NEXT:         storage d32 [size=4, align=4];
// AST-NEXT:         storage d64 [size=8, align=8];
// AST-NEXT:         storage d128 [size=16, align=16];
// AST-NEXT:     }
// AST-NEXT:     fn %[[VALUE_attributed:[0-9]+]] @attributed(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// AST-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// AST-NEXT:             return const<i32>(1);
// AST-NEXT:         else
// AST-NEXT:             return const<i32>(2);
// AST-NEXT:         while %[[VALUE0:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// AST-NEXT:             {
// AST-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// AST-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// AST-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE2]]));
// AST-NEXT:             }
// AST-NEXT:         for %[[VALUE3:[0-9]+]]
// AST-NEXT:             init:
// AST-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// AST-NEXT:             increment: {
// AST-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// AST-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// AST-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE5]]));
// AST-NEXT:                 yield void;
// AST-NEXT:             }
// AST-NEXT:             body:
// AST-NEXT:                 ;
// AST-NEXT:         goto %[[VALUE_done:[0-9]+]];
// AST-NEXT:         label %[[VALUE_done]] done:
// AST-NEXT:             let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// AST-NEXT:             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// AST-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE7]]));
// AST-NEXT:         switch %[[VALUE8:[0-9]+]] read<i32>(%[[VALUE_x]])
// AST-NEXT:             {
// AST-NEXT:                 case %[[VALUE8]] const<i32>(1):
// AST-NEXT:                     let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// AST-NEXT:                     let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// AST-NEXT:                     write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE10]]));
// AST-NEXT:                 ;
// AST-NEXT:                 default %[[VALUE8]]:
// AST-NEXT:                     break %[[VALUE8]];
// AST-NEXT:             }
// AST-NEXT:         let %[[VALUE_local:[0-9]+]] local: i32 [storage=automatic] = read<i32>(%[[VALUE_x]]);
// AST-NEXT:         return read<i32>(%[[VALUE_local]]);
// AST-NEXT:     }
// AST-NEXT: }
// SLATE-FILECHECK-END AST
