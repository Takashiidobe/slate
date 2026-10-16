void abort(void);
void exit(int);

int g(unsigned long long int *v, int n, unsigned int a[], int b) {
  int cnt;
  *v = 0;
  for (cnt = 0; cnt < n; ++cnt)
    *v = *v * b + a[cnt];
  return n;
}

int main(void) {
  int                    res;
  unsigned int           ar[] = {10, 11, 12, 13, 14};
  unsigned long long int v;

  res = g(&v, sizeof(ar) / sizeof(ar[0]), ar, 16);
  if (v != 0xabcdeUL)
    abort();

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
// CHECK-NEXT: fn g(mut v: &mut u64, mut n: i32, mut a: *mut u32, mut b: i32) -> i32 {
// CHECK-NEXT:     let mut cnt: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(v as *mut u64) = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while cnt < n {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(v as *mut u64) = unsafe { *(v as *mut u64) }
// CHECK-NEXT:                 .wrapping_mul((b as i64) as u64)
// CHECK-NEXT:                 .wrapping_add((unsafe { *unsafe { a.offset(cnt as isize) } }) as u64);
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = cnt;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         cnt = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     n
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut res: i32 = 0 as i32;
// CHECK-NEXT:     let mut ar: __SlateAlign16<[u32; 5]> = __SlateAlign16([
// CHECK-NEXT:         (10 as i32) as u32,
// CHECK-NEXT:         (11 as i32) as u32,
// CHECK-NEXT:         (12 as i32) as u32,
// CHECK-NEXT:         (13 as i32) as u32,
// CHECK-NEXT:         (14 as i32) as u32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut v: u64 = 0 as u64;
// CHECK-NEXT:     res = g(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(v)) },
// CHECK-NEXT:         (((20 as u64) / (4 as u64)) as u32) as i32,
// CHECK-NEXT:         ar.0.as_mut_ptr() as *mut u32,
// CHECK-NEXT:         16 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     if v != (703710 as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
