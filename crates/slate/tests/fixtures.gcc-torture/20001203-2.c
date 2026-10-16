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

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn memset([[V_1:__v[0-9]+]]: *mut (), [[V_2:__v[0-9]+]]: i32, [[V_3:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct obstack {
// CHECK-NEXT:     chunk_size: i64,
// CHECK-NEXT:     chunk: *mut _obstack_chunk,
// CHECK-NEXT:     object_base: *mut i8,
// CHECK-NEXT:     next_free: *mut i8,
// CHECK-NEXT:     chunk_limit: *mut i8,
// CHECK-NEXT:     alignment_mask: i32,
// CHECK-NEXT:     maybe_empty_object: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _obstack_chunk {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct objfile {
// CHECK-NEXT:     next: *mut objfile,
// CHECK-NEXT:     type_obstack: obstack,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct r#type {
// CHECK-NEXT:     length: u32,
// CHECK-NEXT:     objfile: *mut objfile,
// CHECK-NEXT:     nfields: i16,
// CHECK-NEXT:     fields: *mut field,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct field {
// CHECK-NEXT:     loc: field_location,
// CHECK-NEXT:     bitsize: i32,
// CHECK-NEXT:     r#type: *mut r#type,
// CHECK-NEXT:     name: *mut i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union field_location {
// CHECK-NEXT:     bitpos: i32,
// CHECK-NEXT:     physaddr: u64,
// CHECK-NEXT:     physname: *mut i8,
// CHECK-NEXT: }
// CHECK-NEXT: fn create_array_type(
// CHECK-NEXT:     mut result_type: *mut r#type,
// CHECK-NEXT:     mut element_type: &r#type,
// CHECK-NEXT: ) -> *mut r#type {
// CHECK-NEXT:     let mut low_bound: i64 = 0 as i64;
// CHECK-NEXT:     let mut high_bound: i64 = 0 as i64;
// CHECK-NEXT:     if result_type == std::ptr::null_mut::<r#type>() {
// CHECK-NEXT:         result_type = alloc_type();
// CHECK-NEXT:     }
// CHECK-NEXT:     get_discrete_bounds(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(low_bound)) },
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(high_bound)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*result_type).length = (((((unsafe {
// CHECK-NEXT:             (*(element_type as *const r#type)).length
// CHECK-NEXT:         }) as u64) as i64) * (high_bound - low_bound + ((1 as i32) as i64))) as i32)
// CHECK-NEXT:             as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*result_type).nfields = (1 as i32) as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: *mut ();
// CHECK-NEXT:     if (unsafe { (*result_type).objfile }) != std::ptr::null_mut::<objfile>() {
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: *mut ();
// CHECK-NEXT:         let mut __h: *mut obstack = unsafe {
// CHECK-NEXT:             std::ptr::addr_of_mut!((* unsafe { (* result_type).objfile }).type_obstack)
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut __o: *mut obstack = __h;
// CHECK-NEXT:         let mut __len: i32 = ((32 as u64) as u32) as i32;
// CHECK-NEXT:         if ((unsafe {
// CHECK-NEXT:             unsafe { (*__o).chunk_limit }
// CHECK-NEXT:                 .offset_from((unsafe { (*__o).next_free }) as *mut i8)
// CHECK-NEXT:         }) as i64) < (__len as i64)
// CHECK-NEXT:         {
// CHECK-NEXT:             _obstack_newchunk(__o, __len);
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: *mut i8 = unsafe { (*__o).next_free };
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: *mut i8 = unsafe { [[V_6]].offset(__len as isize) };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*__o).next_free = [[V_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:         {}
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: *mut ();
// CHECK-NEXT:         let mut value: *mut () = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         value = (unsafe { (*__h).object_base }) as *mut ();
// CHECK-NEXT:         if (unsafe { (*__h).next_free }) == (value as *mut i8) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*__h).maybe_empty_object = (1 as i32) as u32;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*__h).next_free = unsafe {
// CHECK-NEXT:                 std::ptr::null_mut::<i8>()
// CHECK-NEXT:                     .offset(
// CHECK-NEXT:                         (((unsafe {
// CHECK-NEXT:                             unsafe { (*__h).next_free }
// CHECK-NEXT:                                 .offset_from(std::ptr::null_mut::<i8>() as *mut i8)
// CHECK-NEXT:                         }) as i64) + ((unsafe { (*__h).alignment_mask }) as i64)
// CHECK-NEXT:                             & (!unsafe { (*__h).alignment_mask } as i64)) as isize,
// CHECK-NEXT:                     )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         if ((unsafe {
// CHECK-NEXT:             unsafe { (*__h).next_free }.offset_from((unsafe { (*__h).chunk }) as *mut i8)
// CHECK-NEXT:         }) as i64)
// CHECK-NEXT:             > ((unsafe {
// CHECK-NEXT:                 unsafe { (*__h).chunk_limit }
// CHECK-NEXT:                     .offset_from((unsafe { (*__h).chunk }) as *mut i8)
// CHECK-NEXT:             }) as i64)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*__h).next_free = unsafe { (*__h).chunk_limit };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*__h).object_base = unsafe { (*__h).next_free };
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_8]] = value;
// CHECK-NEXT:         [[V_5]] = [[V_8]];
// CHECK-NEXT:         [[V_4]] = [[V_5]];
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_4]] = xmalloc((32 as u64) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*result_type).fields = [[V_4]] as *mut field;
// CHECK-NEXT:     }
// CHECK-NEXT:     result_type
// CHECK-NEXT: }
// CHECK-NEXT: fn alloc_type() -> *mut r#type {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn xmalloc(mut z: u32) -> *mut () {
// CHECK-NEXT:     std::ptr::null_mut::<()>()
// CHECK-NEXT: }
// CHECK-NEXT: fn _obstack_newchunk(mut o: *mut obstack, mut i: i32) {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT: }
// CHECK-NEXT: fn get_discrete_bounds(mut lowp: &mut i64, mut highp: &mut i64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(lowp as *mut i64) = (0 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(highp as *mut i64) = (2 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut element_type: r#type = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut result_type: r#type = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(std::ptr::addr_of_mut!(element_type) as *mut (), 0 as i32, 32 as u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(std::ptr::addr_of_mut!(result_type) as *mut (), 0 as i32, 32 as u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     element_type.length = (4 as i32) as u32;
// CHECK-NEXT:     create_array_type(
// CHECK-NEXT:         std::ptr::addr_of_mut!(result_type),
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(element_type)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     if result_type.length != ((12 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
