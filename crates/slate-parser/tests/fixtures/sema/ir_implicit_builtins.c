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
// IR-NEXT:     fn %12 @__builtin_memcpy(%9 <unnamed>: ptr<void>, %10 <unnamed>: ptr<const void>, %11 <unnamed>: u64) -> ptr<void> [linkage=external] [c_builtin="__builtin_memcpy"];
// IR-NEXT:     fn %16 @__builtin_memset(%13 <unnamed>: ptr<void>, %14 <unnamed>: i32, %15 <unnamed>: u64) -> ptr<void> [linkage=external] [c_builtin="__builtin_memset"];
// IR-NEXT:     fn %20 @__builtin_memcmp(%17 <unnamed>: ptr<const void>, %18 <unnamed>: ptr<const void>, %19 <unnamed>: u64) -> i32 [linkage=external] [c_builtin="__builtin_memcmp"];
// IR-NEXT:     fn %0 @use_memory(%1 destination: ptr<void> [c="void *"], %2 source: ptr<const void> [c="const void *"], %3 size: u64 [c="unsigned long"]) -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void *, const void *, unsigned long)"] {
// IR-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%1), read<ptr<const void>>(%2), read<u64>(%3));
// IR-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%16, read<ptr<void>>(%1), const<i32>(0), read<u64>(%3));
// IR-NEXT:         call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%20, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%1)), read<ptr<const void>>(%2), read<u64>(%3));
// IR-NEXT:     }
// IR-NEXT:     fn %4 @add(%5 left: i32 [c="int"], %6 right: i32 [c="int"], %7 result: ptr<i32> [c="int *"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int, int, int *)"] {
// IR-NEXT:         return from_bool<i32, reason=return>(overflow_add<bool>(read<i32>(%5), read<i32>(%6), deref(read<ptr<i32>>(%7))) [c_builtin="__builtin_add_overflow"]);
// IR-NEXT:     }
// IR-NEXT:     fn %21 @__builtin_abort() -> void [linkage=external] [noreturn] [c_builtin="__builtin_abort"];
// IR-NEXT:     fn %8 @fail() -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void)"] {
// IR-NEXT:         call<void, signature=fn() -> void>(%21);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
