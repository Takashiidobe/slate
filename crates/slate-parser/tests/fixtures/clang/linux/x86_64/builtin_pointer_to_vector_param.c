typedef char v64qi __attribute__((vector_size(64)));

__attribute__((target("avx512bw")))
v64qi masked_load(const void *p, v64qi w, unsigned long long u) {
  return __builtin_ia32_loaddquqi512_mask((const v64qi *)p, w, u);
}

__attribute__((target("avx512bw")))
void masked_store(void *p, v64qi a, unsigned long long u) {
  __builtin_ia32_storedquqi512_mask((v64qi *)p, a, u);
}

// SLATE-FILECHECK-DEFINES CHECK

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: module {
// CHECK-NEXT:     target "x86_64-unknown-linux-gnu" {
// CHECK-NEXT:         endian = little;
// CHECK-NEXT:         pointer [size=8, align=8];
// CHECK-NEXT:         stack_alignment = 16;
// CHECK-NEXT:         long_double = f80;
// CHECK-NEXT:         storage bool [size=1, align=1];
// CHECK-NEXT:         storage i8, u8 [size=1, align=1];
// CHECK-NEXT:         storage i16, u16 [size=2, align=2];
// CHECK-NEXT:         storage i32, u32 [size=4, align=4];
// CHECK-NEXT:         storage i64, u64 [size=8, align=8];
// CHECK-NEXT:         storage i128, u128 [size=16, align=16];
// CHECK-NEXT:         storage bf16 [size=2, align=2];
// CHECK-NEXT:         storage f16 [size=2, align=2];
// CHECK-NEXT:         storage f32 [size=4, align=4];
// CHECK-NEXT:         storage f64 [size=8, align=8];
// CHECK-NEXT:         storage f80 [size=16, align=16];
// CHECK-NEXT:         storage f128 [size=16, align=16];
// CHECK-NEXT:         storage d32 [size=4, align=4];
// CHECK-NEXT:         storage d64 [size=8, align=8];
// CHECK-NEXT:         storage d128 [size=16, align=16];
// CHECK-NEXT:     }
// CHECK-NEXT:     type @type[[TYPE_v64qi:[0-9]+]] v64qi = vector<i8, 64>;
// CHECK-NEXT:     fn %[[VALUE___builtin_ia32_loaddquqi512_mask:[0-9]+]] @__builtin_ia32_loaddquqi512_mask(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const vector<i8, 64>>, %[[VALUE1:[0-9]+]] <unnamed>: vector<i8, 64>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> vector<i8, 64> [linkage=external] [abi=sysv64(scalar, byval<align=64>, scalar) -> direct];
// CHECK-NEXT:     fn %[[VALUE_masked_load:[0-9]+]] @masked_load(%[[VALUE_p:[0-9]+]] p: ptr<const void>, %[[VALUE_w:[0-9]+]] w: vector<i8, 64>, %[[VALUE_u:[0-9]+]] u: u64) -> vector<i8, 64> [linkage=external] [target=+avx512bw] [abi=sysv64(scalar, byval<align=64>, scalar) -> direct] [fallthrough=ub_if_used] {
// CHECK-NEXT:         return call<vector<i8, 64>, signature=fn(ptr<const vector<i8, 64>>, vector<i8, 64>, u64) -> vector<i8, 64>, abi=sysv64(scalar, byval<align=64>, scalar) -> direct>(%[[VALUE___builtin_ia32_loaddquqi512_mask]], pointer_cast<ptr<const vector<i8, 64>>, reason=explicit>(read<ptr<const void>>(%[[VALUE_p]])), read<vector<i8, 64>>(%[[VALUE_w]]), read<u64>(%[[VALUE_u]]));
// CHECK-NEXT:     }
// CHECK-NEXT:     fn %[[VALUE___builtin_ia32_storedquqi512_mask:[0-9]+]] @__builtin_ia32_storedquqi512_mask(%[[VALUE3:[0-9]+]] <unnamed>: ptr<vector<i8, 64>>, %[[VALUE4:[0-9]+]] <unnamed>: vector<i8, 64>, %[[VALUE5:[0-9]+]] <unnamed>: u64) -> void [linkage=external] [abi=sysv64(scalar, byval<align=64>, scalar) -> void];
// CHECK-NEXT:     fn %[[VALUE_masked_store:[0-9]+]] @masked_store(%[[VALUE_p_2:[0-9]+]] p: ptr<void>, %[[VALUE_a:[0-9]+]] a: vector<i8, 64>, %[[VALUE_u_2:[0-9]+]] u: u64) -> void [linkage=external] [target=+avx512bw] [abi=sysv64(scalar, byval<align=64>, scalar) -> void] [fallthrough=ret_void] {
// CHECK-NEXT:         call<void, signature=fn(ptr<vector<i8, 64>>, vector<i8, 64>, u64) -> void, abi=sysv64(scalar, byval<align=64>, scalar) -> void>(%[[VALUE___builtin_ia32_storedquqi512_mask]], pointer_cast<ptr<vector<i8, 64>>, reason=explicit>(read<ptr<void>>(%[[VALUE_p_2]])), read<vector<i8, 64>>(%[[VALUE_a]]), read<u64>(%[[VALUE_u_2]]));
// CHECK-NEXT:     }
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
