#include <stdbool.h>
#include <stdio.h>

bool        enabled = true;
static bool disabled;

int main(void) { printf("%d %d\n", enabled, disabled); }
