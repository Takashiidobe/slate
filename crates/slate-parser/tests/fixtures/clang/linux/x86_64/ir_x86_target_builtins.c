// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

typedef char v16qi __attribute__((vector_size(16)));

void spin(void) {
  __builtin_ia32_pause();
}

unsigned long long ticks(void) {
  return __builtin_ia32_rdtsc() + __rdtsc();
}

int sign_mask(v16qi bytes) {
  return __builtin_ia32_pmovmskb128(bytes);
}

unsigned long long leading_zeros(unsigned long long value) {
  return __builtin_ia32_lzcnt_u64(value);
}

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
// IR-NEXT:     type @type[[TYPE_v16qi:[0-9]+]] v16qi = vector<i8, 16>;
// IR-NEXT:     fn %[[VALUE___builtin_ia32_pause:[0-9]+]] @__builtin_ia32_pause() -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_spin:[0-9]+]] @spin() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         call<void, signature=fn() -> void>(%[[VALUE___builtin_ia32_pause]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE___builtin_ia32_rdtsc:[0-9]+]] @__builtin_ia32_rdtsc() -> u64 [linkage=external];
// IR-NEXT:     fn %[[VALUE_ticks:[0-9]+]] @ticks() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<u64, overflow=wrap>(call<u64, signature=fn() -> u64>(%[[VALUE___builtin_ia32_rdtsc]]), intrinsic<u64, llvm.x86.rdtsc>());
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE___builtin_ia32_pmovmskb128:[0-9]+]] @__builtin_ia32_pmovmskb128(%[[VALUE0:[0-9]+]] <unnamed>: vector<i8, 16>) -> i32 [linkage=external] [memory=none] [abi=sysv64(direct) -> scalar];
// IR-NEXT:     fn %[[VALUE_sign_mask:[0-9]+]] @sign_mask(%[[VALUE_bytes:[0-9]+]] bytes: vector<i8, 16>) -> i32 [linkage=external] [abi=sysv64(direct) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(vector<i8, 16>) -> i32, abi=sysv64(direct) -> scalar>(%[[VALUE___builtin_ia32_pmovmskb128]], read<vector<i8, 16>>(%[[VALUE_bytes]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE___builtin_ia32_lzcnt_u64:[0-9]+]] @__builtin_ia32_lzcnt_u64(%[[VALUE1:[0-9]+]] <unnamed>: u64) -> u64 [linkage=external] [memory=none];
// IR-NEXT:     fn %[[VALUE_leading_zeros:[0-9]+]] @leading_zeros(%[[VALUE_value:[0-9]+]] value: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_ia32_lzcnt_u64]], read<u64>(%[[VALUE_value]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
