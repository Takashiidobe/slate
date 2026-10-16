/* PR rtl-optimization/31691 */
/* Origin: Chi-Hua Chen <stephaniechc-gccbug@yahoo.com> */

extern void abort(void);

static int get_kind(int) __attribute__((noinline));

static int get_kind(int v) {
  volatile int k = v;
  return k;
}

static int some_call(void) __attribute__((noinline));

static int some_call(void) { return 0; }

static void example(int arg) {
  int tmp, kind = get_kind(arg);

  if (kind == 9 || kind == 10 || kind == 5) {
    if (some_call() == 0) {
      if (kind == 9 || kind == 10)
        tmp = arg;
      else
        abort();
    }
  }
}

int main(void) {
  example(10);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/31691
// CHECK-NEXT: //!
// CHECK-NEXT: //! Origin: Chi-Hua Chen <stephaniechc-gccbug@yahoo.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn get_kind(mut k: i32) -> i32 {
// CHECK-NEXT:     unsafe { std::ptr::read_volatile(std::ptr::addr_of!(k)) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn some_call() -> i32 {
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn example(mut arg: i32) {
// CHECK-NEXT:     let mut tmp: i32 = 0 as i32;
// CHECK-NEXT:     let mut kind: i32 = get_kind(arg);
// CHECK-NEXT:     if kind == (9 as i32) || kind == (10 as i32) || kind == (5 as i32) {
// CHECK-NEXT:         if some_call() == (0 as i32) {
// CHECK-NEXT:             if kind == (9 as i32) || kind == (10 as i32) {
// CHECK-NEXT:                 tmp = arg;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     example(10 as i32);
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
