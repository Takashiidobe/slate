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
// IR-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn] [c="void(void)"] [c_builtin="abort"] [c_builtin_kind="library"] [c_builtin_header="stdlib.h"];
// IR-NEXT:     fn %[[VALUE___builtin_trap:[0-9]+]] @__builtin_trap() -> void [linkage=external] [noreturn] [c_builtin="__builtin_trap"] [c_builtin_kind="builtin"];
// IR-NEXT:     fn %[[VALUE___builtin_unreachable:[0-9]+]] @__builtin_unreachable() -> void [linkage=external] [noreturn] [c_builtin="__builtin_unreachable"] [c_builtin_kind="builtin"];
// IR-NEXT:     fn %[[VALUE_pick:[0-9]+]] @pick(%[[VALUE_value:[0-9]+]] value: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int)"] {
// IR-NEXT:         if gt<i32>(read<i32>(%[[VALUE_value]]), const<i32>(0))
// IR-NEXT:             return read<i32>(%[[VALUE_value]]);
// IR-NEXT:         if lt<i32>(read<i32>(%[[VALUE_value]]), const<i32>(0))
// IR-NEXT:             call<void>(%[[VALUE___builtin_trap]]);
// IR-NEXT:         call<void>(%[[VALUE___builtin_unreachable]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn] [c_builtin="__builtin_abort"] [c_builtin_kind="builtin"] [c_builtin_header="stdlib.h"];
// IR-NEXT:     fn %[[VALUE_stop:[0-9]+]] @stop() -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void)"] {
// IR-NEXT:         call<void>(%[[VALUE___builtin_abort]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_stop_declared:[0-9]+]] @stop_declared() -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void)"] {
// IR-NEXT:         call<void>(%[[VALUE_abort]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32 [c="int"]) -> void [linkage=external] [noreturn] [c="void(int)"] [c_builtin="exit"] [c_builtin_kind="library"] [c_builtin_header="stdlib.h"];
// IR-NEXT:     fn %[[VALUE_stop_block_scope:[0-9]+]] @stop_block_scope(%[[VALUE_status:[0-9]+]] status: i32 [c="int"]) -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(int)"] {
// IR-NEXT:         call<void>(%[[VALUE_exit]], read<i32>(%[[VALUE_status]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE___builtin_abs:[0-9]+]] @__builtin_abs(%[[VALUE1:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none] [c_builtin="__builtin_abs"] [c_builtin_kind="builtin"] [c_builtin_header="stdlib.h"];
// IR-NEXT:     fn %[[VALUE_ordinary:[0-9]+]] @ordinary(%[[VALUE_value_2:[0-9]+]] value: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int)"] {
// IR-NEXT:         return add<i32>(add<i32>(call<i32>(%[[VALUE___builtin_abs]], read<i32>(%[[VALUE_value_2]])), reinterpret<i32>(truncate<u32>(const<u64>(8) [size_of="i64"]))), call<i32>(%[[VALUE___builtin_abs]], neg<i32>(read<i32>(%[[VALUE_value_2]]))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_again:[0-9]+]] @again() -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void)"] {
// IR-NEXT:         call<void>(%[[VALUE___builtin_trap]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
