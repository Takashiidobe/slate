/* { dg-require-effective-target int32plus } */
struct big {
  int data[1000000];
};

struct small {
  int data[10];
};

union both {
  struct big   big;
  struct small small;
};

extern void *calloc(__SIZE_TYPE__, __SIZE_TYPE__);
extern void  free(void *);

static int __attribute__((noinline)) foo(int fail, union both *agg) {
  int r;
  if (fail)
    r = agg->big.data[999999];
  else
    r = agg->small.data[0];
  return r;
}

int main(int argc, char *argv[]) {
  union both *agg = calloc(1, sizeof(struct small));
  int         r;

  r = foo((argc > 2000), agg);

  free(agg);
  return r;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn calloc([[V_0:__v[0-9]+]]: u64, [[V_1:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn free([[V_2:__v[0-9]+]]: *mut ());
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// { dg-require-effective-target int32plus }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct big {
// CHECK-NEXT:     data: [i32; 1000000],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct small {
// CHECK-NEXT:     data: [i32; 10],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union both {
// CHECK-NEXT:     big: big,
// CHECK-NEXT:     small: small,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut fail: i32, mut agg: *mut both) -> i32 {
// CHECK-NEXT:     let mut r: i32 = 0 as i32;
// CHECK-NEXT:     if fail != (0 as i32) {
// CHECK-NEXT:         r = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { (*agg).big.data.as_mut_ptr() as *mut i32 }.offset((999999 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         r = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { (*agg).small.data.as_mut_ptr() as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut agg: *mut both =
// CHECK-NEXT:         (unsafe { calloc(((1 as i32) as i64) as u64, 40 as u64) }) as *mut both;
// CHECK-NEXT:     let mut r: i32 = foo((argc > (2000 as i32)) as i32, agg);
// CHECK-NEXT:     unsafe { free(agg as *mut ()) };
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
