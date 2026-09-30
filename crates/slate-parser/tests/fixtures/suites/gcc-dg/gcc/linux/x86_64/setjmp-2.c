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
// DEFAULT-NEXT:     type @type[[TYPE___jmp_buf:[0-9]+]] __jmp_buf = array<i64, 8>;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 __val: array<u64, 16>;
// DEFAULT-NEXT:     } [size=128, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE___sigset_t:[0-9]+]] __sigset_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE___jmp_buf_tag:[0-9]+]] __jmp_buf_tag = struct {
// DEFAULT-NEXT:         field0 __jmpbuf: array<i64, 8>;
// DEFAULT-NEXT:         field1 __mask_was_saved: i32;
// DEFAULT-NEXT:         field2 __saved_mask: @type[[TYPE0]];
// DEFAULT-NEXT:     } [size=200, align=8, offsets=[0, 64, 72]];
// DEFAULT-NEXT:     type @type[[TYPE_jmp_buf:[0-9]+]] jmp_buf = array<@type[[TYPE___jmp_buf_tag]], 1>;
// DEFAULT-NEXT:     type @type[[TYPE___uint32_t:[0-9]+]] __uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___uid_t:[0-9]+]] __uid_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___pid_t:[0-9]+]] __pid_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE___clock_t:[0-9]+]] __clock_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_sigset_t:[0-9]+]] sigset_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_sigval:[0-9]+]] sigval = union {
// DEFAULT-NEXT:         field0 sival_int: i32;
// DEFAULT-NEXT:         field1 sival_ptr: ptr<void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE___sigval_t:[0-9]+]] __sigval_t = @type[[TYPE_sigval]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 si_signo: i32;
// DEFAULT-NEXT:         field1 si_errno: i32;
// DEFAULT-NEXT:         field2 si_code: i32;
// DEFAULT-NEXT:         field3 __pad0: i32;
// DEFAULT-NEXT:         field4 _sifields: @type[[TYPE2:[0-9]+]];
// DEFAULT-NEXT:     } [size=128, align=8, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     type @type[[TYPE2]] = union {
// DEFAULT-NEXT:         field0 _pad: array<i32, 28>;
// DEFAULT-NEXT:         field1 _kill: @type[[TYPE3:[0-9]+]];
// DEFAULT-NEXT:         field2 _timer: @type[[TYPE4:[0-9]+]];
// DEFAULT-NEXT:         field3 _rt: @type[[TYPE5:[0-9]+]];
// DEFAULT-NEXT:         field4 _sigchld: @type[[TYPE6:[0-9]+]];
// DEFAULT-NEXT:         field5 _sigfault: @type[[TYPE7:[0-9]+]];
// DEFAULT-NEXT:         field6 _sigpoll: @type[[TYPE10:[0-9]+]];
// DEFAULT-NEXT:         field7 _sigsys: @type[[TYPE11:[0-9]+]];
// DEFAULT-NEXT:     } [size=112, align=8, offsets=[0, 0, 0, 0, 0, 0, 0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE3]] = struct {
// DEFAULT-NEXT:         field0 si_pid: i32;
// DEFAULT-NEXT:         field1 si_uid: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE4]] = struct {
// DEFAULT-NEXT:         field0 si_tid: i32;
// DEFAULT-NEXT:         field1 si_overrun: i32;
// DEFAULT-NEXT:         field2 si_sigval: @type[[TYPE_sigval]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE5]] = struct {
// DEFAULT-NEXT:         field0 si_pid: i32;
// DEFAULT-NEXT:         field1 si_uid: u32;
// DEFAULT-NEXT:         field2 si_sigval: @type[[TYPE_sigval]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE6]] = struct {
// DEFAULT-NEXT:         field0 si_pid: i32;
// DEFAULT-NEXT:         field1 si_uid: u32;
// DEFAULT-NEXT:         field2 si_status: i32;
// DEFAULT-NEXT:         field3 si_utime: i64;
// DEFAULT-NEXT:         field4 si_stime: i64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 4, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE7]] = struct {
// DEFAULT-NEXT:         field0 si_addr: ptr<void>;
// DEFAULT-NEXT:         field1 si_addr_lsb: i16;
// DEFAULT-NEXT:         field2 _bounds: @type[[TYPE8:[0-9]+]];
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE8]] = union {
// DEFAULT-NEXT:         field0 _addr_bnd: @type[[TYPE9:[0-9]+]];
// DEFAULT-NEXT:         field1 _pkey: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE9]] = struct {
// DEFAULT-NEXT:         field0 _lower: ptr<void>;
// DEFAULT-NEXT:         field1 _upper: ptr<void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE10]] = struct {
// DEFAULT-NEXT:         field0 si_band: i64;
// DEFAULT-NEXT:         field1 si_fd: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE11]] = struct {
// DEFAULT-NEXT:         field0 _call_addr: ptr<void>;
// DEFAULT-NEXT:         field1 _syscall: i32;
// DEFAULT-NEXT:         field2 _arch: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     type @type[[TYPE_siginfo_t:[0-9]+]] siginfo_t = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE___sighandler_t:[0-9]+]] __sighandler_t = ptr<fn(i32) -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_sigaction:[0-9]+]] sigaction = struct {
// DEFAULT-NEXT:         field0 __sigaction_handler: @type[[TYPE12:[0-9]+]];
// DEFAULT-NEXT:         field1 sa_mask: @type[[TYPE0]];
// DEFAULT-NEXT:         field2 sa_flags: i32;
// DEFAULT-NEXT:         field3 sa_restorer: ptr<fn() -> void>;
// DEFAULT-NEXT:     } [size=152, align=8, offsets=[0, 8, 136, 144]];
// DEFAULT-NEXT:     type @type[[TYPE12]] = union {
// DEFAULT-NEXT:         field0 sa_handler: ptr<fn(i32) -> void>;
// DEFAULT-NEXT:         field1 sa_sigaction: ptr<fn(i32, ptr<@type[[TYPE1]]>, ptr<void>) -> void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_segv_jmpbuf:[0-9]+]] segv_jmpbuf: array<@type[[TYPE___jmp_buf_tag]], 1> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_sigfillset:[0-9]+]] @sigfillset(%[[VALUE___set:[0-9]+]] __set: ptr<@type[[TYPE0]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sigprocmask:[0-9]+]] @sigprocmask(%[[VALUE___how:[0-9]+]] __how: i32, %[[VALUE___set_2:[0-9]+]] __set: ptr<const @type[[TYPE0]]> [restrict], %[[VALUE___oset:[0-9]+]] __oset: ptr<@type[[TYPE0]]> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sigaction:[0-9]+]] @sigaction(%[[VALUE___sig:[0-9]+]] __sig: i32, %[[VALUE___act:[0-9]+]] __act: ptr<const @type[[TYPE_sigaction]]> [restrict], %[[VALUE___oact:[0-9]+]] __oact: ptr<@type[[TYPE_sigaction]]> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_longjmp:[0-9]+]] @__builtin_longjmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_segv_handler:[0-9]+]] @segv_handler(%[[VALUE_seg:[0-9]+]] seg: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%[[VALUE___builtin_longjmp]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type[[TYPE___jmp_buf_tag]]>, length=Some(1)>(%[[VALUE_segv_jmpbuf]])), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_setjmp:[0-9]+]] @__builtin_setjmp(%[[VALUE2:[0-9]+]] <unnamed>: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_is_addressable:[0-9]+]] @is_addressable(%[[VALUE_p:[0-9]+]] p: ptr<void>, %[[VALUE_size:[0-9]+]] size: u64) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_cp:[0-9]+]] cp: volatile ptr<volatile i8> [storage=automatic] = pointer_cast<ptr<volatile i8>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         let %[[VALUE_ret:[0-9]+]] ret: volatile i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_sa:[0-9]+]] sa: @type[[TYPE_sigaction]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_origsa:[0-9]+]] origsa: @type[[TYPE_sigaction]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask:[0-9]+]] mask: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<ptr<fn(i32) -> void>>(field0(field0(%[[VALUE_sa]])), function_decay<ptr<fn(i32) -> void>>(%[[VALUE_segv_handler]]));
// DEFAULT-NEXT:         write<i32>(field2(%[[VALUE_sa]]), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE0]]>) -> i32>(%[[VALUE_sigfillset]], addr_of<ptr<@type[[TYPE0]]>>(field1(%[[VALUE_sa]])));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const @type[[TYPE_sigaction]]>, ptr<@type[[TYPE_sigaction]]>) -> i32>(%[[VALUE_sigaction]], const<i32>(11), pointer_cast<ptr<const @type[[TYPE_sigaction]]>, reason=arg>(addr_of<ptr<@type[[TYPE_sigaction]]>>(%[[VALUE_sa]])), addr_of<ptr<@type[[TYPE_sigaction]]>>(%[[VALUE_origsa]]));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const @type[[TYPE0]]>, ptr<@type[[TYPE0]]>) -> i32>(%[[VALUE_sigprocmask]], const<i32>(2), null<ptr<const @type[[TYPE0]]>>, addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_mask]]));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<void>) -> i32>(%[[VALUE___builtin_setjmp]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type[[TYPE___jmp_buf_tag]]>, length=Some(1)>(%[[VALUE_segv_jmpbuf]]))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 while %[[VALUE3:[0-9]+]] {
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_size]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%[[VALUE4]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_size]], read<u64>(%[[VALUE5]]));
// DEFAULT-NEXT:                     yield ne<u64>(read<u64>(%[[VALUE4]]), const<u64>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: ptr<volatile i8> [synthetic] = read<ptr<volatile i8>, volatile>(%[[VALUE_cp]]);
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: ptr<volatile i8> [synthetic] = ptr_offset<ptr<volatile i8>, subtract=false, element=i8, overflow=ub>(read<ptr<volatile i8>>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<volatile i8>, volatile>(%[[VALUE_cp]], read<ptr<volatile i8>>(%[[VALUE7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(%[[VALUE_ret]], const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32, volatile>(%[[VALUE_ret]], const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const @type[[TYPE_sigaction]]>, ptr<@type[[TYPE_sigaction]]>) -> i32>(%[[VALUE_sigaction]], const<i32>(11), pointer_cast<ptr<const @type[[TYPE_sigaction]]>, reason=arg>(addr_of<ptr<@type[[TYPE_sigaction]]>>(%[[VALUE_origsa]])), null<ptr<@type[[TYPE_sigaction]]>>);
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const @type[[TYPE0]]>, ptr<@type[[TYPE0]]>) -> i32>(%[[VALUE_sigprocmask]], const<i32>(2), pointer_cast<ptr<const @type[[TYPE0]]>, reason=arg>(addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_mask]])), null<ptr<@type[[TYPE0]]>>);
// DEFAULT-NEXT:         return read<i32, volatile>(%[[VALUE_ret]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, u64) -> i32>(%[[VALUE_is_addressable]], null<ptr<void>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
