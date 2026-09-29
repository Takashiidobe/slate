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
// DEFAULT-NEXT:     type @type[[TYPE_v4:[0-9]+]] v4 = vector<i32, 4>;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_plain:[0-9]+]] plain: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([116, 111, 112, 32, 108, 101, 118, 101, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_pretty:[0-9]+]] pretty: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_2]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_plain_size:[0-9]+]] plain_size: u64 [storage=static] = const<u64>(1) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_touch:[0-9]+]] @touch() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_plus:[0-9]+]] @plus(%[[VALUE_v:[0-9]+]] v: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<vector<i32, 4>>(%[[VALUE_v]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_choose:[0-9]+]] @choose(%[[VALUE_c:[0-9]+]] c: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_touch]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_touch]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
