// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata

void use_memory(void *destination, const void *source, __SIZE_TYPE__ size) {
  __builtin_memcpy(destination, source, size);
  __builtin_memset(destination, 0, size);
  (void)__builtin_memcmp(destination, source, size);
}

int add(int left, int right, int *result) {
  return __builtin_add_overflow(left, right, result);
}

void fail(void) {
  __builtin_abort();
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
// IR-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [c_builtin="__builtin_memcpy"];
// IR-NEXT:     fn %[[VALUE___builtin_memset:[0-9]+]] @__builtin_memset(%[[VALUE3:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE4:[0-9]+]] <unnamed>: i32, %[[VALUE5:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [c_builtin="__builtin_memset"];
// IR-NEXT:     fn %[[VALUE___builtin_memcmp:[0-9]+]] @__builtin_memcmp(%[[VALUE6:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE7:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE8:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [c_builtin="__builtin_memcmp"];
// IR-NEXT:     fn %[[VALUE_use_memory:[0-9]+]] @use_memory(%[[VALUE_destination:[0-9]+]] destination: ptr<void> [c="void *"], %[[VALUE_source:[0-9]+]] source: ptr<const void> [c="const void *"], %[[VALUE_size:[0-9]+]] size: u64 [c="unsigned long"]) -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void *, const void *, unsigned long)"] {
// IR-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_destination]]), read<ptr<const void>>(%[[VALUE_source]]), read<u64>(%[[VALUE_size]]));
// IR-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], read<ptr<void>>(%[[VALUE_destination]]), const<i32>(0), read<u64>(%[[VALUE_size]]));
// IR-NEXT:         call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_destination]])), read<ptr<const void>>(%[[VALUE_source]]), read<u64>(%[[VALUE_size]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_left:[0-9]+]] left: i32 [c="int"], %[[VALUE_right:[0-9]+]] right: i32 [c="int"], %[[VALUE_result:[0-9]+]] result: ptr<i32> [c="int *"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int, int, int *)"] {
// IR-NEXT:         return from_bool<i32, reason=return>(overflow_add<bool>(read<i32>(%[[VALUE_left]]), read<i32>(%[[VALUE_right]]), deref(read<ptr<i32>>(%[[VALUE_result]]))) [c_builtin="__builtin_add_overflow"]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn] [c_builtin="__builtin_abort"];
// IR-NEXT:     fn %[[VALUE_fail:[0-9]+]] @fail() -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void)"] {
// IR-NEXT:         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
