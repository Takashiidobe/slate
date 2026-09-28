// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

__attribute__((const)) int square(int);
__attribute__((pure)) int length(const char *);
__attribute__((pure, const)) int both_attributes(int);
int plain(int);

int and_const(int a, int b) { return a && square(b); }
int or_pure(const char *s, int b) { return b || length(s); }
int conditional_const(int a, int b) { return a ? square(b) : b; }
int builtin_const(int a, unsigned b) { return a && __builtin_popcount(b); }
int strongest(int a, int b) { return a && both_attributes(b); }
int plain_call(int a, int b) { return a && plain(b); }
int effectful_argument(int a, int b) { return a && square(b++); }

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
// IR-NEXT:     fn %0 @square(%25 <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// IR-NEXT:     fn %1 @length(%26 <unnamed>: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// IR-NEXT:     fn %2 @both_attributes(%27 <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// IR-NEXT:     fn %3 @plain(%28 <unnamed>: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %4 @and_const(%5 a: i32, %6 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32>(logical_and<bool>(ne<i32>(read<i32>(%5), const<i32>(0)), ne<i32>(call<i32>(%0, read<i32>(%6)), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %7 @or_pure(%8 s: ptr<const i8>, %9 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32>(logical_or<bool>(ne<i32>(read<i32>(%9), const<i32>(0)), ne<i32>(call<i32>(%1, read<ptr<const i8>>(%8)), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %10 @conditional_const(%11 a: i32, %12 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return conditional<i32>(ne<i32>(read<i32>(%11), const<i32>(0)), call<i32>(%0, read<i32>(%12)), read<i32>(%12));
// IR-NEXT:     }
// IR-NEXT:     fn %30 @__builtin_popcount(%29 <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// IR-NEXT:     fn %13 @builtin_const(%14 a: i32, %15 b: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32>(logical_and<bool>(ne<i32>(read<i32>(%14), const<i32>(0)), ne<i32>(call<i32>(%30, read<u32>(%15)), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %16 @strongest(%17 a: i32, %18 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32>(logical_and<bool>(ne<i32>(read<i32>(%17), const<i32>(0)), ne<i32>(call<i32>(%2, read<i32>(%18)), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %19 @plain_call(%20 a: i32, %21 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %31: bool [synthetic];
// IR-NEXT:         if ne<i32>(read<i32>(%20), const<i32>(0))
// IR-NEXT:             write<bool>(%31, ne<i32>(call<i32>(%3, read<i32>(%21)), const<i32>(0)));
// IR-NEXT:         else
// IR-NEXT:             write<bool>(%31, const<bool>(false));
// IR-NEXT:         return from_bool<i32>(read<bool>(%31));
// IR-NEXT:     }
// IR-NEXT:     fn %22 @effectful_argument(%23 a: i32, %24 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %32: bool [synthetic];
// IR-NEXT:         if ne<i32>(read<i32>(%23), const<i32>(0))
// IR-NEXT:             let %33: i32 [synthetic] = read<i32>(%24);
// IR-NEXT:             let %34: i32 [synthetic] = add<i32>(read<i32>(%33), const<i32>(1));
// IR-NEXT:             write<i32>(%24, read<i32>(%34));
// IR-NEXT:             write<bool>(%32, ne<i32>(call<i32>(%0, read<i32>(%33)), const<i32>(0)));
// IR-NEXT:         else
// IR-NEXT:             write<bool>(%32, const<bool>(false));
// IR-NEXT:         return from_bool<i32>(read<bool>(%32));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
