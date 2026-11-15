#include <stdio.h>
#include <string.h>

struct pmu {
  int hybrid;
  unsigned long long regs[2][3];
};

unsigned long long regs[2][3];
static const unsigned long long source[2][3] = {{1, 2, 3}, {4, 5, 6}};

static int is_hybrid(struct pmu *pmu) { return pmu->hybrid; }

#define hybrid_var(_pmu, _var)                                                 \
  (*({                                                                         \
    typeof(&_var) __Fp = &_var;                                                \
    if (is_hybrid(_pmu))                                                       \
      __Fp = &(_pmu)->_var;                                                    \
    __Fp;                                                                      \
  }))

int main(void) {
  struct pmu hybrid = {1};
  struct pmu plain = {0};
  memcpy(hybrid_var(&hybrid, regs), source, sizeof(regs));
  memcpy(hybrid_var(&plain, regs), source, sizeof(regs));
  printf("%llu %llu %llu\n", hybrid.regs[1][2], plain.regs[1][2], regs[0][1]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! hybrid_var {
// CHECK-NEXT:     ($_pmu:expr, $_var:expr) => {
// CHECK-NEXT:         { let mut __v : * mut {{\[\[}}u64; 3]; 2]; let mut [[V_0:__v[0-9]+]] : * mut {{\[\[}}u64; 3]; 2] = unsafe
// CHECK-NEXT:         { core::ptr::addr_of_mut!($_var) }; if is_hybrid($_pmu) != 0 { [[V_0]] = unsafe {
// CHECK-NEXT:         core::ptr::addr_of_mut!((*$_pmu).regs) }; } __v = [[V_0]]; unsafe { (* __v)
// CHECK-NEXT:         .as_mut_ptr() as * mut [u64; 3] } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pmu {
// CHECK-NEXT:     hybrid: i32,
// CHECK-NEXT:     regs: {{\[\[}}u64; 3]; 2],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut regs: __SlateAlign16<{{\[\[}}u64; 3]; 2]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     core::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut source: __SlateAlign16<{{\[\[}}u64; 3]; 2]> = __SlateAlign16([
// CHECK-NEXT:     [1u64, 2u64, 3u64],
// CHECK-NEXT:     [4u64, 5u64, 6u64],
// CHECK-NEXT: ]);
// CHECK-NEXT: use slate_support::memcpy;
// CHECK-NEXT: fn is_hybrid(pmu: *mut pmu) -> i32 {
// CHECK-NEXT:     unsafe { (*pmu).hybrid }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut hybrid: pmu = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: pmu = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].hybrid = 1i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut plain: pmu = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: pmu = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].hybrid = 0i32;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of!(source.0) as *const [u64; 3]) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((hybrid_var!(core::ptr::addr_of_mut!(hybrid), regs.0) as *mut ())
// CHECK-NEXT:                 as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             ((core::mem::size_of::<{{\[\[}}u64; 3]; 2]>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of!(source.0) as *const [u64; 3]) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((hybrid_var!(core::ptr::addr_of_mut!(plain), regs.0) as *mut ())
// CHECK-NEXT:                 as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             ((core::mem::size_of::<{{\[\[}}u64; 3]; 2]>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%llu %llu %llu\n".as_ptr(),
// CHECK-NEXT:             hybrid.regs[1][2],
// CHECK-NEXT:             plain.regs[1][2],
// CHECK-NEXT:             regs.0[0][1],
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
