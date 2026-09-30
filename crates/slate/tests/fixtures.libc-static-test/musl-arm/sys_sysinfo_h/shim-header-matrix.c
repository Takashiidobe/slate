#include <sys/sysinfo.h>

_Static_assert(sizeof(struct sysinfo) == 312, "struct sysinfo size differs from oracle");

_Static_assert(_Alignof(struct sysinfo) == 4, "struct sysinfo alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct sysinfo, uptime) == 0, "struct sysinfo.uptime offset differs from oracle");

typedef unsigned long slate_oracle_struct_sysinfo_uptime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sysinfo *)0)->uptime), slate_oracle_struct_sysinfo_uptime), "struct sysinfo.uptime field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sysinfo, loads) == 4, "struct sysinfo.loads offset differs from oracle");

_Static_assert(__builtin_offsetof(struct sysinfo, totalram) == 16, "struct sysinfo.totalram offset differs from oracle");

typedef unsigned long slate_oracle_struct_sysinfo_totalram;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sysinfo *)0)->totalram), slate_oracle_struct_sysinfo_totalram), "struct sysinfo.totalram field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sysinfo, freeram) == 20, "struct sysinfo.freeram offset differs from oracle");

typedef unsigned long slate_oracle_struct_sysinfo_freeram;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sysinfo *)0)->freeram), slate_oracle_struct_sysinfo_freeram), "struct sysinfo.freeram field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sysinfo, sharedram) == 24, "struct sysinfo.sharedram offset differs from oracle");

typedef unsigned long slate_oracle_struct_sysinfo_sharedram;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sysinfo *)0)->sharedram), slate_oracle_struct_sysinfo_sharedram), "struct sysinfo.sharedram field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sysinfo, bufferram) == 28, "struct sysinfo.bufferram offset differs from oracle");

typedef unsigned long slate_oracle_struct_sysinfo_bufferram;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sysinfo *)0)->bufferram), slate_oracle_struct_sysinfo_bufferram), "struct sysinfo.bufferram field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sysinfo, totalswap) == 32, "struct sysinfo.totalswap offset differs from oracle");

typedef unsigned long slate_oracle_struct_sysinfo_totalswap;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sysinfo *)0)->totalswap), slate_oracle_struct_sysinfo_totalswap), "struct sysinfo.totalswap field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sysinfo, freeswap) == 36, "struct sysinfo.freeswap offset differs from oracle");

typedef unsigned long slate_oracle_struct_sysinfo_freeswap;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sysinfo *)0)->freeswap), slate_oracle_struct_sysinfo_freeswap), "struct sysinfo.freeswap field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sysinfo, procs) == 40, "struct sysinfo.procs offset differs from oracle");

typedef unsigned short slate_oracle_struct_sysinfo_procs;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sysinfo *)0)->procs), slate_oracle_struct_sysinfo_procs), "struct sysinfo.procs field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sysinfo, pad) == 42, "struct sysinfo.pad offset differs from oracle");

typedef unsigned short slate_oracle_struct_sysinfo_pad;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sysinfo *)0)->pad), slate_oracle_struct_sysinfo_pad), "struct sysinfo.pad field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sysinfo, totalhigh) == 44, "struct sysinfo.totalhigh offset differs from oracle");

typedef unsigned long slate_oracle_struct_sysinfo_totalhigh;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sysinfo *)0)->totalhigh), slate_oracle_struct_sysinfo_totalhigh), "struct sysinfo.totalhigh field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sysinfo, freehigh) == 48, "struct sysinfo.freehigh offset differs from oracle");

typedef unsigned long slate_oracle_struct_sysinfo_freehigh;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sysinfo *)0)->freehigh), slate_oracle_struct_sysinfo_freehigh), "struct sysinfo.freehigh field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sysinfo, mem_unit) == 52, "struct sysinfo.mem_unit offset differs from oracle");

typedef unsigned int slate_oracle_struct_sysinfo_mem_unit;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sysinfo *)0)->mem_unit), slate_oracle_struct_sysinfo_mem_unit), "struct sysinfo.mem_unit field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sysinfo, __reserved) == 56, "struct sysinfo.__reserved offset differs from oracle");

#ifndef SI_LOAD_SHIFT
#error "sys/sysinfo.h:SI_LOAD_SHIFT macro is missing from libc-shim"
#endif

int main(void) { return 0; }
