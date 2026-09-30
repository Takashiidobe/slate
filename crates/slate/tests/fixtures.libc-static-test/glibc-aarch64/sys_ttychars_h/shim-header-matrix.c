#include <sys/ttychars.h>

_Static_assert(sizeof(struct ttychars) == 14, "struct ttychars size differs from oracle");

_Static_assert(_Alignof(struct ttychars) == 1, "struct ttychars alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct ttychars, tc_erase) == 0, "struct ttychars.tc_erase offset differs from oracle");

typedef char slate_oracle_struct_ttychars_tc_erase;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttychars *)0)->tc_erase), slate_oracle_struct_ttychars_tc_erase), "struct ttychars.tc_erase field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttychars, tc_kill) == 1, "struct ttychars.tc_kill offset differs from oracle");

typedef char slate_oracle_struct_ttychars_tc_kill;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttychars *)0)->tc_kill), slate_oracle_struct_ttychars_tc_kill), "struct ttychars.tc_kill field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttychars, tc_intrc) == 2, "struct ttychars.tc_intrc offset differs from oracle");

typedef char slate_oracle_struct_ttychars_tc_intrc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttychars *)0)->tc_intrc), slate_oracle_struct_ttychars_tc_intrc), "struct ttychars.tc_intrc field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttychars, tc_quitc) == 3, "struct ttychars.tc_quitc offset differs from oracle");

typedef char slate_oracle_struct_ttychars_tc_quitc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttychars *)0)->tc_quitc), slate_oracle_struct_ttychars_tc_quitc), "struct ttychars.tc_quitc field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttychars, tc_startc) == 4, "struct ttychars.tc_startc offset differs from oracle");

typedef char slate_oracle_struct_ttychars_tc_startc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttychars *)0)->tc_startc), slate_oracle_struct_ttychars_tc_startc), "struct ttychars.tc_startc field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttychars, tc_stopc) == 5, "struct ttychars.tc_stopc offset differs from oracle");

typedef char slate_oracle_struct_ttychars_tc_stopc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttychars *)0)->tc_stopc), slate_oracle_struct_ttychars_tc_stopc), "struct ttychars.tc_stopc field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttychars, tc_eofc) == 6, "struct ttychars.tc_eofc offset differs from oracle");

typedef char slate_oracle_struct_ttychars_tc_eofc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttychars *)0)->tc_eofc), slate_oracle_struct_ttychars_tc_eofc), "struct ttychars.tc_eofc field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttychars, tc_brkc) == 7, "struct ttychars.tc_brkc offset differs from oracle");

typedef char slate_oracle_struct_ttychars_tc_brkc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttychars *)0)->tc_brkc), slate_oracle_struct_ttychars_tc_brkc), "struct ttychars.tc_brkc field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttychars, tc_suspc) == 8, "struct ttychars.tc_suspc offset differs from oracle");

typedef char slate_oracle_struct_ttychars_tc_suspc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttychars *)0)->tc_suspc), slate_oracle_struct_ttychars_tc_suspc), "struct ttychars.tc_suspc field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttychars, tc_dsuspc) == 9, "struct ttychars.tc_dsuspc offset differs from oracle");

typedef char slate_oracle_struct_ttychars_tc_dsuspc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttychars *)0)->tc_dsuspc), slate_oracle_struct_ttychars_tc_dsuspc), "struct ttychars.tc_dsuspc field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttychars, tc_rprntc) == 10, "struct ttychars.tc_rprntc offset differs from oracle");

typedef char slate_oracle_struct_ttychars_tc_rprntc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttychars *)0)->tc_rprntc), slate_oracle_struct_ttychars_tc_rprntc), "struct ttychars.tc_rprntc field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttychars, tc_flushc) == 11, "struct ttychars.tc_flushc offset differs from oracle");

typedef char slate_oracle_struct_ttychars_tc_flushc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttychars *)0)->tc_flushc), slate_oracle_struct_ttychars_tc_flushc), "struct ttychars.tc_flushc field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttychars, tc_werasc) == 12, "struct ttychars.tc_werasc offset differs from oracle");

typedef char slate_oracle_struct_ttychars_tc_werasc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttychars *)0)->tc_werasc), slate_oracle_struct_ttychars_tc_werasc), "struct ttychars.tc_werasc field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttychars, tc_lnextc) == 13, "struct ttychars.tc_lnextc offset differs from oracle");

typedef char slate_oracle_struct_ttychars_tc_lnextc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttychars *)0)->tc_lnextc), slate_oracle_struct_ttychars_tc_lnextc), "struct ttychars.tc_lnextc field type differs from oracle");

int main(void) { return 0; }
