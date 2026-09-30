#include <resolv.h>

_Static_assert(sizeof(struct res_sym) == 12, "struct res_sym size differs from oracle");

_Static_assert(_Alignof(struct res_sym) == 4, "struct res_sym alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct res_sym, number) == 0, "struct res_sym.number offset differs from oracle");

typedef int slate_oracle_struct_res_sym_number;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct res_sym *)0)->number), slate_oracle_struct_res_sym_number), "struct res_sym.number field type differs from oracle");

_Static_assert(__builtin_offsetof(struct res_sym, name) == 4, "struct res_sym.name offset differs from oracle");

typedef char * slate_oracle_struct_res_sym_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct res_sym *)0)->name), slate_oracle_struct_res_sym_name), "struct res_sym.name field type differs from oracle");

_Static_assert(__builtin_offsetof(struct res_sym, humanname) == 8, "struct res_sym.humanname offset differs from oracle");

typedef char * slate_oracle_struct_res_sym_humanname;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct res_sym *)0)->humanname), slate_oracle_struct_res_sym_humanname), "struct res_sym.humanname field type differs from oracle");

#ifndef LOCALDOMAINPARTS
#error "resolv.h:LOCALDOMAINPARTS macro is missing from libc-shim"
#endif

#ifndef RES_AAONLY
#error "resolv.h:RES_AAONLY macro is missing from libc-shim"
#endif

#ifndef RES_BLAST
#error "resolv.h:RES_BLAST macro is missing from libc-shim"
#endif

#ifndef RES_DEBUG
#error "resolv.h:RES_DEBUG macro is missing from libc-shim"
#endif

#ifndef RES_DEFAULT
#error "resolv.h:RES_DEFAULT macro is missing from libc-shim"
#endif

#ifndef RES_DEFNAMES
#error "resolv.h:RES_DEFNAMES macro is missing from libc-shim"
#endif

#ifndef RES_DFLRETRY
#error "resolv.h:RES_DFLRETRY macro is missing from libc-shim"
#endif

#ifndef RES_DNSRCH
#error "resolv.h:RES_DNSRCH macro is missing from libc-shim"
#endif

#ifndef RES_IGNTC
#error "resolv.h:RES_IGNTC macro is missing from libc-shim"
#endif

#ifndef RES_INIT
#error "resolv.h:RES_INIT macro is missing from libc-shim"
#endif

#ifndef RES_KEEPTSIG
#error "resolv.h:RES_KEEPTSIG macro is missing from libc-shim"
#endif

#ifndef RES_MAXNDOTS
#error "resolv.h:RES_MAXNDOTS macro is missing from libc-shim"
#endif

#ifndef RES_MAXRETRANS
#error "resolv.h:RES_MAXRETRANS macro is missing from libc-shim"
#endif

#ifndef RES_MAXRETRY
#error "resolv.h:RES_MAXRETRY macro is missing from libc-shim"
#endif

#ifndef RES_MAXTIME
#error "resolv.h:RES_MAXTIME macro is missing from libc-shim"
#endif

#ifndef RES_NOAAAA
#error "resolv.h:RES_NOAAAA macro is missing from libc-shim"
#endif

#ifndef RES_NOALIASES
#error "resolv.h:RES_NOALIASES macro is missing from libc-shim"
#endif

#ifndef RES_NOCHECKNAME
#error "resolv.h:RES_NOCHECKNAME macro is missing from libc-shim"
#endif

#ifndef RES_NORELOAD
#error "resolv.h:RES_NORELOAD macro is missing from libc-shim"
#endif

#ifndef RES_NOTLDQUERY
#error "resolv.h:RES_NOTLDQUERY macro is missing from libc-shim"
#endif

#ifndef RES_PRF_ADD
#error "resolv.h:RES_PRF_ADD macro is missing from libc-shim"
#endif

#ifndef RES_PRF_ANS
#error "resolv.h:RES_PRF_ANS macro is missing from libc-shim"
#endif

#ifndef RES_PRF_AUTH
#error "resolv.h:RES_PRF_AUTH macro is missing from libc-shim"
#endif

#ifndef RES_PRF_CLASS
#error "resolv.h:RES_PRF_CLASS macro is missing from libc-shim"
#endif

#ifndef RES_PRF_CMD
#error "resolv.h:RES_PRF_CMD macro is missing from libc-shim"
#endif

#ifndef RES_PRF_HEAD1
#error "resolv.h:RES_PRF_HEAD1 macro is missing from libc-shim"
#endif

#ifndef RES_PRF_HEAD2
#error "resolv.h:RES_PRF_HEAD2 macro is missing from libc-shim"
#endif

#ifndef RES_PRF_HEADX
#error "resolv.h:RES_PRF_HEADX macro is missing from libc-shim"
#endif

#ifndef RES_PRF_INIT
#error "resolv.h:RES_PRF_INIT macro is missing from libc-shim"
#endif

#ifndef RES_PRF_QUERY
#error "resolv.h:RES_PRF_QUERY macro is missing from libc-shim"
#endif

#ifndef RES_PRF_QUES
#error "resolv.h:RES_PRF_QUES macro is missing from libc-shim"
#endif

#ifndef RES_PRF_REPLY
#error "resolv.h:RES_PRF_REPLY macro is missing from libc-shim"
#endif

#ifndef RES_PRF_STATS
#error "resolv.h:RES_PRF_STATS macro is missing from libc-shim"
#endif

#ifndef RES_PRF_TTLID
#error "resolv.h:RES_PRF_TTLID macro is missing from libc-shim"
#endif

#ifndef RES_PRF_UPDATE
#error "resolv.h:RES_PRF_UPDATE macro is missing from libc-shim"
#endif

#ifndef RES_PRIMARY
#error "resolv.h:RES_PRIMARY macro is missing from libc-shim"
#endif

#ifndef RES_RECURSE
#error "resolv.h:RES_RECURSE macro is missing from libc-shim"
#endif

#ifndef RES_ROTATE
#error "resolv.h:RES_ROTATE macro is missing from libc-shim"
#endif

#ifndef RES_SNGLKUP
#error "resolv.h:RES_SNGLKUP macro is missing from libc-shim"
#endif

#ifndef RES_SNGLKUPREOP
#error "resolv.h:RES_SNGLKUPREOP macro is missing from libc-shim"
#endif

#ifndef RES_STAYOPEN
#error "resolv.h:RES_STAYOPEN macro is missing from libc-shim"
#endif

#ifndef RES_STRICTERR
#error "resolv.h:RES_STRICTERR macro is missing from libc-shim"
#endif

#ifndef RES_TIMEOUT
#error "resolv.h:RES_TIMEOUT macro is missing from libc-shim"
#endif

#ifndef RES_TRUSTAD
#error "resolv.h:RES_TRUSTAD macro is missing from libc-shim"
#endif

#ifndef RES_USEVC
#error "resolv.h:RES_USEVC macro is missing from libc-shim"
#endif

#ifndef RES_USE_DNSSEC
#error "resolv.h:RES_USE_DNSSEC macro is missing from libc-shim"
#endif

#ifndef RES_USE_EDNS0
#error "resolv.h:RES_USE_EDNS0 macro is missing from libc-shim"
#endif

#ifndef b64_ntop
#error "resolv.h:b64_ntop macro is missing from libc-shim"
#endif

#ifndef b64_pton
#error "resolv.h:b64_pton macro is missing from libc-shim"
#endif

#ifndef dn_count_labels
#error "resolv.h:dn_count_labels macro is missing from libc-shim"
#endif

#ifndef fp_nquery
#error "resolv.h:fp_nquery macro is missing from libc-shim"
#endif

#ifndef fp_query
#error "resolv.h:fp_query macro is missing from libc-shim"
#endif

#ifndef fp_resstat
#error "resolv.h:fp_resstat macro is missing from libc-shim"
#endif

#ifndef hostalias
#error "resolv.h:hostalias macro is missing from libc-shim"
#endif

#ifndef loc_aton
#error "resolv.h:loc_aton macro is missing from libc-shim"
#endif

#ifndef loc_ntoa
#error "resolv.h:loc_ntoa macro is missing from libc-shim"
#endif

#ifndef nsaddr
#error "resolv.h:nsaddr macro is missing from libc-shim"
#endif

#ifndef p_cdname
#error "resolv.h:p_cdname macro is missing from libc-shim"
#endif

#ifndef p_cdnname
#error "resolv.h:p_cdnname macro is missing from libc-shim"
#endif

#ifndef p_class
#error "resolv.h:p_class macro is missing from libc-shim"
#endif

#ifndef p_fqname
#error "resolv.h:p_fqname macro is missing from libc-shim"
#endif

#ifndef p_fqnname
#error "resolv.h:p_fqnname macro is missing from libc-shim"
#endif

#ifndef p_option
#error "resolv.h:p_option macro is missing from libc-shim"
#endif

#ifndef p_query
#error "resolv.h:p_query macro is missing from libc-shim"
#endif

#ifndef p_rcode
#error "resolv.h:p_rcode macro is missing from libc-shim"
#endif

#ifndef p_time
#error "resolv.h:p_time macro is missing from libc-shim"
#endif

#ifndef p_type
#error "resolv.h:p_type macro is missing from libc-shim"
#endif

#ifndef putlong
#error "resolv.h:putlong macro is missing from libc-shim"
#endif

#ifndef putshort
#error "resolv.h:putshort macro is missing from libc-shim"
#endif

#ifndef res_close
#error "resolv.h:res_close macro is missing from libc-shim"
#endif

#ifndef res_hostalias
#error "resolv.h:res_hostalias macro is missing from libc-shim"
#endif

#ifndef res_init
#error "resolv.h:res_init macro is missing from libc-shim"
#endif

#ifndef res_isourserver
#error "resolv.h:res_isourserver macro is missing from libc-shim"
#endif

#ifndef res_nameinquery
#error "resolv.h:res_nameinquery macro is missing from libc-shim"
#endif

#ifndef res_nclose
#error "resolv.h:res_nclose macro is missing from libc-shim"
#endif

#ifndef res_ninit
#error "resolv.h:res_ninit macro is missing from libc-shim"
#endif

#ifndef res_queriesmatch
#error "resolv.h:res_queriesmatch macro is missing from libc-shim"
#endif

#ifndef res_randomid
#error "resolv.h:res_randomid macro is missing from libc-shim"
#endif

#ifndef sym_ntop
#error "resolv.h:sym_ntop macro is missing from libc-shim"
#endif

#ifndef sym_ntos
#error "resolv.h:sym_ntos macro is missing from libc-shim"
#endif

#ifndef sym_ston
#error "resolv.h:sym_ston macro is missing from libc-shim"
#endif

int main(void) { return 0; }
