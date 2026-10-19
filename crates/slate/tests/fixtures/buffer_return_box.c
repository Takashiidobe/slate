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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let q: *mut i32 = make(4i32);
// CHECK-NEXT:     let r: *mut i32 = make8();
// CHECK-NEXT:     let m: *mut i32 = maybe(2i32);
// CHECK-NEXT:     let a: *mut i32 = allocfree(3i32);
// CHECK-NEXT:     let raw: *mut i32 = make_raw(2i32);
// CHECK-NEXT:     let raw2: *mut i32 = make_raw(2i32);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {}", unsafe { * q.offset(3 as isize) }, unsafe { * r.offset(7 as
// CHECK-NEXT:         isize) }, ! m.is_null() as i32, ! a.is_null() as i32, read_first(raw), unsafe { *
// CHECK-NEXT:         raw2.offset(1 as isize) }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { free(q as *mut ()) };
// CHECK-NEXT:     unsafe { free(r as *mut ()) };
// CHECK-NEXT:     if !m.is_null() {
// CHECK-NEXT:         unsafe { free(m as *mut ()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { free(a as *mut ()) };
// CHECK-NEXT:     unsafe { free(raw as *mut ()) };
// CHECK-NEXT:     unsafe { free(raw2 as *mut ()) };
// CHECK-NEXT:     let free: Option<unsafe extern "C-unwind" fn(*mut ())> = Some(custom_free);
// CHECK-NEXT:     let shadow: *mut i32 = make_shadow(1i32);
// CHECK-NEXT:     unsafe { free.unwrap()(shadow as *mut ()) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
