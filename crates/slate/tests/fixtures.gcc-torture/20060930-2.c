/* PR middle-end/29272 */

extern void abort(void);

struct S {
  struct S *s;
} s;
struct T {
  struct T *t;
} t;

static inline void foo(void *s) {
  struct T *p = s;
  __builtin_memcpy(&p->t, &t.t, sizeof(t.t));
}

void *__attribute__((noinline)) bar(void *p, struct S *q) {
  q->s = &s;
  foo(p);
  return q->s;
}

int main(void) {
  t.t = &t;
  if (bar(&s, &s) != (void *)&t)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/29272
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     s: *mut S,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: S = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T {
// CHECK-NEXT:     t: *mut T,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut t: T = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut [[S_0:s_[0-9]+]]: *mut ()) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!((*([[S_0]] as *mut T)).t) }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(t.t) }) as *const (),
// CHECK-NEXT:             8 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut p: *mut (), mut q: *mut S) -> *mut () {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*q).s = unsafe { std::ptr::addr_of_mut!(s) };
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(p);
// CHECK-NEXT:     (unsafe { (*q).s }) as *mut ()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         t.t = unsafe { std::ptr::addr_of_mut!(t) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar((unsafe { std::ptr::addr_of_mut!(s) }) as *mut (), unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!(s)
// CHECK-NEXT:     }) != ((unsafe { std::ptr::addr_of_mut!(t) }) as *mut ())
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
