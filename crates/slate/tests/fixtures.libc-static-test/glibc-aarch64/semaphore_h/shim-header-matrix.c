#include <semaphore.h>

extern int slate_oracle_sem_init(sem_t *, int, unsigned int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sem_init), __typeof__(sem_init)),
    "semaphore.h:sem_init declaration differs from oracle");

static __typeof__(sem_init) *const slate_reference_sem_init = &sem_init;

#ifndef SEM_FAILED
#error "semaphore.h:SEM_FAILED macro is missing from libc-shim"
#endif

int main(void) { return 0; }
