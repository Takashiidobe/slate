void abort(void);
void exit(int);

void put_field(unsigned int start, unsigned int len) {
  int cur_bitshift = ((start + len) % 8) - 8;
  if (cur_bitshift > -8)
    exit(0);
}

int main() {
  put_field(0, 1);
  abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn put_field(mut start: u32, mut len: u32) {
// CHECK-NEXT:     let mut cur_bitshift: i32 = (start.wrapping_add(len) % ((8 as i32) as u32))
// CHECK-NEXT:         .wrapping_sub((8 as i32) as u32) as i32;
// CHECK-NEXT:     if cur_bitshift > -(8 as i32) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     put_field((0 as i32) as u32, (1 as i32) as u32);
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT:     0
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
