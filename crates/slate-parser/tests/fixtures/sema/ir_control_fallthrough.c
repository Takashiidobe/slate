// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata
// SLATE-FILECHECK-STD IR c23
void fallthrough(int x) {
    switch (x) {
    case 0: x++;
        [[fallthrough]];
    case 1: break;
    }
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
// IR-NEXT:     fn %0 @fallthrough(%1 x: i32 [c="int"]) -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(int)"] {
// IR-NEXT:         switch %2 read<i32>(%1)
// IR-NEXT:             {
// IR-NEXT:                 case %2 const<i32>(0):
// IR-NEXT:                     update<i32, result=old>(%1, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// IR-NEXT:                  [c_attribute="fallthrough"];
// IR-NEXT:                 case %2 const<i32>(1):
// IR-NEXT:                     break %2;
// IR-NEXT:             }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
