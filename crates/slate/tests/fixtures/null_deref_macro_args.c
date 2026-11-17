#include <stddef.h>
#include <stdio.h>

struct chan { int band; };
struct def { struct chan *chan; int width; };
struct entry { int band; int width; };

#define ASSIGN(d, e) do { if ((d) && (d)->chan) { (e)->band = (d)->chan->band; (e)->width = (d)->width; } else { (e)->band = 0; } } while (0)

#define FIND(start, prev_ptr) ({ int __pos = (start), __prev = 0, __found = 0; \
  while (__pos < 10) { if (__pos == 7) { __found = __pos; if (prev_ptr) *(prev_ptr) = __prev; break; } __prev = __pos; __pos++; } __found; })

static void fill(struct entry *e, struct def *d) { ASSIGN(d, e); }
static void fill_null(struct entry *e) { ASSIGN((struct def *)NULL, e); }

#define STORE(p, i, v) do { if (p) (p)[i] = (v); } while (0)
static void store_null(int i) { STORE((int *)NULL, i, 4); STORE((int *)NULL, 0, 4); }

int main(void) {
  struct chan c = {3};
  struct def d = {&c, 5};
  struct entry e = {9, 9};
  fill(&e, &d);
  printf("%d %d\n", e.band, e.width);
  fill_null(&e);
  printf("%d\n", e.band);
  unsigned char prev = 0;
  int a = FIND(2, &prev);
  int b = FIND(3, (unsigned char *)NULL);
  int slots[3] = {0};
  int *sp = slots;
  STORE(sp, 1, 6);
  store_null(2);
  printf("%d %d %d %d\n", a, b, prev, slots[1]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![allow(deref_nullptr)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! ASSIGN {
// CHECK-NEXT:     ($d:expr, $e:expr) => {
// CHECK-NEXT:         loop { if !$d .is_null() && ! unsafe { (*$d).chan } .is_null() { unsafe { (*$e)
// CHECK-NEXT:         .band = (* (*$d).chan).band; } unsafe { (*$e).width = (*$d).width; } } else {
// CHECK-NEXT:         unsafe { (*$e).band = 0; } } if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! FIND {
// CHECK-NEXT:     ($start:expr, $prev_ptr:expr) => {
// CHECK-NEXT:         { let mut __v : i32; let mut [[V_0:__v[0-9]+]] : i32 = $start; let mut [[V_1:__v[0-9]+]] : i32 = 0; let mut
// CHECK-NEXT:         [[V_2:__v[0-9]+]] : i32 = 0; while [[V_0]] < 10 { if [[V_0]] == 7 { [[V_2]] = [[V_0]]; if !$prev_ptr
// CHECK-NEXT:         .is_null() { unsafe { *$prev_ptr = [[V_1]] as u8; } } break; } [[V_1]] = [[V_0]]; [[V_0]] +=
// CHECK-NEXT:         1; } __v = [[V_2]]; __v }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! STORE__2 {
// CHECK-NEXT:     ($p:expr, $i:expr, $v:expr) => {
// CHECK-NEXT:         loop { if !$p .is_null() { unsafe { *$p .offset(($i) as isize) = $v; } } if true
// CHECK-NEXT:         { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! STORE {
// CHECK-NEXT:     ($p:expr, $i:expr, $v:expr) => {
// CHECK-NEXT:         loop { if !$p .is_null() { unsafe { *$p .offset(($i) as isize) = $v; } } if true
// CHECK-NEXT:         { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct chan {
// CHECK-NEXT:     band: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct def {
// CHECK-NEXT:     chan: *mut chan,
// CHECK-NEXT:     width: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct entry {
// CHECK-NEXT:     band: i32,
// CHECK-NEXT:     width: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn fill(e: *mut entry, d: *mut def) {
// CHECK-NEXT:     ASSIGN!(d, e);
// CHECK-NEXT: }
// CHECK-NEXT: fn fill_null(e: *mut entry) {
// CHECK-NEXT:     ASSIGN!(core::ptr::null_mut::< def > (), e);
// CHECK-NEXT: }
// CHECK-NEXT: fn store_null(i: i32) {
// CHECK-NEXT:     STORE!(core::ptr::null_mut::< i32 > (), i, 4i32);
// CHECK-NEXT:     STORE!(core::ptr::null_mut::< i32 > (), 0i32, 4i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut c: chan = chan { band: 3 };
// CHECK-NEXT:     let mut d: def = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: def = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].chan = core::ptr::addr_of_mut!(c);
// CHECK-NEXT:         [[T_0]].width = 5i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut e: entry = entry { band: 9, width: 9 };
// CHECK-NEXT:     fill(core::ptr::addr_of_mut!(e), core::ptr::addr_of_mut!(d));
// CHECK-NEXT:     println!("{} {}", e.band, e.width);
// CHECK-NEXT:     fill_null(core::ptr::addr_of_mut!(e));
// CHECK-NEXT:     println!("{}", e.band);
// CHECK-NEXT:     let mut prev: u8 = 0;
// CHECK-NEXT:     let a: i32 = FIND!(2i32, core::ptr::addr_of_mut!(prev));
// CHECK-NEXT:     let b: i32 = FIND!(3i32, core::ptr::null_mut::< u8 > ());
// CHECK-NEXT:     let mut slots: [i32; 3] = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: [i32; 3] = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_1]][0] = 0i32;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut sp: *mut i32 = slots.as_mut_ptr() as *mut i32;
// CHECK-NEXT:     STORE__2!(sp, 1i32, 6i32);
// CHECK-NEXT:     store_null(2i32);
// CHECK-NEXT:     println!("{} {} {} {}", a, b, prev as i32, slots[1]);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
