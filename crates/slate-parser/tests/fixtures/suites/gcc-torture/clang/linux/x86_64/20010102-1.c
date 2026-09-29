// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17

/* This testcase derives from gnu obstack.c/obstack.h and failed with
   -O3 -funroll-all-loops, or -O1 -frename-registers -funroll-loops on
   sparc-sun-solaris2.7.

   Copyright (C) 2001  Free Software Foundation.  */

/* { dg-require-effective-target indirect_calls } */

# define PTR_INT_TYPE __PTRDIFF_TYPE__

struct _obstack_chunk
{
  char  *limit;
  struct _obstack_chunk *prev;
  char	contents[4];
};

struct obstack
{
  long	chunk_size;
  struct _obstack_chunk *chunk;
  char	*object_base;
  char	*next_free;
  char	*chunk_limit;
  PTR_INT_TYPE temp;
  int   alignment_mask;
  struct _obstack_chunk *(*chunkfun) (void *, long);
  void (*freefun) (void *, struct _obstack_chunk *);
  void *extra_arg;
  unsigned use_extra_arg:1;
  unsigned maybe_empty_object:1;
  unsigned alloc_failed:1;
};

extern void _obstack_newchunk (struct obstack *, int);

struct fooalign {char x; double d;};
#define DEFAULT_ALIGNMENT  \
  ((PTR_INT_TYPE) ((char *) &((struct fooalign *) 0)->d - (char *) 0))
union fooround {long x; double d;};
#define DEFAULT_ROUNDING (sizeof (union fooround))

#ifndef COPYING_UNIT
#define COPYING_UNIT int
#endif

#define CALL_CHUNKFUN(h, size) \
  (((h) -> use_extra_arg) \
   ? (*(h)->chunkfun) ((h)->extra_arg, (size)) \
   : (*(struct _obstack_chunk *(*) (long)) (h)->chunkfun) ((size)))

#define CALL_FREEFUN(h, old_chunk) \
  do { \
    if ((h) -> use_extra_arg) \
      (*(h)->freefun) ((h)->extra_arg, (old_chunk)); \
    else \
      (*(void (*) (void *)) (h)->freefun) ((old_chunk)); \
  } while (0)

void
_obstack_newchunk (h, length)
     struct obstack *h;
     int length;
{
  register struct _obstack_chunk *old_chunk = h->chunk;
  register struct _obstack_chunk *new_chunk;
  register long	new_size;
  register long obj_size = h->next_free - h->object_base;
  register long i;
  long already;

  new_size = (obj_size + length) + (obj_size >> 3) + 100;
  if (new_size < h->chunk_size)
    new_size = h->chunk_size;

  new_chunk = CALL_CHUNKFUN (h, new_size);
  h->chunk = new_chunk;
  new_chunk->prev = old_chunk;
  new_chunk->limit = h->chunk_limit = (char *) new_chunk + new_size;

  if (h->alignment_mask + 1 >= DEFAULT_ALIGNMENT)
    {
      for (i = obj_size / sizeof (COPYING_UNIT) - 1;
	   i >= 0; i--)
	((COPYING_UNIT *)new_chunk->contents)[i]
	  = ((COPYING_UNIT *)h->object_base)[i];
      already = obj_size / sizeof (COPYING_UNIT) * sizeof (COPYING_UNIT);
    }
  else
    already = 0;
  for (i = already; i < obj_size; i++)
    new_chunk->contents[i] = h->object_base[i];

  if (h->object_base == old_chunk->contents && ! h->maybe_empty_object)
    {
      new_chunk->prev = old_chunk->prev;
      CALL_FREEFUN (h, old_chunk);
    }

  h->object_base = new_chunk->contents;
  h->next_free = h->object_base + obj_size;
  h->maybe_empty_object = 0;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE__obstack_chunk:[0-9]+]] _obstack_chunk = struct {
// DEFAULT-NEXT:         field0 limit: ptr<i8>;
// DEFAULT-NEXT:         field1 prev: ptr<@type[[TYPE__obstack_chunk]]>;
// DEFAULT-NEXT:         field2 contents: array<i8, 4>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_obstack:[0-9]+]] obstack = struct {
// DEFAULT-NEXT:         field0 chunk_size: i64;
// DEFAULT-NEXT:         field1 chunk: ptr<@type[[TYPE__obstack_chunk]]>;
// DEFAULT-NEXT:         field2 object_base: ptr<i8>;
// DEFAULT-NEXT:         field3 next_free: ptr<i8>;
// DEFAULT-NEXT:         field4 chunk_limit: ptr<i8>;
// DEFAULT-NEXT:         field5 temp: i64;
// DEFAULT-NEXT:         field6 alignment_mask: i32;
// DEFAULT-NEXT:         field7 chunkfun: ptr<fn(ptr<void>, i64) -> ptr<@type[[TYPE__obstack_chunk]]>>;
// DEFAULT-NEXT:         field8 freefun: ptr<fn(ptr<void>, ptr<@type[[TYPE__obstack_chunk]]>) -> void>;
// DEFAULT-NEXT:         field9 extra_arg: ptr<void>;
// DEFAULT-NEXT:         field10 use_extra_arg: u32 : 1;
// DEFAULT-NEXT:         field11 maybe_empty_object: u32 : 1;
// DEFAULT-NEXT:         field12 alloc_failed: u32 : 1;
// DEFAULT-NEXT:     } [size=88, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 80, 80], bit_offsets=[None, None, None, None, None, None, None, None, None, None, Some(640), Some(641), Some(642)], bit_units=[(80, 1)], field_units=[None, None, None, None, None, None, None, None, None, None, Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_fooalign:[0-9]+]] fooalign = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_fooround:[0-9]+]] fooround = union {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE__obstack_newchunk:[0-9]+]] @_obstack_newchunk(%[[VALUE_h:[0-9]+]] h: ptr<@type[[TYPE_obstack]]>, %[[VALUE_length:[0-9]+]] length: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_old_chunk:[0-9]+]] old_chunk: ptr<@type[[TYPE__obstack_chunk]]> [storage=automatic] = read<ptr<@type[[TYPE__obstack_chunk]]>>(field1(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]]))));
// DEFAULT-NEXT:         let %[[VALUE_new_chunk:[0-9]+]] new_chunk: ptr<@type[[TYPE__obstack_chunk]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_new_size:[0-9]+]] new_size: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_obj_size:[0-9]+]] obj_size: i64 [storage=automatic] = ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]])))), read<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]])))));
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_already:[0-9]+]] already: i64 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(%[[VALUE_new_size]], add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(read<i64>(%[[VALUE_obj_size]]), widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_length]]))), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_obj_size]]), const<i32>(3))), widen<i64, reason=usual_arith>(const<i32>(100))));
// DEFAULT-NEXT:         if lt<i64>(read<i64>(%[[VALUE_new_size]]), read<i64>(field0(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]])))))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_new_size]], read<i64>(field0(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]])))));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<@type[[TYPE__obstack_chunk]]> [synthetic];
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield10<unit=0, bytes=80..81, bits=0..1>(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]]))))), const<i32>(0))
// DEFAULT-NEXT:             write<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE0]], call<ptr<@type[[TYPE__obstack_chunk]]>, signature=fn(ptr<void>, i64) -> ptr<@type[[TYPE__obstack_chunk]]>>(read<ptr<fn(ptr<void>, i64) -> ptr<@type[[TYPE__obstack_chunk]]>>>(field7(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]])))), read<ptr<void>>(field9(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]])))), read<i64>(%[[VALUE_new_size]])));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE0]], call<ptr<@type[[TYPE__obstack_chunk]]>, signature=fn(i64) -> ptr<@type[[TYPE__obstack_chunk]]>>(pointer_cast<ptr<fn(i64) -> ptr<@type[[TYPE__obstack_chunk]]>>, reason=explicit>(read<ptr<fn(ptr<void>, i64) -> ptr<@type[[TYPE__obstack_chunk]]>>>(field7(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]]))))), read<i64>(%[[VALUE_new_size]])));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE_new_chunk]], read<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE0]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE__obstack_chunk]]>>(field1(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]]))), read<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE_new_chunk]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE__obstack_chunk]]>>(field1(deref(read<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE_new_chunk]]))), read<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE_old_chunk]]));
// DEFAULT-NEXT:         write<ptr<i8>>(field4(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]]))), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE_new_chunk]])), read<i64>(%[[VALUE_new_size]])));
// DEFAULT-NEXT:         write<ptr<i8>>(field0(deref(read<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE_new_chunk]]))), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE_new_chunk]])), read<i64>(%[[VALUE_new_size]])));
// DEFAULT-NEXT:         if ge<i64>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(read<i32>(field6(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]])))), const<i32>(1))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<f64>>(field1(deref(null<ptr<@type[[TYPE_fooalign]]>>)))), null<ptr<i8>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i64>(%[[VALUE_i]], reinterpret<i64, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(div<u64, by_zero=ub>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_obj_size]])), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:                     condition: ge<i64>(read<i64>(%[[VALUE_i]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE2:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                         let %[[VALUE3:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE2]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                         write<i64>(%[[VALUE_i]], read<i64>(%[[VALUE3]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(pointer_cast<ptr<i32>, reason=explicit>(array_decay<ptr<i8>, length=Some(4)>(field2(deref(read<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE_new_chunk]]))))), read<i64>(%[[VALUE_i]]))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]]))))), read<i64>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_already]], reinterpret<i64, reason=assign, fits=unknown>(mul<u64, overflow=wrap>(div<u64, by_zero=ub>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_obj_size]])), const<u64>(4)), const<u64>(4))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i64>(%[[VALUE_already]], widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_i]], read<i64>(%[[VALUE_already]]));
// DEFAULT-NEXT:             condition: lt<i64>(read<i64>(%[[VALUE_i]]), read<i64>(%[[VALUE_obj_size]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE5]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_i]], read<i64>(%[[VALUE6]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(field2(deref(read<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE_new_chunk]])))), read<i64>(%[[VALUE_i]]))), read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]])))), read<i64>(%[[VALUE_i]])))));
// DEFAULT-NEXT:         if logical_and<bool>(eq<ptr<i8>>(read<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]])))), array_decay<ptr<i8>, length=Some(4)>(field2(deref(read<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE_old_chunk]]))))), not<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield11<unit=0, bytes=80..81, bits=1..2>(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]]))))), const<i32>(0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE__obstack_chunk]]>>(field1(deref(read<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE_new_chunk]]))), read<ptr<@type[[TYPE__obstack_chunk]]>>(field1(deref(read<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE_old_chunk]])))));
// DEFAULT-NEXT:                 do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield10<unit=0, bytes=80..81, bits=0..1>(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]]))))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn(ptr<void>, ptr<@type[[TYPE__obstack_chunk]]>) -> void>(read<ptr<fn(ptr<void>, ptr<@type[[TYPE__obstack_chunk]]>) -> void>>(field8(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]])))), read<ptr<void>>(field9(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]])))), read<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE_old_chunk]]));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             call<void, signature=fn(ptr<void>) -> void>(pointer_cast<ptr<fn(ptr<void>) -> void>, reason=explicit>(read<ptr<fn(ptr<void>, ptr<@type[[TYPE__obstack_chunk]]>) -> void>>(field8(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]]))))), pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE_old_chunk]])));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]]))), array_decay<ptr<i8>, length=Some(4)>(field2(deref(read<ptr<@type[[TYPE__obstack_chunk]]>>(%[[VALUE_new_chunk]])))));
// DEFAULT-NEXT:         write<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]]))), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]])))), read<i64>(%[[VALUE_obj_size]])));
// DEFAULT-NEXT:         write<u32>(bitfield11<unit=0, bytes=80..81, bits=1..2>(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE_h]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
