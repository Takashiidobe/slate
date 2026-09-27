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
// IR-NEXT:     fn %0 @switches(%1 x: i16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         while %13 ne<i16>(read<i16>(%1), const<i16>(0))
// IR-NEXT:             {
// IR-NEXT:                 switch %14 widen<i32>(read<i16>(%1))
// IR-NEXT:                     {
// IR-NEXT:                         case %14 const<i32>(0):
// IR-NEXT:                             case %14 const<i32>(1):
// IR-NEXT:                                 let %22: i16 [synthetic] = read<i16>(%1);
// IR-NEXT:                                 let %23: i16 [synthetic] = truncate<i16>(add<i32>(widen<i32>(read<i16>(%22)), const<i32>(1)));
// IR-NEXT:                                 write<i16>(%1, read<i16>(%23));
// IR-NEXT:                         case %14 const<i32>(2) ... const<i32>(4):
// IR-NEXT:                             continue %13;
// IR-NEXT:                         default %14:
// IR-NEXT:                             break %14;
// IR-NEXT:                     }
// IR-NEXT:                 break %13;
// IR-NEXT:             }
// IR-NEXT:         switch %15 widen<i32>(read<i16>(%1))
// IR-NEXT:             {
// IR-NEXT:                 case %15 const<i32>(2):
// IR-NEXT:                     do %16
// IR-NEXT:                         {
// IR-NEXT:                             let %24: i16 [synthetic] = read<i16>(%1);
// IR-NEXT:                             let %25: i16 [synthetic] = truncate<i16>(sub<i32>(widen<i32>(read<i16>(%24)), const<i32>(1)));
// IR-NEXT:                             write<i16>(%1, read<i16>(%25));
// IR-NEXT:                             case %15 const<i32>(5):
// IR-NEXT:                                 let %26: i16 [synthetic] = read<i16>(%1);
// IR-NEXT:                                 let %27: i16 [synthetic] = truncate<i16>(sub<i32>(widen<i32>(read<i16>(%26)), const<i32>(1)));
// IR-NEXT:                                 write<i16>(%1, read<i16>(%27));
// IR-NEXT:                         }
// IR-NEXT:                     while ne<i16>(read<i16>(%1), const<i16>(0));
// IR-NEXT:                 default %15:
// IR-NEXT:                     switch %17 widen<i32>(read<i16>(%1))
// IR-NEXT:                         {
// IR-NEXT:                             case %17 const<i32>(2):
// IR-NEXT:                                 break %17;
// IR-NEXT:                             default %17:
// IR-NEXT:                                 break %17;
// IR-NEXT:                         }
// IR-NEXT:             }
// IR-NEXT:         return widen<i32>(read<i16>(%1));
// IR-NEXT:     }
// IR-NEXT:     fn %2 @indirect(%5 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %6 target: ptr<void> [storage=automatic] = conditional<ptr<void>>(ne<i32>(read<i32>(%5), const<i32>(0)), label_addr<ptr<void>>(%3), label_addr<ptr<void>>(%4));
// IR-NEXT:         goto *read<ptr<void>>(%6);
// IR-NEXT:         label %3 yes:
// IR-NEXT:             return const<i32>(1);
// IR-NEXT:         label %4 no:
// IR-NEXT:             return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %7 @direct_address() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         goto *label_addr<ptr<void>>(%8);
// IR-NEXT:         label %8 done:
// IR-NEXT:             return;
// IR-NEXT:     }
// IR-NEXT:     fn %9 @loops(%10 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(0))
// IR-NEXT:             let %28: i32 [synthetic] = read<i32>(%10);
// IR-NEXT:             let %29: i32 [synthetic] = sub<i32>(read<i32>(%28), const<i32>(1));
// IR-NEXT:             write<i32>(%10, read<i32>(%29));
// IR-NEXT:         else
// IR-NEXT:             {
// IR-NEXT:                 let %30: i32 [synthetic] = read<i32>(%10);
// IR-NEXT:                 let %31: i32 [synthetic] = add<i32>(read<i32>(%30), const<i32>(1));
// IR-NEXT:                 write<i32>(%10, read<i32>(%31));
// IR-NEXT:             }
// IR-NEXT:         while %18 ne<i32>(read<i32>(%10), const<i32>(0))
// IR-NEXT:             {
// IR-NEXT:                 if eq<i32>(read<i32>(%10), const<i32>(2))
// IR-NEXT:                     break %18;
// IR-NEXT:                 let %32: i32 [synthetic] = read<i32>(%10);
// IR-NEXT:                 let %33: i32 [synthetic] = sub<i32>(read<i32>(%32), const<i32>(1));
// IR-NEXT:                 write<i32>(%10, read<i32>(%33));
// IR-NEXT:                 continue %18;
// IR-NEXT:             }
// IR-NEXT:         do %19
// IR-NEXT:             let %34: i32 [synthetic] = read<i32>(%10);
// IR-NEXT:             let %35: i32 [synthetic] = add<i32>(read<i32>(%34), const<i32>(1));
// IR-NEXT:             write<i32>(%10, read<i32>(%35));
// IR-NEXT:         while lt<i32>(read<i32>(%10), const<i32>(3));
// IR-NEXT:         for %20
// IR-NEXT:             init:
// IR-NEXT:                 let %11 i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:                 let %12 j: i32 [storage=automatic] = const<i32>(2);
// IR-NEXT:             condition: lt<i32>(read<i32>(%11), read<i32>(%12))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %36: i32 [synthetic] = read<i32>(%11);
// IR-NEXT:                 let %37: i32 [synthetic] = add<i32>(read<i32>(%36), const<i32>(1));
// IR-NEXT:                 write<i32>(%11, read<i32>(%37));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     if ne<i32>(read<i32>(%10), const<i32>(0))
// IR-NEXT:                         continue %20;
// IR-NEXT:                     break %20;
// IR-NEXT:                 }
// IR-NEXT:         for %21
// IR-NEXT:             init:
// IR-NEXT:             condition: omitted
// IR-NEXT:             increment: omitted
// IR-NEXT:             body:
// IR-NEXT:                 ;
// IR-NEXT:         return conditional<i32>(ne<i32>(read<i32>(%10), const<i32>(0)), read<i32>(%10), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
