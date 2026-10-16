/* { dg-require-effective-target label_values } */

/* As a quality of implementation issue, we should not prevent inlining
   of function explicitly marked inline just because a label therein had
   its address taken.  */

void abort(void);
void exit(int);

static void *ptr1, *ptr2;
static int   i = 1;

static __inline__ void doit(void **pptr, int cond) {
  if (cond) {
  here:
    *pptr = &&here;
  }
}

__attribute__((noinline)) static void f(int cond) { doit(&ptr1, cond); }

__attribute__((noinline)) static void g(int cond) { doit(&ptr2, cond); }

__attribute__((noinline)) static void bar(void);

int main() {
  f(i);
  bar();
  g(i);

#ifdef __OPTIMIZE__
  if (ptr1 == ptr2)
    abort();
#endif

  exit(0);
}

void bar(void) {}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target label_values }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // As a quality of implementation issue, we should not prevent inlining
// CHECK-NEXT: // of function explicitly marked inline just because a label therein had
// CHECK-NEXT: // its address taken.
// CHECK-NEXT: static mut ptr1: *mut () = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut ptr2: *mut () = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn doit(mut pptr: *mut *mut (), mut cond: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             if cond != (0 as i32) {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *pptr = 7usize as *mut ();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut cond: i32) {
// CHECK-NEXT:     doit(unsafe { std::ptr::addr_of_mut!(ptr1) }, cond);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn g(mut cond: i32) {
// CHECK-NEXT:     doit(unsafe { std::ptr::addr_of_mut!(ptr2) }, cond);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f(unsafe { i });
// CHECK-NEXT:     bar();
// CHECK-NEXT:     g(unsafe { i });
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() {}
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
