#ifndef _SLATE_BITS_X86_VM86_H
#define _SLATE_BITS_X86_VM86_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/x86/vm86.h> directly; include a standard header instead."
#endif

#define BIOSSEG 0x0f000
#define CPU_086 0
#define CPU_186 1
#define CPU_286 2
#define CPU_386 3
#define CPU_486 4
#define CPU_586 5
#define VM86_TYPE(v) ((v) & 0xff)
#define VM86_ARG(v) ((v) >> 8)
#define VM86_SIGNAL 0
#define VM86_UNKNOWN 1
#define VM86_INTx 2
#define VM86_STI 3
#define VM86_PICRETURN 4
#define VM86_TRAP 6
#define VM86_PLUS_INSTALL_CHECK 0
#define VM86_ENTER 1
#define VM86_ENTER_NO_BYPASS 2
#define VM86_REQUEST_IRQ 3
#define VM86_FREE_IRQ 4
#define VM86_GET_IRQ_BITS 5
#define VM86_GET_AND_RESET_IRQ 6

#define X86_EFLAGS_IOPL_BIT 12
#define X86_EFLAGS_NT_BIT 14
#define X86_EFLAGS_RF_BIT 16
#define X86_EFLAGS_VM_BIT 17
#define X86_EFLAGS_AC_BIT 18
#define X86_EFLAGS_VIF_BIT 19
#define X86_EFLAGS_VIP_BIT 20
#define X86_EFLAGS_ID_BIT 21
#define X86_CR0_PE_BIT 0
#define X86_CR0_MP_BIT 1
#define X86_CR0_EM_BIT 2
#define X86_CR0_TS_BIT 3
#define X86_CR0_ET_BIT 4
#define X86_CR0_NE_BIT 5
#define X86_CR0_WP_BIT 16
#define X86_CR0_AM_BIT 18
#define X86_CR0_NW_BIT 29
#define X86_CR0_CD_BIT 30
#define X86_CR0_PG_BIT 31
#define X86_CR3_PWT_BIT 3
#define X86_CR3_PCD_BIT 4
#define X86_CR3_PCID_BITS 12
#define X86_CR3_LAM_U57_BIT 61
#define X86_CR3_LAM_U48_BIT 62
#define X86_CR3_PCID_NOFLUSH_BIT 63
#define X86_CR4_VME_BIT 0
#define X86_CR4_PVI_BIT 1
#define X86_CR4_TSD_BIT 2
#define X86_CR4_DE_BIT 3
#define X86_CR4_PSE_BIT 4
#define X86_CR4_PAE_BIT 5
#define X86_CR4_MCE_BIT 6
#define X86_CR4_PGE_BIT 7
#define X86_CR4_PCE_BIT 8
#define X86_CR4_OSFXSR_BIT 9
#define X86_CR4_OSXMMEXCPT_BIT 10
#define X86_CR4_UMIP_BIT 11
#define X86_CR4_LA57_BIT 12
#define X86_CR4_VMXE_BIT 13
#define X86_CR4_SMXE_BIT 14
#define X86_CR4_FSGSBASE_BIT 16
#define X86_CR4_PCIDE_BIT 17
#define X86_CR4_OSXSAVE_BIT 18
#define X86_CR4_SMEP_BIT 20
#define X86_CR4_SMAP_BIT 21
#define X86_CR4_PKE_BIT 22
#define X86_CR4_CET_BIT 23
#define X86_CR4_LASS_BIT 27
#define X86_CR4_LAM_SUP_BIT 28
#define CX86_PCR0 0x20
#define CX86_GCR 0xb8
#define CX86_CCR0 0xc0
#define CX86_CCR1 0xc1
#define CX86_CCR2 0xc2
#define CX86_CCR3 0xc3
#define CX86_CCR4 0xe8
#define CX86_CCR5 0xe9
#define CX86_CCR6 0xea
#define CX86_CCR7 0xeb
#define CX86_PCR1 0xf0
#define CX86_DIR0 0xfe
#define CX86_DIR1 0xff
#define CX86_ARR_BASE 0xc4
#define CX86_RCR_BASE 0xdc

#define X86_EFLAGS_IOPL 0
#define X86_EFLAGS_NT 0
#define X86_EFLAGS_RF 0
#define X86_EFLAGS_VM 0
#define X86_EFLAGS_AC 0
#define X86_EFLAGS_VIF 0
#define X86_EFLAGS_VIP 0
#define X86_EFLAGS_ID 0
#define X86_CR0_PE 0
#define X86_CR0_MP 0
#define X86_CR0_EM 0
#define X86_CR0_TS 0
#define X86_CR0_ET 0
#define X86_CR0_NE 0
#define X86_CR0_WP 0
#define X86_CR0_AM 0
#define X86_CR0_NW 0
#define X86_CR0_CD 0
#define X86_CR0_PG 0
#define X86_CR3_PWT 0
#define X86_CR3_PCD 0
#define X86_CR3_PCID_MASK 0
#define X86_CR3_LAM_U57 0
#define X86_CR3_LAM_U48 0
#define X86_CR3_PCID_NOFLUSH 0
#define X86_CR4_VME 0
#define X86_CR4_PVI 0
#define X86_CR4_TSD 0
#define X86_CR4_DE 0
#define X86_CR4_PSE 0
#define X86_CR4_PAE 0
#define X86_CR4_MCE 0
#define X86_CR4_PGE 0
#define X86_CR4_PCE 0
#define X86_CR4_OSFXSR 0
#define X86_CR4_OSXMMEXCPT 0
#define X86_CR4_UMIP 0
#define X86_CR4_LA57 0
#define X86_CR4_VMXE 0
#define X86_CR4_SMXE 0
#define X86_CR4_FSGSBASE 0
#define X86_CR4_PCIDE 0
#define X86_CR4_OSXSAVE 0
#define X86_CR4_SMEP 0
#define X86_CR4_SMAP 0
#define X86_CR4_PKE 0
#define X86_CR4_CET 0
#define X86_CR4_LASS 0
#define X86_CR4_LAM_SUP 0
#define X86_CR4_FRED 0
#define X86_CR8_TPR 0
#define CR0_STATE 0

struct vm86_regs {
  long ebx, ecx, edx, esi, edi, ebp, eax;
  long __null_ds, __null_es, __null_fs, __null_gs;
  long orig_eax, eip;
  unsigned short cs, __csh;
  long eflags, esp;
  unsigned short ss, __ssh, es, __esh, ds, __dsh, fs, __fsh, gs, __gsh;
};

struct revectored_struct { unsigned long __map[8]; };

struct vm86_struct {
  struct vm86_regs regs;
  unsigned long flags, screen_bitmap, cpu_type;
  struct revectored_struct int_revectored, int21_revectored;
};

#define VM86_SCREEN_BITMAP 0x0001

struct vm86plus_info_struct {
  unsigned long force_return_for_pic:1;
  unsigned long vm86dbg_active:1;
  unsigned long vm86dbg_TFpendig:1;
  unsigned long unused:28;
  unsigned long is_vm86pus:1;
  unsigned char vm86dbg_intxxtab[32];
};

struct vm86plus_struct {
  struct vm86_regs regs;
  unsigned long flags, screen_bitmap, cpu_type;
  struct revectored_struct int_revectored, int21_revectored;
  struct vm86plus_info_struct vm86plus;
};

#endif
