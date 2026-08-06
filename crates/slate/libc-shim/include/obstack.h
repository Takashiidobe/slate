#ifndef _SLATE_OBSTACK_H
#define _SLATE_OBSTACK_H

#include <features.h>

#include <stddef.h>

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

  unsigned use_extra_arg : 1;
  unsigned alloc_failed  : 1;
};

void obstack_free(struct obstack *, void *);

extern void (*obstack_alloc_failed_handler)(void);
extern int  obstack_exit_failure;
int         obstack_init(struct obstack *);
int         obstack_begin(struct obstack *, int);
void       *obstack_alloc(struct obstack *, int);
void       *obstack_copy(struct obstack *, const void *, int);
void       *obstack_copy0(struct obstack *, const void *, int);

void obstack_grow(struct obstack *, const void *, int);
void obstack_grow0(struct obstack *, const void *, int);
void obstack_1grow(struct obstack *, int);
void obstack_blank(struct obstack *, int);

void *obstack_finish(struct obstack *);
void  obstack_free(struct obstack *, void *);

void  *obstack_base(struct obstack *);
void  *obstack_next_free(struct obstack *);
size_t obstack_object_size(struct obstack *);
size_t obstack_room(struct obstack *);
int    obstack_empty_p(struct obstack *);

long obstack_chunk_size(struct obstack *);
int  obstack_alignment_mask(struct obstack *);

#endif
