#ifndef _SLATE_A_OUT_H
#define _SLATE_A_OUT_H

#if defined(__SLATE_LIBC_GLIBC)

#define __GNU_EXEC_MACROS__

struct exec {
  unsigned long a_info;
  unsigned int a_text;
  unsigned int a_data;
  unsigned int a_bss;
  unsigned int a_syms;
  unsigned int a_entry;
  unsigned int a_trsize;
  unsigned int a_drsize;
};

#define N_MAGIC(exec) ((exec).a_info & 0xffff)
#define N_MACHTYPE(exec) ((enum machine_type)(((exec).a_info >> 16) & 0xff))
#define N_FLAGS(exec) ((exec).a_info >> 24 & 0xff)
#define N_SET_INFO(exec, magic, type, flags) \
  ((exec).a_info = ((magic) & 0xffff) | (((int)(type) & 0xff) << 16) | \
                   (((flags) & 0xff) << 24))
#define N_SET_MAGIC(exec, magic) \
  ((exec).a_info = ((exec).a_info & 0xffff0000) | ((magic) & 0xffff))
#define N_SET_MACHTYPE(exec, machtype) \
  ((exec).a_info = ((exec).a_info & 0xff00ffff) | \
                   (((int)(machtype) & 0xff) << 16))
#define N_SET_FLAGS(exec, flags) \
  ((exec).a_info = ((exec).a_info & 0x00ffffff) | (((flags) & 0xff) << 24))

#define OMAGIC 0407
#define NMAGIC 0410
#define ZMAGIC 0413
#define QMAGIC 0314
#define CMAGIC 0421

#define N_TRSIZE(a) ((a).a_trsize)
#define N_DRSIZE(a) ((a).a_drsize)
#define N_SYMSIZE(a) ((a).a_syms)
#define N_MAGIC(exec) ((exec).a_info & 0xffff)
#define N_BADMAG(x) \
  (N_MAGIC(x) != OMAGIC && N_MAGIC(x) != NMAGIC && N_MAGIC(x) != ZMAGIC && \
   N_MAGIC(x) != QMAGIC)
#define _N_HDROFF(x) (1024 - sizeof(struct exec))
#define N_TXTOFF(x) \
  (N_MAGIC(x) == ZMAGIC ? _N_HDROFF(x) + sizeof(struct exec) : \
   (N_MAGIC(x) == QMAGIC ? 0 : sizeof(struct exec)))
#define N_DATOFF(x) (N_TXTOFF(x) + (x).a_text)
#define N_TRELOFF(x) (N_DATOFF(x) + (x).a_data)
#define N_DRELOFF(x) (N_TRELOFF(x) + N_TRSIZE(x))
#define N_SYMOFF(x) (N_DRELOFF(x) + N_DRSIZE(x))
#define N_STROFF(x) (N_SYMOFF(x) + N_SYMSIZE(x))
#define N_TXTADDR(x) (N_MAGIC(x) == QMAGIC ? 4096 : 0)
#define SEGMENT_SIZE 1024
#define _N_SEGMENT_ROUND(x) (((x) + SEGMENT_SIZE - 1) & ~(SEGMENT_SIZE - 1))
#define _N_TXTENDADDR(x) (N_TXTADDR(x) + (x).a_text)
#define N_DATADDR(x) \
  (N_MAGIC(x) == OMAGIC ? _N_TXTENDADDR(x) : _N_SEGMENT_ROUND(_N_TXTENDADDR(x)))
#define N_BSSADDR(x) (N_DATADDR(x) + (x).a_data)

#define N_UNDF 0
#define N_ABS 2
#define N_TEXT 4
#define N_DATA 6
#define N_BSS 8
#define N_FN 15
#define N_EXT 1
#define N_TYPE 036
#define N_STAB 0340
#define N_INDR 0xa
#define N_SETA 0x14
#define N_SETT 0x16
#define N_SETD 0x18
#define N_SETB 0x1a
#define N_SETV 0x1c

enum machine_type {
  M_OLDSUN2 = 0,
  M_68010 = 1,
  M_68020 = 2,
  M_SPARC = 3,
  M_386 = 100,
  M_MIPS1 = 151,
  M_MIPS2 = 152
};

#endif

#endif
