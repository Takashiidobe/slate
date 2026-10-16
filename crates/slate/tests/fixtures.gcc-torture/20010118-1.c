void abort(void);
void exit(int);

typedef struct {
  int a, b, c, d, e, f;
} A;

void foo(A *v, int w, int x, int *y, int *z) {}

void bar(A *v, int x, int y, int w, int h) {
  if (v->a != x || v->b != y) {
    int oldw = w;
    int oldh = h;
    int e    = v->e;
    int f    = v->f;
    int dx, dy;
    foo(v, 0, 0, &w, &h);
    dx    = (oldw - w) * (double)e / 2.0;
    dy    = (oldh - h) * (double)f / 2.0;
    x    += dx;
    y    += dy;
    v->a  = x;
    v->b  = y;
    v->c  = w;
    v->d  = h;
  }
}

int main() {
  A w = {100, 110, 20, 30, -1, -1};
  bar(&w, 400, 420, 50, 70);
  if (w.d != 70)
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
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     c: i32,
// CHECK-NEXT:     d: i32,
// CHECK-NEXT:     e: i32,
// CHECK-NEXT:     f: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut v: &mut [[SLATERECORD_0]], mut w: i32, mut x: i32, mut y: &i32, mut z: &i32) {}
// CHECK-NEXT: fn bar(mut v: &mut [[SLATERECORD_0]], mut x: i32, mut y: i32, mut w: i32, mut h: i32) {
// CHECK-NEXT:     if (unsafe { (*(v as *mut [[SLATERECORD_0]])).a }) != x
// CHECK-NEXT:         || (unsafe { (*(v as *mut [[SLATERECORD_0]])).b }) != y
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut oldw: i32 = w;
// CHECK-NEXT:         let mut oldh: i32 = h;
// CHECK-NEXT:         let mut e: i32 = unsafe { (*(v as *mut [[SLATERECORD_0]])).e };
// CHECK-NEXT:         let mut f: i32 = unsafe { (*(v as *mut [[SLATERECORD_0]])).f };
// CHECK-NEXT:         let mut dx: i32 = 0 as i32;
// CHECK-NEXT:         let mut dy: i32 = 0 as i32;
// CHECK-NEXT:         foo(
// CHECK-NEXT:             v,
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             unsafe { &(*std::ptr::addr_of_mut!(w)) },
// CHECK-NEXT:             unsafe { &(*std::ptr::addr_of_mut!(h)) },
// CHECK-NEXT:         );
// CHECK-NEXT:         dx = (((oldw - w) as f64) * (e as f64) / 2.0f64) as i32;
// CHECK-NEXT:         dy = (((oldh - h) as f64) * (f as f64) / 2.0f64) as i32;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = x;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + dx;
// CHECK-NEXT:         x = [[V_2]];
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = y;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + dy;
// CHECK-NEXT:         y = [[V_4]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(v as *mut [[SLATERECORD_0]])).a = x;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(v as *mut [[SLATERECORD_0]])).b = y;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(v as *mut [[SLATERECORD_0]])).c = w;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(v as *mut [[SLATERECORD_0]])).d = h;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut w: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:         a: 100 as i32,
// CHECK-NEXT:         b: 110 as i32,
// CHECK-NEXT:         c: 20 as i32,
// CHECK-NEXT:         d: 30 as i32,
// CHECK-NEXT:         e: -(1 as i32),
// CHECK-NEXT:         f: -(1 as i32),
// CHECK-NEXT:     };
// CHECK-NEXT:     bar(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(w)) },
// CHECK-NEXT:         400 as i32,
// CHECK-NEXT:         420 as i32,
// CHECK-NEXT:         50 as i32,
// CHECK-NEXT:         70 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     if w.d != (70 as i32) {
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
