#include <mqueue.h>

extern int slate_oracle_mq_open(const char *, int, ...);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mq_open), __typeof__(mq_open)),
    "mqueue.h:mq_open declaration differs from oracle");

static __typeof__(mq_open) *const slate_reference_mq_open = &mq_open;

typedef int slate_oracle_typedef_mqd_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_mqd_t, mqd_t), "typedef mqd_t differs from oracle");

int main(void) { return 0; }
