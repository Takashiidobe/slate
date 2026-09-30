#define _GNU_SOURCE
#define obstack_chunk_alloc malloc
#define obstack_chunk_free  free
#include <malloc.h>
#include <obstack.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int gnu_allocation_extensions(void) {
  int  *values    = reallocarray(NULL, 4, sizeof(*values));
  void *aligned   = memalign(64, 80);
  void *page      = valloc(1);
  void *rounded   = pvalloc(1);
  long  page_size = sysconf(_SC_PAGESIZE);
  int   total     = 0;

  for (int index = 0; index < 4; ++index) {
    values[index] = index + 1;
  }
  total += values[0] + values[1] + values[2] + values[3];
  total += malloc_usable_size(values) >= 4 * sizeof(*values);
  total += aligned != NULL && (uintptr_t)aligned % 64 == 0;
  total += page != NULL && (uintptr_t)page % (uintptr_t)page_size == 0;
  total += rounded != NULL && (uintptr_t)rounded % (uintptr_t)page_size == 0;
  total += malloc_usable_size(rounded) >= (size_t)page_size;
  total += mallopt(M_CHECK_ACTION, 1) != 0;

  free(values);
  free(aligned);
  free(page);
  free(rounded);
  return total;
}

static int gnu_obstack_extensions(void) {
  struct obstack storage;
  char          *first;
  char          *second;
  int            total = 0;

  obstack_init(&storage);
  first   = obstack_copy0(&storage, "gnu", 3);
  second  = obstack_copy0(&storage, "libc", 4);
  total  += strcmp(first, "gnu") == 0;
  total  += strcmp(second, "libc") == 0;
  total  += obstack_object_size(&storage) == 0;
  obstack_free(&storage, NULL);
  return total;
}

int main(void) {
  printf("%d %d\n", gnu_allocation_extensions(), gnu_obstack_extensions());
  return 0;
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_obstack:[0-9]+]] obstack = struct {
// DEFAULT-NEXT:         field0 chunk_size: i64;
// DEFAULT-NEXT:         field1 chunk: ptr<@type[[TYPE__obstack_chunk:[0-9]+]]>;
// DEFAULT-NEXT:         field2 object_base: ptr<i8>;
// DEFAULT-NEXT:         field3 next_free: ptr<i8>;
// DEFAULT-NEXT:         field4 chunk_limit: ptr<i8>;
// DEFAULT-NEXT:         field5 temp: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:         field6 alignment_mask: i32;
// DEFAULT-NEXT:         field7 chunkfun: ptr<fn(ptr<void>, i64) -> ptr<@type[[TYPE__obstack_chunk]]>>;
// DEFAULT-NEXT:         field8 freefun: ptr<fn(ptr<void>, ptr<@type[[TYPE__obstack_chunk]]>) -> void>;
// DEFAULT-NEXT:         field9 extra_arg: ptr<void>;
// DEFAULT-NEXT:         field10 use_extra_arg: u32 : 1;
// DEFAULT-NEXT:         field11 maybe_empty_object: u32 : 1;
// DEFAULT-NEXT:         field12 alloc_failed: u32 : 1;
// DEFAULT-NEXT:     } [size=88, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 80, 80], bit_offsets=[None, None, None, None, None, None, None, None, None, None, Some(640), Some(641), Some(642)], bit_units=[(80, 1)], field_units=[None, None, None, None, None, None, None, None, None, None, Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE__obstack_chunk]] _obstack_chunk = struct {
// DEFAULT-NEXT:         field0 limit: ptr<i8>;
// DEFAULT-NEXT:         field1 prev: ptr<@type[[TYPE__obstack_chunk]]>;
// DEFAULT-NEXT:         field2 contents: array<i8, 4>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = union {
// DEFAULT-NEXT:         field0 tempint: i64;
// DEFAULT-NEXT:         field1 tempptr: ptr<void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_uintptr_t:[0-9]+]] uintptr_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE__SC_ARG_MAX:[0-9]+]] _SC_ARG_MAX = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE__SC_CHILD_MAX:[0-9]+]] _SC_CHILD_MAX = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE__SC_CLK_TCK:[0-9]+]] _SC_CLK_TCK = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE__SC_NGROUPS_MAX:[0-9]+]] _SC_NGROUPS_MAX = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE__SC_OPEN_MAX:[0-9]+]] _SC_OPEN_MAX = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE__SC_STREAM_MAX:[0-9]+]] _SC_STREAM_MAX = const<i32>(5);
// DEFAULT-NEXT:         %[[VALUE__SC_TZNAME_MAX:[0-9]+]] _SC_TZNAME_MAX = const<i32>(6);
// DEFAULT-NEXT:         %[[VALUE__SC_JOB_CONTROL:[0-9]+]] _SC_JOB_CONTROL = const<i32>(7);
// DEFAULT-NEXT:         %[[VALUE__SC_SAVED_IDS:[0-9]+]] _SC_SAVED_IDS = const<i32>(8);
// DEFAULT-NEXT:         %[[VALUE__SC_REALTIME_SIGNALS:[0-9]+]] _SC_REALTIME_SIGNALS = const<i32>(9);
// DEFAULT-NEXT:         %[[VALUE__SC_PRIORITY_SCHEDULING:[0-9]+]] _SC_PRIORITY_SCHEDULING = const<i32>(10);
// DEFAULT-NEXT:         %[[VALUE__SC_TIMERS:[0-9]+]] _SC_TIMERS = const<i32>(11);
// DEFAULT-NEXT:         %[[VALUE__SC_ASYNCHRONOUS_IO:[0-9]+]] _SC_ASYNCHRONOUS_IO = const<i32>(12);
// DEFAULT-NEXT:         %[[VALUE__SC_PRIORITIZED_IO:[0-9]+]] _SC_PRIORITIZED_IO = const<i32>(13);
// DEFAULT-NEXT:         %[[VALUE__SC_SYNCHRONIZED_IO:[0-9]+]] _SC_SYNCHRONIZED_IO = const<i32>(14);
// DEFAULT-NEXT:         %[[VALUE__SC_FSYNC:[0-9]+]] _SC_FSYNC = const<i32>(15);
// DEFAULT-NEXT:         %[[VALUE__SC_MAPPED_FILES:[0-9]+]] _SC_MAPPED_FILES = const<i32>(16);
// DEFAULT-NEXT:         %[[VALUE__SC_MEMLOCK:[0-9]+]] _SC_MEMLOCK = const<i32>(17);
// DEFAULT-NEXT:         %[[VALUE__SC_MEMLOCK_RANGE:[0-9]+]] _SC_MEMLOCK_RANGE = const<i32>(18);
// DEFAULT-NEXT:         %[[VALUE__SC_MEMORY_PROTECTION:[0-9]+]] _SC_MEMORY_PROTECTION = const<i32>(19);
// DEFAULT-NEXT:         %[[VALUE__SC_MESSAGE_PASSING:[0-9]+]] _SC_MESSAGE_PASSING = const<i32>(20);
// DEFAULT-NEXT:         %[[VALUE__SC_SEMAPHORES:[0-9]+]] _SC_SEMAPHORES = const<i32>(21);
// DEFAULT-NEXT:         %[[VALUE__SC_SHARED_MEMORY_OBJECTS:[0-9]+]] _SC_SHARED_MEMORY_OBJECTS = const<i32>(22);
// DEFAULT-NEXT:         %[[VALUE__SC_AIO_LISTIO_MAX:[0-9]+]] _SC_AIO_LISTIO_MAX = const<i32>(23);
// DEFAULT-NEXT:         %[[VALUE__SC_AIO_MAX:[0-9]+]] _SC_AIO_MAX = const<i32>(24);
// DEFAULT-NEXT:         %[[VALUE__SC_AIO_PRIO_DELTA_MAX:[0-9]+]] _SC_AIO_PRIO_DELTA_MAX = const<i32>(25);
// DEFAULT-NEXT:         %[[VALUE__SC_DELAYTIMER_MAX:[0-9]+]] _SC_DELAYTIMER_MAX = const<i32>(26);
// DEFAULT-NEXT:         %[[VALUE__SC_MQ_OPEN_MAX:[0-9]+]] _SC_MQ_OPEN_MAX = const<i32>(27);
// DEFAULT-NEXT:         %[[VALUE__SC_MQ_PRIO_MAX:[0-9]+]] _SC_MQ_PRIO_MAX = const<i32>(28);
// DEFAULT-NEXT:         %[[VALUE__SC_VERSION:[0-9]+]] _SC_VERSION = const<i32>(29);
// DEFAULT-NEXT:         %[[VALUE__SC_PAGESIZE:[0-9]+]] _SC_PAGESIZE = const<i32>(30);
// DEFAULT-NEXT:         %[[VALUE__SC_RTSIG_MAX:[0-9]+]] _SC_RTSIG_MAX = const<i32>(31);
// DEFAULT-NEXT:         %[[VALUE__SC_SEM_NSEMS_MAX:[0-9]+]] _SC_SEM_NSEMS_MAX = const<i32>(32);
// DEFAULT-NEXT:         %[[VALUE__SC_SEM_VALUE_MAX:[0-9]+]] _SC_SEM_VALUE_MAX = const<i32>(33);
// DEFAULT-NEXT:         %[[VALUE__SC_SIGQUEUE_MAX:[0-9]+]] _SC_SIGQUEUE_MAX = const<i32>(34);
// DEFAULT-NEXT:         %[[VALUE__SC_TIMER_MAX:[0-9]+]] _SC_TIMER_MAX = const<i32>(35);
// DEFAULT-NEXT:         %[[VALUE__SC_BC_BASE_MAX:[0-9]+]] _SC_BC_BASE_MAX = const<i32>(36);
// DEFAULT-NEXT:         %[[VALUE__SC_BC_DIM_MAX:[0-9]+]] _SC_BC_DIM_MAX = const<i32>(37);
// DEFAULT-NEXT:         %[[VALUE__SC_BC_SCALE_MAX:[0-9]+]] _SC_BC_SCALE_MAX = const<i32>(38);
// DEFAULT-NEXT:         %[[VALUE__SC_BC_STRING_MAX:[0-9]+]] _SC_BC_STRING_MAX = const<i32>(39);
// DEFAULT-NEXT:         %[[VALUE__SC_COLL_WEIGHTS_MAX:[0-9]+]] _SC_COLL_WEIGHTS_MAX = const<i32>(40);
// DEFAULT-NEXT:         %[[VALUE__SC_EQUIV_CLASS_MAX:[0-9]+]] _SC_EQUIV_CLASS_MAX = const<i32>(41);
// DEFAULT-NEXT:         %[[VALUE__SC_EXPR_NEST_MAX:[0-9]+]] _SC_EXPR_NEST_MAX = const<i32>(42);
// DEFAULT-NEXT:         %[[VALUE__SC_LINE_MAX:[0-9]+]] _SC_LINE_MAX = const<i32>(43);
// DEFAULT-NEXT:         %[[VALUE__SC_RE_DUP_MAX:[0-9]+]] _SC_RE_DUP_MAX = const<i32>(44);
// DEFAULT-NEXT:         %[[VALUE__SC_CHARCLASS_NAME_MAX:[0-9]+]] _SC_CHARCLASS_NAME_MAX = const<i32>(45);
// DEFAULT-NEXT:         %[[VALUE__SC_2_VERSION:[0-9]+]] _SC_2_VERSION = const<i32>(46);
// DEFAULT-NEXT:         %[[VALUE__SC_2_C_BIND:[0-9]+]] _SC_2_C_BIND = const<i32>(47);
// DEFAULT-NEXT:         %[[VALUE__SC_2_C_DEV:[0-9]+]] _SC_2_C_DEV = const<i32>(48);
// DEFAULT-NEXT:         %[[VALUE__SC_2_FORT_DEV:[0-9]+]] _SC_2_FORT_DEV = const<i32>(49);
// DEFAULT-NEXT:         %[[VALUE__SC_2_FORT_RUN:[0-9]+]] _SC_2_FORT_RUN = const<i32>(50);
// DEFAULT-NEXT:         %[[VALUE__SC_2_SW_DEV:[0-9]+]] _SC_2_SW_DEV = const<i32>(51);
// DEFAULT-NEXT:         %[[VALUE__SC_2_LOCALEDEF:[0-9]+]] _SC_2_LOCALEDEF = const<i32>(52);
// DEFAULT-NEXT:         %[[VALUE__SC_PII:[0-9]+]] _SC_PII = const<i32>(53);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_XTI:[0-9]+]] _SC_PII_XTI = const<i32>(54);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_SOCKET:[0-9]+]] _SC_PII_SOCKET = const<i32>(55);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_INTERNET:[0-9]+]] _SC_PII_INTERNET = const<i32>(56);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_OSI:[0-9]+]] _SC_PII_OSI = const<i32>(57);
// DEFAULT-NEXT:         %[[VALUE__SC_POLL:[0-9]+]] _SC_POLL = const<i32>(58);
// DEFAULT-NEXT:         %[[VALUE__SC_SELECT:[0-9]+]] _SC_SELECT = const<i32>(59);
// DEFAULT-NEXT:         %[[VALUE__SC_UIO_MAXIOV:[0-9]+]] _SC_UIO_MAXIOV = const<i32>(60);
// DEFAULT-NEXT:         %[[VALUE__SC_IOV_MAX:[0-9]+]] _SC_IOV_MAX = const<i32>(60);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_INTERNET_STREAM:[0-9]+]] _SC_PII_INTERNET_STREAM = const<i32>(61);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_INTERNET_DGRAM:[0-9]+]] _SC_PII_INTERNET_DGRAM = const<i32>(62);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_OSI_COTS:[0-9]+]] _SC_PII_OSI_COTS = const<i32>(63);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_OSI_CLTS:[0-9]+]] _SC_PII_OSI_CLTS = const<i32>(64);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_OSI_M:[0-9]+]] _SC_PII_OSI_M = const<i32>(65);
// DEFAULT-NEXT:         %[[VALUE__SC_T_IOV_MAX:[0-9]+]] _SC_T_IOV_MAX = const<i32>(66);
// DEFAULT-NEXT:         %[[VALUE__SC_THREADS:[0-9]+]] _SC_THREADS = const<i32>(67);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_SAFE_FUNCTIONS:[0-9]+]] _SC_THREAD_SAFE_FUNCTIONS = const<i32>(68);
// DEFAULT-NEXT:         %[[VALUE__SC_GETGR_R_SIZE_MAX:[0-9]+]] _SC_GETGR_R_SIZE_MAX = const<i32>(69);
// DEFAULT-NEXT:         %[[VALUE__SC_GETPW_R_SIZE_MAX:[0-9]+]] _SC_GETPW_R_SIZE_MAX = const<i32>(70);
// DEFAULT-NEXT:         %[[VALUE__SC_LOGIN_NAME_MAX:[0-9]+]] _SC_LOGIN_NAME_MAX = const<i32>(71);
// DEFAULT-NEXT:         %[[VALUE__SC_TTY_NAME_MAX:[0-9]+]] _SC_TTY_NAME_MAX = const<i32>(72);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_DESTRUCTOR_ITERATIONS:[0-9]+]] _SC_THREAD_DESTRUCTOR_ITERATIONS = const<i32>(73);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_KEYS_MAX:[0-9]+]] _SC_THREAD_KEYS_MAX = const<i32>(74);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_STACK_MIN:[0-9]+]] _SC_THREAD_STACK_MIN = const<i32>(75);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_THREADS_MAX:[0-9]+]] _SC_THREAD_THREADS_MAX = const<i32>(76);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_ATTR_STACKADDR:[0-9]+]] _SC_THREAD_ATTR_STACKADDR = const<i32>(77);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_ATTR_STACKSIZE:[0-9]+]] _SC_THREAD_ATTR_STACKSIZE = const<i32>(78);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_PRIORITY_SCHEDULING:[0-9]+]] _SC_THREAD_PRIORITY_SCHEDULING = const<i32>(79);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_PRIO_INHERIT:[0-9]+]] _SC_THREAD_PRIO_INHERIT = const<i32>(80);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_PRIO_PROTECT:[0-9]+]] _SC_THREAD_PRIO_PROTECT = const<i32>(81);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_PROCESS_SHARED:[0-9]+]] _SC_THREAD_PROCESS_SHARED = const<i32>(82);
// DEFAULT-NEXT:         %[[VALUE__SC_NPROCESSORS_CONF:[0-9]+]] _SC_NPROCESSORS_CONF = const<i32>(83);
// DEFAULT-NEXT:         %[[VALUE__SC_NPROCESSORS_ONLN:[0-9]+]] _SC_NPROCESSORS_ONLN = const<i32>(84);
// DEFAULT-NEXT:         %[[VALUE__SC_PHYS_PAGES:[0-9]+]] _SC_PHYS_PAGES = const<i32>(85);
// DEFAULT-NEXT:         %[[VALUE__SC_AVPHYS_PAGES:[0-9]+]] _SC_AVPHYS_PAGES = const<i32>(86);
// DEFAULT-NEXT:         %[[VALUE__SC_ATEXIT_MAX:[0-9]+]] _SC_ATEXIT_MAX = const<i32>(87);
// DEFAULT-NEXT:         %[[VALUE__SC_PASS_MAX:[0-9]+]] _SC_PASS_MAX = const<i32>(88);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_VERSION:[0-9]+]] _SC_XOPEN_VERSION = const<i32>(89);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_XCU_VERSION:[0-9]+]] _SC_XOPEN_XCU_VERSION = const<i32>(90);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_UNIX:[0-9]+]] _SC_XOPEN_UNIX = const<i32>(91);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_CRYPT:[0-9]+]] _SC_XOPEN_CRYPT = const<i32>(92);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_ENH_I18N:[0-9]+]] _SC_XOPEN_ENH_I18N = const<i32>(93);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_SHM:[0-9]+]] _SC_XOPEN_SHM = const<i32>(94);
// DEFAULT-NEXT:         %[[VALUE__SC_2_CHAR_TERM:[0-9]+]] _SC_2_CHAR_TERM = const<i32>(95);
// DEFAULT-NEXT:         %[[VALUE__SC_2_C_VERSION:[0-9]+]] _SC_2_C_VERSION = const<i32>(96);
// DEFAULT-NEXT:         %[[VALUE__SC_2_UPE:[0-9]+]] _SC_2_UPE = const<i32>(97);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_XPG2:[0-9]+]] _SC_XOPEN_XPG2 = const<i32>(98);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_XPG3:[0-9]+]] _SC_XOPEN_XPG3 = const<i32>(99);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_XPG4:[0-9]+]] _SC_XOPEN_XPG4 = const<i32>(100);
// DEFAULT-NEXT:         %[[VALUE__SC_CHAR_BIT:[0-9]+]] _SC_CHAR_BIT = const<i32>(101);
// DEFAULT-NEXT:         %[[VALUE__SC_CHAR_MAX:[0-9]+]] _SC_CHAR_MAX = const<i32>(102);
// DEFAULT-NEXT:         %[[VALUE__SC_CHAR_MIN:[0-9]+]] _SC_CHAR_MIN = const<i32>(103);
// DEFAULT-NEXT:         %[[VALUE__SC_INT_MAX:[0-9]+]] _SC_INT_MAX = const<i32>(104);
// DEFAULT-NEXT:         %[[VALUE__SC_INT_MIN:[0-9]+]] _SC_INT_MIN = const<i32>(105);
// DEFAULT-NEXT:         %[[VALUE__SC_LONG_BIT:[0-9]+]] _SC_LONG_BIT = const<i32>(106);
// DEFAULT-NEXT:         %[[VALUE__SC_WORD_BIT:[0-9]+]] _SC_WORD_BIT = const<i32>(107);
// DEFAULT-NEXT:         %[[VALUE__SC_MB_LEN_MAX:[0-9]+]] _SC_MB_LEN_MAX = const<i32>(108);
// DEFAULT-NEXT:         %[[VALUE__SC_NZERO:[0-9]+]] _SC_NZERO = const<i32>(109);
// DEFAULT-NEXT:         %[[VALUE__SC_SSIZE_MAX:[0-9]+]] _SC_SSIZE_MAX = const<i32>(110);
// DEFAULT-NEXT:         %[[VALUE__SC_SCHAR_MAX:[0-9]+]] _SC_SCHAR_MAX = const<i32>(111);
// DEFAULT-NEXT:         %[[VALUE__SC_SCHAR_MIN:[0-9]+]] _SC_SCHAR_MIN = const<i32>(112);
// DEFAULT-NEXT:         %[[VALUE__SC_SHRT_MAX:[0-9]+]] _SC_SHRT_MAX = const<i32>(113);
// DEFAULT-NEXT:         %[[VALUE__SC_SHRT_MIN:[0-9]+]] _SC_SHRT_MIN = const<i32>(114);
// DEFAULT-NEXT:         %[[VALUE__SC_UCHAR_MAX:[0-9]+]] _SC_UCHAR_MAX = const<i32>(115);
// DEFAULT-NEXT:         %[[VALUE__SC_UINT_MAX:[0-9]+]] _SC_UINT_MAX = const<i32>(116);
// DEFAULT-NEXT:         %[[VALUE__SC_ULONG_MAX:[0-9]+]] _SC_ULONG_MAX = const<i32>(117);
// DEFAULT-NEXT:         %[[VALUE__SC_USHRT_MAX:[0-9]+]] _SC_USHRT_MAX = const<i32>(118);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_ARGMAX:[0-9]+]] _SC_NL_ARGMAX = const<i32>(119);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_LANGMAX:[0-9]+]] _SC_NL_LANGMAX = const<i32>(120);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_MSGMAX:[0-9]+]] _SC_NL_MSGMAX = const<i32>(121);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_NMAX:[0-9]+]] _SC_NL_NMAX = const<i32>(122);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_SETMAX:[0-9]+]] _SC_NL_SETMAX = const<i32>(123);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_TEXTMAX:[0-9]+]] _SC_NL_TEXTMAX = const<i32>(124);
// DEFAULT-NEXT:         %[[VALUE__SC_XBS5_ILP32_OFF32:[0-9]+]] _SC_XBS5_ILP32_OFF32 = const<i32>(125);
// DEFAULT-NEXT:         %[[VALUE__SC_XBS5_ILP32_OFFBIG:[0-9]+]] _SC_XBS5_ILP32_OFFBIG = const<i32>(126);
// DEFAULT-NEXT:         %[[VALUE__SC_XBS5_LP64_OFF64:[0-9]+]] _SC_XBS5_LP64_OFF64 = const<i32>(127);
// DEFAULT-NEXT:         %[[VALUE__SC_XBS5_LPBIG_OFFBIG:[0-9]+]] _SC_XBS5_LPBIG_OFFBIG = const<i32>(128);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_LEGACY:[0-9]+]] _SC_XOPEN_LEGACY = const<i32>(129);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_REALTIME:[0-9]+]] _SC_XOPEN_REALTIME = const<i32>(130);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_REALTIME_THREADS:[0-9]+]] _SC_XOPEN_REALTIME_THREADS = const<i32>(131);
// DEFAULT-NEXT:         %[[VALUE__SC_ADVISORY_INFO:[0-9]+]] _SC_ADVISORY_INFO = const<i32>(132);
// DEFAULT-NEXT:         %[[VALUE__SC_BARRIERS:[0-9]+]] _SC_BARRIERS = const<i32>(133);
// DEFAULT-NEXT:         %[[VALUE__SC_BASE:[0-9]+]] _SC_BASE = const<i32>(134);
// DEFAULT-NEXT:         %[[VALUE__SC_C_LANG_SUPPORT:[0-9]+]] _SC_C_LANG_SUPPORT = const<i32>(135);
// DEFAULT-NEXT:         %[[VALUE__SC_C_LANG_SUPPORT_R:[0-9]+]] _SC_C_LANG_SUPPORT_R = const<i32>(136);
// DEFAULT-NEXT:         %[[VALUE__SC_CLOCK_SELECTION:[0-9]+]] _SC_CLOCK_SELECTION = const<i32>(137);
// DEFAULT-NEXT:         %[[VALUE__SC_CPUTIME:[0-9]+]] _SC_CPUTIME = const<i32>(138);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_CPUTIME:[0-9]+]] _SC_THREAD_CPUTIME = const<i32>(139);
// DEFAULT-NEXT:         %[[VALUE__SC_DEVICE_IO:[0-9]+]] _SC_DEVICE_IO = const<i32>(140);
// DEFAULT-NEXT:         %[[VALUE__SC_DEVICE_SPECIFIC:[0-9]+]] _SC_DEVICE_SPECIFIC = const<i32>(141);
// DEFAULT-NEXT:         %[[VALUE__SC_DEVICE_SPECIFIC_R:[0-9]+]] _SC_DEVICE_SPECIFIC_R = const<i32>(142);
// DEFAULT-NEXT:         %[[VALUE__SC_FD_MGMT:[0-9]+]] _SC_FD_MGMT = const<i32>(143);
// DEFAULT-NEXT:         %[[VALUE__SC_FIFO:[0-9]+]] _SC_FIFO = const<i32>(144);
// DEFAULT-NEXT:         %[[VALUE__SC_PIPE:[0-9]+]] _SC_PIPE = const<i32>(145);
// DEFAULT-NEXT:         %[[VALUE__SC_FILE_ATTRIBUTES:[0-9]+]] _SC_FILE_ATTRIBUTES = const<i32>(146);
// DEFAULT-NEXT:         %[[VALUE__SC_FILE_LOCKING:[0-9]+]] _SC_FILE_LOCKING = const<i32>(147);
// DEFAULT-NEXT:         %[[VALUE__SC_FILE_SYSTEM:[0-9]+]] _SC_FILE_SYSTEM = const<i32>(148);
// DEFAULT-NEXT:         %[[VALUE__SC_MONOTONIC_CLOCK:[0-9]+]] _SC_MONOTONIC_CLOCK = const<i32>(149);
// DEFAULT-NEXT:         %[[VALUE__SC_MULTI_PROCESS:[0-9]+]] _SC_MULTI_PROCESS = const<i32>(150);
// DEFAULT-NEXT:         %[[VALUE__SC_SINGLE_PROCESS:[0-9]+]] _SC_SINGLE_PROCESS = const<i32>(151);
// DEFAULT-NEXT:         %[[VALUE__SC_NETWORKING:[0-9]+]] _SC_NETWORKING = const<i32>(152);
// DEFAULT-NEXT:         %[[VALUE__SC_READER_WRITER_LOCKS:[0-9]+]] _SC_READER_WRITER_LOCKS = const<i32>(153);
// DEFAULT-NEXT:         %[[VALUE__SC_SPIN_LOCKS:[0-9]+]] _SC_SPIN_LOCKS = const<i32>(154);
// DEFAULT-NEXT:         %[[VALUE__SC_REGEXP:[0-9]+]] _SC_REGEXP = const<i32>(155);
// DEFAULT-NEXT:         %[[VALUE__SC_REGEX_VERSION:[0-9]+]] _SC_REGEX_VERSION = const<i32>(156);
// DEFAULT-NEXT:         %[[VALUE__SC_SHELL:[0-9]+]] _SC_SHELL = const<i32>(157);
// DEFAULT-NEXT:         %[[VALUE__SC_SIGNALS:[0-9]+]] _SC_SIGNALS = const<i32>(158);
// DEFAULT-NEXT:         %[[VALUE__SC_SPAWN:[0-9]+]] _SC_SPAWN = const<i32>(159);
// DEFAULT-NEXT:         %[[VALUE__SC_SPORADIC_SERVER:[0-9]+]] _SC_SPORADIC_SERVER = const<i32>(160);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_SPORADIC_SERVER:[0-9]+]] _SC_THREAD_SPORADIC_SERVER = const<i32>(161);
// DEFAULT-NEXT:         %[[VALUE__SC_SYSTEM_DATABASE:[0-9]+]] _SC_SYSTEM_DATABASE = const<i32>(162);
// DEFAULT-NEXT:         %[[VALUE__SC_SYSTEM_DATABASE_R:[0-9]+]] _SC_SYSTEM_DATABASE_R = const<i32>(163);
// DEFAULT-NEXT:         %[[VALUE__SC_TIMEOUTS:[0-9]+]] _SC_TIMEOUTS = const<i32>(164);
// DEFAULT-NEXT:         %[[VALUE__SC_TYPED_MEMORY_OBJECTS:[0-9]+]] _SC_TYPED_MEMORY_OBJECTS = const<i32>(165);
// DEFAULT-NEXT:         %[[VALUE__SC_USER_GROUPS:[0-9]+]] _SC_USER_GROUPS = const<i32>(166);
// DEFAULT-NEXT:         %[[VALUE__SC_USER_GROUPS_R:[0-9]+]] _SC_USER_GROUPS_R = const<i32>(167);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS:[0-9]+]] _SC_2_PBS = const<i32>(168);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS_ACCOUNTING:[0-9]+]] _SC_2_PBS_ACCOUNTING = const<i32>(169);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS_LOCATE:[0-9]+]] _SC_2_PBS_LOCATE = const<i32>(170);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS_MESSAGE:[0-9]+]] _SC_2_PBS_MESSAGE = const<i32>(171);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS_TRACK:[0-9]+]] _SC_2_PBS_TRACK = const<i32>(172);
// DEFAULT-NEXT:         %[[VALUE__SC_SYMLOOP_MAX:[0-9]+]] _SC_SYMLOOP_MAX = const<i32>(173);
// DEFAULT-NEXT:         %[[VALUE__SC_STREAMS:[0-9]+]] _SC_STREAMS = const<i32>(174);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS_CHECKPOINT:[0-9]+]] _SC_2_PBS_CHECKPOINT = const<i32>(175);
// DEFAULT-NEXT:         %[[VALUE__SC_V6_ILP32_OFF32:[0-9]+]] _SC_V6_ILP32_OFF32 = const<i32>(176);
// DEFAULT-NEXT:         %[[VALUE__SC_V6_ILP32_OFFBIG:[0-9]+]] _SC_V6_ILP32_OFFBIG = const<i32>(177);
// DEFAULT-NEXT:         %[[VALUE__SC_V6_LP64_OFF64:[0-9]+]] _SC_V6_LP64_OFF64 = const<i32>(178);
// DEFAULT-NEXT:         %[[VALUE__SC_V6_LPBIG_OFFBIG:[0-9]+]] _SC_V6_LPBIG_OFFBIG = const<i32>(179);
// DEFAULT-NEXT:         %[[VALUE__SC_HOST_NAME_MAX:[0-9]+]] _SC_HOST_NAME_MAX = const<i32>(180);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE:[0-9]+]] _SC_TRACE = const<i32>(181);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_EVENT_FILTER:[0-9]+]] _SC_TRACE_EVENT_FILTER = const<i32>(182);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_INHERIT:[0-9]+]] _SC_TRACE_INHERIT = const<i32>(183);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_LOG:[0-9]+]] _SC_TRACE_LOG = const<i32>(184);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_ICACHE_SIZE:[0-9]+]] _SC_LEVEL1_ICACHE_SIZE = const<i32>(185);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_ICACHE_ASSOC:[0-9]+]] _SC_LEVEL1_ICACHE_ASSOC = const<i32>(186);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_ICACHE_LINESIZE:[0-9]+]] _SC_LEVEL1_ICACHE_LINESIZE = const<i32>(187);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_DCACHE_SIZE:[0-9]+]] _SC_LEVEL1_DCACHE_SIZE = const<i32>(188);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_DCACHE_ASSOC:[0-9]+]] _SC_LEVEL1_DCACHE_ASSOC = const<i32>(189);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_DCACHE_LINESIZE:[0-9]+]] _SC_LEVEL1_DCACHE_LINESIZE = const<i32>(190);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL2_CACHE_SIZE:[0-9]+]] _SC_LEVEL2_CACHE_SIZE = const<i32>(191);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL2_CACHE_ASSOC:[0-9]+]] _SC_LEVEL2_CACHE_ASSOC = const<i32>(192);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL2_CACHE_LINESIZE:[0-9]+]] _SC_LEVEL2_CACHE_LINESIZE = const<i32>(193);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL3_CACHE_SIZE:[0-9]+]] _SC_LEVEL3_CACHE_SIZE = const<i32>(194);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL3_CACHE_ASSOC:[0-9]+]] _SC_LEVEL3_CACHE_ASSOC = const<i32>(195);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL3_CACHE_LINESIZE:[0-9]+]] _SC_LEVEL3_CACHE_LINESIZE = const<i32>(196);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL4_CACHE_SIZE:[0-9]+]] _SC_LEVEL4_CACHE_SIZE = const<i32>(197);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL4_CACHE_ASSOC:[0-9]+]] _SC_LEVEL4_CACHE_ASSOC = const<i32>(198);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL4_CACHE_LINESIZE:[0-9]+]] _SC_LEVEL4_CACHE_LINESIZE = const<i32>(199);
// DEFAULT-NEXT:         %[[VALUE__SC_IPV6:[0-9]+]] _SC_IPV6 = const<i32>(235);
// DEFAULT-NEXT:         %[[VALUE__SC_RAW_SOCKETS:[0-9]+]] _SC_RAW_SOCKETS = const<i32>(236);
// DEFAULT-NEXT:         %[[VALUE__SC_V7_ILP32_OFF32:[0-9]+]] _SC_V7_ILP32_OFF32 = const<i32>(237);
// DEFAULT-NEXT:         %[[VALUE__SC_V7_ILP32_OFFBIG:[0-9]+]] _SC_V7_ILP32_OFFBIG = const<i32>(238);
// DEFAULT-NEXT:         %[[VALUE__SC_V7_LP64_OFF64:[0-9]+]] _SC_V7_LP64_OFF64 = const<i32>(239);
// DEFAULT-NEXT:         %[[VALUE__SC_V7_LPBIG_OFFBIG:[0-9]+]] _SC_V7_LPBIG_OFFBIG = const<i32>(240);
// DEFAULT-NEXT:         %[[VALUE__SC_SS_REPL_MAX:[0-9]+]] _SC_SS_REPL_MAX = const<i32>(241);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_EVENT_NAME_MAX:[0-9]+]] _SC_TRACE_EVENT_NAME_MAX = const<i32>(242);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_NAME_MAX:[0-9]+]] _SC_TRACE_NAME_MAX = const<i32>(243);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_SYS_MAX:[0-9]+]] _SC_TRACE_SYS_MAX = const<i32>(244);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_USER_EVENT_MAX:[0-9]+]] _SC_TRACE_USER_EVENT_MAX = const<i32>(245);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_STREAMS:[0-9]+]] _SC_XOPEN_STREAMS = const<i32>(246);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_ROBUST_PRIO_INHERIT:[0-9]+]] _SC_THREAD_ROBUST_PRIO_INHERIT = const<i32>(247);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_ROBUST_PRIO_PROTECT:[0-9]+]] _SC_THREAD_ROBUST_PRIO_PROTECT = const<i32>(248);
// DEFAULT-NEXT:         %[[VALUE__SC_MINSIGSTKSZ:[0-9]+]] _SC_MINSIGSTKSZ = const<i32>(249);
// DEFAULT-NEXT:         %[[VALUE__SC_SIGSTKSZ:[0-9]+]] _SC_SIGSTKSZ = const<i32>(250);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([103, 110, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 105, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([103, 110, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 105, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE__SC_CLK_TCK]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_STREAM_MAX]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_REALTIME_SIGNALS]] @reallocarray(%[[VALUE___ptr:[0-9]+]] __ptr: ptr<void>, %[[VALUE___nmemb:[0-9]+]] __nmemb: u64, %[[VALUE___size_2:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_TIMERS]] @free(%[[VALUE___ptr_2:[0-9]+]] __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_SYNCHRONIZED_IO]] @memalign(%[[VALUE___alignment:[0-9]+]] __alignment: u64, %[[VALUE___size_3:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_MAPPED_FILES]] @valloc(%[[VALUE___size_4:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_MEMLOCK_RANGE]] @pvalloc(%[[VALUE___size_5:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_SEMAPHORES]] @mallopt(%[[VALUE___param:[0-9]+]] __param: i32, %[[VALUE___val:[0-9]+]] __val: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_AIO_LISTIO_MAX]] @malloc_usable_size(%[[VALUE___ptr_3:[0-9]+]] __ptr: ptr<void>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_MQ_OPEN_MAX]] @memcpy(%[[VALUE___dest:[0-9]+]] __dest: ptr<void> [restrict], %[[VALUE___src:[0-9]+]] __src: ptr<const void> [restrict], %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_PAGESIZE]] @strcmp(%[[VALUE___s1:[0-9]+]] __s1: ptr<const i8>, %[[VALUE___s2:[0-9]+]] __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE__SC_SEM_VALUE_MAX]] @index(%[[VALUE___s:[0-9]+]] __s: ptr<const i8>, %[[VALUE___c:[0-9]+]] __c: i32) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE__SC_BC_BASE_MAX]] @_obstack_newchunk(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE_obstack]]>, %[[VALUE1:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_BC_DIM_MAX]] @_obstack_begin(%[[VALUE2:[0-9]+]] <unnamed>: ptr<@type[[TYPE_obstack]]>, %[[VALUE3:[0-9]+]] <unnamed>: i32, %[[VALUE4:[0-9]+]] <unnamed>: i32, %[[VALUE5:[0-9]+]] <unnamed>: ptr<fn(i64) -> ptr<void>>, %[[VALUE6:[0-9]+]] <unnamed>: ptr<fn(ptr<void>) -> void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_BC_SCALE_MAX]] @obstack_free(%[[VALUE7:[0-9]+]] <unnamed>: ptr<@type[[TYPE_obstack]]>, %[[VALUE8:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sysconf:[0-9]+]] @sysconf(%[[VALUE___name:[0-9]+]] __name: i32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_gnu_allocation_extensions:[0-9]+]] @gnu_allocation_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_values:[0-9]+]] values: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE__SC_REALTIME_SIGNALS]], null<ptr<void>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), const<u64>(4)));
// DEFAULT-NEXT:         let %[[VALUE_aligned:[0-9]+]] aligned: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE__SC_SYNCHRONIZED_IO]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(80))));
// DEFAULT-NEXT:         let %[[VALUE_page:[0-9]+]] page: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE__SC_MAPPED_FILES]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_rounded:[0-9]+]] rounded: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE__SC_MEMLOCK_RANGE]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_page_size:[0-9]+]] page_size: i64 [storage=automatic] = call<i64, signature=fn(i32) -> i64>(%[[VALUE_sysconf]], const<i32>(30));
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_index:[0-9]+]] index: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_index]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_index]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_index]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_values]]), read<i32>(%[[VALUE_index]]))), add<i32, overflow=ub>(read<i32>(%[[VALUE_index]]), const<i32>(1)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_values]]), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_values]]), const<i32>(1))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_values]]), const<i32>(2))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_values]]), const<i32>(3))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), from_bool<i32, reason=promotion>(ge<u64>(call<u64, signature=fn(ptr<void>) -> u64>(%[[VALUE__SC_AIO_LISTIO_MAX]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_values]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), const<u64>(4)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_aligned]]), null<ptr<void>>), eq<u64>(rem<u64, by_zero=ub>(ptr_to_int<u64, reason=explicit>(read<ptr<void>>(%[[VALUE_aligned]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(64)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_page]]), null<ptr<void>>), eq<u64>(rem<u64, by_zero=ub>(ptr_to_int<u64, reason=explicit>(read<ptr<void>>(%[[VALUE_page]])), reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%[[VALUE_page_size]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_rounded]]), null<ptr<void>>), eq<u64>(rem<u64, by_zero=ub>(ptr_to_int<u64, reason=explicit>(read<ptr<void>>(%[[VALUE_rounded]])), reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%[[VALUE_page_size]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), from_bool<i32, reason=promotion>(ge<u64>(call<u64, signature=fn(ptr<void>) -> u64>(%[[VALUE__SC_AIO_LISTIO_MAX]], read<ptr<void>>(%[[VALUE_rounded]])), reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%[[VALUE_page_size]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE__SC_SEMAPHORES]], neg<i32, overflow=ub>(const<i32>(5)), const<i32>(1)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE__SC_TIMERS]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_values]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE__SC_TIMERS]], read<ptr<void>>(%[[VALUE_aligned]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE__SC_TIMERS]], read<ptr<void>>(%[[VALUE_page]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE__SC_TIMERS]], read<ptr<void>>(%[[VALUE_rounded]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_obstack_extensions:[0-9]+]] @gnu_obstack_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_storage:[0-9]+]] storage: @type[[TYPE_obstack]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_first:[0-9]+]] first: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_second:[0-9]+]] second: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_total_2:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE_obstack]]>, i32, i32, ptr<fn(i64) -> ptr<void>>, ptr<fn(ptr<void>) -> void>) -> i32>(%[[VALUE__SC_BC_DIM_MAX]], addr_of<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_storage]]), const<i32>(0), const<i32>(0), pointer_cast<ptr<fn(i64) -> ptr<void>>, reason=explicit>(function_decay<ptr<fn(u64) -> ptr<void>>>(%[[VALUE__SC_STREAM_MAX]])), function_decay<ptr<fn(ptr<void>) -> void>>(%[[VALUE__SC_TIMERS]]));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: ptr<void> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___h:[0-9]+]] __h: ptr<@type[[TYPE_obstack]]> [storage=automatic] = addr_of<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_storage]]);
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___o:[0-9]+]] __o: ptr<@type[[TYPE_obstack]]> [storage=automatic] = read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___h]]);
// DEFAULT-NEXT:                 let %[[VALUE___len:[0-9]+]] __len: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:                 if gt<ptr<i8>>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o]])))), read<i32>(%[[VALUE___len]])), const<i32>(1)), read<ptr<i8>>(field4(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o]])))))
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type[[TYPE_obstack]]>, i32) -> void>(%[[VALUE__SC_BC_BASE_MAX]], read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o]]), add<i32, overflow=ub>(read<i32>(%[[VALUE___len]]), const<i32>(1)));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE__SC_MQ_OPEN_MAX]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o]]))))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE___len]]))));
// DEFAULT-NEXT:                 let %[[VALUE27:[0-9]+]]: ptr<@type[[TYPE_obstack]]> [synthetic] = read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o]]);
// DEFAULT-NEXT:                 let %[[VALUE28:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE27]]))));
// DEFAULT-NEXT:                 let %[[VALUE29:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE28]]), read<i32>(%[[VALUE___len]]));
// DEFAULT-NEXT:                 write<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE27]]))), read<ptr<i8>>(%[[VALUE29]]));
// DEFAULT-NEXT:                 let %[[VALUE30:[0-9]+]]: ptr<@type[[TYPE_obstack]]> [synthetic] = read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o]]);
// DEFAULT-NEXT:                 let %[[VALUE31:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE30]]))));
// DEFAULT-NEXT:                 let %[[VALUE32:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE31]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE30]]))), read<ptr<i8>>(%[[VALUE32]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE31]])), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE33:[0-9]+]]: ptr<void> [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___o1:[0-9]+]] __o1: ptr<@type[[TYPE_obstack]]> [storage=automatic] = read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___h]]);
// DEFAULT-NEXT:                 let %[[VALUE___value:[0-9]+]] __value: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=explicit>(read<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))));
// DEFAULT-NEXT:                 if eq<ptr<i8>>(read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))), pointer_cast<ptr<i8>, reason=usual_arith>(read<ptr<void>>(%[[VALUE___value]])))
// DEFAULT-NEXT:                     write<u32>(bitfield11<unit=0, bytes=80..81, bits=1..2>(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]]))), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(conditional<ptr<i8>>(lt<u64>(const<u64>(8), const<u64>(8)), read<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))), null<ptr<i8>>), and<i64>(add<i64, overflow=ub>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))), conditional<ptr<i8>>(lt<u64>(const<u64>(8), const<u64>(8)), read<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))), null<ptr<i8>>)), widen<i64, reason=usual_arith>(read<i32>(field6(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))))), widen<i64, reason=usual_arith>(not<i32>(read<i32>(field6(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))))))));
// DEFAULT-NEXT:                 if gt<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))), pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type[[TYPE__obstack_chunk]]>>(field1(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field4(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))), pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type[[TYPE__obstack_chunk]]>>(field1(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))))))
// DEFAULT-NEXT:                     write<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]]))), read<ptr<i8>>(field4(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))));
// DEFAULT-NEXT:                 write<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]]))), read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))));
// DEFAULT-NEXT:                 write<ptr<void>>(%[[VALUE33]], read<ptr<void>>(%[[VALUE___value]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE26]], read<ptr<void>>(%[[VALUE33]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_first]], pointer_cast<ptr<i8>, reason=assign>(read<ptr<void>>(%[[VALUE26]])));
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: ptr<void> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___h_2:[0-9]+]] __h: ptr<@type[[TYPE_obstack]]> [storage=automatic] = addr_of<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_storage]]);
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___o_2:[0-9]+]] __o: ptr<@type[[TYPE_obstack]]> [storage=automatic] = read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___h_2]]);
// DEFAULT-NEXT:                 let %[[VALUE___len_2:[0-9]+]] __len: i32 [storage=automatic] = const<i32>(4);
// DEFAULT-NEXT:                 if gt<ptr<i8>>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o_2]])))), read<i32>(%[[VALUE___len_2]])), const<i32>(1)), read<ptr<i8>>(field4(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o_2]])))))
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type[[TYPE_obstack]]>, i32) -> void>(%[[VALUE__SC_BC_BASE_MAX]], read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o_2]]), add<i32, overflow=ub>(read<i32>(%[[VALUE___len_2]]), const<i32>(1)));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE__SC_MQ_OPEN_MAX]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o_2]]))))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE___len_2]]))));
// DEFAULT-NEXT:                 let %[[VALUE35:[0-9]+]]: ptr<@type[[TYPE_obstack]]> [synthetic] = read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o_2]]);
// DEFAULT-NEXT:                 let %[[VALUE36:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE35]]))));
// DEFAULT-NEXT:                 let %[[VALUE37:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE36]]), read<i32>(%[[VALUE___len_2]]));
// DEFAULT-NEXT:                 write<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE35]]))), read<ptr<i8>>(%[[VALUE37]]));
// DEFAULT-NEXT:                 let %[[VALUE38:[0-9]+]]: ptr<@type[[TYPE_obstack]]> [synthetic] = read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o_2]]);
// DEFAULT-NEXT:                 let %[[VALUE39:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE38]]))));
// DEFAULT-NEXT:                 let %[[VALUE40:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE39]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE38]]))), read<ptr<i8>>(%[[VALUE40]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE39]])), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE41:[0-9]+]]: ptr<void> [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___o1_2:[0-9]+]] __o1: ptr<@type[[TYPE_obstack]]> [storage=automatic] = read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___h_2]]);
// DEFAULT-NEXT:                 let %[[VALUE___value_2:[0-9]+]] __value: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=explicit>(read<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]])))));
// DEFAULT-NEXT:                 if eq<ptr<i8>>(read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]])))), pointer_cast<ptr<i8>, reason=usual_arith>(read<ptr<void>>(%[[VALUE___value_2]])))
// DEFAULT-NEXT:                     write<u32>(bitfield11<unit=0, bytes=80..81, bits=1..2>(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]]))), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(conditional<ptr<i8>>(lt<u64>(const<u64>(8), const<u64>(8)), read<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]])))), null<ptr<i8>>), and<i64>(add<i64, overflow=ub>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]])))), conditional<ptr<i8>>(lt<u64>(const<u64>(8), const<u64>(8)), read<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]])))), null<ptr<i8>>)), widen<i64, reason=usual_arith>(read<i32>(field6(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]])))))), widen<i64, reason=usual_arith>(not<i32>(read<i32>(field6(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]])))))))));
// DEFAULT-NEXT:                 if gt<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]])))), pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type[[TYPE__obstack_chunk]]>>(field1(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]])))))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field4(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]])))), pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type[[TYPE__obstack_chunk]]>>(field1(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]])))))))
// DEFAULT-NEXT:                     write<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]]))), read<ptr<i8>>(field4(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]])))));
// DEFAULT-NEXT:                 write<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]]))), read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1_2]])))));
// DEFAULT-NEXT:                 write<ptr<void>>(%[[VALUE41]], read<ptr<void>>(%[[VALUE___value_2]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE34]], read<ptr<void>>(%[[VALUE41]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_second]], pointer_cast<ptr<i8>, reason=assign>(read<ptr<void>>(%[[VALUE34]])));
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE42]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE__SC_PAGESIZE]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_first]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]]))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE43]]));
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE44]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE__SC_PAGESIZE]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_second]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_4]]))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE45]]));
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___o_3:[0-9]+]] __o: ptr<const @type[[TYPE_obstack]]> [storage=automatic] = pointer_cast<ptr<const @type[[TYPE_obstack]]>, reason=assign>(addr_of<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_storage]]));
// DEFAULT-NEXT:             write<u32>(%[[VALUE47]], reinterpret<u32, reason=explicit, fits=unknown>(truncate<i32, reason=explicit, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<const @type[[TYPE_obstack]]>>(%[[VALUE___o_3]])))), read<ptr<i8>>(field2(deref(read<ptr<const @type[[TYPE_obstack]]>>(%[[VALUE___o_3]]))))))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE46]]), from_bool<i32, reason=promotion>(eq<u32>(read<u32>(%[[VALUE47]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE48]]));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___o_4:[0-9]+]] __o: ptr<@type[[TYPE_obstack]]> [storage=automatic] = addr_of<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_storage]]);
// DEFAULT-NEXT:             let %[[VALUE___obj:[0-9]+]] __obj: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:             if logical_and<bool>(gt<ptr<void>>(read<ptr<void>>(%[[VALUE___obj]]), pointer_cast<ptr<void>, reason=explicit>(read<ptr<@type[[TYPE__obstack_chunk]]>>(field1(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o_4]])))))), lt<ptr<void>>(read<ptr<void>>(%[[VALUE___obj]]), pointer_cast<ptr<void>, reason=explicit>(read<ptr<i8>>(field4(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o_4]])))))))
// DEFAULT-NEXT:                 let %[[VALUE49:[0-9]+]]: ptr<i8> [synthetic] = pointer_cast<ptr<i8>, reason=explicit>(read<ptr<void>>(%[[VALUE___obj]]));
// DEFAULT-NEXT:                 write<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o_4]]))), read<ptr<i8>>(%[[VALUE49]]));
// DEFAULT-NEXT:                 write<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o_4]]))), read<ptr<i8>>(%[[VALUE49]]));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE_obstack]]>, ptr<void>) -> void>(%[[VALUE__SC_BC_SCALE_MAX]], read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o_4]]), read<ptr<void>>(%[[VALUE___obj]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE__SC_CLK_TCK]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_5]])), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_allocation_extensions]]), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_obstack_extensions]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
