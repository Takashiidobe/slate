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



// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 a: ptr<i8>, %4 b: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @showinfo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 uname: array<i8, 33> [storage=automatic] [align=16] = code_units<array<i8, 33>>([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// DEFAULT-NEXT:         let %7 tty: array<i8, 38> [storage=automatic] [align=16] = code_units<array<i8, 38>>([47, 100, 101, 118, 47, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ptr<i8>) -> void>(%2, array_decay<ptr<i8>, length=Some(33)>(%6), array_decay<ptr<i8>, length=Some(38)>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
