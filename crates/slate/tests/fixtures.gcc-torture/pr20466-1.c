int f(int **, int *, int *, int **, int **) __attribute__((__noinline__));
int f(int **ipp, int *i1p, int *i2p, int **i3, int **i4) {
  **ipp = *i1p;
  *ipp  = i2p;
  *i3   = *i4;
  **ipp = 99;
  return 3;
}

extern void exit(int);
extern void abort(void);

int main(void) {
  int  i = 42, i1 = 66, i2 = 1, i3 = -1, i4 = 55;
  int *ip  = &i;
  int *i3p = &i3;
  int *i4p = &i4;

  f(&ip, &i1, &i2, &i3p, &i4p);
  if (i != 66 || ip != &i2 || i2 != 99 || i3 != -1 || i3p != i4p || i4 != 55)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn f(
// CHECK-NEXT:     mut ipp: &mut *mut i32,
// CHECK-NEXT:     mut i1p: &i32,
// CHECK-NEXT:     mut i2p: *mut i32,
// CHECK-NEXT:     mut i3: &mut *mut i32,
// CHECK-NEXT:     mut i4: &*mut i32,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { *(ipp as *mut *mut i32) } = unsafe { *(i1p as *const i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(ipp as *mut *mut i32) = i2p;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(i3 as *mut *mut i32) = unsafe { *(i4 as *const *mut i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { *(ipp as *mut *mut i32) } = 99 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     3 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 42 as i32;
// CHECK-NEXT:     let mut i1: i32 = 66 as i32;
// CHECK-NEXT:     let mut i2: i32 = 1 as i32;
// CHECK-NEXT:     let mut i3: i32 = -(1 as i32);
// CHECK-NEXT:     let mut i4: i32 = 55 as i32;
// CHECK-NEXT:     let mut ip: *mut i32 = std::ptr::addr_of_mut!(i);
// CHECK-NEXT:     let mut i3p: *mut i32 = std::ptr::addr_of_mut!(i3);
// CHECK-NEXT:     let mut i4p: *mut i32 = std::ptr::addr_of_mut!(i4);
// CHECK-NEXT:     f(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(ip)) },
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(i1)) },
// CHECK-NEXT:         std::ptr::addr_of_mut!(i2),
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(i3p)) },
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(i4p)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     if i != (66 as i32) || ip != std::ptr::addr_of_mut!(i2) || i2 != (99 as i32)
// CHECK-NEXT:         || i3 != -(1 as i32) || i3p != i4p || i4 != (55 as i32)
// CHECK-NEXT:     {
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
