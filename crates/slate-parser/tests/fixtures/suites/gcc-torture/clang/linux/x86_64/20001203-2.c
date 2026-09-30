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
// DEFAULT-NEXT:     type @type[[TYPE_obstack:[0-9]+]] obstack = struct {
// DEFAULT-NEXT:         field0 chunk_size: i64;
// DEFAULT-NEXT:         field1 chunk: ptr<@type[[TYPE__obstack_chunk:[0-9]+]]>;
// DEFAULT-NEXT:         field2 object_base: ptr<i8>;
// DEFAULT-NEXT:         field3 next_free: ptr<i8>;
// DEFAULT-NEXT:         field4 chunk_limit: ptr<i8>;
// DEFAULT-NEXT:         field5 alignment_mask: i32;
// DEFAULT-NEXT:         field6 maybe_empty_object: u32;
// DEFAULT-NEXT:     } [size=48, align=8, offsets=[0, 8, 16, 24, 32, 40, 44]];
// DEFAULT-NEXT:     type @type[[TYPE__obstack_chunk]] _obstack_chunk = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_objfile:[0-9]+]] objfile = struct {
// DEFAULT-NEXT:         field0 next: ptr<@type[[TYPE_objfile]]>;
// DEFAULT-NEXT:         field1 type_obstack: @type[[TYPE_obstack]];
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_type:[0-9]+]] type = struct {
// DEFAULT-NEXT:         field0 length: u32;
// DEFAULT-NEXT:         field1 objfile: ptr<@type[[TYPE_objfile]]>;
// DEFAULT-NEXT:         field2 nfields: i16;
// DEFAULT-NEXT:         field3 fields: ptr<@type[[TYPE_field:[0-9]+]]>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_field]] field = struct {
// DEFAULT-NEXT:         field0 loc: @type[[TYPE_field_location:[0-9]+]];
// DEFAULT-NEXT:         field1 bitsize: i32;
// DEFAULT-NEXT:         field2 type: ptr<@type[[TYPE_type]]>;
// DEFAULT-NEXT:         field3 name: ptr<i8>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_field_location]] field_location = union {
// DEFAULT-NEXT:         field0 bitpos: i32;
// DEFAULT-NEXT:         field1 physaddr: u64;
// DEFAULT-NEXT:         field2 physname: ptr<i8>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_alloc_type:[0-9]+]] @alloc_type() -> ptr<@type[[TYPE_type]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_xmalloc:[0-9]+]] @xmalloc(%[[VALUE_z:[0-9]+]] z: u32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE__obstack_newchunk:[0-9]+]] @_obstack_newchunk(%[[VALUE_o:[0-9]+]] o: ptr<@type[[TYPE_obstack]]>, %[[VALUE_i:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_get_discrete_bounds:[0-9]+]] @get_discrete_bounds(%[[VALUE_lowp:[0-9]+]] lowp: ptr<i64>, %[[VALUE_highp:[0-9]+]] highp: ptr<i64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%[[VALUE_lowp]])), widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%[[VALUE_highp]])), widen<i64, reason=assign>(const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: i32, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_create_array_type:[0-9]+]] @create_array_type(%[[VALUE_result_type:[0-9]+]] result_type: ptr<@type[[TYPE_type]]>, %[[VALUE_element_type:[0-9]+]] element_type: ptr<@type[[TYPE_type]]>) -> ptr<@type[[TYPE_type]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_low_bound:[0-9]+]] low_bound: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_high_bound:[0-9]+]] high_bound: i64 [storage=automatic];
// DEFAULT-NEXT:         if eq<ptr<@type[[TYPE_type]]>>(read<ptr<@type[[TYPE_type]]>>(%[[VALUE_result_type]]), null<ptr<@type[[TYPE_type]]>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_type]]>>(%[[VALUE_result_type]], call<ptr<@type[[TYPE_type]]>, signature=fn() -> ptr<@type[[TYPE_type]]>>(%[[VALUE_alloc_type]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i64>, ptr<i64>) -> void>(%[[VALUE_get_discrete_bounds]], addr_of<ptr<i64>>(%[[VALUE_low_bound]]), addr_of<ptr<i64>>(%[[VALUE_high_bound]]));
// DEFAULT-NEXT:         write<u32>(field0(deref(read<ptr<@type[[TYPE_type]]>>(%[[VALUE_result_type]]))), reinterpret<u32, reason=assign, fits=unknown>(truncate<i32, reason=assign, fits=unknown>(mul<i64, overflow=ub>(reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(read<u32>(field0(deref(read<ptr<@type[[TYPE_type]]>>(%[[VALUE_element_type]])))))), add<i64, overflow=ub>(sub<i64, overflow=ub>(read<i64>(%[[VALUE_high_bound]]), read<i64>(%[[VALUE_low_bound]])), widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         write<i16>(field2(deref(read<ptr<@type[[TYPE_type]]>>(%[[VALUE_result_type]]))), truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: ptr<void> [synthetic];
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_objfile]]>>(read<ptr<@type[[TYPE_objfile]]>>(field1(deref(read<ptr<@type[[TYPE_type]]>>(%[[VALUE_result_type]])))), null<ptr<@type[[TYPE_objfile]]>>)
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: ptr<void> [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___h:[0-9]+]] __h: ptr<@type[[TYPE_obstack]]> [storage=automatic] = addr_of<ptr<@type[[TYPE_obstack]]>>(field1(deref(read<ptr<@type[[TYPE_objfile]]>>(field1(deref(read<ptr<@type[[TYPE_type]]>>(%[[VALUE_result_type]])))))));
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE___o:[0-9]+]] __o: ptr<@type[[TYPE_obstack]]> [storage=automatic] = read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___h]]);
// DEFAULT-NEXT:                     let %[[VALUE___len:[0-9]+]] __len: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(32)));
// DEFAULT-NEXT:                     if lt<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field4(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o]])))), read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o]]))))), widen<i64, reason=usual_arith>(read<i32>(%[[VALUE___len]])))
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<@type[[TYPE_obstack]]>, i32) -> void>(%[[VALUE__obstack_newchunk]], read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o]]), read<i32>(%[[VALUE___len]]));
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: ptr<@type[[TYPE_obstack]]> [synthetic] = read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o]]);
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE6]]))));
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE7]]), read<i32>(%[[VALUE___len]]));
// DEFAULT-NEXT:                     write<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE6]]))), read<ptr<i8>>(%[[VALUE8]]));
// DEFAULT-NEXT:                     const<i32>(0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: ptr<void> [synthetic];
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE___o1:[0-9]+]] __o1: ptr<@type[[TYPE_obstack]]> [storage=automatic] = read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___h]]);
// DEFAULT-NEXT:                     let %[[VALUE_value:[0-9]+]] value: ptr<void> [storage=automatic];
// DEFAULT-NEXT:                     write<ptr<void>>(%[[VALUE_value]], pointer_cast<ptr<void>, reason=explicit>(read<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]]))))));
// DEFAULT-NEXT:                     if eq<ptr<i8>>(read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))), pointer_cast<ptr<i8>, reason=usual_arith>(read<ptr<void>>(%[[VALUE_value]])))
// DEFAULT-NEXT:                         write<u32>(field6(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]]))), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(null<ptr<i8>>, and<i64>(add<i64, overflow=ub>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))), null<ptr<i8>>), widen<i64, reason=usual_arith>(read<i32>(field5(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))))), widen<i64, reason=usual_arith>(not<i32>(read<i32>(field5(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))))))));
// DEFAULT-NEXT:                     if gt<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))), pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type[[TYPE__obstack_chunk]]>>(field1(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field4(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))), pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type[[TYPE__obstack_chunk]]>>(field1(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))))))
// DEFAULT-NEXT:                         write<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]]))), read<ptr<i8>>(field4(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))));
// DEFAULT-NEXT:                     write<ptr<i8>>(field2(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]]))), read<ptr<i8>>(field3(deref(read<ptr<@type[[TYPE_obstack]]>>(%[[VALUE___o1]])))));
// DEFAULT-NEXT:                     write<ptr<void>>(%[[VALUE9]], read<ptr<void>>(%[[VALUE_value]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 write<ptr<void>>(%[[VALUE5]], read<ptr<void>>(%[[VALUE9]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE4]], read<ptr<void>>(%[[VALUE5]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE4]], call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_xmalloc]], truncate<u32, reason=arg, fits=always>(const<u64>(32))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_field]]>>(field3(deref(read<ptr<@type[[TYPE_type]]>>(%[[VALUE_result_type]]))), pointer_cast<ptr<@type[[TYPE_field]]>, reason=explicit>(read<ptr<void>>(%[[VALUE4]])));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_type]]>>(%[[VALUE_result_type]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_element_type_2:[0-9]+]] element_type: @type[[TYPE_type]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_result_type_2:[0-9]+]] result_type: @type[[TYPE_type]] [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_type]]>>(%[[VALUE_element_type_2]])), const<i32>(0), const<u64>(32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_type]]>>(%[[VALUE_result_type_2]])), const<i32>(0), const<u64>(32));
// DEFAULT-NEXT:         write<u32>(field0(%[[VALUE_element_type_2]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         call<ptr<@type[[TYPE_type]]>, signature=fn(ptr<@type[[TYPE_type]]>, ptr<@type[[TYPE_type]]>) -> ptr<@type[[TYPE_type]]>>(%[[VALUE_create_array_type]], addr_of<ptr<@type[[TYPE_type]]>>(%[[VALUE_result_type_2]]), addr_of<ptr<@type[[TYPE_type]]>>(%[[VALUE_element_type_2]]));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(field0(%[[VALUE_result_type_2]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
