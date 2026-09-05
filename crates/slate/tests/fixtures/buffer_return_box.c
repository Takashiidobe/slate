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

// @rewrite-fn-begin
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
// @rewrite-fn-end

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut m: *mut i32 = maybe(2);
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = Box::into_raw(make(4)).cast::<i32>();
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = Box::into_raw(make8()).cast::<i32>();
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = allocfree(3);
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = Box::into_raw(make_raw(2)).cast::<i32>();
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = Box::into_raw(make_raw(2)).cast::<i32>();
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d %d %d %d\n".as_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = c"%d %d %d %d %d %d\n".as_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(7) };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = (m != std::ptr::null_mut()) as i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} != std::ptr::null_mut()) as i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = read_first({{__v[0-9]+}});
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         printf(
// REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             unsafe { *{{__v[0-9]+}} },
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-DAG:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = m != std::ptr::null_mut();
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { free(m as *mut core::ffi::c_void) };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-DAG:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-DAG:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = Box::into_raw(make_shadow(1)).cast::<i32>();
// REWRITES-DAG:     unsafe { Some(custom_free).unwrap()({{__v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
