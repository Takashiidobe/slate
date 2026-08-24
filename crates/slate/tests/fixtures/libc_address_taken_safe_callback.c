#include <stdio.h>
#include <stdlib.h>

typedef void *(*ReallocFn)(void *, size_t);
typedef void  (*FreeFn)(void *);

struct Allocator {
  ReallocFn realloc_fn;
  FreeFn    free_fn;
};

struct Allocator alloc = {realloc, free};

int main(void) {
  void *p = alloc.realloc_fn(NULL, 8);
  alloc.free_fn(p);
  printf("ok\n");
  return 0;
}
// REWRITES-DAG: realloc_fn: Option<unsafe extern "C" fn(*mut core::ffi::c_void, u64) -> *mut core::ffi::c_void>
// REWRITES-DAG: free_fn: Option<unsafe extern "C" fn(*mut core::ffi::c_void)>
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: unsafe { unsafe { alloc.realloc_fn }.unwrap()(
// REWRITES-DAG: unsafe { unsafe { alloc.free_fn }.unwrap()(p) };
// REWRITES: {{^}}}
