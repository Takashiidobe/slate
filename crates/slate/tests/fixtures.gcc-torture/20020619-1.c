void abort(void);
#if (__SIZEOF_INT__ == 4)
typedef int int32;
#elif (__SIZEOF_LONG__ == 4)
typedef long int32;
#else
#error Add target support for int32
#endif
static int32 ref(void) {
  union {
    char  c[5];
    int32 i;
  } u;

  __builtin_memset(&u, 0, sizeof(u));
  u.c[0] = 1;
  u.c[1] = 2;
  u.c[2] = 3;
  u.c[3] = 4;

  return u.i;
}

int main() {
  int32 b = ref();
  if (b != 0x01020304 && b != 0x04030201)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     c: [i8; 5],
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn r#ref() -> i32 {
// CHECK-NEXT:     let mut u: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memset(std::ptr::addr_of_mut!(u) as *mut (), 0 as i32, 8 as u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { u.c.as_mut_ptr() as *mut i8 }.offset((0 as i32) as isize) } = (1
// CHECK-NEXT:             as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { u.c.as_mut_ptr() as *mut i8 }.offset((1 as i32) as isize) } = (2
// CHECK-NEXT:             as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { u.c.as_mut_ptr() as *mut i8 }.offset((2 as i32) as isize) } = (3
// CHECK-NEXT:             as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { u.c.as_mut_ptr() as *mut i8 }.offset((3 as i32) as isize) } = (4
// CHECK-NEXT:             as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { u.i }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut b: i32 = r#ref();
// CHECK-NEXT:     if b != (16909060 as i32) && b != (67305985 as i32) {
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
