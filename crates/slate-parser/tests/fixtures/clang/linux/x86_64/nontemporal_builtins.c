typedef float v4sf __attribute__((vector_size(16)));

int load_int(const int *p) { return __builtin_nontemporal_load(p); }
v4sf load_vector(v4sf *p) { return __builtin_nontemporal_load(p); }
void store_converted(long *p, char c) { __builtin_nontemporal_store(c, p); }
void store_pointer(void **p, int *q) { __builtin_nontemporal_store(q, p); }
void store_vector(v4sf *p, v4sf v) { __builtin_nontemporal_store(v, p); }

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
// DEFAULT-NEXT:     type @type[[TYPE_v4sf:[0-9]+]] v4sf = vector<f32, 4>;
// DEFAULT-NEXT:     fn %[[VALUE_load_int:[0-9]+]] @load_int(%[[VALUE_p:[0-9]+]] p: ptr<const i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<const i32>>(%[[VALUE_p]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_load_vector:[0-9]+]] @load_vector(%[[VALUE_p_2:[0-9]+]] p: ptr<vector<f32, 4>> [c="v4sf *"]) -> vector<f32, 4> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] [c="v4sf(v4sf *)"] {
// DEFAULT-NEXT:         return read<vector<f32, 4>>(deref(read<ptr<vector<f32, 4>>>(%[[VALUE_p_2]]))) [c="v4sf"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_store_converted:[0-9]+]] @store_converted(%[[VALUE_p_3:[0-9]+]] p: ptr<i64>, %[[VALUE_c:[0-9]+]] c: i8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%[[VALUE_p_3]])), widen<i64, reason=assign>(read<i8>(%[[VALUE_c]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_store_pointer:[0-9]+]] @store_pointer(%[[VALUE_p_4:[0-9]+]] p: ptr<ptr<void>>, %[[VALUE_q:[0-9]+]] q: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_p_4]])), pointer_cast<ptr<void>, reason=assign>(read<ptr<i32>>(%[[VALUE_q]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_store_vector:[0-9]+]] @store_vector(%[[VALUE_p_5:[0-9]+]] p: ptr<vector<f32, 4>> [c="v4sf *"], %[[VALUE_v:[0-9]+]] v: vector<f32, 4> [c="v4sf"]) -> void [linkage=external] [abi=sysv64(scalar, direct) -> void] [fallthrough=ret_void] [c="void(v4sf *, v4sf)"] {
// DEFAULT-NEXT:         write<vector<f32, 4>>(deref(read<ptr<vector<f32, 4>>>(%[[VALUE_p_5]])), read<vector<f32, 4>>(%[[VALUE_v]]) [c="v4sf"]) [c="v4sf"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
