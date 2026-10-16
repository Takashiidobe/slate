/* From PR target/16176 */
void abort(void);
void exit(int);

struct __attribute__((packed)) s {
  struct s *next;
};

struct s *__attribute__((noinline)) maybe_next(struct s *s, int t) {
  if (t)
    s = s->next;
  return s;
}

int main() {
  struct s s1, s2;

  s1.next = &s2;
  if (maybe_next(&s1, 1) != &s2)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // From PR target/16176
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s {
// CHECK-NEXT:     next: *mut s,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn maybe_next(mut s: *mut s, mut t: i32) -> *mut s {
// CHECK-NEXT:     if t != (0 as i32) {
// CHECK-NEXT:         s = unsafe { (*s).next };
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s1: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut s2: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     s1.next = std::ptr::addr_of_mut!(s2);
// CHECK-NEXT:     if maybe_next(std::ptr::addr_of_mut!(s1), 1 as i32) != std::ptr::addr_of_mut!(s2) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
