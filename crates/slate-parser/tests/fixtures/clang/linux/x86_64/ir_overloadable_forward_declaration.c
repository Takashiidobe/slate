// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu11

int f(int) __attribute__((overloadable));
int f(double) __attribute__((overloadable));
int use1(void) { return f(1) + f(1.0); }
int f(int x) __attribute__((overloadable)) { return x; }
int f(double x) __attribute__((overloadable)) { return 2; }
int f(int) __attribute__((overloadable));
int use2(void) { return f(1) + f(2.0); }

static int s(int) __attribute__((overloadable));
static int s(char *) __attribute__((overloadable));
static int s(int x) __attribute__((overloadable)) { return x; }
int use3(void) { return s(1) + s("a"); }
static int s(char *p) __attribute__((overloadable)) { return 0; }

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
// IR-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// IR-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [asm_name="_Z1fi"] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%[[VALUE_x]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_f_2:[0-9]+]] @f(%[[VALUE_x_2:[0-9]+]] x: f64) -> i32 [linkage=external] [asm_name="_Z1fd"] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(2);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_use1:[0-9]+]] @use1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_f]], const<i32>(1)), call<i32, signature=fn(f64) -> i32>(%[[VALUE_f_2]], const<f64>(1.0)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_use2:[0-9]+]] @use2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_f]], const<i32>(1)), call<i32, signature=fn(f64) -> i32>(%[[VALUE_f_2]], const<f64>(2.0)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_s:[0-9]+]] @s(%[[VALUE_x_3:[0-9]+]] x: i32) -> i32 [linkage=internal] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%[[VALUE_x_3]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_s_2:[0-9]+]] @s(%[[VALUE_p:[0-9]+]] p: ptr<i8>) -> i32 [linkage=internal] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_use3:[0-9]+]] @use3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_s]], const<i32>(1)), call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_s_2]], array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]])));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
