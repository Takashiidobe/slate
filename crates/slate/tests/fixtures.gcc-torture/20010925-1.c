extern void exit(int);
extern void abort(void);

extern void *memcpy(void *, const void *, __SIZE_TYPE__);
int          foo(void *, void *, unsigned int c);

int src[10];
int dst[10];

int main() {
  if (foo(dst, src, 10) != 0)
    abort();
  exit(0);
}

int foo(void *a, void *b, unsigned int c) {
  if (c == 0)
    return 1;

  memcpy(a, b, c);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn memcpy([[V_1:__v[0-9]+]]: *mut (), [[V_2:__v[0-9]+]]: *const (), [[V_3:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut src: __SlateAlign16<[i32; 10]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut dst: __SlateAlign16<[i32; 10]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i32 }) as *mut (),
// CHECK-NEXT:         (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i32 }) as *mut (),
// CHECK-NEXT:         (10 as i32) as u32,
// CHECK-NEXT:     ) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut a: *mut (), mut b: *mut (), mut c: u32) -> i32 {
// CHECK-NEXT:     if c == ((0 as i32) as u32) {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { memcpy(a, b as *const (), c as u64) };
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
