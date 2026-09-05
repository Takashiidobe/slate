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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG:     let mut i: i32 = 0;
// COMMON-LOWERING-DAG:     i = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG:     let mut i: i32 = 0;
// COMMON-LOWERING-DAG:     i = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn bytes_pick() -> *mut u8 {
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: *mut u8 = bytes_pick();
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: fn mutable_pick({{arg[0-9]+}}: i32) -> *mut i8 {
// LOWERING-X86_64-GNU-DAG:     let mut __retval: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = b"mut\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = __retval;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"other\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = __retval;
// LOWERING-X86_64-GNU-DAG: fn const_pick({{arg[0-9]+}}: i32) -> *mut i8 {
// LOWERING-X86_64-GNU-DAG:     let mut __retval: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = b"const\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = __retval;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"other\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = __retval;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"bytes\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%s %s %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = mutable_pick({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = const_pick({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: fn mutable_pick({{arg[0-9]+}}: i32) -> *mut u8 {
// LOWERING-AARCH64-GNU-DAG:     let mut __retval: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = b"mut\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = __retval;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"other\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = __retval;
// LOWERING-AARCH64-GNU-DAG: fn const_pick({{arg[0-9]+}}: i32) -> *mut u8 {
// LOWERING-AARCH64-GNU-DAG:     let mut __retval: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = b"const\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = __retval;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"other\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = __retval;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"bytes\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%s %s %s\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = mutable_pick({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = const_pick({{__v[0-9]+}});
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG:     if {{__v[0-9]+}} == 0 {
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG:     if {{__v[0-9]+}} == 0 {
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn bytes_pick() -> *mut u8 {
// COMMON-REWRITES-DAG:     c"bytes".as_ptr() as *mut u8
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     printf(
// COMMON-REWRITES-DAG:         c"%s %s %s\n".as_ptr(),
// COMMON-REWRITES-DAG:         mutable_pick(0),
// COMMON-REWRITES-DAG:         const_pick(0),
// COMMON-REWRITES-DAG:     )
// COMMON-REWRITES-DAG: };
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: fn mutable_pick(mut {{__v[0-9]+}}: i32) -> *mut i8 {
// REWRITES-X86_64-GNU-DAG:         return c"mut".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-DAG:     c"other".as_ptr() as *mut i8
// REWRITES-X86_64-GNU-DAG: fn const_pick(mut {{__v[0-9]+}}: i32) -> *mut i8 {
// REWRITES-X86_64-GNU-DAG:         return c"const".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-DAG:     c"other".as_ptr() as *mut i8
// REWRITES-X86_64-GNU-DAG:         bytes_pick() as *mut i8,
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: fn mutable_pick(mut {{__v[0-9]+}}: i32) -> *mut u8 {
// REWRITES-AARCH64-GNU-DAG:         return c"mut".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-DAG:     c"other".as_ptr() as *mut u8
// REWRITES-AARCH64-GNU-DAG: fn const_pick(mut {{__v[0-9]+}}: i32) -> *mut u8 {
// REWRITES-AARCH64-GNU-DAG:         return c"const".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-DAG:     c"other".as_ptr() as *mut u8
// REWRITES-AARCH64-GNU-DAG:         bytes_pick(),
// SLATE-FILECHECK-END rewrites-aarch64-gnu
