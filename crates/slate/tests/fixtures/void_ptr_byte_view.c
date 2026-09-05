static int all_bytes_identical(const void *src, unsigned long size) {
  unsigned char b = ((const unsigned char *)src)[0];
  for (unsigned long p = 1; p < size; p++) {
    // @lowering-begin
    // @rewrite-begin
    if (((const unsigned char *)src)[p] != b) {
      return 0;
    }
    // @rewrite-end
    // @lowering-end
  }
  return 1;
}

int main(void) {
  unsigned char buf[4] = {7, 7, 7, 7};
  return all_bytes_identical(buf, 4);
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: {
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = p;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = src;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// LOWERING-DAG:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: u8 = b;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:     if {{__v[0-9]+}} {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         __retval = {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:         return {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{__v[0-9]+}}: u64 = p;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut core::ffi::c_void = src;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-DAG: unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// REWRITES-DAG: let {{__v[0-9]+}}: u8 = unsafe { __arg0_view[(p as usize)] };
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG: let {{__v[0-9]+}}: u8 = b;
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG: let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG: if {{__v[0-9]+}} {
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     __retval = {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// REWRITES-DAG:     return {{__v[0-9]+}};
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
