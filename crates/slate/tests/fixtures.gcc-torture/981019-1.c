extern void abort(void);
extern int  f2(void);
extern int  f3(void);
extern void f1(void);

void ff(int fname, int part, int nparts) {
  if (fname) /* bb 0 */
  {
    if (nparts) /* bb 1 */
      f1();     /* bb 2 */
  } else
    fname = 2; /* bb 3  */

  /* bb 4 is the branch to bb 10
     (bb 10 is physically at the end of the loop) */
  while (f3() /* bb 10 */) {
    if (nparts /* bb 5 */ && f2() /* bb 6 */) {
      f1(); /* bb 7 ... */
      nparts = part;
      if (f3()) /* ... bb 7 */
        f1();   /* bb 8 */
      f1();     /* bb 9 */
      break;
    }
  }

  if (nparts) /* bb 11 */
    f1();     /* bb 12 */
  return;     /* bb 13 */
}

int main(void) {
  ff(0, 1, 0);
  return 0;
}

int f3(void) {
  static int x = 0;
  x            = !x;
  return x;
}
void f1(void) { abort(); }
int  f2(void) { abort(); }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ff(mut fname: i32, mut part: i32, mut nparts: i32) {
// CHECK-NEXT:     if fname != (0 as i32) {
// CHECK-NEXT:         if nparts != (0 as i32) {
// CHECK-NEXT:             f1();
// CHECK-NEXT:         }
// CHECK-NEXT:     // bb 1
// CHECK-NEXT:     // bb 2
// CHECK-NEXT:     } else {
// CHECK-NEXT:         fname = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     // bb 0
// CHECK-NEXT:     // bb 3
// CHECK-NEXT:     // bb 4 is the branch to bb 10
// CHECK-NEXT:     // (bb 10 is physically at the end of the loop)
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while f3() != (0 as i32) {
// CHECK-NEXT:         // bb 10
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:         if nparts != (0 as i32) {
// CHECK-NEXT:             [[V_0]] = f2() != (0 as i32);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             [[V_0]] = false as bool;
// CHECK-NEXT:         }
// CHECK-NEXT:         if [[V_0]] {
// CHECK-NEXT:             // bb 5
// CHECK-NEXT:             // bb 6
// CHECK-NEXT:             f1(); // bb 7 ...
// CHECK-NEXT:             nparts = part;
// CHECK-NEXT:             if f3() != (0 as i32) {
// CHECK-NEXT:                 f1();
// CHECK-NEXT:             }
// CHECK-NEXT:             // ... bb 7
// CHECK-NEXT:             // bb 8
// CHECK-NEXT:             f1(); // bb 9
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if nparts != (0 as i32) {
// CHECK-NEXT:         f1();
// CHECK-NEXT:     }
// CHECK-NEXT:     // bb 11
// CHECK-NEXT:     // bb 12
// CHECK-NEXT:     return; // bb 13
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     ff(0 as i32, 1 as i32, 0 as i32);
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f3() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = !((unsafe { x }) != (0 as i32)) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { x }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn f1() {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f2() -> i32 {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
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
