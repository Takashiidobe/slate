// SLATE-FILECHECK-DEFINES DEFAULT

/* PR optimization/12260.  */

extern int f(void);
extern int g(int);

static char buf[512];
void h(int l) {
    while (l) {
        char *op = buf;
        if (f() == 0)
            break;
        if (g(op - buf + 1))
            break;
    }
}

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
// DEFAULT-NEXT:     global %2 buf: array<i8, 512> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @f() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @g(%6 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @h(%4 l: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         while %7 ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %5 op: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(512)>(%2);
// DEFAULT-NEXT:                 if eq<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:                     break %7;
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, truncate<i32, reason=arg, fits=unknown>(add<i64, overflow=ub>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%5), array_decay<ptr<i8>, length=Some(512)>(%2)), widen<i64, reason=usual_arith>(const<i32>(1))))), const<i32>(0))
// DEFAULT-NEXT:                     break %7;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
