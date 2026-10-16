extern void abort(void);
extern void exit(int);
int         bar(void);
int         baz(void);

struct foo {
  struct foo *next;
};

struct foo *test(struct foo *node) {
  while (node) {
    if (bar() && !baz())
      break;
    node = node->next;
  }
  return node;
}

int bar(void) { return 0; }

int baz(void) { return 0; }

int main(void) {
  struct foo a, b, *c;

  a.next = &b;
  b.next = (struct foo *)0;
  c      = test(&a);
  if (c)
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct foo {
// CHECK-NEXT:     next: *mut foo,
// CHECK-NEXT: }
// CHECK-NEXT: fn test(mut node: *mut foo) -> *mut foo {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while node != std::ptr::null_mut::<foo>() {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:         if bar() != (0 as i32) {
// CHECK-NEXT:             [[V_1]] = !(baz() != (0 as i32));
// CHECK-NEXT:         } else {
// CHECK-NEXT:             [[V_1]] = false as bool;
// CHECK-NEXT:         }
// CHECK-NEXT:         if [[V_1]] {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         node = unsafe { (*node).next };
// CHECK-NEXT:     }
// CHECK-NEXT:     node
// CHECK-NEXT: }
// CHECK-NEXT: fn bar() -> i32 {
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn baz() -> i32 {
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: foo = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut b: foo = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut c: *mut foo = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     a.next = std::ptr::addr_of_mut!(b);
// CHECK-NEXT:     b.next = std::ptr::null_mut::<foo>();
// CHECK-NEXT:     c = test(std::ptr::addr_of_mut!(a));
// CHECK-NEXT:     if c != std::ptr::null_mut::<foo>() {
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
