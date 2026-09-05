static int sum_ints(const void *src, unsigned long n) {
  int total = 0;
  for (unsigned long i = 0; i < n; i++) {
    // @lowering-begin
    // @rewrite-begin
    total += ((const int *)src)[i];
    // @rewrite-end
    // @lowering-end
  }
  return total;
}

int main(void) {
  int buf[4] = {1, 2, 3, 4};
  return sum_ints(buf, 4);
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{__v[0-9]+}}: u64 = i;
// LOWERING-DAG: let {{__v[0-9]+}}: *mut core::ffi::c_void = src;
// LOWERING-DAG: let {{__v[0-9]+}}: *mut i32 = {{__v[0-9]+}} as *mut i32;
// LOWERING-DAG: let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = total;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG: total = {{__v[0-9]+}};
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{__v[0-9]+}}: u64 = i;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut core::ffi::c_void = src;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i32 = {{__v[0-9]+}} as *mut i32;
// REWRITES-DAG: unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = unsafe { __arg0_view[(i as usize)] };
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = total;
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG: total = {{__v[0-9]+}};
// SLATE-FILECHECK-END rewrites
