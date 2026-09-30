// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

int getc(void *);
int f(int);
void sequencing(void *file, int x, int c) {
    do f(c); while ((c = getc(file)) != -1);
    x = c ? f(c++) : c--;
    x = f(c), c = 0;
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
// IR-NEXT:     fn %[[VALUE_getc:[0-9]+]] @getc(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE1:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_sequencing:[0-9]+]] @sequencing(%[[VALUE_file:[0-9]+]] file: ptr<void>, %[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_c:[0-9]+]] c: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         do %[[VALUE2:[0-9]+]]
// IR-NEXT:             call<i32, signature=fn(i32) -> i32>(%[[VALUE_f]], read<i32>(%[[VALUE_c]]));
// IR-NEXT:         while {
// IR-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = call<i32, signature=fn(ptr<void>) -> i32>(%[[VALUE_getc]], read<ptr<void>>(%[[VALUE_file]]));
// IR-NEXT:             write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE3]]));
// IR-NEXT:             yield ne<i32>(read<i32>(%[[VALUE3]]), neg<i32, overflow=ub>(const<i32>(1)));
// IR-NEXT:         };
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic];
// IR-NEXT:         if ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0))
// IR-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// IR-NEXT:             let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// IR-NEXT:             write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE6]]));
// IR-NEXT:             write<i32>(%[[VALUE4]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_f]], read<i32>(%[[VALUE5]])));
// IR-NEXT:         else
// IR-NEXT:             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// IR-NEXT:             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// IR-NEXT:             write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE8]]));
// IR-NEXT:             write<i32>(%[[VALUE4]], read<i32>(%[[VALUE7]]));
// IR-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE4]]));
// IR-NEXT:         write<i32>(%[[VALUE_x]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_f]], read<i32>(%[[VALUE_c]])));
// IR-NEXT:         write<i32>(%[[VALUE_c]], const<i32>(0));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
