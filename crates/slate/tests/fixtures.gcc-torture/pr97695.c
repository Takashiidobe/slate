int *a, b, **c = &a, d, e;

int f(int g, int h) { return !h || (g && h == 1) ? 0 : g / h; }

static void *i(int g) {
  while (e < 2)
    if (!f(g, 9)) {
      while (b)
        ;
      return 0;
    }
  return 0;
}

void j() {
  i(1);
  *c = i(d);
}

int main() {
  j();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: static mut a: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: *mut *mut i32 = unsafe { std::ptr::addr_of_mut!(a) };
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn f(mut g: i32, mut h: i32) -> i32 {
// CHECK-NEXT:     if !(h != (0 as i32)) || g != (0 as i32) && h == (1 as i32) {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         g / h
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn i(mut g: i32) -> *mut () {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { e }) < (2 as i32) {
// CHECK-NEXT:         if !(f(g, 9 as i32) != (0 as i32)) {
// CHECK-NEXT:             '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { b }) != (0 as i32) {
// CHECK-NEXT:                 {}
// CHECK-NEXT:             }
// CHECK-NEXT:             return std::ptr::null_mut::<()>();
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     std::ptr::null_mut::<()>()
// CHECK-NEXT: }
// CHECK-NEXT: fn j() {
// CHECK-NEXT:     i(1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { c } = i(unsafe { d }) as *mut i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     j();
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
