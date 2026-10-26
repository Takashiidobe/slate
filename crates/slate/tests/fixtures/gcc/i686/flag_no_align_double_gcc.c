/* { dg-options "-malign-double -mno-align-double" } */
int printf(const char *, ...);

struct wide {
  char c;
  double d;
  long long l;
};

static unsigned offset(const void *base, const void *field) {
  return (unsigned)((const char *)field - (const char *)base);
}

int main(void) {
  struct wide w = {1, 2.5, 3};
  printf("%zu %zu %u %u\n", sizeof w, _Alignof(struct wide), offset(&w, &w.d), offset(&w, &w.l));
  printf("%d %g %lld\n", w.c, w.d, w.l);
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-malign-double -mno-align-double" }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct wide {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     d: f64,
// CHECK-NEXT:     l: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn offset(base: &(), field: *const ()) -> u32 {
// CHECK-NEXT:     ((unsafe { (field as *const i8).offset_from((base as *const ()) as *const i8) }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut w: wide = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: wide = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].c = 1i8;
// CHECK-NEXT:         [[T_0]].d = 2.5f64;
// CHECK-NEXT:         [[T_0]].l = 3i64;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%zu %zu %u %u\n".as_ptr(),
// CHECK-NEXT:             20u32,
// CHECK-NEXT:             4u32,
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 &(*(std::ptr::addr_of_mut!(w) as *const ())),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(w.d) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 &(*(std::ptr::addr_of_mut!(w) as *const ())),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(w.l) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { printf(c"%d %g %lld\n".as_ptr(), w.c as i32, w.d, w.l) };
// CHECK-NEXT:     0
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
