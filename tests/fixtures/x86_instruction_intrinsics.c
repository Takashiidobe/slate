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
// LOWERING: #![feature(abi_unadjusted)]
// LOWERING-NEXT: #![feature(link_llvm_intrinsics)]
// LOWERING-NEXT: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:     __slate_anon_0: u64,
// LOWERING-NEXT:     __slate_anon_1: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "sse4.2")]
// LOWERING-NEXT: unsafe fn _mm_crc32_u8({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u8) -> u32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { __slate_intrinsic_x86_sse42_crc32_32_8_f27bf8581dad0801({{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "sse4.2")]
// LOWERING-NEXT: unsafe fn _mm_crc32_u16({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u16) -> u32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { __slate_intrinsic_x86_sse42_crc32_32_16_658f6bf45a185a4a({{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "sse4.2")]
// LOWERING-NEXT: unsafe fn _mm_crc32_u32({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32) -> u32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { __slate_intrinsic_x86_sse42_crc32_32_32_f5e6b09e791bc818({{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "sse4.2")]
// LOWERING-NEXT: unsafe fn _mm_crc32_u64({{arg[0-9]+}}: u64, {{arg[0-9]+}}: u64) -> u64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { __slate_intrinsic_x86_sse42_crc32_64_64_a6b1e708219fb1bb({{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "popcnt,sse3,sse4.1,sse4.2,ssse3")]
// LOWERING-NEXT: unsafe fn crc32_probe() -> u64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = 18;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { _mm_crc32_u8({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u16 = 13398;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { _mm_crc32_u16({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 2023406814;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { _mm_crc32_u32({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1311768467463790320u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { _mm_crc32_u64({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         unsafe { __slate_intrinsic_x86_sse2_pause_8e00570d6f1f8c6c() };
// LOWERING-NEXT:     }
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         unsafe { __slate_intrinsic_x86_sse2_lfence_b6f7983e420a3939() };
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { __slate_intrinsic_x86_rdtsc_319a3f94c908d021() };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         unsafe { __slate_intrinsic_x86_sse2_mfence_bb861e7376322ce() };
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_intrinsic_x86_rdtscp_a57815630f0a5a97() };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.__slate_anon_1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}}.__slate_anon_0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         unsafe { __slate_intrinsic_x86_sse_sfence_f8b270d178b3d220() };
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { crc32_probe() };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "unadjusted" {
// LOWERING-NEXT:     #[link_name = "llvm.x86.rdtsc"]
// LOWERING-NEXT:     fn __slate_intrinsic_x86_rdtsc_319a3f94c908d021() -> u64;
// LOWERING-NEXT:     #[link_name = "llvm.x86.rdtscp"]
// LOWERING-NEXT:     fn __slate_intrinsic_x86_rdtscp_a57815630f0a5a97() -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     #[link_name = "llvm.x86.sse2.lfence"]
// LOWERING-NEXT:     fn __slate_intrinsic_x86_sse2_lfence_b6f7983e420a3939();
// LOWERING-NEXT:     #[link_name = "llvm.x86.sse2.mfence"]
// LOWERING-NEXT:     fn __slate_intrinsic_x86_sse2_mfence_bb861e7376322ce();
// LOWERING-NEXT:     #[link_name = "llvm.x86.sse2.pause"]
// LOWERING-NEXT:     fn __slate_intrinsic_x86_sse2_pause_8e00570d6f1f8c6c();
// LOWERING-NEXT:     #[link_name = "llvm.x86.sse42.crc32.32.16"]
// LOWERING-NEXT:     fn __slate_intrinsic_x86_sse42_crc32_32_16_658f6bf45a185a4a(_0: u32, _1: u16) -> u32;
// LOWERING-NEXT:     #[link_name = "llvm.x86.sse42.crc32.32.32"]
// LOWERING-NEXT:     fn __slate_intrinsic_x86_sse42_crc32_32_32_f5e6b09e791bc818(_0: u32, _1: u32) -> u32;
// LOWERING-NEXT:     #[link_name = "llvm.x86.sse42.crc32.32.8"]
// LOWERING-NEXT:     fn __slate_intrinsic_x86_sse42_crc32_32_8_f27bf8581dad0801(_0: u32, _1: u8) -> u32;
// LOWERING-NEXT:     #[link_name = "llvm.x86.sse42.crc32.64.64"]
// LOWERING-NEXT:     fn __slate_intrinsic_x86_sse42_crc32_64_64_a6b1e708219fb1bb(_0: u64, _1: u64) -> u64;
// LOWERING-NEXT:     #[link_name = "llvm.x86.sse.sfence"]
// LOWERING-NEXT:     fn __slate_intrinsic_x86_sse_sfence_f8b270d178b3d220();
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(abi_unadjusted)]
// REWRITES-NEXT: #![feature(link_llvm_intrinsics)]
// REWRITES-NEXT: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:     __slate_anon_0: u64,
// REWRITES-NEXT:     __slate_anon_1: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "sse4.2")]
// REWRITES-NEXT: unsafe fn _mm_crc32_u8({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u8) -> u32 {
// REWRITES-NEXT:     unsafe { __slate_intrinsic_x86_sse42_crc32_32_8_f27bf8581dad0801({{arg[0-9]+}}, {{arg[0-9]+}}) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "sse4.2")]
// REWRITES-NEXT: unsafe fn _mm_crc32_u16({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u16) -> u32 {
// REWRITES-NEXT:     unsafe { __slate_intrinsic_x86_sse42_crc32_32_16_658f6bf45a185a4a({{arg[0-9]+}}, {{arg[0-9]+}}) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "sse4.2")]
// REWRITES-NEXT: unsafe fn _mm_crc32_u32({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32) -> u32 {
// REWRITES-NEXT:     unsafe { __slate_intrinsic_x86_sse42_crc32_32_32_f5e6b09e791bc818({{arg[0-9]+}}, {{arg[0-9]+}}) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "sse4.2")]
// REWRITES-NEXT: unsafe fn _mm_crc32_u64({{arg[0-9]+}}: u64, {{arg[0-9]+}}: u64) -> u64 {
// REWRITES-NEXT:     unsafe { __slate_intrinsic_x86_sse42_crc32_64_64_a6b1e708219fb1bb({{arg[0-9]+}}, {{arg[0-9]+}}) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "popcnt,sse3,sse4.1,sse4.2,ssse3")]
// REWRITES-NEXT: unsafe fn crc32_probe() -> u64 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { _mm_crc32_u8(0, 18) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { _mm_crc32_u16({{_v[0-9]+}}, 13398) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { _mm_crc32_u32({{_v[0-9]+}}, 2023406814) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = 1311768467463790320u64;
// REWRITES-NEXT:     unsafe { _mm_crc32_u64({{_v[0-9]+}} as u64, {{_v[0-9]+}}) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         unsafe { __slate_intrinsic_x86_sse2_pause_8e00570d6f1f8c6c() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         unsafe { __slate_intrinsic_x86_sse2_lfence_b6f7983e420a3939() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { __slate_intrinsic_x86_rdtsc_319a3f94c908d021() };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         unsafe { __slate_intrinsic_x86_sse2_mfence_bb861e7376322ce() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_intrinsic_x86_rdtscp_a57815630f0a5a97() };
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}}.__slate_anon_0;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         unsafe { __slate_intrinsic_x86_sse_sfence_f8b270d178b3d220() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %llu\n".as_ptr(),
// REWRITES-NEXT:             ({{_v[0-9]+}} != 0) as i32,
// REWRITES-NEXT:             ({{_v[0-9]+}} >= {{_v[0-9]+}}) as i32,
// REWRITES-NEXT:             unsafe { crc32_probe() },
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "unadjusted" {
// REWRITES-NEXT:     #[link_name = "llvm.x86.rdtsc"]
// REWRITES-NEXT:     fn __slate_intrinsic_x86_rdtsc_319a3f94c908d021() -> u64;
// REWRITES-NEXT:     #[link_name = "llvm.x86.rdtscp"]
// REWRITES-NEXT:     fn __slate_intrinsic_x86_rdtscp_a57815630f0a5a97() -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     #[link_name = "llvm.x86.sse2.lfence"]
// REWRITES-NEXT:     fn __slate_intrinsic_x86_sse2_lfence_b6f7983e420a3939();
// REWRITES-NEXT:     #[link_name = "llvm.x86.sse2.mfence"]
// REWRITES-NEXT:     fn __slate_intrinsic_x86_sse2_mfence_bb861e7376322ce();
// REWRITES-NEXT:     #[link_name = "llvm.x86.sse2.pause"]
// REWRITES-NEXT:     fn __slate_intrinsic_x86_sse2_pause_8e00570d6f1f8c6c();
// REWRITES-NEXT:     #[link_name = "llvm.x86.sse42.crc32.32.16"]
// REWRITES-NEXT:     fn __slate_intrinsic_x86_sse42_crc32_32_16_658f6bf45a185a4a(_0: u32, _1: u16) -> u32;
// REWRITES-NEXT:     #[link_name = "llvm.x86.sse42.crc32.32.32"]
// REWRITES-NEXT:     fn __slate_intrinsic_x86_sse42_crc32_32_32_f5e6b09e791bc818(_0: u32, _1: u32) -> u32;
// REWRITES-NEXT:     #[link_name = "llvm.x86.sse42.crc32.32.8"]
// REWRITES-NEXT:     fn __slate_intrinsic_x86_sse42_crc32_32_8_f27bf8581dad0801(_0: u32, _1: u8) -> u32;
// REWRITES-NEXT:     #[link_name = "llvm.x86.sse42.crc32.64.64"]
// REWRITES-NEXT:     fn __slate_intrinsic_x86_sse42_crc32_64_64_a6b1e708219fb1bb(_0: u64, _1: u64) -> u64;
// REWRITES-NEXT:     #[link_name = "llvm.x86.sse.sfence"]
// REWRITES-NEXT:     fn __slate_intrinsic_x86_sse_sfence_f8b270d178b3d220();
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
