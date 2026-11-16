int printf(const char *, ...);
unsigned long table[64];
#define const_ptr(sym) ({ __typeof__(sym) __ret; __asm__("mov %1,%0" : "=r"(__ret) : "r"(sym)); __ret; })
#define shift_right_32(val) ({ __typeof__(0u+(val)) __ret = (val); __asm__("shrl $12,%k0" : "+r"(__ret)); __ret; })
static unsigned long *table_ptr = table;
static unsigned long *d_hash(unsigned long hashlen) {
  return const_ptr(table_ptr) + shift_right_32(hashlen);
}
int main(void) {
  printf("%ld\n", (long)(d_hash(0x5000) - table));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-NEXT: static mut table: __SlateAlign16<[u64; 64]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     core::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut table_ptr: *mut u64 = unsafe { core::ptr::addr_of_mut!(table.0) as *mut u64 };
// CHECK-NEXT: fn d_hash(mut [[RET_0:__ret_[0-9]+]]: u64) -> *mut u64 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: *mut u64;
// CHECK-NEXT:     let mut __ret: *mut u64 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {1:r},{0:r}", lateout(reg) [[T_0]], in (reg) table_ptr as u64,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         __ret = unsafe { core::mem::transmute::<u64, *mut u64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_0]] = __ret;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "shrl $12,{0:e}", inlateout(reg) [[RET_0]] as u64 => [[T_1]], options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         [[RET_0]] = unsafe { core::mem::transmute::<u64, u64>([[T_1]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_1]] = [[RET_0]];
// CHECK-NEXT:     unsafe { [[V_0]].add([[V_1]] as usize) }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%ld\n".as_ptr(),
// CHECK-NEXT:             d_hash(20480u64)
// CHECK-NEXT:                 .offset_from((core::ptr::addr_of_mut!(table.0) as *mut u64) as *mut u64)
// CHECK-NEXT:                 as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
