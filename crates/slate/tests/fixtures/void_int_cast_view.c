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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u64 = i;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: *mut core::ffi::c_void = src;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: *mut i32 = {{__v[0-9]+}} as *mut i32;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG: total = {{__v[0-9]+}};
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: *mut i32 = src as *mut i32;
// COMMON-REWRITES-DAG: unsafe { {{__v[0-9]+}}.add(i as usize) };
// COMMON-REWRITES-DAG: total += unsafe { __arg0_view[(i as usize)] };
// SLATE-FILECHECK-END common-rewrites
