#include <stdlib.h>
#include <stdio.h>
static int cmp(const void *a, const void *b) { return *(const int*)a - *(const int*)b; }
int main(void) { int a[5] = {3,1,4,1,5}; qsort(a,5,sizeof(int),cmp); for(int i=0;i<5;i++) printf("%d",a[i]); printf("\n"); return 0; }
