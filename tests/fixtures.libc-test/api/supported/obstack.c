#include <stdlib.h>
#define obstack_chunk_alloc malloc
#define obstack_chunk_free free
#include <obstack.h>

void *allocate_with_arg(void *arg, long size);
void free_with_arg(void *arg, void *chunk);

static void f()
{
struct obstack x;
void *value = 0;
int length = 1;
int integer = 1;
x.chunk_size = 0;
x.chunk = 0;
x.object_base = 0;
x.next_free = 0;
x.chunk_limit = 0;
x.temp.tempint = 0;
x.temp.tempptr = 0;
x.alignment_mask = 0;
x.chunkfun = 0;
x.freefun = 0;
x.extra_arg = 0;
x.use_extra_arg = 0;
x.maybe_empty_object = 0;
x.alloc_failed = 0;
{void(*p)(struct obstack*,int) = _obstack_newchunk;}
{int(*p)(struct obstack*,int,int,void*(*)(long),void(*)(void*)) = _obstack_begin;}
{int(*p)(struct obstack*,int,int,void*(*)(void*,long),void(*)(void*,void*),void*) = _obstack_begin_1;}
{int(*p)(struct obstack*) = _obstack_memory_used;}
{void(*p)(struct obstack*,void*) = (obstack_free);}
{void(**p)(void) = &obstack_alloc_failed_handler;}
{int *p = &obstack_exit_failure;}
obstack_init(&x);
obstack_begin(&x, length);
obstack_specify_allocation(&x, length, length, malloc, free);
obstack_specify_allocation_with_arg(&x, length, length, allocate_with_arg, free_with_arg, value);
obstack_chunkfun(&x, malloc);
obstack_freefun(&x, free);
value = obstack_base(&x);
length = obstack_chunk_size(&x);
value = obstack_next_free(&x);
length = obstack_alignment_mask(&x);
obstack_1grow_fast(&x, integer);
obstack_blank_fast(&x, length);
length = obstack_memory_used(&x);
length = obstack_object_size(&x);
length = obstack_room(&x);
length = obstack_empty_p(&x);
obstack_make_room(&x, length);
obstack_grow(&x, value, length);
obstack_grow0(&x, value, length);
obstack_1grow(&x, integer);
obstack_ptr_grow(&x, value);
obstack_int_grow(&x, integer);
obstack_ptr_grow_fast(&x, value);
obstack_int_grow_fast(&x, integer);
obstack_blank(&x, length);
value = obstack_alloc(&x, length);
value = obstack_copy(&x, value, length);
value = obstack_copy0(&x, value, length);
value = obstack_finish(&x);
obstack_free(&x, value);
}
