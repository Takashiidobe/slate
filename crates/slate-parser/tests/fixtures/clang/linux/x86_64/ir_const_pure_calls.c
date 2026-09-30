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
// IR-NEXT:     fn %[[VALUE_square:[0-9]+]] @square(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// IR-NEXT:     fn %[[VALUE_length:[0-9]+]] @length(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// IR-NEXT:     fn %[[VALUE_both_attributes:[0-9]+]] @both_attributes(%[[VALUE2:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// IR-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE3:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_and_const:[0-9]+]] @and_const(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)), ne<i32>(call<i32>(%[[VALUE_square]], read<i32>(%[[VALUE_b]])), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_or_pure:[0-9]+]] @or_pure(%[[VALUE_s:[0-9]+]] s: ptr<const i8>, %[[VALUE_b_2:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_b_2]]), const<i32>(0)), ne<i32>(call<i32>(%[[VALUE_length]], read<ptr<const i8>>(%[[VALUE_s]])), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_conditional_const:[0-9]+]] @conditional_const(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b_3:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return conditional<i32>(ne<i32>(read<i32>(%[[VALUE_a_2]]), const<i32>(0)), call<i32>(%[[VALUE_square]], read<i32>(%[[VALUE_b_3]])), read<i32>(%[[VALUE_b_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE___builtin_popcount:[0-9]+]] @__builtin_popcount(%[[VALUE4:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// IR-NEXT:     fn %[[VALUE_builtin_const:[0-9]+]] @builtin_const(%[[VALUE_a_3:[0-9]+]] a: i32, %[[VALUE_b_4:[0-9]+]] b: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_a_3]]), const<i32>(0)), ne<i32>(call<i32>(%[[VALUE___builtin_popcount]], read<u32>(%[[VALUE_b_4]])), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_strongest:[0-9]+]] @strongest(%[[VALUE_a_4:[0-9]+]] a: i32, %[[VALUE_b_5:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_a_4]]), const<i32>(0)), ne<i32>(call<i32>(%[[VALUE_both_attributes]], read<i32>(%[[VALUE_b_5]])), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_plain_call:[0-9]+]] @plain_call(%[[VALUE_a_5:[0-9]+]] a: i32, %[[VALUE_b_6:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic];
// IR-NEXT:         if ne<i32>(read<i32>(%[[VALUE_a_5]]), const<i32>(0))
// IR-NEXT:             write<bool>(%[[VALUE5]], ne<i32>(call<i32>(%[[VALUE_plain]], read<i32>(%[[VALUE_b_6]])), const<i32>(0)));
// IR-NEXT:         else
// IR-NEXT:             write<bool>(%[[VALUE5]], const<bool>(false));
// IR-NEXT:         return from_bool<i32>(read<bool>(%[[VALUE5]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_effectful_argument:[0-9]+]] @effectful_argument(%[[VALUE_a_6:[0-9]+]] a: i32, %[[VALUE_b_7:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// IR-NEXT:         if ne<i32>(read<i32>(%[[VALUE_a_6]]), const<i32>(0))
// IR-NEXT:             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b_7]]);
// IR-NEXT:             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE7]]), const<i32>(1));
// IR-NEXT:             write<i32>(%[[VALUE_b_7]], read<i32>(%[[VALUE8]]));
// IR-NEXT:             write<bool>(%[[VALUE6]], ne<i32>(call<i32>(%[[VALUE_square]], read<i32>(%[[VALUE7]])), const<i32>(0)));
// IR-NEXT:         else
// IR-NEXT:             write<bool>(%[[VALUE6]], const<bool>(false));
// IR-NEXT:         return from_bool<i32>(read<bool>(%[[VALUE6]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
