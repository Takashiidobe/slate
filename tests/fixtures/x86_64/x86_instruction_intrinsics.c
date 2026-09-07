#include <immintrin.h>
#include <stdio.h>

__attribute__((target("sse4.2"))) static unsigned long long crc32_probe(void) {
  unsigned int crc = _mm_crc32_u8(0u, 0x12u);
  crc              = _mm_crc32_u16(crc, 0x3456u);
  crc              = _mm_crc32_u32(crc, 0x789abcdeu);
  return _mm_crc32_u64(crc, 0x123456789abcdef0ull);
}

int main(void) {
  unsigned int auxiliary = 0;
  __builtin_ia32_pause();
  __builtin_ia32_lfence();
  unsigned long long first = __builtin_ia32_rdtsc();
  __builtin_ia32_mfence();
  unsigned long long second = __builtin_ia32_rdtscp(&auxiliary);
  __builtin_ia32_sfence();
  printf("%d %d %llu\n", first != 0, second >= first, crc32_probe());
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU: #![feature(abi_unadjusted)]
// LOWERING-X86_64-GNU-NEXT: #![feature(link_llvm_intrinsics)]
// LOWERING-X86_64-GNU-NEXT: #![feature(c_variadic)]
// LOWERING-X86_64-GNU-NEXT: #![allow(
// LOWERING-X86_64-GNU-NEXT:     dead_code,
// LOWERING-X86_64-GNU-NEXT:     unused,
// LOWERING-X86_64-GNU-NEXT:     non_camel_case_types,
// LOWERING-X86_64-GNU-NEXT:     non_snake_case,
// LOWERING-X86_64-GNU-NEXT:     non_upper_case_globals,
// LOWERING-X86_64-GNU-NEXT:     arithmetic_overflow,
// LOWERING-X86_64-GNU-NEXT:     unconditional_panic,
// LOWERING-X86_64-GNU-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-X86_64-GNU-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-X86_64-GNU-NEXT:     unused_comparisons
// LOWERING-X86_64-GNU-NEXT: )]
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[repr(C)]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-X86_64-GNU-NEXT:     __slate_anon_0: u64,
// LOWERING-X86_64-GNU-NEXT:     __slate_anon_1: u32,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: unsafe extern "C" {
// LOWERING-X86_64-GNU-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn main() {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_intrinsic_x86_sse2_pause_8e00570d6f1f8c6c() };
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_intrinsic_x86_sse2_lfence_b6f7983e420a3939() };
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { __slate_intrinsic_x86_rdtsc_319a3f94c908d021() };
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_intrinsic_x86_sse2_mfence_bb861e7376322ce() };
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_intrinsic_x86_rdtscp_a57815630f0a5a97() };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.__slate_anon_1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}}.__slate_anon_0;
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_intrinsic_x86_sse_sfence_f8b270d178b3d220() };
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %llu\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} >= {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { crc32_probe() };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[target_feature(enable = "popcnt,sse3,sse4.1,sse4.2,ssse3")]
// LOWERING-X86_64-GNU-NEXT: unsafe fn crc32_probe() -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 18;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { _mm_crc32_u8({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u16 = 13398;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { _mm_crc32_u16({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = 2023406814;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { _mm_crc32_u32({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = 1311768467463790320u64;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { _mm_crc32_u64({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[target_feature(enable = "sse4.2")]
// LOWERING-X86_64-GNU-NEXT: unsafe fn _mm_crc32_u8({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u8) -> u32 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { __slate_intrinsic_x86_sse42_crc32_32_8_f27bf8581dad0801({{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[target_feature(enable = "sse4.2")]
// LOWERING-X86_64-GNU-NEXT: unsafe fn _mm_crc32_u16({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u16) -> u32 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { __slate_intrinsic_x86_sse42_crc32_32_16_658f6bf45a185a4a({{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[target_feature(enable = "sse4.2")]
// LOWERING-X86_64-GNU-NEXT: unsafe fn _mm_crc32_u32({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32) -> u32 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { __slate_intrinsic_x86_sse42_crc32_32_32_f5e6b09e791bc818({{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[target_feature(enable = "sse4.2")]
// LOWERING-X86_64-GNU-NEXT: unsafe fn _mm_crc32_u64({{arg[0-9]+}}: u64, {{arg[0-9]+}}: u64) -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { __slate_intrinsic_x86_sse42_crc32_64_64_a6b1e708219fb1bb({{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: unsafe extern "unadjusted" {
// LOWERING-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.rdtsc"]
// LOWERING-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_rdtsc_319a3f94c908d021() -> u64;
// LOWERING-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.rdtscp"]
// LOWERING-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_rdtscp_a57815630f0a5a97() -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.sse2.lfence"]
// LOWERING-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_sse2_lfence_b6f7983e420a3939();
// LOWERING-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.sse2.mfence"]
// LOWERING-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_sse2_mfence_bb861e7376322ce();
// LOWERING-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.sse2.pause"]
// LOWERING-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_sse2_pause_8e00570d6f1f8c6c();
// LOWERING-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.sse42.crc32.32.16"]
// LOWERING-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_sse42_crc32_32_16_658f6bf45a185a4a(_0: u32, _1: u16) -> u32;
// LOWERING-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.sse42.crc32.32.32"]
// LOWERING-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_sse42_crc32_32_32_f5e6b09e791bc818(_0: u32, _1: u32) -> u32;
// LOWERING-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.sse42.crc32.32.8"]
// LOWERING-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_sse42_crc32_32_8_f27bf8581dad0801(_0: u32, _1: u8) -> u32;
// LOWERING-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.sse42.crc32.64.64"]
// LOWERING-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_sse42_crc32_64_64_a6b1e708219fb1bb(_0: u64, _1: u64) -> u64;
// LOWERING-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.sse.sfence"]
// LOWERING-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_sse_sfence_f8b270d178b3d220();
// LOWERING-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU: #![feature(abi_unadjusted)]
// REWRITES-X86_64-GNU-NEXT: #![feature(link_llvm_intrinsics)]
// REWRITES-X86_64-GNU-NEXT: #![feature(c_variadic)]
// REWRITES-X86_64-GNU-NEXT: #![allow(
// REWRITES-X86_64-GNU-NEXT:     dead_code,
// REWRITES-X86_64-GNU-NEXT:     unused,
// REWRITES-X86_64-GNU-NEXT:     non_camel_case_types,
// REWRITES-X86_64-GNU-NEXT:     non_snake_case,
// REWRITES-X86_64-GNU-NEXT:     non_upper_case_globals,
// REWRITES-X86_64-GNU-NEXT:     arithmetic_overflow,
// REWRITES-X86_64-GNU-NEXT:     unconditional_panic,
// REWRITES-X86_64-GNU-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-X86_64-GNU-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-X86_64-GNU-NEXT:     unused_comparisons
// REWRITES-X86_64-GNU-NEXT: )]
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[repr(C)]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-X86_64-GNU-NEXT:     __slate_anon_0: u64,
// REWRITES-X86_64-GNU-NEXT:     __slate_anon_1: u32,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn main() {
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         unsafe { __slate_intrinsic_x86_sse2_pause_8e00570d6f1f8c6c() };
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         unsafe { __slate_intrinsic_x86_sse2_lfence_b6f7983e420a3939() };
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { __slate_intrinsic_x86_rdtsc_319a3f94c908d021() };
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         unsafe { __slate_intrinsic_x86_sse2_mfence_bb861e7376322ce() };
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_intrinsic_x86_rdtscp_a57815630f0a5a97() };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}}.__slate_anon_0;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         unsafe { __slate_intrinsic_x86_sse_sfence_f8b270d178b3d220() };
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         printf(
// REWRITES-X86_64-GNU-NEXT:             c"%d %d %llu\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:             ({{__v[0-9]+}} != 0) as i32,
// REWRITES-X86_64-GNU-NEXT:             ({{__v[0-9]+}} >= {{__v[0-9]+}}) as i32,
// REWRITES-X86_64-GNU-NEXT:             unsafe { crc32_probe() },
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     std::process::exit(0 as i32);
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[target_feature(enable = "popcnt,sse3,sse4.1,sse4.2,ssse3")]
// REWRITES-X86_64-GNU-NEXT: unsafe fn crc32_probe() -> u64 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { _mm_crc32_u8(0, 18) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { _mm_crc32_u16({{__v[0-9]+}}, 13398) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { _mm_crc32_u32({{__v[0-9]+}}, 2023406814) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u64 = 1311768467463790320u64;
// REWRITES-X86_64-GNU-NEXT:     unsafe { _mm_crc32_u64({{__v[0-9]+}} as u64, {{__v[0-9]+}}) }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[target_feature(enable = "sse4.2")]
// REWRITES-X86_64-GNU-NEXT: unsafe fn _mm_crc32_u8({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u8) -> u32 {
// REWRITES-X86_64-GNU-NEXT:     unsafe { __slate_intrinsic_x86_sse42_crc32_32_8_f27bf8581dad0801({{arg[0-9]+}}, {{arg[0-9]+}}) }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[target_feature(enable = "sse4.2")]
// REWRITES-X86_64-GNU-NEXT: unsafe fn _mm_crc32_u16({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u16) -> u32 {
// REWRITES-X86_64-GNU-NEXT:     unsafe { __slate_intrinsic_x86_sse42_crc32_32_16_658f6bf45a185a4a({{arg[0-9]+}}, {{arg[0-9]+}}) }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[target_feature(enable = "sse4.2")]
// REWRITES-X86_64-GNU-NEXT: unsafe fn _mm_crc32_u32({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32) -> u32 {
// REWRITES-X86_64-GNU-NEXT:     unsafe { __slate_intrinsic_x86_sse42_crc32_32_32_f5e6b09e791bc818({{arg[0-9]+}}, {{arg[0-9]+}}) }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[target_feature(enable = "sse4.2")]
// REWRITES-X86_64-GNU-NEXT: unsafe fn _mm_crc32_u64({{arg[0-9]+}}: u64, {{arg[0-9]+}}: u64) -> u64 {
// REWRITES-X86_64-GNU-NEXT:     unsafe { __slate_intrinsic_x86_sse42_crc32_64_64_a6b1e708219fb1bb({{arg[0-9]+}}, {{arg[0-9]+}}) }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "unadjusted" {
// REWRITES-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.rdtsc"]
// REWRITES-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_rdtsc_319a3f94c908d021() -> u64;
// REWRITES-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.rdtscp"]
// REWRITES-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_rdtscp_a57815630f0a5a97() -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.sse2.lfence"]
// REWRITES-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_sse2_lfence_b6f7983e420a3939();
// REWRITES-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.sse2.mfence"]
// REWRITES-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_sse2_mfence_bb861e7376322ce();
// REWRITES-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.sse2.pause"]
// REWRITES-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_sse2_pause_8e00570d6f1f8c6c();
// REWRITES-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.sse42.crc32.32.16"]
// REWRITES-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_sse42_crc32_32_16_658f6bf45a185a4a(_0: u32, _1: u16) -> u32;
// REWRITES-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.sse42.crc32.32.32"]
// REWRITES-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_sse42_crc32_32_32_f5e6b09e791bc818(_0: u32, _1: u32) -> u32;
// REWRITES-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.sse42.crc32.32.8"]
// REWRITES-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_sse42_crc32_32_8_f27bf8581dad0801(_0: u32, _1: u8) -> u32;
// REWRITES-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.sse42.crc32.64.64"]
// REWRITES-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_sse42_crc32_64_64_a6b1e708219fb1bb(_0: u64, _1: u64) -> u64;
// REWRITES-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.sse.sfence"]
// REWRITES-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_sse_sfence_f8b270d178b3d220();
// REWRITES-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites
