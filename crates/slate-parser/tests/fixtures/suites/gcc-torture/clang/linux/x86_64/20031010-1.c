// SLATE-FILECHECK-DEFINES DEFAULT

/* This crashed the ARM backend with -mcpu=iwmmxt -O because an insn
   required a split which was not available for the iwmmxt.  */
inline int *f1(int* a, int* b) { if (*b < *a) return b; return a; }
int f2(char *d, char *e, int f) { int g = e - d; return *f1(&f, &g); }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %0 @f1(%1 a: ptr<i32>, %2 b: ptr<i32>) -> ptr<i32> [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<i32>(read<i32>(deref(read<ptr<i32>>(%2))), read<i32>(deref(read<ptr<i32>>(%1))))
// DEFAULT-NEXT:             return read<ptr<i32>>(%2);
// DEFAULT-NEXT:         return read<ptr<i32>>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @f2(%4 d: ptr<i8>, %5 e: ptr<i8>, %6 f: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 g: i32 [storage=automatic] = truncate<i32, reason=assign, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%5), read<ptr<i8>>(%4)));
// DEFAULT-NEXT:         return read<i32>(deref(call<ptr<i32>, signature=fn(ptr<i32>, ptr<i32>) -> ptr<i32>>(%0, addr_of<ptr<i32>>(%6), addr_of<ptr<i32>>(%7))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
