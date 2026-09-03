#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
char *mutable_pick(int i) {
  if (i == 0) {
    return "mut";
  }
  return "other";
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
const char *const_pick(int i) {
  if (i == 0) {
    return "const";
  }
  return "other";
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
unsigned char *bytes_pick(void) { return (unsigned char *)"bytes"; }
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  // @lowering-begin
  // @rewrite-begin
  printf("%s %s %s\n", mutable_pick(0), const_pick(0), (char *)bytes_pick());
  // @rewrite-end
  // @lowering-end
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn mutable_pick({{arg[0-9]+}}: i32) -> *mut i8 {
// LOWERING-DAG:     let mut i: i32 = 0;
// LOWERING-DAG:     let mut __retval: *mut i8 = std::ptr::null_mut();
// LOWERING-DAG:     i = {{arg[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: *mut i8 = b"mut\0".as_ptr() as *mut i8;
// LOWERING-DAG:             __retval = {{_v[0-9]+}};
// LOWERING-DAG:             let {{_v[0-9]+}}: *mut i8 = __retval;
// LOWERING-DAG:             return {{_v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i8 = b"other\0".as_ptr() as *mut i8;
// LOWERING-DAG:     __retval = {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i8 = __retval;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn const_pick({{arg[0-9]+}}: i32) -> *mut i8 {
// LOWERING-DAG:     let mut i: i32 = 0;
// LOWERING-DAG:     let mut __retval: *mut i8 = std::ptr::null_mut();
// LOWERING-DAG:     i = {{arg[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: *mut i8 = b"const\0".as_ptr() as *mut i8;
// LOWERING-DAG:             __retval = {{_v[0-9]+}};
// LOWERING-DAG:             let {{_v[0-9]+}}: *mut i8 = __retval;
// LOWERING-DAG:             return {{_v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i8 = b"other\0".as_ptr() as *mut i8;
// LOWERING-DAG:     __retval = {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i8 = __retval;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn bytes_pick() -> *mut u8 {
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i8 = b"bytes\0".as_ptr() as *mut i8;
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut u8 = {{_v[0-9]+}} as *mut u8;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%s %s %s\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = mutable_pick({{_v[0-9]+}});
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = const_pick({{_v[0-9]+}});
// LOWERING-DAG: let {{_v[0-9]+}}: *mut u8 = bytes_pick();
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn mutable_pick(mut {{_v[0-9]+}}: i32) -> *mut i8 {
// REWRITES-DAG:     if {{_v[0-9]+}} == 0 {
// REWRITES-DAG:         return c"mut".as_ptr() as *mut i8;
// REWRITES-DAG:     }
// REWRITES-DAG:     c"other".as_ptr() as *mut i8
// REWRITES-DAG: }
// REWRITES-DAG: fn const_pick(mut {{_v[0-9]+}}: i32) -> *mut i8 {
// REWRITES-DAG:     if {{_v[0-9]+}} == 0 {
// REWRITES-DAG:         return c"const".as_ptr() as *mut i8;
// REWRITES-DAG:     }
// REWRITES-DAG:     c"other".as_ptr() as *mut i8
// REWRITES-DAG: }
// REWRITES-DAG: fn bytes_pick() -> *mut u8 {
// REWRITES-DAG:     c"bytes".as_ptr() as *mut u8
// REWRITES-DAG: }
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     printf(
// REWRITES-DAG:         c"%s %s %s\n".as_ptr(),
// REWRITES-DAG:         mutable_pick(0),
// REWRITES-DAG:         const_pick(0),
// REWRITES-DAG:         bytes_pick() as *mut i8,
// REWRITES-DAG:     )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
