#include <stdio.h>
#include <string.h>

void abort(void);
void exit(int);

char out[100];

typedef struct {
  double d;
  int    i[3];
} B;
typedef struct {
  char c[33], c1;
} X;

char c1 = 'a';
char c2 = 127;
char c3 = (char)128;
char c4 = (char)255;
char c5 = -1;

double d1 = 0.1;
double d2 = 0.2;
double d3 = 0.3;
double d4 = 0.4;
double d5 = 0.5;
double d6 = 0.6;
double d7 = 0.7;
double d8 = 0.8;
double d9 = 0.9;

B B1 = {0.1, {1, 2, 3}};
B B2 = {0.2, {5, 4, 3}};
X X1 = {"abcdefghijklmnopqrstuvwxyzABCDEF", 'G'};
X X2 = {"123", '9'};
X X3 = {"return-return-return", 'R'};

X f(B a, char b, double c, B d) {
  static X xr = {"return val", 'R'};
  X        r;
  r    = xr;
  r.c1 = b;
  sprintf(out, "X f(B,char,double,B):({%g,{%d,%d,%d}},'%c',%g,{%g,{%d,%d,%d}})",
          a.d, a.i[0], a.i[1], a.i[2], b, c, d.d, d.i[0], d.i[1], d.i[2]);
  return r;
}

X (*fp)(B, char, double, B) = &f;

int main(void) {
  X    Xr;
  char tmp[100];

  Xr = f(B1, c2, d3, B2);
  strcpy(tmp, out);
  Xr.c[0] = Xr.c1 = '\0';
  Xr              = (*fp)(B1, c2, d3, B2);
  if (strcmp(tmp, out))
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn sprintf(__s: *mut i8, __format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn strcpy(__dest: *mut i8, __src: *const i8) -> *mut i8;
// CHECK-NEXT:     fn strcmp(__s1: *const i8, __s2: *const i8) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut out: __SlateAlign16<[i8; 100]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     d: f64,
// CHECK-NEXT:     i: [i32; 3],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     c: [i8; 33],
// CHECK-NEXT:     c1: i8,
// CHECK-NEXT: }
// CHECK-NEXT: static mut c1: i8 = (97 as i32) as i8;
// CHECK-NEXT: static mut c2: i8 = (127 as i32) as i8;
// CHECK-NEXT: static mut c3: i8 = (128 as i32) as i8;
// CHECK-NEXT: static mut c4: i8 = (255 as i32) as i8;
// CHECK-NEXT: static mut c5: i8 = -(1 as i32) as i8;
// CHECK-NEXT: static mut d1: f64 = 0.1f64;
// CHECK-NEXT: static mut d2: f64 = 0.2f64;
// CHECK-NEXT: static mut d3: f64 = 0.3f64;
// CHECK-NEXT: static mut d4: f64 = 0.4f64;
// CHECK-NEXT: static mut d5: f64 = 0.5f64;
// CHECK-NEXT: static mut d6: f64 = 0.6f64;
// CHECK-NEXT: static mut d7: f64 = 0.7f64;
// CHECK-NEXT: static mut d8: f64 = 0.8f64;
// CHECK-NEXT: static mut d9: f64 = 0.9f64;
// CHECK-NEXT: static mut B1: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:     d: 0.1f64,
// CHECK-NEXT:     i: [1 as i32, 2 as i32, 3 as i32],
// CHECK-NEXT: };
// CHECK-NEXT: static mut B2: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:     d: 0.2f64,
// CHECK-NEXT:     i: [5 as i32, 4 as i32, 3 as i32],
// CHECK-NEXT: };
// CHECK-NEXT: static mut X1: [[SLATERECORD_1]] = [[SLATERECORD_1]] {
// CHECK-NEXT:     c: [
// CHECK-NEXT:         97 as i8,
// CHECK-NEXT:         98 as i8,
// CHECK-NEXT:         99 as i8,
// CHECK-NEXT:         100 as i8,
// CHECK-NEXT:         101 as i8,
// CHECK-NEXT:         102 as i8,
// CHECK-NEXT:         103 as i8,
// CHECK-NEXT:         104 as i8,
// CHECK-NEXT:         105 as i8,
// CHECK-NEXT:         106 as i8,
// CHECK-NEXT:         107 as i8,
// CHECK-NEXT:         108 as i8,
// CHECK-NEXT:         109 as i8,
// CHECK-NEXT:         110 as i8,
// CHECK-NEXT:         111 as i8,
// CHECK-NEXT:         112 as i8,
// CHECK-NEXT:         113 as i8,
// CHECK-NEXT:         114 as i8,
// CHECK-NEXT:         115 as i8,
// CHECK-NEXT:         116 as i8,
// CHECK-NEXT:         117 as i8,
// CHECK-NEXT:         118 as i8,
// CHECK-NEXT:         119 as i8,
// CHECK-NEXT:         120 as i8,
// CHECK-NEXT:         121 as i8,
// CHECK-NEXT:         122 as i8,
// CHECK-NEXT:         65 as i8,
// CHECK-NEXT:         66 as i8,
// CHECK-NEXT:         67 as i8,
// CHECK-NEXT:         68 as i8,
// CHECK-NEXT:         69 as i8,
// CHECK-NEXT:         70 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:     ],
// CHECK-NEXT:     c1: (71 as i32) as i8,
// CHECK-NEXT: };
// CHECK-NEXT: static mut X2: [[SLATERECORD_1]] = [[SLATERECORD_1]] {
// CHECK-NEXT:     c: [
// CHECK-NEXT:         49 as i8,
// CHECK-NEXT:         50 as i8,
// CHECK-NEXT:         51 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:     ],
// CHECK-NEXT:     c1: (57 as i32) as i8,
// CHECK-NEXT: };
// CHECK-NEXT: static mut X3: [[SLATERECORD_1]] = [[SLATERECORD_1]] {
// CHECK-NEXT:     c: [
// CHECK-NEXT:         114 as i8,
// CHECK-NEXT:         101 as i8,
// CHECK-NEXT:         116 as i8,
// CHECK-NEXT:         117 as i8,
// CHECK-NEXT:         114 as i8,
// CHECK-NEXT:         110 as i8,
// CHECK-NEXT:         45 as i8,
// CHECK-NEXT:         114 as i8,
// CHECK-NEXT:         101 as i8,
// CHECK-NEXT:         116 as i8,
// CHECK-NEXT:         117 as i8,
// CHECK-NEXT:         114 as i8,
// CHECK-NEXT:         110 as i8,
// CHECK-NEXT:         45 as i8,
// CHECK-NEXT:         114 as i8,
// CHECK-NEXT:         101 as i8,
// CHECK-NEXT:         116 as i8,
// CHECK-NEXT:         117 as i8,
// CHECK-NEXT:         114 as i8,
// CHECK-NEXT:         110 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:     ],
// CHECK-NEXT:     c1: (82 as i32) as i8,
// CHECK-NEXT: };
// CHECK-NEXT: extern "C-unwind" fn f(
// CHECK-NEXT:     mut a: [[SLATERECORD_0]],
// CHECK-NEXT:     mut b: i8,
// CHECK-NEXT:     mut c: f64,
// CHECK-NEXT:     mut d: [[SLATERECORD_0]],
// CHECK-NEXT: ) -> [[SLATERECORD_1]] {
// CHECK-NEXT:     let mut r: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     r = unsafe { xr };
// CHECK-NEXT:     r.c1 = b;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         sprintf(
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(out.0) as *mut i8 },
// CHECK-NEXT:             c"X f(B,char,double,B):({%g,{%d,%d,%d}},'%c',%g,{%g,{%d,%d,%d}})".as_ptr()
// CHECK-NEXT:                 as *const i8,
// CHECK-NEXT:             a.d,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (a.i.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) }
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (a.i.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) }
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (a.i.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) }
// CHECK-NEXT:             },
// CHECK-NEXT:             b as i32,
// CHECK-NEXT:             c,
// CHECK-NEXT:             d.d,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (d.i.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) }
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (d.i.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) }
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (d.i.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) }
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return r;
// CHECK-NEXT: }
// CHECK-NEXT: static mut xr: [[SLATERECORD_1]] = [[SLATERECORD_1]] {
// CHECK-NEXT:     c: [
// CHECK-NEXT:         114 as i8,
// CHECK-NEXT:         101 as i8,
// CHECK-NEXT:         116 as i8,
// CHECK-NEXT:         117 as i8,
// CHECK-NEXT:         114 as i8,
// CHECK-NEXT:         110 as i8,
// CHECK-NEXT:         32 as i8,
// CHECK-NEXT:         118 as i8,
// CHECK-NEXT:         97 as i8,
// CHECK-NEXT:         108 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:     ],
// CHECK-NEXT:     c1: (82 as i32) as i8,
// CHECK-NEXT: };
// CHECK-NEXT: static mut fp: Option<
// CHECK-NEXT:     unsafe extern "C-unwind" fn(
// CHECK-NEXT:         [[SLATERECORD_0]],
// CHECK-NEXT:         i8,
// CHECK-NEXT:         f64,
// CHECK-NEXT:         [[SLATERECORD_0]],
// CHECK-NEXT:     ) -> [[SLATERECORD_1]],
// CHECK-NEXT: > = Some(f);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut Xr: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut tmp: __SlateAlign16<[i8; 100]> = __SlateAlign16([0 as i8; 100]);
// CHECK-NEXT:     Xr = f(unsafe { B1 }, unsafe { c2 }, unsafe { d3 }, unsafe { B2 });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strcpy(
// CHECK-NEXT:             tmp.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(out.0) as *mut i8 }) as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     Xr.c1 = (0 as i32) as i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (Xr.c.as_mut_ptr() as *mut i8).offset((0 as i32) as isize) } = (0
// CHECK-NEXT:             as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     Xr = unsafe {
// CHECK-NEXT:         unsafe { fp }
// CHECK-NEXT:             .unwrap()(unsafe { B1 }, unsafe { c2 }, unsafe { d3 }, unsafe { B2 })
// CHECK-NEXT:     };
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strcmp(
// CHECK-NEXT:             tmp.0.as_mut_ptr() as *const i8,
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(out.0) as *mut i8 }) as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
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
