/* PR tree-optimization/91597 */

enum E { A, B, C };
struct __attribute__((aligned(4))) S {
  enum E e;
};

enum E foo(struct S *o) {
  if (((__UINTPTR_TYPE__)(o) & 1) == 0)
    return o->e;
  else
    return A;
}

int bar(struct S *o) { return foo(o) == B || foo(o) == C; }

static inline void baz(struct S *o, int d) {
  if (__builtin_expect(!bar(o), 0))
    __builtin_abort();
  if (d > 2)
    return;
  baz(o, d + 1);
}

void qux(struct S *o) {
  switch (o->e) {
  case A:
    return;
  case B:
    baz(o, 0);
    break;
  case C:
    baz(o, 0);
    break;
  }
}

struct S s = {C};

int main() {
  qux(&s);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/91597
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __builtin_expect([[V_0:__v[0-9]+]]: i64, [[V_1:__v[0-9]+]]: i64) -> i64;
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     e: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut o: *mut S) -> u32 {
// CHECK-NEXT:     if (o as u64) & (((1 as i32) as i64) as u64) == (((0 as i32) as i64) as u64) {
// CHECK-NEXT:         return unsafe { (*o).e };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return (0 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut o: *mut S) -> i32 {
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (foo(o) as u32) == ((1 as i32) as u32) {
// CHECK-NEXT:         [[V_2]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = (foo(o) as u32) == ((2 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_2]] as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut o: *mut S, mut d: i32) {
// CHECK-NEXT:     if (!(bar(o) != (0 as i32)) as i64) != (0 as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if d > (2 as i32) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     baz(o, d + (1 as i32));
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn qux(mut o: *mut S) {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match (unsafe { (*o).e }) as u32 {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 return;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 baz(o, 0 as i32);
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 baz(o, 0 as i32);
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: S = S {
// CHECK-NEXT:     e: ((2 as i32) as u32) as u32,
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     qux(unsafe { std::ptr::addr_of_mut!(s) });
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
