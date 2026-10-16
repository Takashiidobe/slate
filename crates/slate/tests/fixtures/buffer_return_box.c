#include <stdio.h>
#include <stdlib.h>

static int *make(int n) {
  int *p = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    p[i] = i * i;
  }
  return p;
}

static int *make8(void) {
  int *p = malloc(8 * sizeof(int));
  for (int i = 0; i < 8; i++) {
    p[i] = i;
  }
  return p;
}

static int *make_raw(int n) {
  int *p = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    p[i] = i + 1;
  }
  return p;
}

static int read_first(int *p) { return p[0]; }

static int *make_shadow(int n) {
  int *p = malloc(n * sizeof(int));
  p[0]   = 9;
  return p;
}

static void custom_free(void *p) { (void)p; }

static int *maybe(int n) {
  if (n < 0) {
    return NULL;
  }
  return malloc(n * sizeof(int));
}

static int *allocfree(int n) {
  int *p = malloc(n * sizeof(int));
  free(p);
  return malloc(n * sizeof(int));
}

// @check-fn-begin
int main(void) {
  int *q    = make(4);
  int *r    = make8();
  int *m    = maybe(2);
  int *a    = allocfree(3);
  int *raw  = make_raw(2);
  int *raw2 = make_raw(2);
  printf("%d %d %d %d %d %d\n", q[3], r[7], (int)(m != NULL), (int)(a != NULL),
         read_first(raw), raw2[1]);
  free(q);
  free(r);
  if (m) {
    free(m);
  }
  free(a);
  free(raw);
  free(raw2);
  {
    void (*free)(void *) = custom_free;
    int *shadow          = make_shadow(1);
    free(shadow);
  }
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut q: *mut i32 = make(4 as i32);
// CHECK-NEXT:     let mut r: *mut i32 = make8();
// CHECK-NEXT:     let mut m: *mut i32 = maybe(2 as i32);
// CHECK-NEXT:     let mut a: *mut i32 = allocfree(3 as i32);
// CHECK-NEXT:     let mut raw: *mut i32 = make_raw(2 as i32);
// CHECK-NEXT:     let mut raw2: *mut i32 = make_raw(2 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %d %d %d\n".as_ptr() as *const i8,
// CHECK-NEXT:             unsafe { *unsafe { q.offset((3 as i32) as isize) } },
// CHECK-NEXT:             unsafe { *unsafe { r.offset((7 as i32) as isize) } },
// CHECK-NEXT:             (m != std::ptr::null_mut::<i32>()) as i32,
// CHECK-NEXT:             (a != std::ptr::null_mut::<i32>()) as i32,
// CHECK-NEXT:             read_first(raw),
// CHECK-NEXT:             unsafe { *unsafe { raw2.offset((1 as i32) as isize) } },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { free(q as *mut ()) };
// CHECK-NEXT:     unsafe { free(r as *mut ()) };
// CHECK-NEXT:     if m != std::ptr::null_mut::<i32>() {
// CHECK-NEXT:         unsafe { free(m as *mut ()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { free(a as *mut ()) };
// CHECK-NEXT:     unsafe { free(raw as *mut ()) };
// CHECK-NEXT:     unsafe { free(raw2 as *mut ()) };
// CHECK-NEXT:     let mut free: Option<unsafe extern "C-unwind" fn(*mut ())> = Some(custom_free);
// CHECK-NEXT:     let mut shadow: *mut i32 = make_shadow(1 as i32);
// CHECK-NEXT:     unsafe { free.unwrap()(shadow as *mut ()) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
