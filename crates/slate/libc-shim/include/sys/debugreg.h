#ifndef _SYS_DEBUGREG_H
#define _SYS_DEBUGREG_H
#if defined(__SLATE_LIBC_GLIBC) && (defined(__SLATE_ARCH_X86) || defined(__SLATE_ARCH_X86_64))
#define DR_FIRSTADDR 0
#define DR_LASTADDR 3
#define DR_STATUS 6
#define DR_CONTROL 7
#define DR_TRAP0 0x1
#define DR_TRAP1 0x2
#define DR_TRAP2 0x4
#define DR_TRAP3 0x8
#define DR_STEP 0x4000
#define DR_SWITCH 0x8000
#define DR_CONTROL_SHIFT 16
#define DR_CONTROL_SIZE 4
#define DR_RW_EXECUTE 0x0
#define DR_RW_WRITE 0x1
#define DR_RW_READ 0x3
#define DR_LEN_1 0x0
#define DR_LEN_2 0x4
#define DR_LEN_4 0xC
#if defined(__SLATE_ARCH_X86_64)
#define DR_LEN_8 0x8
#define DR_CONTROL_RESERVED 0xFFFFFFFF0000FC00ULL
#else
#define DR_CONTROL_RESERVED 0x00FC00U
#endif
#define DR_LOCAL_ENABLE_SHIFT 0
#define DR_GLOBAL_ENABLE_SHIFT 1
#define DR_ENABLE_SIZE 2
#define DR_LOCAL_ENABLE_MASK 0x55
#define DR_GLOBAL_ENABLE_MASK 0xAA
#define DR_LOCAL_SLOWDOWN 0x100
#define DR_GLOBAL_SLOWDOWN 0x200
#endif
#endif
