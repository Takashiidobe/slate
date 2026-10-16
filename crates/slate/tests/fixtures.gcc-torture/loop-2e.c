void abort(void);

void f(int *p, int **q) {
  int i;
  for (i = 0; i < 40; i++) {
    *q++ = &p[i];
  }
}

int main() {
  void         *p;
  int          *q[40];
  __SIZE_TYPE__ start;

  /* Find the signed middle of the address space.  */
  if (sizeof(start) == sizeof(int))
    start = (__SIZE_TYPE__)__INT_MAX__;
  else if (sizeof(start) == sizeof(long))
    start = (__SIZE_TYPE__)__LONG_MAX__;
  else if (sizeof(start) == sizeof(long long))
    start = (__SIZE_TYPE__)__LONG_LONG_MAX__;
  else
    return 0;

  /* Arbitrarily align the pointer.  */
  start &= -32;

  /* Pretend that's good enough to start address arithmetic.  */
  p = (void *)start;

  /* Verify that GIV replacement computes the correct results.  */
  q[39] = 0;
  f(p, q);
  if (q[39] != (int *)p + 39)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut p: *mut i32, mut q: *mut *mut i32) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (40 as i32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: *mut *mut i32 = q;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: *mut *mut i32 = unsafe { [[V_0]].offset((1 as i32) as isize) };
// CHECK-NEXT:         q = [[V_1]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_0]] = unsafe { p.offset(i as isize) };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut p: *mut () = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut q: __SlateAlign16<[*mut i32; 40]> = __SlateAlign16([0 as *mut i32; 40]);
// CHECK-NEXT:     let mut start: u64 = 0 as u64;
// CHECK-NEXT:     // Find the signed middle of the address space.
// CHECK-NEXT:     if (8 as u64) == (4 as u64) {
// CHECK-NEXT:         start = ((2147483647 as i32) as i64) as u64;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if (8 as u64) == (8 as u64) {
// CHECK-NEXT:             start = (9223372036854775807 as i64) as u64;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if (8 as u64) == (8 as u64) {
// CHECK-NEXT:                 start = (9223372036854775807 as i64) as u64;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 return 0 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     // Arbitrarily align the pointer.
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u64 = start;
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u64 = [[V_4]] & ((-(32 as i32) as i64) as u64);
// CHECK-NEXT:     start = [[V_5]];
// CHECK-NEXT:     // Pretend that's good enough to start address arithmetic.
// CHECK-NEXT:     p = start as *mut ();
// CHECK-NEXT:     // Verify that GIV replacement computes the correct results.
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (q.0.as_mut_ptr() as *mut *mut i32).offset((39 as i32) as isize) } =
// CHECK-NEXT:             std::ptr::null_mut::<i32>();
// CHECK-NEXT:     }
// CHECK-NEXT:     f(p as *mut i32, q.0.as_mut_ptr() as *mut *mut i32);
// CHECK-NEXT:     if (unsafe { *unsafe { (q.0.as_mut_ptr() as *mut *mut i32).offset((39 as i32) as isize) } })
// CHECK-NEXT:         != unsafe { (p as *mut i32).offset((39 as i32) as isize) }
// CHECK-NEXT:     {
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
