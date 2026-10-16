/* PR target/122000 */

char                                                                    c = 1;
__attribute__((aligned(sizeof(unsigned long long)))) unsigned long long ll;

int main() {
#if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8) &&                             \
    __SIZEOF_LONG_LONG__ == 8 && __CHAR_BIT__ == 8
  unsigned long long x = __sync_add_and_fetch(&ll, c + 0xfedcba9876543210ULL);
  if (x != 0xfedcba9876543211ULL)
    __builtin_abort();
#endif
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/122000
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i8 = (1 as i32) as i8;
// CHECK-EMPTY:
// CHECK-NEXT: static mut ll: u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u64 = 0 as u64;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] =
// CHECK-NEXT:             ((((unsafe { c }) as i32) as i64) as u64).wrapping_add(18364758544493064720 as u64);
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = std::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(ll) }) as *mut u64,
// CHECK-NEXT:         )
// CHECK-NEXT:         .fetch_add([[T_0]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_1]].wrapping_add([[T_0]])
// CHECK-NEXT:     };
// CHECK-NEXT:     x = [[V_0]];
// CHECK-NEXT:     if x != (18364758544493064721 as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
