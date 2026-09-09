#ifndef _SLATE_LINK_H
#define _SLATE_LINK_H

#include <features.h>

#if defined(__SLATE_LIBC_GLIBC)
#include <dlfcn.h>
#include <sys/types.h>
#endif

#include <elf.h>
#define __NEED_size_t
#define __NEED_uint32_t
#include <bits/types.h>

#if UINTPTR_MAX > 0xffffffff
#define ElfW(type) Elf64_##type
#else
#define ElfW(type) Elf32_##type
#endif

typedef uint32_t ElfSymdx;

#if defined(__SLATE_LIBC_MUSL)
typedef uint32_t Elf_Symndx;
#endif

#if defined(__SLATE_LIBC_GLIBC) && defined(__SLATE_ARCH_X86)
typedef struct La_i86_regs {
  uint32_t lr_edx;
  uint32_t lr_ecx;
  uint32_t lr_eax;
  uint32_t lr_ebp;
  uint32_t lr_esp;
} La_i86_regs;
#endif

#if defined(__SLATE_LIBC_GLIBC) && defined(__SLATE_ARCH_ARM)
typedef struct La_arm_regs {
  uint32_t lr_reg[4];
  uint32_t lr_sp;
  uint32_t lr_lr;
  uint32_t lr_coproc[42];
} La_arm_regs;
#endif

struct dl_phdr_info {
  ElfW(Addr) dlpi_addr;
  const char *dlpi_name;
  const ElfW(Phdr) * dlpi_phdr;
  ElfW(Half) dlpi_phnum;
  unsigned long long int dlpi_adds;
  unsigned long long int dlpi_subs;
  size_t                 dlpi_tls_modid;
  void                  *dlpi_tls_data;
};

struct link_map {
  ElfW(Addr) l_addr;
  char *l_name;
  ElfW(Dyn) * l_ld;
  struct link_map *l_next, *l_prev;
};

struct r_debug {
  int              r_version;
  struct link_map *r_map;
  ElfW(Addr) r_brk;
  enum { RT_CONSISTENT, RT_ADD, RT_DELETE } r_state;
  ElfW(Addr) r_ldbase;
};

int dl_iterate_phdr(int (*)(struct dl_phdr_info *, size_t, void *), void *);

#if defined(__SLATE_LIBC_GLIBC)
#define LAV_CURRENT 2
#endif
#if (defined(__SLATE_LIBC_GLIBC) && defined(__SLATE_ARCH_X86_64))
#define La_x32_regs La_x86_64_regs
#define La_x32_retval La_x86_64_retval
#endif

#endif
