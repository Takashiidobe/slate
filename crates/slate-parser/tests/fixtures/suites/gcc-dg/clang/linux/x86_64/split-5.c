/* { dg-do run } */
/* { dg-require-effective-target split_stack } */
/* { dg-require-effective-target pthread_h } */
/* { dg-require-effective-target ucontext_h } */
/* { dg-options "-pthread -fsplit-stack" } */

#include <stdlib.h>
#include <pthread.h>
#include <ucontext.h>

extern void __splitstack_getcontext (void *context[10]);

extern void __splitstack_setcontext (void *context[10]);

extern void *__splitstack_makecontext (size_t, void *context[10], size_t *);

extern void __splitstack_block_signals (int *, int *);

extern void __splitstack_block_signals_context (void *context[10], int *,
						int *);

extern void *__splitstack_find (void *, void *, size_t *, void **, void **,
				void **);

extern void *__splitstack_find_context (void *context[10], size_t *, void **,
					void **, void **);

static ucontext_t c1;
static void *s1[10];

static ucontext_t c2;
static void *s2[10];

static void swap (ucontext_t *, void *fs[10], ucontext_t *, void *ts[10])
  __attribute__ ((no_split_stack));

static void
swap (ucontext_t *fu, void *fs[10], ucontext_t *tu, void *ts[10])
{
  __splitstack_getcontext (fs);
  __splitstack_setcontext (ts);
  swapcontext (fu, tu);
  __splitstack_setcontext (fs);
}

/* Use a noinline function to ensure that the buffer is not removed
   from the stack.  */
static void use_buffer (char *buf) __attribute__ ((noinline));
static void
use_buffer (char *buf)
{
  buf[0] = '\0';
}

static void
down (int i, const char *msg, ucontext_t *me, void *mes[10],
      ucontext_t *other, void *others[10])
{
  char buf[10000];

  if (i > 0)
    {
      use_buffer (buf);
      swap (me, mes, other, others);
      down (i - 1, msg, me, mes, other, others);
    }
  else
    {
      int c = 0;
      void *stack;
      size_t stack_size;
      void *next_segment = NULL;
      void *next_sp = NULL;
      void *initial_sp = NULL;

      stack = __splitstack_find_context (mes, &stack_size, &next_segment,
					&next_sp, &initial_sp);
      if (stack != NULL)
	{
	  ++c;
	  while (__splitstack_find (next_segment, next_sp, &stack_size,
				    &next_segment, &next_sp, &initial_sp)
		 != NULL)
	    ++c;
	}
    }
}

static void
go1 (void)
{
  down (1000, "go1", &c1, s1, &c2, s2);
  pthread_exit (NULL);
}

static void
go2 (void)
{
  down (1000, "go2", &c2, s2, &c1, s1);
  pthread_exit (NULL);
}

struct thread_context
{
  ucontext_t *u;
  void **s;
};

static void *start_thread (void *) __attribute__ ((no_split_stack));

static void *
start_thread (void *context)
{
  struct thread_context *tc = (struct thread_context *) context;
  int block;

  block = 0;
  __splitstack_block_signals (&block, NULL);
  __splitstack_setcontext (tc->s);
  setcontext (tc->u);
  abort ();
}

int
main (int argc __attribute__ ((unused)), char **argv __attribute__ ((unused)))
{
  pthread_t tid;
  int err;
  size_t size;
  struct thread_context tc;
  int block;

  if (getcontext (&c1) < 0)
    abort ();

  c2 = c1;

  c1.uc_stack.ss_sp = __splitstack_makecontext (8192, &s1[0], &size);
  if (c1.uc_stack.ss_sp == NULL)
    abort ();
  c1.uc_stack.ss_flags = 0;
  c1.uc_stack.ss_size = size;
  c1.uc_link = NULL;
  block = 0;
  __splitstack_block_signals_context (&s1[0], &block, NULL);
  makecontext (&c1, go1, 0);

  c2.uc_stack.ss_sp = __splitstack_makecontext (8192, &s2[0], &size);
  if (c2.uc_stack.ss_sp == NULL)
    abort ();
  c2.uc_stack.ss_flags = 0;
  c2.uc_stack.ss_size = size;
  c2.uc_link = NULL;
  __splitstack_block_signals_context (&s2[0], &block, NULL);
  makecontext (&c2, go2, 0);

  block = 0;
  __splitstack_block_signals (&block, NULL);

  tc.u = &c1;
  tc.s = &s1[0];
  err = pthread_create (&tid, NULL, start_thread, &tc);
  if (err != 0)
    abort ();

  err = pthread_join (tid, NULL);
  if (err != 0)
    abort ();

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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 __uint16_t = u16;
// DEFAULT-NEXT:     type @type2 __uint32_t = u32;
// DEFAULT-NEXT:     type @type3 __uint64_t = u64;
// DEFAULT-NEXT:     type @type4 = struct {
// DEFAULT-NEXT:         field0 __val: array<u64, 16>;
// DEFAULT-NEXT:     } [size=128, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type5 __sigset_t = @type4;
// DEFAULT-NEXT:     type @type6 sigset_t = @type4;
// DEFAULT-NEXT:     type @type7 pthread_t = u64;
// DEFAULT-NEXT:     type @type8 pthread_attr_t = union {
// DEFAULT-NEXT:         field0 __size: array<i8, 56>;
// DEFAULT-NEXT:         field1 __align: i64;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type9 pthread_attr_t = @type8;
// DEFAULT-NEXT:     type @type10 = struct {
// DEFAULT-NEXT:         field0 ss_sp: ptr<void>;
// DEFAULT-NEXT:         field1 ss_flags: i32;
// DEFAULT-NEXT:         field2 ss_size: u64;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type11 stack_t = @type10;
// DEFAULT-NEXT:     type @type12 greg_t = i64;
// DEFAULT-NEXT:     type @type13 gregset_t = array<i64, 23>;
// DEFAULT-NEXT:     type @type14 _libc_fpxreg = struct {
// DEFAULT-NEXT:         field0 significand: array<u16, 4>;
// DEFAULT-NEXT:         field1 exponent: u16;
// DEFAULT-NEXT:         field2 __glibc_reserved1: array<u16, 3>;
// DEFAULT-NEXT:     } [size=16, align=2, offsets=[0, 8, 10]];
// DEFAULT-NEXT:     type @type15 _libc_xmmreg = struct {
// DEFAULT-NEXT:         field0 element: array<u32, 4>;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type16 _libc_fpstate = struct {
// DEFAULT-NEXT:         field0 cwd: u16;
// DEFAULT-NEXT:         field1 swd: u16;
// DEFAULT-NEXT:         field2 ftw: u16;
// DEFAULT-NEXT:         field3 fop: u16;
// DEFAULT-NEXT:         field4 rip: u64;
// DEFAULT-NEXT:         field5 rdp: u64;
// DEFAULT-NEXT:         field6 mxcsr: u32;
// DEFAULT-NEXT:         field7 mxcr_mask: u32;
// DEFAULT-NEXT:         field8 _st: array<@type14, 8>;
// DEFAULT-NEXT:         field9 _xmm: array<@type15, 16>;
// DEFAULT-NEXT:         field10 __glibc_reserved1: array<u32, 24>;
// DEFAULT-NEXT:     } [size=512, align=8, offsets=[0, 2, 4, 6, 8, 16, 24, 28, 32, 160, 416]];
// DEFAULT-NEXT:     type @type17 fpregset_t = ptr<@type16>;
// DEFAULT-NEXT:     type @type18 = struct {
// DEFAULT-NEXT:         field0 gregs: array<i64, 23>;
// DEFAULT-NEXT:         field1 fpregs: ptr<@type16>;
// DEFAULT-NEXT:         field2 __reserved1: array<u64, 8>;
// DEFAULT-NEXT:     } [size=256, align=8, offsets=[0, 184, 192]];
// DEFAULT-NEXT:     type @type19 mcontext_t = @type18;
// DEFAULT-NEXT:     type @type20 ucontext_t = struct {
// DEFAULT-NEXT:         field0 uc_flags: u64;
// DEFAULT-NEXT:         field1 uc_link: ptr<@type20>;
// DEFAULT-NEXT:         field2 uc_stack: @type10;
// DEFAULT-NEXT:         field3 uc_mcontext: @type18;
// DEFAULT-NEXT:         field4 uc_sigmask: @type4;
// DEFAULT-NEXT:         field5 __fpregs_mem: @type16;
// DEFAULT-NEXT:         field6 __ssp: array<u64, 4>;
// DEFAULT-NEXT:     } [size=968, align=8, offsets=[0, 8, 16, 40, 296, 424, 936]];
// DEFAULT-NEXT:     type @type21 ucontext_t = @type20;
// DEFAULT-NEXT:     type @type22 thread_context = struct {
// DEFAULT-NEXT:         field0 u: ptr<@type20>;
// DEFAULT-NEXT:         field1 s: ptr<ptr<void>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %56 c1: @type20 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %57 s1: array<ptr<void>, 10> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %58 c2: @type20 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %59 s2: array<ptr<void>, 10> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %140 .str140: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([103, 111, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %141 .str141: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([103, 111, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %10 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %15 @pthread_create(%99 __newthread: ptr<u64> [restrict], %100 __attr: ptr<const @type8> [restrict], %101 __start_routine: ptr<fn(ptr<void>) -> ptr<void>>, %102 __arg: ptr<void> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %17 @pthread_exit(%103 __retval: ptr<void>) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %20 @pthread_join(%104 __th: u64, %105 __thread_return: ptr<ptr<void>>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %34 @getcontext(%106 __ucp: ptr<@type20>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %36 @setcontext(%107 __ucp: ptr<const @type20>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %39 @swapcontext(%108 __oucp: ptr<@type20> [restrict], %109 __ucp: ptr<const @type20> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %43 @makecontext(%110 __ucp: ptr<@type20>, %111 __func: ptr<fn() -> void>, %112 __argc: i32, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %45 @__splitstack_getcontext(%113 context: ptr<ptr<void>> [array=10]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %47 @__splitstack_setcontext(%114 context: ptr<ptr<void>> [array=10]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %49 @__splitstack_makecontext(%115 <unnamed>: u64, %116 context: ptr<ptr<void>> [array=10], %117 <unnamed>: ptr<u64>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %50 @__splitstack_block_signals(%118 <unnamed>: ptr<i32>, %119 <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %52 @__splitstack_block_signals_context(%120 context: ptr<ptr<void>> [array=10], %121 <unnamed>: ptr<i32>, %122 <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %53 @__splitstack_find(%123 <unnamed>: ptr<void>, %124 <unnamed>: ptr<void>, %125 <unnamed>: ptr<u64>, %126 <unnamed>: ptr<ptr<void>>, %127 <unnamed>: ptr<ptr<void>>, %128 <unnamed>: ptr<ptr<void>>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %55 @__splitstack_find_context(%129 context: ptr<ptr<void>> [array=10], %130 <unnamed>: ptr<u64>, %131 <unnamed>: ptr<ptr<void>>, %132 <unnamed>: ptr<ptr<void>>, %133 <unnamed>: ptr<ptr<void>>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %62 @swap(%63 fu: ptr<@type20>, %64 fs: ptr<ptr<void>> [array=10], %65 tu: ptr<@type20>, %66 ts: ptr<ptr<void>> [array=10]) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>) -> void>(%45, read<ptr<ptr<void>>>(%64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>) -> void>(%47, read<ptr<ptr<void>>>(%66));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type20>, ptr<const @type20>) -> i32>(%39, read<ptr<@type20>>(%63), pointer_cast<ptr<const @type20>, reason=arg>(read<ptr<@type20>>(%65)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>) -> void>(%47, read<ptr<ptr<void>>>(%64));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %68 @use_buffer(%69 buf: ptr<i8>) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%69), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %70 @down(%71 i: i32, %72 msg: ptr<const i8>, %73 me: ptr<@type20>, %74 mes: ptr<ptr<void>> [array=10], %75 other: ptr<@type20>, %76 others: ptr<ptr<void>> [array=10]) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %77 buf: array<i8, 10000> [storage=automatic] [align=16];
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%71), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<i8>) -> void>(%68, array_decay<ptr<i8>, length=Some(10000)>(%77));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type20>, ptr<ptr<void>>, ptr<@type20>, ptr<ptr<void>>) -> void>(%62, read<ptr<@type20>>(%73), read<ptr<ptr<void>>>(%74), read<ptr<@type20>>(%75), read<ptr<ptr<void>>>(%76));
// DEFAULT-NEXT:                 call<void, signature=fn(i32, ptr<const i8>, ptr<@type20>, ptr<ptr<void>>, ptr<@type20>, ptr<ptr<void>>) -> void>(%70, sub<i32, overflow=ub>(read<i32>(%71), const<i32>(1)), read<ptr<const i8>>(%72), read<ptr<@type20>>(%73), read<ptr<ptr<void>>>(%74), read<ptr<@type20>>(%75), read<ptr<ptr<void>>>(%76));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %78 c: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %79 stack: ptr<void> [storage=automatic];
// DEFAULT-NEXT:                 let %80 stack_size: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %81 next_segment: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:                 let %82 next_sp: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:                 let %83 initial_sp: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:                 write<ptr<void>>(%79, call<ptr<void>, signature=fn(ptr<ptr<void>>, ptr<u64>, ptr<ptr<void>>, ptr<ptr<void>>, ptr<ptr<void>>) -> ptr<void>>(%55, read<ptr<ptr<void>>>(%74), addr_of<ptr<u64>>(%80), addr_of<ptr<ptr<void>>>(%81), addr_of<ptr<ptr<void>>>(%82), addr_of<ptr<ptr<void>>>(%83)));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<ptr<void>>, ptr<u64>, ptr<ptr<void>>, ptr<ptr<void>>, ptr<ptr<void>>) -> ptr<void>>(%55, read<ptr<ptr<void>>>(%74), addr_of<ptr<u64>>(%80), addr_of<ptr<ptr<void>>>(%81), addr_of<ptr<ptr<void>>>(%82), addr_of<ptr<ptr<void>>>(%83));
// DEFAULT-NEXT:                 if ne<ptr<void>>(read<ptr<void>>(%79), null<ptr<void>>)
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %143: i32 [synthetic] = read<i32>(%78);
// DEFAULT-NEXT:                         let %144: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%143), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%78, read<i32>(%144));
// DEFAULT-NEXT:                         while %139 ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, ptr<void>, ptr<u64>, ptr<ptr<void>>, ptr<ptr<void>>, ptr<ptr<void>>) -> ptr<void>>(%53, read<ptr<void>>(%81), read<ptr<void>>(%82), addr_of<ptr<u64>>(%80), addr_of<ptr<ptr<void>>>(%81), addr_of<ptr<ptr<void>>>(%82), addr_of<ptr<ptr<void>>>(%83)), null<ptr<void>>)
// DEFAULT-NEXT:                             let %145: i32 [synthetic] = read<i32>(%78);
// DEFAULT-NEXT:                             let %146: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%145), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%78, read<i32>(%146));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %84 @go1() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<const i8>, ptr<@type20>, ptr<ptr<void>>, ptr<@type20>, ptr<ptr<void>>) -> void>(%70, const<i32>(1000), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%140)), addr_of<ptr<@type20>>(%56), array_decay<ptr<ptr<void>>, length=Some(10)>(%57), addr_of<ptr<@type20>>(%58), array_decay<ptr<ptr<void>>, length=Some(10)>(%59));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%17, null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %85 @go2() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<const i8>, ptr<@type20>, ptr<ptr<void>>, ptr<@type20>, ptr<ptr<void>>) -> void>(%70, const<i32>(1000), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%141)), addr_of<ptr<@type20>>(%58), array_decay<ptr<ptr<void>>, length=Some(10)>(%59), addr_of<ptr<@type20>>(%56), array_decay<ptr<ptr<void>>, length=Some(10)>(%57));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%17, null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %87 @start_thread(%88 context: ptr<void>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %89 tc: ptr<@type22> [storage=automatic] = pointer_cast<ptr<@type22>, reason=explicit>(read<ptr<void>>(%88));
// DEFAULT-NEXT:         let %90 block: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%90, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<i32>) -> void>(%50, addr_of<ptr<i32>>(%90), null<ptr<i32>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>) -> void>(%47, read<ptr<ptr<void>>>(field1(deref(read<ptr<@type22>>(%89)))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const @type20>) -> i32>(%36, pointer_cast<ptr<const @type20>, reason=arg>(read<ptr<@type20>>(field0(deref(read<ptr<@type22>>(%89))))));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %91 @main(%92 argc: i32, %93 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %94 tid: u64 [storage=automatic];
// DEFAULT-NEXT:         let %95 err: i32 [storage=automatic];
// DEFAULT-NEXT:         let %96 size: u64 [storage=automatic];
// DEFAULT-NEXT:         let %97 tc: @type22 [storage=automatic];
// DEFAULT-NEXT:         let %98 block: i32 [storage=automatic];
// DEFAULT-NEXT:         if lt<i32>(call<i32, signature=fn(ptr<@type20>) -> i32>(%34, addr_of<ptr<@type20>>(%56)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         write<@type20>(%58, copy<@type20, reason=assign>(read<@type20>(%56)));
// DEFAULT-NEXT:         write<ptr<void>>(field0(field2(%56)), call<ptr<void>, signature=fn(u64, ptr<ptr<void>>, ptr<u64>) -> ptr<void>>(%49, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8192))), addr_of<ptr<ptr<void>>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(10)>(%57), const<i32>(0)))), addr_of<ptr<u64>>(%96)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, ptr<ptr<void>>, ptr<u64>) -> ptr<void>>(%49, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8192))), addr_of<ptr<ptr<void>>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(10)>(%57), const<i32>(0)))), addr_of<ptr<u64>>(%96));
// DEFAULT-NEXT:         if eq<ptr<void>>(read<ptr<void>>(field0(field2(%56))), null<ptr<void>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         write<i32>(field1(field2(%56)), const<i32>(0));
// DEFAULT-NEXT:         write<u64>(field2(field2(%56)), read<u64>(%96));
// DEFAULT-NEXT:         write<ptr<@type20>>(field1(%56), null<ptr<@type20>>);
// DEFAULT-NEXT:         write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>, ptr<i32>, ptr<i32>) -> void>(%52, addr_of<ptr<ptr<void>>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(10)>(%57), const<i32>(0)))), addr_of<ptr<i32>>(%98), null<ptr<i32>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type20>, ptr<fn() -> void>, i32, ...) -> void>(%43, addr_of<ptr<@type20>>(%56), function_decay<ptr<fn() -> void>>(%84), const<i32>(0));
// DEFAULT-NEXT:         write<ptr<void>>(field0(field2(%58)), call<ptr<void>, signature=fn(u64, ptr<ptr<void>>, ptr<u64>) -> ptr<void>>(%49, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8192))), addr_of<ptr<ptr<void>>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(10)>(%59), const<i32>(0)))), addr_of<ptr<u64>>(%96)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, ptr<ptr<void>>, ptr<u64>) -> ptr<void>>(%49, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8192))), addr_of<ptr<ptr<void>>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(10)>(%59), const<i32>(0)))), addr_of<ptr<u64>>(%96));
// DEFAULT-NEXT:         if eq<ptr<void>>(read<ptr<void>>(field0(field2(%58))), null<ptr<void>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         write<i32>(field1(field2(%58)), const<i32>(0));
// DEFAULT-NEXT:         write<u64>(field2(field2(%58)), read<u64>(%96));
// DEFAULT-NEXT:         write<ptr<@type20>>(field1(%58), null<ptr<@type20>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>, ptr<i32>, ptr<i32>) -> void>(%52, addr_of<ptr<ptr<void>>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(10)>(%59), const<i32>(0)))), addr_of<ptr<i32>>(%98), null<ptr<i32>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type20>, ptr<fn() -> void>, i32, ...) -> void>(%43, addr_of<ptr<@type20>>(%58), function_decay<ptr<fn() -> void>>(%85), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<i32>) -> void>(%50, addr_of<ptr<i32>>(%98), null<ptr<i32>>);
// DEFAULT-NEXT:         write<ptr<@type20>>(field0(%97), addr_of<ptr<@type20>>(%56));
// DEFAULT-NEXT:         write<ptr<ptr<void>>>(field1(%97), addr_of<ptr<ptr<void>>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(10)>(%57), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%95, call<i32, signature=fn(ptr<u64>, ptr<const @type8>, ptr<fn(ptr<void>) -> ptr<void>>, ptr<void>) -> i32>(%15, addr_of<ptr<u64>>(%94), null<ptr<const @type8>>, function_decay<ptr<fn(ptr<void>) -> ptr<void>>>(%87), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type22>>(%97))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<u64>, ptr<const @type8>, ptr<fn(ptr<void>) -> ptr<void>>, ptr<void>) -> i32>(%15, addr_of<ptr<u64>>(%94), null<ptr<const @type8>>, function_decay<ptr<fn(ptr<void>) -> ptr<void>>>(%87), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type22>>(%97)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%95), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         write<i32>(%95, call<i32, signature=fn(u64, ptr<ptr<void>>) -> i32>(%20, read<u64>(%94), null<ptr<ptr<void>>>));
// DEFAULT-NEXT:         call<i32, signature=fn(u64, ptr<ptr<void>>) -> i32>(%20, read<u64>(%94), null<ptr<ptr<void>>>);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%95), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
