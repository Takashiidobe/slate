void abort(void);

static const unsigned char f[] = "\0\377";
static const unsigned char g[] = "\0ÿ";

int main(void) {
  if (sizeof f != 3 || sizeof g != 3)
    abort();
  if (f[0] != g[0])
    abort();
  if (f[1] != g[1])
    abort();
  if (f[2] != g[2])
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut f: [u8; 3] = [0 as u8, 255 as u8, 0 as u8];
// CHECK-NEXT: static mut g: [u8; 3] = [0 as u8, 255 as u8, 0 as u8];
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (3 as u64) != (((3 as i32) as i64) as u64)
// CHECK-NEXT:         || (3 as u64) != (((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of!(f) as *const u8 }.offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) as u32) as i32)
// CHECK-NEXT:         != (((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of!(g) as *const u8 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of!(f) as *const u8 }.offset((1 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) as u32) as i32)
// CHECK-NEXT:         != (((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of!(g) as *const u8 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of!(f) as *const u8 }.offset((2 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) as u32) as i32)
// CHECK-NEXT:         != (((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of!(g) as *const u8 }.offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as u32) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
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
