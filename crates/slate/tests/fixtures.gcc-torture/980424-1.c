void abort(void);
void exit(int);

int i, a[99];

void f(int one) {
  if (one != 1)
    abort();
}

void g() { f(a[i & 0x3f]); }

int main() {
  a[0] = 1;
  i    = 0x40;
  g();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut a: __SlateAlign16<[i32; 99]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT: fn f(mut one: i32) {
// CHECK-NEXT:     if one != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn g() {
// CHECK-NEXT:     f(unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }
// CHECK-NEXT:                 .offset(((unsafe { i }) & (63 as i32)) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         } = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = 64 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     g();
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
