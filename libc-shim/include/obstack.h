#ifndef _SLATE_OBSTACK_H
#define _SLATE_OBSTACK_H

#include <features.h>

#include <stddef.h>
#include <string.h>

#define __BPTR_ALIGN(base, pointer, alignment)                                  \
  ((base) + (((pointer) - (base) + (alignment)) & ~(alignment)))
#define __PTR_ALIGN(base, pointer, alignment)                                   \
  __BPTR_ALIGN(sizeof(ptrdiff_t) < sizeof(void *) ? (base) : (char *)0,         \
               (pointer), (alignment))

struct _obstack_chunk {
  char                  *limit;
  struct _obstack_chunk *prev;
  char                   contents[4];
};

struct obstack {
  long                   chunk_size;
  struct _obstack_chunk *chunk;
  char                  *object_base;
  char                  *next_free;
  char                  *chunk_limit;

  union {
    ptrdiff_t tempint;
    void     *tempptr;
  } temp;

  int alignment_mask;

  struct _obstack_chunk *(*chunkfun)(void *, long);
  void                   (*freefun)(void *, struct _obstack_chunk *);
  void                  *extra_arg;

  unsigned use_extra_arg     : 1;
  unsigned maybe_empty_object : 1;
  unsigned alloc_failed      : 1;
};

void _obstack_newchunk(struct obstack *, int);
int  _obstack_begin(struct obstack *, int, int, void *(*)(long),
                    void (*)(void *));
int  _obstack_begin_1(struct obstack *, int, int, void *(*)(void *, long),
                      void (*)(void *, void *), void *);
int  _obstack_memory_used(struct obstack *);
void obstack_free(struct obstack *, void *);

extern void (*obstack_alloc_failed_handler)(void);
extern int obstack_exit_failure;

#ifndef __obstack_free
#define __obstack_free obstack_free
#endif

#define obstack_base(obstack) ((void *)(obstack)->object_base)
#define obstack_chunk_size(obstack) ((obstack)->chunk_size)
#define obstack_next_free(obstack) ((obstack)->next_free)
#define obstack_alignment_mask(obstack) ((obstack)->alignment_mask)

#define obstack_init(obstack)                                                 \
  _obstack_begin((obstack), 0, 0, (void *(*)(long))obstack_chunk_alloc,       \
                 (void (*)(void *))obstack_chunk_free)
#define obstack_begin(obstack, size)                                          \
  _obstack_begin((obstack), (size), 0,                                       \
                 (void *(*)(long))obstack_chunk_alloc,                        \
                 (void (*)(void *))obstack_chunk_free)
#define obstack_specify_allocation(obstack, size, alignment, chunkfun,        \
                                   freefun)                                    \
  _obstack_begin((obstack), (size), (alignment),                             \
                 (void *(*)(long))(chunkfun), (void (*)(void *))(freefun))
#define obstack_specify_allocation_with_arg(                                  \
    obstack, size, alignment, chunkfun, freefun, arg)                         \
  _obstack_begin_1((obstack), (size), (alignment),                           \
                   (void *(*)(void *, long))(chunkfun),                       \
                   (void (*)(void *, void *))(freefun), (arg))
#define obstack_chunkfun(obstack, new_chunkfun)                               \
  ((obstack)->chunkfun =                                                     \
       (struct _obstack_chunk *(*)(void *, long))(new_chunkfun))
#define obstack_freefun(obstack, new_freefun)                                 \
  ((obstack)->freefun =                                                      \
       (void (*)(void *, struct _obstack_chunk *))(new_freefun))

#define obstack_1grow_fast(obstack, value)                                    \
  (*((obstack)->next_free)++ = (value))
#define obstack_blank_fast(obstack, length) ((obstack)->next_free += (length))
#define obstack_memory_used(obstack) _obstack_memory_used(obstack)
#define obstack_object_size(obstack)                                          \
  ((unsigned)((obstack)->next_free - (obstack)->object_base))
#define obstack_room(obstack)                                                 \
  ((unsigned)((obstack)->chunk_limit - (obstack)->next_free))
#define obstack_empty_p(obstack)                                              \
  ((obstack)->chunk->prev == 0 &&                                            \
   (obstack)->next_free ==                                                   \
       __PTR_ALIGN((char *)(obstack)->chunk, (obstack)->chunk->contents,     \
                   (obstack)->alignment_mask))

#define obstack_make_room(obstack, length)                                    \
  ((obstack)->temp.tempint = (length),                                       \
   ((obstack)->next_free + (obstack)->temp.tempint >                         \
            (obstack)->chunk_limit                                           \
        ? (_obstack_newchunk((obstack), (obstack)->temp.tempint), 0)         \
        : 0))
#define obstack_grow(obstack, source, length)                                 \
  ((obstack)->temp.tempint = (length),                                       \
   ((obstack)->next_free + (obstack)->temp.tempint >                         \
            (obstack)->chunk_limit                                           \
        ? (_obstack_newchunk((obstack), (obstack)->temp.tempint), 0)         \
        : 0),                                                               \
   memcpy((obstack)->next_free, (source), (obstack)->temp.tempint),          \
   (obstack)->next_free += (obstack)->temp.tempint)
#define obstack_grow0(obstack, source, length)                                \
  ((obstack)->temp.tempint = (length),                                       \
   ((obstack)->next_free + (obstack)->temp.tempint + 1 >                     \
            (obstack)->chunk_limit                                           \
        ? (_obstack_newchunk((obstack), (obstack)->temp.tempint + 1), 0)     \
        : 0),                                                               \
   memcpy((obstack)->next_free, (source), (obstack)->temp.tempint),          \
   (obstack)->next_free += (obstack)->temp.tempint,                          \
   *((obstack)->next_free)++ = 0)
#define obstack_1grow(obstack, value)                                         \
  (((obstack)->next_free + 1 > (obstack)->chunk_limit                        \
        ? (_obstack_newchunk((obstack), 1), 0)                               \
        : 0),                                                               \
   obstack_1grow_fast((obstack), (value)))
#define obstack_ptr_grow(obstack, value)                                      \
  (((obstack)->next_free + sizeof(void *) > (obstack)->chunk_limit           \
        ? (_obstack_newchunk((obstack), sizeof(void *)), 0)                  \
        : 0),                                                               \
   obstack_ptr_grow_fast((obstack), (value)))
#define obstack_int_grow(obstack, value)                                      \
  (((obstack)->next_free + sizeof(int) > (obstack)->chunk_limit              \
        ? (_obstack_newchunk((obstack), sizeof(int)), 0)                     \
        : 0),                                                               \
   obstack_int_grow_fast((obstack), (value)))
#define obstack_ptr_grow_fast(obstack, value)                                 \
  (((const void **)((obstack)->next_free += sizeof(void *)))[-1] = (value))
#define obstack_int_grow_fast(obstack, value)                                 \
  (((int *)((obstack)->next_free += sizeof(int)))[-1] = (value))
#define obstack_blank(obstack, length)                                        \
  ((obstack)->temp.tempint = (length),                                       \
   ((obstack)->chunk_limit - (obstack)->next_free <                          \
            (obstack)->temp.tempint                                          \
        ? (_obstack_newchunk((obstack), (obstack)->temp.tempint), 0)         \
        : 0),                                                               \
   obstack_blank_fast((obstack), (obstack)->temp.tempint))
#define obstack_alloc(obstack, length)                                        \
  (obstack_blank((obstack), (length)), obstack_finish((obstack)))
#define obstack_copy(obstack, source, length)                                 \
  (obstack_grow((obstack), (source), (length)), obstack_finish((obstack)))
#define obstack_copy0(obstack, source, length)                                \
  (obstack_grow0((obstack), (source), (length)), obstack_finish((obstack)))
#define obstack_finish(obstack)                                               \
  (((obstack)->next_free == (obstack)->object_base                           \
        ? (((obstack)->maybe_empty_object = 1), 0)                           \
        : 0),                                                               \
   (obstack)->temp.tempptr = (obstack)->object_base,                         \
   (obstack)->next_free =                                                    \
       __PTR_ALIGN((obstack)->object_base, (obstack)->next_free,             \
                   (obstack)->alignment_mask),                               \
   ((obstack)->next_free - (char *)(obstack)->chunk >                        \
            (obstack)->chunk_limit - (char *)(obstack)->chunk                \
        ? ((obstack)->next_free = (obstack)->chunk_limit)                    \
        : 0),                                                               \
   (obstack)->object_base = (obstack)->next_free, (obstack)->temp.tempptr)
#define obstack_free(obstack, object)                                         \
  ((obstack)->temp.tempint =                                                 \
       (char *)(object) - (char *)(obstack)->chunk,                          \
   ((obstack)->temp.tempint > 0 &&                                           \
            (obstack)->temp.tempint <                                        \
                (obstack)->chunk_limit - (char *)(obstack)->chunk            \
        ? (void)((obstack)->next_free = (obstack)->object_base =             \
                     (obstack)->temp.tempint + (char *)(obstack)->chunk)      \
        : (__obstack_free)(                                                  \
              (obstack),                                                     \
              (obstack)->temp.tempint + (char *)(obstack)->chunk)))

#endif
