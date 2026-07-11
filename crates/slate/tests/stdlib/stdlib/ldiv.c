#include <stdlib.h>
#include <stdio.h>
int main(void) { ldiv_t d = ldiv(17L, 5L); printf("%ld %ld\n", d.quot, d.rem); return 0; }
