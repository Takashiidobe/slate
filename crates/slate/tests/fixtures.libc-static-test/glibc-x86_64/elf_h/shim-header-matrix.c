#include <elf.h>

typedef unsigned short slate_oracle_typedef_Elf32_Half;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_Elf32_Half, Elf32_Half), "typedef Elf32_Half differs from oracle");

#ifndef AT_BASE
#error "elf.h:AT_BASE macro is missing from libc-shim"
#endif

#ifndef AT_BASE_PLATFORM
#error "elf.h:AT_BASE_PLATFORM macro is missing from libc-shim"
#endif

#ifndef AT_CLKTCK
#error "elf.h:AT_CLKTCK macro is missing from libc-shim"
#endif

#ifndef AT_DCACHEBSIZE
#error "elf.h:AT_DCACHEBSIZE macro is missing from libc-shim"
#endif

#ifndef AT_EGID
#error "elf.h:AT_EGID macro is missing from libc-shim"
#endif

#ifndef AT_ENTRY
#error "elf.h:AT_ENTRY macro is missing from libc-shim"
#endif

#ifndef AT_EUID
#error "elf.h:AT_EUID macro is missing from libc-shim"
#endif

#ifndef AT_EXECFD
#error "elf.h:AT_EXECFD macro is missing from libc-shim"
#endif

#ifndef AT_EXECFN
#error "elf.h:AT_EXECFN macro is missing from libc-shim"
#endif

#ifndef AT_FLAGS
#error "elf.h:AT_FLAGS macro is missing from libc-shim"
#endif

#ifndef AT_FPUCW
#error "elf.h:AT_FPUCW macro is missing from libc-shim"
#endif

#ifndef AT_GID
#error "elf.h:AT_GID macro is missing from libc-shim"
#endif

#ifndef AT_HWCAP
#error "elf.h:AT_HWCAP macro is missing from libc-shim"
#endif

#ifndef AT_HWCAP2
#error "elf.h:AT_HWCAP2 macro is missing from libc-shim"
#endif

#ifndef AT_HWCAP3
#error "elf.h:AT_HWCAP3 macro is missing from libc-shim"
#endif

#ifndef AT_HWCAP4
#error "elf.h:AT_HWCAP4 macro is missing from libc-shim"
#endif

#ifndef AT_ICACHEBSIZE
#error "elf.h:AT_ICACHEBSIZE macro is missing from libc-shim"
#endif

#ifndef AT_IGNORE
#error "elf.h:AT_IGNORE macro is missing from libc-shim"
#endif

#ifndef AT_IGNOREPPC
#error "elf.h:AT_IGNOREPPC macro is missing from libc-shim"
#endif

#ifndef AT_L1D_CACHEGEOMETRY
#error "elf.h:AT_L1D_CACHEGEOMETRY macro is missing from libc-shim"
#endif

#ifndef AT_L1D_CACHESHAPE
#error "elf.h:AT_L1D_CACHESHAPE macro is missing from libc-shim"
#endif

#ifndef AT_L1D_CACHESIZE
#error "elf.h:AT_L1D_CACHESIZE macro is missing from libc-shim"
#endif

#ifndef AT_L1I_CACHEGEOMETRY
#error "elf.h:AT_L1I_CACHEGEOMETRY macro is missing from libc-shim"
#endif

#ifndef AT_L1I_CACHESHAPE
#error "elf.h:AT_L1I_CACHESHAPE macro is missing from libc-shim"
#endif

#ifndef AT_L1I_CACHESIZE
#error "elf.h:AT_L1I_CACHESIZE macro is missing from libc-shim"
#endif

#ifndef AT_L2_CACHEGEOMETRY
#error "elf.h:AT_L2_CACHEGEOMETRY macro is missing from libc-shim"
#endif

#ifndef AT_L2_CACHESHAPE
#error "elf.h:AT_L2_CACHESHAPE macro is missing from libc-shim"
#endif

#ifndef AT_L2_CACHESIZE
#error "elf.h:AT_L2_CACHESIZE macro is missing from libc-shim"
#endif

#ifndef AT_L3_CACHEGEOMETRY
#error "elf.h:AT_L3_CACHEGEOMETRY macro is missing from libc-shim"
#endif

#ifndef AT_L3_CACHESHAPE
#error "elf.h:AT_L3_CACHESHAPE macro is missing from libc-shim"
#endif

#ifndef AT_L3_CACHESIZE
#error "elf.h:AT_L3_CACHESIZE macro is missing from libc-shim"
#endif

#ifndef AT_MINSIGSTKSZ
#error "elf.h:AT_MINSIGSTKSZ macro is missing from libc-shim"
#endif

#ifndef AT_NOTELF
#error "elf.h:AT_NOTELF macro is missing from libc-shim"
#endif

#ifndef AT_NULL
#error "elf.h:AT_NULL macro is missing from libc-shim"
#endif

#ifndef AT_PAGESZ
#error "elf.h:AT_PAGESZ macro is missing from libc-shim"
#endif

#ifndef AT_PHDR
#error "elf.h:AT_PHDR macro is missing from libc-shim"
#endif

#ifndef AT_PHENT
#error "elf.h:AT_PHENT macro is missing from libc-shim"
#endif

#ifndef AT_PHNUM
#error "elf.h:AT_PHNUM macro is missing from libc-shim"
#endif

#ifndef AT_PLATFORM
#error "elf.h:AT_PLATFORM macro is missing from libc-shim"
#endif

#ifndef AT_RANDOM
#error "elf.h:AT_RANDOM macro is missing from libc-shim"
#endif

#ifndef AT_RSEQ_ALIGN
#error "elf.h:AT_RSEQ_ALIGN macro is missing from libc-shim"
#endif

#ifndef AT_RSEQ_FEATURE_SIZE
#error "elf.h:AT_RSEQ_FEATURE_SIZE macro is missing from libc-shim"
#endif

#ifndef AT_SECURE
#error "elf.h:AT_SECURE macro is missing from libc-shim"
#endif

#ifndef AT_SYSINFO
#error "elf.h:AT_SYSINFO macro is missing from libc-shim"
#endif

#ifndef AT_SYSINFO_EHDR
#error "elf.h:AT_SYSINFO_EHDR macro is missing from libc-shim"
#endif

#ifndef AT_UCACHEBSIZE
#error "elf.h:AT_UCACHEBSIZE macro is missing from libc-shim"
#endif

#ifndef AT_UID
#error "elf.h:AT_UID macro is missing from libc-shim"
#endif

#ifndef DF_1_CONFALT
#error "elf.h:DF_1_CONFALT macro is missing from libc-shim"
#endif

#ifndef DF_1_DIRECT
#error "elf.h:DF_1_DIRECT macro is missing from libc-shim"
#endif

#ifndef DF_1_DISPRELDNE
#error "elf.h:DF_1_DISPRELDNE macro is missing from libc-shim"
#endif

#ifndef DF_1_DISPRELPND
#error "elf.h:DF_1_DISPRELPND macro is missing from libc-shim"
#endif

#ifndef DF_1_EDITED
#error "elf.h:DF_1_EDITED macro is missing from libc-shim"
#endif

#ifndef DF_1_ENDFILTEE
#error "elf.h:DF_1_ENDFILTEE macro is missing from libc-shim"
#endif

#ifndef DF_1_GLOBAL
#error "elf.h:DF_1_GLOBAL macro is missing from libc-shim"
#endif

#ifndef DF_1_GLOBAUDIT
#error "elf.h:DF_1_GLOBAUDIT macro is missing from libc-shim"
#endif

#ifndef DF_1_GROUP
#error "elf.h:DF_1_GROUP macro is missing from libc-shim"
#endif

#ifndef DF_1_IGNMULDEF
#error "elf.h:DF_1_IGNMULDEF macro is missing from libc-shim"
#endif

#ifndef DF_1_INITFIRST
#error "elf.h:DF_1_INITFIRST macro is missing from libc-shim"
#endif

#ifndef DF_1_INTERPOSE
#error "elf.h:DF_1_INTERPOSE macro is missing from libc-shim"
#endif

#ifndef DF_1_KMOD
#error "elf.h:DF_1_KMOD macro is missing from libc-shim"
#endif

#ifndef DF_1_LOADFLTR
#error "elf.h:DF_1_LOADFLTR macro is missing from libc-shim"
#endif

#ifndef DF_1_NOCOMMON
#error "elf.h:DF_1_NOCOMMON macro is missing from libc-shim"
#endif

#ifndef DF_1_NODEFLIB
#error "elf.h:DF_1_NODEFLIB macro is missing from libc-shim"
#endif

#ifndef DF_1_NODELETE
#error "elf.h:DF_1_NODELETE macro is missing from libc-shim"
#endif

#ifndef DF_1_NODIRECT
#error "elf.h:DF_1_NODIRECT macro is missing from libc-shim"
#endif

#ifndef DF_1_NODUMP
#error "elf.h:DF_1_NODUMP macro is missing from libc-shim"
#endif

#ifndef DF_1_NOHDR
#error "elf.h:DF_1_NOHDR macro is missing from libc-shim"
#endif

#ifndef DF_1_NOKSYMS
#error "elf.h:DF_1_NOKSYMS macro is missing from libc-shim"
#endif

#ifndef DF_1_NOOPEN
#error "elf.h:DF_1_NOOPEN macro is missing from libc-shim"
#endif

#ifndef DF_1_NORELOC
#error "elf.h:DF_1_NORELOC macro is missing from libc-shim"
#endif

#ifndef DF_1_NOW
#error "elf.h:DF_1_NOW macro is missing from libc-shim"
#endif

#ifndef DF_1_ORIGIN
#error "elf.h:DF_1_ORIGIN macro is missing from libc-shim"
#endif

#ifndef DF_1_PIE
#error "elf.h:DF_1_PIE macro is missing from libc-shim"
#endif

#ifndef DF_1_SINGLETON
#error "elf.h:DF_1_SINGLETON macro is missing from libc-shim"
#endif

#ifndef DF_1_STUB
#error "elf.h:DF_1_STUB macro is missing from libc-shim"
#endif

#ifndef DF_1_SYMINTPOSE
#error "elf.h:DF_1_SYMINTPOSE macro is missing from libc-shim"
#endif

#ifndef DF_1_TRANS
#error "elf.h:DF_1_TRANS macro is missing from libc-shim"
#endif

#ifndef DF_1_WEAKFILTER
#error "elf.h:DF_1_WEAKFILTER macro is missing from libc-shim"
#endif

#ifndef DF_BIND_NOW
#error "elf.h:DF_BIND_NOW macro is missing from libc-shim"
#endif

#ifndef DF_ORIGIN
#error "elf.h:DF_ORIGIN macro is missing from libc-shim"
#endif

#ifndef DF_P1_GROUPPERM
#error "elf.h:DF_P1_GROUPPERM macro is missing from libc-shim"
#endif

#ifndef DF_P1_LAZYLOAD
#error "elf.h:DF_P1_LAZYLOAD macro is missing from libc-shim"
#endif

#ifndef DF_STATIC_TLS
#error "elf.h:DF_STATIC_TLS macro is missing from libc-shim"
#endif

#ifndef DF_SYMBOLIC
#error "elf.h:DF_SYMBOLIC macro is missing from libc-shim"
#endif

#ifndef DF_TEXTREL
#error "elf.h:DF_TEXTREL macro is missing from libc-shim"
#endif

#ifndef DTF_1_CONFEXP
#error "elf.h:DTF_1_CONFEXP macro is missing from libc-shim"
#endif

#ifndef DTF_1_PARINIT
#error "elf.h:DTF_1_PARINIT macro is missing from libc-shim"
#endif

#ifndef DT_AARCH64_BTI_PLT
#error "elf.h:DT_AARCH64_BTI_PLT macro is missing from libc-shim"
#endif

#ifndef DT_AARCH64_NUM
#error "elf.h:DT_AARCH64_NUM macro is missing from libc-shim"
#endif

#ifndef DT_AARCH64_PAC_PLT
#error "elf.h:DT_AARCH64_PAC_PLT macro is missing from libc-shim"
#endif

#ifndef DT_AARCH64_VARIANT_PCS
#error "elf.h:DT_AARCH64_VARIANT_PCS macro is missing from libc-shim"
#endif

#ifndef DT_ADDRNUM
#error "elf.h:DT_ADDRNUM macro is missing from libc-shim"
#endif

#ifndef DT_ADDRRNGHI
#error "elf.h:DT_ADDRRNGHI macro is missing from libc-shim"
#endif

#ifndef DT_ADDRRNGLO
#error "elf.h:DT_ADDRRNGLO macro is missing from libc-shim"
#endif

#ifndef DT_ADDRTAGIDX
#error "elf.h:DT_ADDRTAGIDX macro is missing from libc-shim"
#endif

#ifndef DT_ALPHA_NUM
#error "elf.h:DT_ALPHA_NUM macro is missing from libc-shim"
#endif

#ifndef DT_ALPHA_PLTRO
#error "elf.h:DT_ALPHA_PLTRO macro is missing from libc-shim"
#endif

#ifndef DT_AUDIT
#error "elf.h:DT_AUDIT macro is missing from libc-shim"
#endif

#ifndef DT_AUXILIARY
#error "elf.h:DT_AUXILIARY macro is missing from libc-shim"
#endif

#ifndef DT_BIND_NOW
#error "elf.h:DT_BIND_NOW macro is missing from libc-shim"
#endif

#ifndef DT_CHECKSUM
#error "elf.h:DT_CHECKSUM macro is missing from libc-shim"
#endif

#ifndef DT_CONFIG
#error "elf.h:DT_CONFIG macro is missing from libc-shim"
#endif

#ifndef DT_DEBUG
#error "elf.h:DT_DEBUG macro is missing from libc-shim"
#endif

#ifndef DT_DEPAUDIT
#error "elf.h:DT_DEPAUDIT macro is missing from libc-shim"
#endif

#ifndef DT_ENCODING
#error "elf.h:DT_ENCODING macro is missing from libc-shim"
#endif

#ifndef DT_EXTRANUM
#error "elf.h:DT_EXTRANUM macro is missing from libc-shim"
#endif

#ifndef DT_EXTRATAGIDX
#error "elf.h:DT_EXTRATAGIDX macro is missing from libc-shim"
#endif

#ifndef DT_FEATURE_1
#error "elf.h:DT_FEATURE_1 macro is missing from libc-shim"
#endif

#ifndef DT_FILTER
#error "elf.h:DT_FILTER macro is missing from libc-shim"
#endif

#ifndef DT_FINI
#error "elf.h:DT_FINI macro is missing from libc-shim"
#endif

#ifndef DT_FINI_ARRAY
#error "elf.h:DT_FINI_ARRAY macro is missing from libc-shim"
#endif

#ifndef DT_FINI_ARRAYSZ
#error "elf.h:DT_FINI_ARRAYSZ macro is missing from libc-shim"
#endif

#ifndef DT_FLAGS
#error "elf.h:DT_FLAGS macro is missing from libc-shim"
#endif

#ifndef DT_FLAGS_1
#error "elf.h:DT_FLAGS_1 macro is missing from libc-shim"
#endif

#ifndef DT_GNU_CONFLICT
#error "elf.h:DT_GNU_CONFLICT macro is missing from libc-shim"
#endif

#ifndef DT_GNU_CONFLICTSZ
#error "elf.h:DT_GNU_CONFLICTSZ macro is missing from libc-shim"
#endif

#ifndef DT_GNU_HASH
#error "elf.h:DT_GNU_HASH macro is missing from libc-shim"
#endif

#ifndef DT_GNU_LIBLIST
#error "elf.h:DT_GNU_LIBLIST macro is missing from libc-shim"
#endif

#ifndef DT_GNU_LIBLISTSZ
#error "elf.h:DT_GNU_LIBLISTSZ macro is missing from libc-shim"
#endif

#ifndef DT_GNU_PRELINKED
#error "elf.h:DT_GNU_PRELINKED macro is missing from libc-shim"
#endif

#ifndef DT_HASH
#error "elf.h:DT_HASH macro is missing from libc-shim"
#endif

#ifndef DT_HIOS
#error "elf.h:DT_HIOS macro is missing from libc-shim"
#endif

#ifndef DT_HIPROC
#error "elf.h:DT_HIPROC macro is missing from libc-shim"
#endif

#ifndef DT_IA_64_NUM
#error "elf.h:DT_IA_64_NUM macro is missing from libc-shim"
#endif

#ifndef DT_IA_64_PLT_RESERVE
#error "elf.h:DT_IA_64_PLT_RESERVE macro is missing from libc-shim"
#endif

#ifndef DT_INIT
#error "elf.h:DT_INIT macro is missing from libc-shim"
#endif

#ifndef DT_INIT_ARRAY
#error "elf.h:DT_INIT_ARRAY macro is missing from libc-shim"
#endif

#ifndef DT_INIT_ARRAYSZ
#error "elf.h:DT_INIT_ARRAYSZ macro is missing from libc-shim"
#endif

#ifndef DT_JMPREL
#error "elf.h:DT_JMPREL macro is missing from libc-shim"
#endif

#ifndef DT_LOOS
#error "elf.h:DT_LOOS macro is missing from libc-shim"
#endif

#ifndef DT_LOPROC
#error "elf.h:DT_LOPROC macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_AUX_DYNAMIC
#error "elf.h:DT_MIPS_AUX_DYNAMIC macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_BASE_ADDRESS
#error "elf.h:DT_MIPS_BASE_ADDRESS macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_COMPACT_SIZE
#error "elf.h:DT_MIPS_COMPACT_SIZE macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_CONFLICT
#error "elf.h:DT_MIPS_CONFLICT macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_CONFLICTNO
#error "elf.h:DT_MIPS_CONFLICTNO macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_CXX_FLAGS
#error "elf.h:DT_MIPS_CXX_FLAGS macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_DELTA_CLASS
#error "elf.h:DT_MIPS_DELTA_CLASS macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_DELTA_CLASSSYM
#error "elf.h:DT_MIPS_DELTA_CLASSSYM macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_DELTA_CLASSSYM_NO
#error "elf.h:DT_MIPS_DELTA_CLASSSYM_NO macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_DELTA_CLASS_NO
#error "elf.h:DT_MIPS_DELTA_CLASS_NO macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_DELTA_INSTANCE
#error "elf.h:DT_MIPS_DELTA_INSTANCE macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_DELTA_INSTANCE_NO
#error "elf.h:DT_MIPS_DELTA_INSTANCE_NO macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_DELTA_RELOC
#error "elf.h:DT_MIPS_DELTA_RELOC macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_DELTA_RELOC_NO
#error "elf.h:DT_MIPS_DELTA_RELOC_NO macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_DELTA_SYM
#error "elf.h:DT_MIPS_DELTA_SYM macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_DELTA_SYM_NO
#error "elf.h:DT_MIPS_DELTA_SYM_NO macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_DYNSTR_ALIGN
#error "elf.h:DT_MIPS_DYNSTR_ALIGN macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_FLAGS
#error "elf.h:DT_MIPS_FLAGS macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_GOTSYM
#error "elf.h:DT_MIPS_GOTSYM macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_GP_VALUE
#error "elf.h:DT_MIPS_GP_VALUE macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_HIDDEN_GOTIDX
#error "elf.h:DT_MIPS_HIDDEN_GOTIDX macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_HIPAGENO
#error "elf.h:DT_MIPS_HIPAGENO macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_ICHECKSUM
#error "elf.h:DT_MIPS_ICHECKSUM macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_INTERFACE
#error "elf.h:DT_MIPS_INTERFACE macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_INTERFACE_SIZE
#error "elf.h:DT_MIPS_INTERFACE_SIZE macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_IVERSION
#error "elf.h:DT_MIPS_IVERSION macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_LIBLIST
#error "elf.h:DT_MIPS_LIBLIST macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_LIBLISTNO
#error "elf.h:DT_MIPS_LIBLISTNO macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_LOCALPAGE_GOTIDX
#error "elf.h:DT_MIPS_LOCALPAGE_GOTIDX macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_LOCAL_GOTIDX
#error "elf.h:DT_MIPS_LOCAL_GOTIDX macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_LOCAL_GOTNO
#error "elf.h:DT_MIPS_LOCAL_GOTNO macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_MSYM
#error "elf.h:DT_MIPS_MSYM macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_NUM
#error "elf.h:DT_MIPS_NUM macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_OPTIONS
#error "elf.h:DT_MIPS_OPTIONS macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_PERF_SUFFIX
#error "elf.h:DT_MIPS_PERF_SUFFIX macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_PIXIE_INIT
#error "elf.h:DT_MIPS_PIXIE_INIT macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_PLTGOT
#error "elf.h:DT_MIPS_PLTGOT macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_PROTECTED_GOTIDX
#error "elf.h:DT_MIPS_PROTECTED_GOTIDX macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_RLD_MAP
#error "elf.h:DT_MIPS_RLD_MAP macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_RLD_MAP_REL
#error "elf.h:DT_MIPS_RLD_MAP_REL macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_RLD_TEXT_RESOLVE_ADDR
#error "elf.h:DT_MIPS_RLD_TEXT_RESOLVE_ADDR macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_RLD_VERSION
#error "elf.h:DT_MIPS_RLD_VERSION macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_RWPLT
#error "elf.h:DT_MIPS_RWPLT macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_SYMBOL_LIB
#error "elf.h:DT_MIPS_SYMBOL_LIB macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_SYMTABNO
#error "elf.h:DT_MIPS_SYMTABNO macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_TIME_STAMP
#error "elf.h:DT_MIPS_TIME_STAMP macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_UNREFEXTNO
#error "elf.h:DT_MIPS_UNREFEXTNO macro is missing from libc-shim"
#endif

#ifndef DT_MIPS_XHASH
#error "elf.h:DT_MIPS_XHASH macro is missing from libc-shim"
#endif

#ifndef DT_MOVEENT
#error "elf.h:DT_MOVEENT macro is missing from libc-shim"
#endif

#ifndef DT_MOVESZ
#error "elf.h:DT_MOVESZ macro is missing from libc-shim"
#endif

#ifndef DT_MOVETAB
#error "elf.h:DT_MOVETAB macro is missing from libc-shim"
#endif

#ifndef DT_NEEDED
#error "elf.h:DT_NEEDED macro is missing from libc-shim"
#endif

#ifndef DT_NIOS2_GP
#error "elf.h:DT_NIOS2_GP macro is missing from libc-shim"
#endif

#ifndef DT_NULL
#error "elf.h:DT_NULL macro is missing from libc-shim"
#endif

#ifndef DT_NUM
#error "elf.h:DT_NUM macro is missing from libc-shim"
#endif

#ifndef DT_PLTGOT
#error "elf.h:DT_PLTGOT macro is missing from libc-shim"
#endif

#ifndef DT_PLTPAD
#error "elf.h:DT_PLTPAD macro is missing from libc-shim"
#endif

#ifndef DT_PLTPADSZ
#error "elf.h:DT_PLTPADSZ macro is missing from libc-shim"
#endif

#ifndef DT_PLTREL
#error "elf.h:DT_PLTREL macro is missing from libc-shim"
#endif

#ifndef DT_PLTRELSZ
#error "elf.h:DT_PLTRELSZ macro is missing from libc-shim"
#endif

#ifndef DT_POSFLAG_1
#error "elf.h:DT_POSFLAG_1 macro is missing from libc-shim"
#endif

#ifndef DT_PPC64_GLINK
#error "elf.h:DT_PPC64_GLINK macro is missing from libc-shim"
#endif

#ifndef DT_PPC64_NUM
#error "elf.h:DT_PPC64_NUM macro is missing from libc-shim"
#endif

#ifndef DT_PPC64_OPD
#error "elf.h:DT_PPC64_OPD macro is missing from libc-shim"
#endif

#ifndef DT_PPC64_OPDSZ
#error "elf.h:DT_PPC64_OPDSZ macro is missing from libc-shim"
#endif

#ifndef DT_PPC64_OPT
#error "elf.h:DT_PPC64_OPT macro is missing from libc-shim"
#endif

#ifndef DT_PPC_GOT
#error "elf.h:DT_PPC_GOT macro is missing from libc-shim"
#endif

#ifndef DT_PPC_NUM
#error "elf.h:DT_PPC_NUM macro is missing from libc-shim"
#endif

#ifndef DT_PPC_OPT
#error "elf.h:DT_PPC_OPT macro is missing from libc-shim"
#endif

#ifndef DT_PREINIT_ARRAY
#error "elf.h:DT_PREINIT_ARRAY macro is missing from libc-shim"
#endif

#ifndef DT_PREINIT_ARRAYSZ
#error "elf.h:DT_PREINIT_ARRAYSZ macro is missing from libc-shim"
#endif

#ifndef DT_PROCNUM
#error "elf.h:DT_PROCNUM macro is missing from libc-shim"
#endif

#ifndef DT_REL
#error "elf.h:DT_REL macro is missing from libc-shim"
#endif

#ifndef DT_RELA
#error "elf.h:DT_RELA macro is missing from libc-shim"
#endif

#ifndef DT_RELACOUNT
#error "elf.h:DT_RELACOUNT macro is missing from libc-shim"
#endif

#ifndef DT_RELAENT
#error "elf.h:DT_RELAENT macro is missing from libc-shim"
#endif

#ifndef DT_RELASZ
#error "elf.h:DT_RELASZ macro is missing from libc-shim"
#endif

#ifndef DT_RELCOUNT
#error "elf.h:DT_RELCOUNT macro is missing from libc-shim"
#endif

#ifndef DT_RELENT
#error "elf.h:DT_RELENT macro is missing from libc-shim"
#endif

#ifndef DT_RELR
#error "elf.h:DT_RELR macro is missing from libc-shim"
#endif

#ifndef DT_RELRENT
#error "elf.h:DT_RELRENT macro is missing from libc-shim"
#endif

#ifndef DT_RELRSZ
#error "elf.h:DT_RELRSZ macro is missing from libc-shim"
#endif

#ifndef DT_RELSZ
#error "elf.h:DT_RELSZ macro is missing from libc-shim"
#endif

#ifndef DT_RISCV_VARIANT_CC
#error "elf.h:DT_RISCV_VARIANT_CC macro is missing from libc-shim"
#endif

#ifndef DT_RPATH
#error "elf.h:DT_RPATH macro is missing from libc-shim"
#endif

#ifndef DT_RUNPATH
#error "elf.h:DT_RUNPATH macro is missing from libc-shim"
#endif

#ifndef DT_SONAME
#error "elf.h:DT_SONAME macro is missing from libc-shim"
#endif

#ifndef DT_SPARC_NUM
#error "elf.h:DT_SPARC_NUM macro is missing from libc-shim"
#endif

#ifndef DT_SPARC_REGISTER
#error "elf.h:DT_SPARC_REGISTER macro is missing from libc-shim"
#endif

#ifndef DT_STRSZ
#error "elf.h:DT_STRSZ macro is missing from libc-shim"
#endif

#ifndef DT_STRTAB
#error "elf.h:DT_STRTAB macro is missing from libc-shim"
#endif

#ifndef DT_SYMBOLIC
#error "elf.h:DT_SYMBOLIC macro is missing from libc-shim"
#endif

#ifndef DT_SYMENT
#error "elf.h:DT_SYMENT macro is missing from libc-shim"
#endif

#ifndef DT_SYMINENT
#error "elf.h:DT_SYMINENT macro is missing from libc-shim"
#endif

#ifndef DT_SYMINFO
#error "elf.h:DT_SYMINFO macro is missing from libc-shim"
#endif

#ifndef DT_SYMINSZ
#error "elf.h:DT_SYMINSZ macro is missing from libc-shim"
#endif

#ifndef DT_SYMTAB
#error "elf.h:DT_SYMTAB macro is missing from libc-shim"
#endif

#ifndef DT_SYMTAB_SHNDX
#error "elf.h:DT_SYMTAB_SHNDX macro is missing from libc-shim"
#endif

#ifndef DT_TEXTREL
#error "elf.h:DT_TEXTREL macro is missing from libc-shim"
#endif

#ifndef DT_TLSDESC_GOT
#error "elf.h:DT_TLSDESC_GOT macro is missing from libc-shim"
#endif

#ifndef DT_TLSDESC_PLT
#error "elf.h:DT_TLSDESC_PLT macro is missing from libc-shim"
#endif

#ifndef DT_VALNUM
#error "elf.h:DT_VALNUM macro is missing from libc-shim"
#endif

#ifndef DT_VALRNGHI
#error "elf.h:DT_VALRNGHI macro is missing from libc-shim"
#endif

#ifndef DT_VALRNGLO
#error "elf.h:DT_VALRNGLO macro is missing from libc-shim"
#endif

#ifndef DT_VALTAGIDX
#error "elf.h:DT_VALTAGIDX macro is missing from libc-shim"
#endif

#ifndef DT_VERDEF
#error "elf.h:DT_VERDEF macro is missing from libc-shim"
#endif

#ifndef DT_VERDEFNUM
#error "elf.h:DT_VERDEFNUM macro is missing from libc-shim"
#endif

#ifndef DT_VERNEED
#error "elf.h:DT_VERNEED macro is missing from libc-shim"
#endif

#ifndef DT_VERNEEDNUM
#error "elf.h:DT_VERNEEDNUM macro is missing from libc-shim"
#endif

#ifndef DT_VERSIONTAGIDX
#error "elf.h:DT_VERSIONTAGIDX macro is missing from libc-shim"
#endif

#ifndef DT_VERSIONTAGNUM
#error "elf.h:DT_VERSIONTAGNUM macro is missing from libc-shim"
#endif

#ifndef DT_VERSYM
#error "elf.h:DT_VERSYM macro is missing from libc-shim"
#endif

#ifndef DT_X86_64_NUM
#error "elf.h:DT_X86_64_NUM macro is missing from libc-shim"
#endif

#ifndef DT_X86_64_PLT
#error "elf.h:DT_X86_64_PLT macro is missing from libc-shim"
#endif

#ifndef DT_X86_64_PLTENT
#error "elf.h:DT_X86_64_PLTENT macro is missing from libc-shim"
#endif

#ifndef DT_X86_64_PLTSZ
#error "elf.h:DT_X86_64_PLTSZ macro is missing from libc-shim"
#endif

#ifndef EFA_PARISC_1_0
#error "elf.h:EFA_PARISC_1_0 macro is missing from libc-shim"
#endif

#ifndef EFA_PARISC_1_1
#error "elf.h:EFA_PARISC_1_1 macro is missing from libc-shim"
#endif

#ifndef EFA_PARISC_2_0
#error "elf.h:EFA_PARISC_2_0 macro is missing from libc-shim"
#endif

#ifndef EF_ALPHA_32BIT
#error "elf.h:EF_ALPHA_32BIT macro is missing from libc-shim"
#endif

#ifndef EF_ALPHA_CANRELAX
#error "elf.h:EF_ALPHA_CANRELAX macro is missing from libc-shim"
#endif

#ifndef EF_ARC_ALL_MSK
#error "elf.h:EF_ARC_ALL_MSK macro is missing from libc-shim"
#endif

#ifndef EF_ARC_MACH_MSK
#error "elf.h:EF_ARC_MACH_MSK macro is missing from libc-shim"
#endif

#ifndef EF_ARC_OSABI_MSK
#error "elf.h:EF_ARC_OSABI_MSK macro is missing from libc-shim"
#endif

#ifndef EF_ARM_ABI_FLOAT_HARD
#error "elf.h:EF_ARM_ABI_FLOAT_HARD macro is missing from libc-shim"
#endif

#ifndef EF_ARM_ABI_FLOAT_SOFT
#error "elf.h:EF_ARM_ABI_FLOAT_SOFT macro is missing from libc-shim"
#endif

#ifndef EF_ARM_ALIGN8
#error "elf.h:EF_ARM_ALIGN8 macro is missing from libc-shim"
#endif

#ifndef EF_ARM_APCS_26
#error "elf.h:EF_ARM_APCS_26 macro is missing from libc-shim"
#endif

#ifndef EF_ARM_APCS_FLOAT
#error "elf.h:EF_ARM_APCS_FLOAT macro is missing from libc-shim"
#endif

#ifndef EF_ARM_BE8
#error "elf.h:EF_ARM_BE8 macro is missing from libc-shim"
#endif

#ifndef EF_ARM_DYNSYMSUSESEGIDX
#error "elf.h:EF_ARM_DYNSYMSUSESEGIDX macro is missing from libc-shim"
#endif

#ifndef EF_ARM_EABIMASK
#error "elf.h:EF_ARM_EABIMASK macro is missing from libc-shim"
#endif

#ifndef EF_ARM_EABI_UNKNOWN
#error "elf.h:EF_ARM_EABI_UNKNOWN macro is missing from libc-shim"
#endif

#ifndef EF_ARM_EABI_VER1
#error "elf.h:EF_ARM_EABI_VER1 macro is missing from libc-shim"
#endif

#ifndef EF_ARM_EABI_VER2
#error "elf.h:EF_ARM_EABI_VER2 macro is missing from libc-shim"
#endif

#ifndef EF_ARM_EABI_VER3
#error "elf.h:EF_ARM_EABI_VER3 macro is missing from libc-shim"
#endif

#ifndef EF_ARM_EABI_VER4
#error "elf.h:EF_ARM_EABI_VER4 macro is missing from libc-shim"
#endif

#ifndef EF_ARM_EABI_VER5
#error "elf.h:EF_ARM_EABI_VER5 macro is missing from libc-shim"
#endif

#ifndef EF_ARM_EABI_VERSION
#error "elf.h:EF_ARM_EABI_VERSION macro is missing from libc-shim"
#endif

#ifndef EF_ARM_HASENTRY
#error "elf.h:EF_ARM_HASENTRY macro is missing from libc-shim"
#endif

#ifndef EF_ARM_INTERWORK
#error "elf.h:EF_ARM_INTERWORK macro is missing from libc-shim"
#endif

#ifndef EF_ARM_LE8
#error "elf.h:EF_ARM_LE8 macro is missing from libc-shim"
#endif

#ifndef EF_ARM_MAPSYMSFIRST
#error "elf.h:EF_ARM_MAPSYMSFIRST macro is missing from libc-shim"
#endif

#ifndef EF_ARM_MAVERICK_FLOAT
#error "elf.h:EF_ARM_MAVERICK_FLOAT macro is missing from libc-shim"
#endif

#ifndef EF_ARM_NEW_ABI
#error "elf.h:EF_ARM_NEW_ABI macro is missing from libc-shim"
#endif

#ifndef EF_ARM_OLD_ABI
#error "elf.h:EF_ARM_OLD_ABI macro is missing from libc-shim"
#endif

#ifndef EF_ARM_PIC
#error "elf.h:EF_ARM_PIC macro is missing from libc-shim"
#endif

#ifndef EF_ARM_RELEXEC
#error "elf.h:EF_ARM_RELEXEC macro is missing from libc-shim"
#endif

#ifndef EF_ARM_SOFT_FLOAT
#error "elf.h:EF_ARM_SOFT_FLOAT macro is missing from libc-shim"
#endif

#ifndef EF_ARM_SYMSARESORTED
#error "elf.h:EF_ARM_SYMSARESORTED macro is missing from libc-shim"
#endif

#ifndef EF_ARM_VFP_FLOAT
#error "elf.h:EF_ARM_VFP_FLOAT macro is missing from libc-shim"
#endif

#ifndef EF_CPU32
#error "elf.h:EF_CPU32 macro is missing from libc-shim"
#endif

#ifndef EF_CSKY_ABIMASK
#error "elf.h:EF_CSKY_ABIMASK macro is missing from libc-shim"
#endif

#ifndef EF_CSKY_ABIV1
#error "elf.h:EF_CSKY_ABIV1 macro is missing from libc-shim"
#endif

#ifndef EF_CSKY_ABIV2
#error "elf.h:EF_CSKY_ABIV2 macro is missing from libc-shim"
#endif

#ifndef EF_CSKY_OTHER
#error "elf.h:EF_CSKY_OTHER macro is missing from libc-shim"
#endif

#ifndef EF_CSKY_PROCESSOR
#error "elf.h:EF_CSKY_PROCESSOR macro is missing from libc-shim"
#endif

#ifndef EF_IA_64_ABI64
#error "elf.h:EF_IA_64_ABI64 macro is missing from libc-shim"
#endif

#ifndef EF_IA_64_ARCH
#error "elf.h:EF_IA_64_ARCH macro is missing from libc-shim"
#endif

#ifndef EF_IA_64_MASKOS
#error "elf.h:EF_IA_64_MASKOS macro is missing from libc-shim"
#endif

#ifndef EF_LARCH_ABI_DOUBLE_FLOAT
#error "elf.h:EF_LARCH_ABI_DOUBLE_FLOAT macro is missing from libc-shim"
#endif

#ifndef EF_LARCH_ABI_MODIFIER_MASK
#error "elf.h:EF_LARCH_ABI_MODIFIER_MASK macro is missing from libc-shim"
#endif

#ifndef EF_LARCH_ABI_SINGLE_FLOAT
#error "elf.h:EF_LARCH_ABI_SINGLE_FLOAT macro is missing from libc-shim"
#endif

#ifndef EF_LARCH_ABI_SOFT_FLOAT
#error "elf.h:EF_LARCH_ABI_SOFT_FLOAT macro is missing from libc-shim"
#endif

#ifndef EF_LARCH_OBJABI_V1
#error "elf.h:EF_LARCH_OBJABI_V1 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_32BITMODE
#error "elf.h:EF_MIPS_32BITMODE macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ABI
#error "elf.h:EF_MIPS_ABI macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ABI2
#error "elf.h:EF_MIPS_ABI2 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ABI_EABI32
#error "elf.h:EF_MIPS_ABI_EABI32 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ABI_EABI64
#error "elf.h:EF_MIPS_ABI_EABI64 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ABI_O32
#error "elf.h:EF_MIPS_ABI_O32 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ABI_O64
#error "elf.h:EF_MIPS_ABI_O64 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ABI_ON32
#error "elf.h:EF_MIPS_ABI_ON32 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ARCH
#error "elf.h:EF_MIPS_ARCH macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ARCH_1
#error "elf.h:EF_MIPS_ARCH_1 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ARCH_2
#error "elf.h:EF_MIPS_ARCH_2 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ARCH_3
#error "elf.h:EF_MIPS_ARCH_3 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ARCH_32
#error "elf.h:EF_MIPS_ARCH_32 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ARCH_32R2
#error "elf.h:EF_MIPS_ARCH_32R2 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ARCH_32R6
#error "elf.h:EF_MIPS_ARCH_32R6 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ARCH_4
#error "elf.h:EF_MIPS_ARCH_4 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ARCH_5
#error "elf.h:EF_MIPS_ARCH_5 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ARCH_64
#error "elf.h:EF_MIPS_ARCH_64 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ARCH_64R2
#error "elf.h:EF_MIPS_ARCH_64R2 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ARCH_64R6
#error "elf.h:EF_MIPS_ARCH_64R6 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ARCH_ASE
#error "elf.h:EF_MIPS_ARCH_ASE macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ARCH_ASE_M16
#error "elf.h:EF_MIPS_ARCH_ASE_M16 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ARCH_ASE_MDMX
#error "elf.h:EF_MIPS_ARCH_ASE_MDMX macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_ARCH_ASE_MICROMIPS
#error "elf.h:EF_MIPS_ARCH_ASE_MICROMIPS macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_CPIC
#error "elf.h:EF_MIPS_CPIC macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_FP64
#error "elf.h:EF_MIPS_FP64 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH
#error "elf.h:EF_MIPS_MACH macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_3900
#error "elf.h:EF_MIPS_MACH_3900 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_4010
#error "elf.h:EF_MIPS_MACH_4010 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_4100
#error "elf.h:EF_MIPS_MACH_4100 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_4111
#error "elf.h:EF_MIPS_MACH_4111 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_4120
#error "elf.h:EF_MIPS_MACH_4120 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_4650
#error "elf.h:EF_MIPS_MACH_4650 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_5400
#error "elf.h:EF_MIPS_MACH_5400 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_5500
#error "elf.h:EF_MIPS_MACH_5500 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_5900
#error "elf.h:EF_MIPS_MACH_5900 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_9000
#error "elf.h:EF_MIPS_MACH_9000 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_ALLEGREX
#error "elf.h:EF_MIPS_MACH_ALLEGREX macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_GS264E
#error "elf.h:EF_MIPS_MACH_GS264E macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_GS464
#error "elf.h:EF_MIPS_MACH_GS464 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_GS464E
#error "elf.h:EF_MIPS_MACH_GS464E macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_IAMR2
#error "elf.h:EF_MIPS_MACH_IAMR2 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_LS2E
#error "elf.h:EF_MIPS_MACH_LS2E macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_LS2F
#error "elf.h:EF_MIPS_MACH_LS2F macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_OCTEON
#error "elf.h:EF_MIPS_MACH_OCTEON macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_OCTEON2
#error "elf.h:EF_MIPS_MACH_OCTEON2 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_OCTEON3
#error "elf.h:EF_MIPS_MACH_OCTEON3 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_SB1
#error "elf.h:EF_MIPS_MACH_SB1 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_MACH_XLR
#error "elf.h:EF_MIPS_MACH_XLR macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_NAN2008
#error "elf.h:EF_MIPS_NAN2008 macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_NOREORDER
#error "elf.h:EF_MIPS_NOREORDER macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_OPTIONS_FIRST
#error "elf.h:EF_MIPS_OPTIONS_FIRST macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_PIC
#error "elf.h:EF_MIPS_PIC macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_UCODE
#error "elf.h:EF_MIPS_UCODE macro is missing from libc-shim"
#endif

#ifndef EF_MIPS_XGOT
#error "elf.h:EF_MIPS_XGOT macro is missing from libc-shim"
#endif

#ifndef EF_PARISC_ARCH
#error "elf.h:EF_PARISC_ARCH macro is missing from libc-shim"
#endif

#ifndef EF_PARISC_EXT
#error "elf.h:EF_PARISC_EXT macro is missing from libc-shim"
#endif

#ifndef EF_PARISC_LAZYSWAP
#error "elf.h:EF_PARISC_LAZYSWAP macro is missing from libc-shim"
#endif

#ifndef EF_PARISC_LSB
#error "elf.h:EF_PARISC_LSB macro is missing from libc-shim"
#endif

#ifndef EF_PARISC_NO_KABP
#error "elf.h:EF_PARISC_NO_KABP macro is missing from libc-shim"
#endif

#ifndef EF_PARISC_TRAPNIL
#error "elf.h:EF_PARISC_TRAPNIL macro is missing from libc-shim"
#endif

#ifndef EF_PARISC_WIDE
#error "elf.h:EF_PARISC_WIDE macro is missing from libc-shim"
#endif

#ifndef EF_PPC64_ABI
#error "elf.h:EF_PPC64_ABI macro is missing from libc-shim"
#endif

#ifndef EF_PPC_EMB
#error "elf.h:EF_PPC_EMB macro is missing from libc-shim"
#endif

#ifndef EF_PPC_RELOCATABLE
#error "elf.h:EF_PPC_RELOCATABLE macro is missing from libc-shim"
#endif

#ifndef EF_PPC_RELOCATABLE_LIB
#error "elf.h:EF_PPC_RELOCATABLE_LIB macro is missing from libc-shim"
#endif

#ifndef EF_RISCV_FLOAT_ABI
#error "elf.h:EF_RISCV_FLOAT_ABI macro is missing from libc-shim"
#endif

#ifndef EF_RISCV_FLOAT_ABI_DOUBLE
#error "elf.h:EF_RISCV_FLOAT_ABI_DOUBLE macro is missing from libc-shim"
#endif

#ifndef EF_RISCV_FLOAT_ABI_QUAD
#error "elf.h:EF_RISCV_FLOAT_ABI_QUAD macro is missing from libc-shim"
#endif

#ifndef EF_RISCV_FLOAT_ABI_SINGLE
#error "elf.h:EF_RISCV_FLOAT_ABI_SINGLE macro is missing from libc-shim"
#endif

#ifndef EF_RISCV_FLOAT_ABI_SOFT
#error "elf.h:EF_RISCV_FLOAT_ABI_SOFT macro is missing from libc-shim"
#endif

#ifndef EF_RISCV_RVC
#error "elf.h:EF_RISCV_RVC macro is missing from libc-shim"
#endif

#ifndef EF_RISCV_RVE
#error "elf.h:EF_RISCV_RVE macro is missing from libc-shim"
#endif

#ifndef EF_RISCV_TSO
#error "elf.h:EF_RISCV_TSO macro is missing from libc-shim"
#endif

#ifndef EF_S390_HIGH_GPRS
#error "elf.h:EF_S390_HIGH_GPRS macro is missing from libc-shim"
#endif

#ifndef EF_SH1
#error "elf.h:EF_SH1 macro is missing from libc-shim"
#endif

#ifndef EF_SH2
#error "elf.h:EF_SH2 macro is missing from libc-shim"
#endif

#ifndef EF_SH2A
#error "elf.h:EF_SH2A macro is missing from libc-shim"
#endif

#ifndef EF_SH2A_NOFPU
#error "elf.h:EF_SH2A_NOFPU macro is missing from libc-shim"
#endif

#ifndef EF_SH2A_SH3E
#error "elf.h:EF_SH2A_SH3E macro is missing from libc-shim"
#endif

#ifndef EF_SH2A_SH3_NOFPU
#error "elf.h:EF_SH2A_SH3_NOFPU macro is missing from libc-shim"
#endif

#ifndef EF_SH2A_SH4
#error "elf.h:EF_SH2A_SH4 macro is missing from libc-shim"
#endif

#ifndef EF_SH2A_SH4_NOFPU
#error "elf.h:EF_SH2A_SH4_NOFPU macro is missing from libc-shim"
#endif

#ifndef EF_SH2E
#error "elf.h:EF_SH2E macro is missing from libc-shim"
#endif

#ifndef EF_SH3
#error "elf.h:EF_SH3 macro is missing from libc-shim"
#endif

#ifndef EF_SH3E
#error "elf.h:EF_SH3E macro is missing from libc-shim"
#endif

#ifndef EF_SH3_DSP
#error "elf.h:EF_SH3_DSP macro is missing from libc-shim"
#endif

#ifndef EF_SH3_NOMMU
#error "elf.h:EF_SH3_NOMMU macro is missing from libc-shim"
#endif

#ifndef EF_SH4
#error "elf.h:EF_SH4 macro is missing from libc-shim"
#endif

#ifndef EF_SH4A
#error "elf.h:EF_SH4A macro is missing from libc-shim"
#endif

#ifndef EF_SH4AL_DSP
#error "elf.h:EF_SH4AL_DSP macro is missing from libc-shim"
#endif

#ifndef EF_SH4A_NOFPU
#error "elf.h:EF_SH4A_NOFPU macro is missing from libc-shim"
#endif

#ifndef EF_SH4_NOFPU
#error "elf.h:EF_SH4_NOFPU macro is missing from libc-shim"
#endif

#ifndef EF_SH4_NOMMU_NOFPU
#error "elf.h:EF_SH4_NOMMU_NOFPU macro is missing from libc-shim"
#endif

#ifndef EF_SH_DSP
#error "elf.h:EF_SH_DSP macro is missing from libc-shim"
#endif

#ifndef EF_SH_MACH_MASK
#error "elf.h:EF_SH_MACH_MASK macro is missing from libc-shim"
#endif

#ifndef EF_SH_UNKNOWN
#error "elf.h:EF_SH_UNKNOWN macro is missing from libc-shim"
#endif

#ifndef EF_SPARCV9_MM
#error "elf.h:EF_SPARCV9_MM macro is missing from libc-shim"
#endif

#ifndef EF_SPARCV9_PSO
#error "elf.h:EF_SPARCV9_PSO macro is missing from libc-shim"
#endif

#ifndef EF_SPARCV9_RMO
#error "elf.h:EF_SPARCV9_RMO macro is missing from libc-shim"
#endif

#ifndef EF_SPARCV9_TSO
#error "elf.h:EF_SPARCV9_TSO macro is missing from libc-shim"
#endif

#ifndef EF_SPARC_32PLUS
#error "elf.h:EF_SPARC_32PLUS macro is missing from libc-shim"
#endif

#ifndef EF_SPARC_EXT_MASK
#error "elf.h:EF_SPARC_EXT_MASK macro is missing from libc-shim"
#endif

#ifndef EF_SPARC_HAL_R1
#error "elf.h:EF_SPARC_HAL_R1 macro is missing from libc-shim"
#endif

#ifndef EF_SPARC_LEDATA
#error "elf.h:EF_SPARC_LEDATA macro is missing from libc-shim"
#endif

#ifndef EF_SPARC_SUN_US1
#error "elf.h:EF_SPARC_SUN_US1 macro is missing from libc-shim"
#endif

#ifndef EF_SPARC_SUN_US3
#error "elf.h:EF_SPARC_SUN_US3 macro is missing from libc-shim"
#endif

#ifndef EI_ABIVERSION
#error "elf.h:EI_ABIVERSION macro is missing from libc-shim"
#endif

#ifndef EI_CLASS
#error "elf.h:EI_CLASS macro is missing from libc-shim"
#endif

#ifndef EI_DATA
#error "elf.h:EI_DATA macro is missing from libc-shim"
#endif

#ifndef EI_MAG0
#error "elf.h:EI_MAG0 macro is missing from libc-shim"
#endif

#ifndef EI_MAG1
#error "elf.h:EI_MAG1 macro is missing from libc-shim"
#endif

#ifndef EI_MAG2
#error "elf.h:EI_MAG2 macro is missing from libc-shim"
#endif

#ifndef EI_MAG3
#error "elf.h:EI_MAG3 macro is missing from libc-shim"
#endif

#ifndef EI_NIDENT
#error "elf.h:EI_NIDENT macro is missing from libc-shim"
#endif

#ifndef EI_OSABI
#error "elf.h:EI_OSABI macro is missing from libc-shim"
#endif

#ifndef EI_PAD
#error "elf.h:EI_PAD macro is missing from libc-shim"
#endif

#ifndef EI_VERSION
#error "elf.h:EI_VERSION macro is missing from libc-shim"
#endif

#ifndef ELF32_M_INFO
#error "elf.h:ELF32_M_INFO macro is missing from libc-shim"
#endif

#ifndef ELF32_M_SIZE
#error "elf.h:ELF32_M_SIZE macro is missing from libc-shim"
#endif

#ifndef ELF32_M_SYM
#error "elf.h:ELF32_M_SYM macro is missing from libc-shim"
#endif

#ifndef ELF32_R_INFO
#error "elf.h:ELF32_R_INFO macro is missing from libc-shim"
#endif

#ifndef ELF32_R_SYM
#error "elf.h:ELF32_R_SYM macro is missing from libc-shim"
#endif

#ifndef ELF32_R_TYPE
#error "elf.h:ELF32_R_TYPE macro is missing from libc-shim"
#endif

#ifndef ELF32_ST_BIND
#error "elf.h:ELF32_ST_BIND macro is missing from libc-shim"
#endif

#ifndef ELF32_ST_INFO
#error "elf.h:ELF32_ST_INFO macro is missing from libc-shim"
#endif

#ifndef ELF32_ST_TYPE
#error "elf.h:ELF32_ST_TYPE macro is missing from libc-shim"
#endif

#ifndef ELF32_ST_VISIBILITY
#error "elf.h:ELF32_ST_VISIBILITY macro is missing from libc-shim"
#endif

#ifndef ELF64_M_INFO
#error "elf.h:ELF64_M_INFO macro is missing from libc-shim"
#endif

#ifndef ELF64_M_SIZE
#error "elf.h:ELF64_M_SIZE macro is missing from libc-shim"
#endif

#ifndef ELF64_M_SYM
#error "elf.h:ELF64_M_SYM macro is missing from libc-shim"
#endif

#ifndef ELF64_R_INFO
#error "elf.h:ELF64_R_INFO macro is missing from libc-shim"
#endif

#ifndef ELF64_R_SYM
#error "elf.h:ELF64_R_SYM macro is missing from libc-shim"
#endif

#ifndef ELF64_R_TYPE
#error "elf.h:ELF64_R_TYPE macro is missing from libc-shim"
#endif

#ifndef ELF64_ST_BIND
#error "elf.h:ELF64_ST_BIND macro is missing from libc-shim"
#endif

#ifndef ELF64_ST_INFO
#error "elf.h:ELF64_ST_INFO macro is missing from libc-shim"
#endif

#ifndef ELF64_ST_TYPE
#error "elf.h:ELF64_ST_TYPE macro is missing from libc-shim"
#endif

#ifndef ELF64_ST_VISIBILITY
#error "elf.h:ELF64_ST_VISIBILITY macro is missing from libc-shim"
#endif

#ifndef ELFCLASS32
#error "elf.h:ELFCLASS32 macro is missing from libc-shim"
#endif

#ifndef ELFCLASS64
#error "elf.h:ELFCLASS64 macro is missing from libc-shim"
#endif

#ifndef ELFCLASSNONE
#error "elf.h:ELFCLASSNONE macro is missing from libc-shim"
#endif

#ifndef ELFCLASSNUM
#error "elf.h:ELFCLASSNUM macro is missing from libc-shim"
#endif

#ifndef ELFCOMPRESS_HIOS
#error "elf.h:ELFCOMPRESS_HIOS macro is missing from libc-shim"
#endif

#ifndef ELFCOMPRESS_HIPROC
#error "elf.h:ELFCOMPRESS_HIPROC macro is missing from libc-shim"
#endif

#ifndef ELFCOMPRESS_LOOS
#error "elf.h:ELFCOMPRESS_LOOS macro is missing from libc-shim"
#endif

#ifndef ELFCOMPRESS_LOPROC
#error "elf.h:ELFCOMPRESS_LOPROC macro is missing from libc-shim"
#endif

#ifndef ELFCOMPRESS_ZLIB
#error "elf.h:ELFCOMPRESS_ZLIB macro is missing from libc-shim"
#endif

#ifndef ELFCOMPRESS_ZSTD
#error "elf.h:ELFCOMPRESS_ZSTD macro is missing from libc-shim"
#endif

#ifndef ELFDATA2LSB
#error "elf.h:ELFDATA2LSB macro is missing from libc-shim"
#endif

#ifndef ELFDATA2MSB
#error "elf.h:ELFDATA2MSB macro is missing from libc-shim"
#endif

#ifndef ELFDATANONE
#error "elf.h:ELFDATANONE macro is missing from libc-shim"
#endif

#ifndef ELFDATANUM
#error "elf.h:ELFDATANUM macro is missing from libc-shim"
#endif

#ifndef ELFMAG
#error "elf.h:ELFMAG macro is missing from libc-shim"
#endif

#ifndef ELFMAG0
#error "elf.h:ELFMAG0 macro is missing from libc-shim"
#endif

#ifndef ELFMAG1
#error "elf.h:ELFMAG1 macro is missing from libc-shim"
#endif

#ifndef ELFMAG2
#error "elf.h:ELFMAG2 macro is missing from libc-shim"
#endif

#ifndef ELFMAG3
#error "elf.h:ELFMAG3 macro is missing from libc-shim"
#endif

#ifndef ELFOSABI_AIX
#error "elf.h:ELFOSABI_AIX macro is missing from libc-shim"
#endif

#ifndef ELFOSABI_ARM
#error "elf.h:ELFOSABI_ARM macro is missing from libc-shim"
#endif

#ifndef ELFOSABI_ARM_AEABI
#error "elf.h:ELFOSABI_ARM_AEABI macro is missing from libc-shim"
#endif

#ifndef ELFOSABI_FREEBSD
#error "elf.h:ELFOSABI_FREEBSD macro is missing from libc-shim"
#endif

#ifndef ELFOSABI_GNU
#error "elf.h:ELFOSABI_GNU macro is missing from libc-shim"
#endif

#ifndef ELFOSABI_HPUX
#error "elf.h:ELFOSABI_HPUX macro is missing from libc-shim"
#endif

#ifndef ELFOSABI_IRIX
#error "elf.h:ELFOSABI_IRIX macro is missing from libc-shim"
#endif

#ifndef ELFOSABI_LINUX
#error "elf.h:ELFOSABI_LINUX macro is missing from libc-shim"
#endif

#ifndef ELFOSABI_MODESTO
#error "elf.h:ELFOSABI_MODESTO macro is missing from libc-shim"
#endif

#ifndef ELFOSABI_NETBSD
#error "elf.h:ELFOSABI_NETBSD macro is missing from libc-shim"
#endif

#ifndef ELFOSABI_NONE
#error "elf.h:ELFOSABI_NONE macro is missing from libc-shim"
#endif

#ifndef ELFOSABI_OPENBSD
#error "elf.h:ELFOSABI_OPENBSD macro is missing from libc-shim"
#endif

#ifndef ELFOSABI_SOLARIS
#error "elf.h:ELFOSABI_SOLARIS macro is missing from libc-shim"
#endif

#ifndef ELFOSABI_STANDALONE
#error "elf.h:ELFOSABI_STANDALONE macro is missing from libc-shim"
#endif

#ifndef ELFOSABI_SYSV
#error "elf.h:ELFOSABI_SYSV macro is missing from libc-shim"
#endif

#ifndef ELFOSABI_TRU64
#error "elf.h:ELFOSABI_TRU64 macro is missing from libc-shim"
#endif

#ifndef ELF_NOTE_ABI
#error "elf.h:ELF_NOTE_ABI macro is missing from libc-shim"
#endif

#ifndef ELF_NOTE_FDO
#error "elf.h:ELF_NOTE_FDO macro is missing from libc-shim"
#endif

#ifndef ELF_NOTE_GNU
#error "elf.h:ELF_NOTE_GNU macro is missing from libc-shim"
#endif

#ifndef ELF_NOTE_OS_FREEBSD
#error "elf.h:ELF_NOTE_OS_FREEBSD macro is missing from libc-shim"
#endif

#ifndef ELF_NOTE_OS_GNU
#error "elf.h:ELF_NOTE_OS_GNU macro is missing from libc-shim"
#endif

#ifndef ELF_NOTE_OS_LINUX
#error "elf.h:ELF_NOTE_OS_LINUX macro is missing from libc-shim"
#endif

#ifndef ELF_NOTE_OS_SOLARIS2
#error "elf.h:ELF_NOTE_OS_SOLARIS2 macro is missing from libc-shim"
#endif

#ifndef ELF_NOTE_PAGESIZE_HINT
#error "elf.h:ELF_NOTE_PAGESIZE_HINT macro is missing from libc-shim"
#endif

#ifndef ELF_NOTE_SOLARIS
#error "elf.h:ELF_NOTE_SOLARIS macro is missing from libc-shim"
#endif

#ifndef EM_386
#error "elf.h:EM_386 macro is missing from libc-shim"
#endif

#ifndef EM_56800EX
#error "elf.h:EM_56800EX macro is missing from libc-shim"
#endif

#ifndef EM_68HC05
#error "elf.h:EM_68HC05 macro is missing from libc-shim"
#endif

#ifndef EM_68HC08
#error "elf.h:EM_68HC08 macro is missing from libc-shim"
#endif

#ifndef EM_68HC11
#error "elf.h:EM_68HC11 macro is missing from libc-shim"
#endif

#ifndef EM_68HC12
#error "elf.h:EM_68HC12 macro is missing from libc-shim"
#endif

#ifndef EM_68HC16
#error "elf.h:EM_68HC16 macro is missing from libc-shim"
#endif

#ifndef EM_68K
#error "elf.h:EM_68K macro is missing from libc-shim"
#endif

#ifndef EM_78KOR
#error "elf.h:EM_78KOR macro is missing from libc-shim"
#endif

#ifndef EM_8051
#error "elf.h:EM_8051 macro is missing from libc-shim"
#endif

#ifndef EM_860
#error "elf.h:EM_860 macro is missing from libc-shim"
#endif

#ifndef EM_88K
#error "elf.h:EM_88K macro is missing from libc-shim"
#endif

#ifndef EM_960
#error "elf.h:EM_960 macro is missing from libc-shim"
#endif

#ifndef EM_AARCH64
#error "elf.h:EM_AARCH64 macro is missing from libc-shim"
#endif

#ifndef EM_ALPHA
#error "elf.h:EM_ALPHA macro is missing from libc-shim"
#endif

#ifndef EM_ALTERA_NIOS2
#error "elf.h:EM_ALTERA_NIOS2 macro is missing from libc-shim"
#endif

#ifndef EM_AMDGPU
#error "elf.h:EM_AMDGPU macro is missing from libc-shim"
#endif

#ifndef EM_ARC
#error "elf.h:EM_ARC macro is missing from libc-shim"
#endif

#ifndef EM_ARCA
#error "elf.h:EM_ARCA macro is missing from libc-shim"
#endif

#ifndef EM_ARCV2
#error "elf.h:EM_ARCV2 macro is missing from libc-shim"
#endif

#ifndef EM_ARC_A5
#error "elf.h:EM_ARC_A5 macro is missing from libc-shim"
#endif

#ifndef EM_ARC_COMPACT
#error "elf.h:EM_ARC_COMPACT macro is missing from libc-shim"
#endif

#ifndef EM_ARM
#error "elf.h:EM_ARM macro is missing from libc-shim"
#endif

#ifndef EM_AVR
#error "elf.h:EM_AVR macro is missing from libc-shim"
#endif

#ifndef EM_AVR32
#error "elf.h:EM_AVR32 macro is missing from libc-shim"
#endif

#ifndef EM_BA1
#error "elf.h:EM_BA1 macro is missing from libc-shim"
#endif

#ifndef EM_BA2
#error "elf.h:EM_BA2 macro is missing from libc-shim"
#endif

#ifndef EM_BLACKFIN
#error "elf.h:EM_BLACKFIN macro is missing from libc-shim"
#endif

#ifndef EM_BPF
#error "elf.h:EM_BPF macro is missing from libc-shim"
#endif

#ifndef EM_C166
#error "elf.h:EM_C166 macro is missing from libc-shim"
#endif

#ifndef EM_CDP
#error "elf.h:EM_CDP macro is missing from libc-shim"
#endif

#ifndef EM_CE
#error "elf.h:EM_CE macro is missing from libc-shim"
#endif

#ifndef EM_CLOUDSHIELD
#error "elf.h:EM_CLOUDSHIELD macro is missing from libc-shim"
#endif

#ifndef EM_COGE
#error "elf.h:EM_COGE macro is missing from libc-shim"
#endif

#ifndef EM_COLDFIRE
#error "elf.h:EM_COLDFIRE macro is missing from libc-shim"
#endif

#ifndef EM_COOL
#error "elf.h:EM_COOL macro is missing from libc-shim"
#endif

#ifndef EM_COREA_1ST
#error "elf.h:EM_COREA_1ST macro is missing from libc-shim"
#endif

#ifndef EM_COREA_2ND
#error "elf.h:EM_COREA_2ND macro is missing from libc-shim"
#endif

#ifndef EM_CR
#error "elf.h:EM_CR macro is missing from libc-shim"
#endif

#ifndef EM_CR16
#error "elf.h:EM_CR16 macro is missing from libc-shim"
#endif

#ifndef EM_CRAYNV2
#error "elf.h:EM_CRAYNV2 macro is missing from libc-shim"
#endif

#ifndef EM_CRIS
#error "elf.h:EM_CRIS macro is missing from libc-shim"
#endif

#ifndef EM_CRX
#error "elf.h:EM_CRX macro is missing from libc-shim"
#endif

#ifndef EM_CSKY
#error "elf.h:EM_CSKY macro is missing from libc-shim"
#endif

#ifndef EM_CSR_KALIMBA
#error "elf.h:EM_CSR_KALIMBA macro is missing from libc-shim"
#endif

#ifndef EM_CUDA
#error "elf.h:EM_CUDA macro is missing from libc-shim"
#endif

#ifndef EM_CYPRESS_M8C
#error "elf.h:EM_CYPRESS_M8C macro is missing from libc-shim"
#endif

#ifndef EM_D10V
#error "elf.h:EM_D10V macro is missing from libc-shim"
#endif

#ifndef EM_D30V
#error "elf.h:EM_D30V macro is missing from libc-shim"
#endif

#ifndef EM_DSP24
#error "elf.h:EM_DSP24 macro is missing from libc-shim"
#endif

#ifndef EM_DSPIC30F
#error "elf.h:EM_DSPIC30F macro is missing from libc-shim"
#endif

#ifndef EM_DXP
#error "elf.h:EM_DXP macro is missing from libc-shim"
#endif

#ifndef EM_ECOG16
#error "elf.h:EM_ECOG16 macro is missing from libc-shim"
#endif

#ifndef EM_ECOG1X
#error "elf.h:EM_ECOG1X macro is missing from libc-shim"
#endif

#ifndef EM_ECOG2
#error "elf.h:EM_ECOG2 macro is missing from libc-shim"
#endif

#ifndef EM_EMX16
#error "elf.h:EM_EMX16 macro is missing from libc-shim"
#endif

#ifndef EM_EMX8
#error "elf.h:EM_EMX8 macro is missing from libc-shim"
#endif

#ifndef EM_ETPU
#error "elf.h:EM_ETPU macro is missing from libc-shim"
#endif

#ifndef EM_EXCESS
#error "elf.h:EM_EXCESS macro is missing from libc-shim"
#endif

#ifndef EM_F2MC16
#error "elf.h:EM_F2MC16 macro is missing from libc-shim"
#endif

#ifndef EM_FAKE_ALPHA
#error "elf.h:EM_FAKE_ALPHA macro is missing from libc-shim"
#endif

#ifndef EM_FIREPATH
#error "elf.h:EM_FIREPATH macro is missing from libc-shim"
#endif

#ifndef EM_FR20
#error "elf.h:EM_FR20 macro is missing from libc-shim"
#endif

#ifndef EM_FR30
#error "elf.h:EM_FR30 macro is missing from libc-shim"
#endif

#ifndef EM_FT32
#error "elf.h:EM_FT32 macro is missing from libc-shim"
#endif

#ifndef EM_FX66
#error "elf.h:EM_FX66 macro is missing from libc-shim"
#endif

#ifndef EM_H8S
#error "elf.h:EM_H8S macro is missing from libc-shim"
#endif

#ifndef EM_H8_300
#error "elf.h:EM_H8_300 macro is missing from libc-shim"
#endif

#ifndef EM_H8_300H
#error "elf.h:EM_H8_300H macro is missing from libc-shim"
#endif

#ifndef EM_H8_500
#error "elf.h:EM_H8_500 macro is missing from libc-shim"
#endif

#ifndef EM_HUANY
#error "elf.h:EM_HUANY macro is missing from libc-shim"
#endif

#ifndef EM_IAMCU
#error "elf.h:EM_IAMCU macro is missing from libc-shim"
#endif

#ifndef EM_IA_64
#error "elf.h:EM_IA_64 macro is missing from libc-shim"
#endif

#ifndef EM_INTELGT
#error "elf.h:EM_INTELGT macro is missing from libc-shim"
#endif

#ifndef EM_IP2K
#error "elf.h:EM_IP2K macro is missing from libc-shim"
#endif

#ifndef EM_JAVELIN
#error "elf.h:EM_JAVELIN macro is missing from libc-shim"
#endif

#ifndef EM_K10M
#error "elf.h:EM_K10M macro is missing from libc-shim"
#endif

#ifndef EM_KM32
#error "elf.h:EM_KM32 macro is missing from libc-shim"
#endif

#ifndef EM_KMX32
#error "elf.h:EM_KMX32 macro is missing from libc-shim"
#endif

#ifndef EM_KVARC
#error "elf.h:EM_KVARC macro is missing from libc-shim"
#endif

#ifndef EM_L10M
#error "elf.h:EM_L10M macro is missing from libc-shim"
#endif

#ifndef EM_LATTICEMICO32
#error "elf.h:EM_LATTICEMICO32 macro is missing from libc-shim"
#endif

#ifndef EM_LOONGARCH
#error "elf.h:EM_LOONGARCH macro is missing from libc-shim"
#endif

#ifndef EM_M16C
#error "elf.h:EM_M16C macro is missing from libc-shim"
#endif

#ifndef EM_M32
#error "elf.h:EM_M32 macro is missing from libc-shim"
#endif

#ifndef EM_M32C
#error "elf.h:EM_M32C macro is missing from libc-shim"
#endif

#ifndef EM_M32R
#error "elf.h:EM_M32R macro is missing from libc-shim"
#endif

#ifndef EM_MANIK
#error "elf.h:EM_MANIK macro is missing from libc-shim"
#endif

#ifndef EM_MAX
#error "elf.h:EM_MAX macro is missing from libc-shim"
#endif

#ifndef EM_MAXQ30
#error "elf.h:EM_MAXQ30 macro is missing from libc-shim"
#endif

#ifndef EM_MCHP_PIC
#error "elf.h:EM_MCHP_PIC macro is missing from libc-shim"
#endif

#ifndef EM_MCST_ELBRUS
#error "elf.h:EM_MCST_ELBRUS macro is missing from libc-shim"
#endif

#ifndef EM_ME16
#error "elf.h:EM_ME16 macro is missing from libc-shim"
#endif

#ifndef EM_METAG
#error "elf.h:EM_METAG macro is missing from libc-shim"
#endif

#ifndef EM_MICROBLAZE
#error "elf.h:EM_MICROBLAZE macro is missing from libc-shim"
#endif

#ifndef EM_MIPS
#error "elf.h:EM_MIPS macro is missing from libc-shim"
#endif

#ifndef EM_MIPS_RS3_LE
#error "elf.h:EM_MIPS_RS3_LE macro is missing from libc-shim"
#endif

#ifndef EM_MIPS_X
#error "elf.h:EM_MIPS_X macro is missing from libc-shim"
#endif

#ifndef EM_MMA
#error "elf.h:EM_MMA macro is missing from libc-shim"
#endif

#ifndef EM_MMDSP_PLUS
#error "elf.h:EM_MMDSP_PLUS macro is missing from libc-shim"
#endif

#ifndef EM_MMIX
#error "elf.h:EM_MMIX macro is missing from libc-shim"
#endif

#ifndef EM_MN10200
#error "elf.h:EM_MN10200 macro is missing from libc-shim"
#endif

#ifndef EM_MN10300
#error "elf.h:EM_MN10300 macro is missing from libc-shim"
#endif

#ifndef EM_MOXIE
#error "elf.h:EM_MOXIE macro is missing from libc-shim"
#endif

#ifndef EM_MSP430
#error "elf.h:EM_MSP430 macro is missing from libc-shim"
#endif

#ifndef EM_NCPU
#error "elf.h:EM_NCPU macro is missing from libc-shim"
#endif

#ifndef EM_NDR1
#error "elf.h:EM_NDR1 macro is missing from libc-shim"
#endif

#ifndef EM_NDS32
#error "elf.h:EM_NDS32 macro is missing from libc-shim"
#endif

#ifndef EM_NONE
#error "elf.h:EM_NONE macro is missing from libc-shim"
#endif

#ifndef EM_NORC
#error "elf.h:EM_NORC macro is missing from libc-shim"
#endif

#ifndef EM_NS32K
#error "elf.h:EM_NS32K macro is missing from libc-shim"
#endif

#ifndef EM_NUM
#error "elf.h:EM_NUM macro is missing from libc-shim"
#endif

#ifndef EM_OPEN8
#error "elf.h:EM_OPEN8 macro is missing from libc-shim"
#endif

#ifndef EM_OPENRISC
#error "elf.h:EM_OPENRISC macro is missing from libc-shim"
#endif

#ifndef EM_PARISC
#error "elf.h:EM_PARISC macro is missing from libc-shim"
#endif

#ifndef EM_PCP
#error "elf.h:EM_PCP macro is missing from libc-shim"
#endif

#ifndef EM_PDP10
#error "elf.h:EM_PDP10 macro is missing from libc-shim"
#endif

#ifndef EM_PDP11
#error "elf.h:EM_PDP11 macro is missing from libc-shim"
#endif

#ifndef EM_PDSP
#error "elf.h:EM_PDSP macro is missing from libc-shim"
#endif

#ifndef EM_PJ
#error "elf.h:EM_PJ macro is missing from libc-shim"
#endif

#ifndef EM_PPC
#error "elf.h:EM_PPC macro is missing from libc-shim"
#endif

#ifndef EM_PPC64
#error "elf.h:EM_PPC64 macro is missing from libc-shim"
#endif

#ifndef EM_PRISM
#error "elf.h:EM_PRISM macro is missing from libc-shim"
#endif

#ifndef EM_QDSP6
#error "elf.h:EM_QDSP6 macro is missing from libc-shim"
#endif

#ifndef EM_R32C
#error "elf.h:EM_R32C macro is missing from libc-shim"
#endif

#ifndef EM_RCE
#error "elf.h:EM_RCE macro is missing from libc-shim"
#endif

#ifndef EM_RH32
#error "elf.h:EM_RH32 macro is missing from libc-shim"
#endif

#ifndef EM_RISCV
#error "elf.h:EM_RISCV macro is missing from libc-shim"
#endif

#ifndef EM_RL78
#error "elf.h:EM_RL78 macro is missing from libc-shim"
#endif

#ifndef EM_RS08
#error "elf.h:EM_RS08 macro is missing from libc-shim"
#endif

#ifndef EM_RX
#error "elf.h:EM_RX macro is missing from libc-shim"
#endif

#ifndef EM_S370
#error "elf.h:EM_S370 macro is missing from libc-shim"
#endif

#ifndef EM_S390
#error "elf.h:EM_S390 macro is missing from libc-shim"
#endif

#ifndef EM_SCORE7
#error "elf.h:EM_SCORE7 macro is missing from libc-shim"
#endif

#ifndef EM_SEP
#error "elf.h:EM_SEP macro is missing from libc-shim"
#endif

#ifndef EM_SE_C17
#error "elf.h:EM_SE_C17 macro is missing from libc-shim"
#endif

#ifndef EM_SE_C33
#error "elf.h:EM_SE_C33 macro is missing from libc-shim"
#endif

#ifndef EM_SH
#error "elf.h:EM_SH macro is missing from libc-shim"
#endif

#ifndef EM_SHARC
#error "elf.h:EM_SHARC macro is missing from libc-shim"
#endif

#ifndef EM_SLE9X
#error "elf.h:EM_SLE9X macro is missing from libc-shim"
#endif

#ifndef EM_SNP1K
#error "elf.h:EM_SNP1K macro is missing from libc-shim"
#endif

#ifndef EM_SPARC
#error "elf.h:EM_SPARC macro is missing from libc-shim"
#endif

#ifndef EM_SPARC32PLUS
#error "elf.h:EM_SPARC32PLUS macro is missing from libc-shim"
#endif

#ifndef EM_SPARCV9
#error "elf.h:EM_SPARCV9 macro is missing from libc-shim"
#endif

#ifndef EM_SPU
#error "elf.h:EM_SPU macro is missing from libc-shim"
#endif

#ifndef EM_ST100
#error "elf.h:EM_ST100 macro is missing from libc-shim"
#endif

#ifndef EM_ST19
#error "elf.h:EM_ST19 macro is missing from libc-shim"
#endif

#ifndef EM_ST200
#error "elf.h:EM_ST200 macro is missing from libc-shim"
#endif

#ifndef EM_ST7
#error "elf.h:EM_ST7 macro is missing from libc-shim"
#endif

#ifndef EM_ST9PLUS
#error "elf.h:EM_ST9PLUS macro is missing from libc-shim"
#endif

#ifndef EM_STARCORE
#error "elf.h:EM_STARCORE macro is missing from libc-shim"
#endif

#ifndef EM_STM8
#error "elf.h:EM_STM8 macro is missing from libc-shim"
#endif

#ifndef EM_STXP7X
#error "elf.h:EM_STXP7X macro is missing from libc-shim"
#endif

#ifndef EM_SVX
#error "elf.h:EM_SVX macro is missing from libc-shim"
#endif

#ifndef EM_TILE64
#error "elf.h:EM_TILE64 macro is missing from libc-shim"
#endif

#ifndef EM_TILEGX
#error "elf.h:EM_TILEGX macro is missing from libc-shim"
#endif

#ifndef EM_TILEPRO
#error "elf.h:EM_TILEPRO macro is missing from libc-shim"
#endif

#ifndef EM_TINYJ
#error "elf.h:EM_TINYJ macro is missing from libc-shim"
#endif

#ifndef EM_TI_ARP32
#error "elf.h:EM_TI_ARP32 macro is missing from libc-shim"
#endif

#ifndef EM_TI_C2000
#error "elf.h:EM_TI_C2000 macro is missing from libc-shim"
#endif

#ifndef EM_TI_C5500
#error "elf.h:EM_TI_C5500 macro is missing from libc-shim"
#endif

#ifndef EM_TI_C6000
#error "elf.h:EM_TI_C6000 macro is missing from libc-shim"
#endif

#ifndef EM_TI_PRU
#error "elf.h:EM_TI_PRU macro is missing from libc-shim"
#endif

#ifndef EM_TMM_GPP
#error "elf.h:EM_TMM_GPP macro is missing from libc-shim"
#endif

#ifndef EM_TPC
#error "elf.h:EM_TPC macro is missing from libc-shim"
#endif

#ifndef EM_TRICORE
#error "elf.h:EM_TRICORE macro is missing from libc-shim"
#endif

#ifndef EM_TRIMEDIA
#error "elf.h:EM_TRIMEDIA macro is missing from libc-shim"
#endif

#ifndef EM_TSK3000
#error "elf.h:EM_TSK3000 macro is missing from libc-shim"
#endif

#ifndef EM_UNICORE
#error "elf.h:EM_UNICORE macro is missing from libc-shim"
#endif

#ifndef EM_V800
#error "elf.h:EM_V800 macro is missing from libc-shim"
#endif

#ifndef EM_V850
#error "elf.h:EM_V850 macro is missing from libc-shim"
#endif

#ifndef EM_VAX
#error "elf.h:EM_VAX macro is missing from libc-shim"
#endif

#ifndef EM_VIDEOCORE
#error "elf.h:EM_VIDEOCORE macro is missing from libc-shim"
#endif

#ifndef EM_VIDEOCORE3
#error "elf.h:EM_VIDEOCORE3 macro is missing from libc-shim"
#endif

#ifndef EM_VIDEOCORE5
#error "elf.h:EM_VIDEOCORE5 macro is missing from libc-shim"
#endif

#ifndef EM_VISIUM
#error "elf.h:EM_VISIUM macro is missing from libc-shim"
#endif

#ifndef EM_VPP500
#error "elf.h:EM_VPP500 macro is missing from libc-shim"
#endif

#ifndef EM_X86_64
#error "elf.h:EM_X86_64 macro is missing from libc-shim"
#endif

#ifndef EM_XCORE
#error "elf.h:EM_XCORE macro is missing from libc-shim"
#endif

#ifndef EM_XGATE
#error "elf.h:EM_XGATE macro is missing from libc-shim"
#endif

#ifndef EM_XIMO16
#error "elf.h:EM_XIMO16 macro is missing from libc-shim"
#endif

#ifndef EM_XTENSA
#error "elf.h:EM_XTENSA macro is missing from libc-shim"
#endif

#ifndef EM_Z80
#error "elf.h:EM_Z80 macro is missing from libc-shim"
#endif

#ifndef EM_ZSP
#error "elf.h:EM_ZSP macro is missing from libc-shim"
#endif

#ifndef ET_CORE
#error "elf.h:ET_CORE macro is missing from libc-shim"
#endif

#ifndef ET_DYN
#error "elf.h:ET_DYN macro is missing from libc-shim"
#endif

#ifndef ET_EXEC
#error "elf.h:ET_EXEC macro is missing from libc-shim"
#endif

#ifndef ET_HIOS
#error "elf.h:ET_HIOS macro is missing from libc-shim"
#endif

#ifndef ET_HIPROC
#error "elf.h:ET_HIPROC macro is missing from libc-shim"
#endif

#ifndef ET_LOOS
#error "elf.h:ET_LOOS macro is missing from libc-shim"
#endif

#ifndef ET_LOPROC
#error "elf.h:ET_LOPROC macro is missing from libc-shim"
#endif

#ifndef ET_NONE
#error "elf.h:ET_NONE macro is missing from libc-shim"
#endif

#ifndef ET_NUM
#error "elf.h:ET_NUM macro is missing from libc-shim"
#endif

#ifndef ET_REL
#error "elf.h:ET_REL macro is missing from libc-shim"
#endif

#ifndef EV_CURRENT
#error "elf.h:EV_CURRENT macro is missing from libc-shim"
#endif

#ifndef EV_NONE
#error "elf.h:EV_NONE macro is missing from libc-shim"
#endif

#ifndef EV_NUM
#error "elf.h:EV_NUM macro is missing from libc-shim"
#endif

#ifndef E_MIPS_ARCH_1
#error "elf.h:E_MIPS_ARCH_1 macro is missing from libc-shim"
#endif

#ifndef E_MIPS_ARCH_2
#error "elf.h:E_MIPS_ARCH_2 macro is missing from libc-shim"
#endif

#ifndef E_MIPS_ARCH_3
#error "elf.h:E_MIPS_ARCH_3 macro is missing from libc-shim"
#endif

#ifndef E_MIPS_ARCH_32
#error "elf.h:E_MIPS_ARCH_32 macro is missing from libc-shim"
#endif

#ifndef E_MIPS_ARCH_4
#error "elf.h:E_MIPS_ARCH_4 macro is missing from libc-shim"
#endif

#ifndef E_MIPS_ARCH_5
#error "elf.h:E_MIPS_ARCH_5 macro is missing from libc-shim"
#endif

#ifndef E_MIPS_ARCH_64
#error "elf.h:E_MIPS_ARCH_64 macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_1_NEEDED
#error "elf.h:GNU_PROPERTY_1_NEEDED macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_1_NEEDED_INDIRECT_EXTERN_ACCESS
#error "elf.h:GNU_PROPERTY_1_NEEDED_INDIRECT_EXTERN_ACCESS macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_AARCH64_FEATURE_1_AND
#error "elf.h:GNU_PROPERTY_AARCH64_FEATURE_1_AND macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_AARCH64_FEATURE_1_BTI
#error "elf.h:GNU_PROPERTY_AARCH64_FEATURE_1_BTI macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_AARCH64_FEATURE_1_GCS
#error "elf.h:GNU_PROPERTY_AARCH64_FEATURE_1_GCS macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_AARCH64_FEATURE_1_PAC
#error "elf.h:GNU_PROPERTY_AARCH64_FEATURE_1_PAC macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_HIPROC
#error "elf.h:GNU_PROPERTY_HIPROC macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_HIUSER
#error "elf.h:GNU_PROPERTY_HIUSER macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_LOPROC
#error "elf.h:GNU_PROPERTY_LOPROC macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_LOUSER
#error "elf.h:GNU_PROPERTY_LOUSER macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_NO_COPY_ON_PROTECTED
#error "elf.h:GNU_PROPERTY_NO_COPY_ON_PROTECTED macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_STACK_SIZE
#error "elf.h:GNU_PROPERTY_STACK_SIZE macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_UINT32_AND_HI
#error "elf.h:GNU_PROPERTY_UINT32_AND_HI macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_UINT32_AND_LO
#error "elf.h:GNU_PROPERTY_UINT32_AND_LO macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_UINT32_OR_HI
#error "elf.h:GNU_PROPERTY_UINT32_OR_HI macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_UINT32_OR_LO
#error "elf.h:GNU_PROPERTY_UINT32_OR_LO macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_X86_FEATURE_1_AND
#error "elf.h:GNU_PROPERTY_X86_FEATURE_1_AND macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_X86_FEATURE_1_IBT
#error "elf.h:GNU_PROPERTY_X86_FEATURE_1_IBT macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_X86_FEATURE_1_SHSTK
#error "elf.h:GNU_PROPERTY_X86_FEATURE_1_SHSTK macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_X86_ISA_1_BASELINE
#error "elf.h:GNU_PROPERTY_X86_ISA_1_BASELINE macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_X86_ISA_1_NEEDED
#error "elf.h:GNU_PROPERTY_X86_ISA_1_NEEDED macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_X86_ISA_1_USED
#error "elf.h:GNU_PROPERTY_X86_ISA_1_USED macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_X86_ISA_1_V2
#error "elf.h:GNU_PROPERTY_X86_ISA_1_V2 macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_X86_ISA_1_V3
#error "elf.h:GNU_PROPERTY_X86_ISA_1_V3 macro is missing from libc-shim"
#endif

#ifndef GNU_PROPERTY_X86_ISA_1_V4
#error "elf.h:GNU_PROPERTY_X86_ISA_1_V4 macro is missing from libc-shim"
#endif

#ifndef GRP_COMDAT
#error "elf.h:GRP_COMDAT macro is missing from libc-shim"
#endif

#ifndef LITUSE_ALPHA_ADDR
#error "elf.h:LITUSE_ALPHA_ADDR macro is missing from libc-shim"
#endif

#ifndef LITUSE_ALPHA_BASE
#error "elf.h:LITUSE_ALPHA_BASE macro is missing from libc-shim"
#endif

#ifndef LITUSE_ALPHA_BYTOFF
#error "elf.h:LITUSE_ALPHA_BYTOFF macro is missing from libc-shim"
#endif

#ifndef LITUSE_ALPHA_JSR
#error "elf.h:LITUSE_ALPHA_JSR macro is missing from libc-shim"
#endif

#ifndef LITUSE_ALPHA_TLS_GD
#error "elf.h:LITUSE_ALPHA_TLS_GD macro is missing from libc-shim"
#endif

#ifndef LITUSE_ALPHA_TLS_LDM
#error "elf.h:LITUSE_ALPHA_TLS_LDM macro is missing from libc-shim"
#endif

#ifndef LL_DELAY_LOAD
#error "elf.h:LL_DELAY_LOAD macro is missing from libc-shim"
#endif

#ifndef LL_DELTA
#error "elf.h:LL_DELTA macro is missing from libc-shim"
#endif

#ifndef LL_EXACT_MATCH
#error "elf.h:LL_EXACT_MATCH macro is missing from libc-shim"
#endif

#ifndef LL_EXPORTS
#error "elf.h:LL_EXPORTS macro is missing from libc-shim"
#endif

#ifndef LL_IGNORE_INT_VER
#error "elf.h:LL_IGNORE_INT_VER macro is missing from libc-shim"
#endif

#ifndef LL_NONE
#error "elf.h:LL_NONE macro is missing from libc-shim"
#endif

#ifndef LL_REQUIRE_MINOR
#error "elf.h:LL_REQUIRE_MINOR macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_ASE_DSP
#error "elf.h:MIPS_AFL_ASE_DSP macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_ASE_DSPR2
#error "elf.h:MIPS_AFL_ASE_DSPR2 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_ASE_EVA
#error "elf.h:MIPS_AFL_ASE_EVA macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_ASE_MASK
#error "elf.h:MIPS_AFL_ASE_MASK macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_ASE_MCU
#error "elf.h:MIPS_AFL_ASE_MCU macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_ASE_MDMX
#error "elf.h:MIPS_AFL_ASE_MDMX macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_ASE_MICROMIPS
#error "elf.h:MIPS_AFL_ASE_MICROMIPS macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_ASE_MIPS16
#error "elf.h:MIPS_AFL_ASE_MIPS16 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_ASE_MIPS3D
#error "elf.h:MIPS_AFL_ASE_MIPS3D macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_ASE_MSA
#error "elf.h:MIPS_AFL_ASE_MSA macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_ASE_MT
#error "elf.h:MIPS_AFL_ASE_MT macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_ASE_SMARTMIPS
#error "elf.h:MIPS_AFL_ASE_SMARTMIPS macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_ASE_VIRT
#error "elf.h:MIPS_AFL_ASE_VIRT macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_ASE_XPA
#error "elf.h:MIPS_AFL_ASE_XPA macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_10000
#error "elf.h:MIPS_AFL_EXT_10000 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_3900
#error "elf.h:MIPS_AFL_EXT_3900 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_4010
#error "elf.h:MIPS_AFL_EXT_4010 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_4100
#error "elf.h:MIPS_AFL_EXT_4100 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_4111
#error "elf.h:MIPS_AFL_EXT_4111 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_4120
#error "elf.h:MIPS_AFL_EXT_4120 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_4650
#error "elf.h:MIPS_AFL_EXT_4650 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_5400
#error "elf.h:MIPS_AFL_EXT_5400 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_5500
#error "elf.h:MIPS_AFL_EXT_5500 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_5900
#error "elf.h:MIPS_AFL_EXT_5900 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_LOONGSON_2E
#error "elf.h:MIPS_AFL_EXT_LOONGSON_2E macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_LOONGSON_2F
#error "elf.h:MIPS_AFL_EXT_LOONGSON_2F macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_LOONGSON_3A
#error "elf.h:MIPS_AFL_EXT_LOONGSON_3A macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_OCTEON
#error "elf.h:MIPS_AFL_EXT_OCTEON macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_OCTEON2
#error "elf.h:MIPS_AFL_EXT_OCTEON2 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_OCTEONP
#error "elf.h:MIPS_AFL_EXT_OCTEONP macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_SB1
#error "elf.h:MIPS_AFL_EXT_SB1 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_EXT_XLR
#error "elf.h:MIPS_AFL_EXT_XLR macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_FLAGS1_ODDSPREG
#error "elf.h:MIPS_AFL_FLAGS1_ODDSPREG macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_REG_128
#error "elf.h:MIPS_AFL_REG_128 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_REG_32
#error "elf.h:MIPS_AFL_REG_32 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_REG_64
#error "elf.h:MIPS_AFL_REG_64 macro is missing from libc-shim"
#endif

#ifndef MIPS_AFL_REG_NONE
#error "elf.h:MIPS_AFL_REG_NONE macro is missing from libc-shim"
#endif

#ifndef NOTE_GNU_PROPERTY_SECTION_NAME
#error "elf.h:NOTE_GNU_PROPERTY_SECTION_NAME macro is missing from libc-shim"
#endif

#ifndef NT_386_IOPERM
#error "elf.h:NT_386_IOPERM macro is missing from libc-shim"
#endif

#ifndef NT_386_TLS
#error "elf.h:NT_386_TLS macro is missing from libc-shim"
#endif

#ifndef NT_ARM_FPMR
#error "elf.h:NT_ARM_FPMR macro is missing from libc-shim"
#endif

#ifndef NT_ARM_GCS
#error "elf.h:NT_ARM_GCS macro is missing from libc-shim"
#endif

#ifndef NT_ARM_HW_BREAK
#error "elf.h:NT_ARM_HW_BREAK macro is missing from libc-shim"
#endif

#ifndef NT_ARM_HW_WATCH
#error "elf.h:NT_ARM_HW_WATCH macro is missing from libc-shim"
#endif

#ifndef NT_ARM_PACA_KEYS
#error "elf.h:NT_ARM_PACA_KEYS macro is missing from libc-shim"
#endif

#ifndef NT_ARM_PACG_KEYS
#error "elf.h:NT_ARM_PACG_KEYS macro is missing from libc-shim"
#endif

#ifndef NT_ARM_PAC_ENABLED_KEYS
#error "elf.h:NT_ARM_PAC_ENABLED_KEYS macro is missing from libc-shim"
#endif

#ifndef NT_ARM_PAC_MASK
#error "elf.h:NT_ARM_PAC_MASK macro is missing from libc-shim"
#endif

#ifndef NT_ARM_POE
#error "elf.h:NT_ARM_POE macro is missing from libc-shim"
#endif

#ifndef NT_ARM_SSVE
#error "elf.h:NT_ARM_SSVE macro is missing from libc-shim"
#endif

#ifndef NT_ARM_SVE
#error "elf.h:NT_ARM_SVE macro is missing from libc-shim"
#endif

#ifndef NT_ARM_SYSTEM_CALL
#error "elf.h:NT_ARM_SYSTEM_CALL macro is missing from libc-shim"
#endif

#ifndef NT_ARM_TAGGED_ADDR_CTRL
#error "elf.h:NT_ARM_TAGGED_ADDR_CTRL macro is missing from libc-shim"
#endif

#ifndef NT_ARM_TLS
#error "elf.h:NT_ARM_TLS macro is missing from libc-shim"
#endif

#ifndef NT_ARM_VFP
#error "elf.h:NT_ARM_VFP macro is missing from libc-shim"
#endif

#ifndef NT_ARM_ZA
#error "elf.h:NT_ARM_ZA macro is missing from libc-shim"
#endif

#ifndef NT_ARM_ZT
#error "elf.h:NT_ARM_ZT macro is missing from libc-shim"
#endif

#ifndef NT_ASRS
#error "elf.h:NT_ASRS macro is missing from libc-shim"
#endif

#ifndef NT_AUXV
#error "elf.h:NT_AUXV macro is missing from libc-shim"
#endif

#ifndef NT_FDO_DLOPEN_METADATA
#error "elf.h:NT_FDO_DLOPEN_METADATA macro is missing from libc-shim"
#endif

#ifndef NT_FDO_PACKAGING_METADATA
#error "elf.h:NT_FDO_PACKAGING_METADATA macro is missing from libc-shim"
#endif

#ifndef NT_FILE
#error "elf.h:NT_FILE macro is missing from libc-shim"
#endif

#ifndef NT_FPREGSET
#error "elf.h:NT_FPREGSET macro is missing from libc-shim"
#endif

#ifndef NT_GNU_ABI_TAG
#error "elf.h:NT_GNU_ABI_TAG macro is missing from libc-shim"
#endif

#ifndef NT_GNU_BUILD_ID
#error "elf.h:NT_GNU_BUILD_ID macro is missing from libc-shim"
#endif

#ifndef NT_GNU_GOLD_VERSION
#error "elf.h:NT_GNU_GOLD_VERSION macro is missing from libc-shim"
#endif

#ifndef NT_GNU_HWCAP
#error "elf.h:NT_GNU_HWCAP macro is missing from libc-shim"
#endif

#ifndef NT_GNU_PROPERTY_TYPE_0
#error "elf.h:NT_GNU_PROPERTY_TYPE_0 macro is missing from libc-shim"
#endif

#ifndef NT_GWINDOWS
#error "elf.h:NT_GWINDOWS macro is missing from libc-shim"
#endif

#ifndef NT_LOONGARCH_CPUCFG
#error "elf.h:NT_LOONGARCH_CPUCFG macro is missing from libc-shim"
#endif

#ifndef NT_LOONGARCH_CSR
#error "elf.h:NT_LOONGARCH_CSR macro is missing from libc-shim"
#endif

#ifndef NT_LOONGARCH_HW_BREAK
#error "elf.h:NT_LOONGARCH_HW_BREAK macro is missing from libc-shim"
#endif

#ifndef NT_LOONGARCH_HW_WATCH
#error "elf.h:NT_LOONGARCH_HW_WATCH macro is missing from libc-shim"
#endif

#ifndef NT_LOONGARCH_LASX
#error "elf.h:NT_LOONGARCH_LASX macro is missing from libc-shim"
#endif

#ifndef NT_LOONGARCH_LBT
#error "elf.h:NT_LOONGARCH_LBT macro is missing from libc-shim"
#endif

#ifndef NT_LOONGARCH_LSX
#error "elf.h:NT_LOONGARCH_LSX macro is missing from libc-shim"
#endif

#ifndef NT_LWPSINFO
#error "elf.h:NT_LWPSINFO macro is missing from libc-shim"
#endif

#ifndef NT_LWPSTATUS
#error "elf.h:NT_LWPSTATUS macro is missing from libc-shim"
#endif

#ifndef NT_MIPS_DSP
#error "elf.h:NT_MIPS_DSP macro is missing from libc-shim"
#endif

#ifndef NT_MIPS_FP_MODE
#error "elf.h:NT_MIPS_FP_MODE macro is missing from libc-shim"
#endif

#ifndef NT_MIPS_MSA
#error "elf.h:NT_MIPS_MSA macro is missing from libc-shim"
#endif

#ifndef NT_PLATFORM
#error "elf.h:NT_PLATFORM macro is missing from libc-shim"
#endif

#ifndef NT_PPC_DEXCR
#error "elf.h:NT_PPC_DEXCR macro is missing from libc-shim"
#endif

#ifndef NT_PPC_DSCR
#error "elf.h:NT_PPC_DSCR macro is missing from libc-shim"
#endif

#ifndef NT_PPC_EBB
#error "elf.h:NT_PPC_EBB macro is missing from libc-shim"
#endif

#ifndef NT_PPC_HASHKEYR
#error "elf.h:NT_PPC_HASHKEYR macro is missing from libc-shim"
#endif

#ifndef NT_PPC_PKEY
#error "elf.h:NT_PPC_PKEY macro is missing from libc-shim"
#endif

#ifndef NT_PPC_PMU
#error "elf.h:NT_PPC_PMU macro is missing from libc-shim"
#endif

#ifndef NT_PPC_PPR
#error "elf.h:NT_PPC_PPR macro is missing from libc-shim"
#endif

#ifndef NT_PPC_SPE
#error "elf.h:NT_PPC_SPE macro is missing from libc-shim"
#endif

#ifndef NT_PPC_TAR
#error "elf.h:NT_PPC_TAR macro is missing from libc-shim"
#endif

#ifndef NT_PPC_TM_CDSCR
#error "elf.h:NT_PPC_TM_CDSCR macro is missing from libc-shim"
#endif

#ifndef NT_PPC_TM_CFPR
#error "elf.h:NT_PPC_TM_CFPR macro is missing from libc-shim"
#endif

#ifndef NT_PPC_TM_CGPR
#error "elf.h:NT_PPC_TM_CGPR macro is missing from libc-shim"
#endif

#ifndef NT_PPC_TM_CPPR
#error "elf.h:NT_PPC_TM_CPPR macro is missing from libc-shim"
#endif

#ifndef NT_PPC_TM_CTAR
#error "elf.h:NT_PPC_TM_CTAR macro is missing from libc-shim"
#endif

#ifndef NT_PPC_TM_CVMX
#error "elf.h:NT_PPC_TM_CVMX macro is missing from libc-shim"
#endif

#ifndef NT_PPC_TM_CVSX
#error "elf.h:NT_PPC_TM_CVSX macro is missing from libc-shim"
#endif

#ifndef NT_PPC_TM_SPR
#error "elf.h:NT_PPC_TM_SPR macro is missing from libc-shim"
#endif

#ifndef NT_PPC_VMX
#error "elf.h:NT_PPC_VMX macro is missing from libc-shim"
#endif

#ifndef NT_PPC_VSX
#error "elf.h:NT_PPC_VSX macro is missing from libc-shim"
#endif

#ifndef NT_PRCRED
#error "elf.h:NT_PRCRED macro is missing from libc-shim"
#endif

#ifndef NT_PRFPREG
#error "elf.h:NT_PRFPREG macro is missing from libc-shim"
#endif

#ifndef NT_PRFPXREG
#error "elf.h:NT_PRFPXREG macro is missing from libc-shim"
#endif

#ifndef NT_PRPSINFO
#error "elf.h:NT_PRPSINFO macro is missing from libc-shim"
#endif

#ifndef NT_PRSTATUS
#error "elf.h:NT_PRSTATUS macro is missing from libc-shim"
#endif

#ifndef NT_PRXFPREG
#error "elf.h:NT_PRXFPREG macro is missing from libc-shim"
#endif

#ifndef NT_PRXREG
#error "elf.h:NT_PRXREG macro is missing from libc-shim"
#endif

#ifndef NT_PSINFO
#error "elf.h:NT_PSINFO macro is missing from libc-shim"
#endif

#ifndef NT_PSTATUS
#error "elf.h:NT_PSTATUS macro is missing from libc-shim"
#endif

#ifndef NT_RISCV_CSR
#error "elf.h:NT_RISCV_CSR macro is missing from libc-shim"
#endif

#ifndef NT_RISCV_TAGGED_ADDR_CTRL
#error "elf.h:NT_RISCV_TAGGED_ADDR_CTRL macro is missing from libc-shim"
#endif

#ifndef NT_RISCV_USER_CFI
#error "elf.h:NT_RISCV_USER_CFI macro is missing from libc-shim"
#endif

#ifndef NT_RISCV_VECTOR
#error "elf.h:NT_RISCV_VECTOR macro is missing from libc-shim"
#endif

#ifndef NT_S390_CTRS
#error "elf.h:NT_S390_CTRS macro is missing from libc-shim"
#endif

#ifndef NT_S390_GS_BC
#error "elf.h:NT_S390_GS_BC macro is missing from libc-shim"
#endif

#ifndef NT_S390_GS_CB
#error "elf.h:NT_S390_GS_CB macro is missing from libc-shim"
#endif

#ifndef NT_S390_HIGH_GPRS
#error "elf.h:NT_S390_HIGH_GPRS macro is missing from libc-shim"
#endif

#ifndef NT_S390_LAST_BREAK
#error "elf.h:NT_S390_LAST_BREAK macro is missing from libc-shim"
#endif

#ifndef NT_S390_PREFIX
#error "elf.h:NT_S390_PREFIX macro is missing from libc-shim"
#endif

#ifndef NT_S390_PV_CPU_DATA
#error "elf.h:NT_S390_PV_CPU_DATA macro is missing from libc-shim"
#endif

#ifndef NT_S390_RI_CB
#error "elf.h:NT_S390_RI_CB macro is missing from libc-shim"
#endif

#ifndef NT_S390_SYSTEM_CALL
#error "elf.h:NT_S390_SYSTEM_CALL macro is missing from libc-shim"
#endif

#ifndef NT_S390_TDB
#error "elf.h:NT_S390_TDB macro is missing from libc-shim"
#endif

#ifndef NT_S390_TIMER
#error "elf.h:NT_S390_TIMER macro is missing from libc-shim"
#endif

#ifndef NT_S390_TODCMP
#error "elf.h:NT_S390_TODCMP macro is missing from libc-shim"
#endif

#ifndef NT_S390_TODPREG
#error "elf.h:NT_S390_TODPREG macro is missing from libc-shim"
#endif

#ifndef NT_S390_VXRS_HIGH
#error "elf.h:NT_S390_VXRS_HIGH macro is missing from libc-shim"
#endif

#ifndef NT_S390_VXRS_LOW
#error "elf.h:NT_S390_VXRS_LOW macro is missing from libc-shim"
#endif

#ifndef NT_SIGINFO
#error "elf.h:NT_SIGINFO macro is missing from libc-shim"
#endif

#ifndef NT_TASKSTRUCT
#error "elf.h:NT_TASKSTRUCT macro is missing from libc-shim"
#endif

#ifndef NT_UTSNAME
#error "elf.h:NT_UTSNAME macro is missing from libc-shim"
#endif

#ifndef NT_VERSION
#error "elf.h:NT_VERSION macro is missing from libc-shim"
#endif

#ifndef NT_VMCOREDD
#error "elf.h:NT_VMCOREDD macro is missing from libc-shim"
#endif

#ifndef NT_X86_SHSTK
#error "elf.h:NT_X86_SHSTK macro is missing from libc-shim"
#endif

#ifndef NT_X86_XSAVE_LAYOUT
#error "elf.h:NT_X86_XSAVE_LAYOUT macro is missing from libc-shim"
#endif

#ifndef NT_X86_XSTATE
#error "elf.h:NT_X86_XSTATE macro is missing from libc-shim"
#endif

#ifndef ODK_EXCEPTIONS
#error "elf.h:ODK_EXCEPTIONS macro is missing from libc-shim"
#endif

#ifndef ODK_FILL
#error "elf.h:ODK_FILL macro is missing from libc-shim"
#endif

#ifndef ODK_HWAND
#error "elf.h:ODK_HWAND macro is missing from libc-shim"
#endif

#ifndef ODK_HWOR
#error "elf.h:ODK_HWOR macro is missing from libc-shim"
#endif

#ifndef ODK_HWPATCH
#error "elf.h:ODK_HWPATCH macro is missing from libc-shim"
#endif

#ifndef ODK_NULL
#error "elf.h:ODK_NULL macro is missing from libc-shim"
#endif

#ifndef ODK_PAD
#error "elf.h:ODK_PAD macro is missing from libc-shim"
#endif

#ifndef ODK_REGINFO
#error "elf.h:ODK_REGINFO macro is missing from libc-shim"
#endif

#ifndef ODK_TAGS
#error "elf.h:ODK_TAGS macro is missing from libc-shim"
#endif

#ifndef OEX_DISMISS
#error "elf.h:OEX_DISMISS macro is missing from libc-shim"
#endif

#ifndef OEX_FPDBUG
#error "elf.h:OEX_FPDBUG macro is missing from libc-shim"
#endif

#ifndef OEX_FPU_DIV0
#error "elf.h:OEX_FPU_DIV0 macro is missing from libc-shim"
#endif

#ifndef OEX_FPU_INEX
#error "elf.h:OEX_FPU_INEX macro is missing from libc-shim"
#endif

#ifndef OEX_FPU_INVAL
#error "elf.h:OEX_FPU_INVAL macro is missing from libc-shim"
#endif

#ifndef OEX_FPU_MAX
#error "elf.h:OEX_FPU_MAX macro is missing from libc-shim"
#endif

#ifndef OEX_FPU_MIN
#error "elf.h:OEX_FPU_MIN macro is missing from libc-shim"
#endif

#ifndef OEX_FPU_OFLO
#error "elf.h:OEX_FPU_OFLO macro is missing from libc-shim"
#endif

#ifndef OEX_FPU_UFLO
#error "elf.h:OEX_FPU_UFLO macro is missing from libc-shim"
#endif

#ifndef OEX_PAGE0
#error "elf.h:OEX_PAGE0 macro is missing from libc-shim"
#endif

#ifndef OEX_PRECISEFP
#error "elf.h:OEX_PRECISEFP macro is missing from libc-shim"
#endif

#ifndef OEX_SMM
#error "elf.h:OEX_SMM macro is missing from libc-shim"
#endif

#ifndef OHWA0_R4KEOP_CHECKED
#error "elf.h:OHWA0_R4KEOP_CHECKED macro is missing from libc-shim"
#endif

#ifndef OHWA1_R4KEOP_CLEAN
#error "elf.h:OHWA1_R4KEOP_CLEAN macro is missing from libc-shim"
#endif

#ifndef OHW_R4KEOP
#error "elf.h:OHW_R4KEOP macro is missing from libc-shim"
#endif

#ifndef OHW_R5KCVTL
#error "elf.h:OHW_R5KCVTL macro is missing from libc-shim"
#endif

#ifndef OHW_R5KEOP
#error "elf.h:OHW_R5KEOP macro is missing from libc-shim"
#endif

#ifndef OHW_R8KPFETCH
#error "elf.h:OHW_R8KPFETCH macro is missing from libc-shim"
#endif

#ifndef OPAD_POSTFIX
#error "elf.h:OPAD_POSTFIX macro is missing from libc-shim"
#endif

#ifndef OPAD_PREFIX
#error "elf.h:OPAD_PREFIX macro is missing from libc-shim"
#endif

#ifndef OPAD_SYMBOL
#error "elf.h:OPAD_SYMBOL macro is missing from libc-shim"
#endif

#ifndef PF_ARM_ABS
#error "elf.h:PF_ARM_ABS macro is missing from libc-shim"
#endif

#ifndef PF_ARM_PI
#error "elf.h:PF_ARM_PI macro is missing from libc-shim"
#endif

#ifndef PF_ARM_SB
#error "elf.h:PF_ARM_SB macro is missing from libc-shim"
#endif

#ifndef PF_HP_CODE
#error "elf.h:PF_HP_CODE macro is missing from libc-shim"
#endif

#ifndef PF_HP_FAR_SHARED
#error "elf.h:PF_HP_FAR_SHARED macro is missing from libc-shim"
#endif

#ifndef PF_HP_LAZYSWAP
#error "elf.h:PF_HP_LAZYSWAP macro is missing from libc-shim"
#endif

#ifndef PF_HP_MODIFY
#error "elf.h:PF_HP_MODIFY macro is missing from libc-shim"
#endif

#ifndef PF_HP_NEAR_SHARED
#error "elf.h:PF_HP_NEAR_SHARED macro is missing from libc-shim"
#endif

#ifndef PF_HP_PAGE_SIZE
#error "elf.h:PF_HP_PAGE_SIZE macro is missing from libc-shim"
#endif

#ifndef PF_HP_SBP
#error "elf.h:PF_HP_SBP macro is missing from libc-shim"
#endif

#ifndef PF_IA_64_NORECOV
#error "elf.h:PF_IA_64_NORECOV macro is missing from libc-shim"
#endif

#ifndef PF_MASKOS
#error "elf.h:PF_MASKOS macro is missing from libc-shim"
#endif

#ifndef PF_MASKPROC
#error "elf.h:PF_MASKPROC macro is missing from libc-shim"
#endif

#ifndef PF_MIPS_LOCAL
#error "elf.h:PF_MIPS_LOCAL macro is missing from libc-shim"
#endif

#ifndef PF_PARISC_SBP
#error "elf.h:PF_PARISC_SBP macro is missing from libc-shim"
#endif

#ifndef PF_R
#error "elf.h:PF_R macro is missing from libc-shim"
#endif

#ifndef PF_W
#error "elf.h:PF_W macro is missing from libc-shim"
#endif

#ifndef PF_X
#error "elf.h:PF_X macro is missing from libc-shim"
#endif

#ifndef PN_XNUM
#error "elf.h:PN_XNUM macro is missing from libc-shim"
#endif

#ifndef PPC64_LOCAL_ENTRY_OFFSET
#error "elf.h:PPC64_LOCAL_ENTRY_OFFSET macro is missing from libc-shim"
#endif

#ifndef PPC64_OPT_LOCALENTRY
#error "elf.h:PPC64_OPT_LOCALENTRY macro is missing from libc-shim"
#endif

#ifndef PPC64_OPT_MULTI_TOC
#error "elf.h:PPC64_OPT_MULTI_TOC macro is missing from libc-shim"
#endif

#ifndef PPC64_OPT_TLS
#error "elf.h:PPC64_OPT_TLS macro is missing from libc-shim"
#endif

#ifndef PPC_OPT_TLS
#error "elf.h:PPC_OPT_TLS macro is missing from libc-shim"
#endif

#ifndef PT_AARCH64_MEMTAG_MTE
#error "elf.h:PT_AARCH64_MEMTAG_MTE macro is missing from libc-shim"
#endif

#ifndef PT_ARM_EXIDX
#error "elf.h:PT_ARM_EXIDX macro is missing from libc-shim"
#endif

#ifndef PT_DYNAMIC
#error "elf.h:PT_DYNAMIC macro is missing from libc-shim"
#endif

#ifndef PT_GNU_EH_FRAME
#error "elf.h:PT_GNU_EH_FRAME macro is missing from libc-shim"
#endif

#ifndef PT_GNU_PROPERTY
#error "elf.h:PT_GNU_PROPERTY macro is missing from libc-shim"
#endif

#ifndef PT_GNU_RELRO
#error "elf.h:PT_GNU_RELRO macro is missing from libc-shim"
#endif

#ifndef PT_GNU_SFRAME
#error "elf.h:PT_GNU_SFRAME macro is missing from libc-shim"
#endif

#ifndef PT_GNU_STACK
#error "elf.h:PT_GNU_STACK macro is missing from libc-shim"
#endif

#ifndef PT_HIOS
#error "elf.h:PT_HIOS macro is missing from libc-shim"
#endif

#ifndef PT_HIPROC
#error "elf.h:PT_HIPROC macro is missing from libc-shim"
#endif

#ifndef PT_HISUNW
#error "elf.h:PT_HISUNW macro is missing from libc-shim"
#endif

#ifndef PT_HP_CORE_COMM
#error "elf.h:PT_HP_CORE_COMM macro is missing from libc-shim"
#endif

#ifndef PT_HP_CORE_KERNEL
#error "elf.h:PT_HP_CORE_KERNEL macro is missing from libc-shim"
#endif

#ifndef PT_HP_CORE_LOADABLE
#error "elf.h:PT_HP_CORE_LOADABLE macro is missing from libc-shim"
#endif

#ifndef PT_HP_CORE_MMF
#error "elf.h:PT_HP_CORE_MMF macro is missing from libc-shim"
#endif

#ifndef PT_HP_CORE_NONE
#error "elf.h:PT_HP_CORE_NONE macro is missing from libc-shim"
#endif

#ifndef PT_HP_CORE_PROC
#error "elf.h:PT_HP_CORE_PROC macro is missing from libc-shim"
#endif

#ifndef PT_HP_CORE_SHM
#error "elf.h:PT_HP_CORE_SHM macro is missing from libc-shim"
#endif

#ifndef PT_HP_CORE_STACK
#error "elf.h:PT_HP_CORE_STACK macro is missing from libc-shim"
#endif

#ifndef PT_HP_CORE_VERSION
#error "elf.h:PT_HP_CORE_VERSION macro is missing from libc-shim"
#endif

#ifndef PT_HP_FASTBIND
#error "elf.h:PT_HP_FASTBIND macro is missing from libc-shim"
#endif

#ifndef PT_HP_HSL_ANNOT
#error "elf.h:PT_HP_HSL_ANNOT macro is missing from libc-shim"
#endif

#ifndef PT_HP_OPT_ANNOT
#error "elf.h:PT_HP_OPT_ANNOT macro is missing from libc-shim"
#endif

#ifndef PT_HP_PARALLEL
#error "elf.h:PT_HP_PARALLEL macro is missing from libc-shim"
#endif

#ifndef PT_HP_STACK
#error "elf.h:PT_HP_STACK macro is missing from libc-shim"
#endif

#ifndef PT_HP_TLS
#error "elf.h:PT_HP_TLS macro is missing from libc-shim"
#endif

#ifndef PT_IA_64_ARCHEXT
#error "elf.h:PT_IA_64_ARCHEXT macro is missing from libc-shim"
#endif

#ifndef PT_IA_64_HP_HSL_ANOT
#error "elf.h:PT_IA_64_HP_HSL_ANOT macro is missing from libc-shim"
#endif

#ifndef PT_IA_64_HP_OPT_ANOT
#error "elf.h:PT_IA_64_HP_OPT_ANOT macro is missing from libc-shim"
#endif

#ifndef PT_IA_64_HP_STACK
#error "elf.h:PT_IA_64_HP_STACK macro is missing from libc-shim"
#endif

#ifndef PT_IA_64_UNWIND
#error "elf.h:PT_IA_64_UNWIND macro is missing from libc-shim"
#endif

#ifndef PT_INTERP
#error "elf.h:PT_INTERP macro is missing from libc-shim"
#endif

#ifndef PT_LOAD
#error "elf.h:PT_LOAD macro is missing from libc-shim"
#endif

#ifndef PT_LOOS
#error "elf.h:PT_LOOS macro is missing from libc-shim"
#endif

#ifndef PT_LOPROC
#error "elf.h:PT_LOPROC macro is missing from libc-shim"
#endif

#ifndef PT_LOSUNW
#error "elf.h:PT_LOSUNW macro is missing from libc-shim"
#endif

#ifndef PT_MIPS_ABIFLAGS
#error "elf.h:PT_MIPS_ABIFLAGS macro is missing from libc-shim"
#endif

#ifndef PT_MIPS_OPTIONS
#error "elf.h:PT_MIPS_OPTIONS macro is missing from libc-shim"
#endif

#ifndef PT_MIPS_REGINFO
#error "elf.h:PT_MIPS_REGINFO macro is missing from libc-shim"
#endif

#ifndef PT_MIPS_RTPROC
#error "elf.h:PT_MIPS_RTPROC macro is missing from libc-shim"
#endif

#ifndef PT_NOTE
#error "elf.h:PT_NOTE macro is missing from libc-shim"
#endif

#ifndef PT_NULL
#error "elf.h:PT_NULL macro is missing from libc-shim"
#endif

#ifndef PT_NUM
#error "elf.h:PT_NUM macro is missing from libc-shim"
#endif

#ifndef PT_PARISC_ARCHEXT
#error "elf.h:PT_PARISC_ARCHEXT macro is missing from libc-shim"
#endif

#ifndef PT_PARISC_UNWIND
#error "elf.h:PT_PARISC_UNWIND macro is missing from libc-shim"
#endif

#ifndef PT_PHDR
#error "elf.h:PT_PHDR macro is missing from libc-shim"
#endif

#ifndef PT_RISCV_ATTRIBUTES
#error "elf.h:PT_RISCV_ATTRIBUTES macro is missing from libc-shim"
#endif

#ifndef PT_SHLIB
#error "elf.h:PT_SHLIB macro is missing from libc-shim"
#endif

#ifndef PT_SUNWBSS
#error "elf.h:PT_SUNWBSS macro is missing from libc-shim"
#endif

#ifndef PT_SUNWSTACK
#error "elf.h:PT_SUNWSTACK macro is missing from libc-shim"
#endif

#ifndef PT_TLS
#error "elf.h:PT_TLS macro is missing from libc-shim"
#endif

#ifndef RHF_CORD
#error "elf.h:RHF_CORD macro is missing from libc-shim"
#endif

#ifndef RHF_DEFAULT_DELAY_LOAD
#error "elf.h:RHF_DEFAULT_DELAY_LOAD macro is missing from libc-shim"
#endif

#ifndef RHF_DELTA_C_PLUS_PLUS
#error "elf.h:RHF_DELTA_C_PLUS_PLUS macro is missing from libc-shim"
#endif

#ifndef RHF_GUARANTEE_INIT
#error "elf.h:RHF_GUARANTEE_INIT macro is missing from libc-shim"
#endif

#ifndef RHF_GUARANTEE_START_INIT
#error "elf.h:RHF_GUARANTEE_START_INIT macro is missing from libc-shim"
#endif

#ifndef RHF_NONE
#error "elf.h:RHF_NONE macro is missing from libc-shim"
#endif

#ifndef RHF_NOTPOT
#error "elf.h:RHF_NOTPOT macro is missing from libc-shim"
#endif

#ifndef RHF_NO_LIBRARY_REPLACEMENT
#error "elf.h:RHF_NO_LIBRARY_REPLACEMENT macro is missing from libc-shim"
#endif

#ifndef RHF_NO_MOVE
#error "elf.h:RHF_NO_MOVE macro is missing from libc-shim"
#endif

#ifndef RHF_NO_UNRES_UNDEF
#error "elf.h:RHF_NO_UNRES_UNDEF macro is missing from libc-shim"
#endif

#ifndef RHF_PIXIE
#error "elf.h:RHF_PIXIE macro is missing from libc-shim"
#endif

#ifndef RHF_QUICKSTART
#error "elf.h:RHF_QUICKSTART macro is missing from libc-shim"
#endif

#ifndef RHF_REQUICKSTART
#error "elf.h:RHF_REQUICKSTART macro is missing from libc-shim"
#endif

#ifndef RHF_REQUICKSTARTED
#error "elf.h:RHF_REQUICKSTARTED macro is missing from libc-shim"
#endif

#ifndef RHF_RLD_ORDER_SAFE
#error "elf.h:RHF_RLD_ORDER_SAFE macro is missing from libc-shim"
#endif

#ifndef RHF_SGI_ONLY
#error "elf.h:RHF_SGI_ONLY macro is missing from libc-shim"
#endif

#ifndef R_386_16
#error "elf.h:R_386_16 macro is missing from libc-shim"
#endif

#ifndef R_386_32
#error "elf.h:R_386_32 macro is missing from libc-shim"
#endif

#ifndef R_386_32PLT
#error "elf.h:R_386_32PLT macro is missing from libc-shim"
#endif

#ifndef R_386_8
#error "elf.h:R_386_8 macro is missing from libc-shim"
#endif

#ifndef R_386_COPY
#error "elf.h:R_386_COPY macro is missing from libc-shim"
#endif

#ifndef R_386_GLOB_DAT
#error "elf.h:R_386_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_386_GOT32
#error "elf.h:R_386_GOT32 macro is missing from libc-shim"
#endif

#ifndef R_386_GOT32X
#error "elf.h:R_386_GOT32X macro is missing from libc-shim"
#endif

#ifndef R_386_GOTOFF
#error "elf.h:R_386_GOTOFF macro is missing from libc-shim"
#endif

#ifndef R_386_GOTPC
#error "elf.h:R_386_GOTPC macro is missing from libc-shim"
#endif

#ifndef R_386_IRELATIVE
#error "elf.h:R_386_IRELATIVE macro is missing from libc-shim"
#endif

#ifndef R_386_JMP_SLOT
#error "elf.h:R_386_JMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_386_NONE
#error "elf.h:R_386_NONE macro is missing from libc-shim"
#endif

#ifndef R_386_NUM
#error "elf.h:R_386_NUM macro is missing from libc-shim"
#endif

#ifndef R_386_PC16
#error "elf.h:R_386_PC16 macro is missing from libc-shim"
#endif

#ifndef R_386_PC32
#error "elf.h:R_386_PC32 macro is missing from libc-shim"
#endif

#ifndef R_386_PC8
#error "elf.h:R_386_PC8 macro is missing from libc-shim"
#endif

#ifndef R_386_PLT32
#error "elf.h:R_386_PLT32 macro is missing from libc-shim"
#endif

#ifndef R_386_RELATIVE
#error "elf.h:R_386_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_386_SIZE32
#error "elf.h:R_386_SIZE32 macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_DESC
#error "elf.h:R_386_TLS_DESC macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_DESC_CALL
#error "elf.h:R_386_TLS_DESC_CALL macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_DTPMOD32
#error "elf.h:R_386_TLS_DTPMOD32 macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_DTPOFF32
#error "elf.h:R_386_TLS_DTPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_GD
#error "elf.h:R_386_TLS_GD macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_GD_32
#error "elf.h:R_386_TLS_GD_32 macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_GD_CALL
#error "elf.h:R_386_TLS_GD_CALL macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_GD_POP
#error "elf.h:R_386_TLS_GD_POP macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_GD_PUSH
#error "elf.h:R_386_TLS_GD_PUSH macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_GOTDESC
#error "elf.h:R_386_TLS_GOTDESC macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_GOTIE
#error "elf.h:R_386_TLS_GOTIE macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_IE
#error "elf.h:R_386_TLS_IE macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_IE_32
#error "elf.h:R_386_TLS_IE_32 macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_LDM
#error "elf.h:R_386_TLS_LDM macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_LDM_32
#error "elf.h:R_386_TLS_LDM_32 macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_LDM_CALL
#error "elf.h:R_386_TLS_LDM_CALL macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_LDM_POP
#error "elf.h:R_386_TLS_LDM_POP macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_LDM_PUSH
#error "elf.h:R_386_TLS_LDM_PUSH macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_LDO_32
#error "elf.h:R_386_TLS_LDO_32 macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_LE
#error "elf.h:R_386_TLS_LE macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_LE_32
#error "elf.h:R_386_TLS_LE_32 macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_TPOFF
#error "elf.h:R_386_TLS_TPOFF macro is missing from libc-shim"
#endif

#ifndef R_386_TLS_TPOFF32
#error "elf.h:R_386_TLS_TPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_390_12
#error "elf.h:R_390_12 macro is missing from libc-shim"
#endif

#ifndef R_390_16
#error "elf.h:R_390_16 macro is missing from libc-shim"
#endif

#ifndef R_390_20
#error "elf.h:R_390_20 macro is missing from libc-shim"
#endif

#ifndef R_390_32
#error "elf.h:R_390_32 macro is missing from libc-shim"
#endif

#ifndef R_390_64
#error "elf.h:R_390_64 macro is missing from libc-shim"
#endif

#ifndef R_390_8
#error "elf.h:R_390_8 macro is missing from libc-shim"
#endif

#ifndef R_390_COPY
#error "elf.h:R_390_COPY macro is missing from libc-shim"
#endif

#ifndef R_390_GLOB_DAT
#error "elf.h:R_390_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_390_GOT12
#error "elf.h:R_390_GOT12 macro is missing from libc-shim"
#endif

#ifndef R_390_GOT16
#error "elf.h:R_390_GOT16 macro is missing from libc-shim"
#endif

#ifndef R_390_GOT20
#error "elf.h:R_390_GOT20 macro is missing from libc-shim"
#endif

#ifndef R_390_GOT32
#error "elf.h:R_390_GOT32 macro is missing from libc-shim"
#endif

#ifndef R_390_GOT64
#error "elf.h:R_390_GOT64 macro is missing from libc-shim"
#endif

#ifndef R_390_GOTENT
#error "elf.h:R_390_GOTENT macro is missing from libc-shim"
#endif

#ifndef R_390_GOTOFF16
#error "elf.h:R_390_GOTOFF16 macro is missing from libc-shim"
#endif

#ifndef R_390_GOTOFF32
#error "elf.h:R_390_GOTOFF32 macro is missing from libc-shim"
#endif

#ifndef R_390_GOTOFF64
#error "elf.h:R_390_GOTOFF64 macro is missing from libc-shim"
#endif

#ifndef R_390_GOTPC
#error "elf.h:R_390_GOTPC macro is missing from libc-shim"
#endif

#ifndef R_390_GOTPCDBL
#error "elf.h:R_390_GOTPCDBL macro is missing from libc-shim"
#endif

#ifndef R_390_GOTPLT12
#error "elf.h:R_390_GOTPLT12 macro is missing from libc-shim"
#endif

#ifndef R_390_GOTPLT16
#error "elf.h:R_390_GOTPLT16 macro is missing from libc-shim"
#endif

#ifndef R_390_GOTPLT20
#error "elf.h:R_390_GOTPLT20 macro is missing from libc-shim"
#endif

#ifndef R_390_GOTPLT32
#error "elf.h:R_390_GOTPLT32 macro is missing from libc-shim"
#endif

#ifndef R_390_GOTPLT64
#error "elf.h:R_390_GOTPLT64 macro is missing from libc-shim"
#endif

#ifndef R_390_GOTPLTENT
#error "elf.h:R_390_GOTPLTENT macro is missing from libc-shim"
#endif

#ifndef R_390_IRELATIVE
#error "elf.h:R_390_IRELATIVE macro is missing from libc-shim"
#endif

#ifndef R_390_JMP_SLOT
#error "elf.h:R_390_JMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_390_NONE
#error "elf.h:R_390_NONE macro is missing from libc-shim"
#endif

#ifndef R_390_NUM
#error "elf.h:R_390_NUM macro is missing from libc-shim"
#endif

#ifndef R_390_PC16
#error "elf.h:R_390_PC16 macro is missing from libc-shim"
#endif

#ifndef R_390_PC16DBL
#error "elf.h:R_390_PC16DBL macro is missing from libc-shim"
#endif

#ifndef R_390_PC32
#error "elf.h:R_390_PC32 macro is missing from libc-shim"
#endif

#ifndef R_390_PC32DBL
#error "elf.h:R_390_PC32DBL macro is missing from libc-shim"
#endif

#ifndef R_390_PC64
#error "elf.h:R_390_PC64 macro is missing from libc-shim"
#endif

#ifndef R_390_PLT16DBL
#error "elf.h:R_390_PLT16DBL macro is missing from libc-shim"
#endif

#ifndef R_390_PLT32
#error "elf.h:R_390_PLT32 macro is missing from libc-shim"
#endif

#ifndef R_390_PLT32DBL
#error "elf.h:R_390_PLT32DBL macro is missing from libc-shim"
#endif

#ifndef R_390_PLT64
#error "elf.h:R_390_PLT64 macro is missing from libc-shim"
#endif

#ifndef R_390_PLTOFF16
#error "elf.h:R_390_PLTOFF16 macro is missing from libc-shim"
#endif

#ifndef R_390_PLTOFF32
#error "elf.h:R_390_PLTOFF32 macro is missing from libc-shim"
#endif

#ifndef R_390_PLTOFF64
#error "elf.h:R_390_PLTOFF64 macro is missing from libc-shim"
#endif

#ifndef R_390_RELATIVE
#error "elf.h:R_390_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_DTPMOD
#error "elf.h:R_390_TLS_DTPMOD macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_DTPOFF
#error "elf.h:R_390_TLS_DTPOFF macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_GD32
#error "elf.h:R_390_TLS_GD32 macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_GD64
#error "elf.h:R_390_TLS_GD64 macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_GDCALL
#error "elf.h:R_390_TLS_GDCALL macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_GOTIE12
#error "elf.h:R_390_TLS_GOTIE12 macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_GOTIE20
#error "elf.h:R_390_TLS_GOTIE20 macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_GOTIE32
#error "elf.h:R_390_TLS_GOTIE32 macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_GOTIE64
#error "elf.h:R_390_TLS_GOTIE64 macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_IE32
#error "elf.h:R_390_TLS_IE32 macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_IE64
#error "elf.h:R_390_TLS_IE64 macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_IEENT
#error "elf.h:R_390_TLS_IEENT macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_LDCALL
#error "elf.h:R_390_TLS_LDCALL macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_LDM32
#error "elf.h:R_390_TLS_LDM32 macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_LDM64
#error "elf.h:R_390_TLS_LDM64 macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_LDO32
#error "elf.h:R_390_TLS_LDO32 macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_LDO64
#error "elf.h:R_390_TLS_LDO64 macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_LE32
#error "elf.h:R_390_TLS_LE32 macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_LE64
#error "elf.h:R_390_TLS_LE64 macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_LOAD
#error "elf.h:R_390_TLS_LOAD macro is missing from libc-shim"
#endif

#ifndef R_390_TLS_TPOFF
#error "elf.h:R_390_TLS_TPOFF macro is missing from libc-shim"
#endif

#ifndef R_68K_16
#error "elf.h:R_68K_16 macro is missing from libc-shim"
#endif

#ifndef R_68K_32
#error "elf.h:R_68K_32 macro is missing from libc-shim"
#endif

#ifndef R_68K_8
#error "elf.h:R_68K_8 macro is missing from libc-shim"
#endif

#ifndef R_68K_COPY
#error "elf.h:R_68K_COPY macro is missing from libc-shim"
#endif

#ifndef R_68K_GLOB_DAT
#error "elf.h:R_68K_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_68K_GOT16
#error "elf.h:R_68K_GOT16 macro is missing from libc-shim"
#endif

#ifndef R_68K_GOT16O
#error "elf.h:R_68K_GOT16O macro is missing from libc-shim"
#endif

#ifndef R_68K_GOT32
#error "elf.h:R_68K_GOT32 macro is missing from libc-shim"
#endif

#ifndef R_68K_GOT32O
#error "elf.h:R_68K_GOT32O macro is missing from libc-shim"
#endif

#ifndef R_68K_GOT8
#error "elf.h:R_68K_GOT8 macro is missing from libc-shim"
#endif

#ifndef R_68K_GOT8O
#error "elf.h:R_68K_GOT8O macro is missing from libc-shim"
#endif

#ifndef R_68K_JMP_SLOT
#error "elf.h:R_68K_JMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_68K_NONE
#error "elf.h:R_68K_NONE macro is missing from libc-shim"
#endif

#ifndef R_68K_NUM
#error "elf.h:R_68K_NUM macro is missing from libc-shim"
#endif

#ifndef R_68K_PC16
#error "elf.h:R_68K_PC16 macro is missing from libc-shim"
#endif

#ifndef R_68K_PC32
#error "elf.h:R_68K_PC32 macro is missing from libc-shim"
#endif

#ifndef R_68K_PC8
#error "elf.h:R_68K_PC8 macro is missing from libc-shim"
#endif

#ifndef R_68K_PLT16
#error "elf.h:R_68K_PLT16 macro is missing from libc-shim"
#endif

#ifndef R_68K_PLT16O
#error "elf.h:R_68K_PLT16O macro is missing from libc-shim"
#endif

#ifndef R_68K_PLT32
#error "elf.h:R_68K_PLT32 macro is missing from libc-shim"
#endif

#ifndef R_68K_PLT32O
#error "elf.h:R_68K_PLT32O macro is missing from libc-shim"
#endif

#ifndef R_68K_PLT8
#error "elf.h:R_68K_PLT8 macro is missing from libc-shim"
#endif

#ifndef R_68K_PLT8O
#error "elf.h:R_68K_PLT8O macro is missing from libc-shim"
#endif

#ifndef R_68K_RELATIVE
#error "elf.h:R_68K_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_DTPMOD32
#error "elf.h:R_68K_TLS_DTPMOD32 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_DTPREL32
#error "elf.h:R_68K_TLS_DTPREL32 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_GD16
#error "elf.h:R_68K_TLS_GD16 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_GD32
#error "elf.h:R_68K_TLS_GD32 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_GD8
#error "elf.h:R_68K_TLS_GD8 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_IE16
#error "elf.h:R_68K_TLS_IE16 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_IE32
#error "elf.h:R_68K_TLS_IE32 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_IE8
#error "elf.h:R_68K_TLS_IE8 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_LDM16
#error "elf.h:R_68K_TLS_LDM16 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_LDM32
#error "elf.h:R_68K_TLS_LDM32 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_LDM8
#error "elf.h:R_68K_TLS_LDM8 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_LDO16
#error "elf.h:R_68K_TLS_LDO16 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_LDO32
#error "elf.h:R_68K_TLS_LDO32 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_LDO8
#error "elf.h:R_68K_TLS_LDO8 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_LE16
#error "elf.h:R_68K_TLS_LE16 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_LE32
#error "elf.h:R_68K_TLS_LE32 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_LE8
#error "elf.h:R_68K_TLS_LE8 macro is missing from libc-shim"
#endif

#ifndef R_68K_TLS_TPREL32
#error "elf.h:R_68K_TLS_TPREL32 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_ABS16
#error "elf.h:R_AARCH64_ABS16 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_ABS32
#error "elf.h:R_AARCH64_ABS32 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_ABS64
#error "elf.h:R_AARCH64_ABS64 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_ADD_ABS_LO12_NC
#error "elf.h:R_AARCH64_ADD_ABS_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_ADR_GOT_PAGE
#error "elf.h:R_AARCH64_ADR_GOT_PAGE macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_ADR_PREL_LO21
#error "elf.h:R_AARCH64_ADR_PREL_LO21 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_ADR_PREL_PG_HI21
#error "elf.h:R_AARCH64_ADR_PREL_PG_HI21 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_ADR_PREL_PG_HI21_NC
#error "elf.h:R_AARCH64_ADR_PREL_PG_HI21_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_CALL26
#error "elf.h:R_AARCH64_CALL26 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_CONDBR19
#error "elf.h:R_AARCH64_CONDBR19 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_COPY
#error "elf.h:R_AARCH64_COPY macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_GLOB_DAT
#error "elf.h:R_AARCH64_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_GOTREL32
#error "elf.h:R_AARCH64_GOTREL32 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_GOTREL64
#error "elf.h:R_AARCH64_GOTREL64 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_GOT_LD_PREL19
#error "elf.h:R_AARCH64_GOT_LD_PREL19 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_IRELATIVE
#error "elf.h:R_AARCH64_IRELATIVE macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_JUMP26
#error "elf.h:R_AARCH64_JUMP26 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_JUMP_SLOT
#error "elf.h:R_AARCH64_JUMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_LD64_GOTOFF_LO15
#error "elf.h:R_AARCH64_LD64_GOTOFF_LO15 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_LD64_GOTPAGE_LO15
#error "elf.h:R_AARCH64_LD64_GOTPAGE_LO15 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_LD64_GOT_LO12_NC
#error "elf.h:R_AARCH64_LD64_GOT_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_LDST128_ABS_LO12_NC
#error "elf.h:R_AARCH64_LDST128_ABS_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_LDST16_ABS_LO12_NC
#error "elf.h:R_AARCH64_LDST16_ABS_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_LDST32_ABS_LO12_NC
#error "elf.h:R_AARCH64_LDST32_ABS_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_LDST64_ABS_LO12_NC
#error "elf.h:R_AARCH64_LDST64_ABS_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_LDST8_ABS_LO12_NC
#error "elf.h:R_AARCH64_LDST8_ABS_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_LD_PREL_LO19
#error "elf.h:R_AARCH64_LD_PREL_LO19 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_GOTOFF_G0
#error "elf.h:R_AARCH64_MOVW_GOTOFF_G0 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_GOTOFF_G0_NC
#error "elf.h:R_AARCH64_MOVW_GOTOFF_G0_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_GOTOFF_G1
#error "elf.h:R_AARCH64_MOVW_GOTOFF_G1 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_GOTOFF_G1_NC
#error "elf.h:R_AARCH64_MOVW_GOTOFF_G1_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_GOTOFF_G2
#error "elf.h:R_AARCH64_MOVW_GOTOFF_G2 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_GOTOFF_G2_NC
#error "elf.h:R_AARCH64_MOVW_GOTOFF_G2_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_GOTOFF_G3
#error "elf.h:R_AARCH64_MOVW_GOTOFF_G3 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_PREL_G0
#error "elf.h:R_AARCH64_MOVW_PREL_G0 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_PREL_G0_NC
#error "elf.h:R_AARCH64_MOVW_PREL_G0_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_PREL_G1
#error "elf.h:R_AARCH64_MOVW_PREL_G1 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_PREL_G1_NC
#error "elf.h:R_AARCH64_MOVW_PREL_G1_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_PREL_G2
#error "elf.h:R_AARCH64_MOVW_PREL_G2 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_PREL_G2_NC
#error "elf.h:R_AARCH64_MOVW_PREL_G2_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_PREL_G3
#error "elf.h:R_AARCH64_MOVW_PREL_G3 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_SABS_G0
#error "elf.h:R_AARCH64_MOVW_SABS_G0 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_SABS_G1
#error "elf.h:R_AARCH64_MOVW_SABS_G1 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_SABS_G2
#error "elf.h:R_AARCH64_MOVW_SABS_G2 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_UABS_G0
#error "elf.h:R_AARCH64_MOVW_UABS_G0 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_UABS_G0_NC
#error "elf.h:R_AARCH64_MOVW_UABS_G0_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_UABS_G1
#error "elf.h:R_AARCH64_MOVW_UABS_G1 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_UABS_G1_NC
#error "elf.h:R_AARCH64_MOVW_UABS_G1_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_UABS_G2
#error "elf.h:R_AARCH64_MOVW_UABS_G2 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_UABS_G2_NC
#error "elf.h:R_AARCH64_MOVW_UABS_G2_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_MOVW_UABS_G3
#error "elf.h:R_AARCH64_MOVW_UABS_G3 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_NONE
#error "elf.h:R_AARCH64_NONE macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_PREL16
#error "elf.h:R_AARCH64_PREL16 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_PREL32
#error "elf.h:R_AARCH64_PREL32 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_PREL64
#error "elf.h:R_AARCH64_PREL64 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_RELATIVE
#error "elf.h:R_AARCH64_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSDESC
#error "elf.h:R_AARCH64_TLSDESC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSDESC_ADD
#error "elf.h:R_AARCH64_TLSDESC_ADD macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSDESC_ADD_LO12
#error "elf.h:R_AARCH64_TLSDESC_ADD_LO12 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSDESC_ADR_PAGE21
#error "elf.h:R_AARCH64_TLSDESC_ADR_PAGE21 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSDESC_ADR_PREL21
#error "elf.h:R_AARCH64_TLSDESC_ADR_PREL21 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSDESC_CALL
#error "elf.h:R_AARCH64_TLSDESC_CALL macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSDESC_LD64_LO12
#error "elf.h:R_AARCH64_TLSDESC_LD64_LO12 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSDESC_LDR
#error "elf.h:R_AARCH64_TLSDESC_LDR macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSDESC_LD_PREL19
#error "elf.h:R_AARCH64_TLSDESC_LD_PREL19 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSDESC_OFF_G0_NC
#error "elf.h:R_AARCH64_TLSDESC_OFF_G0_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSDESC_OFF_G1
#error "elf.h:R_AARCH64_TLSDESC_OFF_G1 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSGD_ADD_LO12_NC
#error "elf.h:R_AARCH64_TLSGD_ADD_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSGD_ADR_PAGE21
#error "elf.h:R_AARCH64_TLSGD_ADR_PAGE21 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSGD_ADR_PREL21
#error "elf.h:R_AARCH64_TLSGD_ADR_PREL21 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSGD_MOVW_G0_NC
#error "elf.h:R_AARCH64_TLSGD_MOVW_G0_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSGD_MOVW_G1
#error "elf.h:R_AARCH64_TLSGD_MOVW_G1 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSIE_ADR_GOTTPREL_PAGE21
#error "elf.h:R_AARCH64_TLSIE_ADR_GOTTPREL_PAGE21 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSIE_LD64_GOTTPREL_LO12_NC
#error "elf.h:R_AARCH64_TLSIE_LD64_GOTTPREL_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSIE_LD_GOTTPREL_PREL19
#error "elf.h:R_AARCH64_TLSIE_LD_GOTTPREL_PREL19 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSIE_MOVW_GOTTPREL_G0_NC
#error "elf.h:R_AARCH64_TLSIE_MOVW_GOTTPREL_G0_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSIE_MOVW_GOTTPREL_G1
#error "elf.h:R_AARCH64_TLSIE_MOVW_GOTTPREL_G1 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_ADD_DTPREL_HI12
#error "elf.h:R_AARCH64_TLSLD_ADD_DTPREL_HI12 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_ADD_DTPREL_LO12
#error "elf.h:R_AARCH64_TLSLD_ADD_DTPREL_LO12 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_ADD_DTPREL_LO12_NC
#error "elf.h:R_AARCH64_TLSLD_ADD_DTPREL_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_ADD_LO12_NC
#error "elf.h:R_AARCH64_TLSLD_ADD_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_ADR_PAGE21
#error "elf.h:R_AARCH64_TLSLD_ADR_PAGE21 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_ADR_PREL21
#error "elf.h:R_AARCH64_TLSLD_ADR_PREL21 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_LDST128_DTPREL_LO12
#error "elf.h:R_AARCH64_TLSLD_LDST128_DTPREL_LO12 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_LDST128_DTPREL_LO12_NC
#error "elf.h:R_AARCH64_TLSLD_LDST128_DTPREL_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_LDST16_DTPREL_LO12
#error "elf.h:R_AARCH64_TLSLD_LDST16_DTPREL_LO12 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_LDST16_DTPREL_LO12_NC
#error "elf.h:R_AARCH64_TLSLD_LDST16_DTPREL_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_LDST32_DTPREL_LO12
#error "elf.h:R_AARCH64_TLSLD_LDST32_DTPREL_LO12 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_LDST32_DTPREL_LO12_NC
#error "elf.h:R_AARCH64_TLSLD_LDST32_DTPREL_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_LDST64_DTPREL_LO12
#error "elf.h:R_AARCH64_TLSLD_LDST64_DTPREL_LO12 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_LDST64_DTPREL_LO12_NC
#error "elf.h:R_AARCH64_TLSLD_LDST64_DTPREL_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_LDST8_DTPREL_LO12
#error "elf.h:R_AARCH64_TLSLD_LDST8_DTPREL_LO12 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_LDST8_DTPREL_LO12_NC
#error "elf.h:R_AARCH64_TLSLD_LDST8_DTPREL_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_LD_PREL19
#error "elf.h:R_AARCH64_TLSLD_LD_PREL19 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_MOVW_DTPREL_G0
#error "elf.h:R_AARCH64_TLSLD_MOVW_DTPREL_G0 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_MOVW_DTPREL_G0_NC
#error "elf.h:R_AARCH64_TLSLD_MOVW_DTPREL_G0_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_MOVW_DTPREL_G1
#error "elf.h:R_AARCH64_TLSLD_MOVW_DTPREL_G1 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_MOVW_DTPREL_G1_NC
#error "elf.h:R_AARCH64_TLSLD_MOVW_DTPREL_G1_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_MOVW_DTPREL_G2
#error "elf.h:R_AARCH64_TLSLD_MOVW_DTPREL_G2 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_MOVW_G0_NC
#error "elf.h:R_AARCH64_TLSLD_MOVW_G0_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLD_MOVW_G1
#error "elf.h:R_AARCH64_TLSLD_MOVW_G1 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_ADD_TPREL_HI12
#error "elf.h:R_AARCH64_TLSLE_ADD_TPREL_HI12 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_ADD_TPREL_LO12
#error "elf.h:R_AARCH64_TLSLE_ADD_TPREL_LO12 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_ADD_TPREL_LO12_NC
#error "elf.h:R_AARCH64_TLSLE_ADD_TPREL_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_LDST128_TPREL_LO12
#error "elf.h:R_AARCH64_TLSLE_LDST128_TPREL_LO12 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_LDST128_TPREL_LO12_NC
#error "elf.h:R_AARCH64_TLSLE_LDST128_TPREL_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_LDST16_TPREL_LO12
#error "elf.h:R_AARCH64_TLSLE_LDST16_TPREL_LO12 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_LDST16_TPREL_LO12_NC
#error "elf.h:R_AARCH64_TLSLE_LDST16_TPREL_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_LDST32_TPREL_LO12
#error "elf.h:R_AARCH64_TLSLE_LDST32_TPREL_LO12 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_LDST32_TPREL_LO12_NC
#error "elf.h:R_AARCH64_TLSLE_LDST32_TPREL_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_LDST64_TPREL_LO12
#error "elf.h:R_AARCH64_TLSLE_LDST64_TPREL_LO12 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_LDST64_TPREL_LO12_NC
#error "elf.h:R_AARCH64_TLSLE_LDST64_TPREL_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_LDST8_TPREL_LO12
#error "elf.h:R_AARCH64_TLSLE_LDST8_TPREL_LO12 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_LDST8_TPREL_LO12_NC
#error "elf.h:R_AARCH64_TLSLE_LDST8_TPREL_LO12_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_MOVW_TPREL_G0
#error "elf.h:R_AARCH64_TLSLE_MOVW_TPREL_G0 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_MOVW_TPREL_G0_NC
#error "elf.h:R_AARCH64_TLSLE_MOVW_TPREL_G0_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_MOVW_TPREL_G1
#error "elf.h:R_AARCH64_TLSLE_MOVW_TPREL_G1 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_MOVW_TPREL_G1_NC
#error "elf.h:R_AARCH64_TLSLE_MOVW_TPREL_G1_NC macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLSLE_MOVW_TPREL_G2
#error "elf.h:R_AARCH64_TLSLE_MOVW_TPREL_G2 macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLS_DTPMOD
#error "elf.h:R_AARCH64_TLS_DTPMOD macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLS_DTPREL
#error "elf.h:R_AARCH64_TLS_DTPREL macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TLS_TPREL
#error "elf.h:R_AARCH64_TLS_TPREL macro is missing from libc-shim"
#endif

#ifndef R_AARCH64_TSTBR14
#error "elf.h:R_AARCH64_TSTBR14 macro is missing from libc-shim"
#endif

#ifndef R_AC_SECTOFF_S9
#error "elf.h:R_AC_SECTOFF_S9 macro is missing from libc-shim"
#endif

#ifndef R_AC_SECTOFF_S9_1
#error "elf.h:R_AC_SECTOFF_S9_1 macro is missing from libc-shim"
#endif

#ifndef R_AC_SECTOFF_S9_2
#error "elf.h:R_AC_SECTOFF_S9_2 macro is missing from libc-shim"
#endif

#ifndef R_AC_SECTOFF_U8
#error "elf.h:R_AC_SECTOFF_U8 macro is missing from libc-shim"
#endif

#ifndef R_AC_SECTOFF_U8_1
#error "elf.h:R_AC_SECTOFF_U8_1 macro is missing from libc-shim"
#endif

#ifndef R_AC_SECTOFF_U8_2
#error "elf.h:R_AC_SECTOFF_U8_2 macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_BRADDR
#error "elf.h:R_ALPHA_BRADDR macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_COPY
#error "elf.h:R_ALPHA_COPY macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_DTPMOD64
#error "elf.h:R_ALPHA_DTPMOD64 macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_DTPREL16
#error "elf.h:R_ALPHA_DTPREL16 macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_DTPREL64
#error "elf.h:R_ALPHA_DTPREL64 macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_DTPRELHI
#error "elf.h:R_ALPHA_DTPRELHI macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_DTPRELLO
#error "elf.h:R_ALPHA_DTPRELLO macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_GLOB_DAT
#error "elf.h:R_ALPHA_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_GOTDTPREL
#error "elf.h:R_ALPHA_GOTDTPREL macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_GOTTPREL
#error "elf.h:R_ALPHA_GOTTPREL macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_GPDISP
#error "elf.h:R_ALPHA_GPDISP macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_GPREL16
#error "elf.h:R_ALPHA_GPREL16 macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_GPREL32
#error "elf.h:R_ALPHA_GPREL32 macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_GPRELHIGH
#error "elf.h:R_ALPHA_GPRELHIGH macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_GPRELLOW
#error "elf.h:R_ALPHA_GPRELLOW macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_HINT
#error "elf.h:R_ALPHA_HINT macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_JMP_SLOT
#error "elf.h:R_ALPHA_JMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_LITERAL
#error "elf.h:R_ALPHA_LITERAL macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_LITUSE
#error "elf.h:R_ALPHA_LITUSE macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_NONE
#error "elf.h:R_ALPHA_NONE macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_NUM
#error "elf.h:R_ALPHA_NUM macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_REFLONG
#error "elf.h:R_ALPHA_REFLONG macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_REFQUAD
#error "elf.h:R_ALPHA_REFQUAD macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_RELATIVE
#error "elf.h:R_ALPHA_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_SREL16
#error "elf.h:R_ALPHA_SREL16 macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_SREL32
#error "elf.h:R_ALPHA_SREL32 macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_SREL64
#error "elf.h:R_ALPHA_SREL64 macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_TLSGD
#error "elf.h:R_ALPHA_TLSGD macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_TLS_GD_HI
#error "elf.h:R_ALPHA_TLS_GD_HI macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_TLS_LDM
#error "elf.h:R_ALPHA_TLS_LDM macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_TPREL16
#error "elf.h:R_ALPHA_TPREL16 macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_TPREL64
#error "elf.h:R_ALPHA_TPREL64 macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_TPRELHI
#error "elf.h:R_ALPHA_TPRELHI macro is missing from libc-shim"
#endif

#ifndef R_ALPHA_TPRELLO
#error "elf.h:R_ALPHA_TPRELLO macro is missing from libc-shim"
#endif

#ifndef R_ARC_16
#error "elf.h:R_ARC_16 macro is missing from libc-shim"
#endif

#ifndef R_ARC_24
#error "elf.h:R_ARC_24 macro is missing from libc-shim"
#endif

#ifndef R_ARC_32
#error "elf.h:R_ARC_32 macro is missing from libc-shim"
#endif

#ifndef R_ARC_32_ME
#error "elf.h:R_ARC_32_ME macro is missing from libc-shim"
#endif

#ifndef R_ARC_32_PCREL
#error "elf.h:R_ARC_32_PCREL macro is missing from libc-shim"
#endif

#ifndef R_ARC_8
#error "elf.h:R_ARC_8 macro is missing from libc-shim"
#endif

#ifndef R_ARC_B22_PCREL
#error "elf.h:R_ARC_B22_PCREL macro is missing from libc-shim"
#endif

#ifndef R_ARC_COPY
#error "elf.h:R_ARC_COPY macro is missing from libc-shim"
#endif

#ifndef R_ARC_GLOB_DAT
#error "elf.h:R_ARC_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_ARC_GOT32
#error "elf.h:R_ARC_GOT32 macro is missing from libc-shim"
#endif

#ifndef R_ARC_GOTOFF
#error "elf.h:R_ARC_GOTOFF macro is missing from libc-shim"
#endif

#ifndef R_ARC_GOTPC
#error "elf.h:R_ARC_GOTPC macro is missing from libc-shim"
#endif

#ifndef R_ARC_GOTPC32
#error "elf.h:R_ARC_GOTPC32 macro is missing from libc-shim"
#endif

#ifndef R_ARC_H30
#error "elf.h:R_ARC_H30 macro is missing from libc-shim"
#endif

#ifndef R_ARC_H30_ME
#error "elf.h:R_ARC_H30_ME macro is missing from libc-shim"
#endif

#ifndef R_ARC_JLI_SECTOFF
#error "elf.h:R_ARC_JLI_SECTOFF macro is missing from libc-shim"
#endif

#ifndef R_ARC_JMP_SLOT
#error "elf.h:R_ARC_JMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_ARC_N16
#error "elf.h:R_ARC_N16 macro is missing from libc-shim"
#endif

#ifndef R_ARC_N24
#error "elf.h:R_ARC_N24 macro is missing from libc-shim"
#endif

#ifndef R_ARC_N32
#error "elf.h:R_ARC_N32 macro is missing from libc-shim"
#endif

#ifndef R_ARC_N32_ME
#error "elf.h:R_ARC_N32_ME macro is missing from libc-shim"
#endif

#ifndef R_ARC_N8
#error "elf.h:R_ARC_N8 macro is missing from libc-shim"
#endif

#ifndef R_ARC_NONE
#error "elf.h:R_ARC_NONE macro is missing from libc-shim"
#endif

#ifndef R_ARC_NPS_CMEM16
#error "elf.h:R_ARC_NPS_CMEM16 macro is missing from libc-shim"
#endif

#ifndef R_ARC_PC32
#error "elf.h:R_ARC_PC32 macro is missing from libc-shim"
#endif

#ifndef R_ARC_PLT32
#error "elf.h:R_ARC_PLT32 macro is missing from libc-shim"
#endif

#ifndef R_ARC_RELATIVE
#error "elf.h:R_ARC_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_ARC_S13_PCREL
#error "elf.h:R_ARC_S13_PCREL macro is missing from libc-shim"
#endif

#ifndef R_ARC_S21H_PCREL
#error "elf.h:R_ARC_S21H_PCREL macro is missing from libc-shim"
#endif

#ifndef R_ARC_S21H_PCREL_PLT
#error "elf.h:R_ARC_S21H_PCREL_PLT macro is missing from libc-shim"
#endif

#ifndef R_ARC_S21W_PCREL
#error "elf.h:R_ARC_S21W_PCREL macro is missing from libc-shim"
#endif

#ifndef R_ARC_S21W_PCREL_PLT
#error "elf.h:R_ARC_S21W_PCREL_PLT macro is missing from libc-shim"
#endif

#ifndef R_ARC_S25H_PCREL
#error "elf.h:R_ARC_S25H_PCREL macro is missing from libc-shim"
#endif

#ifndef R_ARC_S25H_PCREL_PLT
#error "elf.h:R_ARC_S25H_PCREL_PLT macro is missing from libc-shim"
#endif

#ifndef R_ARC_S25W_PCREL
#error "elf.h:R_ARC_S25W_PCREL macro is missing from libc-shim"
#endif

#ifndef R_ARC_S25W_PCREL_PLT
#error "elf.h:R_ARC_S25W_PCREL_PLT macro is missing from libc-shim"
#endif

#ifndef R_ARC_SDA
#error "elf.h:R_ARC_SDA macro is missing from libc-shim"
#endif

#ifndef R_ARC_SDA16_LD
#error "elf.h:R_ARC_SDA16_LD macro is missing from libc-shim"
#endif

#ifndef R_ARC_SDA16_LD1
#error "elf.h:R_ARC_SDA16_LD1 macro is missing from libc-shim"
#endif

#ifndef R_ARC_SDA16_LD2
#error "elf.h:R_ARC_SDA16_LD2 macro is missing from libc-shim"
#endif

#ifndef R_ARC_SDA16_ST2
#error "elf.h:R_ARC_SDA16_ST2 macro is missing from libc-shim"
#endif

#ifndef R_ARC_SDA32
#error "elf.h:R_ARC_SDA32 macro is missing from libc-shim"
#endif

#ifndef R_ARC_SDA32_ME
#error "elf.h:R_ARC_SDA32_ME macro is missing from libc-shim"
#endif

#ifndef R_ARC_SDA_12
#error "elf.h:R_ARC_SDA_12 macro is missing from libc-shim"
#endif

#ifndef R_ARC_SDA_LDST
#error "elf.h:R_ARC_SDA_LDST macro is missing from libc-shim"
#endif

#ifndef R_ARC_SDA_LDST1
#error "elf.h:R_ARC_SDA_LDST1 macro is missing from libc-shim"
#endif

#ifndef R_ARC_SDA_LDST2
#error "elf.h:R_ARC_SDA_LDST2 macro is missing from libc-shim"
#endif

#ifndef R_ARC_SECTOFF
#error "elf.h:R_ARC_SECTOFF macro is missing from libc-shim"
#endif

#ifndef R_ARC_SECTOFF_1
#error "elf.h:R_ARC_SECTOFF_1 macro is missing from libc-shim"
#endif

#ifndef R_ARC_SECTOFF_2
#error "elf.h:R_ARC_SECTOFF_2 macro is missing from libc-shim"
#endif

#ifndef R_ARC_SECTOFF_ME
#error "elf.h:R_ARC_SECTOFF_ME macro is missing from libc-shim"
#endif

#ifndef R_ARC_SECTOFF_ME_1
#error "elf.h:R_ARC_SECTOFF_ME_1 macro is missing from libc-shim"
#endif

#ifndef R_ARC_SECTOFF_ME_2
#error "elf.h:R_ARC_SECTOFF_ME_2 macro is missing from libc-shim"
#endif

#ifndef R_ARC_SECTOFF_S9
#error "elf.h:R_ARC_SECTOFF_S9 macro is missing from libc-shim"
#endif

#ifndef R_ARC_SECTOFF_U8
#error "elf.h:R_ARC_SECTOFF_U8 macro is missing from libc-shim"
#endif

#ifndef R_ARC_TLS_DTPMOD
#error "elf.h:R_ARC_TLS_DTPMOD macro is missing from libc-shim"
#endif

#ifndef R_ARC_TLS_DTPOFF
#error "elf.h:R_ARC_TLS_DTPOFF macro is missing from libc-shim"
#endif

#ifndef R_ARC_TLS_DTPOFF_S9
#error "elf.h:R_ARC_TLS_DTPOFF_S9 macro is missing from libc-shim"
#endif

#ifndef R_ARC_TLS_GD_CALL
#error "elf.h:R_ARC_TLS_GD_CALL macro is missing from libc-shim"
#endif

#ifndef R_ARC_TLS_GD_GOT
#error "elf.h:R_ARC_TLS_GD_GOT macro is missing from libc-shim"
#endif

#ifndef R_ARC_TLS_GD_LD
#error "elf.h:R_ARC_TLS_GD_LD macro is missing from libc-shim"
#endif

#ifndef R_ARC_TLS_IE_GOT
#error "elf.h:R_ARC_TLS_IE_GOT macro is missing from libc-shim"
#endif

#ifndef R_ARC_TLS_LE_32
#error "elf.h:R_ARC_TLS_LE_32 macro is missing from libc-shim"
#endif

#ifndef R_ARC_TLS_LE_S9
#error "elf.h:R_ARC_TLS_LE_S9 macro is missing from libc-shim"
#endif

#ifndef R_ARC_TLS_TPOFF
#error "elf.h:R_ARC_TLS_TPOFF macro is missing from libc-shim"
#endif

#ifndef R_ARC_W
#error "elf.h:R_ARC_W macro is missing from libc-shim"
#endif

#ifndef R_ARC_W_ME
#error "elf.h:R_ARC_W_ME macro is missing from libc-shim"
#endif

#ifndef R_ARM_ABS12
#error "elf.h:R_ARM_ABS12 macro is missing from libc-shim"
#endif

#ifndef R_ARM_ABS16
#error "elf.h:R_ARM_ABS16 macro is missing from libc-shim"
#endif

#ifndef R_ARM_ABS32
#error "elf.h:R_ARM_ABS32 macro is missing from libc-shim"
#endif

#ifndef R_ARM_ABS32_NOI
#error "elf.h:R_ARM_ABS32_NOI macro is missing from libc-shim"
#endif

#ifndef R_ARM_ABS8
#error "elf.h:R_ARM_ABS8 macro is missing from libc-shim"
#endif

#ifndef R_ARM_ALU_PCREL_15_8
#error "elf.h:R_ARM_ALU_PCREL_15_8 macro is missing from libc-shim"
#endif

#ifndef R_ARM_ALU_PCREL_23_15
#error "elf.h:R_ARM_ALU_PCREL_23_15 macro is missing from libc-shim"
#endif

#ifndef R_ARM_ALU_PCREL_7_0
#error "elf.h:R_ARM_ALU_PCREL_7_0 macro is missing from libc-shim"
#endif

#ifndef R_ARM_ALU_PC_G0
#error "elf.h:R_ARM_ALU_PC_G0 macro is missing from libc-shim"
#endif

#ifndef R_ARM_ALU_PC_G0_NC
#error "elf.h:R_ARM_ALU_PC_G0_NC macro is missing from libc-shim"
#endif

#ifndef R_ARM_ALU_PC_G1
#error "elf.h:R_ARM_ALU_PC_G1 macro is missing from libc-shim"
#endif

#ifndef R_ARM_ALU_PC_G1_NC
#error "elf.h:R_ARM_ALU_PC_G1_NC macro is missing from libc-shim"
#endif

#ifndef R_ARM_ALU_PC_G2
#error "elf.h:R_ARM_ALU_PC_G2 macro is missing from libc-shim"
#endif

#ifndef R_ARM_ALU_SBREL_19_12
#error "elf.h:R_ARM_ALU_SBREL_19_12 macro is missing from libc-shim"
#endif

#ifndef R_ARM_ALU_SBREL_27_20
#error "elf.h:R_ARM_ALU_SBREL_27_20 macro is missing from libc-shim"
#endif

#ifndef R_ARM_ALU_SB_G0
#error "elf.h:R_ARM_ALU_SB_G0 macro is missing from libc-shim"
#endif

#ifndef R_ARM_ALU_SB_G0_NC
#error "elf.h:R_ARM_ALU_SB_G0_NC macro is missing from libc-shim"
#endif

#ifndef R_ARM_ALU_SB_G1
#error "elf.h:R_ARM_ALU_SB_G1 macro is missing from libc-shim"
#endif

#ifndef R_ARM_ALU_SB_G1_NC
#error "elf.h:R_ARM_ALU_SB_G1_NC macro is missing from libc-shim"
#endif

#ifndef R_ARM_ALU_SB_G2
#error "elf.h:R_ARM_ALU_SB_G2 macro is missing from libc-shim"
#endif

#ifndef R_ARM_AMP_VCALL9
#error "elf.h:R_ARM_AMP_VCALL9 macro is missing from libc-shim"
#endif

#ifndef R_ARM_BASE_ABS
#error "elf.h:R_ARM_BASE_ABS macro is missing from libc-shim"
#endif

#ifndef R_ARM_CALL
#error "elf.h:R_ARM_CALL macro is missing from libc-shim"
#endif

#ifndef R_ARM_COPY
#error "elf.h:R_ARM_COPY macro is missing from libc-shim"
#endif

#ifndef R_ARM_GLOB_DAT
#error "elf.h:R_ARM_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_ARM_GNU_VTENTRY
#error "elf.h:R_ARM_GNU_VTENTRY macro is missing from libc-shim"
#endif

#ifndef R_ARM_GNU_VTINHERIT
#error "elf.h:R_ARM_GNU_VTINHERIT macro is missing from libc-shim"
#endif

#ifndef R_ARM_GOT32
#error "elf.h:R_ARM_GOT32 macro is missing from libc-shim"
#endif

#ifndef R_ARM_GOTOFF
#error "elf.h:R_ARM_GOTOFF macro is missing from libc-shim"
#endif

#ifndef R_ARM_GOTOFF12
#error "elf.h:R_ARM_GOTOFF12 macro is missing from libc-shim"
#endif

#ifndef R_ARM_GOTPC
#error "elf.h:R_ARM_GOTPC macro is missing from libc-shim"
#endif

#ifndef R_ARM_GOTRELAX
#error "elf.h:R_ARM_GOTRELAX macro is missing from libc-shim"
#endif

#ifndef R_ARM_GOT_ABS
#error "elf.h:R_ARM_GOT_ABS macro is missing from libc-shim"
#endif

#ifndef R_ARM_GOT_BREL12
#error "elf.h:R_ARM_GOT_BREL12 macro is missing from libc-shim"
#endif

#ifndef R_ARM_GOT_PREL
#error "elf.h:R_ARM_GOT_PREL macro is missing from libc-shim"
#endif

#ifndef R_ARM_IRELATIVE
#error "elf.h:R_ARM_IRELATIVE macro is missing from libc-shim"
#endif

#ifndef R_ARM_JUMP24
#error "elf.h:R_ARM_JUMP24 macro is missing from libc-shim"
#endif

#ifndef R_ARM_JUMP_SLOT
#error "elf.h:R_ARM_JUMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDC_PC_G0
#error "elf.h:R_ARM_LDC_PC_G0 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDC_PC_G1
#error "elf.h:R_ARM_LDC_PC_G1 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDC_PC_G2
#error "elf.h:R_ARM_LDC_PC_G2 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDC_SB_G0
#error "elf.h:R_ARM_LDC_SB_G0 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDC_SB_G1
#error "elf.h:R_ARM_LDC_SB_G1 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDC_SB_G2
#error "elf.h:R_ARM_LDC_SB_G2 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDRS_PC_G0
#error "elf.h:R_ARM_LDRS_PC_G0 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDRS_PC_G1
#error "elf.h:R_ARM_LDRS_PC_G1 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDRS_PC_G2
#error "elf.h:R_ARM_LDRS_PC_G2 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDRS_SB_G0
#error "elf.h:R_ARM_LDRS_SB_G0 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDRS_SB_G1
#error "elf.h:R_ARM_LDRS_SB_G1 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDRS_SB_G2
#error "elf.h:R_ARM_LDRS_SB_G2 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDR_PC_G1
#error "elf.h:R_ARM_LDR_PC_G1 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDR_PC_G2
#error "elf.h:R_ARM_LDR_PC_G2 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDR_SBREL_11_0
#error "elf.h:R_ARM_LDR_SBREL_11_0 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDR_SB_G0
#error "elf.h:R_ARM_LDR_SB_G0 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDR_SB_G1
#error "elf.h:R_ARM_LDR_SB_G1 macro is missing from libc-shim"
#endif

#ifndef R_ARM_LDR_SB_G2
#error "elf.h:R_ARM_LDR_SB_G2 macro is missing from libc-shim"
#endif

#ifndef R_ARM_ME_TOO
#error "elf.h:R_ARM_ME_TOO macro is missing from libc-shim"
#endif

#ifndef R_ARM_MOVT_ABS
#error "elf.h:R_ARM_MOVT_ABS macro is missing from libc-shim"
#endif

#ifndef R_ARM_MOVT_BREL
#error "elf.h:R_ARM_MOVT_BREL macro is missing from libc-shim"
#endif

#ifndef R_ARM_MOVT_PREL
#error "elf.h:R_ARM_MOVT_PREL macro is missing from libc-shim"
#endif

#ifndef R_ARM_MOVW_ABS_NC
#error "elf.h:R_ARM_MOVW_ABS_NC macro is missing from libc-shim"
#endif

#ifndef R_ARM_MOVW_BREL
#error "elf.h:R_ARM_MOVW_BREL macro is missing from libc-shim"
#endif

#ifndef R_ARM_MOVW_BREL_NC
#error "elf.h:R_ARM_MOVW_BREL_NC macro is missing from libc-shim"
#endif

#ifndef R_ARM_MOVW_PREL_NC
#error "elf.h:R_ARM_MOVW_PREL_NC macro is missing from libc-shim"
#endif

#ifndef R_ARM_NONE
#error "elf.h:R_ARM_NONE macro is missing from libc-shim"
#endif

#ifndef R_ARM_NUM
#error "elf.h:R_ARM_NUM macro is missing from libc-shim"
#endif

#ifndef R_ARM_PC13
#error "elf.h:R_ARM_PC13 macro is missing from libc-shim"
#endif

#ifndef R_ARM_PC24
#error "elf.h:R_ARM_PC24 macro is missing from libc-shim"
#endif

#ifndef R_ARM_PLT32
#error "elf.h:R_ARM_PLT32 macro is missing from libc-shim"
#endif

#ifndef R_ARM_PLT32_ABS
#error "elf.h:R_ARM_PLT32_ABS macro is missing from libc-shim"
#endif

#ifndef R_ARM_PREL31
#error "elf.h:R_ARM_PREL31 macro is missing from libc-shim"
#endif

#ifndef R_ARM_RABS22
#error "elf.h:R_ARM_RABS22 macro is missing from libc-shim"
#endif

#ifndef R_ARM_RBASE
#error "elf.h:R_ARM_RBASE macro is missing from libc-shim"
#endif

#ifndef R_ARM_REL32
#error "elf.h:R_ARM_REL32 macro is missing from libc-shim"
#endif

#ifndef R_ARM_REL32_NOI
#error "elf.h:R_ARM_REL32_NOI macro is missing from libc-shim"
#endif

#ifndef R_ARM_RELATIVE
#error "elf.h:R_ARM_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_ARM_RPC24
#error "elf.h:R_ARM_RPC24 macro is missing from libc-shim"
#endif

#ifndef R_ARM_RREL32
#error "elf.h:R_ARM_RREL32 macro is missing from libc-shim"
#endif

#ifndef R_ARM_RSBREL32
#error "elf.h:R_ARM_RSBREL32 macro is missing from libc-shim"
#endif

#ifndef R_ARM_RXPC25
#error "elf.h:R_ARM_RXPC25 macro is missing from libc-shim"
#endif

#ifndef R_ARM_SBREL31
#error "elf.h:R_ARM_SBREL31 macro is missing from libc-shim"
#endif

#ifndef R_ARM_SBREL32
#error "elf.h:R_ARM_SBREL32 macro is missing from libc-shim"
#endif

#ifndef R_ARM_SWI24
#error "elf.h:R_ARM_SWI24 macro is missing from libc-shim"
#endif

#ifndef R_ARM_TARGET1
#error "elf.h:R_ARM_TARGET1 macro is missing from libc-shim"
#endif

#ifndef R_ARM_TARGET2
#error "elf.h:R_ARM_TARGET2 macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_ABS5
#error "elf.h:R_ARM_THM_ABS5 macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_ALU_PREL_11_0
#error "elf.h:R_ARM_THM_ALU_PREL_11_0 macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_GOT_BREL12
#error "elf.h:R_ARM_THM_GOT_BREL12 macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_JUMP19
#error "elf.h:R_ARM_THM_JUMP19 macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_JUMP24
#error "elf.h:R_ARM_THM_JUMP24 macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_JUMP6
#error "elf.h:R_ARM_THM_JUMP6 macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_MOVT_ABS
#error "elf.h:R_ARM_THM_MOVT_ABS macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_MOVT_BREL
#error "elf.h:R_ARM_THM_MOVT_BREL macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_MOVT_PREL
#error "elf.h:R_ARM_THM_MOVT_PREL macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_MOVW_ABS_NC
#error "elf.h:R_ARM_THM_MOVW_ABS_NC macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_MOVW_BREL
#error "elf.h:R_ARM_THM_MOVW_BREL macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_MOVW_BREL_NC
#error "elf.h:R_ARM_THM_MOVW_BREL_NC macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_MOVW_PREL_NC
#error "elf.h:R_ARM_THM_MOVW_PREL_NC macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_PC11
#error "elf.h:R_ARM_THM_PC11 macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_PC12
#error "elf.h:R_ARM_THM_PC12 macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_PC22
#error "elf.h:R_ARM_THM_PC22 macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_PC8
#error "elf.h:R_ARM_THM_PC8 macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_PC9
#error "elf.h:R_ARM_THM_PC9 macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_RPC22
#error "elf.h:R_ARM_THM_RPC22 macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_SWI8
#error "elf.h:R_ARM_THM_SWI8 macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_TLS_CALL
#error "elf.h:R_ARM_THM_TLS_CALL macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_TLS_DESCSEQ
#error "elf.h:R_ARM_THM_TLS_DESCSEQ macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_TLS_DESCSEQ16
#error "elf.h:R_ARM_THM_TLS_DESCSEQ16 macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_TLS_DESCSEQ32
#error "elf.h:R_ARM_THM_TLS_DESCSEQ32 macro is missing from libc-shim"
#endif

#ifndef R_ARM_THM_XPC22
#error "elf.h:R_ARM_THM_XPC22 macro is missing from libc-shim"
#endif

#ifndef R_ARM_TLS_CALL
#error "elf.h:R_ARM_TLS_CALL macro is missing from libc-shim"
#endif

#ifndef R_ARM_TLS_DESC
#error "elf.h:R_ARM_TLS_DESC macro is missing from libc-shim"
#endif

#ifndef R_ARM_TLS_DESCSEQ
#error "elf.h:R_ARM_TLS_DESCSEQ macro is missing from libc-shim"
#endif

#ifndef R_ARM_TLS_DTPMOD32
#error "elf.h:R_ARM_TLS_DTPMOD32 macro is missing from libc-shim"
#endif

#ifndef R_ARM_TLS_DTPOFF32
#error "elf.h:R_ARM_TLS_DTPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_ARM_TLS_GD32
#error "elf.h:R_ARM_TLS_GD32 macro is missing from libc-shim"
#endif

#ifndef R_ARM_TLS_GOTDESC
#error "elf.h:R_ARM_TLS_GOTDESC macro is missing from libc-shim"
#endif

#ifndef R_ARM_TLS_IE12GP
#error "elf.h:R_ARM_TLS_IE12GP macro is missing from libc-shim"
#endif

#ifndef R_ARM_TLS_IE32
#error "elf.h:R_ARM_TLS_IE32 macro is missing from libc-shim"
#endif

#ifndef R_ARM_TLS_LDM32
#error "elf.h:R_ARM_TLS_LDM32 macro is missing from libc-shim"
#endif

#ifndef R_ARM_TLS_LDO12
#error "elf.h:R_ARM_TLS_LDO12 macro is missing from libc-shim"
#endif

#ifndef R_ARM_TLS_LDO32
#error "elf.h:R_ARM_TLS_LDO32 macro is missing from libc-shim"
#endif

#ifndef R_ARM_TLS_LE12
#error "elf.h:R_ARM_TLS_LE12 macro is missing from libc-shim"
#endif

#ifndef R_ARM_TLS_LE32
#error "elf.h:R_ARM_TLS_LE32 macro is missing from libc-shim"
#endif

#ifndef R_ARM_TLS_TPOFF32
#error "elf.h:R_ARM_TLS_TPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_ARM_V4BX
#error "elf.h:R_ARM_V4BX macro is missing from libc-shim"
#endif

#ifndef R_ARM_XPC25
#error "elf.h:R_ARM_XPC25 macro is missing from libc-shim"
#endif

#ifndef R_BPF_64_32
#error "elf.h:R_BPF_64_32 macro is missing from libc-shim"
#endif

#ifndef R_BPF_64_64
#error "elf.h:R_BPF_64_64 macro is missing from libc-shim"
#endif

#ifndef R_BPF_NONE
#error "elf.h:R_BPF_NONE macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_ADDR32
#error "elf.h:R_CKCORE_ADDR32 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_ADDRGOT
#error "elf.h:R_CKCORE_ADDRGOT macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_ADDRGOT_HI16
#error "elf.h:R_CKCORE_ADDRGOT_HI16 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_ADDRGOT_LO16
#error "elf.h:R_CKCORE_ADDRGOT_LO16 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_ADDRPLT
#error "elf.h:R_CKCORE_ADDRPLT macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_ADDRPLT_HI16
#error "elf.h:R_CKCORE_ADDRPLT_HI16 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_ADDRPLT_LO16
#error "elf.h:R_CKCORE_ADDRPLT_LO16 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_ADDR_HI16
#error "elf.h:R_CKCORE_ADDR_HI16 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_ADDR_LO16
#error "elf.h:R_CKCORE_ADDR_LO16 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_COPY
#error "elf.h:R_CKCORE_COPY macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_DOFFSET_IMM18
#error "elf.h:R_CKCORE_DOFFSET_IMM18 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_DOFFSET_IMM18BY2
#error "elf.h:R_CKCORE_DOFFSET_IMM18BY2 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_DOFFSET_IMM18BY4
#error "elf.h:R_CKCORE_DOFFSET_IMM18BY4 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_DOFFSET_LO16
#error "elf.h:R_CKCORE_DOFFSET_LO16 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_GLOB_DAT
#error "elf.h:R_CKCORE_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_GOT12
#error "elf.h:R_CKCORE_GOT12 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_GOT32
#error "elf.h:R_CKCORE_GOT32 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_GOTOFF
#error "elf.h:R_CKCORE_GOTOFF macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_GOTOFF_HI16
#error "elf.h:R_CKCORE_GOTOFF_HI16 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_GOTOFF_LO16
#error "elf.h:R_CKCORE_GOTOFF_LO16 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_GOTPC
#error "elf.h:R_CKCORE_GOTPC macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_GOTPC_HI16
#error "elf.h:R_CKCORE_GOTPC_HI16 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_GOTPC_LO16
#error "elf.h:R_CKCORE_GOTPC_LO16 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_GOT_HI16
#error "elf.h:R_CKCORE_GOT_HI16 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_GOT_IMM18BY4
#error "elf.h:R_CKCORE_GOT_IMM18BY4 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_GOT_LO16
#error "elf.h:R_CKCORE_GOT_LO16 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_JUMP_SLOT
#error "elf.h:R_CKCORE_JUMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_NONE
#error "elf.h:R_CKCORE_NONE macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PCREL32
#error "elf.h:R_CKCORE_PCREL32 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PCRELIMM11BY2
#error "elf.h:R_CKCORE_PCRELIMM11BY2 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PCRELIMM8BY4
#error "elf.h:R_CKCORE_PCRELIMM8BY4 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PCRELJSR_IMM11BY2
#error "elf.h:R_CKCORE_PCRELJSR_IMM11BY2 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PCREL_IMM10BY2
#error "elf.h:R_CKCORE_PCREL_IMM10BY2 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PCREL_IMM10BY4
#error "elf.h:R_CKCORE_PCREL_IMM10BY4 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PCREL_IMM16BY2
#error "elf.h:R_CKCORE_PCREL_IMM16BY2 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PCREL_IMM16BY4
#error "elf.h:R_CKCORE_PCREL_IMM16BY4 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PCREL_IMM18BY2
#error "elf.h:R_CKCORE_PCREL_IMM18BY2 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PCREL_IMM26BY2
#error "elf.h:R_CKCORE_PCREL_IMM26BY2 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PCREL_IMM7BY4
#error "elf.h:R_CKCORE_PCREL_IMM7BY4 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PCREL_JSR_IMM26BY2
#error "elf.h:R_CKCORE_PCREL_JSR_IMM26BY2 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PLT12
#error "elf.h:R_CKCORE_PLT12 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PLT32
#error "elf.h:R_CKCORE_PLT32 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PLT_HI16
#error "elf.h:R_CKCORE_PLT_HI16 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PLT_IMM18BY4
#error "elf.h:R_CKCORE_PLT_IMM18BY4 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_PLT_LO16
#error "elf.h:R_CKCORE_PLT_LO16 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_RELATIVE
#error "elf.h:R_CKCORE_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_TLS_DTPMOD32
#error "elf.h:R_CKCORE_TLS_DTPMOD32 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_TLS_DTPOFF32
#error "elf.h:R_CKCORE_TLS_DTPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_TLS_GD32
#error "elf.h:R_CKCORE_TLS_GD32 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_TLS_IE32
#error "elf.h:R_CKCORE_TLS_IE32 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_TLS_LDM32
#error "elf.h:R_CKCORE_TLS_LDM32 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_TLS_LDO32
#error "elf.h:R_CKCORE_TLS_LDO32 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_TLS_LE32
#error "elf.h:R_CKCORE_TLS_LE32 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_TLS_TPOFF32
#error "elf.h:R_CKCORE_TLS_TPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_CKCORE_TOFFSET_LO16
#error "elf.h:R_CKCORE_TOFFSET_LO16 macro is missing from libc-shim"
#endif

#ifndef R_CRIS_16
#error "elf.h:R_CRIS_16 macro is missing from libc-shim"
#endif

#ifndef R_CRIS_16_GOT
#error "elf.h:R_CRIS_16_GOT macro is missing from libc-shim"
#endif

#ifndef R_CRIS_16_GOTPLT
#error "elf.h:R_CRIS_16_GOTPLT macro is missing from libc-shim"
#endif

#ifndef R_CRIS_16_PCREL
#error "elf.h:R_CRIS_16_PCREL macro is missing from libc-shim"
#endif

#ifndef R_CRIS_32
#error "elf.h:R_CRIS_32 macro is missing from libc-shim"
#endif

#ifndef R_CRIS_32_GOT
#error "elf.h:R_CRIS_32_GOT macro is missing from libc-shim"
#endif

#ifndef R_CRIS_32_GOTPLT
#error "elf.h:R_CRIS_32_GOTPLT macro is missing from libc-shim"
#endif

#ifndef R_CRIS_32_GOTREL
#error "elf.h:R_CRIS_32_GOTREL macro is missing from libc-shim"
#endif

#ifndef R_CRIS_32_PCREL
#error "elf.h:R_CRIS_32_PCREL macro is missing from libc-shim"
#endif

#ifndef R_CRIS_32_PLT_GOTREL
#error "elf.h:R_CRIS_32_PLT_GOTREL macro is missing from libc-shim"
#endif

#ifndef R_CRIS_32_PLT_PCREL
#error "elf.h:R_CRIS_32_PLT_PCREL macro is missing from libc-shim"
#endif

#ifndef R_CRIS_8
#error "elf.h:R_CRIS_8 macro is missing from libc-shim"
#endif

#ifndef R_CRIS_8_PCREL
#error "elf.h:R_CRIS_8_PCREL macro is missing from libc-shim"
#endif

#ifndef R_CRIS_COPY
#error "elf.h:R_CRIS_COPY macro is missing from libc-shim"
#endif

#ifndef R_CRIS_GLOB_DAT
#error "elf.h:R_CRIS_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_CRIS_GNU_VTENTRY
#error "elf.h:R_CRIS_GNU_VTENTRY macro is missing from libc-shim"
#endif

#ifndef R_CRIS_GNU_VTINHERIT
#error "elf.h:R_CRIS_GNU_VTINHERIT macro is missing from libc-shim"
#endif

#ifndef R_CRIS_JUMP_SLOT
#error "elf.h:R_CRIS_JUMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_CRIS_NONE
#error "elf.h:R_CRIS_NONE macro is missing from libc-shim"
#endif

#ifndef R_CRIS_NUM
#error "elf.h:R_CRIS_NUM macro is missing from libc-shim"
#endif

#ifndef R_CRIS_RELATIVE
#error "elf.h:R_CRIS_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_IA64_COPY
#error "elf.h:R_IA64_COPY macro is missing from libc-shim"
#endif

#ifndef R_IA64_DIR32LSB
#error "elf.h:R_IA64_DIR32LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_DIR32MSB
#error "elf.h:R_IA64_DIR32MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_DIR64LSB
#error "elf.h:R_IA64_DIR64LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_DIR64MSB
#error "elf.h:R_IA64_DIR64MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_DTPMOD64LSB
#error "elf.h:R_IA64_DTPMOD64LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_DTPMOD64MSB
#error "elf.h:R_IA64_DTPMOD64MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_DTPREL14
#error "elf.h:R_IA64_DTPREL14 macro is missing from libc-shim"
#endif

#ifndef R_IA64_DTPREL22
#error "elf.h:R_IA64_DTPREL22 macro is missing from libc-shim"
#endif

#ifndef R_IA64_DTPREL32LSB
#error "elf.h:R_IA64_DTPREL32LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_DTPREL32MSB
#error "elf.h:R_IA64_DTPREL32MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_DTPREL64I
#error "elf.h:R_IA64_DTPREL64I macro is missing from libc-shim"
#endif

#ifndef R_IA64_DTPREL64LSB
#error "elf.h:R_IA64_DTPREL64LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_DTPREL64MSB
#error "elf.h:R_IA64_DTPREL64MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_FPTR32LSB
#error "elf.h:R_IA64_FPTR32LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_FPTR32MSB
#error "elf.h:R_IA64_FPTR32MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_FPTR64I
#error "elf.h:R_IA64_FPTR64I macro is missing from libc-shim"
#endif

#ifndef R_IA64_FPTR64LSB
#error "elf.h:R_IA64_FPTR64LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_FPTR64MSB
#error "elf.h:R_IA64_FPTR64MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_GPREL22
#error "elf.h:R_IA64_GPREL22 macro is missing from libc-shim"
#endif

#ifndef R_IA64_GPREL32LSB
#error "elf.h:R_IA64_GPREL32LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_GPREL32MSB
#error "elf.h:R_IA64_GPREL32MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_GPREL64I
#error "elf.h:R_IA64_GPREL64I macro is missing from libc-shim"
#endif

#ifndef R_IA64_GPREL64LSB
#error "elf.h:R_IA64_GPREL64LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_GPREL64MSB
#error "elf.h:R_IA64_GPREL64MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_IMM14
#error "elf.h:R_IA64_IMM14 macro is missing from libc-shim"
#endif

#ifndef R_IA64_IMM22
#error "elf.h:R_IA64_IMM22 macro is missing from libc-shim"
#endif

#ifndef R_IA64_IMM64
#error "elf.h:R_IA64_IMM64 macro is missing from libc-shim"
#endif

#ifndef R_IA64_IPLTLSB
#error "elf.h:R_IA64_IPLTLSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_IPLTMSB
#error "elf.h:R_IA64_IPLTMSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_LDXMOV
#error "elf.h:R_IA64_LDXMOV macro is missing from libc-shim"
#endif

#ifndef R_IA64_LTOFF22
#error "elf.h:R_IA64_LTOFF22 macro is missing from libc-shim"
#endif

#ifndef R_IA64_LTOFF22X
#error "elf.h:R_IA64_LTOFF22X macro is missing from libc-shim"
#endif

#ifndef R_IA64_LTOFF64I
#error "elf.h:R_IA64_LTOFF64I macro is missing from libc-shim"
#endif

#ifndef R_IA64_LTOFF_DTPMOD22
#error "elf.h:R_IA64_LTOFF_DTPMOD22 macro is missing from libc-shim"
#endif

#ifndef R_IA64_LTOFF_DTPREL22
#error "elf.h:R_IA64_LTOFF_DTPREL22 macro is missing from libc-shim"
#endif

#ifndef R_IA64_LTOFF_FPTR22
#error "elf.h:R_IA64_LTOFF_FPTR22 macro is missing from libc-shim"
#endif

#ifndef R_IA64_LTOFF_FPTR32LSB
#error "elf.h:R_IA64_LTOFF_FPTR32LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_LTOFF_FPTR32MSB
#error "elf.h:R_IA64_LTOFF_FPTR32MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_LTOFF_FPTR64I
#error "elf.h:R_IA64_LTOFF_FPTR64I macro is missing from libc-shim"
#endif

#ifndef R_IA64_LTOFF_FPTR64LSB
#error "elf.h:R_IA64_LTOFF_FPTR64LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_LTOFF_FPTR64MSB
#error "elf.h:R_IA64_LTOFF_FPTR64MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_LTOFF_TPREL22
#error "elf.h:R_IA64_LTOFF_TPREL22 macro is missing from libc-shim"
#endif

#ifndef R_IA64_LTV32LSB
#error "elf.h:R_IA64_LTV32LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_LTV32MSB
#error "elf.h:R_IA64_LTV32MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_LTV64LSB
#error "elf.h:R_IA64_LTV64LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_LTV64MSB
#error "elf.h:R_IA64_LTV64MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_NONE
#error "elf.h:R_IA64_NONE macro is missing from libc-shim"
#endif

#ifndef R_IA64_PCREL21B
#error "elf.h:R_IA64_PCREL21B macro is missing from libc-shim"
#endif

#ifndef R_IA64_PCREL21BI
#error "elf.h:R_IA64_PCREL21BI macro is missing from libc-shim"
#endif

#ifndef R_IA64_PCREL21F
#error "elf.h:R_IA64_PCREL21F macro is missing from libc-shim"
#endif

#ifndef R_IA64_PCREL21M
#error "elf.h:R_IA64_PCREL21M macro is missing from libc-shim"
#endif

#ifndef R_IA64_PCREL22
#error "elf.h:R_IA64_PCREL22 macro is missing from libc-shim"
#endif

#ifndef R_IA64_PCREL32LSB
#error "elf.h:R_IA64_PCREL32LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_PCREL32MSB
#error "elf.h:R_IA64_PCREL32MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_PCREL60B
#error "elf.h:R_IA64_PCREL60B macro is missing from libc-shim"
#endif

#ifndef R_IA64_PCREL64I
#error "elf.h:R_IA64_PCREL64I macro is missing from libc-shim"
#endif

#ifndef R_IA64_PCREL64LSB
#error "elf.h:R_IA64_PCREL64LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_PCREL64MSB
#error "elf.h:R_IA64_PCREL64MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_PLTOFF22
#error "elf.h:R_IA64_PLTOFF22 macro is missing from libc-shim"
#endif

#ifndef R_IA64_PLTOFF64I
#error "elf.h:R_IA64_PLTOFF64I macro is missing from libc-shim"
#endif

#ifndef R_IA64_PLTOFF64LSB
#error "elf.h:R_IA64_PLTOFF64LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_PLTOFF64MSB
#error "elf.h:R_IA64_PLTOFF64MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_REL32LSB
#error "elf.h:R_IA64_REL32LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_REL32MSB
#error "elf.h:R_IA64_REL32MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_REL64LSB
#error "elf.h:R_IA64_REL64LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_REL64MSB
#error "elf.h:R_IA64_REL64MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_SECREL32LSB
#error "elf.h:R_IA64_SECREL32LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_SECREL32MSB
#error "elf.h:R_IA64_SECREL32MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_SECREL64LSB
#error "elf.h:R_IA64_SECREL64LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_SECREL64MSB
#error "elf.h:R_IA64_SECREL64MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_SEGREL32LSB
#error "elf.h:R_IA64_SEGREL32LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_SEGREL32MSB
#error "elf.h:R_IA64_SEGREL32MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_SEGREL64LSB
#error "elf.h:R_IA64_SEGREL64LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_SEGREL64MSB
#error "elf.h:R_IA64_SEGREL64MSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_SUB
#error "elf.h:R_IA64_SUB macro is missing from libc-shim"
#endif

#ifndef R_IA64_TPREL14
#error "elf.h:R_IA64_TPREL14 macro is missing from libc-shim"
#endif

#ifndef R_IA64_TPREL22
#error "elf.h:R_IA64_TPREL22 macro is missing from libc-shim"
#endif

#ifndef R_IA64_TPREL64I
#error "elf.h:R_IA64_TPREL64I macro is missing from libc-shim"
#endif

#ifndef R_IA64_TPREL64LSB
#error "elf.h:R_IA64_TPREL64LSB macro is missing from libc-shim"
#endif

#ifndef R_IA64_TPREL64MSB
#error "elf.h:R_IA64_TPREL64MSB macro is missing from libc-shim"
#endif

#ifndef R_LARCH_32
#error "elf.h:R_LARCH_32 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_32_PCREL
#error "elf.h:R_LARCH_32_PCREL macro is missing from libc-shim"
#endif

#ifndef R_LARCH_64
#error "elf.h:R_LARCH_64 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_64_PCREL
#error "elf.h:R_LARCH_64_PCREL macro is missing from libc-shim"
#endif

#ifndef R_LARCH_ABS64_HI12
#error "elf.h:R_LARCH_ABS64_HI12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_ABS64_LO20
#error "elf.h:R_LARCH_ABS64_LO20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_ABS_HI20
#error "elf.h:R_LARCH_ABS_HI20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_ABS_LO12
#error "elf.h:R_LARCH_ABS_LO12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_ADD16
#error "elf.h:R_LARCH_ADD16 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_ADD24
#error "elf.h:R_LARCH_ADD24 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_ADD32
#error "elf.h:R_LARCH_ADD32 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_ADD6
#error "elf.h:R_LARCH_ADD6 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_ADD64
#error "elf.h:R_LARCH_ADD64 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_ADD8
#error "elf.h:R_LARCH_ADD8 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_ADD_ULEB128
#error "elf.h:R_LARCH_ADD_ULEB128 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_ALIGN
#error "elf.h:R_LARCH_ALIGN macro is missing from libc-shim"
#endif

#ifndef R_LARCH_B16
#error "elf.h:R_LARCH_B16 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_B21
#error "elf.h:R_LARCH_B21 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_B26
#error "elf.h:R_LARCH_B26 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_CALL36
#error "elf.h:R_LARCH_CALL36 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_CFA
#error "elf.h:R_LARCH_CFA macro is missing from libc-shim"
#endif

#ifndef R_LARCH_COPY
#error "elf.h:R_LARCH_COPY macro is missing from libc-shim"
#endif

#ifndef R_LARCH_DELETE
#error "elf.h:R_LARCH_DELETE macro is missing from libc-shim"
#endif

#ifndef R_LARCH_GNU_VTENTRY
#error "elf.h:R_LARCH_GNU_VTENTRY macro is missing from libc-shim"
#endif

#ifndef R_LARCH_GNU_VTINHERIT
#error "elf.h:R_LARCH_GNU_VTINHERIT macro is missing from libc-shim"
#endif

#ifndef R_LARCH_GOT64_HI12
#error "elf.h:R_LARCH_GOT64_HI12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_GOT64_LO20
#error "elf.h:R_LARCH_GOT64_LO20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_GOT64_PC_HI12
#error "elf.h:R_LARCH_GOT64_PC_HI12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_GOT64_PC_LO20
#error "elf.h:R_LARCH_GOT64_PC_LO20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_GOT_HI20
#error "elf.h:R_LARCH_GOT_HI20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_GOT_LO12
#error "elf.h:R_LARCH_GOT_LO12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_GOT_PC_HI20
#error "elf.h:R_LARCH_GOT_PC_HI20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_GOT_PC_LO12
#error "elf.h:R_LARCH_GOT_PC_LO12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_IRELATIVE
#error "elf.h:R_LARCH_IRELATIVE macro is missing from libc-shim"
#endif

#ifndef R_LARCH_JUMP_SLOT
#error "elf.h:R_LARCH_JUMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_LARCH_MARK_LA
#error "elf.h:R_LARCH_MARK_LA macro is missing from libc-shim"
#endif

#ifndef R_LARCH_MARK_PCREL
#error "elf.h:R_LARCH_MARK_PCREL macro is missing from libc-shim"
#endif

#ifndef R_LARCH_NONE
#error "elf.h:R_LARCH_NONE macro is missing from libc-shim"
#endif

#ifndef R_LARCH_PCALA64_HI12
#error "elf.h:R_LARCH_PCALA64_HI12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_PCALA64_LO20
#error "elf.h:R_LARCH_PCALA64_LO20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_PCALA_HI20
#error "elf.h:R_LARCH_PCALA_HI20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_PCALA_LO12
#error "elf.h:R_LARCH_PCALA_LO12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_PCREL20_S2
#error "elf.h:R_LARCH_PCREL20_S2 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_RELATIVE
#error "elf.h:R_LARCH_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_LARCH_RELAX
#error "elf.h:R_LARCH_RELAX macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_ADD
#error "elf.h:R_LARCH_SOP_ADD macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_AND
#error "elf.h:R_LARCH_SOP_AND macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_ASSERT
#error "elf.h:R_LARCH_SOP_ASSERT macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_IF_ELSE
#error "elf.h:R_LARCH_SOP_IF_ELSE macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_NOT
#error "elf.h:R_LARCH_SOP_NOT macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_POP_32_S_0_10_10_16_S2
#error "elf.h:R_LARCH_SOP_POP_32_S_0_10_10_16_S2 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_POP_32_S_0_5_10_16_S2
#error "elf.h:R_LARCH_SOP_POP_32_S_0_5_10_16_S2 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_POP_32_S_10_12
#error "elf.h:R_LARCH_SOP_POP_32_S_10_12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_POP_32_S_10_16
#error "elf.h:R_LARCH_SOP_POP_32_S_10_16 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_POP_32_S_10_16_S2
#error "elf.h:R_LARCH_SOP_POP_32_S_10_16_S2 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_POP_32_S_10_5
#error "elf.h:R_LARCH_SOP_POP_32_S_10_5 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_POP_32_S_5_20
#error "elf.h:R_LARCH_SOP_POP_32_S_5_20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_POP_32_U
#error "elf.h:R_LARCH_SOP_POP_32_U macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_POP_32_U_10_12
#error "elf.h:R_LARCH_SOP_POP_32_U_10_12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_PUSH_ABSOLUTE
#error "elf.h:R_LARCH_SOP_PUSH_ABSOLUTE macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_PUSH_DUP
#error "elf.h:R_LARCH_SOP_PUSH_DUP macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_PUSH_GPREL
#error "elf.h:R_LARCH_SOP_PUSH_GPREL macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_PUSH_PCREL
#error "elf.h:R_LARCH_SOP_PUSH_PCREL macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_PUSH_PLT_PCREL
#error "elf.h:R_LARCH_SOP_PUSH_PLT_PCREL macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_PUSH_TLS_GD
#error "elf.h:R_LARCH_SOP_PUSH_TLS_GD macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_PUSH_TLS_GOT
#error "elf.h:R_LARCH_SOP_PUSH_TLS_GOT macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_PUSH_TLS_TPREL
#error "elf.h:R_LARCH_SOP_PUSH_TLS_TPREL macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_SL
#error "elf.h:R_LARCH_SOP_SL macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_SR
#error "elf.h:R_LARCH_SOP_SR macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SOP_SUB
#error "elf.h:R_LARCH_SOP_SUB macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SUB16
#error "elf.h:R_LARCH_SUB16 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SUB24
#error "elf.h:R_LARCH_SUB24 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SUB32
#error "elf.h:R_LARCH_SUB32 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SUB6
#error "elf.h:R_LARCH_SUB6 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SUB64
#error "elf.h:R_LARCH_SUB64 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SUB8
#error "elf.h:R_LARCH_SUB8 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_SUB_ULEB128
#error "elf.h:R_LARCH_SUB_ULEB128 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DESC32
#error "elf.h:R_LARCH_TLS_DESC32 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DESC64
#error "elf.h:R_LARCH_TLS_DESC64 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DESC64_HI12
#error "elf.h:R_LARCH_TLS_DESC64_HI12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DESC64_LO20
#error "elf.h:R_LARCH_TLS_DESC64_LO20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DESC64_PC_HI12
#error "elf.h:R_LARCH_TLS_DESC64_PC_HI12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DESC64_PC_LO20
#error "elf.h:R_LARCH_TLS_DESC64_PC_LO20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DESC_CALL
#error "elf.h:R_LARCH_TLS_DESC_CALL macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DESC_HI20
#error "elf.h:R_LARCH_TLS_DESC_HI20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DESC_LD
#error "elf.h:R_LARCH_TLS_DESC_LD macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DESC_LO12
#error "elf.h:R_LARCH_TLS_DESC_LO12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DESC_PCREL20_S2
#error "elf.h:R_LARCH_TLS_DESC_PCREL20_S2 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DESC_PC_HI20
#error "elf.h:R_LARCH_TLS_DESC_PC_HI20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DESC_PC_LO12
#error "elf.h:R_LARCH_TLS_DESC_PC_LO12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DTPMOD32
#error "elf.h:R_LARCH_TLS_DTPMOD32 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DTPMOD64
#error "elf.h:R_LARCH_TLS_DTPMOD64 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DTPREL32
#error "elf.h:R_LARCH_TLS_DTPREL32 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_DTPREL64
#error "elf.h:R_LARCH_TLS_DTPREL64 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_GD_HI20
#error "elf.h:R_LARCH_TLS_GD_HI20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_GD_PCREL20_S2
#error "elf.h:R_LARCH_TLS_GD_PCREL20_S2 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_GD_PC_HI20
#error "elf.h:R_LARCH_TLS_GD_PC_HI20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_IE64_HI12
#error "elf.h:R_LARCH_TLS_IE64_HI12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_IE64_LO20
#error "elf.h:R_LARCH_TLS_IE64_LO20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_IE64_PC_HI12
#error "elf.h:R_LARCH_TLS_IE64_PC_HI12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_IE64_PC_LO20
#error "elf.h:R_LARCH_TLS_IE64_PC_LO20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_IE_HI20
#error "elf.h:R_LARCH_TLS_IE_HI20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_IE_LO12
#error "elf.h:R_LARCH_TLS_IE_LO12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_IE_PC_HI20
#error "elf.h:R_LARCH_TLS_IE_PC_HI20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_IE_PC_LO12
#error "elf.h:R_LARCH_TLS_IE_PC_LO12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_LD_HI20
#error "elf.h:R_LARCH_TLS_LD_HI20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_LD_PCREL20_S2
#error "elf.h:R_LARCH_TLS_LD_PCREL20_S2 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_LD_PC_HI20
#error "elf.h:R_LARCH_TLS_LD_PC_HI20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_LE64_HI12
#error "elf.h:R_LARCH_TLS_LE64_HI12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_LE64_LO20
#error "elf.h:R_LARCH_TLS_LE64_LO20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_LE_ADD_R
#error "elf.h:R_LARCH_TLS_LE_ADD_R macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_LE_HI20
#error "elf.h:R_LARCH_TLS_LE_HI20 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_LE_HI20_R
#error "elf.h:R_LARCH_TLS_LE_HI20_R macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_LE_LO12
#error "elf.h:R_LARCH_TLS_LE_LO12 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_LE_LO12_R
#error "elf.h:R_LARCH_TLS_LE_LO12_R macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_TPREL32
#error "elf.h:R_LARCH_TLS_TPREL32 macro is missing from libc-shim"
#endif

#ifndef R_LARCH_TLS_TPREL64
#error "elf.h:R_LARCH_TLS_TPREL64 macro is missing from libc-shim"
#endif

#ifndef R_M32R_10_PCREL
#error "elf.h:R_M32R_10_PCREL macro is missing from libc-shim"
#endif

#ifndef R_M32R_10_PCREL_RELA
#error "elf.h:R_M32R_10_PCREL_RELA macro is missing from libc-shim"
#endif

#ifndef R_M32R_16
#error "elf.h:R_M32R_16 macro is missing from libc-shim"
#endif

#ifndef R_M32R_16_RELA
#error "elf.h:R_M32R_16_RELA macro is missing from libc-shim"
#endif

#ifndef R_M32R_18_PCREL
#error "elf.h:R_M32R_18_PCREL macro is missing from libc-shim"
#endif

#ifndef R_M32R_18_PCREL_RELA
#error "elf.h:R_M32R_18_PCREL_RELA macro is missing from libc-shim"
#endif

#ifndef R_M32R_24
#error "elf.h:R_M32R_24 macro is missing from libc-shim"
#endif

#ifndef R_M32R_24_RELA
#error "elf.h:R_M32R_24_RELA macro is missing from libc-shim"
#endif

#ifndef R_M32R_26_PCREL
#error "elf.h:R_M32R_26_PCREL macro is missing from libc-shim"
#endif

#ifndef R_M32R_26_PCREL_RELA
#error "elf.h:R_M32R_26_PCREL_RELA macro is missing from libc-shim"
#endif

#ifndef R_M32R_26_PLTREL
#error "elf.h:R_M32R_26_PLTREL macro is missing from libc-shim"
#endif

#ifndef R_M32R_32
#error "elf.h:R_M32R_32 macro is missing from libc-shim"
#endif

#ifndef R_M32R_32_RELA
#error "elf.h:R_M32R_32_RELA macro is missing from libc-shim"
#endif

#ifndef R_M32R_COPY
#error "elf.h:R_M32R_COPY macro is missing from libc-shim"
#endif

#ifndef R_M32R_GLOB_DAT
#error "elf.h:R_M32R_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_M32R_GNU_VTENTRY
#error "elf.h:R_M32R_GNU_VTENTRY macro is missing from libc-shim"
#endif

#ifndef R_M32R_GNU_VTINHERIT
#error "elf.h:R_M32R_GNU_VTINHERIT macro is missing from libc-shim"
#endif

#ifndef R_M32R_GOT16_HI_SLO
#error "elf.h:R_M32R_GOT16_HI_SLO macro is missing from libc-shim"
#endif

#ifndef R_M32R_GOT16_HI_ULO
#error "elf.h:R_M32R_GOT16_HI_ULO macro is missing from libc-shim"
#endif

#ifndef R_M32R_GOT16_LO
#error "elf.h:R_M32R_GOT16_LO macro is missing from libc-shim"
#endif

#ifndef R_M32R_GOT24
#error "elf.h:R_M32R_GOT24 macro is missing from libc-shim"
#endif

#ifndef R_M32R_GOTOFF
#error "elf.h:R_M32R_GOTOFF macro is missing from libc-shim"
#endif

#ifndef R_M32R_GOTOFF_HI_SLO
#error "elf.h:R_M32R_GOTOFF_HI_SLO macro is missing from libc-shim"
#endif

#ifndef R_M32R_GOTOFF_HI_ULO
#error "elf.h:R_M32R_GOTOFF_HI_ULO macro is missing from libc-shim"
#endif

#ifndef R_M32R_GOTOFF_LO
#error "elf.h:R_M32R_GOTOFF_LO macro is missing from libc-shim"
#endif

#ifndef R_M32R_GOTPC24
#error "elf.h:R_M32R_GOTPC24 macro is missing from libc-shim"
#endif

#ifndef R_M32R_GOTPC_HI_SLO
#error "elf.h:R_M32R_GOTPC_HI_SLO macro is missing from libc-shim"
#endif

#ifndef R_M32R_GOTPC_HI_ULO
#error "elf.h:R_M32R_GOTPC_HI_ULO macro is missing from libc-shim"
#endif

#ifndef R_M32R_GOTPC_LO
#error "elf.h:R_M32R_GOTPC_LO macro is missing from libc-shim"
#endif

#ifndef R_M32R_HI16_SLO
#error "elf.h:R_M32R_HI16_SLO macro is missing from libc-shim"
#endif

#ifndef R_M32R_HI16_SLO_RELA
#error "elf.h:R_M32R_HI16_SLO_RELA macro is missing from libc-shim"
#endif

#ifndef R_M32R_HI16_ULO
#error "elf.h:R_M32R_HI16_ULO macro is missing from libc-shim"
#endif

#ifndef R_M32R_HI16_ULO_RELA
#error "elf.h:R_M32R_HI16_ULO_RELA macro is missing from libc-shim"
#endif

#ifndef R_M32R_JMP_SLOT
#error "elf.h:R_M32R_JMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_M32R_LO16
#error "elf.h:R_M32R_LO16 macro is missing from libc-shim"
#endif

#ifndef R_M32R_LO16_RELA
#error "elf.h:R_M32R_LO16_RELA macro is missing from libc-shim"
#endif

#ifndef R_M32R_NONE
#error "elf.h:R_M32R_NONE macro is missing from libc-shim"
#endif

#ifndef R_M32R_NUM
#error "elf.h:R_M32R_NUM macro is missing from libc-shim"
#endif

#ifndef R_M32R_REL32
#error "elf.h:R_M32R_REL32 macro is missing from libc-shim"
#endif

#ifndef R_M32R_RELATIVE
#error "elf.h:R_M32R_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_M32R_RELA_GNU_VTENTRY
#error "elf.h:R_M32R_RELA_GNU_VTENTRY macro is missing from libc-shim"
#endif

#ifndef R_M32R_RELA_GNU_VTINHERIT
#error "elf.h:R_M32R_RELA_GNU_VTINHERIT macro is missing from libc-shim"
#endif

#ifndef R_M32R_SDA16
#error "elf.h:R_M32R_SDA16 macro is missing from libc-shim"
#endif

#ifndef R_M32R_SDA16_RELA
#error "elf.h:R_M32R_SDA16_RELA macro is missing from libc-shim"
#endif

#ifndef R_METAG_ADDR32
#error "elf.h:R_METAG_ADDR32 macro is missing from libc-shim"
#endif

#ifndef R_METAG_COPY
#error "elf.h:R_METAG_COPY macro is missing from libc-shim"
#endif

#ifndef R_METAG_GETSETOFF
#error "elf.h:R_METAG_GETSETOFF macro is missing from libc-shim"
#endif

#ifndef R_METAG_GETSET_GOT
#error "elf.h:R_METAG_GETSET_GOT macro is missing from libc-shim"
#endif

#ifndef R_METAG_GETSET_GOTOFF
#error "elf.h:R_METAG_GETSET_GOTOFF macro is missing from libc-shim"
#endif

#ifndef R_METAG_GLOB_DAT
#error "elf.h:R_METAG_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_METAG_GNU_VTENTRY
#error "elf.h:R_METAG_GNU_VTENTRY macro is missing from libc-shim"
#endif

#ifndef R_METAG_GNU_VTINHERIT
#error "elf.h:R_METAG_GNU_VTINHERIT macro is missing from libc-shim"
#endif

#ifndef R_METAG_GOTOFF
#error "elf.h:R_METAG_GOTOFF macro is missing from libc-shim"
#endif

#ifndef R_METAG_HI16_GOTOFF
#error "elf.h:R_METAG_HI16_GOTOFF macro is missing from libc-shim"
#endif

#ifndef R_METAG_HI16_GOTPC
#error "elf.h:R_METAG_HI16_GOTPC macro is missing from libc-shim"
#endif

#ifndef R_METAG_HI16_PLT
#error "elf.h:R_METAG_HI16_PLT macro is missing from libc-shim"
#endif

#ifndef R_METAG_HIADDR16
#error "elf.h:R_METAG_HIADDR16 macro is missing from libc-shim"
#endif

#ifndef R_METAG_HIOG
#error "elf.h:R_METAG_HIOG macro is missing from libc-shim"
#endif

#ifndef R_METAG_JMP_SLOT
#error "elf.h:R_METAG_JMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_METAG_LO16_GOTOFF
#error "elf.h:R_METAG_LO16_GOTOFF macro is missing from libc-shim"
#endif

#ifndef R_METAG_LO16_GOTPC
#error "elf.h:R_METAG_LO16_GOTPC macro is missing from libc-shim"
#endif

#ifndef R_METAG_LO16_PLT
#error "elf.h:R_METAG_LO16_PLT macro is missing from libc-shim"
#endif

#ifndef R_METAG_LOADDR16
#error "elf.h:R_METAG_LOADDR16 macro is missing from libc-shim"
#endif

#ifndef R_METAG_LOOG
#error "elf.h:R_METAG_LOOG macro is missing from libc-shim"
#endif

#ifndef R_METAG_NONE
#error "elf.h:R_METAG_NONE macro is missing from libc-shim"
#endif

#ifndef R_METAG_PLT
#error "elf.h:R_METAG_PLT macro is missing from libc-shim"
#endif

#ifndef R_METAG_REG16OP1
#error "elf.h:R_METAG_REG16OP1 macro is missing from libc-shim"
#endif

#ifndef R_METAG_REG16OP2
#error "elf.h:R_METAG_REG16OP2 macro is missing from libc-shim"
#endif

#ifndef R_METAG_REG16OP3
#error "elf.h:R_METAG_REG16OP3 macro is missing from libc-shim"
#endif

#ifndef R_METAG_REG32OP1
#error "elf.h:R_METAG_REG32OP1 macro is missing from libc-shim"
#endif

#ifndef R_METAG_REG32OP2
#error "elf.h:R_METAG_REG32OP2 macro is missing from libc-shim"
#endif

#ifndef R_METAG_REG32OP3
#error "elf.h:R_METAG_REG32OP3 macro is missing from libc-shim"
#endif

#ifndef R_METAG_REG32OP4
#error "elf.h:R_METAG_REG32OP4 macro is missing from libc-shim"
#endif

#ifndef R_METAG_REL16
#error "elf.h:R_METAG_REL16 macro is missing from libc-shim"
#endif

#ifndef R_METAG_REL8
#error "elf.h:R_METAG_REL8 macro is missing from libc-shim"
#endif

#ifndef R_METAG_RELATIVE
#error "elf.h:R_METAG_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_METAG_RELBRANCH
#error "elf.h:R_METAG_RELBRANCH macro is missing from libc-shim"
#endif

#ifndef R_METAG_RELBRANCH_PLT
#error "elf.h:R_METAG_RELBRANCH_PLT macro is missing from libc-shim"
#endif

#ifndef R_METAG_TLS_DTPMOD
#error "elf.h:R_METAG_TLS_DTPMOD macro is missing from libc-shim"
#endif

#ifndef R_METAG_TLS_DTPOFF
#error "elf.h:R_METAG_TLS_DTPOFF macro is missing from libc-shim"
#endif

#ifndef R_METAG_TLS_GD
#error "elf.h:R_METAG_TLS_GD macro is missing from libc-shim"
#endif

#ifndef R_METAG_TLS_IE
#error "elf.h:R_METAG_TLS_IE macro is missing from libc-shim"
#endif

#ifndef R_METAG_TLS_IENONPIC
#error "elf.h:R_METAG_TLS_IENONPIC macro is missing from libc-shim"
#endif

#ifndef R_METAG_TLS_IENONPIC_HI16
#error "elf.h:R_METAG_TLS_IENONPIC_HI16 macro is missing from libc-shim"
#endif

#ifndef R_METAG_TLS_IENONPIC_LO16
#error "elf.h:R_METAG_TLS_IENONPIC_LO16 macro is missing from libc-shim"
#endif

#ifndef R_METAG_TLS_LDM
#error "elf.h:R_METAG_TLS_LDM macro is missing from libc-shim"
#endif

#ifndef R_METAG_TLS_LDO
#error "elf.h:R_METAG_TLS_LDO macro is missing from libc-shim"
#endif

#ifndef R_METAG_TLS_LDO_HI16
#error "elf.h:R_METAG_TLS_LDO_HI16 macro is missing from libc-shim"
#endif

#ifndef R_METAG_TLS_LDO_LO16
#error "elf.h:R_METAG_TLS_LDO_LO16 macro is missing from libc-shim"
#endif

#ifndef R_METAG_TLS_LE
#error "elf.h:R_METAG_TLS_LE macro is missing from libc-shim"
#endif

#ifndef R_METAG_TLS_LE_HI16
#error "elf.h:R_METAG_TLS_LE_HI16 macro is missing from libc-shim"
#endif

#ifndef R_METAG_TLS_LE_LO16
#error "elf.h:R_METAG_TLS_LE_LO16 macro is missing from libc-shim"
#endif

#ifndef R_METAG_TLS_TPOFF
#error "elf.h:R_METAG_TLS_TPOFF macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_32
#error "elf.h:R_MICROBLAZE_32 macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_32_LO
#error "elf.h:R_MICROBLAZE_32_LO macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_32_PCREL
#error "elf.h:R_MICROBLAZE_32_PCREL macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_32_PCREL_LO
#error "elf.h:R_MICROBLAZE_32_PCREL_LO macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_32_SYM_OP_SYM
#error "elf.h:R_MICROBLAZE_32_SYM_OP_SYM macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_64
#error "elf.h:R_MICROBLAZE_64 macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_64_NONE
#error "elf.h:R_MICROBLAZE_64_NONE macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_64_PCREL
#error "elf.h:R_MICROBLAZE_64_PCREL macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_COPY
#error "elf.h:R_MICROBLAZE_COPY macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_GLOB_DAT
#error "elf.h:R_MICROBLAZE_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_GNU_VTENTRY
#error "elf.h:R_MICROBLAZE_GNU_VTENTRY macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_GNU_VTINHERIT
#error "elf.h:R_MICROBLAZE_GNU_VTINHERIT macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_GOTOFF_32
#error "elf.h:R_MICROBLAZE_GOTOFF_32 macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_GOTOFF_64
#error "elf.h:R_MICROBLAZE_GOTOFF_64 macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_GOTPC_64
#error "elf.h:R_MICROBLAZE_GOTPC_64 macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_GOT_64
#error "elf.h:R_MICROBLAZE_GOT_64 macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_JUMP_SLOT
#error "elf.h:R_MICROBLAZE_JUMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_NONE
#error "elf.h:R_MICROBLAZE_NONE macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_PLT_64
#error "elf.h:R_MICROBLAZE_PLT_64 macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_REL
#error "elf.h:R_MICROBLAZE_REL macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_SRO32
#error "elf.h:R_MICROBLAZE_SRO32 macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_SRW32
#error "elf.h:R_MICROBLAZE_SRW32 macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_TLS
#error "elf.h:R_MICROBLAZE_TLS macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_TLSDTPMOD32
#error "elf.h:R_MICROBLAZE_TLSDTPMOD32 macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_TLSDTPREL32
#error "elf.h:R_MICROBLAZE_TLSDTPREL32 macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_TLSDTPREL64
#error "elf.h:R_MICROBLAZE_TLSDTPREL64 macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_TLSGD
#error "elf.h:R_MICROBLAZE_TLSGD macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_TLSGOTTPREL32
#error "elf.h:R_MICROBLAZE_TLSGOTTPREL32 macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_TLSLD
#error "elf.h:R_MICROBLAZE_TLSLD macro is missing from libc-shim"
#endif

#ifndef R_MICROBLAZE_TLSTPREL32
#error "elf.h:R_MICROBLAZE_TLSTPREL32 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_26_S1
#error "elf.h:R_MICROMIPS_26_S1 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_CALL16
#error "elf.h:R_MICROMIPS_CALL16 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_CALL_HI16
#error "elf.h:R_MICROMIPS_CALL_HI16 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_CALL_LO16
#error "elf.h:R_MICROMIPS_CALL_LO16 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_GOT16
#error "elf.h:R_MICROMIPS_GOT16 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_GOT_DISP
#error "elf.h:R_MICROMIPS_GOT_DISP macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_GOT_HI16
#error "elf.h:R_MICROMIPS_GOT_HI16 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_GOT_LO16
#error "elf.h:R_MICROMIPS_GOT_LO16 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_GOT_OFST
#error "elf.h:R_MICROMIPS_GOT_OFST macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_GOT_PAGE
#error "elf.h:R_MICROMIPS_GOT_PAGE macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_GPREL16
#error "elf.h:R_MICROMIPS_GPREL16 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_GPREL7_S2
#error "elf.h:R_MICROMIPS_GPREL7_S2 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_HI0_LO16
#error "elf.h:R_MICROMIPS_HI0_LO16 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_HI16
#error "elf.h:R_MICROMIPS_HI16 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_HIGHER
#error "elf.h:R_MICROMIPS_HIGHER macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_HIGHEST
#error "elf.h:R_MICROMIPS_HIGHEST macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_JALR
#error "elf.h:R_MICROMIPS_JALR macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_LITERAL
#error "elf.h:R_MICROMIPS_LITERAL macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_LO16
#error "elf.h:R_MICROMIPS_LO16 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_PC10_S1
#error "elf.h:R_MICROMIPS_PC10_S1 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_PC16_S1
#error "elf.h:R_MICROMIPS_PC16_S1 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_PC23_S2
#error "elf.h:R_MICROMIPS_PC23_S2 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_PC7_S1
#error "elf.h:R_MICROMIPS_PC7_S1 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_SCN_DISP
#error "elf.h:R_MICROMIPS_SCN_DISP macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_SUB
#error "elf.h:R_MICROMIPS_SUB macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_TLS_DTPREL_HI16
#error "elf.h:R_MICROMIPS_TLS_DTPREL_HI16 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_TLS_DTPREL_LO16
#error "elf.h:R_MICROMIPS_TLS_DTPREL_LO16 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_TLS_GD
#error "elf.h:R_MICROMIPS_TLS_GD macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_TLS_GOTTPREL
#error "elf.h:R_MICROMIPS_TLS_GOTTPREL macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_TLS_LDM
#error "elf.h:R_MICROMIPS_TLS_LDM macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_TLS_TPREL_HI16
#error "elf.h:R_MICROMIPS_TLS_TPREL_HI16 macro is missing from libc-shim"
#endif

#ifndef R_MICROMIPS_TLS_TPREL_LO16
#error "elf.h:R_MICROMIPS_TLS_TPREL_LO16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS16_26
#error "elf.h:R_MIPS16_26 macro is missing from libc-shim"
#endif

#ifndef R_MIPS16_CALL16
#error "elf.h:R_MIPS16_CALL16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS16_GOT16
#error "elf.h:R_MIPS16_GOT16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS16_GPREL
#error "elf.h:R_MIPS16_GPREL macro is missing from libc-shim"
#endif

#ifndef R_MIPS16_HI16
#error "elf.h:R_MIPS16_HI16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS16_LO16
#error "elf.h:R_MIPS16_LO16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS16_PC16_S1
#error "elf.h:R_MIPS16_PC16_S1 macro is missing from libc-shim"
#endif

#ifndef R_MIPS16_TLS_DTPREL_HI16
#error "elf.h:R_MIPS16_TLS_DTPREL_HI16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS16_TLS_DTPREL_LO16
#error "elf.h:R_MIPS16_TLS_DTPREL_LO16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS16_TLS_GD
#error "elf.h:R_MIPS16_TLS_GD macro is missing from libc-shim"
#endif

#ifndef R_MIPS16_TLS_GOTTPREL
#error "elf.h:R_MIPS16_TLS_GOTTPREL macro is missing from libc-shim"
#endif

#ifndef R_MIPS16_TLS_LDM
#error "elf.h:R_MIPS16_TLS_LDM macro is missing from libc-shim"
#endif

#ifndef R_MIPS16_TLS_TPREL_HI16
#error "elf.h:R_MIPS16_TLS_TPREL_HI16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS16_TLS_TPREL_LO16
#error "elf.h:R_MIPS16_TLS_TPREL_LO16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_16
#error "elf.h:R_MIPS_16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_26
#error "elf.h:R_MIPS_26 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_32
#error "elf.h:R_MIPS_32 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_64
#error "elf.h:R_MIPS_64 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_ADD_IMMEDIATE
#error "elf.h:R_MIPS_ADD_IMMEDIATE macro is missing from libc-shim"
#endif

#ifndef R_MIPS_CALL16
#error "elf.h:R_MIPS_CALL16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_CALL_HI16
#error "elf.h:R_MIPS_CALL_HI16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_CALL_LO16
#error "elf.h:R_MIPS_CALL_LO16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_COPY
#error "elf.h:R_MIPS_COPY macro is missing from libc-shim"
#endif

#ifndef R_MIPS_DELETE
#error "elf.h:R_MIPS_DELETE macro is missing from libc-shim"
#endif

#ifndef R_MIPS_EH
#error "elf.h:R_MIPS_EH macro is missing from libc-shim"
#endif

#ifndef R_MIPS_GLOB_DAT
#error "elf.h:R_MIPS_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_MIPS_GNU_REL16_S2
#error "elf.h:R_MIPS_GNU_REL16_S2 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_GNU_VTENTRY
#error "elf.h:R_MIPS_GNU_VTENTRY macro is missing from libc-shim"
#endif

#ifndef R_MIPS_GNU_VTINHERIT
#error "elf.h:R_MIPS_GNU_VTINHERIT macro is missing from libc-shim"
#endif

#ifndef R_MIPS_GOT16
#error "elf.h:R_MIPS_GOT16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_GOT_DISP
#error "elf.h:R_MIPS_GOT_DISP macro is missing from libc-shim"
#endif

#ifndef R_MIPS_GOT_HI16
#error "elf.h:R_MIPS_GOT_HI16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_GOT_LO16
#error "elf.h:R_MIPS_GOT_LO16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_GOT_OFST
#error "elf.h:R_MIPS_GOT_OFST macro is missing from libc-shim"
#endif

#ifndef R_MIPS_GOT_PAGE
#error "elf.h:R_MIPS_GOT_PAGE macro is missing from libc-shim"
#endif

#ifndef R_MIPS_GPREL16
#error "elf.h:R_MIPS_GPREL16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_GPREL32
#error "elf.h:R_MIPS_GPREL32 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_HI16
#error "elf.h:R_MIPS_HI16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_HIGHER
#error "elf.h:R_MIPS_HIGHER macro is missing from libc-shim"
#endif

#ifndef R_MIPS_HIGHEST
#error "elf.h:R_MIPS_HIGHEST macro is missing from libc-shim"
#endif

#ifndef R_MIPS_INSERT_A
#error "elf.h:R_MIPS_INSERT_A macro is missing from libc-shim"
#endif

#ifndef R_MIPS_INSERT_B
#error "elf.h:R_MIPS_INSERT_B macro is missing from libc-shim"
#endif

#ifndef R_MIPS_JALR
#error "elf.h:R_MIPS_JALR macro is missing from libc-shim"
#endif

#ifndef R_MIPS_JUMP_SLOT
#error "elf.h:R_MIPS_JUMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_MIPS_LITERAL
#error "elf.h:R_MIPS_LITERAL macro is missing from libc-shim"
#endif

#ifndef R_MIPS_LO16
#error "elf.h:R_MIPS_LO16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_NONE
#error "elf.h:R_MIPS_NONE macro is missing from libc-shim"
#endif

#ifndef R_MIPS_NUM
#error "elf.h:R_MIPS_NUM macro is missing from libc-shim"
#endif

#ifndef R_MIPS_PC16
#error "elf.h:R_MIPS_PC16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_PC18_S3
#error "elf.h:R_MIPS_PC18_S3 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_PC19_S2
#error "elf.h:R_MIPS_PC19_S2 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_PC21_S2
#error "elf.h:R_MIPS_PC21_S2 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_PC26_S2
#error "elf.h:R_MIPS_PC26_S2 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_PC32
#error "elf.h:R_MIPS_PC32 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_PCHI16
#error "elf.h:R_MIPS_PCHI16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_PCLO16
#error "elf.h:R_MIPS_PCLO16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_PJUMP
#error "elf.h:R_MIPS_PJUMP macro is missing from libc-shim"
#endif

#ifndef R_MIPS_REL16
#error "elf.h:R_MIPS_REL16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_REL32
#error "elf.h:R_MIPS_REL32 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_RELATIVE
#error "elf.h:R_MIPS_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_MIPS_RELGOT
#error "elf.h:R_MIPS_RELGOT macro is missing from libc-shim"
#endif

#ifndef R_MIPS_SCN_DISP
#error "elf.h:R_MIPS_SCN_DISP macro is missing from libc-shim"
#endif

#ifndef R_MIPS_SHIFT5
#error "elf.h:R_MIPS_SHIFT5 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_SHIFT6
#error "elf.h:R_MIPS_SHIFT6 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_SUB
#error "elf.h:R_MIPS_SUB macro is missing from libc-shim"
#endif

#ifndef R_MIPS_TLS_DTPMOD32
#error "elf.h:R_MIPS_TLS_DTPMOD32 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_TLS_DTPMOD64
#error "elf.h:R_MIPS_TLS_DTPMOD64 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_TLS_DTPREL32
#error "elf.h:R_MIPS_TLS_DTPREL32 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_TLS_DTPREL64
#error "elf.h:R_MIPS_TLS_DTPREL64 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_TLS_DTPREL_HI16
#error "elf.h:R_MIPS_TLS_DTPREL_HI16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_TLS_DTPREL_LO16
#error "elf.h:R_MIPS_TLS_DTPREL_LO16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_TLS_GD
#error "elf.h:R_MIPS_TLS_GD macro is missing from libc-shim"
#endif

#ifndef R_MIPS_TLS_GOTTPREL
#error "elf.h:R_MIPS_TLS_GOTTPREL macro is missing from libc-shim"
#endif

#ifndef R_MIPS_TLS_LDM
#error "elf.h:R_MIPS_TLS_LDM macro is missing from libc-shim"
#endif

#ifndef R_MIPS_TLS_TPREL32
#error "elf.h:R_MIPS_TLS_TPREL32 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_TLS_TPREL64
#error "elf.h:R_MIPS_TLS_TPREL64 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_TLS_TPREL_HI16
#error "elf.h:R_MIPS_TLS_TPREL_HI16 macro is missing from libc-shim"
#endif

#ifndef R_MIPS_TLS_TPREL_LO16
#error "elf.h:R_MIPS_TLS_TPREL_LO16 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_16
#error "elf.h:R_MN10300_16 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_24
#error "elf.h:R_MN10300_24 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_32
#error "elf.h:R_MN10300_32 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_8
#error "elf.h:R_MN10300_8 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_ALIGN
#error "elf.h:R_MN10300_ALIGN macro is missing from libc-shim"
#endif

#ifndef R_MN10300_COPY
#error "elf.h:R_MN10300_COPY macro is missing from libc-shim"
#endif

#ifndef R_MN10300_GLOB_DAT
#error "elf.h:R_MN10300_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_MN10300_GNU_VTENTRY
#error "elf.h:R_MN10300_GNU_VTENTRY macro is missing from libc-shim"
#endif

#ifndef R_MN10300_GNU_VTINHERIT
#error "elf.h:R_MN10300_GNU_VTINHERIT macro is missing from libc-shim"
#endif

#ifndef R_MN10300_GOT16
#error "elf.h:R_MN10300_GOT16 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_GOT24
#error "elf.h:R_MN10300_GOT24 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_GOT32
#error "elf.h:R_MN10300_GOT32 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_GOTOFF16
#error "elf.h:R_MN10300_GOTOFF16 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_GOTOFF24
#error "elf.h:R_MN10300_GOTOFF24 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_GOTOFF32
#error "elf.h:R_MN10300_GOTOFF32 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_GOTPC16
#error "elf.h:R_MN10300_GOTPC16 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_GOTPC32
#error "elf.h:R_MN10300_GOTPC32 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_JMP_SLOT
#error "elf.h:R_MN10300_JMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_MN10300_NONE
#error "elf.h:R_MN10300_NONE macro is missing from libc-shim"
#endif

#ifndef R_MN10300_NUM
#error "elf.h:R_MN10300_NUM macro is missing from libc-shim"
#endif

#ifndef R_MN10300_PCREL16
#error "elf.h:R_MN10300_PCREL16 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_PCREL32
#error "elf.h:R_MN10300_PCREL32 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_PCREL8
#error "elf.h:R_MN10300_PCREL8 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_PLT16
#error "elf.h:R_MN10300_PLT16 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_PLT32
#error "elf.h:R_MN10300_PLT32 macro is missing from libc-shim"
#endif

#ifndef R_MN10300_RELATIVE
#error "elf.h:R_MN10300_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_MN10300_SYM_DIFF
#error "elf.h:R_MN10300_SYM_DIFF macro is missing from libc-shim"
#endif

#ifndef R_MN10300_TLS_DTPMOD
#error "elf.h:R_MN10300_TLS_DTPMOD macro is missing from libc-shim"
#endif

#ifndef R_MN10300_TLS_DTPOFF
#error "elf.h:R_MN10300_TLS_DTPOFF macro is missing from libc-shim"
#endif

#ifndef R_MN10300_TLS_GD
#error "elf.h:R_MN10300_TLS_GD macro is missing from libc-shim"
#endif

#ifndef R_MN10300_TLS_GOTIE
#error "elf.h:R_MN10300_TLS_GOTIE macro is missing from libc-shim"
#endif

#ifndef R_MN10300_TLS_IE
#error "elf.h:R_MN10300_TLS_IE macro is missing from libc-shim"
#endif

#ifndef R_MN10300_TLS_LD
#error "elf.h:R_MN10300_TLS_LD macro is missing from libc-shim"
#endif

#ifndef R_MN10300_TLS_LDO
#error "elf.h:R_MN10300_TLS_LDO macro is missing from libc-shim"
#endif

#ifndef R_MN10300_TLS_LE
#error "elf.h:R_MN10300_TLS_LE macro is missing from libc-shim"
#endif

#ifndef R_MN10300_TLS_TPOFF
#error "elf.h:R_MN10300_TLS_TPOFF macro is missing from libc-shim"
#endif

#ifndef R_NDS32_32_RELA
#error "elf.h:R_NDS32_32_RELA macro is missing from libc-shim"
#endif

#ifndef R_NDS32_COPY
#error "elf.h:R_NDS32_COPY macro is missing from libc-shim"
#endif

#ifndef R_NDS32_GLOB_DAT
#error "elf.h:R_NDS32_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_NDS32_JMP_SLOT
#error "elf.h:R_NDS32_JMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_NDS32_NONE
#error "elf.h:R_NDS32_NONE macro is missing from libc-shim"
#endif

#ifndef R_NDS32_RELATIVE
#error "elf.h:R_NDS32_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_NDS32_TLS_DESC
#error "elf.h:R_NDS32_TLS_DESC macro is missing from libc-shim"
#endif

#ifndef R_NDS32_TLS_TPOFF
#error "elf.h:R_NDS32_TLS_TPOFF macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_ALIGN
#error "elf.h:R_NIOS2_ALIGN macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_BFD_RELOC_16
#error "elf.h:R_NIOS2_BFD_RELOC_16 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_BFD_RELOC_32
#error "elf.h:R_NIOS2_BFD_RELOC_32 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_BFD_RELOC_8
#error "elf.h:R_NIOS2_BFD_RELOC_8 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_CACHE_OPX
#error "elf.h:R_NIOS2_CACHE_OPX macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_CALL16
#error "elf.h:R_NIOS2_CALL16 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_CALL26
#error "elf.h:R_NIOS2_CALL26 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_CALL26_NOAT
#error "elf.h:R_NIOS2_CALL26_NOAT macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_CALLR
#error "elf.h:R_NIOS2_CALLR macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_CALL_HA
#error "elf.h:R_NIOS2_CALL_HA macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_CALL_LO
#error "elf.h:R_NIOS2_CALL_LO macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_CJMP
#error "elf.h:R_NIOS2_CJMP macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_COPY
#error "elf.h:R_NIOS2_COPY macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_GLOB_DAT
#error "elf.h:R_NIOS2_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_GNU_VTENTRY
#error "elf.h:R_NIOS2_GNU_VTENTRY macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_GNU_VTINHERIT
#error "elf.h:R_NIOS2_GNU_VTINHERIT macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_GOT16
#error "elf.h:R_NIOS2_GOT16 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_GOTOFF
#error "elf.h:R_NIOS2_GOTOFF macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_GOTOFF_HA
#error "elf.h:R_NIOS2_GOTOFF_HA macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_GOTOFF_LO
#error "elf.h:R_NIOS2_GOTOFF_LO macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_GOT_HA
#error "elf.h:R_NIOS2_GOT_HA macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_GOT_LO
#error "elf.h:R_NIOS2_GOT_LO macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_GPREL
#error "elf.h:R_NIOS2_GPREL macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_HI16
#error "elf.h:R_NIOS2_HI16 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_HIADJ16
#error "elf.h:R_NIOS2_HIADJ16 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_IMM5
#error "elf.h:R_NIOS2_IMM5 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_IMM6
#error "elf.h:R_NIOS2_IMM6 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_IMM8
#error "elf.h:R_NIOS2_IMM8 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_JUMP_SLOT
#error "elf.h:R_NIOS2_JUMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_LO16
#error "elf.h:R_NIOS2_LO16 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_NONE
#error "elf.h:R_NIOS2_NONE macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_PCREL16
#error "elf.h:R_NIOS2_PCREL16 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_PCREL_HA
#error "elf.h:R_NIOS2_PCREL_HA macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_PCREL_LO
#error "elf.h:R_NIOS2_PCREL_LO macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_RELATIVE
#error "elf.h:R_NIOS2_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_S16
#error "elf.h:R_NIOS2_S16 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_TLS_DTPMOD
#error "elf.h:R_NIOS2_TLS_DTPMOD macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_TLS_DTPREL
#error "elf.h:R_NIOS2_TLS_DTPREL macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_TLS_GD16
#error "elf.h:R_NIOS2_TLS_GD16 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_TLS_IE16
#error "elf.h:R_NIOS2_TLS_IE16 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_TLS_LDM16
#error "elf.h:R_NIOS2_TLS_LDM16 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_TLS_LDO16
#error "elf.h:R_NIOS2_TLS_LDO16 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_TLS_LE16
#error "elf.h:R_NIOS2_TLS_LE16 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_TLS_TPREL
#error "elf.h:R_NIOS2_TLS_TPREL macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_U16
#error "elf.h:R_NIOS2_U16 macro is missing from libc-shim"
#endif

#ifndef R_NIOS2_UJMP
#error "elf.h:R_NIOS2_UJMP macro is missing from libc-shim"
#endif

#ifndef R_OR1K_16
#error "elf.h:R_OR1K_16 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_16_PCREL
#error "elf.h:R_OR1K_16_PCREL macro is missing from libc-shim"
#endif

#ifndef R_OR1K_32
#error "elf.h:R_OR1K_32 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_32_PCREL
#error "elf.h:R_OR1K_32_PCREL macro is missing from libc-shim"
#endif

#ifndef R_OR1K_8
#error "elf.h:R_OR1K_8 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_8_PCREL
#error "elf.h:R_OR1K_8_PCREL macro is missing from libc-shim"
#endif

#ifndef R_OR1K_COPY
#error "elf.h:R_OR1K_COPY macro is missing from libc-shim"
#endif

#ifndef R_OR1K_GLOB_DAT
#error "elf.h:R_OR1K_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_OR1K_GNU_VTENTRY
#error "elf.h:R_OR1K_GNU_VTENTRY macro is missing from libc-shim"
#endif

#ifndef R_OR1K_GNU_VTINHERIT
#error "elf.h:R_OR1K_GNU_VTINHERIT macro is missing from libc-shim"
#endif

#ifndef R_OR1K_GOT16
#error "elf.h:R_OR1K_GOT16 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_GOTOFF_HI16
#error "elf.h:R_OR1K_GOTOFF_HI16 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_GOTOFF_LO16
#error "elf.h:R_OR1K_GOTOFF_LO16 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_GOTPC_HI16
#error "elf.h:R_OR1K_GOTPC_HI16 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_GOTPC_LO16
#error "elf.h:R_OR1K_GOTPC_LO16 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_HI_16_IN_INSN
#error "elf.h:R_OR1K_HI_16_IN_INSN macro is missing from libc-shim"
#endif

#ifndef R_OR1K_INSN_REL_26
#error "elf.h:R_OR1K_INSN_REL_26 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_JMP_SLOT
#error "elf.h:R_OR1K_JMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_OR1K_LO_16_IN_INSN
#error "elf.h:R_OR1K_LO_16_IN_INSN macro is missing from libc-shim"
#endif

#ifndef R_OR1K_NONE
#error "elf.h:R_OR1K_NONE macro is missing from libc-shim"
#endif

#ifndef R_OR1K_PLT26
#error "elf.h:R_OR1K_PLT26 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_RELATIVE
#error "elf.h:R_OR1K_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_OR1K_TLS_DTPMOD
#error "elf.h:R_OR1K_TLS_DTPMOD macro is missing from libc-shim"
#endif

#ifndef R_OR1K_TLS_DTPOFF
#error "elf.h:R_OR1K_TLS_DTPOFF macro is missing from libc-shim"
#endif

#ifndef R_OR1K_TLS_GD_HI16
#error "elf.h:R_OR1K_TLS_GD_HI16 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_TLS_GD_LO16
#error "elf.h:R_OR1K_TLS_GD_LO16 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_TLS_IE_HI16
#error "elf.h:R_OR1K_TLS_IE_HI16 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_TLS_IE_LO16
#error "elf.h:R_OR1K_TLS_IE_LO16 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_TLS_LDM_HI16
#error "elf.h:R_OR1K_TLS_LDM_HI16 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_TLS_LDM_LO16
#error "elf.h:R_OR1K_TLS_LDM_LO16 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_TLS_LDO_HI16
#error "elf.h:R_OR1K_TLS_LDO_HI16 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_TLS_LDO_LO16
#error "elf.h:R_OR1K_TLS_LDO_LO16 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_TLS_LE_HI16
#error "elf.h:R_OR1K_TLS_LE_HI16 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_TLS_LE_LO16
#error "elf.h:R_OR1K_TLS_LE_LO16 macro is missing from libc-shim"
#endif

#ifndef R_OR1K_TLS_TPOFF
#error "elf.h:R_OR1K_TLS_TPOFF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_COPY
#error "elf.h:R_PARISC_COPY macro is missing from libc-shim"
#endif

#ifndef R_PARISC_DIR14DR
#error "elf.h:R_PARISC_DIR14DR macro is missing from libc-shim"
#endif

#ifndef R_PARISC_DIR14R
#error "elf.h:R_PARISC_DIR14R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_DIR14WR
#error "elf.h:R_PARISC_DIR14WR macro is missing from libc-shim"
#endif

#ifndef R_PARISC_DIR16DF
#error "elf.h:R_PARISC_DIR16DF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_DIR16F
#error "elf.h:R_PARISC_DIR16F macro is missing from libc-shim"
#endif

#ifndef R_PARISC_DIR16WF
#error "elf.h:R_PARISC_DIR16WF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_DIR17F
#error "elf.h:R_PARISC_DIR17F macro is missing from libc-shim"
#endif

#ifndef R_PARISC_DIR17R
#error "elf.h:R_PARISC_DIR17R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_DIR21L
#error "elf.h:R_PARISC_DIR21L macro is missing from libc-shim"
#endif

#ifndef R_PARISC_DIR32
#error "elf.h:R_PARISC_DIR32 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_DIR64
#error "elf.h:R_PARISC_DIR64 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_DPREL14R
#error "elf.h:R_PARISC_DPREL14R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_DPREL21L
#error "elf.h:R_PARISC_DPREL21L macro is missing from libc-shim"
#endif

#ifndef R_PARISC_EPLT
#error "elf.h:R_PARISC_EPLT macro is missing from libc-shim"
#endif

#ifndef R_PARISC_FPTR64
#error "elf.h:R_PARISC_FPTR64 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_GNU_VTENTRY
#error "elf.h:R_PARISC_GNU_VTENTRY macro is missing from libc-shim"
#endif

#ifndef R_PARISC_GNU_VTINHERIT
#error "elf.h:R_PARISC_GNU_VTINHERIT macro is missing from libc-shim"
#endif

#ifndef R_PARISC_GPREL14DR
#error "elf.h:R_PARISC_GPREL14DR macro is missing from libc-shim"
#endif

#ifndef R_PARISC_GPREL14R
#error "elf.h:R_PARISC_GPREL14R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_GPREL14WR
#error "elf.h:R_PARISC_GPREL14WR macro is missing from libc-shim"
#endif

#ifndef R_PARISC_GPREL16DF
#error "elf.h:R_PARISC_GPREL16DF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_GPREL16F
#error "elf.h:R_PARISC_GPREL16F macro is missing from libc-shim"
#endif

#ifndef R_PARISC_GPREL16WF
#error "elf.h:R_PARISC_GPREL16WF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_GPREL21L
#error "elf.h:R_PARISC_GPREL21L macro is missing from libc-shim"
#endif

#ifndef R_PARISC_GPREL64
#error "elf.h:R_PARISC_GPREL64 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_HIRESERVE
#error "elf.h:R_PARISC_HIRESERVE macro is missing from libc-shim"
#endif

#ifndef R_PARISC_IPLT
#error "elf.h:R_PARISC_IPLT macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LORESERVE
#error "elf.h:R_PARISC_LORESERVE macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF14DR
#error "elf.h:R_PARISC_LTOFF14DR macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF14R
#error "elf.h:R_PARISC_LTOFF14R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF14WR
#error "elf.h:R_PARISC_LTOFF14WR macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF16DF
#error "elf.h:R_PARISC_LTOFF16DF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF16F
#error "elf.h:R_PARISC_LTOFF16F macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF16WF
#error "elf.h:R_PARISC_LTOFF16WF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF21L
#error "elf.h:R_PARISC_LTOFF21L macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF64
#error "elf.h:R_PARISC_LTOFF64 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_FPTR14DR
#error "elf.h:R_PARISC_LTOFF_FPTR14DR macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_FPTR14R
#error "elf.h:R_PARISC_LTOFF_FPTR14R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_FPTR14WR
#error "elf.h:R_PARISC_LTOFF_FPTR14WR macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_FPTR16DF
#error "elf.h:R_PARISC_LTOFF_FPTR16DF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_FPTR16F
#error "elf.h:R_PARISC_LTOFF_FPTR16F macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_FPTR16WF
#error "elf.h:R_PARISC_LTOFF_FPTR16WF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_FPTR21L
#error "elf.h:R_PARISC_LTOFF_FPTR21L macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_FPTR32
#error "elf.h:R_PARISC_LTOFF_FPTR32 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_FPTR64
#error "elf.h:R_PARISC_LTOFF_FPTR64 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_TP14DR
#error "elf.h:R_PARISC_LTOFF_TP14DR macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_TP14F
#error "elf.h:R_PARISC_LTOFF_TP14F macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_TP14R
#error "elf.h:R_PARISC_LTOFF_TP14R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_TP14WR
#error "elf.h:R_PARISC_LTOFF_TP14WR macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_TP16DF
#error "elf.h:R_PARISC_LTOFF_TP16DF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_TP16F
#error "elf.h:R_PARISC_LTOFF_TP16F macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_TP16WF
#error "elf.h:R_PARISC_LTOFF_TP16WF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_TP21L
#error "elf.h:R_PARISC_LTOFF_TP21L macro is missing from libc-shim"
#endif

#ifndef R_PARISC_LTOFF_TP64
#error "elf.h:R_PARISC_LTOFF_TP64 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_NONE
#error "elf.h:R_PARISC_NONE macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PCREL14DR
#error "elf.h:R_PARISC_PCREL14DR macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PCREL14R
#error "elf.h:R_PARISC_PCREL14R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PCREL14WR
#error "elf.h:R_PARISC_PCREL14WR macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PCREL16DF
#error "elf.h:R_PARISC_PCREL16DF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PCREL16F
#error "elf.h:R_PARISC_PCREL16F macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PCREL16WF
#error "elf.h:R_PARISC_PCREL16WF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PCREL17F
#error "elf.h:R_PARISC_PCREL17F macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PCREL17R
#error "elf.h:R_PARISC_PCREL17R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PCREL21L
#error "elf.h:R_PARISC_PCREL21L macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PCREL22F
#error "elf.h:R_PARISC_PCREL22F macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PCREL32
#error "elf.h:R_PARISC_PCREL32 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PCREL64
#error "elf.h:R_PARISC_PCREL64 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PLABEL14R
#error "elf.h:R_PARISC_PLABEL14R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PLABEL21L
#error "elf.h:R_PARISC_PLABEL21L macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PLABEL32
#error "elf.h:R_PARISC_PLABEL32 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PLTOFF14DR
#error "elf.h:R_PARISC_PLTOFF14DR macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PLTOFF14R
#error "elf.h:R_PARISC_PLTOFF14R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PLTOFF14WR
#error "elf.h:R_PARISC_PLTOFF14WR macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PLTOFF16DF
#error "elf.h:R_PARISC_PLTOFF16DF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PLTOFF16F
#error "elf.h:R_PARISC_PLTOFF16F macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PLTOFF16WF
#error "elf.h:R_PARISC_PLTOFF16WF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_PLTOFF21L
#error "elf.h:R_PARISC_PLTOFF21L macro is missing from libc-shim"
#endif

#ifndef R_PARISC_SECREL32
#error "elf.h:R_PARISC_SECREL32 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_SECREL64
#error "elf.h:R_PARISC_SECREL64 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_SEGBASE
#error "elf.h:R_PARISC_SEGBASE macro is missing from libc-shim"
#endif

#ifndef R_PARISC_SEGREL32
#error "elf.h:R_PARISC_SEGREL32 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_SEGREL64
#error "elf.h:R_PARISC_SEGREL64 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_DTPMOD32
#error "elf.h:R_PARISC_TLS_DTPMOD32 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_DTPMOD64
#error "elf.h:R_PARISC_TLS_DTPMOD64 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_DTPOFF32
#error "elf.h:R_PARISC_TLS_DTPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_DTPOFF64
#error "elf.h:R_PARISC_TLS_DTPOFF64 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_GD14R
#error "elf.h:R_PARISC_TLS_GD14R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_GD21L
#error "elf.h:R_PARISC_TLS_GD21L macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_GDCALL
#error "elf.h:R_PARISC_TLS_GDCALL macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_IE14R
#error "elf.h:R_PARISC_TLS_IE14R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_IE21L
#error "elf.h:R_PARISC_TLS_IE21L macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_LDM14R
#error "elf.h:R_PARISC_TLS_LDM14R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_LDM21L
#error "elf.h:R_PARISC_TLS_LDM21L macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_LDMCALL
#error "elf.h:R_PARISC_TLS_LDMCALL macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_LDO14R
#error "elf.h:R_PARISC_TLS_LDO14R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_LDO21L
#error "elf.h:R_PARISC_TLS_LDO21L macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_LE14R
#error "elf.h:R_PARISC_TLS_LE14R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_LE21L
#error "elf.h:R_PARISC_TLS_LE21L macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_TPREL32
#error "elf.h:R_PARISC_TLS_TPREL32 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TLS_TPREL64
#error "elf.h:R_PARISC_TLS_TPREL64 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TPREL14DR
#error "elf.h:R_PARISC_TPREL14DR macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TPREL14R
#error "elf.h:R_PARISC_TPREL14R macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TPREL14WR
#error "elf.h:R_PARISC_TPREL14WR macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TPREL16DF
#error "elf.h:R_PARISC_TPREL16DF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TPREL16F
#error "elf.h:R_PARISC_TPREL16F macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TPREL16WF
#error "elf.h:R_PARISC_TPREL16WF macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TPREL21L
#error "elf.h:R_PARISC_TPREL21L macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TPREL32
#error "elf.h:R_PARISC_TPREL32 macro is missing from libc-shim"
#endif

#ifndef R_PARISC_TPREL64
#error "elf.h:R_PARISC_TPREL64 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR14
#error "elf.h:R_PPC64_ADDR14 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR14_BRNTAKEN
#error "elf.h:R_PPC64_ADDR14_BRNTAKEN macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR14_BRTAKEN
#error "elf.h:R_PPC64_ADDR14_BRTAKEN macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR16
#error "elf.h:R_PPC64_ADDR16 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR16_DS
#error "elf.h:R_PPC64_ADDR16_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR16_HA
#error "elf.h:R_PPC64_ADDR16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR16_HI
#error "elf.h:R_PPC64_ADDR16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR16_HIGH
#error "elf.h:R_PPC64_ADDR16_HIGH macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR16_HIGHA
#error "elf.h:R_PPC64_ADDR16_HIGHA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR16_HIGHER
#error "elf.h:R_PPC64_ADDR16_HIGHER macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR16_HIGHERA
#error "elf.h:R_PPC64_ADDR16_HIGHERA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR16_HIGHEST
#error "elf.h:R_PPC64_ADDR16_HIGHEST macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR16_HIGHESTA
#error "elf.h:R_PPC64_ADDR16_HIGHESTA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR16_LO
#error "elf.h:R_PPC64_ADDR16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR16_LO_DS
#error "elf.h:R_PPC64_ADDR16_LO_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR24
#error "elf.h:R_PPC64_ADDR24 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR30
#error "elf.h:R_PPC64_ADDR30 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR32
#error "elf.h:R_PPC64_ADDR32 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_ADDR64
#error "elf.h:R_PPC64_ADDR64 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_COPY
#error "elf.h:R_PPC64_COPY macro is missing from libc-shim"
#endif

#ifndef R_PPC64_DTPMOD64
#error "elf.h:R_PPC64_DTPMOD64 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_DTPREL16
#error "elf.h:R_PPC64_DTPREL16 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_DTPREL16_DS
#error "elf.h:R_PPC64_DTPREL16_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_DTPREL16_HA
#error "elf.h:R_PPC64_DTPREL16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_DTPREL16_HI
#error "elf.h:R_PPC64_DTPREL16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC64_DTPREL16_HIGH
#error "elf.h:R_PPC64_DTPREL16_HIGH macro is missing from libc-shim"
#endif

#ifndef R_PPC64_DTPREL16_HIGHA
#error "elf.h:R_PPC64_DTPREL16_HIGHA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_DTPREL16_HIGHER
#error "elf.h:R_PPC64_DTPREL16_HIGHER macro is missing from libc-shim"
#endif

#ifndef R_PPC64_DTPREL16_HIGHERA
#error "elf.h:R_PPC64_DTPREL16_HIGHERA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_DTPREL16_HIGHEST
#error "elf.h:R_PPC64_DTPREL16_HIGHEST macro is missing from libc-shim"
#endif

#ifndef R_PPC64_DTPREL16_HIGHESTA
#error "elf.h:R_PPC64_DTPREL16_HIGHESTA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_DTPREL16_LO
#error "elf.h:R_PPC64_DTPREL16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC64_DTPREL16_LO_DS
#error "elf.h:R_PPC64_DTPREL16_LO_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_DTPREL64
#error "elf.h:R_PPC64_DTPREL64 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GLOB_DAT
#error "elf.h:R_PPC64_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT16
#error "elf.h:R_PPC64_GOT16 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT16_DS
#error "elf.h:R_PPC64_GOT16_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT16_HA
#error "elf.h:R_PPC64_GOT16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT16_HI
#error "elf.h:R_PPC64_GOT16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT16_LO
#error "elf.h:R_PPC64_GOT16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT16_LO_DS
#error "elf.h:R_PPC64_GOT16_LO_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT_DTPREL16_DS
#error "elf.h:R_PPC64_GOT_DTPREL16_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT_DTPREL16_HA
#error "elf.h:R_PPC64_GOT_DTPREL16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT_DTPREL16_HI
#error "elf.h:R_PPC64_GOT_DTPREL16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT_DTPREL16_LO_DS
#error "elf.h:R_PPC64_GOT_DTPREL16_LO_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT_TLSGD16
#error "elf.h:R_PPC64_GOT_TLSGD16 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT_TLSGD16_HA
#error "elf.h:R_PPC64_GOT_TLSGD16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT_TLSGD16_HI
#error "elf.h:R_PPC64_GOT_TLSGD16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT_TLSGD16_LO
#error "elf.h:R_PPC64_GOT_TLSGD16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT_TLSLD16
#error "elf.h:R_PPC64_GOT_TLSLD16 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT_TLSLD16_HA
#error "elf.h:R_PPC64_GOT_TLSLD16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT_TLSLD16_HI
#error "elf.h:R_PPC64_GOT_TLSLD16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT_TLSLD16_LO
#error "elf.h:R_PPC64_GOT_TLSLD16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT_TPREL16_DS
#error "elf.h:R_PPC64_GOT_TPREL16_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT_TPREL16_HA
#error "elf.h:R_PPC64_GOT_TPREL16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT_TPREL16_HI
#error "elf.h:R_PPC64_GOT_TPREL16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC64_GOT_TPREL16_LO_DS
#error "elf.h:R_PPC64_GOT_TPREL16_LO_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_IRELATIVE
#error "elf.h:R_PPC64_IRELATIVE macro is missing from libc-shim"
#endif

#ifndef R_PPC64_JMP_IREL
#error "elf.h:R_PPC64_JMP_IREL macro is missing from libc-shim"
#endif

#ifndef R_PPC64_JMP_SLOT
#error "elf.h:R_PPC64_JMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_PPC64_NONE
#error "elf.h:R_PPC64_NONE macro is missing from libc-shim"
#endif

#ifndef R_PPC64_PLT16_HA
#error "elf.h:R_PPC64_PLT16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_PLT16_HI
#error "elf.h:R_PPC64_PLT16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC64_PLT16_LO
#error "elf.h:R_PPC64_PLT16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC64_PLT16_LO_DS
#error "elf.h:R_PPC64_PLT16_LO_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_PLT32
#error "elf.h:R_PPC64_PLT32 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_PLT64
#error "elf.h:R_PPC64_PLT64 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_PLTGOT16
#error "elf.h:R_PPC64_PLTGOT16 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_PLTGOT16_DS
#error "elf.h:R_PPC64_PLTGOT16_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_PLTGOT16_HA
#error "elf.h:R_PPC64_PLTGOT16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_PLTGOT16_HI
#error "elf.h:R_PPC64_PLTGOT16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC64_PLTGOT16_LO
#error "elf.h:R_PPC64_PLTGOT16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC64_PLTGOT16_LO_DS
#error "elf.h:R_PPC64_PLTGOT16_LO_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_PLTREL32
#error "elf.h:R_PPC64_PLTREL32 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_PLTREL64
#error "elf.h:R_PPC64_PLTREL64 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_REL14
#error "elf.h:R_PPC64_REL14 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_REL14_BRNTAKEN
#error "elf.h:R_PPC64_REL14_BRNTAKEN macro is missing from libc-shim"
#endif

#ifndef R_PPC64_REL14_BRTAKEN
#error "elf.h:R_PPC64_REL14_BRTAKEN macro is missing from libc-shim"
#endif

#ifndef R_PPC64_REL16
#error "elf.h:R_PPC64_REL16 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_REL16_HA
#error "elf.h:R_PPC64_REL16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_REL16_HI
#error "elf.h:R_PPC64_REL16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC64_REL16_LO
#error "elf.h:R_PPC64_REL16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC64_REL24
#error "elf.h:R_PPC64_REL24 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_REL32
#error "elf.h:R_PPC64_REL32 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_REL64
#error "elf.h:R_PPC64_REL64 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_RELATIVE
#error "elf.h:R_PPC64_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_PPC64_SECTOFF
#error "elf.h:R_PPC64_SECTOFF macro is missing from libc-shim"
#endif

#ifndef R_PPC64_SECTOFF_DS
#error "elf.h:R_PPC64_SECTOFF_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_SECTOFF_HA
#error "elf.h:R_PPC64_SECTOFF_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_SECTOFF_HI
#error "elf.h:R_PPC64_SECTOFF_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC64_SECTOFF_LO
#error "elf.h:R_PPC64_SECTOFF_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC64_SECTOFF_LO_DS
#error "elf.h:R_PPC64_SECTOFF_LO_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TLS
#error "elf.h:R_PPC64_TLS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TLSGD
#error "elf.h:R_PPC64_TLSGD macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TLSLD
#error "elf.h:R_PPC64_TLSLD macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TOC
#error "elf.h:R_PPC64_TOC macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TOC16
#error "elf.h:R_PPC64_TOC16 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TOC16_DS
#error "elf.h:R_PPC64_TOC16_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TOC16_HA
#error "elf.h:R_PPC64_TOC16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TOC16_HI
#error "elf.h:R_PPC64_TOC16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TOC16_LO
#error "elf.h:R_PPC64_TOC16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TOC16_LO_DS
#error "elf.h:R_PPC64_TOC16_LO_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TOCSAVE
#error "elf.h:R_PPC64_TOCSAVE macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TPREL16
#error "elf.h:R_PPC64_TPREL16 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TPREL16_DS
#error "elf.h:R_PPC64_TPREL16_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TPREL16_HA
#error "elf.h:R_PPC64_TPREL16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TPREL16_HI
#error "elf.h:R_PPC64_TPREL16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TPREL16_HIGH
#error "elf.h:R_PPC64_TPREL16_HIGH macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TPREL16_HIGHA
#error "elf.h:R_PPC64_TPREL16_HIGHA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TPREL16_HIGHER
#error "elf.h:R_PPC64_TPREL16_HIGHER macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TPREL16_HIGHERA
#error "elf.h:R_PPC64_TPREL16_HIGHERA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TPREL16_HIGHEST
#error "elf.h:R_PPC64_TPREL16_HIGHEST macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TPREL16_HIGHESTA
#error "elf.h:R_PPC64_TPREL16_HIGHESTA macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TPREL16_LO
#error "elf.h:R_PPC64_TPREL16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TPREL16_LO_DS
#error "elf.h:R_PPC64_TPREL16_LO_DS macro is missing from libc-shim"
#endif

#ifndef R_PPC64_TPREL64
#error "elf.h:R_PPC64_TPREL64 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_UADDR16
#error "elf.h:R_PPC64_UADDR16 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_UADDR32
#error "elf.h:R_PPC64_UADDR32 macro is missing from libc-shim"
#endif

#ifndef R_PPC64_UADDR64
#error "elf.h:R_PPC64_UADDR64 macro is missing from libc-shim"
#endif

#ifndef R_PPC_ADDR14
#error "elf.h:R_PPC_ADDR14 macro is missing from libc-shim"
#endif

#ifndef R_PPC_ADDR14_BRNTAKEN
#error "elf.h:R_PPC_ADDR14_BRNTAKEN macro is missing from libc-shim"
#endif

#ifndef R_PPC_ADDR14_BRTAKEN
#error "elf.h:R_PPC_ADDR14_BRTAKEN macro is missing from libc-shim"
#endif

#ifndef R_PPC_ADDR16
#error "elf.h:R_PPC_ADDR16 macro is missing from libc-shim"
#endif

#ifndef R_PPC_ADDR16_HA
#error "elf.h:R_PPC_ADDR16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC_ADDR16_HI
#error "elf.h:R_PPC_ADDR16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC_ADDR16_LO
#error "elf.h:R_PPC_ADDR16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC_ADDR24
#error "elf.h:R_PPC_ADDR24 macro is missing from libc-shim"
#endif

#ifndef R_PPC_ADDR32
#error "elf.h:R_PPC_ADDR32 macro is missing from libc-shim"
#endif

#ifndef R_PPC_COPY
#error "elf.h:R_PPC_COPY macro is missing from libc-shim"
#endif

#ifndef R_PPC_DIAB_RELSDA_HA
#error "elf.h:R_PPC_DIAB_RELSDA_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC_DIAB_RELSDA_HI
#error "elf.h:R_PPC_DIAB_RELSDA_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC_DIAB_RELSDA_LO
#error "elf.h:R_PPC_DIAB_RELSDA_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC_DIAB_SDA21_HA
#error "elf.h:R_PPC_DIAB_SDA21_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC_DIAB_SDA21_HI
#error "elf.h:R_PPC_DIAB_SDA21_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC_DIAB_SDA21_LO
#error "elf.h:R_PPC_DIAB_SDA21_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC_DTPMOD32
#error "elf.h:R_PPC_DTPMOD32 macro is missing from libc-shim"
#endif

#ifndef R_PPC_DTPREL16
#error "elf.h:R_PPC_DTPREL16 macro is missing from libc-shim"
#endif

#ifndef R_PPC_DTPREL16_HA
#error "elf.h:R_PPC_DTPREL16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC_DTPREL16_HI
#error "elf.h:R_PPC_DTPREL16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC_DTPREL16_LO
#error "elf.h:R_PPC_DTPREL16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC_DTPREL32
#error "elf.h:R_PPC_DTPREL32 macro is missing from libc-shim"
#endif

#ifndef R_PPC_EMB_BIT_FLD
#error "elf.h:R_PPC_EMB_BIT_FLD macro is missing from libc-shim"
#endif

#ifndef R_PPC_EMB_MRKREF
#error "elf.h:R_PPC_EMB_MRKREF macro is missing from libc-shim"
#endif

#ifndef R_PPC_EMB_NADDR16
#error "elf.h:R_PPC_EMB_NADDR16 macro is missing from libc-shim"
#endif

#ifndef R_PPC_EMB_NADDR16_HA
#error "elf.h:R_PPC_EMB_NADDR16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC_EMB_NADDR16_HI
#error "elf.h:R_PPC_EMB_NADDR16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC_EMB_NADDR16_LO
#error "elf.h:R_PPC_EMB_NADDR16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC_EMB_NADDR32
#error "elf.h:R_PPC_EMB_NADDR32 macro is missing from libc-shim"
#endif

#ifndef R_PPC_EMB_RELSDA
#error "elf.h:R_PPC_EMB_RELSDA macro is missing from libc-shim"
#endif

#ifndef R_PPC_EMB_RELSEC16
#error "elf.h:R_PPC_EMB_RELSEC16 macro is missing from libc-shim"
#endif

#ifndef R_PPC_EMB_RELST_HA
#error "elf.h:R_PPC_EMB_RELST_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC_EMB_RELST_HI
#error "elf.h:R_PPC_EMB_RELST_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC_EMB_RELST_LO
#error "elf.h:R_PPC_EMB_RELST_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC_EMB_SDA21
#error "elf.h:R_PPC_EMB_SDA21 macro is missing from libc-shim"
#endif

#ifndef R_PPC_EMB_SDA2I16
#error "elf.h:R_PPC_EMB_SDA2I16 macro is missing from libc-shim"
#endif

#ifndef R_PPC_EMB_SDA2REL
#error "elf.h:R_PPC_EMB_SDA2REL macro is missing from libc-shim"
#endif

#ifndef R_PPC_EMB_SDAI16
#error "elf.h:R_PPC_EMB_SDAI16 macro is missing from libc-shim"
#endif

#ifndef R_PPC_GLOB_DAT
#error "elf.h:R_PPC_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT16
#error "elf.h:R_PPC_GOT16 macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT16_HA
#error "elf.h:R_PPC_GOT16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT16_HI
#error "elf.h:R_PPC_GOT16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT16_LO
#error "elf.h:R_PPC_GOT16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT_DTPREL16
#error "elf.h:R_PPC_GOT_DTPREL16 macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT_DTPREL16_HA
#error "elf.h:R_PPC_GOT_DTPREL16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT_DTPREL16_HI
#error "elf.h:R_PPC_GOT_DTPREL16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT_DTPREL16_LO
#error "elf.h:R_PPC_GOT_DTPREL16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT_TLSGD16
#error "elf.h:R_PPC_GOT_TLSGD16 macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT_TLSGD16_HA
#error "elf.h:R_PPC_GOT_TLSGD16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT_TLSGD16_HI
#error "elf.h:R_PPC_GOT_TLSGD16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT_TLSGD16_LO
#error "elf.h:R_PPC_GOT_TLSGD16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT_TLSLD16
#error "elf.h:R_PPC_GOT_TLSLD16 macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT_TLSLD16_HA
#error "elf.h:R_PPC_GOT_TLSLD16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT_TLSLD16_HI
#error "elf.h:R_PPC_GOT_TLSLD16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT_TLSLD16_LO
#error "elf.h:R_PPC_GOT_TLSLD16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT_TPREL16
#error "elf.h:R_PPC_GOT_TPREL16 macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT_TPREL16_HA
#error "elf.h:R_PPC_GOT_TPREL16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT_TPREL16_HI
#error "elf.h:R_PPC_GOT_TPREL16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC_GOT_TPREL16_LO
#error "elf.h:R_PPC_GOT_TPREL16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC_IRELATIVE
#error "elf.h:R_PPC_IRELATIVE macro is missing from libc-shim"
#endif

#ifndef R_PPC_JMP_SLOT
#error "elf.h:R_PPC_JMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_PPC_LOCAL24PC
#error "elf.h:R_PPC_LOCAL24PC macro is missing from libc-shim"
#endif

#ifndef R_PPC_NONE
#error "elf.h:R_PPC_NONE macro is missing from libc-shim"
#endif

#ifndef R_PPC_PLT16_HA
#error "elf.h:R_PPC_PLT16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC_PLT16_HI
#error "elf.h:R_PPC_PLT16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC_PLT16_LO
#error "elf.h:R_PPC_PLT16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC_PLT32
#error "elf.h:R_PPC_PLT32 macro is missing from libc-shim"
#endif

#ifndef R_PPC_PLTREL24
#error "elf.h:R_PPC_PLTREL24 macro is missing from libc-shim"
#endif

#ifndef R_PPC_PLTREL32
#error "elf.h:R_PPC_PLTREL32 macro is missing from libc-shim"
#endif

#ifndef R_PPC_REL14
#error "elf.h:R_PPC_REL14 macro is missing from libc-shim"
#endif

#ifndef R_PPC_REL14_BRNTAKEN
#error "elf.h:R_PPC_REL14_BRNTAKEN macro is missing from libc-shim"
#endif

#ifndef R_PPC_REL14_BRTAKEN
#error "elf.h:R_PPC_REL14_BRTAKEN macro is missing from libc-shim"
#endif

#ifndef R_PPC_REL16
#error "elf.h:R_PPC_REL16 macro is missing from libc-shim"
#endif

#ifndef R_PPC_REL16_HA
#error "elf.h:R_PPC_REL16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC_REL16_HI
#error "elf.h:R_PPC_REL16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC_REL16_LO
#error "elf.h:R_PPC_REL16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC_REL24
#error "elf.h:R_PPC_REL24 macro is missing from libc-shim"
#endif

#ifndef R_PPC_REL32
#error "elf.h:R_PPC_REL32 macro is missing from libc-shim"
#endif

#ifndef R_PPC_RELATIVE
#error "elf.h:R_PPC_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_PPC_SDAREL16
#error "elf.h:R_PPC_SDAREL16 macro is missing from libc-shim"
#endif

#ifndef R_PPC_SECTOFF
#error "elf.h:R_PPC_SECTOFF macro is missing from libc-shim"
#endif

#ifndef R_PPC_SECTOFF_HA
#error "elf.h:R_PPC_SECTOFF_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC_SECTOFF_HI
#error "elf.h:R_PPC_SECTOFF_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC_SECTOFF_LO
#error "elf.h:R_PPC_SECTOFF_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC_TLS
#error "elf.h:R_PPC_TLS macro is missing from libc-shim"
#endif

#ifndef R_PPC_TLSGD
#error "elf.h:R_PPC_TLSGD macro is missing from libc-shim"
#endif

#ifndef R_PPC_TLSLD
#error "elf.h:R_PPC_TLSLD macro is missing from libc-shim"
#endif

#ifndef R_PPC_TOC16
#error "elf.h:R_PPC_TOC16 macro is missing from libc-shim"
#endif

#ifndef R_PPC_TPREL16
#error "elf.h:R_PPC_TPREL16 macro is missing from libc-shim"
#endif

#ifndef R_PPC_TPREL16_HA
#error "elf.h:R_PPC_TPREL16_HA macro is missing from libc-shim"
#endif

#ifndef R_PPC_TPREL16_HI
#error "elf.h:R_PPC_TPREL16_HI macro is missing from libc-shim"
#endif

#ifndef R_PPC_TPREL16_LO
#error "elf.h:R_PPC_TPREL16_LO macro is missing from libc-shim"
#endif

#ifndef R_PPC_TPREL32
#error "elf.h:R_PPC_TPREL32 macro is missing from libc-shim"
#endif

#ifndef R_PPC_UADDR16
#error "elf.h:R_PPC_UADDR16 macro is missing from libc-shim"
#endif

#ifndef R_PPC_UADDR32
#error "elf.h:R_PPC_UADDR32 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_32
#error "elf.h:R_RISCV_32 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_32_PCREL
#error "elf.h:R_RISCV_32_PCREL macro is missing from libc-shim"
#endif

#ifndef R_RISCV_64
#error "elf.h:R_RISCV_64 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_ADD16
#error "elf.h:R_RISCV_ADD16 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_ADD32
#error "elf.h:R_RISCV_ADD32 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_ADD64
#error "elf.h:R_RISCV_ADD64 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_ADD8
#error "elf.h:R_RISCV_ADD8 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_ALIGN
#error "elf.h:R_RISCV_ALIGN macro is missing from libc-shim"
#endif

#ifndef R_RISCV_BRANCH
#error "elf.h:R_RISCV_BRANCH macro is missing from libc-shim"
#endif

#ifndef R_RISCV_CALL
#error "elf.h:R_RISCV_CALL macro is missing from libc-shim"
#endif

#ifndef R_RISCV_CALL_PLT
#error "elf.h:R_RISCV_CALL_PLT macro is missing from libc-shim"
#endif

#ifndef R_RISCV_COPY
#error "elf.h:R_RISCV_COPY macro is missing from libc-shim"
#endif

#ifndef R_RISCV_GOT32_PCREL
#error "elf.h:R_RISCV_GOT32_PCREL macro is missing from libc-shim"
#endif

#ifndef R_RISCV_GOT_HI20
#error "elf.h:R_RISCV_GOT_HI20 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_HI20
#error "elf.h:R_RISCV_HI20 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_IRELATIVE
#error "elf.h:R_RISCV_IRELATIVE macro is missing from libc-shim"
#endif

#ifndef R_RISCV_JAL
#error "elf.h:R_RISCV_JAL macro is missing from libc-shim"
#endif

#ifndef R_RISCV_JUMP_SLOT
#error "elf.h:R_RISCV_JUMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_RISCV_LO12_I
#error "elf.h:R_RISCV_LO12_I macro is missing from libc-shim"
#endif

#ifndef R_RISCV_LO12_S
#error "elf.h:R_RISCV_LO12_S macro is missing from libc-shim"
#endif

#ifndef R_RISCV_NONE
#error "elf.h:R_RISCV_NONE macro is missing from libc-shim"
#endif

#ifndef R_RISCV_NUM
#error "elf.h:R_RISCV_NUM macro is missing from libc-shim"
#endif

#ifndef R_RISCV_PCREL_HI20
#error "elf.h:R_RISCV_PCREL_HI20 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_PCREL_LO12_I
#error "elf.h:R_RISCV_PCREL_LO12_I macro is missing from libc-shim"
#endif

#ifndef R_RISCV_PCREL_LO12_S
#error "elf.h:R_RISCV_PCREL_LO12_S macro is missing from libc-shim"
#endif

#ifndef R_RISCV_PLT32
#error "elf.h:R_RISCV_PLT32 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_RELATIVE
#error "elf.h:R_RISCV_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_RISCV_RELAX
#error "elf.h:R_RISCV_RELAX macro is missing from libc-shim"
#endif

#ifndef R_RISCV_RVC_BRANCH
#error "elf.h:R_RISCV_RVC_BRANCH macro is missing from libc-shim"
#endif

#ifndef R_RISCV_RVC_JUMP
#error "elf.h:R_RISCV_RVC_JUMP macro is missing from libc-shim"
#endif

#ifndef R_RISCV_SET16
#error "elf.h:R_RISCV_SET16 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_SET32
#error "elf.h:R_RISCV_SET32 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_SET6
#error "elf.h:R_RISCV_SET6 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_SET8
#error "elf.h:R_RISCV_SET8 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_SET_ULEB128
#error "elf.h:R_RISCV_SET_ULEB128 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_SUB16
#error "elf.h:R_RISCV_SUB16 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_SUB32
#error "elf.h:R_RISCV_SUB32 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_SUB6
#error "elf.h:R_RISCV_SUB6 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_SUB64
#error "elf.h:R_RISCV_SUB64 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_SUB8
#error "elf.h:R_RISCV_SUB8 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_SUB_ULEB128
#error "elf.h:R_RISCV_SUB_ULEB128 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TLSDESC
#error "elf.h:R_RISCV_TLSDESC macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TLSDESC_ADD_LO12
#error "elf.h:R_RISCV_TLSDESC_ADD_LO12 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TLSDESC_CALL
#error "elf.h:R_RISCV_TLSDESC_CALL macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TLSDESC_HI20
#error "elf.h:R_RISCV_TLSDESC_HI20 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TLSDESC_LOAD_LO12
#error "elf.h:R_RISCV_TLSDESC_LOAD_LO12 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TLS_DTPMOD32
#error "elf.h:R_RISCV_TLS_DTPMOD32 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TLS_DTPMOD64
#error "elf.h:R_RISCV_TLS_DTPMOD64 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TLS_DTPREL32
#error "elf.h:R_RISCV_TLS_DTPREL32 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TLS_DTPREL64
#error "elf.h:R_RISCV_TLS_DTPREL64 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TLS_GD_HI20
#error "elf.h:R_RISCV_TLS_GD_HI20 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TLS_GOT_HI20
#error "elf.h:R_RISCV_TLS_GOT_HI20 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TLS_TPREL32
#error "elf.h:R_RISCV_TLS_TPREL32 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TLS_TPREL64
#error "elf.h:R_RISCV_TLS_TPREL64 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TPREL_ADD
#error "elf.h:R_RISCV_TPREL_ADD macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TPREL_HI20
#error "elf.h:R_RISCV_TPREL_HI20 macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TPREL_LO12_I
#error "elf.h:R_RISCV_TPREL_LO12_I macro is missing from libc-shim"
#endif

#ifndef R_RISCV_TPREL_LO12_S
#error "elf.h:R_RISCV_TPREL_LO12_S macro is missing from libc-shim"
#endif

#ifndef R_SH_ALIGN
#error "elf.h:R_SH_ALIGN macro is missing from libc-shim"
#endif

#ifndef R_SH_CODE
#error "elf.h:R_SH_CODE macro is missing from libc-shim"
#endif

#ifndef R_SH_COPY
#error "elf.h:R_SH_COPY macro is missing from libc-shim"
#endif

#ifndef R_SH_COUNT
#error "elf.h:R_SH_COUNT macro is missing from libc-shim"
#endif

#ifndef R_SH_DATA
#error "elf.h:R_SH_DATA macro is missing from libc-shim"
#endif

#ifndef R_SH_DIR32
#error "elf.h:R_SH_DIR32 macro is missing from libc-shim"
#endif

#ifndef R_SH_DIR8BP
#error "elf.h:R_SH_DIR8BP macro is missing from libc-shim"
#endif

#ifndef R_SH_DIR8L
#error "elf.h:R_SH_DIR8L macro is missing from libc-shim"
#endif

#ifndef R_SH_DIR8W
#error "elf.h:R_SH_DIR8W macro is missing from libc-shim"
#endif

#ifndef R_SH_DIR8WPL
#error "elf.h:R_SH_DIR8WPL macro is missing from libc-shim"
#endif

#ifndef R_SH_DIR8WPN
#error "elf.h:R_SH_DIR8WPN macro is missing from libc-shim"
#endif

#ifndef R_SH_DIR8WPZ
#error "elf.h:R_SH_DIR8WPZ macro is missing from libc-shim"
#endif

#ifndef R_SH_GLOB_DAT
#error "elf.h:R_SH_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_SH_GNU_VTENTRY
#error "elf.h:R_SH_GNU_VTENTRY macro is missing from libc-shim"
#endif

#ifndef R_SH_GNU_VTINHERIT
#error "elf.h:R_SH_GNU_VTINHERIT macro is missing from libc-shim"
#endif

#ifndef R_SH_GOT32
#error "elf.h:R_SH_GOT32 macro is missing from libc-shim"
#endif

#ifndef R_SH_GOTOFF
#error "elf.h:R_SH_GOTOFF macro is missing from libc-shim"
#endif

#ifndef R_SH_GOTPC
#error "elf.h:R_SH_GOTPC macro is missing from libc-shim"
#endif

#ifndef R_SH_IND12W
#error "elf.h:R_SH_IND12W macro is missing from libc-shim"
#endif

#ifndef R_SH_JMP_SLOT
#error "elf.h:R_SH_JMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_SH_LABEL
#error "elf.h:R_SH_LABEL macro is missing from libc-shim"
#endif

#ifndef R_SH_NONE
#error "elf.h:R_SH_NONE macro is missing from libc-shim"
#endif

#ifndef R_SH_NUM
#error "elf.h:R_SH_NUM macro is missing from libc-shim"
#endif

#ifndef R_SH_PLT32
#error "elf.h:R_SH_PLT32 macro is missing from libc-shim"
#endif

#ifndef R_SH_REL32
#error "elf.h:R_SH_REL32 macro is missing from libc-shim"
#endif

#ifndef R_SH_RELATIVE
#error "elf.h:R_SH_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_SH_SWITCH16
#error "elf.h:R_SH_SWITCH16 macro is missing from libc-shim"
#endif

#ifndef R_SH_SWITCH32
#error "elf.h:R_SH_SWITCH32 macro is missing from libc-shim"
#endif

#ifndef R_SH_SWITCH8
#error "elf.h:R_SH_SWITCH8 macro is missing from libc-shim"
#endif

#ifndef R_SH_TLS_DTPMOD32
#error "elf.h:R_SH_TLS_DTPMOD32 macro is missing from libc-shim"
#endif

#ifndef R_SH_TLS_DTPOFF32
#error "elf.h:R_SH_TLS_DTPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_SH_TLS_GD_32
#error "elf.h:R_SH_TLS_GD_32 macro is missing from libc-shim"
#endif

#ifndef R_SH_TLS_IE_32
#error "elf.h:R_SH_TLS_IE_32 macro is missing from libc-shim"
#endif

#ifndef R_SH_TLS_LDO_32
#error "elf.h:R_SH_TLS_LDO_32 macro is missing from libc-shim"
#endif

#ifndef R_SH_TLS_LD_32
#error "elf.h:R_SH_TLS_LD_32 macro is missing from libc-shim"
#endif

#ifndef R_SH_TLS_LE_32
#error "elf.h:R_SH_TLS_LE_32 macro is missing from libc-shim"
#endif

#ifndef R_SH_TLS_TPOFF32
#error "elf.h:R_SH_TLS_TPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_SH_USES
#error "elf.h:R_SH_USES macro is missing from libc-shim"
#endif

#ifndef R_SPARC_10
#error "elf.h:R_SPARC_10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_11
#error "elf.h:R_SPARC_11 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_13
#error "elf.h:R_SPARC_13 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_16
#error "elf.h:R_SPARC_16 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_22
#error "elf.h:R_SPARC_22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_32
#error "elf.h:R_SPARC_32 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_5
#error "elf.h:R_SPARC_5 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_6
#error "elf.h:R_SPARC_6 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_64
#error "elf.h:R_SPARC_64 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_7
#error "elf.h:R_SPARC_7 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_8
#error "elf.h:R_SPARC_8 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_COPY
#error "elf.h:R_SPARC_COPY macro is missing from libc-shim"
#endif

#ifndef R_SPARC_DISP16
#error "elf.h:R_SPARC_DISP16 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_DISP32
#error "elf.h:R_SPARC_DISP32 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_DISP64
#error "elf.h:R_SPARC_DISP64 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_DISP8
#error "elf.h:R_SPARC_DISP8 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_GLOB_DAT
#error "elf.h:R_SPARC_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_SPARC_GLOB_JMP
#error "elf.h:R_SPARC_GLOB_JMP macro is missing from libc-shim"
#endif

#ifndef R_SPARC_GNU_VTENTRY
#error "elf.h:R_SPARC_GNU_VTENTRY macro is missing from libc-shim"
#endif

#ifndef R_SPARC_GNU_VTINHERIT
#error "elf.h:R_SPARC_GNU_VTINHERIT macro is missing from libc-shim"
#endif

#ifndef R_SPARC_GOT10
#error "elf.h:R_SPARC_GOT10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_GOT13
#error "elf.h:R_SPARC_GOT13 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_GOT22
#error "elf.h:R_SPARC_GOT22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_GOTDATA_HIX22
#error "elf.h:R_SPARC_GOTDATA_HIX22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_GOTDATA_LOX10
#error "elf.h:R_SPARC_GOTDATA_LOX10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_GOTDATA_OP
#error "elf.h:R_SPARC_GOTDATA_OP macro is missing from libc-shim"
#endif

#ifndef R_SPARC_GOTDATA_OP_HIX22
#error "elf.h:R_SPARC_GOTDATA_OP_HIX22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_GOTDATA_OP_LOX10
#error "elf.h:R_SPARC_GOTDATA_OP_LOX10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_H34
#error "elf.h:R_SPARC_H34 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_H44
#error "elf.h:R_SPARC_H44 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_HH22
#error "elf.h:R_SPARC_HH22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_HI22
#error "elf.h:R_SPARC_HI22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_HIPLT22
#error "elf.h:R_SPARC_HIPLT22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_HIX22
#error "elf.h:R_SPARC_HIX22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_HM10
#error "elf.h:R_SPARC_HM10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_IRELATIVE
#error "elf.h:R_SPARC_IRELATIVE macro is missing from libc-shim"
#endif

#ifndef R_SPARC_JMP_IREL
#error "elf.h:R_SPARC_JMP_IREL macro is missing from libc-shim"
#endif

#ifndef R_SPARC_JMP_SLOT
#error "elf.h:R_SPARC_JMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_SPARC_L44
#error "elf.h:R_SPARC_L44 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_LM22
#error "elf.h:R_SPARC_LM22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_LO10
#error "elf.h:R_SPARC_LO10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_LOPLT10
#error "elf.h:R_SPARC_LOPLT10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_LOX10
#error "elf.h:R_SPARC_LOX10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_M44
#error "elf.h:R_SPARC_M44 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_NONE
#error "elf.h:R_SPARC_NONE macro is missing from libc-shim"
#endif

#ifndef R_SPARC_NUM
#error "elf.h:R_SPARC_NUM macro is missing from libc-shim"
#endif

#ifndef R_SPARC_OLO10
#error "elf.h:R_SPARC_OLO10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_PC10
#error "elf.h:R_SPARC_PC10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_PC22
#error "elf.h:R_SPARC_PC22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_PCPLT10
#error "elf.h:R_SPARC_PCPLT10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_PCPLT22
#error "elf.h:R_SPARC_PCPLT22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_PCPLT32
#error "elf.h:R_SPARC_PCPLT32 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_PC_HH22
#error "elf.h:R_SPARC_PC_HH22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_PC_HM10
#error "elf.h:R_SPARC_PC_HM10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_PC_LM22
#error "elf.h:R_SPARC_PC_LM22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_PLT32
#error "elf.h:R_SPARC_PLT32 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_PLT64
#error "elf.h:R_SPARC_PLT64 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_REGISTER
#error "elf.h:R_SPARC_REGISTER macro is missing from libc-shim"
#endif

#ifndef R_SPARC_RELATIVE
#error "elf.h:R_SPARC_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_SPARC_REV32
#error "elf.h:R_SPARC_REV32 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_SIZE32
#error "elf.h:R_SPARC_SIZE32 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_SIZE64
#error "elf.h:R_SPARC_SIZE64 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_DTPMOD32
#error "elf.h:R_SPARC_TLS_DTPMOD32 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_DTPMOD64
#error "elf.h:R_SPARC_TLS_DTPMOD64 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_DTPOFF32
#error "elf.h:R_SPARC_TLS_DTPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_DTPOFF64
#error "elf.h:R_SPARC_TLS_DTPOFF64 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_GD_ADD
#error "elf.h:R_SPARC_TLS_GD_ADD macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_GD_CALL
#error "elf.h:R_SPARC_TLS_GD_CALL macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_GD_HI22
#error "elf.h:R_SPARC_TLS_GD_HI22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_GD_LO10
#error "elf.h:R_SPARC_TLS_GD_LO10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_IE_ADD
#error "elf.h:R_SPARC_TLS_IE_ADD macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_IE_HI22
#error "elf.h:R_SPARC_TLS_IE_HI22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_IE_LD
#error "elf.h:R_SPARC_TLS_IE_LD macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_IE_LDX
#error "elf.h:R_SPARC_TLS_IE_LDX macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_IE_LO10
#error "elf.h:R_SPARC_TLS_IE_LO10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_LDM_ADD
#error "elf.h:R_SPARC_TLS_LDM_ADD macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_LDM_CALL
#error "elf.h:R_SPARC_TLS_LDM_CALL macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_LDM_HI22
#error "elf.h:R_SPARC_TLS_LDM_HI22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_LDM_LO10
#error "elf.h:R_SPARC_TLS_LDM_LO10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_LDO_ADD
#error "elf.h:R_SPARC_TLS_LDO_ADD macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_LDO_HIX22
#error "elf.h:R_SPARC_TLS_LDO_HIX22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_LDO_LOX10
#error "elf.h:R_SPARC_TLS_LDO_LOX10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_LE_HIX22
#error "elf.h:R_SPARC_TLS_LE_HIX22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_LE_LOX10
#error "elf.h:R_SPARC_TLS_LE_LOX10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_TPOFF32
#error "elf.h:R_SPARC_TLS_TPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_TLS_TPOFF64
#error "elf.h:R_SPARC_TLS_TPOFF64 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_UA16
#error "elf.h:R_SPARC_UA16 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_UA32
#error "elf.h:R_SPARC_UA32 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_UA64
#error "elf.h:R_SPARC_UA64 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_WDISP10
#error "elf.h:R_SPARC_WDISP10 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_WDISP16
#error "elf.h:R_SPARC_WDISP16 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_WDISP19
#error "elf.h:R_SPARC_WDISP19 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_WDISP22
#error "elf.h:R_SPARC_WDISP22 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_WDISP30
#error "elf.h:R_SPARC_WDISP30 macro is missing from libc-shim"
#endif

#ifndef R_SPARC_WPLT30
#error "elf.h:R_SPARC_WPLT30 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_16
#error "elf.h:R_TILEGX_16 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_16_PCREL
#error "elf.h:R_TILEGX_16_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_32
#error "elf.h:R_TILEGX_32 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_32_PCREL
#error "elf.h:R_TILEGX_32_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_64
#error "elf.h:R_TILEGX_64 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_64_PCREL
#error "elf.h:R_TILEGX_64_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_8
#error "elf.h:R_TILEGX_8 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_8_PCREL
#error "elf.h:R_TILEGX_8_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_BROFF_X1
#error "elf.h:R_TILEGX_BROFF_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_COPY
#error "elf.h:R_TILEGX_COPY macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_DEST_IMM8_X1
#error "elf.h:R_TILEGX_DEST_IMM8_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_GLOB_DAT
#error "elf.h:R_TILEGX_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_GNU_VTENTRY
#error "elf.h:R_TILEGX_GNU_VTENTRY macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_GNU_VTINHERIT
#error "elf.h:R_TILEGX_GNU_VTINHERIT macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_HW0
#error "elf.h:R_TILEGX_HW0 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_HW0_LAST
#error "elf.h:R_TILEGX_HW0_LAST macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_HW1
#error "elf.h:R_TILEGX_HW1 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_HW1_LAST
#error "elf.h:R_TILEGX_HW1_LAST macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_HW2
#error "elf.h:R_TILEGX_HW2 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_HW2_LAST
#error "elf.h:R_TILEGX_HW2_LAST macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_HW3
#error "elf.h:R_TILEGX_HW3 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW0
#error "elf.h:R_TILEGX_IMM16_X0_HW0 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW0_GOT
#error "elf.h:R_TILEGX_IMM16_X0_HW0_GOT macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW0_LAST
#error "elf.h:R_TILEGX_IMM16_X0_HW0_LAST macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW0_LAST_GOT
#error "elf.h:R_TILEGX_IMM16_X0_HW0_LAST_GOT macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW0_LAST_PCREL
#error "elf.h:R_TILEGX_IMM16_X0_HW0_LAST_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW0_LAST_PLT_PCREL
#error "elf.h:R_TILEGX_IMM16_X0_HW0_LAST_PLT_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW0_LAST_TLS_GD
#error "elf.h:R_TILEGX_IMM16_X0_HW0_LAST_TLS_GD macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW0_LAST_TLS_IE
#error "elf.h:R_TILEGX_IMM16_X0_HW0_LAST_TLS_IE macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW0_LAST_TLS_LE
#error "elf.h:R_TILEGX_IMM16_X0_HW0_LAST_TLS_LE macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW0_PCREL
#error "elf.h:R_TILEGX_IMM16_X0_HW0_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW0_PLT_PCREL
#error "elf.h:R_TILEGX_IMM16_X0_HW0_PLT_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW0_TLS_GD
#error "elf.h:R_TILEGX_IMM16_X0_HW0_TLS_GD macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW0_TLS_IE
#error "elf.h:R_TILEGX_IMM16_X0_HW0_TLS_IE macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW0_TLS_LE
#error "elf.h:R_TILEGX_IMM16_X0_HW0_TLS_LE macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW1
#error "elf.h:R_TILEGX_IMM16_X0_HW1 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW1_LAST
#error "elf.h:R_TILEGX_IMM16_X0_HW1_LAST macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW1_LAST_GOT
#error "elf.h:R_TILEGX_IMM16_X0_HW1_LAST_GOT macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW1_LAST_PCREL
#error "elf.h:R_TILEGX_IMM16_X0_HW1_LAST_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW1_LAST_PLT_PCREL
#error "elf.h:R_TILEGX_IMM16_X0_HW1_LAST_PLT_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW1_LAST_TLS_GD
#error "elf.h:R_TILEGX_IMM16_X0_HW1_LAST_TLS_GD macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW1_LAST_TLS_IE
#error "elf.h:R_TILEGX_IMM16_X0_HW1_LAST_TLS_IE macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW1_LAST_TLS_LE
#error "elf.h:R_TILEGX_IMM16_X0_HW1_LAST_TLS_LE macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW1_PCREL
#error "elf.h:R_TILEGX_IMM16_X0_HW1_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW1_PLT_PCREL
#error "elf.h:R_TILEGX_IMM16_X0_HW1_PLT_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW2
#error "elf.h:R_TILEGX_IMM16_X0_HW2 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW2_LAST
#error "elf.h:R_TILEGX_IMM16_X0_HW2_LAST macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW2_LAST_PCREL
#error "elf.h:R_TILEGX_IMM16_X0_HW2_LAST_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW2_LAST_PLT_PCREL
#error "elf.h:R_TILEGX_IMM16_X0_HW2_LAST_PLT_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW2_PCREL
#error "elf.h:R_TILEGX_IMM16_X0_HW2_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW2_PLT_PCREL
#error "elf.h:R_TILEGX_IMM16_X0_HW2_PLT_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW3
#error "elf.h:R_TILEGX_IMM16_X0_HW3 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW3_PCREL
#error "elf.h:R_TILEGX_IMM16_X0_HW3_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X0_HW3_PLT_PCREL
#error "elf.h:R_TILEGX_IMM16_X0_HW3_PLT_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW0
#error "elf.h:R_TILEGX_IMM16_X1_HW0 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW0_GOT
#error "elf.h:R_TILEGX_IMM16_X1_HW0_GOT macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW0_LAST
#error "elf.h:R_TILEGX_IMM16_X1_HW0_LAST macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW0_LAST_GOT
#error "elf.h:R_TILEGX_IMM16_X1_HW0_LAST_GOT macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW0_LAST_PCREL
#error "elf.h:R_TILEGX_IMM16_X1_HW0_LAST_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW0_LAST_PLT_PCREL
#error "elf.h:R_TILEGX_IMM16_X1_HW0_LAST_PLT_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW0_LAST_TLS_GD
#error "elf.h:R_TILEGX_IMM16_X1_HW0_LAST_TLS_GD macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW0_LAST_TLS_IE
#error "elf.h:R_TILEGX_IMM16_X1_HW0_LAST_TLS_IE macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW0_LAST_TLS_LE
#error "elf.h:R_TILEGX_IMM16_X1_HW0_LAST_TLS_LE macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW0_PCREL
#error "elf.h:R_TILEGX_IMM16_X1_HW0_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW0_PLT_PCREL
#error "elf.h:R_TILEGX_IMM16_X1_HW0_PLT_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW0_TLS_GD
#error "elf.h:R_TILEGX_IMM16_X1_HW0_TLS_GD macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW0_TLS_IE
#error "elf.h:R_TILEGX_IMM16_X1_HW0_TLS_IE macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW0_TLS_LE
#error "elf.h:R_TILEGX_IMM16_X1_HW0_TLS_LE macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW1
#error "elf.h:R_TILEGX_IMM16_X1_HW1 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW1_LAST
#error "elf.h:R_TILEGX_IMM16_X1_HW1_LAST macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW1_LAST_GOT
#error "elf.h:R_TILEGX_IMM16_X1_HW1_LAST_GOT macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW1_LAST_PCREL
#error "elf.h:R_TILEGX_IMM16_X1_HW1_LAST_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW1_LAST_PLT_PCREL
#error "elf.h:R_TILEGX_IMM16_X1_HW1_LAST_PLT_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW1_LAST_TLS_GD
#error "elf.h:R_TILEGX_IMM16_X1_HW1_LAST_TLS_GD macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW1_LAST_TLS_IE
#error "elf.h:R_TILEGX_IMM16_X1_HW1_LAST_TLS_IE macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW1_LAST_TLS_LE
#error "elf.h:R_TILEGX_IMM16_X1_HW1_LAST_TLS_LE macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW1_PCREL
#error "elf.h:R_TILEGX_IMM16_X1_HW1_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW1_PLT_PCREL
#error "elf.h:R_TILEGX_IMM16_X1_HW1_PLT_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW2
#error "elf.h:R_TILEGX_IMM16_X1_HW2 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW2_LAST
#error "elf.h:R_TILEGX_IMM16_X1_HW2_LAST macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW2_LAST_PCREL
#error "elf.h:R_TILEGX_IMM16_X1_HW2_LAST_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW2_LAST_PLT_PCREL
#error "elf.h:R_TILEGX_IMM16_X1_HW2_LAST_PLT_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW2_PCREL
#error "elf.h:R_TILEGX_IMM16_X1_HW2_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW2_PLT_PCREL
#error "elf.h:R_TILEGX_IMM16_X1_HW2_PLT_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW3
#error "elf.h:R_TILEGX_IMM16_X1_HW3 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW3_PCREL
#error "elf.h:R_TILEGX_IMM16_X1_HW3_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM16_X1_HW3_PLT_PCREL
#error "elf.h:R_TILEGX_IMM16_X1_HW3_PLT_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM8_X0
#error "elf.h:R_TILEGX_IMM8_X0 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM8_X0_TLS_ADD
#error "elf.h:R_TILEGX_IMM8_X0_TLS_ADD macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM8_X0_TLS_GD_ADD
#error "elf.h:R_TILEGX_IMM8_X0_TLS_GD_ADD macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM8_X1
#error "elf.h:R_TILEGX_IMM8_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM8_X1_TLS_ADD
#error "elf.h:R_TILEGX_IMM8_X1_TLS_ADD macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM8_X1_TLS_GD_ADD
#error "elf.h:R_TILEGX_IMM8_X1_TLS_GD_ADD macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM8_Y0
#error "elf.h:R_TILEGX_IMM8_Y0 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM8_Y0_TLS_ADD
#error "elf.h:R_TILEGX_IMM8_Y0_TLS_ADD macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM8_Y0_TLS_GD_ADD
#error "elf.h:R_TILEGX_IMM8_Y0_TLS_GD_ADD macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM8_Y1
#error "elf.h:R_TILEGX_IMM8_Y1 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM8_Y1_TLS_ADD
#error "elf.h:R_TILEGX_IMM8_Y1_TLS_ADD macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_IMM8_Y1_TLS_GD_ADD
#error "elf.h:R_TILEGX_IMM8_Y1_TLS_GD_ADD macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_JMP_SLOT
#error "elf.h:R_TILEGX_JMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_JUMPOFF_X1
#error "elf.h:R_TILEGX_JUMPOFF_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_JUMPOFF_X1_PLT
#error "elf.h:R_TILEGX_JUMPOFF_X1_PLT macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_MF_IMM14_X1
#error "elf.h:R_TILEGX_MF_IMM14_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_MMEND_X0
#error "elf.h:R_TILEGX_MMEND_X0 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_MMSTART_X0
#error "elf.h:R_TILEGX_MMSTART_X0 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_MT_IMM14_X1
#error "elf.h:R_TILEGX_MT_IMM14_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_NONE
#error "elf.h:R_TILEGX_NONE macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_NUM
#error "elf.h:R_TILEGX_NUM macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_RELATIVE
#error "elf.h:R_TILEGX_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_SHAMT_X0
#error "elf.h:R_TILEGX_SHAMT_X0 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_SHAMT_X1
#error "elf.h:R_TILEGX_SHAMT_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_SHAMT_Y0
#error "elf.h:R_TILEGX_SHAMT_Y0 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_SHAMT_Y1
#error "elf.h:R_TILEGX_SHAMT_Y1 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_TLS_DTPMOD32
#error "elf.h:R_TILEGX_TLS_DTPMOD32 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_TLS_DTPMOD64
#error "elf.h:R_TILEGX_TLS_DTPMOD64 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_TLS_DTPOFF32
#error "elf.h:R_TILEGX_TLS_DTPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_TLS_DTPOFF64
#error "elf.h:R_TILEGX_TLS_DTPOFF64 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_TLS_GD_CALL
#error "elf.h:R_TILEGX_TLS_GD_CALL macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_TLS_IE_LOAD
#error "elf.h:R_TILEGX_TLS_IE_LOAD macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_TLS_TPOFF32
#error "elf.h:R_TILEGX_TLS_TPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_TILEGX_TLS_TPOFF64
#error "elf.h:R_TILEGX_TLS_TPOFF64 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_16
#error "elf.h:R_TILEPRO_16 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_16_PCREL
#error "elf.h:R_TILEPRO_16_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_32
#error "elf.h:R_TILEPRO_32 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_32_PCREL
#error "elf.h:R_TILEPRO_32_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_8
#error "elf.h:R_TILEPRO_8 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_8_PCREL
#error "elf.h:R_TILEPRO_8_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_BROFF_X1
#error "elf.h:R_TILEPRO_BROFF_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_COPY
#error "elf.h:R_TILEPRO_COPY macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_DEST_IMM8_X1
#error "elf.h:R_TILEPRO_DEST_IMM8_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_GLOB_DAT
#error "elf.h:R_TILEPRO_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_GNU_VTENTRY
#error "elf.h:R_TILEPRO_GNU_VTENTRY macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_GNU_VTINHERIT
#error "elf.h:R_TILEPRO_GNU_VTINHERIT macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_HA16
#error "elf.h:R_TILEPRO_HA16 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_HI16
#error "elf.h:R_TILEPRO_HI16 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0
#error "elf.h:R_TILEPRO_IMM16_X0 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_GOT
#error "elf.h:R_TILEPRO_IMM16_X0_GOT macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_GOT_HA
#error "elf.h:R_TILEPRO_IMM16_X0_GOT_HA macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_GOT_HI
#error "elf.h:R_TILEPRO_IMM16_X0_GOT_HI macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_GOT_LO
#error "elf.h:R_TILEPRO_IMM16_X0_GOT_LO macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_HA
#error "elf.h:R_TILEPRO_IMM16_X0_HA macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_HA_PCREL
#error "elf.h:R_TILEPRO_IMM16_X0_HA_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_HI
#error "elf.h:R_TILEPRO_IMM16_X0_HI macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_HI_PCREL
#error "elf.h:R_TILEPRO_IMM16_X0_HI_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_LO
#error "elf.h:R_TILEPRO_IMM16_X0_LO macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_LO_PCREL
#error "elf.h:R_TILEPRO_IMM16_X0_LO_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_PCREL
#error "elf.h:R_TILEPRO_IMM16_X0_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_TLS_GD
#error "elf.h:R_TILEPRO_IMM16_X0_TLS_GD macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_TLS_GD_HA
#error "elf.h:R_TILEPRO_IMM16_X0_TLS_GD_HA macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_TLS_GD_HI
#error "elf.h:R_TILEPRO_IMM16_X0_TLS_GD_HI macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_TLS_GD_LO
#error "elf.h:R_TILEPRO_IMM16_X0_TLS_GD_LO macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_TLS_IE
#error "elf.h:R_TILEPRO_IMM16_X0_TLS_IE macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_TLS_IE_HA
#error "elf.h:R_TILEPRO_IMM16_X0_TLS_IE_HA macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_TLS_IE_HI
#error "elf.h:R_TILEPRO_IMM16_X0_TLS_IE_HI macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_TLS_IE_LO
#error "elf.h:R_TILEPRO_IMM16_X0_TLS_IE_LO macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_TLS_LE
#error "elf.h:R_TILEPRO_IMM16_X0_TLS_LE macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_TLS_LE_HA
#error "elf.h:R_TILEPRO_IMM16_X0_TLS_LE_HA macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_TLS_LE_HI
#error "elf.h:R_TILEPRO_IMM16_X0_TLS_LE_HI macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X0_TLS_LE_LO
#error "elf.h:R_TILEPRO_IMM16_X0_TLS_LE_LO macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1
#error "elf.h:R_TILEPRO_IMM16_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_GOT
#error "elf.h:R_TILEPRO_IMM16_X1_GOT macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_GOT_HA
#error "elf.h:R_TILEPRO_IMM16_X1_GOT_HA macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_GOT_HI
#error "elf.h:R_TILEPRO_IMM16_X1_GOT_HI macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_GOT_LO
#error "elf.h:R_TILEPRO_IMM16_X1_GOT_LO macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_HA
#error "elf.h:R_TILEPRO_IMM16_X1_HA macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_HA_PCREL
#error "elf.h:R_TILEPRO_IMM16_X1_HA_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_HI
#error "elf.h:R_TILEPRO_IMM16_X1_HI macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_HI_PCREL
#error "elf.h:R_TILEPRO_IMM16_X1_HI_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_LO
#error "elf.h:R_TILEPRO_IMM16_X1_LO macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_LO_PCREL
#error "elf.h:R_TILEPRO_IMM16_X1_LO_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_PCREL
#error "elf.h:R_TILEPRO_IMM16_X1_PCREL macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_TLS_GD
#error "elf.h:R_TILEPRO_IMM16_X1_TLS_GD macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_TLS_GD_HA
#error "elf.h:R_TILEPRO_IMM16_X1_TLS_GD_HA macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_TLS_GD_HI
#error "elf.h:R_TILEPRO_IMM16_X1_TLS_GD_HI macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_TLS_GD_LO
#error "elf.h:R_TILEPRO_IMM16_X1_TLS_GD_LO macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_TLS_IE
#error "elf.h:R_TILEPRO_IMM16_X1_TLS_IE macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_TLS_IE_HA
#error "elf.h:R_TILEPRO_IMM16_X1_TLS_IE_HA macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_TLS_IE_HI
#error "elf.h:R_TILEPRO_IMM16_X1_TLS_IE_HI macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_TLS_IE_LO
#error "elf.h:R_TILEPRO_IMM16_X1_TLS_IE_LO macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_TLS_LE
#error "elf.h:R_TILEPRO_IMM16_X1_TLS_LE macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_TLS_LE_HA
#error "elf.h:R_TILEPRO_IMM16_X1_TLS_LE_HA macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_TLS_LE_HI
#error "elf.h:R_TILEPRO_IMM16_X1_TLS_LE_HI macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM16_X1_TLS_LE_LO
#error "elf.h:R_TILEPRO_IMM16_X1_TLS_LE_LO macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM8_X0
#error "elf.h:R_TILEPRO_IMM8_X0 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM8_X0_TLS_GD_ADD
#error "elf.h:R_TILEPRO_IMM8_X0_TLS_GD_ADD macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM8_X1
#error "elf.h:R_TILEPRO_IMM8_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM8_X1_TLS_GD_ADD
#error "elf.h:R_TILEPRO_IMM8_X1_TLS_GD_ADD macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM8_Y0
#error "elf.h:R_TILEPRO_IMM8_Y0 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM8_Y0_TLS_GD_ADD
#error "elf.h:R_TILEPRO_IMM8_Y0_TLS_GD_ADD macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM8_Y1
#error "elf.h:R_TILEPRO_IMM8_Y1 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_IMM8_Y1_TLS_GD_ADD
#error "elf.h:R_TILEPRO_IMM8_Y1_TLS_GD_ADD macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_JMP_SLOT
#error "elf.h:R_TILEPRO_JMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_JOFFLONG_X1
#error "elf.h:R_TILEPRO_JOFFLONG_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_JOFFLONG_X1_PLT
#error "elf.h:R_TILEPRO_JOFFLONG_X1_PLT macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_LO16
#error "elf.h:R_TILEPRO_LO16 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_MF_IMM15_X1
#error "elf.h:R_TILEPRO_MF_IMM15_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_MMEND_X0
#error "elf.h:R_TILEPRO_MMEND_X0 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_MMEND_X1
#error "elf.h:R_TILEPRO_MMEND_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_MMSTART_X0
#error "elf.h:R_TILEPRO_MMSTART_X0 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_MMSTART_X1
#error "elf.h:R_TILEPRO_MMSTART_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_MT_IMM15_X1
#error "elf.h:R_TILEPRO_MT_IMM15_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_NONE
#error "elf.h:R_TILEPRO_NONE macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_NUM
#error "elf.h:R_TILEPRO_NUM macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_RELATIVE
#error "elf.h:R_TILEPRO_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_SHAMT_X0
#error "elf.h:R_TILEPRO_SHAMT_X0 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_SHAMT_X1
#error "elf.h:R_TILEPRO_SHAMT_X1 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_SHAMT_Y0
#error "elf.h:R_TILEPRO_SHAMT_Y0 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_SHAMT_Y1
#error "elf.h:R_TILEPRO_SHAMT_Y1 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_TLS_DTPMOD32
#error "elf.h:R_TILEPRO_TLS_DTPMOD32 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_TLS_DTPOFF32
#error "elf.h:R_TILEPRO_TLS_DTPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_TLS_GD_CALL
#error "elf.h:R_TILEPRO_TLS_GD_CALL macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_TLS_IE_LOAD
#error "elf.h:R_TILEPRO_TLS_IE_LOAD macro is missing from libc-shim"
#endif

#ifndef R_TILEPRO_TLS_TPOFF32
#error "elf.h:R_TILEPRO_TLS_TPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_16
#error "elf.h:R_X86_64_16 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_32
#error "elf.h:R_X86_64_32 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_32S
#error "elf.h:R_X86_64_32S macro is missing from libc-shim"
#endif

#ifndef R_X86_64_64
#error "elf.h:R_X86_64_64 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_8
#error "elf.h:R_X86_64_8 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_COPY
#error "elf.h:R_X86_64_COPY macro is missing from libc-shim"
#endif

#ifndef R_X86_64_DTPMOD64
#error "elf.h:R_X86_64_DTPMOD64 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_DTPOFF32
#error "elf.h:R_X86_64_DTPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_DTPOFF64
#error "elf.h:R_X86_64_DTPOFF64 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_GLOB_DAT
#error "elf.h:R_X86_64_GLOB_DAT macro is missing from libc-shim"
#endif

#ifndef R_X86_64_GOT32
#error "elf.h:R_X86_64_GOT32 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_GOT64
#error "elf.h:R_X86_64_GOT64 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_GOTOFF64
#error "elf.h:R_X86_64_GOTOFF64 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_GOTPC32
#error "elf.h:R_X86_64_GOTPC32 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_GOTPC32_TLSDESC
#error "elf.h:R_X86_64_GOTPC32_TLSDESC macro is missing from libc-shim"
#endif

#ifndef R_X86_64_GOTPC64
#error "elf.h:R_X86_64_GOTPC64 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_GOTPCREL
#error "elf.h:R_X86_64_GOTPCREL macro is missing from libc-shim"
#endif

#ifndef R_X86_64_GOTPCREL64
#error "elf.h:R_X86_64_GOTPCREL64 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_GOTPCRELX
#error "elf.h:R_X86_64_GOTPCRELX macro is missing from libc-shim"
#endif

#ifndef R_X86_64_GOTPLT64
#error "elf.h:R_X86_64_GOTPLT64 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_GOTTPOFF
#error "elf.h:R_X86_64_GOTTPOFF macro is missing from libc-shim"
#endif

#ifndef R_X86_64_IRELATIVE
#error "elf.h:R_X86_64_IRELATIVE macro is missing from libc-shim"
#endif

#ifndef R_X86_64_JUMP_SLOT
#error "elf.h:R_X86_64_JUMP_SLOT macro is missing from libc-shim"
#endif

#ifndef R_X86_64_NONE
#error "elf.h:R_X86_64_NONE macro is missing from libc-shim"
#endif

#ifndef R_X86_64_NUM
#error "elf.h:R_X86_64_NUM macro is missing from libc-shim"
#endif

#ifndef R_X86_64_PC16
#error "elf.h:R_X86_64_PC16 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_PC32
#error "elf.h:R_X86_64_PC32 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_PC64
#error "elf.h:R_X86_64_PC64 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_PC8
#error "elf.h:R_X86_64_PC8 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_PLT32
#error "elf.h:R_X86_64_PLT32 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_PLTOFF64
#error "elf.h:R_X86_64_PLTOFF64 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_RELATIVE
#error "elf.h:R_X86_64_RELATIVE macro is missing from libc-shim"
#endif

#ifndef R_X86_64_RELATIVE64
#error "elf.h:R_X86_64_RELATIVE64 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_REX_GOTPCRELX
#error "elf.h:R_X86_64_REX_GOTPCRELX macro is missing from libc-shim"
#endif

#ifndef R_X86_64_SIZE32
#error "elf.h:R_X86_64_SIZE32 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_SIZE64
#error "elf.h:R_X86_64_SIZE64 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_TLSDESC
#error "elf.h:R_X86_64_TLSDESC macro is missing from libc-shim"
#endif

#ifndef R_X86_64_TLSDESC_CALL
#error "elf.h:R_X86_64_TLSDESC_CALL macro is missing from libc-shim"
#endif

#ifndef R_X86_64_TLSGD
#error "elf.h:R_X86_64_TLSGD macro is missing from libc-shim"
#endif

#ifndef R_X86_64_TLSLD
#error "elf.h:R_X86_64_TLSLD macro is missing from libc-shim"
#endif

#ifndef R_X86_64_TPOFF32
#error "elf.h:R_X86_64_TPOFF32 macro is missing from libc-shim"
#endif

#ifndef R_X86_64_TPOFF64
#error "elf.h:R_X86_64_TPOFF64 macro is missing from libc-shim"
#endif

#ifndef SELFMAG
#error "elf.h:SELFMAG macro is missing from libc-shim"
#endif

#ifndef SHF_ALLOC
#error "elf.h:SHF_ALLOC macro is missing from libc-shim"
#endif

#ifndef SHF_ALPHA_GPREL
#error "elf.h:SHF_ALPHA_GPREL macro is missing from libc-shim"
#endif

#ifndef SHF_ARM_COMDEF
#error "elf.h:SHF_ARM_COMDEF macro is missing from libc-shim"
#endif

#ifndef SHF_ARM_ENTRYSECT
#error "elf.h:SHF_ARM_ENTRYSECT macro is missing from libc-shim"
#endif

#ifndef SHF_COMPRESSED
#error "elf.h:SHF_COMPRESSED macro is missing from libc-shim"
#endif

#ifndef SHF_EXCLUDE
#error "elf.h:SHF_EXCLUDE macro is missing from libc-shim"
#endif

#ifndef SHF_EXECINSTR
#error "elf.h:SHF_EXECINSTR macro is missing from libc-shim"
#endif

#ifndef SHF_GNU_RETAIN
#error "elf.h:SHF_GNU_RETAIN macro is missing from libc-shim"
#endif

#ifndef SHF_GROUP
#error "elf.h:SHF_GROUP macro is missing from libc-shim"
#endif

#ifndef SHF_IA_64_NORECOV
#error "elf.h:SHF_IA_64_NORECOV macro is missing from libc-shim"
#endif

#ifndef SHF_IA_64_SHORT
#error "elf.h:SHF_IA_64_SHORT macro is missing from libc-shim"
#endif

#ifndef SHF_INFO_LINK
#error "elf.h:SHF_INFO_LINK macro is missing from libc-shim"
#endif

#ifndef SHF_LINK_ORDER
#error "elf.h:SHF_LINK_ORDER macro is missing from libc-shim"
#endif

#ifndef SHF_MASKOS
#error "elf.h:SHF_MASKOS macro is missing from libc-shim"
#endif

#ifndef SHF_MASKPROC
#error "elf.h:SHF_MASKPROC macro is missing from libc-shim"
#endif

#ifndef SHF_MERGE
#error "elf.h:SHF_MERGE macro is missing from libc-shim"
#endif

#ifndef SHF_MIPS_ADDR
#error "elf.h:SHF_MIPS_ADDR macro is missing from libc-shim"
#endif

#ifndef SHF_MIPS_GPREL
#error "elf.h:SHF_MIPS_GPREL macro is missing from libc-shim"
#endif

#ifndef SHF_MIPS_LOCAL
#error "elf.h:SHF_MIPS_LOCAL macro is missing from libc-shim"
#endif

#ifndef SHF_MIPS_MERGE
#error "elf.h:SHF_MIPS_MERGE macro is missing from libc-shim"
#endif

#ifndef SHF_MIPS_NAMES
#error "elf.h:SHF_MIPS_NAMES macro is missing from libc-shim"
#endif

#ifndef SHF_MIPS_NODUPE
#error "elf.h:SHF_MIPS_NODUPE macro is missing from libc-shim"
#endif

#ifndef SHF_MIPS_NOSTRIP
#error "elf.h:SHF_MIPS_NOSTRIP macro is missing from libc-shim"
#endif

#ifndef SHF_MIPS_STRINGS
#error "elf.h:SHF_MIPS_STRINGS macro is missing from libc-shim"
#endif

#ifndef SHF_ORDERED
#error "elf.h:SHF_ORDERED macro is missing from libc-shim"
#endif

#ifndef SHF_OS_NONCONFORMING
#error "elf.h:SHF_OS_NONCONFORMING macro is missing from libc-shim"
#endif

#ifndef SHF_PARISC_HUGE
#error "elf.h:SHF_PARISC_HUGE macro is missing from libc-shim"
#endif

#ifndef SHF_PARISC_SBP
#error "elf.h:SHF_PARISC_SBP macro is missing from libc-shim"
#endif

#ifndef SHF_PARISC_SHORT
#error "elf.h:SHF_PARISC_SHORT macro is missing from libc-shim"
#endif

#ifndef SHF_STRINGS
#error "elf.h:SHF_STRINGS macro is missing from libc-shim"
#endif

#ifndef SHF_TLS
#error "elf.h:SHF_TLS macro is missing from libc-shim"
#endif

#ifndef SHF_WRITE
#error "elf.h:SHF_WRITE macro is missing from libc-shim"
#endif

#ifndef SHN_ABS
#error "elf.h:SHN_ABS macro is missing from libc-shim"
#endif

#ifndef SHN_AFTER
#error "elf.h:SHN_AFTER macro is missing from libc-shim"
#endif

#ifndef SHN_BEFORE
#error "elf.h:SHN_BEFORE macro is missing from libc-shim"
#endif

#ifndef SHN_COMMON
#error "elf.h:SHN_COMMON macro is missing from libc-shim"
#endif

#ifndef SHN_HIOS
#error "elf.h:SHN_HIOS macro is missing from libc-shim"
#endif

#ifndef SHN_HIPROC
#error "elf.h:SHN_HIPROC macro is missing from libc-shim"
#endif

#ifndef SHN_HIRESERVE
#error "elf.h:SHN_HIRESERVE macro is missing from libc-shim"
#endif

#ifndef SHN_LOOS
#error "elf.h:SHN_LOOS macro is missing from libc-shim"
#endif

#ifndef SHN_LOPROC
#error "elf.h:SHN_LOPROC macro is missing from libc-shim"
#endif

#ifndef SHN_LORESERVE
#error "elf.h:SHN_LORESERVE macro is missing from libc-shim"
#endif

#ifndef SHN_MIPS_ACOMMON
#error "elf.h:SHN_MIPS_ACOMMON macro is missing from libc-shim"
#endif

#ifndef SHN_MIPS_DATA
#error "elf.h:SHN_MIPS_DATA macro is missing from libc-shim"
#endif

#ifndef SHN_MIPS_SCOMMON
#error "elf.h:SHN_MIPS_SCOMMON macro is missing from libc-shim"
#endif

#ifndef SHN_MIPS_SUNDEFINED
#error "elf.h:SHN_MIPS_SUNDEFINED macro is missing from libc-shim"
#endif

#ifndef SHN_MIPS_TEXT
#error "elf.h:SHN_MIPS_TEXT macro is missing from libc-shim"
#endif

#ifndef SHN_PARISC_ANSI_COMMON
#error "elf.h:SHN_PARISC_ANSI_COMMON macro is missing from libc-shim"
#endif

#ifndef SHN_PARISC_HUGE_COMMON
#error "elf.h:SHN_PARISC_HUGE_COMMON macro is missing from libc-shim"
#endif

#ifndef SHN_UNDEF
#error "elf.h:SHN_UNDEF macro is missing from libc-shim"
#endif

#ifndef SHN_XINDEX
#error "elf.h:SHN_XINDEX macro is missing from libc-shim"
#endif

#ifndef SHT_ALPHA_DEBUG
#error "elf.h:SHT_ALPHA_DEBUG macro is missing from libc-shim"
#endif

#ifndef SHT_ALPHA_REGINFO
#error "elf.h:SHT_ALPHA_REGINFO macro is missing from libc-shim"
#endif

#ifndef SHT_ARC_ATTRIBUTES
#error "elf.h:SHT_ARC_ATTRIBUTES macro is missing from libc-shim"
#endif

#ifndef SHT_ARM_ATTRIBUTES
#error "elf.h:SHT_ARM_ATTRIBUTES macro is missing from libc-shim"
#endif

#ifndef SHT_ARM_EXIDX
#error "elf.h:SHT_ARM_EXIDX macro is missing from libc-shim"
#endif

#ifndef SHT_ARM_PREEMPTMAP
#error "elf.h:SHT_ARM_PREEMPTMAP macro is missing from libc-shim"
#endif

#ifndef SHT_CHECKSUM
#error "elf.h:SHT_CHECKSUM macro is missing from libc-shim"
#endif

#ifndef SHT_CSKY_ATTRIBUTES
#error "elf.h:SHT_CSKY_ATTRIBUTES macro is missing from libc-shim"
#endif

#ifndef SHT_DYNAMIC
#error "elf.h:SHT_DYNAMIC macro is missing from libc-shim"
#endif

#ifndef SHT_DYNSYM
#error "elf.h:SHT_DYNSYM macro is missing from libc-shim"
#endif

#ifndef SHT_FINI_ARRAY
#error "elf.h:SHT_FINI_ARRAY macro is missing from libc-shim"
#endif

#ifndef SHT_GNU_ATTRIBUTES
#error "elf.h:SHT_GNU_ATTRIBUTES macro is missing from libc-shim"
#endif

#ifndef SHT_GNU_HASH
#error "elf.h:SHT_GNU_HASH macro is missing from libc-shim"
#endif

#ifndef SHT_GNU_LIBLIST
#error "elf.h:SHT_GNU_LIBLIST macro is missing from libc-shim"
#endif

#ifndef SHT_GNU_verdef
#error "elf.h:SHT_GNU_verdef macro is missing from libc-shim"
#endif

#ifndef SHT_GNU_verneed
#error "elf.h:SHT_GNU_verneed macro is missing from libc-shim"
#endif

#ifndef SHT_GNU_versym
#error "elf.h:SHT_GNU_versym macro is missing from libc-shim"
#endif

#ifndef SHT_GROUP
#error "elf.h:SHT_GROUP macro is missing from libc-shim"
#endif

#ifndef SHT_HASH
#error "elf.h:SHT_HASH macro is missing from libc-shim"
#endif

#ifndef SHT_HIOS
#error "elf.h:SHT_HIOS macro is missing from libc-shim"
#endif

#ifndef SHT_HIPROC
#error "elf.h:SHT_HIPROC macro is missing from libc-shim"
#endif

#ifndef SHT_HISUNW
#error "elf.h:SHT_HISUNW macro is missing from libc-shim"
#endif

#ifndef SHT_HIUSER
#error "elf.h:SHT_HIUSER macro is missing from libc-shim"
#endif

#ifndef SHT_IA_64_EXT
#error "elf.h:SHT_IA_64_EXT macro is missing from libc-shim"
#endif

#ifndef SHT_IA_64_UNWIND
#error "elf.h:SHT_IA_64_UNWIND macro is missing from libc-shim"
#endif

#ifndef SHT_INIT_ARRAY
#error "elf.h:SHT_INIT_ARRAY macro is missing from libc-shim"
#endif

#ifndef SHT_LOOS
#error "elf.h:SHT_LOOS macro is missing from libc-shim"
#endif

#ifndef SHT_LOPROC
#error "elf.h:SHT_LOPROC macro is missing from libc-shim"
#endif

#ifndef SHT_LOSUNW
#error "elf.h:SHT_LOSUNW macro is missing from libc-shim"
#endif

#ifndef SHT_LOUSER
#error "elf.h:SHT_LOUSER macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_ABIFLAGS
#error "elf.h:SHT_MIPS_ABIFLAGS macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_AUXSYM
#error "elf.h:SHT_MIPS_AUXSYM macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_CONFLICT
#error "elf.h:SHT_MIPS_CONFLICT macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_CONTENT
#error "elf.h:SHT_MIPS_CONTENT macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_DEBUG
#error "elf.h:SHT_MIPS_DEBUG macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_DELTACLASS
#error "elf.h:SHT_MIPS_DELTACLASS macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_DELTADECL
#error "elf.h:SHT_MIPS_DELTADECL macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_DELTAINST
#error "elf.h:SHT_MIPS_DELTAINST macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_DELTASYM
#error "elf.h:SHT_MIPS_DELTASYM macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_DENSE
#error "elf.h:SHT_MIPS_DENSE macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_DWARF
#error "elf.h:SHT_MIPS_DWARF macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_EH_REGION
#error "elf.h:SHT_MIPS_EH_REGION macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_EVENTS
#error "elf.h:SHT_MIPS_EVENTS macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_EXTSYM
#error "elf.h:SHT_MIPS_EXTSYM macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_FDESC
#error "elf.h:SHT_MIPS_FDESC macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_GPTAB
#error "elf.h:SHT_MIPS_GPTAB macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_IFACE
#error "elf.h:SHT_MIPS_IFACE macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_LIBLIST
#error "elf.h:SHT_MIPS_LIBLIST macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_LINE
#error "elf.h:SHT_MIPS_LINE macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_LOCSTR
#error "elf.h:SHT_MIPS_LOCSTR macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_LOCSYM
#error "elf.h:SHT_MIPS_LOCSYM macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_MSYM
#error "elf.h:SHT_MIPS_MSYM macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_OPTIONS
#error "elf.h:SHT_MIPS_OPTIONS macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_OPTSYM
#error "elf.h:SHT_MIPS_OPTSYM macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_PACKAGE
#error "elf.h:SHT_MIPS_PACKAGE macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_PACKSYM
#error "elf.h:SHT_MIPS_PACKSYM macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_PDESC
#error "elf.h:SHT_MIPS_PDESC macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_PDR_EXCEPTION
#error "elf.h:SHT_MIPS_PDR_EXCEPTION macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_PIXIE
#error "elf.h:SHT_MIPS_PIXIE macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_REGINFO
#error "elf.h:SHT_MIPS_REGINFO macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_RELD
#error "elf.h:SHT_MIPS_RELD macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_RFDESC
#error "elf.h:SHT_MIPS_RFDESC macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_SHDR
#error "elf.h:SHT_MIPS_SHDR macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_SYMBOL_LIB
#error "elf.h:SHT_MIPS_SYMBOL_LIB macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_TRANSLATE
#error "elf.h:SHT_MIPS_TRANSLATE macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_UCODE
#error "elf.h:SHT_MIPS_UCODE macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_WHIRL
#error "elf.h:SHT_MIPS_WHIRL macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_XHASH
#error "elf.h:SHT_MIPS_XHASH macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_XLATE
#error "elf.h:SHT_MIPS_XLATE macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_XLATE_DEBUG
#error "elf.h:SHT_MIPS_XLATE_DEBUG macro is missing from libc-shim"
#endif

#ifndef SHT_MIPS_XLATE_OLD
#error "elf.h:SHT_MIPS_XLATE_OLD macro is missing from libc-shim"
#endif

#ifndef SHT_NOBITS
#error "elf.h:SHT_NOBITS macro is missing from libc-shim"
#endif

#ifndef SHT_NOTE
#error "elf.h:SHT_NOTE macro is missing from libc-shim"
#endif

#ifndef SHT_NULL
#error "elf.h:SHT_NULL macro is missing from libc-shim"
#endif

#ifndef SHT_NUM
#error "elf.h:SHT_NUM macro is missing from libc-shim"
#endif

#ifndef SHT_PARISC_DOC
#error "elf.h:SHT_PARISC_DOC macro is missing from libc-shim"
#endif

#ifndef SHT_PARISC_EXT
#error "elf.h:SHT_PARISC_EXT macro is missing from libc-shim"
#endif

#ifndef SHT_PARISC_UNWIND
#error "elf.h:SHT_PARISC_UNWIND macro is missing from libc-shim"
#endif

#ifndef SHT_PREINIT_ARRAY
#error "elf.h:SHT_PREINIT_ARRAY macro is missing from libc-shim"
#endif

#ifndef SHT_PROGBITS
#error "elf.h:SHT_PROGBITS macro is missing from libc-shim"
#endif

#ifndef SHT_REL
#error "elf.h:SHT_REL macro is missing from libc-shim"
#endif

#ifndef SHT_RELA
#error "elf.h:SHT_RELA macro is missing from libc-shim"
#endif

#ifndef SHT_RELR
#error "elf.h:SHT_RELR macro is missing from libc-shim"
#endif

#ifndef SHT_RISCV_ATTRIBUTES
#error "elf.h:SHT_RISCV_ATTRIBUTES macro is missing from libc-shim"
#endif

#ifndef SHT_SHLIB
#error "elf.h:SHT_SHLIB macro is missing from libc-shim"
#endif

#ifndef SHT_STRTAB
#error "elf.h:SHT_STRTAB macro is missing from libc-shim"
#endif

#ifndef SHT_SUNW_COMDAT
#error "elf.h:SHT_SUNW_COMDAT macro is missing from libc-shim"
#endif

#ifndef SHT_SUNW_move
#error "elf.h:SHT_SUNW_move macro is missing from libc-shim"
#endif

#ifndef SHT_SUNW_syminfo
#error "elf.h:SHT_SUNW_syminfo macro is missing from libc-shim"
#endif

#ifndef SHT_SYMTAB
#error "elf.h:SHT_SYMTAB macro is missing from libc-shim"
#endif

#ifndef SHT_SYMTAB_SHNDX
#error "elf.h:SHT_SYMTAB_SHNDX macro is missing from libc-shim"
#endif

#ifndef SHT_X86_64_UNWIND
#error "elf.h:SHT_X86_64_UNWIND macro is missing from libc-shim"
#endif

#ifndef STB_GLOBAL
#error "elf.h:STB_GLOBAL macro is missing from libc-shim"
#endif

#ifndef STB_GNU_UNIQUE
#error "elf.h:STB_GNU_UNIQUE macro is missing from libc-shim"
#endif

#ifndef STB_HIOS
#error "elf.h:STB_HIOS macro is missing from libc-shim"
#endif

#ifndef STB_HIPROC
#error "elf.h:STB_HIPROC macro is missing from libc-shim"
#endif

#ifndef STB_LOCAL
#error "elf.h:STB_LOCAL macro is missing from libc-shim"
#endif

#ifndef STB_LOOS
#error "elf.h:STB_LOOS macro is missing from libc-shim"
#endif

#ifndef STB_LOPROC
#error "elf.h:STB_LOPROC macro is missing from libc-shim"
#endif

#ifndef STB_MIPS_SPLIT_COMMON
#error "elf.h:STB_MIPS_SPLIT_COMMON macro is missing from libc-shim"
#endif

#ifndef STB_NUM
#error "elf.h:STB_NUM macro is missing from libc-shim"
#endif

#ifndef STB_WEAK
#error "elf.h:STB_WEAK macro is missing from libc-shim"
#endif

#ifndef STN_UNDEF
#error "elf.h:STN_UNDEF macro is missing from libc-shim"
#endif

#ifndef STO_AARCH64_VARIANT_PCS
#error "elf.h:STO_AARCH64_VARIANT_PCS macro is missing from libc-shim"
#endif

#ifndef STO_ALPHA_NOPV
#error "elf.h:STO_ALPHA_NOPV macro is missing from libc-shim"
#endif

#ifndef STO_ALPHA_STD_GPLOAD
#error "elf.h:STO_ALPHA_STD_GPLOAD macro is missing from libc-shim"
#endif

#ifndef STO_MIPS_DEFAULT
#error "elf.h:STO_MIPS_DEFAULT macro is missing from libc-shim"
#endif

#ifndef STO_MIPS_HIDDEN
#error "elf.h:STO_MIPS_HIDDEN macro is missing from libc-shim"
#endif

#ifndef STO_MIPS_INTERNAL
#error "elf.h:STO_MIPS_INTERNAL macro is missing from libc-shim"
#endif

#ifndef STO_MIPS_PLT
#error "elf.h:STO_MIPS_PLT macro is missing from libc-shim"
#endif

#ifndef STO_MIPS_PROTECTED
#error "elf.h:STO_MIPS_PROTECTED macro is missing from libc-shim"
#endif

#ifndef STO_MIPS_SC_ALIGN_UNUSED
#error "elf.h:STO_MIPS_SC_ALIGN_UNUSED macro is missing from libc-shim"
#endif

#ifndef STO_PPC64_LOCAL_BIT
#error "elf.h:STO_PPC64_LOCAL_BIT macro is missing from libc-shim"
#endif

#ifndef STO_PPC64_LOCAL_MASK
#error "elf.h:STO_PPC64_LOCAL_MASK macro is missing from libc-shim"
#endif

#ifndef STO_RISCV_VARIANT_CC
#error "elf.h:STO_RISCV_VARIANT_CC macro is missing from libc-shim"
#endif

#ifndef STT_ARM_16BIT
#error "elf.h:STT_ARM_16BIT macro is missing from libc-shim"
#endif

#ifndef STT_ARM_TFUNC
#error "elf.h:STT_ARM_TFUNC macro is missing from libc-shim"
#endif

#ifndef STT_COMMON
#error "elf.h:STT_COMMON macro is missing from libc-shim"
#endif

#ifndef STT_FILE
#error "elf.h:STT_FILE macro is missing from libc-shim"
#endif

#ifndef STT_FUNC
#error "elf.h:STT_FUNC macro is missing from libc-shim"
#endif

#ifndef STT_GNU_IFUNC
#error "elf.h:STT_GNU_IFUNC macro is missing from libc-shim"
#endif

#ifndef STT_HIOS
#error "elf.h:STT_HIOS macro is missing from libc-shim"
#endif

#ifndef STT_HIPROC
#error "elf.h:STT_HIPROC macro is missing from libc-shim"
#endif

#ifndef STT_HP_OPAQUE
#error "elf.h:STT_HP_OPAQUE macro is missing from libc-shim"
#endif

#ifndef STT_HP_STUB
#error "elf.h:STT_HP_STUB macro is missing from libc-shim"
#endif

#ifndef STT_LOOS
#error "elf.h:STT_LOOS macro is missing from libc-shim"
#endif

#ifndef STT_LOPROC
#error "elf.h:STT_LOPROC macro is missing from libc-shim"
#endif

#ifndef STT_NOTYPE
#error "elf.h:STT_NOTYPE macro is missing from libc-shim"
#endif

#ifndef STT_NUM
#error "elf.h:STT_NUM macro is missing from libc-shim"
#endif

#ifndef STT_OBJECT
#error "elf.h:STT_OBJECT macro is missing from libc-shim"
#endif

#ifndef STT_PARISC_MILLICODE
#error "elf.h:STT_PARISC_MILLICODE macro is missing from libc-shim"
#endif

#ifndef STT_SECTION
#error "elf.h:STT_SECTION macro is missing from libc-shim"
#endif

#ifndef STT_SPARC_REGISTER
#error "elf.h:STT_SPARC_REGISTER macro is missing from libc-shim"
#endif

#ifndef STT_TLS
#error "elf.h:STT_TLS macro is missing from libc-shim"
#endif

#ifndef STV_DEFAULT
#error "elf.h:STV_DEFAULT macro is missing from libc-shim"
#endif

#ifndef STV_HIDDEN
#error "elf.h:STV_HIDDEN macro is missing from libc-shim"
#endif

#ifndef STV_INTERNAL
#error "elf.h:STV_INTERNAL macro is missing from libc-shim"
#endif

#ifndef STV_PROTECTED
#error "elf.h:STV_PROTECTED macro is missing from libc-shim"
#endif

#ifndef SYMINFO_BT_LOWRESERVE
#error "elf.h:SYMINFO_BT_LOWRESERVE macro is missing from libc-shim"
#endif

#ifndef SYMINFO_BT_PARENT
#error "elf.h:SYMINFO_BT_PARENT macro is missing from libc-shim"
#endif

#ifndef SYMINFO_BT_SELF
#error "elf.h:SYMINFO_BT_SELF macro is missing from libc-shim"
#endif

#ifndef SYMINFO_CURRENT
#error "elf.h:SYMINFO_CURRENT macro is missing from libc-shim"
#endif

#ifndef SYMINFO_FLG_COPY
#error "elf.h:SYMINFO_FLG_COPY macro is missing from libc-shim"
#endif

#ifndef SYMINFO_FLG_DIRECT
#error "elf.h:SYMINFO_FLG_DIRECT macro is missing from libc-shim"
#endif

#ifndef SYMINFO_FLG_LAZYLOAD
#error "elf.h:SYMINFO_FLG_LAZYLOAD macro is missing from libc-shim"
#endif

#ifndef SYMINFO_FLG_PASSTHRU
#error "elf.h:SYMINFO_FLG_PASSTHRU macro is missing from libc-shim"
#endif

#ifndef SYMINFO_NONE
#error "elf.h:SYMINFO_NONE macro is missing from libc-shim"
#endif

#ifndef SYMINFO_NUM
#error "elf.h:SYMINFO_NUM macro is missing from libc-shim"
#endif

#ifndef VER_DEF_CURRENT
#error "elf.h:VER_DEF_CURRENT macro is missing from libc-shim"
#endif

#ifndef VER_DEF_NONE
#error "elf.h:VER_DEF_NONE macro is missing from libc-shim"
#endif

#ifndef VER_DEF_NUM
#error "elf.h:VER_DEF_NUM macro is missing from libc-shim"
#endif

#ifndef VER_FLG_BASE
#error "elf.h:VER_FLG_BASE macro is missing from libc-shim"
#endif

#ifndef VER_FLG_WEAK
#error "elf.h:VER_FLG_WEAK macro is missing from libc-shim"
#endif

#ifndef VER_NDX_ELIMINATE
#error "elf.h:VER_NDX_ELIMINATE macro is missing from libc-shim"
#endif

#ifndef VER_NDX_GLOBAL
#error "elf.h:VER_NDX_GLOBAL macro is missing from libc-shim"
#endif

#ifndef VER_NDX_LOCAL
#error "elf.h:VER_NDX_LOCAL macro is missing from libc-shim"
#endif

#ifndef VER_NDX_LORESERVE
#error "elf.h:VER_NDX_LORESERVE macro is missing from libc-shim"
#endif

#ifndef VER_NEED_CURRENT
#error "elf.h:VER_NEED_CURRENT macro is missing from libc-shim"
#endif

#ifndef VER_NEED_NONE
#error "elf.h:VER_NEED_NONE macro is missing from libc-shim"
#endif

#ifndef VER_NEED_NUM
#error "elf.h:VER_NEED_NUM macro is missing from libc-shim"
#endif

int main(void) { return 0; }
