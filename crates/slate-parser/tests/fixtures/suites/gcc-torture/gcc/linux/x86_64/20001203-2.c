void abort(void);
void exit(int);

struct obstack {
  long                   chunk_size;
  struct _obstack_chunk *chunk;
  char                  *object_base;
  char                  *next_free;
  char                  *chunk_limit;
  int                    alignment_mask;
  unsigned               maybe_empty_object;
};

struct objfile {
  struct objfile *next;
  struct obstack  type_obstack;
};

struct type {
  unsigned        length;
  struct objfile *objfile;
  short           nfields;
  struct field {
    union field_location {
      int           bitpos;
      unsigned long physaddr;
      char         *physname;
    } loc;
    int          bitsize;
    struct type *type;
    char        *name;
  } *fields;
};

struct type *alloc_type(void);
void        *xmalloc(unsigned int z);
void         _obstack_newchunk(struct obstack *o, int i);
void         get_discrete_bounds(long long *lowp, long long *highp);

extern void *memset(void *, int, __SIZE_TYPE__);

struct type *create_array_type(struct type *result_type,
                               struct type *element_type) {
  long long low_bound, high_bound;
  if (result_type == ((void *)0)) {
    result_type = alloc_type();
  }
  get_discrete_bounds(&low_bound, &high_bound);
  (result_type)->length = (element_type)->length * (high_bound - low_bound + 1);
  (result_type)->nfields = 1;
  (result_type)->fields =
      (struct field *)((result_type)->objfile != ((void *)0)
                           ? ({
                               struct obstack *__h =
                                   (&(result_type)->objfile->type_obstack);
                               {
                                 struct obstack *__o = (__h);
                                 int __len           = ((sizeof(struct field)));
                                 if (__o->chunk_limit - __o->next_free < __len)
                                   _obstack_newchunk(__o, __len);
                                 __o->next_free += __len;
                                 (void)0;
                               };
                               ({
                                 struct obstack *__o1 = (__h);
                                 void           *value;
                                 value = (void *)__o1->object_base;
                                 if (__o1->next_free == value)
                                   __o1->maybe_empty_object = 1;
                                 __o1->next_free =
                                     (((((__o1->next_free) - (char *)0) +
                                        __o1->alignment_mask) &
                                       ~(__o1->alignment_mask)) +
                                      (char *)0);
                                 if (__o1->next_free - (char *)__o1->chunk >
                                     __o1->chunk_limit - (char *)__o1->chunk)
                                   __o1->next_free = __o1->chunk_limit;
                                 __o1->object_base = __o1->next_free;
                                 value;
                               });
                             })
                           : xmalloc(sizeof(struct field)));
  return (result_type);
}

struct type *alloc_type(void) { abort(); }
void        *xmalloc(unsigned int z) { return 0; }
void         _obstack_newchunk(struct obstack *o, int i) { abort(); }
void         get_discrete_bounds(long long *lowp, long long *highp) {
  *lowp  = 0;
  *highp = 2;
}

int main(void) {
  struct type element_type;
  struct type result_type;

  memset(&element_type, 0, sizeof(struct type));
  memset(&result_type, 0, sizeof(struct type));
  element_type.length = 4;
  create_array_type(&result_type, &element_type);
  if (result_type.length != 12)
    abort();
  exit(0);
}



// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     type @type0 obstack = struct {
// DEFAULT-NEXT:         field0 chunk_size: i64;
// DEFAULT-NEXT:         field1 chunk: ptr<@type1>;
// DEFAULT-NEXT:         field2 object_base: ptr<i8>;
// DEFAULT-NEXT:         field3 next_free: ptr<i8>;
// DEFAULT-NEXT:         field4 chunk_limit: ptr<i8>;
// DEFAULT-NEXT:         field5 alignment_mask: i32;
// DEFAULT-NEXT:         field6 maybe_empty_object: u32;
// DEFAULT-NEXT:     } [size=48, align=8, offsets=[0, 8, 16, 24, 32, 40, 44]];
// DEFAULT-NEXT:     type @type1 _obstack_chunk = struct incomplete;
// DEFAULT-NEXT:     type @type2 objfile = struct {
// DEFAULT-NEXT:         field0 next: ptr<@type2>;
// DEFAULT-NEXT:         field1 type_obstack: @type0;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type3 type = struct {
// DEFAULT-NEXT:         field0 length: u32;
// DEFAULT-NEXT:         field1 objfile: ptr<@type2>;
// DEFAULT-NEXT:         field2 nfields: i16;
// DEFAULT-NEXT:         field3 fields: ptr<@type4>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type4 field = struct {
// DEFAULT-NEXT:         field0 loc: @type5;
// DEFAULT-NEXT:         field1 bitsize: i32;
// DEFAULT-NEXT:         field2 type: ptr<@type3>;
// DEFAULT-NEXT:         field3 name: ptr<i8>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type5 field_location = union {
// DEFAULT-NEXT:         field0 bitpos: i32;
// DEFAULT-NEXT:         field1 physaddr: u64;
// DEFAULT-NEXT:         field2 physname: ptr<i8>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%36 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @alloc_type() -> ptr<@type3> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @xmalloc(%28 z: u32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @_obstack_newchunk(%29 o: ptr<@type0>, %30 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @get_discrete_bounds(%31 lowp: ptr<i64>, %32 highp: ptr<i64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%31)), widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%32)), widen<i64, reason=assign>(const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @memset(%42 <unnamed>: ptr<void>, %43 <unnamed>: i32, %44 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %18 @create_array_type(%19 result_type: ptr<@type3>, %20 element_type: ptr<@type3>) -> ptr<@type3> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %21 low_bound: i64 [storage=automatic];
// DEFAULT-NEXT:         let %22 high_bound: i64 [storage=automatic];
// DEFAULT-NEXT:         if eq<ptr<@type3>>(read<ptr<@type3>>(%19), null<ptr<@type3>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<@type3>>(%19, call<ptr<@type3>, signature=fn() -> ptr<@type3>>(%8));
// DEFAULT-NEXT:                 call<ptr<@type3>, signature=fn() -> ptr<@type3>>(%8);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i64>, ptr<i64>) -> void>(%16, addr_of<ptr<i64>>(%21), addr_of<ptr<i64>>(%22));
// DEFAULT-NEXT:         write<u32>(field0(deref(read<ptr<@type3>>(%19))), reinterpret<u32, reason=assign, fits=unknown>(truncate<i32, reason=assign, fits=unknown>(mul<i64, overflow=ub>(reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(read<u32>(field0(deref(read<ptr<@type3>>(%20)))))), add<i64, overflow=ub>(sub<i64, overflow=ub>(read<i64>(%22), read<i64>(%21)), widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         write<i16>(field2(deref(read<ptr<@type3>>(%19))), truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %45: ptr<void> [synthetic];
// DEFAULT-NEXT:         if ne<ptr<@type2>>(read<ptr<@type2>>(field1(deref(read<ptr<@type3>>(%19)))), null<ptr<@type2>>)
// DEFAULT-NEXT:             let %46: ptr<void> [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %23 __h: ptr<@type0> [storage=automatic] = addr_of<ptr<@type0>>(field1(deref(read<ptr<@type2>>(field1(deref(read<ptr<@type3>>(%19)))))));
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %24 __o: ptr<@type0> [storage=automatic] = read<ptr<@type0>>(%23);
// DEFAULT-NEXT:                     let %25 __len: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(32)));
// DEFAULT-NEXT:                     if lt<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field4(deref(read<ptr<@type0>>(%24)))), read<ptr<i8>>(field3(deref(read<ptr<@type0>>(%24))))), widen<i64, reason=usual_arith>(read<i32>(%25)))
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<@type0>, i32) -> void>(%13, read<ptr<@type0>>(%24), read<i32>(%25));
// DEFAULT-NEXT:                     let %47: ptr<@type0> [synthetic] = read<ptr<@type0>>(%24);
// DEFAULT-NEXT:                     let %48: ptr<i8> [synthetic] = read<ptr<i8>>(field3(deref(read<ptr<@type0>>(%47))));
// DEFAULT-NEXT:                     let %49: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%48), read<i32>(%25));
// DEFAULT-NEXT:                     write<ptr<i8>>(field3(deref(read<ptr<@type0>>(%47))), read<ptr<i8>>(%49));
// DEFAULT-NEXT:                     const<i32>(0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 let %50: ptr<void> [synthetic];
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %26 __o1: ptr<@type0> [storage=automatic] = read<ptr<@type0>>(%23);
// DEFAULT-NEXT:                     let %27 value: ptr<void> [storage=automatic];
// DEFAULT-NEXT:                     write<ptr<void>>(%27, pointer_cast<ptr<void>, reason=explicit>(read<ptr<i8>>(field2(deref(read<ptr<@type0>>(%26))))));
// DEFAULT-NEXT:                     if eq<ptr<i8>>(read<ptr<i8>>(field3(deref(read<ptr<@type0>>(%26)))), pointer_cast<ptr<i8>, reason=usual_arith>(read<ptr<void>>(%27)))
// DEFAULT-NEXT:                         write<u32>(field6(deref(read<ptr<@type0>>(%26))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<ptr<i8>>(field3(deref(read<ptr<@type0>>(%26))), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(null<ptr<i8>>, and<i64>(add<i64, overflow=ub>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type0>>(%26)))), null<ptr<i8>>), widen<i64, reason=usual_arith>(read<i32>(field5(deref(read<ptr<@type0>>(%26)))))), widen<i64, reason=usual_arith>(not<i32>(read<i32>(field5(deref(read<ptr<@type0>>(%26)))))))));
// DEFAULT-NEXT:                     if gt<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type0>>(%26)))), pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type1>>(field1(deref(read<ptr<@type0>>(%26)))))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field4(deref(read<ptr<@type0>>(%26)))), pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type1>>(field1(deref(read<ptr<@type0>>(%26)))))))
// DEFAULT-NEXT:                         write<ptr<i8>>(field3(deref(read<ptr<@type0>>(%26))), read<ptr<i8>>(field4(deref(read<ptr<@type0>>(%26)))));
// DEFAULT-NEXT:                     write<ptr<i8>>(field2(deref(read<ptr<@type0>>(%26))), read<ptr<i8>>(field3(deref(read<ptr<@type0>>(%26)))));
// DEFAULT-NEXT:                     write<ptr<void>>(%50, read<ptr<void>>(%27));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 write<ptr<void>>(%46, read<ptr<void>>(%50));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<ptr<void>>(%45, read<ptr<void>>(%46));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<ptr<void>>(%45, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%10, truncate<u32, reason=arg, fits=always>(const<u64>(32))));
// DEFAULT-NEXT:         write<ptr<@type4>>(field3(deref(read<ptr<@type3>>(%19))), pointer_cast<ptr<@type4>, reason=explicit>(read<ptr<void>>(%45)));
// DEFAULT-NEXT:         return read<ptr<@type3>>(%19);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %34 element_type: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %35 result_type: @type3 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%17, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type3>>(%34)), const<i32>(0), const<u64>(32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%17, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type3>>(%35)), const<i32>(0), const<u64>(32));
// DEFAULT-NEXT:         write<u32>(field0(%34), reinterpret<u32, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         call<ptr<@type3>, signature=fn(ptr<@type3>, ptr<@type3>) -> ptr<@type3>>(%18, addr_of<ptr<@type3>>(%35), addr_of<ptr<@type3>>(%34));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(field0(%35)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
