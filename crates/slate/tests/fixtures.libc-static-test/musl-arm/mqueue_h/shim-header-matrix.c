#include <mqueue.h>

typedef int slate_oracle_typedef_mqd_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_mqd_t, mqd_t), "typedef mqd_t differs from oracle");

int main(void) { return 0; }
