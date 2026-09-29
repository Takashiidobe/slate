// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
#include "ir_control_switch.c"
#include "ir_control_indirect.c"
#include "ir_control_loops.c"

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
// IR-NEXT:                 switch %[[VALUE1:[0-9]+]] widen<i32>(read<i16>(%[[VALUE_x]]))
// IR-NEXT:                     {
// IR-NEXT:                         case %[[VALUE1]] const<i32>(0):
// IR-NEXT:                             case %[[VALUE1]] const<i32>(1):
// IR-NEXT:                                 let %[[VALUE2:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_x]]);
// IR-NEXT:                                 let %[[VALUE3:[0-9]+]]: i16 [synthetic] = truncate<i16>(add<i32>(widen<i32>(read<i16>(%[[VALUE2]])), const<i32>(1)));
// IR-NEXT:                                 write<i16>(%[[VALUE_x]], read<i16>(%[[VALUE3]]));
// IR-NEXT:                         case %[[VALUE1]] const<i32>(2) ... const<i32>(4):
// IR-NEXT:                             continue %[[VALUE0]];
// IR-NEXT:                         default %[[VALUE1]]:
// IR-NEXT:                             break %[[VALUE1]];
// IR-NEXT:                     }
// IR-NEXT:                 break %[[VALUE0]];
// IR-NEXT:             }
// IR-NEXT:         switch %[[VALUE4:[0-9]+]] widen<i32>(read<i16>(%[[VALUE_x]]))
// IR-NEXT:             {
// IR-NEXT:                 case %[[VALUE4]] const<i32>(2):
// IR-NEXT:                     do %[[VALUE5:[0-9]+]]
// IR-NEXT:                         {
// IR-NEXT:                             let %[[VALUE6:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_x]]);
// IR-NEXT:                             let %[[VALUE7:[0-9]+]]: i16 [synthetic] = truncate<i16>(sub<i32>(widen<i32>(read<i16>(%[[VALUE6]])), const<i32>(1)));
// IR-NEXT:                             write<i16>(%[[VALUE_x]], read<i16>(%[[VALUE7]]));
// IR-NEXT:                             case %[[VALUE4]] const<i32>(5):
// IR-NEXT:                                 let %[[VALUE8:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_x]]);
// IR-NEXT:                                 let %[[VALUE9:[0-9]+]]: i16 [synthetic] = truncate<i16>(sub<i32>(widen<i32>(read<i16>(%[[VALUE8]])), const<i32>(1)));
// IR-NEXT:                                 write<i16>(%[[VALUE_x]], read<i16>(%[[VALUE9]]));
// IR-NEXT:                         }
// IR-NEXT:                     while ne<i16>(read<i16>(%[[VALUE_x]]), const<i16>(0));
// IR-NEXT:                 default %[[VALUE4]]:
// IR-NEXT:                     switch %[[VALUE10:[0-9]+]] widen<i32>(read<i16>(%[[VALUE_x]]))
// IR-NEXT:                         {
// IR-NEXT:                             case %[[VALUE10]] const<i32>(2):
// IR-NEXT:                                 break %[[VALUE10]];
// IR-NEXT:                             default %[[VALUE10]]:
// IR-NEXT:                                 break %[[VALUE10]];
// IR-NEXT:                         }
// IR-NEXT:             }
// IR-NEXT:         return widen<i32>(read<i16>(%[[VALUE_x]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_indirect:[0-9]+]] @indirect(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_target:[0-9]+]] target: ptr<void> [storage=automatic] = conditional<ptr<void>>(ne<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(0)), label_addr<ptr<void>>(%[[VALUE_yes:[0-9]+]]), label_addr<ptr<void>>(%[[VALUE_no:[0-9]+]]));
// IR-NEXT:         goto *read<ptr<void>>(%[[VALUE_target]]);
// IR-NEXT:         label %[[VALUE_yes]] yes:
// IR-NEXT:             return const<i32>(1);
// IR-NEXT:         label %[[VALUE_no]] no:
// IR-NEXT:             return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_direct_address:[0-9]+]] @direct_address() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         goto *label_addr<ptr<void>>(%[[VALUE_done:[0-9]+]]);
// IR-NEXT:         label %[[VALUE_done]] done:
// IR-NEXT:             return;
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_loops:[0-9]+]] @loops(%[[VALUE_x_3:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x_3]]), const<i32>(0))
// IR-NEXT:             let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_3]]);
// IR-NEXT:             let %[[VALUE12:[0-9]+]]: i32 [synthetic] = sub<i32>(read<i32>(%[[VALUE11]]), const<i32>(1));
// IR-NEXT:             write<i32>(%[[VALUE_x_3]], read<i32>(%[[VALUE12]]));
// IR-NEXT:         else
// IR-NEXT:             {
// IR-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_3]]);
// IR-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE13]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_x_3]], read<i32>(%[[VALUE14]]));
// IR-NEXT:             }
// IR-NEXT:         while %[[VALUE15:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_x_3]]), const<i32>(0))
// IR-NEXT:             {
// IR-NEXT:                 if eq<i32>(read<i32>(%[[VALUE_x_3]]), const<i32>(2))
// IR-NEXT:                     break %[[VALUE15]];
// IR-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_3]]);
// IR-NEXT:                 let %[[VALUE17:[0-9]+]]: i32 [synthetic] = sub<i32>(read<i32>(%[[VALUE16]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_x_3]], read<i32>(%[[VALUE17]]));
// IR-NEXT:                 continue %[[VALUE15]];
// IR-NEXT:             }
// IR-NEXT:         do %[[VALUE18:[0-9]+]]
// IR-NEXT:             let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_3]]);
// IR-NEXT:             let %[[VALUE20:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE19]]), const<i32>(1));
// IR-NEXT:             write<i32>(%[[VALUE_x_3]], read<i32>(%[[VALUE20]]));
// IR-NEXT:         while lt<i32>(read<i32>(%[[VALUE_x_3]]), const<i32>(3));
// IR-NEXT:         for %[[VALUE21:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:                 let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(2);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// IR-NEXT:                 let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE22]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE23]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_x_3]]), const<i32>(0))
// IR-NEXT:                         continue %[[VALUE21]];
// IR-NEXT:                     break %[[VALUE21]];
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE24:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:             condition: omitted
// IR-NEXT:             increment: omitted
// IR-NEXT:             body:
// IR-NEXT:                 ;
// IR-NEXT:         return conditional<i32>(ne<i32>(read<i32>(%[[VALUE_x_3]]), const<i32>(0)), read<i32>(%[[VALUE_x_3]]), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
