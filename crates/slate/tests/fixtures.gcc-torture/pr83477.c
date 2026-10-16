int yf = 0;

void pl(int q5, int nd) {
  unsigned int hp = q5;
  int          zx = (q5 == 0) ? hp : (hp / q5);

  yf = ((nd < 2) * zx != 0) ? nd : 0;
}

int main(void) {
  pl(1, !yf);
  if (yf != 1)
    __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut yf: i32 = 0 as i32;
// CHECK-NEXT: fn pl(mut q5: i32, mut nd: i32) {
// CHECK-NEXT:     let mut hp: u32 = q5 as u32;
// CHECK-NEXT:     let mut zx: i32 = (if q5 == (0 as i32) { hp } else { hp / (q5 as u32) }) as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         yf = if ((nd < (2 as i32)) as i32) * zx != (0 as i32) { nd } else { 0 as i32 };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     pl(1 as i32, !((unsafe { yf }) != (0 as i32)) as i32);
// CHECK-NEXT:     if (unsafe { yf }) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
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
