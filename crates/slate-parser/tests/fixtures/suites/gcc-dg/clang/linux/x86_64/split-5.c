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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___uint16_t:[0-9]+]] __uint16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE___uint32_t:[0-9]+]] __uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___uint64_t:[0-9]+]] __uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 __val: array<u64, 16>;
// DEFAULT-NEXT:     } [size=128, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE___sigset_t:[0-9]+]] __sigset_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_sigset_t:[0-9]+]] sigset_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_pthread_t:[0-9]+]] pthread_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_pthread_attr_t:[0-9]+]] pthread_attr_t = union {
// DEFAULT-NEXT:         field0 __size: array<i8, 56>;
// DEFAULT-NEXT:         field1 __align: i64;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_pthread_attr_t_2:[0-9]+]] pthread_attr_t = @type[[TYPE_pthread_attr_t]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 ss_sp: ptr<void>;
// DEFAULT-NEXT:         field1 ss_flags: i32;
// DEFAULT-NEXT:         field2 ss_size: u64;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_stack_t:[0-9]+]] stack_t = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE_greg_t:[0-9]+]] greg_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_gregset_t:[0-9]+]] gregset_t = array<i64, 23>;
// DEFAULT-NEXT:     type @type[[TYPE__libc_fpxreg:[0-9]+]] _libc_fpxreg = struct {
// DEFAULT-NEXT:         field0 significand: array<u16, 4>;
// DEFAULT-NEXT:         field1 exponent: u16;
// DEFAULT-NEXT:         field2 __glibc_reserved1: array<u16, 3>;
// DEFAULT-NEXT:     } [size=16, align=2, offsets=[0, 8, 10]];
// DEFAULT-NEXT:     type @type[[TYPE__libc_xmmreg:[0-9]+]] _libc_xmmreg = struct {
// DEFAULT-NEXT:         field0 element: array<u32, 4>;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE__libc_fpstate:[0-9]+]] _libc_fpstate = struct {
// DEFAULT-NEXT:         field0 cwd: u16;
// DEFAULT-NEXT:         field1 swd: u16;
// DEFAULT-NEXT:         field2 ftw: u16;
// DEFAULT-NEXT:         field3 fop: u16;
// DEFAULT-NEXT:         field4 rip: u64;
// DEFAULT-NEXT:         field5 rdp: u64;
// DEFAULT-NEXT:         field6 mxcsr: u32;
// DEFAULT-NEXT:         field7 mxcr_mask: u32;
// DEFAULT-NEXT:         field8 _st: array<@type[[TYPE__libc_fpxreg]], 8>;
// DEFAULT-NEXT:         field9 _xmm: array<@type[[TYPE__libc_xmmreg]], 16>;
// DEFAULT-NEXT:         field10 __glibc_reserved1: array<u32, 24>;
// DEFAULT-NEXT:     } [size=512, align=8, offsets=[0, 2, 4, 6, 8, 16, 24, 28, 32, 160, 416]];
// DEFAULT-NEXT:     type @type[[TYPE_fpregset_t:[0-9]+]] fpregset_t = ptr<@type[[TYPE__libc_fpstate]]>;
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 gregs: array<i64, 23>;
// DEFAULT-NEXT:         field1 fpregs: ptr<@type[[TYPE__libc_fpstate]]>;
// DEFAULT-NEXT:         field2 __reserved1: array<u64, 8>;
// DEFAULT-NEXT:     } [size=256, align=8, offsets=[0, 184, 192]];
// DEFAULT-NEXT:     type @type[[TYPE_mcontext_t:[0-9]+]] mcontext_t = @type[[TYPE2]];
// DEFAULT-NEXT:     type @type[[TYPE_ucontext_t:[0-9]+]] ucontext_t = struct {
// DEFAULT-NEXT:         field0 uc_flags: u64;
// DEFAULT-NEXT:         field1 uc_link: ptr<@type[[TYPE_ucontext_t]]>;
// DEFAULT-NEXT:         field2 uc_stack: @type[[TYPE1]];
// DEFAULT-NEXT:         field3 uc_mcontext: @type[[TYPE2]];
// DEFAULT-NEXT:         field4 uc_sigmask: @type[[TYPE0]];
// DEFAULT-NEXT:         field5 __fpregs_mem: @type[[TYPE__libc_fpstate]];
// DEFAULT-NEXT:         field6 __ssp: array<u64, 4>;
// DEFAULT-NEXT:     } [size=968, align=8, offsets=[0, 8, 16, 40, 296, 424, 936]];
// DEFAULT-NEXT:     type @type[[TYPE_ucontext_t_2:[0-9]+]] ucontext_t = @type[[TYPE_ucontext_t]];
// DEFAULT-NEXT:     type @type[[TYPE_thread_context:[0-9]+]] thread_context = struct {
// DEFAULT-NEXT:         field0 u: ptr<@type[[TYPE_ucontext_t]]>;
// DEFAULT-NEXT:         field1 s: ptr<ptr<void>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_c1:[0-9]+]] c1: @type[[TYPE_ucontext_t]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s1:[0-9]+]] s1: array<ptr<void>, 10> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c2:[0-9]+]] c2: @type[[TYPE_ucontext_t]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s2:[0-9]+]] s2: array<ptr<void>, 10> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([103, 111, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([103, 111, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_pthread_create:[0-9]+]] @pthread_create(%[[VALUE___newthread:[0-9]+]] __newthread: ptr<u64> [restrict], %[[VALUE___attr:[0-9]+]] __attr: ptr<const @type[[TYPE_pthread_attr_t]]> [restrict], %[[VALUE___start_routine:[0-9]+]] __start_routine: ptr<fn(ptr<void>) -> ptr<void>>, %[[VALUE___arg:[0-9]+]] __arg: ptr<void> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_pthread_exit:[0-9]+]] @pthread_exit(%[[VALUE___retval:[0-9]+]] __retval: ptr<void>) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_pthread_join:[0-9]+]] @pthread_join(%[[VALUE___th:[0-9]+]] __th: u64, %[[VALUE___thread_return:[0-9]+]] __thread_return: ptr<ptr<void>>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_getcontext:[0-9]+]] @getcontext(%[[VALUE___ucp:[0-9]+]] __ucp: ptr<@type[[TYPE_ucontext_t]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_setcontext:[0-9]+]] @setcontext(%[[VALUE___ucp_2:[0-9]+]] __ucp: ptr<const @type[[TYPE_ucontext_t]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_swapcontext:[0-9]+]] @swapcontext(%[[VALUE___oucp:[0-9]+]] __oucp: ptr<@type[[TYPE_ucontext_t]]> [restrict], %[[VALUE___ucp_3:[0-9]+]] __ucp: ptr<const @type[[TYPE_ucontext_t]]> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_makecontext:[0-9]+]] @makecontext(%[[VALUE___ucp_4:[0-9]+]] __ucp: ptr<@type[[TYPE_ucontext_t]]>, %[[VALUE___func:[0-9]+]] __func: ptr<fn() -> void>, %[[VALUE___argc:[0-9]+]] __argc: i32, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___splitstack_getcontext:[0-9]+]] @__splitstack_getcontext(%[[VALUE_context:[0-9]+]] context: ptr<ptr<void>> [array=10]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___splitstack_setcontext:[0-9]+]] @__splitstack_setcontext(%[[VALUE_context_2:[0-9]+]] context: ptr<ptr<void>> [array=10]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___splitstack_makecontext:[0-9]+]] @__splitstack_makecontext(%[[VALUE0:[0-9]+]] <unnamed>: u64, %[[VALUE_context_3:[0-9]+]] context: ptr<ptr<void>> [array=10], %[[VALUE1:[0-9]+]] <unnamed>: ptr<u64>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___splitstack_block_signals:[0-9]+]] @__splitstack_block_signals(%[[VALUE2:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE3:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___splitstack_block_signals_context:[0-9]+]] @__splitstack_block_signals_context(%[[VALUE_context_4:[0-9]+]] context: ptr<ptr<void>> [array=10], %[[VALUE4:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE5:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___splitstack_find:[0-9]+]] @__splitstack_find(%[[VALUE6:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE7:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE8:[0-9]+]] <unnamed>: ptr<u64>, %[[VALUE9:[0-9]+]] <unnamed>: ptr<ptr<void>>, %[[VALUE10:[0-9]+]] <unnamed>: ptr<ptr<void>>, %[[VALUE11:[0-9]+]] <unnamed>: ptr<ptr<void>>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___splitstack_find_context:[0-9]+]] @__splitstack_find_context(%[[VALUE_context_5:[0-9]+]] context: ptr<ptr<void>> [array=10], %[[VALUE12:[0-9]+]] <unnamed>: ptr<u64>, %[[VALUE13:[0-9]+]] <unnamed>: ptr<ptr<void>>, %[[VALUE14:[0-9]+]] <unnamed>: ptr<ptr<void>>, %[[VALUE15:[0-9]+]] <unnamed>: ptr<ptr<void>>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_swap:[0-9]+]] @swap(%[[VALUE_fu:[0-9]+]] fu: ptr<@type[[TYPE_ucontext_t]]>, %[[VALUE_fs:[0-9]+]] fs: ptr<ptr<void>> [array=10], %[[VALUE_tu:[0-9]+]] tu: ptr<@type[[TYPE_ucontext_t]]>, %[[VALUE_ts:[0-9]+]] ts: ptr<ptr<void>> [array=10]) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>) -> void>(%[[VALUE___splitstack_getcontext]], read<ptr<ptr<void>>>(%[[VALUE_fs]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>) -> void>(%[[VALUE___splitstack_setcontext]], read<ptr<ptr<void>>>(%[[VALUE_ts]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE_ucontext_t]]>, ptr<const @type[[TYPE_ucontext_t]]>) -> i32>(%[[VALUE_swapcontext]], read<ptr<@type[[TYPE_ucontext_t]]>>(%[[VALUE_fu]]), pointer_cast<ptr<const @type[[TYPE_ucontext_t]]>, reason=arg>(read<ptr<@type[[TYPE_ucontext_t]]>>(%[[VALUE_tu]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>) -> void>(%[[VALUE___splitstack_setcontext]], read<ptr<ptr<void>>>(%[[VALUE_fs]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use_buffer:[0-9]+]] @use_buffer(%[[VALUE_buf:[0-9]+]] buf: ptr<i8>) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_buf]]), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_down:[0-9]+]] @down(%[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_msg:[0-9]+]] msg: ptr<const i8>, %[[VALUE_me:[0-9]+]] me: ptr<@type[[TYPE_ucontext_t]]>, %[[VALUE_mes:[0-9]+]] mes: ptr<ptr<void>> [array=10], %[[VALUE_other:[0-9]+]] other: ptr<@type[[TYPE_ucontext_t]]>, %[[VALUE_others:[0-9]+]] others: ptr<ptr<void>> [array=10]) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buf_2:[0-9]+]] buf: array<i8, 10000> [storage=automatic] [align=16];
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<i8>) -> void>(%[[VALUE_use_buffer]], array_decay<ptr<i8>, length=Some(10000)>(%[[VALUE_buf_2]]));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE_ucontext_t]]>, ptr<ptr<void>>, ptr<@type[[TYPE_ucontext_t]]>, ptr<ptr<void>>) -> void>(%[[VALUE_swap]], read<ptr<@type[[TYPE_ucontext_t]]>>(%[[VALUE_me]]), read<ptr<ptr<void>>>(%[[VALUE_mes]]), read<ptr<@type[[TYPE_ucontext_t]]>>(%[[VALUE_other]]), read<ptr<ptr<void>>>(%[[VALUE_others]]));
// DEFAULT-NEXT:                 call<void, signature=fn(i32, ptr<const i8>, ptr<@type[[TYPE_ucontext_t]]>, ptr<ptr<void>>, ptr<@type[[TYPE_ucontext_t]]>, ptr<ptr<void>>) -> void>(%[[VALUE_down]], sub<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(1)), read<ptr<const i8>>(%[[VALUE_msg]]), read<ptr<@type[[TYPE_ucontext_t]]>>(%[[VALUE_me]]), read<ptr<ptr<void>>>(%[[VALUE_mes]]), read<ptr<@type[[TYPE_ucontext_t]]>>(%[[VALUE_other]]), read<ptr<ptr<void>>>(%[[VALUE_others]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_stack:[0-9]+]] stack: ptr<void> [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_stack_size:[0-9]+]] stack_size: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_next_segment:[0-9]+]] next_segment: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:                 let %[[VALUE_next_sp:[0-9]+]] next_sp: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:                 let %[[VALUE_initial_sp:[0-9]+]] initial_sp: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:                 write<ptr<void>>(%[[VALUE_stack]], call<ptr<void>, signature=fn(ptr<ptr<void>>, ptr<u64>, ptr<ptr<void>>, ptr<ptr<void>>, ptr<ptr<void>>) -> ptr<void>>(%[[VALUE___splitstack_find_context]], read<ptr<ptr<void>>>(%[[VALUE_mes]]), addr_of<ptr<u64>>(%[[VALUE_stack_size]]), addr_of<ptr<ptr<void>>>(%[[VALUE_next_segment]]), addr_of<ptr<ptr<void>>>(%[[VALUE_next_sp]]), addr_of<ptr<ptr<void>>>(%[[VALUE_initial_sp]])));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<ptr<void>>, ptr<u64>, ptr<ptr<void>>, ptr<ptr<void>>, ptr<ptr<void>>) -> ptr<void>>(%[[VALUE___splitstack_find_context]], read<ptr<ptr<void>>>(%[[VALUE_mes]]), addr_of<ptr<u64>>(%[[VALUE_stack_size]]), addr_of<ptr<ptr<void>>>(%[[VALUE_next_segment]]), addr_of<ptr<ptr<void>>>(%[[VALUE_next_sp]]), addr_of<ptr<ptr<void>>>(%[[VALUE_initial_sp]]));
// DEFAULT-NEXT:                 if ne<ptr<void>>(read<ptr<void>>(%[[VALUE_stack]]), null<ptr<void>>)
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:                         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:                         while %[[VALUE18:[0-9]+]] ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, ptr<void>, ptr<u64>, ptr<ptr<void>>, ptr<ptr<void>>, ptr<ptr<void>>) -> ptr<void>>(%[[VALUE___splitstack_find]], read<ptr<void>>(%[[VALUE_next_segment]]), read<ptr<void>>(%[[VALUE_next_sp]]), addr_of<ptr<u64>>(%[[VALUE_stack_size]]), addr_of<ptr<ptr<void>>>(%[[VALUE_next_segment]]), addr_of<ptr<ptr<void>>>(%[[VALUE_next_sp]]), addr_of<ptr<ptr<void>>>(%[[VALUE_initial_sp]])), null<ptr<void>>)
// DEFAULT-NEXT:                             let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:                             let %[[VALUE20:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE19]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE20]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_go1:[0-9]+]] @go1() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<const i8>, ptr<@type[[TYPE_ucontext_t]]>, ptr<ptr<void>>, ptr<@type[[TYPE_ucontext_t]]>, ptr<ptr<void>>) -> void>(%[[VALUE_down]], const<i32>(1000), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), addr_of<ptr<@type[[TYPE_ucontext_t]]>>(%[[VALUE_c1]]), array_decay<ptr<ptr<void>>, length=Some(10)>(%[[VALUE_s1]]), addr_of<ptr<@type[[TYPE_ucontext_t]]>>(%[[VALUE_c2]]), array_decay<ptr<ptr<void>>, length=Some(10)>(%[[VALUE_s2]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_pthread_exit]], null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_go2:[0-9]+]] @go2() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<const i8>, ptr<@type[[TYPE_ucontext_t]]>, ptr<ptr<void>>, ptr<@type[[TYPE_ucontext_t]]>, ptr<ptr<void>>) -> void>(%[[VALUE_down]], const<i32>(1000), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), addr_of<ptr<@type[[TYPE_ucontext_t]]>>(%[[VALUE_c2]]), array_decay<ptr<ptr<void>>, length=Some(10)>(%[[VALUE_s2]]), addr_of<ptr<@type[[TYPE_ucontext_t]]>>(%[[VALUE_c1]]), array_decay<ptr<ptr<void>>, length=Some(10)>(%[[VALUE_s1]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_pthread_exit]], null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_start_thread:[0-9]+]] @start_thread(%[[VALUE_context_6:[0-9]+]] context: ptr<void>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_tc:[0-9]+]] tc: ptr<@type[[TYPE_thread_context]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_thread_context]]>, reason=explicit>(read<ptr<void>>(%[[VALUE_context_6]]));
// DEFAULT-NEXT:         let %[[VALUE_block:[0-9]+]] block: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_block]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<i32>) -> void>(%[[VALUE___splitstack_block_signals]], addr_of<ptr<i32>>(%[[VALUE_block]]), null<ptr<i32>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>) -> void>(%[[VALUE___splitstack_setcontext]], read<ptr<ptr<void>>>(field1(deref(read<ptr<@type[[TYPE_thread_context]]>>(%[[VALUE_tc]])))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const @type[[TYPE_ucontext_t]]>) -> i32>(%[[VALUE_setcontext]], pointer_cast<ptr<const @type[[TYPE_ucontext_t]]>, reason=arg>(read<ptr<@type[[TYPE_ucontext_t]]>>(field0(deref(read<ptr<@type[[TYPE_thread_context]]>>(%[[VALUE_tc]]))))));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_tid:[0-9]+]] tid: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_err:[0-9]+]] err: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_size:[0-9]+]] size: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_tc_2:[0-9]+]] tc: @type[[TYPE_thread_context]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_block_2:[0-9]+]] block: i32 [storage=automatic];
// DEFAULT-NEXT:         if lt<i32>(call<i32, signature=fn(ptr<@type[[TYPE_ucontext_t]]>) -> i32>(%[[VALUE_getcontext]], addr_of<ptr<@type[[TYPE_ucontext_t]]>>(%[[VALUE_c1]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_ucontext_t]]>(%[[VALUE_c2]], copy<@type[[TYPE_ucontext_t]], reason=assign>(read<@type[[TYPE_ucontext_t]]>(%[[VALUE_c1]])));
// DEFAULT-NEXT:         write<ptr<void>>(field0(field2(%[[VALUE_c1]])), call<ptr<void>, signature=fn(u64, ptr<ptr<void>>, ptr<u64>) -> ptr<void>>(%[[VALUE___splitstack_makecontext]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8192))), addr_of<ptr<ptr<void>>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(10)>(%[[VALUE_s1]]), const<i32>(0)))), addr_of<ptr<u64>>(%[[VALUE_size]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, ptr<ptr<void>>, ptr<u64>) -> ptr<void>>(%[[VALUE___splitstack_makecontext]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8192))), addr_of<ptr<ptr<void>>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(10)>(%[[VALUE_s1]]), const<i32>(0)))), addr_of<ptr<u64>>(%[[VALUE_size]]));
// DEFAULT-NEXT:         if eq<ptr<void>>(read<ptr<void>>(field0(field2(%[[VALUE_c1]]))), null<ptr<void>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(field1(field2(%[[VALUE_c1]])), const<i32>(0));
// DEFAULT-NEXT:         write<u64>(field2(field2(%[[VALUE_c1]])), read<u64>(%[[VALUE_size]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_ucontext_t]]>>(field1(%[[VALUE_c1]]), null<ptr<@type[[TYPE_ucontext_t]]>>);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_block_2]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>, ptr<i32>, ptr<i32>) -> void>(%[[VALUE___splitstack_block_signals_context]], addr_of<ptr<ptr<void>>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(10)>(%[[VALUE_s1]]), const<i32>(0)))), addr_of<ptr<i32>>(%[[VALUE_block_2]]), null<ptr<i32>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_ucontext_t]]>, ptr<fn() -> void>, i32, ...) -> void>(%[[VALUE_makecontext]], addr_of<ptr<@type[[TYPE_ucontext_t]]>>(%[[VALUE_c1]]), function_decay<ptr<fn() -> void>>(%[[VALUE_go1]]), const<i32>(0));
// DEFAULT-NEXT:         write<ptr<void>>(field0(field2(%[[VALUE_c2]])), call<ptr<void>, signature=fn(u64, ptr<ptr<void>>, ptr<u64>) -> ptr<void>>(%[[VALUE___splitstack_makecontext]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8192))), addr_of<ptr<ptr<void>>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(10)>(%[[VALUE_s2]]), const<i32>(0)))), addr_of<ptr<u64>>(%[[VALUE_size]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, ptr<ptr<void>>, ptr<u64>) -> ptr<void>>(%[[VALUE___splitstack_makecontext]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8192))), addr_of<ptr<ptr<void>>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(10)>(%[[VALUE_s2]]), const<i32>(0)))), addr_of<ptr<u64>>(%[[VALUE_size]]));
// DEFAULT-NEXT:         if eq<ptr<void>>(read<ptr<void>>(field0(field2(%[[VALUE_c2]]))), null<ptr<void>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(field1(field2(%[[VALUE_c2]])), const<i32>(0));
// DEFAULT-NEXT:         write<u64>(field2(field2(%[[VALUE_c2]])), read<u64>(%[[VALUE_size]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_ucontext_t]]>>(field1(%[[VALUE_c2]]), null<ptr<@type[[TYPE_ucontext_t]]>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>, ptr<i32>, ptr<i32>) -> void>(%[[VALUE___splitstack_block_signals_context]], addr_of<ptr<ptr<void>>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(10)>(%[[VALUE_s2]]), const<i32>(0)))), addr_of<ptr<i32>>(%[[VALUE_block_2]]), null<ptr<i32>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_ucontext_t]]>, ptr<fn() -> void>, i32, ...) -> void>(%[[VALUE_makecontext]], addr_of<ptr<@type[[TYPE_ucontext_t]]>>(%[[VALUE_c2]]), function_decay<ptr<fn() -> void>>(%[[VALUE_go2]]), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_block_2]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<i32>) -> void>(%[[VALUE___splitstack_block_signals]], addr_of<ptr<i32>>(%[[VALUE_block_2]]), null<ptr<i32>>);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_ucontext_t]]>>(field0(%[[VALUE_tc_2]]), addr_of<ptr<@type[[TYPE_ucontext_t]]>>(%[[VALUE_c1]]));
// DEFAULT-NEXT:         write<ptr<ptr<void>>>(field1(%[[VALUE_tc_2]]), addr_of<ptr<ptr<void>>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(10)>(%[[VALUE_s1]]), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_err]], call<i32, signature=fn(ptr<u64>, ptr<const @type[[TYPE_pthread_attr_t]]>, ptr<fn(ptr<void>) -> ptr<void>>, ptr<void>) -> i32>(%[[VALUE_pthread_create]], addr_of<ptr<u64>>(%[[VALUE_tid]]), null<ptr<const @type[[TYPE_pthread_attr_t]]>>, function_decay<ptr<fn(ptr<void>) -> ptr<void>>>(%[[VALUE_start_thread]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_thread_context]]>>(%[[VALUE_tc_2]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<u64>, ptr<const @type[[TYPE_pthread_attr_t]]>, ptr<fn(ptr<void>) -> ptr<void>>, ptr<void>) -> i32>(%[[VALUE_pthread_create]], addr_of<ptr<u64>>(%[[VALUE_tid]]), null<ptr<const @type[[TYPE_pthread_attr_t]]>>, function_decay<ptr<fn(ptr<void>) -> ptr<void>>>(%[[VALUE_start_thread]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_thread_context]]>>(%[[VALUE_tc_2]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_err]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_err]], call<i32, signature=fn(u64, ptr<ptr<void>>) -> i32>(%[[VALUE_pthread_join]], read<u64>(%[[VALUE_tid]]), null<ptr<ptr<void>>>));
// DEFAULT-NEXT:         call<i32, signature=fn(u64, ptr<ptr<void>>) -> i32>(%[[VALUE_pthread_join]], read<u64>(%[[VALUE_tid]]), null<ptr<ptr<void>>>);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_err]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
