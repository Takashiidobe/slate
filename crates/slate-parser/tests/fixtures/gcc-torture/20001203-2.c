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


// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "abort",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 0,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "exit",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Integer(
// DEFAULT-NEXT:                           Ranked {
// DEFAULT-NEXT:                               rank: Int,
// DEFAULT-NEXT:                               signed: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 1,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "obstack",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Long,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "chunk_size",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 4,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Tagged {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: Some(
// DEFAULT-NEXT:                                       "_obstack_chunk",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "chunk",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 5,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Char {
// DEFAULT-NEXT:                                       signed: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "object_base",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 6,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Char {
// DEFAULT-NEXT:                                       signed: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "next_free",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 7,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Char {
// DEFAULT-NEXT:                                       signed: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "chunk_limit",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 8,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "alignment_mask",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 9,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: false,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "maybe_empty_object",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 10,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 3,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[3]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "objfile",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Tagged {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: Some(
// DEFAULT-NEXT:                                       "objfile",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "next",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 14,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Tagged {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: Some(
// DEFAULT-NEXT:                                       "obstack",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "type_obstack",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 15,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 13,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[4]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "type",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: false,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "length",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 19,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Tagged {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: Some(
// DEFAULT-NEXT:                                       "objfile",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "objfile",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 20,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Short,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "nfields",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 21,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Tagged {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: Some(
// DEFAULT-NEXT:                                       "field",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   body: Some(
// DEFAULT-NEXT:                                       Fields(
// DEFAULT-NEXT:                                           [
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Tagged {
// DEFAULT-NEXT:                                                               kind: Union,
// DEFAULT-NEXT:                                                               name: Some(
// DEFAULT-NEXT:                                                                   "field_location",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               body: Some(
// DEFAULT-NEXT:                                                                   Fields(
// DEFAULT-NEXT:                                                                       [
// DEFAULT-NEXT:                                                                           FieldDecl {
// DEFAULT-NEXT:                                                                               declaration: Declaration {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                                           Ranked {
// DEFAULT-NEXT:                                                                                               rank: Int,
// DEFAULT-NEXT:                                                                                               signed: true,
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Name(
// DEFAULT-NEXT:                                                                                       "bitpos",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       0,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: 0,
// DEFAULT-NEXT:                                                                                   header: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           FieldDecl {
// DEFAULT-NEXT:                                                                               declaration: Declaration {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                                           Ranked {
// DEFAULT-NEXT:                                                                                               rank: Long,
// DEFAULT-NEXT:                                                                                               signed: false,
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Name(
// DEFAULT-NEXT:                                                                                       "physaddr",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       0,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: 0,
// DEFAULT-NEXT:                                                                                   header: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           FieldDecl {
// DEFAULT-NEXT:                                                                               declaration: Declaration {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                                           Char {
// DEFAULT-NEXT:                                                                                               signed: None,
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Pointer {
// DEFAULT-NEXT:                                                                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                                       inner: Name(
// DEFAULT-NEXT:                                                                                           "physname",
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               provenance: Provenance {
// DEFAULT-NEXT:                                                                                   file: FileId(
// DEFAULT-NEXT:                                                                                       0,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   kind: System,
// DEFAULT-NEXT:                                                                                   line: 0,
// DEFAULT-NEXT:                                                                                   header: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "loc",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Integer(
// DEFAULT-NEXT:                                                               Ranked {
// DEFAULT-NEXT:                                                                   rank: Int,
// DEFAULT-NEXT:                                                                   signed: true,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "bitsize",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Tagged {
// DEFAULT-NEXT:                                                               kind: Struct,
// DEFAULT-NEXT:                                                               name: Some(
// DEFAULT-NEXT:                                                                   "type",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Pointer {
// DEFAULT-NEXT:                                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                           inner: Name(
// DEFAULT-NEXT:                                                               "type",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               FieldDecl {
// DEFAULT-NEXT:                                                   declaration: Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Integer(
// DEFAULT-NEXT:                                                               Char {
// DEFAULT-NEXT:                                                                   signed: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarator: Pointer {
// DEFAULT-NEXT:                                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                           inner: Name(
// DEFAULT-NEXT:                                                               "name",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   provenance: Provenance {
// DEFAULT-NEXT:                                                       file: FileId(
// DEFAULT-NEXT:                                                           0,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       kind: System,
// DEFAULT-NEXT:                                                       line: 0,
// DEFAULT-NEXT:                                                       header: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "fields",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 22,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 18,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[5]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tagged {
// DEFAULT-NEXT:                   kind: Struct,
// DEFAULT-NEXT:                   name: Some(
// DEFAULT-NEXT:                       "type",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Pointer {
// DEFAULT-NEXT:                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                   inner: Name(
// DEFAULT-NEXT:                       "alloc_type",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 34,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[6]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Pointer {
// DEFAULT-NEXT:                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                   inner: Name(
// DEFAULT-NEXT:                       "xmalloc",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Integer(
// DEFAULT-NEXT:                           Ranked {
// DEFAULT-NEXT:                               rank: Int,
// DEFAULT-NEXT:                               signed: false,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Name(
// DEFAULT-NEXT:                               "z",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 35,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[7]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "_obstack_newchunk",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Tagged {
// DEFAULT-NEXT:                           kind: Struct,
// DEFAULT-NEXT:                           name: Some(
// DEFAULT-NEXT:                               "obstack",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "o",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Integer(
// DEFAULT-NEXT:                           Ranked {
// DEFAULT-NEXT:                               rank: Int,
// DEFAULT-NEXT:                               signed: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Name(
// DEFAULT-NEXT:                               "i",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 36,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[8]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "get_discrete_bounds",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Integer(
// DEFAULT-NEXT:                           Ranked {
// DEFAULT-NEXT:                               rank: LongLong,
// DEFAULT-NEXT:                               signed: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "lowp",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Integer(
// DEFAULT-NEXT:                           Ranked {
// DEFAULT-NEXT:                               rank: LongLong,
// DEFAULT-NEXT:                               signed: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "highp",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 37,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[9]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Pointer {
// DEFAULT-NEXT:                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                   inner: Name(
// DEFAULT-NEXT:                       "memset",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Void,
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Abstract,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Integer(
// DEFAULT-NEXT:                           Ranked {
// DEFAULT-NEXT:                               rank: Int,
// DEFAULT-NEXT:                               signed: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Integer(
// DEFAULT-NEXT:                           Ranked {
// DEFAULT-NEXT:                               rank: Long,
// DEFAULT-NEXT:                               signed: false,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 39,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[10]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Pointer {
// DEFAULT-NEXT:               pointee: Tagged {
// DEFAULT-NEXT:                   kind: Struct,
// DEFAULT-NEXT:                   name: Some(
// DEFAULT-NEXT:                       "type",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           name: "create_array_type",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Tagged {
// DEFAULT-NEXT:                       kind: Struct,
// DEFAULT-NEXT:                       name: Some(
// DEFAULT-NEXT:                           "type",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "result_type",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Tagged {
// DEFAULT-NEXT:                       kind: Struct,
// DEFAULT-NEXT:                       name: Some(
// DEFAULT-NEXT:                           "type",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "element_type",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Block(
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: LongLong,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "low_bound",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: LongLong,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "high_bound",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "result_type",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Cast {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               value: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "result_type",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "alloc_type",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "get_discrete_bounds",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "low_bound",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "high_bound",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Arrow {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "result_type",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "length",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Mul,
// DEFAULT-NEXT:                               left: Arrow {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "element_type",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   field: "length",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Add,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "high_bound",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Identifier(
// DEFAULT-NEXT:                                           "low_bound",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Integer(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Arrow {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "result_type",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "nfields",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               1,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Arrow {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "result_type",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "fields",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Cast {
// DEFAULT-NEXT:                               ty: Tagged {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: Some(
// DEFAULT-NEXT:                                       "field",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               value: Ternary {
// DEFAULT-NEXT:                                   condition: Binary {
// DEFAULT-NEXT:                                       op: NotEqual,
// DEFAULT-NEXT:                                       left: Arrow {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "result_type",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           field: "objfile",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Cast {
// DEFAULT-NEXT:                                           ty: Void,
// DEFAULT-NEXT:                                           declarator: Pointer {
// DEFAULT-NEXT:                                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                                               inner: Abstract,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           value: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_value: StatementExpression(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Struct,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "obstack",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__h",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Amp,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "result_type",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "objfile",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "type_obstack",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           LBrace,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Struct,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "obstack",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__h",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Int,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__len",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Sizeof,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Struct,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "field",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               If,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "chunk_limit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Minus,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "next_free",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Less,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__len",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "_obstack_newchunk",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Comma,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__len",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "next_free",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           PlusEqual,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__len",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Void,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "0",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           RBrace,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           LBrace,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Struct,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "obstack",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__h",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Void,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "value",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "value",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Void,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "object_base",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               If,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "next_free",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           EqualEqual,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "value",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "maybe_empty_object",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "next_free",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "next_free",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Minus,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Char,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "0",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Plus,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "alignment_mask",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Amp,
// DEFAULT-NEXT:                                           Tilde,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "alignment_mask",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Plus,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Char,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "0",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               If,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "next_free",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Minus,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Char,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "chunk",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Greater,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "chunk_limit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Minus,
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Char,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Star,
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "chunk",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "next_free",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "chunk_limit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "object_base",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "__o1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Arrow,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "next_free",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "value",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           RBrace,
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   else_value: Call {
// DEFAULT-NEXT:                                       callee: Identifier(
// DEFAULT-NEXT:                                           "xmalloc",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       arguments: [
// DEFAULT-NEXT:                                           SizeOfType {
// DEFAULT-NEXT:                                               ty: Tagged {
// DEFAULT-NEXT:                                                   kind: Struct,
// DEFAULT-NEXT:                                                   name: Some(
// DEFAULT-NEXT:                                                       "field",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "result_type",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 41,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[11]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Pointer {
// DEFAULT-NEXT:               pointee: Tagged {
// DEFAULT-NEXT:                   kind: Struct,
// DEFAULT-NEXT:                   name: Some(
// DEFAULT-NEXT:                       "type",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           name: "alloc_type",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "abort",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 85,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[12]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Pointer {
// DEFAULT-NEXT:               pointee: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           name: "xmalloc",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "z",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 86,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[13]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "_obstack_newchunk",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Tagged {
// DEFAULT-NEXT:                       kind: Struct,
// DEFAULT-NEXT:                       name: Some(
// DEFAULT-NEXT:                           "obstack",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "o",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "i",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "abort",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 87,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[14]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "get_discrete_bounds",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: LongLong,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "lowp",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: LongLong,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "highp",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Deref(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "lowp",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Deref(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "highp",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               2,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 88,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[15]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Struct,
// DEFAULT-NEXT:                               name: Some(
// DEFAULT-NEXT:                                   "type",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "element_type",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Struct,
// DEFAULT-NEXT:                               name: Some(
// DEFAULT-NEXT:                                   "type",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "result_type",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "memset",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "element_type",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               SizeOfType {
// DEFAULT-NEXT:                                   ty: Tagged {
// DEFAULT-NEXT:                                       kind: Struct,
// DEFAULT-NEXT:                                       name: Some(
// DEFAULT-NEXT:                                           "type",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "memset",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "result_type",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               SizeOfType {
// DEFAULT-NEXT:                                   ty: Tagged {
// DEFAULT-NEXT:                                       kind: Struct,
// DEFAULT-NEXT:                                       name: Some(
// DEFAULT-NEXT:                                           "type",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Member {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "element_type",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "length",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               4,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "create_array_type",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "result_type",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               AddrOf(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "element_type",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Member {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "result_type",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "length",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               12,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "exit",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 93,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
