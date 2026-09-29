
//  enum CallingConv {
//    CC_C,           // __attribute__((cdecl))
//    CC_X86StdCall,  // __attribute__((stdcall))
//    CC_X86FastCall, // __attribute__((fastcall))
//    CC_X86ThisCall, // __attribute__((thiscall))
//    CC_X86VectorCall, // __attribute__((vectorcall))
//    CC_X86Pascal,   // __attribute__((pascal))
//    CC_Win64,       // __attribute__((ms_abi))
//    CC_X86_64SysV,  // __attribute__((sysv_abi))
//    CC_X86RegCall, // __attribute__((regcall))
//    CC_AAPCS,       // __attribute__((pcs("aapcs")))
//    CC_AAPCS_VFP,   // __attribute__((pcs("aapcs-vfp")))
//    CC_IntelOclBicc, // __attribute__((intel_ocl_bicc))
//    CC_OpenCLKernel, // inferred for OpenCL kernels
//    CC_Swift,        // __attribute__((swiftcall))
//    CC_SwiftAsync,   // __attribute__((swiftasynccall))
//    CC_PreserveMost, // __attribute__((preserve_most))
//    CC_PreserveAll,  // __attribute__((preserve_all))
//    CC_PreserveNone,  // __attribute__((preserve_none))
//  };

#ifdef __x86_64__

#ifdef __linux__
__attribute__((ms_abi)) int add_msabi(int a, int b) {
  return a+b;
}

__attribute__((regcall)) int add_regcall(int a, int b) {
  return a+b;
}

__attribute__((preserve_most)) int add_preserve_most(int a, int b) {
  return a+b;
}

__attribute__((preserve_all)) int add_preserve_all(int a, int b) {
  return a+b;
}

__attribute__((preserve_none)) int add_preserve_none(int a, int b) {
  return a+b;
}

__attribute__((swiftcall)) int add_swiftcall(int a, int b) {
  return a+b;
}

__attribute__((swiftasynccall)) int add_swiftasynccall(int a, int b) {
  return a+b;
}

__attribute__((intel_ocl_bicc)) int add_inteloclbicc(int a, int b) {
  return a+b;
}
#endif

#if defined(_WIN64) || defined(__CYGWIN__)
__attribute__((sysv_abi)) int add_sysvabi(int a, int b) {
  return a+b;
}
#endif

#endif

#ifdef __i386__
__attribute__((stdcall)) int add_stdcall(int a, int b) {
  return a+b;
}

__attribute__((fastcall)) int add_fastcall(int a, int b) {
  return a+b;
}

__attribute__((thiscall)) int add_thiscall(int a, int b) {
  return a+b;
}

__attribute__((vectorcall)) int add_vectorcall(int a, int b) {
  return a+b;
}

__attribute__((pascal)) int add_pascal(int a, int b) {
  return a+b;
}
#endif

#ifdef __arm__
__attribute__((pcs("aapcs"))) int add_aapcs(int a, int b) {
  return a+b;
}

__attribute__((pcs("aapcs-vfp"))) int add_aapcs_vfp(int a, int b) {
  return a+b;
}
#endif

#ifdef __SPIRV__
int add_spir(int a, int b) {
  return a+b;
}
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_add_sysvabi:[0-9]+]] @add_sysvabi(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
