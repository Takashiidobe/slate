/* PR middle-end/31309 */
/* Origin: Peeter Joot <peeterj@ca.ibm.com> */

/* { dg-do run { target *-*-linux* *-*-gnu* *-*-uclinux* } } */
/* { dg-add-options stack_size } */
 
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#if defined(STACK_SIZE) && (STACK_SIZE < 128*1024)
 #define CHUNK_SIZE 4096
#else
 #define CHUNK_SIZE 16384
#endif

unsigned long ossAlignX(unsigned long i, unsigned long X)
{
   return ((i + (X - 1)) & ~(unsigned long) (X - 1));
}

struct STRUCT_6_BYTES
{
   unsigned char slot[sizeof(unsigned short)];
   unsigned char page[sizeof(unsigned int)];
};

struct SQLU_DICT_INFO_0
{
   void *pBlah;
   char bSomeFlag1;
   char bSomeFlag2;
   struct STRUCT_6_BYTES dRID;
};

struct SQLU_DATAPART_0
{
   struct SQLU_DICT_INFO_0 *pDictRidderInfo;
};

struct XXX
{
   struct SQLU_DATAPART_0 *m_pDatapart;
};

struct STRUCT_6_BYTES INIT_6_BYTES_ZERO()
{
   struct STRUCT_6_BYTES ridOut = {{0,0}, {0,0,0,0}};
   return ridOut;
}

void Initialize(struct XXX *this, int iIndex)
{
   struct SQLU_DICT_INFO_0 *pDictRidderInfo
     = this->m_pDatapart[iIndex].pDictRidderInfo;
   pDictRidderInfo->bSomeFlag1 = 0;
   pDictRidderInfo->bSomeFlag2 = 0;
   pDictRidderInfo->dRID = INIT_6_BYTES_ZERO();
}

int main(void)
{
   int rc;

   struct stuff
   {
      char c0[CHUNK_SIZE-sizeof(struct XXX)];
      struct XXX o;
      char c1[CHUNK_SIZE*2-sizeof(struct SQLU_DATAPART_0)];
      struct SQLU_DATAPART_0 dp;
      char c2[CHUNK_SIZE*2-sizeof(struct SQLU_DICT_INFO_0)];
      struct SQLU_DICT_INFO_0 di;
      char c3[CHUNK_SIZE];
   };

   char buf[sizeof(struct stuff)+CHUNK_SIZE];
   struct stuff *u
     = (struct stuff *)ossAlignX((unsigned long)&buf[0], CHUNK_SIZE);

   /* This test assumes system memory page
      size of CHUNK_SIZE bytes or less.  */
   if (sysconf(_SC_PAGESIZE) > CHUNK_SIZE)
     return 0;

   memset(u, 1, sizeof(struct stuff));
   u->c1[0] = '\xAA';
   u->c2[0] = '\xBB';
   u->c3[0] = '\xCC';

   rc = mprotect(u->c1, CHUNK_SIZE, PROT_NONE);
   if (rc == -1)
      printf("mprotect:c1: %d: %d(%s)\n", rc, errno, strerror(errno));

   rc = mprotect(u->c2, CHUNK_SIZE, PROT_NONE);
   if (rc == -1)
      printf("mprotect:c2: %d: %d(%s)\n", rc, errno, strerror(errno));

   rc = mprotect(u->c3, CHUNK_SIZE, PROT_NONE);
   if (rc == -1)
      printf("mprotect:c3: %d: %d(%s)\n", rc, errno, strerror(errno));

   u->o.m_pDatapart = &u->dp;
   u->dp.pDictRidderInfo = &u->di;
   Initialize(&u->o, 0);

   mprotect(u->c1, CHUNK_SIZE, PROT_READ|PROT_WRITE);
   mprotect(u->c2, CHUNK_SIZE, PROT_READ|PROT_WRITE);
   mprotect(u->c3, CHUNK_SIZE, PROT_READ|PROT_WRITE);

   return 0;
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
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
// DEFAULT-NEXT:     type @type[[TYPE_STRUCT_6_BYTES:[0-9]+]] STRUCT_6_BYTES = struct {
// DEFAULT-NEXT:         field0 slot: array<u8, 2>;
// DEFAULT-NEXT:         field1 page: array<u8, 4>;
// DEFAULT-NEXT:     } [size=6, align=1, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type[[TYPE_SQLU_DICT_INFO_0:[0-9]+]] SQLU_DICT_INFO_0 = struct {
// DEFAULT-NEXT:         field0 pBlah: ptr<void>;
// DEFAULT-NEXT:         field1 bSomeFlag1: i8;
// DEFAULT-NEXT:         field2 bSomeFlag2: i8;
// DEFAULT-NEXT:         field3 dRID: @type[[TYPE_STRUCT_6_BYTES]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 9, 10]];
// DEFAULT-NEXT:     type @type[[TYPE_SQLU_DATAPART_0:[0-9]+]] SQLU_DATAPART_0 = struct {
// DEFAULT-NEXT:         field0 pDictRidderInfo: ptr<@type[[TYPE_SQLU_DICT_INFO_0]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_XXX:[0-9]+]] XXX = struct {
// DEFAULT-NEXT:         field0 m_pDatapart: ptr<@type[[TYPE_SQLU_DATAPART_0]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_stuff:[0-9]+]] stuff = struct {
// DEFAULT-NEXT:         field0 c0: array<i8, 16376>;
// DEFAULT-NEXT:         field1 o: @type[[TYPE_XXX]];
// DEFAULT-NEXT:         field2 c1: array<i8, 32760>;
// DEFAULT-NEXT:         field3 dp: @type[[TYPE_SQLU_DATAPART_0]];
// DEFAULT-NEXT:         field4 c2: array<i8, 32752>;
// DEFAULT-NEXT:         field5 di: @type[[TYPE_SQLU_DICT_INFO_0]];
// DEFAULT-NEXT:         field6 c3: array<i8, 16384>;
// DEFAULT-NEXT:     } [size=98304, align=8, offsets=[0, 16376, 16384, 49144, 49152, 81904, 81920]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([109, 112, 114, 111, 116, 101, 99, 116, 58, 99, 49, 58, 32, 37, 100, 58, 32, 37, 100, 40, 37, 115, 41, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([109, 112, 114, 111, 116, 101, 99, 116, 58, 99, 50, 58, 32, 37, 100, 58, 32, 37, 100, 40, 37, 115, 41, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([109, 112, 114, 111, 116, 101, 99, 116, 58, 99, 51, 58, 32, 37, 100, 58, 32, 37, 100, 40, 37, 115, 41, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE__SC_OPEN_MAX]] @mprotect(%[[VALUE___addr:[0-9]+]] __addr: ptr<void>, %[[VALUE___len:[0-9]+]] __len: u64, %[[VALUE___prot:[0-9]+]] __prot: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_SAVED_IDS]] @memset(%[[VALUE___s:[0-9]+]] __s: ptr<void>, %[[VALUE___c:[0-9]+]] __c: i32, %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_PRIORITY_SCHEDULING]] @strerror(%[[VALUE___errnum:[0-9]+]] __errnum: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_ASYNCHRONOUS_IO]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_PRIORITIZED_IO]] @__errno_location() -> ptr<i32> [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_sysconf:[0-9]+]] @sysconf(%[[VALUE___name:[0-9]+]] __name: i32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ossAlignX:[0-9]+]] @ossAlignX(%[[VALUE_i:[0-9]+]] i: u64, %[[VALUE_X:[0-9]+]] X: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<u64>(add<u64, overflow=wrap>(read<u64>(%[[VALUE_i]]), sub<u64, overflow=wrap>(read<u64>(%[[VALUE_X]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), not<u64>(sub<u64, overflow=wrap>(read<u64>(%[[VALUE_X]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_INIT_6_BYTES_ZERO:[0-9]+]] @INIT_6_BYTES_ZERO(unprototyped) -> @type[[TYPE_STRUCT_6_BYTES]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ridOut:[0-9]+]] ridOut: @type[[TYPE_STRUCT_6_BYTES]] [storage=automatic] = aggregate<@type[[TYPE_STRUCT_6_BYTES]], zero_fill=false>(field0 = aggregate<array<u8, 2>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)))), field1 = aggregate<array<u8, 4>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         return copy<@type[[TYPE_STRUCT_6_BYTES]], reason=return>(read<@type[[TYPE_STRUCT_6_BYTES]]>(%[[VALUE_ridOut]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Initialize:[0-9]+]] @Initialize(%[[VALUE_this:[0-9]+]] this: ptr<@type[[TYPE_XXX]]>, %[[VALUE_iIndex:[0-9]+]] iIndex: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_pDictRidderInfo:[0-9]+]] pDictRidderInfo: ptr<@type[[TYPE_SQLU_DICT_INFO_0]]> [storage=automatic] = read<ptr<@type[[TYPE_SQLU_DICT_INFO_0]]>>(field0(deref(ptr_offset<ptr<@type[[TYPE_SQLU_DATAPART_0]]>, subtract=false, element=@type[[TYPE_SQLU_DATAPART_0]], overflow=ub>(read<ptr<@type[[TYPE_SQLU_DATAPART_0]]>>(field0(deref(read<ptr<@type[[TYPE_XXX]]>>(%[[VALUE_this]])))), read<i32>(%[[VALUE_iIndex]])))));
// DEFAULT-NEXT:         write<i8>(field1(deref(read<ptr<@type[[TYPE_SQLU_DICT_INFO_0]]>>(%[[VALUE_pDictRidderInfo]]))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(field2(deref(read<ptr<@type[[TYPE_SQLU_DICT_INFO_0]]>>(%[[VALUE_pDictRidderInfo]]))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<@type[[TYPE_STRUCT_6_BYTES]]>(field3(deref(read<ptr<@type[[TYPE_SQLU_DICT_INFO_0]]>>(%[[VALUE_pDictRidderInfo]]))), copy<@type[[TYPE_STRUCT_6_BYTES]], reason=assign>(call<@type[[TYPE_STRUCT_6_BYTES]], signature=fn(unprototyped) -> @type[[TYPE_STRUCT_6_BYTES]], abi=sysv64() -> native_c>(%[[VALUE_INIT_6_BYTES_ZERO]])));
// DEFAULT-NEXT:         copy<@type[[TYPE_STRUCT_6_BYTES]], reason=assign>(call<@type[[TYPE_STRUCT_6_BYTES]], signature=fn(unprototyped) -> @type[[TYPE_STRUCT_6_BYTES]], abi=sysv64() -> native_c>(%[[VALUE_INIT_6_BYTES_ZERO]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_rc:[0-9]+]] rc: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: array<i8, 114688> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: ptr<@type[[TYPE_stuff]]> [storage=automatic] = int_to_ptr<ptr<@type[[TYPE_stuff]]>, reason=explicit>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_ossAlignX]], ptr_to_int<u64, reason=explicit>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(114688)>(%[[VALUE_buf]]), const<i32>(0))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384)))));
// DEFAULT-NEXT:         if gt<i64>(call<i64, signature=fn(i32) -> i64>(%[[VALUE_sysconf]], const<i32>(30)), widen<i64, reason=usual_arith>(const<i32>(16384)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE__SC_SAVED_IDS]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]])), const<i32>(1), const<u64>(98304));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32760)>(field2(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]])))), const<i32>(0))), truncate<i8, reason=assign, fits=unknown>(const<i32>(-86)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32752)>(field4(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]])))), const<i32>(0))), truncate<i8, reason=assign, fits=unknown>(const<i32>(-69)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16384)>(field6(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]])))), const<i32>(0))), truncate<i8, reason=assign, fits=unknown>(const<i32>(-52)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_rc]], call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%[[VALUE__SC_OPEN_MAX]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(32760)>(field2(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]]))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%[[VALUE__SC_OPEN_MAX]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(32760)>(field2(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]]))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), const<i32>(0));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_rc]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE__SC_ASYNCHRONOUS_IO]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(25)>(%[[VALUE_str]])), read<i32>(%[[VALUE_rc]]), read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%[[VALUE__SC_PRIORITIZED_IO]]))), call<ptr<i8>, signature=fn(i32) -> ptr<i8>>(%[[VALUE__SC_PRIORITY_SCHEDULING]], read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%[[VALUE__SC_PRIORITIZED_IO]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_rc]], call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%[[VALUE__SC_OPEN_MAX]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(32752)>(field4(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]]))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%[[VALUE__SC_OPEN_MAX]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(32752)>(field4(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]]))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), const<i32>(0));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_rc]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE__SC_ASYNCHRONOUS_IO]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(25)>(%[[VALUE_str_2]])), read<i32>(%[[VALUE_rc]]), read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%[[VALUE__SC_PRIORITIZED_IO]]))), call<ptr<i8>, signature=fn(i32) -> ptr<i8>>(%[[VALUE__SC_PRIORITY_SCHEDULING]], read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%[[VALUE__SC_PRIORITIZED_IO]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_rc]], call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%[[VALUE__SC_OPEN_MAX]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(16384)>(field6(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]]))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%[[VALUE__SC_OPEN_MAX]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(16384)>(field6(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]]))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), const<i32>(0));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_rc]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE__SC_ASYNCHRONOUS_IO]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(25)>(%[[VALUE_str_3]])), read<i32>(%[[VALUE_rc]]), read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%[[VALUE__SC_PRIORITIZED_IO]]))), call<ptr<i8>, signature=fn(i32) -> ptr<i8>>(%[[VALUE__SC_PRIORITY_SCHEDULING]], read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%[[VALUE__SC_PRIORITIZED_IO]])))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_SQLU_DATAPART_0]]>>(field0(field1(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]])))), addr_of<ptr<@type[[TYPE_SQLU_DATAPART_0]]>>(field3(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]])))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_SQLU_DICT_INFO_0]]>>(field0(field3(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]])))), addr_of<ptr<@type[[TYPE_SQLU_DICT_INFO_0]]>>(field5(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]])))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_XXX]]>, i32) -> void>(%[[VALUE_Initialize]], addr_of<ptr<@type[[TYPE_XXX]]>>(field1(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]])))), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%[[VALUE__SC_OPEN_MAX]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(32760)>(field2(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]]))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), or<i32>(const<i32>(1), const<i32>(2)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%[[VALUE__SC_OPEN_MAX]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(32752)>(field4(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]]))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), or<i32>(const<i32>(1), const<i32>(2)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%[[VALUE__SC_OPEN_MAX]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(16384)>(field6(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_u]]))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), or<i32>(const<i32>(1), const<i32>(2)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
