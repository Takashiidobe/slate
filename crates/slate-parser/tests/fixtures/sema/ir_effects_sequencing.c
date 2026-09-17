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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %0 @getc(%6 <unnamed>: ptr<void>) -> i32 [linkage=external];
// IR-NEXT:     fn %1 @f(%7 <unnamed>: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %2 @sequencing(%3 file: ptr<void>, %4 x: i32, %5 c: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         do %8
// IR-NEXT:             call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%5));
// IR-NEXT:         while {
// IR-NEXT:             write<i32>(%5, call<i32, signature=fn(ptr<void>) -> i32>(%0, read<ptr<void>>(%3)));
// IR-NEXT:             yield ne<i32>(call<i32, signature=fn(ptr<void>) -> i32>(%0, read<ptr<void>>(%3)), neg<i32, overflow=ub>(const<i32>(1)));
// IR-NEXT:         };
// IR-NEXT:         let %9: i32 [synthetic];
// IR-NEXT:         if ne<i32>(read<i32>(%5), const<i32>(0))
// IR-NEXT:             let %10: i32 [synthetic] = read<i32>(%5);
// IR-NEXT:             let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// IR-NEXT:             write<i32>(%5, read<i32>(%11));
// IR-NEXT:             write<i32>(%9, call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%10)));
// IR-NEXT:         else
// IR-NEXT:             let %12: i32 [synthetic] = read<i32>(%5);
// IR-NEXT:             let %13: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// IR-NEXT:             write<i32>(%5, read<i32>(%13));
// IR-NEXT:             write<i32>(%9, read<i32>(%12));
// IR-NEXT:         write<i32>(%4, read<i32>(%9));
// IR-NEXT:         write<i32>(%4, call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%5)));
// IR-NEXT:         call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%5));
// IR-NEXT:         write<i32>(%5, const<i32>(0));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
