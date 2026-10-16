extern void abort(void);

int arr[] = {1, 2, 3, 4};
int count = 0;

int __attribute__((noinline)) incr(void) { return ++count; }

int main() {
  arr[count++] = incr();
  if (count != 2 || arr[count] != 3)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut arr: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT:     2 as i32,
// CHECK-NEXT:     3 as i32,
// CHECK-NEXT:     4 as i32,
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut count: i32 = 0 as i32;
// CHECK-NEXT: fn incr() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { count };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_1]]
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = unsafe { count };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(arr.0) as *mut i32 }.offset([[V_2]] as isize)
// CHECK-NEXT:         } = incr();
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { count }) != (2 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(arr.0) as *mut i32 }
// CHECK-NEXT:                     .offset((unsafe { count }) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != (3 as i32)
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
