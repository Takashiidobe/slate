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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = p;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = src;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u8 = b;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:         return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: *mut u8 = src as *mut u8;
// COMMON-REWRITES-DAG: unsafe { {{__v[0-9]+}}.add(p as usize) };
// COMMON-REWRITES-DAG: if ((unsafe { __arg0_view[(p as usize)] }) as i32) != (b as i32) {
// COMMON-REWRITES-DAG:     return 0;
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
