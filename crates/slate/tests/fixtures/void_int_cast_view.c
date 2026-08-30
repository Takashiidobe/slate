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
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = i;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut core::ffi::c_void = src;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add({{_v[0-9]+}} as usize) };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = total;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: total = {{_v[0-9]+}};
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i32 = src as *mut i32;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(i as usize) };
// REWRITES-DAG: total = total + unsafe { __arg0_view[(i as usize)] };
// SLATE-FILECHECK-END rewrites
