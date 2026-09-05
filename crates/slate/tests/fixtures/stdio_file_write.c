#include <stdio.h>

// @rewrite-fn-begin
int main(void) {
  remove("slate_stdio_file_write.tmp");
  FILE *f = fopen("slate_stdio_file_write.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("owned\n", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_file_write.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char buf[16] = {0};
  fgets(buf, sizeof buf, g);
  fclose(g);
  fputs(buf, stdout);
  remove("slate_stdio_file_write.tmp");
  return 0;
}
// @rewrite-fn-end

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn main() {
// COMMON-REWRITES-DAG:     let mut __retval: i32 = 0;
// COMMON-REWRITES-DAG:     let mut f: *mut libc::FILE = std::ptr::null_mut();
// COMMON-REWRITES-DAG:     let mut g: *mut libc::FILE = std::ptr::null_mut();
// COMMON-REWRITES-DAG:     unsafe { remove(c"slate_stdio_file_write.tmp".as_ptr()) };
// COMMON-REWRITES-DAG:     f = unsafe { fopen(c"slate_stdio_file_write.tmp".as_ptr(), c"w".as_ptr()) };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = !(f != std::ptr::null_mut());
// COMMON-REWRITES-DAG:     if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         unsafe { puts(c"open-fail".as_ptr()) };
// COMMON-REWRITES-DAG:         __retval = 0;
// COMMON-REWRITES-DAG:         std::process::exit(__retval as i32);
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     unsafe { fputs(c"owned\n".as_ptr(), f as *mut libc::FILE) };
// COMMON-REWRITES-DAG:     unsafe { fclose(f as *mut libc::FILE) };
// COMMON-REWRITES-DAG:     g = unsafe { fopen(c"slate_stdio_file_write.tmp".as_ptr(), c"r".as_ptr()) };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = !(g != std::ptr::null_mut());
// COMMON-REWRITES-DAG:     if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         unsafe { puts(c"reopen-fail".as_ptr()) };
// COMMON-REWRITES-DAG:         __retval = 0;
// COMMON-REWRITES-DAG:         std::process::exit(__retval as i32);
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     (unsafe {
// COMMON-REWRITES-DAG:         fgets(
// COMMON-REWRITES-DAG:             buf.as_mut_ptr() as *mut core::ffi::c_char,
// COMMON-REWRITES-DAG:             16 as i32,
// COMMON-REWRITES-DAG:             g as *mut libc::FILE,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     unsafe { fclose(g as *mut libc::FILE) };
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         fputs(
// COMMON-REWRITES-DAG:             buf.as_mut_ptr() as *const core::ffi::c_char,
// COMMON-REWRITES-DAG:             (unsafe { stdout }) as *mut libc::FILE,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     unsafe { remove(c"slate_stdio_file_write.tmp".as_ptr()) };
// COMMON-REWRITES-DAG:     __retval = 0;
// COMMON-REWRITES-DAG:     std::process::exit(__retval as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG:     let mut buf: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-X86_64-GNU-DAG:     *buf = [0; 16];
// REWRITES-X86_64-GNU-DAG:     }) as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG:     let mut buf: [u8; 16] = [0; 16];
// REWRITES-AARCH64-GNU-DAG:     buf = [0; 16];
// REWRITES-AARCH64-GNU-DAG:     }) as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
