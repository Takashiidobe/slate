extern void abort(void);

typedef unsigned long my_uintptr_t;

int check_a(my_uintptr_t tagged_ptr);

int __attribute__((noinline)) try_a(my_uintptr_t x) {
  my_uintptr_t  heap[2];
  my_uintptr_t *hp = heap;

  hp[0] = x;
  hp[1] = 0;
  return check_a((my_uintptr_t)(void *)((char *)hp + 1));
}

int __attribute__((noinline)) check_a(my_uintptr_t tagged_ptr) {
  my_uintptr_t *hp = (my_uintptr_t *)(void *)((char *)tagged_ptr - 1);

  if (hp[0] == 42 && hp[1] == 0)
    return 0;
  return -1;
}

int main(void) {
  if (try_a(42) < 0)
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
// CHECK-NEXT: fn try_a(mut x: u64) -> i32 {
// CHECK-NEXT:     let mut heap: __SlateAlign16<[u64; 2]> = __SlateAlign16([0 as u64; 2]);
// CHECK-NEXT:     let mut hp: *mut u64 = heap.0.as_mut_ptr() as *mut u64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { hp.offset((0 as i32) as isize) } = x;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { hp.offset((1 as i32) as isize) } = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     check_a(((unsafe { (hp as *mut i8).offset((1 as i32) as isize) }) as *mut ()) as u64)
// CHECK-NEXT: }
// CHECK-NEXT: fn check_a(mut tagged_ptr: u64) -> i32 {
// CHECK-NEXT:     let mut hp: *mut u64 = ((unsafe {
// CHECK-NEXT:         (tagged_ptr as *mut i8).offset(-((1 as i32) as isize))
// CHECK-NEXT:     }) as *mut ()) as *mut u64;
// CHECK-NEXT:     if (unsafe { *unsafe { hp.offset((0 as i32) as isize) } })
// CHECK-NEXT:         == (((42 as i32) as i64) as u64)
// CHECK-NEXT:         && (unsafe { *unsafe { hp.offset((1 as i32) as isize) } })
// CHECK-NEXT:             == (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     -(1 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if try_a(((42 as i32) as i64) as u64) < (0 as i32) {
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
