#include <stdio.h>

struct Inner {
  int value;
  double weight;
};

struct Outer {
  int id;
  struct Inner inner;
  int items[4];
  struct Inner *link;
};

static struct Outer global_outer;

static void bump(int *slot, int by) { *slot += by; }

static void scale(double *slot, double factor) { *slot *= factor; }

static int *pick(struct Outer *outer, int which) {
  if (which == 0) {
    return &outer->id;
  }
  if (which == 1) {
    return &outer->inner.value;
  }
  return &outer->items[which - 2];
}

static struct Inner *inner_of(struct Outer *outer) { return &outer->inner; }

int main(void) {
  struct Outer local = {1, {2, 1.5}, {10, 20, 30, 40}, 0};
  struct Inner spare = {7, 0.25};
  local.link = &spare;

  bump(&local.id, 5);
  bump(&local.inner.value, 3);
  bump(&local.items[2], 100);
  scale(&local.inner.weight, 4.0);
  bump(&local.link->value, 9);
  scale(&local.link->weight, 8.0);

  int *slot = pick(&local, 3);
  *slot = *slot * 2;
  *pick(&local, 0) += 1000;

  struct Inner *inner = inner_of(&local);
  inner->value += 50;

  global_outer.id = 3;
  bump(&global_outer.id, 4);
  bump(&global_outer.inner.value, 11);
  scale(&global_outer.inner.weight, 2.0);
  int *global_slot = &global_outer.items[1];
  *global_slot = 77;
  global_outer.link = &global_outer.inner;
  bump(&global_outer.link->value, 1);

  printf("%d %d %d %d %d %d\n", local.id, local.inner.value, local.items[0],
         local.items[1], local.items[2], local.items[3]);
  printf("%.2f %d %.2f\n", local.inner.weight, spare.value, spare.weight);
  printf("%d %d %.2f %d\n", global_outer.id, global_outer.inner.value,
         global_outer.inner.weight, global_outer.items[1]);
  return local.id % 7;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Inner {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT:     weight: f64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Outer {
// CHECK-NEXT:     id: i32,
// CHECK-NEXT:     inner: Inner,
// CHECK-NEXT:     items: [i32; 4],
// CHECK-NEXT:     link: *mut Inner,
// CHECK-NEXT: }
// CHECK-NEXT: static mut global_outer: Outer = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn bump([[V_0:__v[0-9]+]]: *mut i32, by: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_0]] = *[[V_0]] + by;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn scale([[V_1:__v[0-9]+]]: *mut f64, factor: f64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_1]] = *[[V_1]] * factor;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn pick(outer: &mut Outer, which: i32) -> *mut i32 {
// CHECK-NEXT:     if which == 0 {
// CHECK-NEXT:         return unsafe { std::ptr::addr_of_mut!((* (outer as * mut Outer)).id) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if which == 1 {
// CHECK-NEXT:         return unsafe { std::ptr::addr_of_mut!((* (outer as * mut Outer)).inner.value) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ((*(outer as *mut Outer)).items.as_mut_ptr() as *mut i32)
// CHECK-NEXT:             .offset((which - 2) as isize)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn inner_of(outer: &mut Outer) -> *mut Inner {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!((* (outer as * mut Outer)).inner) }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut local: Outer = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: Outer = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].id = 1i32;
// CHECK-NEXT:         [[T_0]].inner = {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: Inner = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]].value = 2i32;
// CHECK-NEXT:             [[T_1]].weight = 1.5f64;
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_0]].items = [10i32, 20i32, 30i32, 40i32];
// CHECK-NEXT:         [[T_0]].link = std::ptr::null_mut::<Inner>();
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut spare: Inner = {
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: Inner = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]].value = 7i32;
// CHECK-NEXT:         [[T_2]].weight = 0.25f64;
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     local.link = std::ptr::addr_of_mut!(spare);
// CHECK-NEXT:     bump(std::ptr::addr_of_mut!(local.id), 5i32);
// CHECK-NEXT:     bump(std::ptr::addr_of_mut!(local.inner.value), 3i32);
// CHECK-NEXT:     bump(unsafe { (local.items.as_mut_ptr() as *mut i32).offset(2 as isize) }, 100i32);
// CHECK-NEXT:     scale(std::ptr::addr_of_mut!(local.inner.weight), 4.0f64);
// CHECK-NEXT:     bump(unsafe { std::ptr::addr_of_mut!((* local.link).value) }, 9i32);
// CHECK-NEXT:     scale(unsafe { std::ptr::addr_of_mut!((* local.link).weight) }, 8.0f64);
// CHECK-NEXT:     let slot: *mut i32 = pick(unsafe { &mut (*std::ptr::addr_of_mut!(local)) }, 3i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *slot = *slot * 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: *mut i32 = pick(unsafe { &mut (*std::ptr::addr_of_mut!(local)) }, 0i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_2]] = *[[V_2]] + 1000;
// CHECK-NEXT:     }
// CHECK-NEXT:     let inner: *mut Inner = inner_of(unsafe { &mut (*std::ptr::addr_of_mut!(local)) });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*inner).value = (*inner).value + 50;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         global_outer.id = 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     bump(unsafe { std::ptr::addr_of_mut!(global_outer.id) }, 4i32);
// CHECK-NEXT:     bump(unsafe { std::ptr::addr_of_mut!(global_outer.inner.value) }, 11i32);
// CHECK-NEXT:     scale(unsafe { std::ptr::addr_of_mut!(global_outer.inner.weight) }, 2.0f64);
// CHECK-NEXT:     let global_slot: *mut i32 = unsafe {
// CHECK-NEXT:         (std::ptr::addr_of_mut!(global_outer.items) as *mut i32).offset(1 as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *global_slot = 77;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         global_outer.link = std::ptr::addr_of_mut!(global_outer.inner);
// CHECK-NEXT:     }
// CHECK-NEXT:     bump(unsafe { std::ptr::addr_of_mut!((* global_outer.link).value) }, 1i32);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {}", local.id, local.inner.value, local.items[0], local.items[1],
// CHECK-NEXT:         local.items[2], local.items[3]
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%.2f %d %.2f\n".as_ptr(), local.inner.weight, spare.value, spare.weight)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %.2f %d\n".as_ptr(),
// CHECK-NEXT:             global_outer.id,
// CHECK-NEXT:             global_outer.inner.value,
// CHECK-NEXT:             global_outer.inner.weight,
// CHECK-NEXT:             global_outer.items[1],
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     local.id % 7
// CHECK-NEXT: }
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
