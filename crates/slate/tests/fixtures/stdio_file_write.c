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

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut __retval: i32 = 0;
// REWRITES-DAG:     let mut f: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-DAG:     let mut g: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-X86_64-GNU-DAG:     let mut buf: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-AARCH64-GNU-DAG:     let mut buf: [u8; 16] = [0; 16];
// REWRITES-DAG:     unsafe { remove(c"slate_stdio_file_write.tmp".as_ptr()) };
// REWRITES-DAG:     f = unsafe { fopen(c"slate_stdio_file_write.tmp".as_ptr(), c"w".as_ptr()) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !(f != std::ptr::null_mut());
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { puts(c"open-fail".as_ptr()) };
// REWRITES-DAG:         __retval = 0;
// REWRITES-DAG:         std::process::exit(__retval as i32);
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { fputs(c"owned\n".as_ptr(), f as *mut libc::FILE) };
// REWRITES-DAG:     unsafe { fclose(f as *mut libc::FILE) };
// REWRITES-DAG:     g = unsafe { fopen(c"slate_stdio_file_write.tmp".as_ptr(), c"r".as_ptr()) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !(g != std::ptr::null_mut());
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { puts(c"reopen-fail".as_ptr()) };
// REWRITES-DAG:         __retval = 0;
// REWRITES-DAG:         std::process::exit(__retval as i32);
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     *buf = [0; 16];
// REWRITES-AARCH64-GNU-DAG:     buf = [0; 16];
// REWRITES-DAG:     (unsafe {
// REWRITES-DAG:         fgets(
// REWRITES-DAG:             buf.as_mut_ptr() as *mut core::ffi::c_char,
// REWRITES-DAG:             16 as i32,
// REWRITES-DAG:             g as *mut libc::FILE,
// REWRITES-DAG:         )
// REWRITES-X86_64-GNU-DAG:     }) as *mut i8;
// REWRITES-AARCH64-GNU-DAG:     }) as *mut u8;
// REWRITES-DAG:     unsafe { fclose(g as *mut libc::FILE) };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         fputs(
// REWRITES-DAG:             buf.as_mut_ptr() as *const core::ffi::c_char,
// REWRITES-DAG:             (unsafe { stdout }) as *mut libc::FILE,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe { remove(c"slate_stdio_file_write.tmp".as_ptr()) };
// REWRITES-DAG:     __retval = 0;
// REWRITES-DAG:     std::process::exit(__retval as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
