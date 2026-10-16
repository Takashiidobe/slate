/* PR middle-end/19857 */

typedef struct {
  char c[8];
} V
#ifdef __ELF__
    __attribute__((aligned(8)))
#endif
    ;
typedef __SIZE_TYPE__ size_t;
V                     v;
void                  abort(void);

int main(void) {
  V *w = &v;
  if (((size_t)((float *)((size_t)w & ~(size_t)3)) % 8) != 0 ||
      ((size_t)w & 1)) {
#ifndef __ELF__
    if (((size_t)&v & 7) == 0)
#endif
      abort();
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/19857
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     c: [i8; 8],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: struct __SlateAlign8<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut v: __SlateAlign8<[[SLATERECORD_0]]> = __SlateAlign8(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut w: *mut [[SLATERECORD_0]] = unsafe { std::ptr::addr_of_mut!(v.0) };
// CHECK-NEXT:     if ((((w as u64) & !(((3 as i32) as i64) as u64)) as *mut f32) as u64)
// CHECK-NEXT:         % (((8 as i32) as i64) as u64)
// CHECK-NEXT:         != (((0 as i32) as i64) as u64)
// CHECK-NEXT:         || (w as u64) & (((1 as i32) as i64) as u64) != (0 as u64)
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
