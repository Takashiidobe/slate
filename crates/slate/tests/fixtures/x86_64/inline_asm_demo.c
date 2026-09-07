#include <stdio.h>

typedef unsigned int U32;

static int add_via_asm(int x) {
  __asm__ volatile("addl $5, %0" : "+r"(x));
  return x;
}

static U32 cpuid_leaf7_feature(void) {
  U32 eax, ebx, ecx;
  __asm__("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx) : "a"(7), "c"(0) : "edx");
  return ebx != 0 || eax != 0 || ecx != 0;
}

int main(void) {
  printf("%d %u\n", add_via_asm(37), cpuid_leaf7_feature());
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU: #![feature(c_variadic)]
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
// LOWERING-X86_64-GNU-NEXT:     __slate_anon_0: u32,
// LOWERING-X86_64-GNU-NEXT:     __slate_anon_1: u32,
// LOWERING-X86_64-GNU-NEXT:     __slate_anon_2: u32,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: unsafe extern "C" {
// LOWERING-X86_64-GNU-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn main() {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %u\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 37;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = add_via_asm({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = cpuid_leaf7_feature();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn add_via_asm({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         core::arch::asm!("addl $5, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn cpuid_leaf7_feature() -> u32 {
// LOWERING-X86_64-GNU-NEXT:     let mut eax: u32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let mut ecx: u32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let mut __asm_result: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-X86_64-GNU-NEXT:         __slate_anon_0: 0,
// LOWERING-X86_64-GNU-NEXT:         __slate_anon_1: 0,
// LOWERING-X86_64-GNU-NEXT:         __slate_anon_2: 0,
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32;
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         core::arch::asm!("push %rbx\n\tcpuid\n\tmov %ebx, %edi\n\tpop %rbx", lateout("eax") {{__v[0-9]+}}, lateout("edi") {{__v[0-9]+}}, lateout("ecx") {{__v[0-9]+}}, in("eax") {{__v[0-9]+}}, in("ecx") {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     eax = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     ecx = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: bool = true;
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:     } else {
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: u32 = eax;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: u32 = 0;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: bool = true;
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:     } else {
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: u32 = ecx;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: u32 = 0;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU: #![feature(c_variadic)]
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
// REWRITES-X86_64-GNU-NEXT:     __slate_anon_0: u32,
// REWRITES-X86_64-GNU-NEXT:     __slate_anon_1: u32,
// REWRITES-X86_64-GNU-NEXT:     __slate_anon_2: u32,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn main() {
// REWRITES-X86_64-GNU-NEXT:     unsafe { printf(c"%d %u\n".as_ptr(), add_via_asm(37), cpuid_leaf7_feature()) };
// REWRITES-X86_64-GNU-NEXT:     std::process::exit(0 as i32);
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn add_via_asm({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         core::arch::asm!("addl $5, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn cpuid_leaf7_feature() -> u32 {
// REWRITES-X86_64-GNU-NEXT:     let mut eax: u32 = 0;
// REWRITES-X86_64-GNU-NEXT:     let mut ecx: u32 = 0;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         core::arch::asm!("push %rbx\n\tcpuid\n\tmov %ebx, %edi\n\tpop %rbx", lateout("eax") {{__v[0-9]+}}, lateout("edi") {{__v[0-9]+}}, lateout("ecx") {{__v[0-9]+}}, in("eax") {{__v[0-9]+}}, in("ecx") {{__v[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     eax = {{__v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:     ecx = {{__v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} != 0 {
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: bool = true;
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:     } else {
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: bool = eax != 0;
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: bool = true;
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:     } else {
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: bool = ecx != 0;
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     ({{__v[0-9]+}} as i32) as u32
// REWRITES-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites
