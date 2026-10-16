/* PR rtl-optimization/45912 */

extern void abort(void);

static void *__attribute__((noinline, noclone))
get_addr_base_and_unit_offset(void *base, long long *i) {
  *i = 0;
  return base;
}

static void *__attribute__((noinline, noclone))
build_int_cst(void *base, long long offset) {
  if (offset != 4)
    abort();

  return base;
}

static void *__attribute__((noinline, noclone))
build_ref_for_offset(void *base, long long offset) {
  long long base_offset;
  base = get_addr_base_and_unit_offset(base, &base_offset);
  return build_int_cst(base, base_offset + offset / 8);
}

int main(void) {
  void *ret = build_ref_for_offset((void *)0, 32);
  if (ret != (void *)0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/45912
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn get_addr_base_and_unit_offset(mut base: *mut (), mut i: &mut i64) -> *mut () {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(i as *mut i64) = (0 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     base
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn build_int_cst(mut base: *mut (), mut offset: i64) -> *mut () {
// CHECK-NEXT:     if offset != ((4 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     base
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn build_ref_for_offset(mut base: *mut (), mut offset: i64) -> *mut () {
// CHECK-NEXT:     let mut base_offset: i64 = 0 as i64;
// CHECK-NEXT:     base =
// CHECK-NEXT:         get_addr_base_and_unit_offset(base, unsafe { &mut (*std::ptr::addr_of_mut!(base_offset)) });
// CHECK-NEXT:     build_int_cst(base, base_offset + offset / ((8 as i32) as i64))
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut ret: *mut () = build_ref_for_offset(std::ptr::null_mut::<()>(), (32 as i32) as i64);
// CHECK-NEXT:     if ret != std::ptr::null_mut::<()>() {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
