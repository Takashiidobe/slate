/* PR middle-end/17813 */
/* Origin: Tom Hughes <tom@compton.nu> */
/* { dg-do run { target i?86-*-linux* i?86-*-gnu* x86_64-*-linux* } } */
/* { dg-options "-O -fomit-frame-pointer" } */
/* { dg-options "-O -fomit-frame-pointer -march=i386" { target { { i?86-*-* x86_64-*-* } && ia32 } } } */

#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>

static jmp_buf segv_jmpbuf;

static void segv_handler(int seg)
{
   __builtin_longjmp(segv_jmpbuf, 1);
}

static int is_addressable(void *p, size_t size)
{
   volatile char * volatile cp = (volatile char *)p;
   volatile int ret;
   struct sigaction sa, origsa;
   sigset_t mask;
   
   sa.sa_handler = segv_handler;
   sa.sa_flags = 0;
   sigfillset(&sa.sa_mask);
   sigaction(SIGSEGV, &sa, &origsa);
   sigprocmask(SIG_SETMASK, NULL, &mask);

   if (__builtin_setjmp(segv_jmpbuf) == 0) {
      while(size--)
	 *cp++;
      ret = 1;
    } else
      ret = 0;

   sigaction(SIGSEGV, &origsa, NULL);
   sigprocmask(SIG_SETMASK, &mask, NULL);

   return ret;
}

int main(int argc, char **argv)
{
   is_addressable(0x0, 1);
   return 0;
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type0 __jmp_buf = array<i64, 8>;
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 __val: array<u64, 16>;
// DEFAULT-NEXT:     } [size=128, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type2 __sigset_t = @type1;
// DEFAULT-NEXT:     type @type3 __jmp_buf_tag = struct {
// DEFAULT-NEXT:         field0 __jmpbuf: array<i64, 8>;
// DEFAULT-NEXT:         field1 __mask_was_saved: i32;
// DEFAULT-NEXT:         field2 __saved_mask: @type1;
// DEFAULT-NEXT:     } [size=200, align=8, offsets=[0, 64, 72]];
// DEFAULT-NEXT:     type @type4 jmp_buf = array<@type3, 1>;
// DEFAULT-NEXT:     type @type5 __uint32_t = u32;
// DEFAULT-NEXT:     type @type6 __uid_t = u32;
// DEFAULT-NEXT:     type @type7 __pid_t = i32;
// DEFAULT-NEXT:     type @type8 __clock_t = i64;
// DEFAULT-NEXT:     type @type9 sigset_t = @type1;
// DEFAULT-NEXT:     type @type10 sigval = union {
// DEFAULT-NEXT:         field0 sival_int: i32;
// DEFAULT-NEXT:         field1 sival_ptr: ptr<void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type11 __sigval_t = @type10;
// DEFAULT-NEXT:     type @type12 = struct {
// DEFAULT-NEXT:         field0 si_signo: i32;
// DEFAULT-NEXT:         field1 si_errno: i32;
// DEFAULT-NEXT:         field2 si_code: i32;
// DEFAULT-NEXT:         field3 __pad0: i32;
// DEFAULT-NEXT:         field4 _sifields: @type13;
// DEFAULT-NEXT:     } [size=128, align=8, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     type @type13 = union {
// DEFAULT-NEXT:         field0 _pad: array<i32, 28>;
// DEFAULT-NEXT:         field1 _kill: @type14;
// DEFAULT-NEXT:         field2 _timer: @type15;
// DEFAULT-NEXT:         field3 _rt: @type16;
// DEFAULT-NEXT:         field4 _sigchld: @type17;
// DEFAULT-NEXT:         field5 _sigfault: @type18;
// DEFAULT-NEXT:         field6 _sigpoll: @type21;
// DEFAULT-NEXT:         field7 _sigsys: @type22;
// DEFAULT-NEXT:     } [size=112, align=8, offsets=[0, 0, 0, 0, 0, 0, 0, 0]];
// DEFAULT-NEXT:     type @type14 = struct {
// DEFAULT-NEXT:         field0 si_pid: i32;
// DEFAULT-NEXT:         field1 si_uid: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type15 = struct {
// DEFAULT-NEXT:         field0 si_tid: i32;
// DEFAULT-NEXT:         field1 si_overrun: i32;
// DEFAULT-NEXT:         field2 si_sigval: @type10;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type16 = struct {
// DEFAULT-NEXT:         field0 si_pid: i32;
// DEFAULT-NEXT:         field1 si_uid: u32;
// DEFAULT-NEXT:         field2 si_sigval: @type10;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type17 = struct {
// DEFAULT-NEXT:         field0 si_pid: i32;
// DEFAULT-NEXT:         field1 si_uid: u32;
// DEFAULT-NEXT:         field2 si_status: i32;
// DEFAULT-NEXT:         field3 si_utime: i64;
// DEFAULT-NEXT:         field4 si_stime: i64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 4, 8, 16, 24]];
// DEFAULT-NEXT:     type @type18 = struct {
// DEFAULT-NEXT:         field0 si_addr: ptr<void>;
// DEFAULT-NEXT:         field1 si_addr_lsb: i16;
// DEFAULT-NEXT:         field2 _bounds: @type19;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type19 = union {
// DEFAULT-NEXT:         field0 _addr_bnd: @type20;
// DEFAULT-NEXT:         field1 _pkey: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type20 = struct {
// DEFAULT-NEXT:         field0 _lower: ptr<void>;
// DEFAULT-NEXT:         field1 _upper: ptr<void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type21 = struct {
// DEFAULT-NEXT:         field0 si_band: i64;
// DEFAULT-NEXT:         field1 si_fd: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type22 = struct {
// DEFAULT-NEXT:         field0 _call_addr: ptr<void>;
// DEFAULT-NEXT:         field1 _syscall: i32;
// DEFAULT-NEXT:         field2 _arch: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     type @type23 siginfo_t = @type12;
// DEFAULT-NEXT:     type @type24 __sighandler_t = ptr<fn(i32) -> void>;
// DEFAULT-NEXT:     type @type25 sigaction = struct {
// DEFAULT-NEXT:         field0 __sigaction_handler: @type26;
// DEFAULT-NEXT:         field1 sa_mask: @type1;
// DEFAULT-NEXT:         field2 sa_flags: i32;
// DEFAULT-NEXT:         field3 sa_restorer: ptr<fn() -> void>;
// DEFAULT-NEXT:     } [size=152, align=8, offsets=[0, 8, 136, 144]];
// DEFAULT-NEXT:     type @type26 = union {
// DEFAULT-NEXT:         field0 sa_handler: ptr<fn(i32) -> void>;
// DEFAULT-NEXT:         field1 sa_sigaction: ptr<fn(i32, ptr<@type12>, ptr<void>) -> void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type27 size_t = u64;
// DEFAULT-NEXT:     global %38 segv_jmpbuf: array<@type3, 1> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     fn %26 @sigfillset(%52 __set: ptr<@type1>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %32 @sigprocmask(%53 __how: i32, %54 __set: ptr<const @type1> [restrict], %55 __oset: ptr<@type1> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %36 @sigaction(%56 __sig: i32, %57 __act: ptr<const @type25> [restrict], %58 __oact: ptr<@type25> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %61 @__builtin_longjmp(%59 <unnamed>: ptr<void>, %60 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %39 @segv_handler(%40 seg: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%61, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type3>, length=Some(1)>(%38)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %63 @__builtin_setjmp(%62 <unnamed>: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %41 @is_addressable(%42 p: ptr<void>, %43 size: u64) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %44 cp: volatile ptr<volatile i8> [storage=automatic] = pointer_cast<ptr<volatile i8>, reason=explicit>(read<ptr<void>>(%42));
// DEFAULT-NEXT:         let %45 ret: volatile i32 [storage=automatic];
// DEFAULT-NEXT:         let %46 sa: @type25 [storage=automatic];
// DEFAULT-NEXT:         let %47 origsa: @type25 [storage=automatic];
// DEFAULT-NEXT:         let %48 mask: @type1 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<fn(i32) -> void>>(field0(field0(%46)), function_decay<ptr<fn(i32) -> void>>(%39));
// DEFAULT-NEXT:         write<i32>(field2(%46), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type1>) -> i32>(%26, addr_of<ptr<@type1>>(field1(%46)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const @type25>, ptr<@type25>) -> i32>(%36, const<i32>(11), pointer_cast<ptr<const @type25>, reason=arg>(addr_of<ptr<@type25>>(%46)), addr_of<ptr<@type25>>(%47));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const @type1>, ptr<@type1>) -> i32>(%32, const<i32>(2), null<ptr<const @type1>>, addr_of<ptr<@type1>>(%48));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<void>) -> i32>(%63, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type3>, length=Some(1)>(%38))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 while %64 {
// DEFAULT-NEXT:                     let %65: u64 [synthetic] = read<u64>(%43);
// DEFAULT-NEXT:                     let %66: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%65), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                     write<u64>(%43, read<u64>(%66));
// DEFAULT-NEXT:                     yield ne<u64>(read<u64>(%65), const<u64>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                     let %67: ptr<volatile i8> [synthetic] = read<ptr<volatile i8>, volatile>(%44);
// DEFAULT-NEXT:                     let %68: ptr<volatile i8> [synthetic] = ptr_offset<ptr<volatile i8>, subtract=false, element=i8, overflow=ub>(read<ptr<volatile i8>>(%67), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<volatile i8>, volatile>(%44, read<ptr<volatile i8>>(%68));
// DEFAULT-NEXT:                 write<i32, volatile>(%45, const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32, volatile>(%45, const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const @type25>, ptr<@type25>) -> i32>(%36, const<i32>(11), pointer_cast<ptr<const @type25>, reason=arg>(addr_of<ptr<@type25>>(%47)), null<ptr<@type25>>);
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const @type1>, ptr<@type1>) -> i32>(%32, const<i32>(2), pointer_cast<ptr<const @type1>, reason=arg>(addr_of<ptr<@type1>>(%48)), null<ptr<@type1>>);
// DEFAULT-NEXT:         return read<i32, volatile>(%45);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @main(%50 argc: i32, %51 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, u64) -> i32>(%41, null<ptr<void>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
