#include <malloc.h>
#define T(t) (t*)0;
#define F(t,n) {t *y = &x.n;}
#define C(n) switch(n){case n:;}
static void f()
{
T(size_t)
T(FILE)
T(struct mallinfo)
T(struct mallinfo2)
{
struct mallinfo x;
F(int,arena)
F(int,ordblks)
F(int,smblks)
F(int,hblks)
F(int,hblkhd)
F(int,usmblks)
F(int,fsmblks)
F(int,uordblks)
F(int,fordblks)
F(int,keepcost)
}
{
struct mallinfo2 x;
F(size_t,arena)
F(size_t,ordblks)
F(size_t,smblks)
F(size_t,hblks)
F(size_t,hblkhd)
F(size_t,usmblks)
F(size_t,fsmblks)
F(size_t,uordblks)
F(size_t,fordblks)
F(size_t,keepcost)
}
C(M_MXFAST)
C(M_NLBLKS)
C(M_GRAIN)
C(M_KEEP)
C(M_TRIM_THRESHOLD)
C(M_TOP_PAD)
C(M_MMAP_THRESHOLD)
C(M_MMAP_MAX)
C(M_CHECK_ACTION)
C(M_PERTURB)
C(M_ARENA_TEST)
C(M_ARENA_MAX)
{void*(*p)(size_t) = malloc;}
{void*(*p)(size_t,size_t) = calloc;}
{void*(*p)(void*,size_t) = realloc;}
{void*(*p)(void*,size_t,size_t) = reallocarray;}
{void(*p)(void*) = free;}
{void*(*p)(size_t,size_t) = memalign;}
{void*(*p)(size_t) = valloc;}
{void*(*p)(size_t) = pvalloc;}
{struct mallinfo(*p)(void) = mallinfo;}
{struct mallinfo2(*p)(void) = mallinfo2;}
{int(*p)(int,int) = mallopt;}
{int(*p)(size_t) = malloc_trim;}
{size_t(*p)(void*) = malloc_usable_size;}
{void(*p)(void) = malloc_stats;}
{int(*p)(int,FILE*) = malloc_info;}
}
