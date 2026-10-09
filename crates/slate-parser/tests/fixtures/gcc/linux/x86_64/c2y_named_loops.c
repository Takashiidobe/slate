void named_loops(int n) {
outer:
    for (int i = 0; i < n; ++i) {
inner:
        while (n) {
            if (n == 1) continue outer;
            if (n == 2) break inner;
            break;
        }
        continue;
    }
again:
    do {
        if (n) continue again;
        break again;
    } while (n);
choice:
    switch (n) {
    case 0: break choice;
    default: break;
    }
}

// SLATE-FILECHECK-DEFINES C2Y
// SLATE-FILECHECK-STD C2Y c2y
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-DEFINES GNU17
// SLATE-FILECHECK-STD GNU17 gnu17

// SLATE-FILECHECK-BEGIN C2Y
// C2Y: module {
// C2Y-NEXT:     target "x86_64-unknown-linux-gnu" {
// C2Y-NEXT:         endian = little;
// C2Y-NEXT:         pointer [size=8, align=8];
// C2Y-NEXT:         stack_alignment = 16;
// C2Y-NEXT:         long_double = f80;
// C2Y-NEXT:         storage bool [size=1, align=1];
// C2Y-NEXT:         storage i8, u8 [size=1, align=1];
// C2Y-NEXT:         storage i16, u16 [size=2, align=2];
// C2Y-NEXT:         storage i32, u32 [size=4, align=4];
// C2Y-NEXT:         storage i64, u64 [size=8, align=8];
// C2Y-NEXT:         storage i128, u128 [size=16, align=16];
// C2Y-NEXT:         storage bf16 [size=2, align=2];
// C2Y-NEXT:         storage f16 [size=2, align=2];
// C2Y-NEXT:         storage f32 [size=4, align=4];
// C2Y-NEXT:         storage f64 [size=8, align=8];
// C2Y-NEXT:         storage f80 [size=16, align=16];
// C2Y-NEXT:         storage f128 [size=16, align=16];
// C2Y-NEXT:         storage d32 [size=4, align=4];
// C2Y-NEXT:         storage d64 [size=8, align=8];
// C2Y-NEXT:         storage d128 [size=16, align=16];
// C2Y-NEXT:     }
// C2Y-NEXT:     fn %[[VALUE_named_loops:[0-9]+]] @named_loops(%[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// C2Y-NEXT:         label %[[VALUE_outer:[0-9]+]] outer:
// C2Y-NEXT:             for %[[VALUE0:[0-9]+]]
// C2Y-NEXT:                 init:
// C2Y-NEXT:                     let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// C2Y-NEXT:                 condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_n]]))
// C2Y-NEXT:                 increment: {
// C2Y-NEXT:                     let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// C2Y-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// C2Y-NEXT:                     write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// C2Y-NEXT:                     yield void;
// C2Y-NEXT:                 }
// C2Y-NEXT:                 body:
// C2Y-NEXT:                     {
// C2Y-NEXT:                         label %[[VALUE_inner:[0-9]+]] inner:
// C2Y-NEXT:                             while %[[VALUE3:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0))
// C2Y-NEXT:                                 {
// C2Y-NEXT:                                     if eq<i32>(read<i32>(%[[VALUE_n]]), const<i32>(1))
// C2Y-NEXT:                                         continue %[[VALUE0]];
// C2Y-NEXT:                                     if eq<i32>(read<i32>(%[[VALUE_n]]), const<i32>(2))
// C2Y-NEXT:                                         break %[[VALUE3]];
// C2Y-NEXT:                                     break %[[VALUE3]];
// C2Y-NEXT:                                 }
// C2Y-NEXT:                         continue %[[VALUE0]];
// C2Y-NEXT:                     }
// C2Y-NEXT:         label %[[VALUE_again:[0-9]+]] again:
// C2Y-NEXT:             do %[[VALUE4:[0-9]+]]
// C2Y-NEXT:                 {
// C2Y-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0))
// C2Y-NEXT:                         continue %[[VALUE4]];
// C2Y-NEXT:                     break %[[VALUE4]];
// C2Y-NEXT:                 }
// C2Y-NEXT:             while ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0));
// C2Y-NEXT:         label %[[VALUE_choice:[0-9]+]] choice:
// C2Y-NEXT:             switch %[[VALUE5:[0-9]+]] read<i32>(%[[VALUE_n]])
// C2Y-NEXT:                 {
// C2Y-NEXT:                     case %[[VALUE5]] const<i32>(0):
// C2Y-NEXT:                         break %[[VALUE5]];
// C2Y-NEXT:                     default %[[VALUE5]]:
// C2Y-NEXT:                         break %[[VALUE5]];
// C2Y-NEXT:                 }
// C2Y-NEXT:     }
// C2Y-NEXT: }
// SLATE-FILECHECK-END C2Y
// SLATE-FILECHECK-BEGIN C23
// C23: module {
// C23-NEXT:     target "x86_64-unknown-linux-gnu" {
// C23-NEXT:         endian = little;
// C23-NEXT:         pointer [size=8, align=8];
// C23-NEXT:         stack_alignment = 16;
// C23-NEXT:         long_double = f80;
// C23-NEXT:         storage bool [size=1, align=1];
// C23-NEXT:         storage i8, u8 [size=1, align=1];
// C23-NEXT:         storage i16, u16 [size=2, align=2];
// C23-NEXT:         storage i32, u32 [size=4, align=4];
// C23-NEXT:         storage i64, u64 [size=8, align=8];
// C23-NEXT:         storage i128, u128 [size=16, align=16];
// C23-NEXT:         storage bf16 [size=2, align=2];
// C23-NEXT:         storage f16 [size=2, align=2];
// C23-NEXT:         storage f32 [size=4, align=4];
// C23-NEXT:         storage f64 [size=8, align=8];
// C23-NEXT:         storage f80 [size=16, align=16];
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:         storage d32 [size=4, align=4];
// C23-NEXT:         storage d64 [size=8, align=8];
// C23-NEXT:         storage d128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     fn %[[VALUE_named_loops:[0-9]+]] @named_loops(%[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// C23-NEXT:         label %[[VALUE_outer:[0-9]+]] outer:
// C23-NEXT:             for %[[VALUE0:[0-9]+]]
// C23-NEXT:                 init:
// C23-NEXT:                     let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// C23-NEXT:                 condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_n]]))
// C23-NEXT:                 increment: {
// C23-NEXT:                     let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// C23-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// C23-NEXT:                     write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// C23-NEXT:                     yield void;
// C23-NEXT:                 }
// C23-NEXT:                 body:
// C23-NEXT:                     {
// C23-NEXT:                         label %[[VALUE_inner:[0-9]+]] inner:
// C23-NEXT:                             while %[[VALUE3:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0))
// C23-NEXT:                                 {
// C23-NEXT:                                     if eq<i32>(read<i32>(%[[VALUE_n]]), const<i32>(1))
// C23-NEXT:                                         continue %[[VALUE0]];
// C23-NEXT:                                     if eq<i32>(read<i32>(%[[VALUE_n]]), const<i32>(2))
// C23-NEXT:                                         break %[[VALUE3]];
// C23-NEXT:                                     break %[[VALUE3]];
// C23-NEXT:                                 }
// C23-NEXT:                         continue %[[VALUE0]];
// C23-NEXT:                     }
// C23-NEXT:         label %[[VALUE_again:[0-9]+]] again:
// C23-NEXT:             do %[[VALUE4:[0-9]+]]
// C23-NEXT:                 {
// C23-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0))
// C23-NEXT:                         continue %[[VALUE4]];
// C23-NEXT:                     break %[[VALUE4]];
// C23-NEXT:                 }
// C23-NEXT:             while ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0));
// C23-NEXT:         label %[[VALUE_choice:[0-9]+]] choice:
// C23-NEXT:             switch %[[VALUE5:[0-9]+]] read<i32>(%[[VALUE_n]])
// C23-NEXT:                 {
// C23-NEXT:                     case %[[VALUE5]] const<i32>(0):
// C23-NEXT:                         break %[[VALUE5]];
// C23-NEXT:                     default %[[VALUE5]]:
// C23-NEXT:                         break %[[VALUE5]];
// C23-NEXT:                 }
// C23-NEXT:     }
// C23-NEXT: }
// SLATE-FILECHECK-END C23
// SLATE-FILECHECK-BEGIN GNU17
// GNU17: module {
// GNU17-NEXT:     target "x86_64-unknown-linux-gnu" {
// GNU17-NEXT:         endian = little;
// GNU17-NEXT:         pointer [size=8, align=8];
// GNU17-NEXT:         stack_alignment = 16;
// GNU17-NEXT:         long_double = f80;
// GNU17-NEXT:         storage bool [size=1, align=1];
// GNU17-NEXT:         storage i8, u8 [size=1, align=1];
// GNU17-NEXT:         storage i16, u16 [size=2, align=2];
// GNU17-NEXT:         storage i32, u32 [size=4, align=4];
// GNU17-NEXT:         storage i64, u64 [size=8, align=8];
// GNU17-NEXT:         storage i128, u128 [size=16, align=16];
// GNU17-NEXT:         storage bf16 [size=2, align=2];
// GNU17-NEXT:         storage f16 [size=2, align=2];
// GNU17-NEXT:         storage f32 [size=4, align=4];
// GNU17-NEXT:         storage f64 [size=8, align=8];
// GNU17-NEXT:         storage f80 [size=16, align=16];
// GNU17-NEXT:         storage f128 [size=16, align=16];
// GNU17-NEXT:         storage d32 [size=4, align=4];
// GNU17-NEXT:         storage d64 [size=8, align=8];
// GNU17-NEXT:         storage d128 [size=16, align=16];
// GNU17-NEXT:     }
// GNU17-NEXT:     fn %[[VALUE_named_loops:[0-9]+]] @named_loops(%[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// GNU17-NEXT:         label %[[VALUE_outer:[0-9]+]] outer:
// GNU17-NEXT:             for %[[VALUE0:[0-9]+]]
// GNU17-NEXT:                 init:
// GNU17-NEXT:                     let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// GNU17-NEXT:                 condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_n]]))
// GNU17-NEXT:                 increment: {
// GNU17-NEXT:                     let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// GNU17-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// GNU17-NEXT:                     write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// GNU17-NEXT:                     yield void;
// GNU17-NEXT:                 }
// GNU17-NEXT:                 body:
// GNU17-NEXT:                     {
// GNU17-NEXT:                         label %[[VALUE_inner:[0-9]+]] inner:
// GNU17-NEXT:                             while %[[VALUE3:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0))
// GNU17-NEXT:                                 {
// GNU17-NEXT:                                     if eq<i32>(read<i32>(%[[VALUE_n]]), const<i32>(1))
// GNU17-NEXT:                                         continue %[[VALUE0]];
// GNU17-NEXT:                                     if eq<i32>(read<i32>(%[[VALUE_n]]), const<i32>(2))
// GNU17-NEXT:                                         break %[[VALUE3]];
// GNU17-NEXT:                                     break %[[VALUE3]];
// GNU17-NEXT:                                 }
// GNU17-NEXT:                         continue %[[VALUE0]];
// GNU17-NEXT:                     }
// GNU17-NEXT:         label %[[VALUE_again:[0-9]+]] again:
// GNU17-NEXT:             do %[[VALUE4:[0-9]+]]
// GNU17-NEXT:                 {
// GNU17-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0))
// GNU17-NEXT:                         continue %[[VALUE4]];
// GNU17-NEXT:                     break %[[VALUE4]];
// GNU17-NEXT:                 }
// GNU17-NEXT:             while ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0));
// GNU17-NEXT:         label %[[VALUE_choice:[0-9]+]] choice:
// GNU17-NEXT:             switch %[[VALUE5:[0-9]+]] read<i32>(%[[VALUE_n]])
// GNU17-NEXT:                 {
// GNU17-NEXT:                     case %[[VALUE5]] const<i32>(0):
// GNU17-NEXT:                         break %[[VALUE5]];
// GNU17-NEXT:                     default %[[VALUE5]]:
// GNU17-NEXT:                         break %[[VALUE5]];
// GNU17-NEXT:                 }
// GNU17-NEXT:     }
// GNU17-NEXT: }
// SLATE-FILECHECK-END GNU17
