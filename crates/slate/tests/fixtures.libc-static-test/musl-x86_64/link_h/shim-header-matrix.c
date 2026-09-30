#include <link.h>

typedef unsigned int slate_oracle_typedef_Elf_Symndx;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_Elf_Symndx, Elf_Symndx), "typedef Elf_Symndx differs from oracle");

_Static_assert(sizeof(struct dl_phdr_info) == 64, "struct dl_phdr_info size differs from oracle");

_Static_assert(_Alignof(struct dl_phdr_info) == 8, "struct dl_phdr_info alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct dl_phdr_info, dlpi_addr) == 0, "struct dl_phdr_info.dlpi_addr offset differs from oracle");

typedef unsigned long slate_oracle_struct_dl_phdr_info_dlpi_addr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dl_phdr_info *)0)->dlpi_addr), slate_oracle_struct_dl_phdr_info_dlpi_addr), "struct dl_phdr_info.dlpi_addr field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dl_phdr_info, dlpi_name) == 8, "struct dl_phdr_info.dlpi_name offset differs from oracle");

typedef const char * slate_oracle_struct_dl_phdr_info_dlpi_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dl_phdr_info *)0)->dlpi_name), slate_oracle_struct_dl_phdr_info_dlpi_name), "struct dl_phdr_info.dlpi_name field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dl_phdr_info, dlpi_phdr) == 16, "struct dl_phdr_info.dlpi_phdr offset differs from oracle");

typedef const Elf64_Phdr * slate_oracle_struct_dl_phdr_info_dlpi_phdr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dl_phdr_info *)0)->dlpi_phdr), slate_oracle_struct_dl_phdr_info_dlpi_phdr), "struct dl_phdr_info.dlpi_phdr field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dl_phdr_info, dlpi_phnum) == 24, "struct dl_phdr_info.dlpi_phnum offset differs from oracle");

typedef unsigned short slate_oracle_struct_dl_phdr_info_dlpi_phnum;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dl_phdr_info *)0)->dlpi_phnum), slate_oracle_struct_dl_phdr_info_dlpi_phnum), "struct dl_phdr_info.dlpi_phnum field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dl_phdr_info, dlpi_adds) == 32, "struct dl_phdr_info.dlpi_adds offset differs from oracle");

typedef unsigned long long slate_oracle_struct_dl_phdr_info_dlpi_adds;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dl_phdr_info *)0)->dlpi_adds), slate_oracle_struct_dl_phdr_info_dlpi_adds), "struct dl_phdr_info.dlpi_adds field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dl_phdr_info, dlpi_subs) == 40, "struct dl_phdr_info.dlpi_subs offset differs from oracle");

typedef unsigned long long slate_oracle_struct_dl_phdr_info_dlpi_subs;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dl_phdr_info *)0)->dlpi_subs), slate_oracle_struct_dl_phdr_info_dlpi_subs), "struct dl_phdr_info.dlpi_subs field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dl_phdr_info, dlpi_tls_modid) == 48, "struct dl_phdr_info.dlpi_tls_modid offset differs from oracle");

typedef unsigned long slate_oracle_struct_dl_phdr_info_dlpi_tls_modid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dl_phdr_info *)0)->dlpi_tls_modid), slate_oracle_struct_dl_phdr_info_dlpi_tls_modid), "struct dl_phdr_info.dlpi_tls_modid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dl_phdr_info, dlpi_tls_data) == 56, "struct dl_phdr_info.dlpi_tls_data offset differs from oracle");

typedef void * slate_oracle_struct_dl_phdr_info_dlpi_tls_data;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dl_phdr_info *)0)->dlpi_tls_data), slate_oracle_struct_dl_phdr_info_dlpi_tls_data), "struct dl_phdr_info.dlpi_tls_data field type differs from oracle");

#ifndef ElfW
#error "link.h:ElfW macro is missing from libc-shim"
#endif

int main(void) { return 0; }
