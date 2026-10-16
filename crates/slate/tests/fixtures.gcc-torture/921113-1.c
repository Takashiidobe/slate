/* { dg-require-stack-size "128 * 128 * 4 + 1024" } */

void abort(void);
void exit(int);

typedef struct {
  float wsx;
} struct_list;

typedef struct_list *list_t;

typedef struct {
  float x, y;
} vector_t;

void w(float x, float y) {}

void f1(float x, float y) {
  if (x != 0 || y != 0)
    abort();
}
void f2(float x, float y) {
  if (x != 1 || y != 1)
    abort();
}

void gitter(int count, vector_t pos[], list_t list, int *nww, vector_t limit[2],
            float r) {
  float d;
  int   gitt[128][128];

  f1(limit[0].x, limit[0].y);
  f2(limit[1].x, limit[1].y);

  *nww = 0;

  d = pos[0].x;
  if (d <= 0.) {
    w(d, r);
    if (d <= r * 0.5) {
      w(d, r);
      list[0].wsx = 1;
    }
  }
}

vector_t pos[1]   = {{0., 0.}};
vector_t limit[2] = {{0., 0.}, {1., 1.}};

int main(void) {
  int         nww;
  struct_list list;

  gitter(1, pos, &list, &nww, limit, 1.);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-stack-size "128 * 128 * 4 + 1024" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     wsx: f32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     x: f32,
// CHECK-NEXT:     y: f32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn w(mut x: f32, mut y: f32) {}
// CHECK-EMPTY:
// CHECK-NEXT: fn f1(mut x: f32, mut y: f32) {
// CHECK-NEXT:     if x != ((0 as i32) as f32) || y != ((0 as i32) as f32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f2(mut x: f32, mut y: f32) {
// CHECK-NEXT:     if x != ((1 as i32) as f32) || y != ((1 as i32) as f32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gitter(
// CHECK-NEXT:     mut count: i32,
// CHECK-NEXT:     mut [[POS_0:pos_[0-9]+]]: *mut [[SLATERECORD_1]],
// CHECK-NEXT:     mut list: *mut [[SLATERECORD_0]],
// CHECK-NEXT:     mut nww: &mut i32,
// CHECK-NEXT:     mut [[LIMIT_0:limit_[0-9]+]]: *mut [[SLATERECORD_1]],
// CHECK-NEXT:     mut r: f32,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut d: f32 = 0 as f32;
// CHECK-NEXT:     f1(
// CHECK-NEXT:         unsafe { (*unsafe { [[LIMIT_0]].offset((0 as i32) as isize) }).x },
// CHECK-NEXT:         unsafe { (*unsafe { [[LIMIT_0]].offset((0 as i32) as isize) }).y },
// CHECK-NEXT:     );
// CHECK-NEXT:     f2(
// CHECK-NEXT:         unsafe { (*unsafe { [[LIMIT_0]].offset((1 as i32) as isize) }).x },
// CHECK-NEXT:         unsafe { (*unsafe { [[LIMIT_0]].offset((1 as i32) as isize) }).y },
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(nww as *mut i32) = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     d = unsafe { (*unsafe { [[POS_0]].offset((0 as i32) as isize) }).x };
// CHECK-NEXT:     if (d as f64) <= 0.0f64 {
// CHECK-NEXT:         w(d, r);
// CHECK-NEXT:         if (d as f64) <= (r as f64) * 0.5f64 {
// CHECK-NEXT:             w(d, r);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe { list.offset((0 as i32) as isize) }).wsx = (1 as i32) as f32;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut pos: [[[SLATERECORD_1]]; 1] = [[[SLATERECORD_1]] {
// CHECK-NEXT:     x: 0.0f64 as f32,
// CHECK-NEXT:     y: 0.0f64 as f32,
// CHECK-NEXT: }];
// CHECK-EMPTY:
// CHECK-NEXT: static mut limit: __SlateAlign16<[[[SLATERECORD_1]]; 2]> = __SlateAlign16([
// CHECK-NEXT:     [[SLATERECORD_1]] {
// CHECK-NEXT:         x: 0.0f64 as f32,
// CHECK-NEXT:         y: 0.0f64 as f32,
// CHECK-NEXT:     },
// CHECK-NEXT:     [[SLATERECORD_1]] {
// CHECK-NEXT:         x: 1.0f64 as f32,
// CHECK-NEXT:         y: 1.0f64 as f32,
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut nww: i32 = 0 as i32;
// CHECK-NEXT:     let mut list: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     gitter(
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(pos) as *mut [[SLATERECORD_1]] },
// CHECK-NEXT:         std::ptr::addr_of_mut!(list),
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(nww)) },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(limit.0) as *mut [[SLATERECORD_1]] },
// CHECK-NEXT:         1.0f64 as f32,
// CHECK-NEXT:     );
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
