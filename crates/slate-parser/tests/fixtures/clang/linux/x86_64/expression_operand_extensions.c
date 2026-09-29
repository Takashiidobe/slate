typedef int v4 __attribute__((vector_size(16)));
const char *plain = __func__;
const char *pretty = __PRETTY_FUNCTION__;
unsigned long plain_size = sizeof(__func__);
void touch(void);
v4 plus(v4 v) { return +v; }
void choose(int c) {
  c ? touch() : 0;
  c ? (void)0 : touch();
}

// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     type @type0 v4 = vector<i32, 4>;
// DEFAULT-NEXT:     global %9 .str9: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %1 plain: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(1)>(%9)) [linkage=external];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([116, 111, 112, 32, 108, 101, 118, 101, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %2 pretty: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%10)) [linkage=external];
// DEFAULT-NEXT:     global %3 plain_size: u64 [storage=static] = const<u64>(1) [linkage=external];
// DEFAULT-NEXT:     fn %4 @touch() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @plus(%6 v: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<vector<i32, 4>>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @choose(%8 c: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
