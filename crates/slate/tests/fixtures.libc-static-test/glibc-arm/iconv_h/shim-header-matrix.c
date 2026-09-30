#include <iconv.h>

typedef void * slate_oracle_typedef_iconv_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_iconv_t, iconv_t), "typedef iconv_t differs from oracle");

int main(void) { return 0; }
