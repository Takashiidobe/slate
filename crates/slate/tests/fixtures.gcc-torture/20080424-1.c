/* PR tree-optimization/36008 */

extern void abort(void);

int g[48][3][3];

void __attribute__((noinline)) bar(int x[3][3], int y[3][3]) {
  static int i;
  if (x != g[i + 8] || y != g[i++])
    abort();
}

static inline void __attribute__((always_inline)) foo(int x[][3][3]) {
  int i;
  for (i = 0; i < 8; i++) {
    int k = i + 8;
    bar(x[k], x[k - 8]);
  }
}

int main() {
  foo(g);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/36008
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: __SlateAlign16<{{\[\[}}[i32; 3]; 3]; 48]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: *mut [i32; 3], mut y: *mut [i32; 3]) {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if x != unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(g.0) as *mut {{\[\[}}i32; 3]; 3] }
// CHECK-NEXT:                 .offset(((unsafe { i }) + (8 as i32)) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:         .as_mut_ptr() as *mut [i32; 3]
// CHECK-NEXT:     } {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = unsafe { i };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             i = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_0]] = y
// CHECK-NEXT:             != unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(g.0) as *mut {{\[\[}}i32; 3]; 3] }
// CHECK-NEXT:                         .offset([[V_1]] as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_mut_ptr() as *mut [i32; 3]
// CHECK-NEXT:             };
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: *mut {{\[\[}}i32; 3]; 3]) {
// CHECK-NEXT:     let mut [[I_0:i_[0-9]+]]: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while [[I_0]] < (8 as i32) {
// CHECK-NEXT:         let mut k: i32 = [[I_0]] + (8 as i32);
// CHECK-NEXT:         bar(
// CHECK-NEXT:             unsafe { (*unsafe { x.offset(k as isize) }).as_mut_ptr() as *mut [i32; 3] },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe { x.offset((k - (8 as i32)) as isize) }).as_mut_ptr() as *mut [i32; 3]
// CHECK-NEXT:             },
// CHECK-NEXT:         );
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[I_0]];
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         [[I_0]] = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(unsafe { std::ptr::addr_of_mut!(g.0) as *mut {{\[\[}}i32; 3]; 3] });
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
