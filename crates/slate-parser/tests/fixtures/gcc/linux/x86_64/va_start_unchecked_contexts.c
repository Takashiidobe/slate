// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

void sysv_ms_start(int n, ...) {
  __builtin_ms_va_list ap;
  __builtin_ms_va_start(ap, n);
  __builtin_ms_va_end(ap);
}

void __attribute__((ms_abi)) win64_native_start(int n, ...) {
  __builtin_va_list ap;
  __builtin_va_start(ap, n);
  __builtin_va_end(ap);
}

void fixed_start(int n) {
  __builtin_va_list ap;
  __builtin_va_start(ap, n);
  __builtin_va_end(ap);
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
// IR-NEXT:     fn %[[VALUE_sysv_ms_start:[0-9]+]] @sysv_ms_start(%[[VALUE_n:[0-9]+]] n: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: ptr<i8> [storage=automatic];
// IR-NEXT:         va_start<win64>(%[[VALUE_ap]]);
// IR-NEXT:         va_end<win64>(%[[VALUE_ap]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_win64_native_start:[0-9]+]] @win64_native_start(%[[VALUE_n_2:[0-9]+]] n: i32, ...) -> void [linkage=external] [abi=win64 ms_abi(scalar) -> void] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_ap_2:[0-9]+]] ap: va_list [storage=automatic];
// IR-NEXT:         va_start(%[[VALUE_ap_2]]);
// IR-NEXT:         va_end(%[[VALUE_ap_2]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_fixed_start:[0-9]+]] @fixed_start(%[[VALUE_n_3:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_ap_3:[0-9]+]] ap: va_list [storage=automatic];
// IR-NEXT:         va_start(%[[VALUE_ap_3]]);
// IR-NEXT:         va_end(%[[VALUE_ap_3]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
