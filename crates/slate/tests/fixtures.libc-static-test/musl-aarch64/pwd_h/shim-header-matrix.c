#include <pwd.h>

_Static_assert(sizeof(struct passwd) == 48, "struct passwd size differs from oracle");

_Static_assert(_Alignof(struct passwd) == 8, "struct passwd alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct passwd, pw_name) == 0, "struct passwd.pw_name offset differs from oracle");

typedef char * slate_oracle_struct_passwd_pw_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct passwd *)0)->pw_name), slate_oracle_struct_passwd_pw_name), "struct passwd.pw_name field type differs from oracle");

_Static_assert(__builtin_offsetof(struct passwd, pw_passwd) == 8, "struct passwd.pw_passwd offset differs from oracle");

typedef char * slate_oracle_struct_passwd_pw_passwd;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct passwd *)0)->pw_passwd), slate_oracle_struct_passwd_pw_passwd), "struct passwd.pw_passwd field type differs from oracle");

_Static_assert(__builtin_offsetof(struct passwd, pw_uid) == 16, "struct passwd.pw_uid offset differs from oracle");

typedef unsigned int slate_oracle_struct_passwd_pw_uid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct passwd *)0)->pw_uid), slate_oracle_struct_passwd_pw_uid), "struct passwd.pw_uid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct passwd, pw_gid) == 20, "struct passwd.pw_gid offset differs from oracle");

typedef unsigned int slate_oracle_struct_passwd_pw_gid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct passwd *)0)->pw_gid), slate_oracle_struct_passwd_pw_gid), "struct passwd.pw_gid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct passwd, pw_gecos) == 24, "struct passwd.pw_gecos offset differs from oracle");

typedef char * slate_oracle_struct_passwd_pw_gecos;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct passwd *)0)->pw_gecos), slate_oracle_struct_passwd_pw_gecos), "struct passwd.pw_gecos field type differs from oracle");

_Static_assert(__builtin_offsetof(struct passwd, pw_dir) == 32, "struct passwd.pw_dir offset differs from oracle");

typedef char * slate_oracle_struct_passwd_pw_dir;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct passwd *)0)->pw_dir), slate_oracle_struct_passwd_pw_dir), "struct passwd.pw_dir field type differs from oracle");

_Static_assert(__builtin_offsetof(struct passwd, pw_shell) == 40, "struct passwd.pw_shell offset differs from oracle");

typedef char * slate_oracle_struct_passwd_pw_shell;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct passwd *)0)->pw_shell), slate_oracle_struct_passwd_pw_shell), "struct passwd.pw_shell field type differs from oracle");

int main(void) { return 0; }
