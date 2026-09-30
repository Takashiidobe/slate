#include <crypt.h>

_Static_assert(sizeof(struct crypt_data) == 260, "struct crypt_data size differs from oracle");

_Static_assert(_Alignof(struct crypt_data) == 4, "struct crypt_data alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct crypt_data, initialized) == 0, "struct crypt_data.initialized offset differs from oracle");

typedef int slate_oracle_struct_crypt_data_initialized;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct crypt_data *)0)->initialized), slate_oracle_struct_crypt_data_initialized), "struct crypt_data.initialized field type differs from oracle");

_Static_assert(__builtin_offsetof(struct crypt_data, __buf) == 4, "struct crypt_data.__buf offset differs from oracle");

int main(void) { return 0; }
