// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir --show-metadata

void abort(void);

int pick(int value) {
  if (value > 0)
    return value;
  if (value < 0)
    __builtin_trap();
  __builtin_unreachable();
}

void stop(void) {
  __builtin_abort();
}

void stop_declared(void) {
  abort();
}

void stop_block_scope(int status) {
  void exit(int);
  exit(status);
}

int ordinary(int value) {
  return __builtin_abs(value) + (int)sizeof(__builtin_labs(value)) + __builtin_abs(-value);
}

void again(void) {
  __builtin_trap();
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
// IR-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn] [c="void(void)"] [c_builtin="abort"];
// IR-NEXT:     fn %11 @__builtin_trap() -> void [linkage=external] [noreturn] [c_builtin="__builtin_trap"];
// IR-NEXT:     fn %12 @__builtin_unreachable() -> void [linkage=external] [noreturn] [c_builtin="__builtin_unreachable"];
// IR-NEXT:     fn %1 @pick(%2 value: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int)"] {
// IR-NEXT:         if gt<i32>(read<i32>(%2), const<i32>(0))
// IR-NEXT:             return read<i32>(%2);
// IR-NEXT:         if lt<i32>(read<i32>(%2), const<i32>(0))
// IR-NEXT:             call<void>(%11);
// IR-NEXT:         call<void>(%12);
// IR-NEXT:     }
// IR-NEXT:     fn %13 @__builtin_abort() -> void [linkage=external] [noreturn] [c_builtin="__builtin_abort"];
// IR-NEXT:     fn %3 @stop() -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void)"] {
// IR-NEXT:         call<void>(%13);
// IR-NEXT:     }
// IR-NEXT:     fn %4 @stop_declared() -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void)"] {
// IR-NEXT:         call<void>(%0);
// IR-NEXT:     }
// IR-NEXT:     fn %7 @exit(%14 <unnamed>: i32 [c="int"]) -> void [linkage=external] [noreturn] [c="void(int)"] [c_builtin="exit"];
// IR-NEXT:     fn %5 @stop_block_scope(%6 status: i32 [c="int"]) -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(int)"] {
// IR-NEXT:         call<void>(%7, read<i32>(%6));
// IR-NEXT:     }
// IR-NEXT:     fn %16 @__builtin_abs(%15 <unnamed>: i32) -> i32 [linkage=external] [memory=none] [c_builtin="__builtin_abs"];
// IR-NEXT:     fn %8 @ordinary(%9 value: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int)"] {
// IR-NEXT:         return add<i32>(add<i32>(call<i32>(%16, read<i32>(%9)), reinterpret<i32>(truncate<u32>(const<u64>(8) [size_of="i64"]))), call<i32>(%16, neg<i32>(read<i32>(%9))));
// IR-NEXT:     }
// IR-NEXT:     fn %10 @again() -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void)"] {
// IR-NEXT:         call<void>(%11);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
