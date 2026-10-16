/* This testcase originally provoked an unaligned access fault on Alpha.

   Since Digital Unix and Linux (and probably others) by default fix
   these up in the kernel, the failure was not visible unless one
   is sitting at the console examining logs.

   So: If we know how, ask the kernel to deliver SIGBUS instead so
   that the test case visibly fails.  */

void abort(void);
void exit(int);

#if defined(__alpha__) && defined(__linux__)
#include <asm/sysinfo.h>
#include <asm/unistd.h>

int syscall(int, ...);

static inline int setsysinfo(unsigned long op, void *buffer, unsigned long size,
                             int *start, void *arg, unsigned long flag) {
  syscall(__NR_osf_setsysinfo, op, buffer, size, start, arg, flag);
}

static void __attribute__((constructor)) trap_unaligned(void) {
  unsigned int buf[2];
  buf[0] = SSIN_UACPROC;
  buf[1] = UAC_SIGBUS | UAC_NOPRINT;
  setsysinfo(SSI_NVPAIRS, buf, 1, 0, 0, 0);
}
#endif /* alpha */

void foo(char *a, char *b) {}

void showinfo() {
  char uname[33] = "", tty[38] = "/dev/";
  foo(uname, tty);
}

int main() {
  showinfo();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This testcase originally provoked an unaligned access fault on Alpha.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Since Digital Unix and Linux (and probably others) by default fix
// CHECK-NEXT: //! these up in the kernel, the failure was not visible unless one
// CHECK-NEXT: //! is sitting at the console examining logs.
// CHECK-NEXT: //!
// CHECK-NEXT: //! So: If we know how, ask the kernel to deliver SIGBUS instead so
// CHECK-NEXT: //! that the test case visibly fails.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut a: *mut i8, mut b: *mut i8) {}
// CHECK-EMPTY:
// CHECK-NEXT: fn showinfo() {
// CHECK-NEXT:     let mut uname: __SlateAlign16<[i8; 33]> = __SlateAlign16([
// CHECK-NEXT:         0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:         0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:         0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:         0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut tty: __SlateAlign16<[i8; 38]> = __SlateAlign16([
// CHECK-NEXT:         47 as i8, 100 as i8, 101 as i8, 118 as i8, 47 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:         0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:         0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:         0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8, 0 as i8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     foo(
// CHECK-NEXT:         uname.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         tty.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     showinfo();
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
