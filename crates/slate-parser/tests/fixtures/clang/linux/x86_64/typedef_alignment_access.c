typedef long long V __attribute__((vector_size(16)));
typedef struct S { V b; } P __attribute__((aligned(1)));
typedef int A __attribute__((aligned(1)));
V f(P *p) { return p->b; }
int g(A *a) { return *a; }


