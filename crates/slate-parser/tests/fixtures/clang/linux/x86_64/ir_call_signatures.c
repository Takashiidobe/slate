// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-ARGS --dump-ir

int old();
int before(short small, float real) {
    return old(small, real);
}
int old(int first, double second);
int after(short small, float real) {
    return old(small, real);
}
extern int variadic(int first, ...);
extern void consume(int value);
int main(void) {
    int value = variadic(1, 2, "text");
    consume(value);
    before(3, 4.0f);
    after(5, 6.0f);
}

// SLATE-FILECHECK-BEGIN C17
// C17: module {
// C17-NEXT:     target "x86_64-unknown-linux-gnu" {
// C17-NEXT:         endian = little;
// C17-NEXT:         pointer [size=8, align=8];
// C17-NEXT:         stack_alignment = 16;
// C17-NEXT:         long_double = f80;
// C17-NEXT:         storage bool [size=1, align=1];
// C17-NEXT:         storage i8, u8 [size=1, align=1];
// C17-NEXT:         storage i16, u16 [size=2, align=2];
// C17-NEXT:         storage i32, u32 [size=4, align=4];
// C17-NEXT:         storage i64, u64 [size=8, align=8];
// C17-NEXT:         storage i128, u128 [size=16, align=16];
// C17-NEXT:         storage bf16 [size=2, align=2];
// C17-NEXT:         storage f16 [size=2, align=2];
// C17-NEXT:         storage f32 [size=4, align=4];
// C17-NEXT:         storage f64 [size=8, align=8];
// C17-NEXT:         storage f80 [size=16, align=16];
// C17-NEXT:         storage f128 [size=16, align=16];
// C17-NEXT:         storage d32 [size=4, align=4];
// C17-NEXT:         storage d64 [size=8, align=8];
// C17-NEXT:         storage d128 [size=16, align=16];
// C17-NEXT:     }
// C17-NEXT:     global %19 .str19: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 101, 120, 116, 0]) [linkage=internal];
// C17-NEXT:     fn %0 @old(%15 first: i32, %16 second: f64) -> i32 [linkage=external];
// C17-NEXT:     fn %1 @before(%2 small: i16, %3 real: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C17-NEXT:         return call<i32, signature=fn(unprototyped) -> i32>(%0, widen<i32, reason=vararg>(read<i16>(%2)), float_widen<f64, reason=vararg>(read<f32>(%3)));
// C17-NEXT:     }
// C17-NEXT:     fn %6 @after(%7 small: i16, %8 real: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C17-NEXT:         return call<i32, signature=fn(i32, f64) -> i32>(%0, widen<i32, reason=arg>(read<i16>(%7)), float_widen<f64, reason=arg>(read<f32>(%8)));
// C17-NEXT:     }
// C17-NEXT:     fn %10 @variadic(%17 first: i32, ...) -> i32 [linkage=external];
// C17-NEXT:     fn %12 @consume(%18 value: i32) -> void [linkage=external];
// C17-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// C17-NEXT:         let %14 value: i32 [storage=automatic] = call<i32, signature=fn(i32, ...) -> i32>(%10, const<i32>(1), const<i32>(2), array_decay<ptr<i8>, length=Some(5)>(%19));
// C17-NEXT:         call<void, signature=fn(i32) -> void>(%12, read<i32>(%14));
// C17-NEXT:         call<i32, signature=fn(i16, f32) -> i32>(%1, truncate<i16, reason=arg, fits=always>(const<i32>(3)), const<f32>(4.0));
// C17-NEXT:         call<i32, signature=fn(i16, f32) -> i32>(%6, truncate<i16, reason=arg, fits=always>(const<i32>(5)), const<f32>(6.0));
// C17-NEXT:     }
// C17-NEXT: }
// SLATE-FILECHECK-END C17
