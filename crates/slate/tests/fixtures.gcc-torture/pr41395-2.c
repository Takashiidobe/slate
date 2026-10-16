struct VEC_char_base {
  unsigned num;
  unsigned alloc;
  union {
    short vec[1];
    struct {
      int i;
      int j;
      int k;
    } a;
  } u;
};

short __attribute__((noinline)) foo(struct VEC_char_base *p, int i) {
  short *q;
  p->u.vec[i] = 0;
  q           = &p->u.vec[16];
  *q          = 1;
  return p->u.vec[i];
}

extern void  abort(void);
extern void *malloc(__SIZE_TYPE__);

int main() {
  struct VEC_char_base *p = malloc(sizeof(struct VEC_char_base) + 256);
  if (foo(p, 16) != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn malloc([[V_0:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct VEC_char_base {
// CHECK-NEXT:     num: u32,
// CHECK-NEXT:     alloc: u32,
// CHECK-NEXT:     u: [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0]] {
// CHECK-NEXT:     vec: [i16; 1],
// CHECK-NEXT:     a: [[SLATERECORD_1:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1]] {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     j: i32,
// CHECK-NEXT:     k: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut p: *mut VEC_char_base, mut i: i32) -> i16 {
// CHECK-NEXT:     let mut q: *mut i16 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { (*p).u.vec.as_mut_ptr() as *mut i16 }.offset(i as isize) } = (0
// CHECK-NEXT:             as i32) as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     q = unsafe {
// CHECK-NEXT:         unsafe { (*p).u.vec.as_mut_ptr() as *mut i16 }.offset((16 as i32) as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *q = (1 as i32) as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { (*p).u.vec.as_mut_ptr() as *mut i16 }.offset(i as isize) }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut p: *mut VEC_char_base = (unsafe {
// CHECK-NEXT:         malloc((20 as u64).wrapping_add(((256 as i32) as i64) as u64))
// CHECK-NEXT:     }) as *mut VEC_char_base;
// CHECK-NEXT:     if (foo(p, 16 as i32) as i32) != (1 as i32) {
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
