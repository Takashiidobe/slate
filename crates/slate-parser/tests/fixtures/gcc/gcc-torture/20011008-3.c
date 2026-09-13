/* { dg-add-options stack_size } */

extern void exit(int);
extern void abort(void);

typedef unsigned int  u_int32_t;
typedef unsigned char u_int8_t;
typedef int           int32_t;

typedef enum {
  TXNLIST_DELETE,
  TXNLIST_LSN,
  TXNLIST_TXNID,
  TXNLIST_PGNO
} db_txnlist_type;

struct __db_lsn;
typedef struct __db_lsn DB_LSN;
struct __db_lsn {
  u_int32_t file;
  u_int32_t offset;
};
struct __db_txnlist;
typedef struct __db_txnlist DB_TXNLIST;

struct __db_txnlist {
  db_txnlist_type type;
  struct {
    struct __db_txnlist  *le_next;
    struct __db_txnlist **le_prev;
  } links;
  union {
    struct {
      u_int32_t txnid;
      int32_t   generation;
      int32_t   aborted;
    } t;
    struct {

      u_int32_t flags;
      int32_t   fileid;
      u_int32_t count;
      char     *fname;
    } d;
    struct {
      int32_t ntxns;
      int32_t maxn;
      DB_LSN *lsn_array;
    } l;
    struct {
      int32_t  nentries;
      int32_t  maxentry;
      char    *fname;
      int32_t  fileid;
      void    *pgno_array;
      u_int8_t uid[20];
    } p;
  } u;
};

int log_compare(const DB_LSN *a, const DB_LSN *b) { return 1; }

int __db_txnlist_lsnadd(int val, DB_TXNLIST *elp, DB_LSN *lsnp,
                        u_int32_t flags) {
  int i;

  for (i = 0; i < (!(flags & (0x1)) ? 1 : elp->u.l.ntxns); i++) {
    int    __j;
    DB_LSN __tmp;
    val++;
    for (__j = 0; __j < elp->u.l.ntxns - 1; __j++)
      if (log_compare(&elp->u.l.lsn_array[__j], &elp->u.l.lsn_array[__j + 1]) <
          0) {
        __tmp                       = elp->u.l.lsn_array[__j];
        elp->u.l.lsn_array[__j]     = elp->u.l.lsn_array[__j + 1];
        elp->u.l.lsn_array[__j + 1] = __tmp;
      }
  }

  *lsnp = elp->u.l.lsn_array[0];
  return val;
}

#if defined(STACK_SIZE) && STACK_SIZE < 12350
#define VLEN (STACK_SIZE / 10)
#else
#define VLEN 1235
#endif

int main(void) {
  DB_TXNLIST el;
  DB_LSN     lsn, lsn_a[VLEN];

  el.u.l.ntxns     = VLEN - 1;
  el.u.l.lsn_array = lsn_a;

  if (__db_txnlist_lsnadd(0, &el, &lsn, 0) != 1)
    abort();

  if (__db_txnlist_lsnadd(0, &el, &lsn, 1) != VLEN - 1)
    abort();

  exit(0);
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment(
// DEFAULT-NEXT:       CommentGroup {
// DEFAULT-NEXT:           comments: [
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* { dg-add-options stack_size } */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 0,
// DEFAULT-NEXT:                       length: 35,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 0,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "exit",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 2,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "abort",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 3,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "u_int32_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 5,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: Some(
// DEFAULT-NEXT:                           false,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "u_int8_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 6,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[5]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "int32_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 7,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[6]: Enum(
// DEFAULT-NEXT:       EnumDecl {
// DEFAULT-NEXT:           name: None,
// DEFAULT-NEXT:           enumerators: [
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "TXNLIST_DELETE",
// DEFAULT-NEXT:                   value: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "TXNLIST_LSN",
// DEFAULT-NEXT:                   value: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "TXNLIST_TXNID",
// DEFAULT-NEXT:                   value: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Enumerator {
// DEFAULT-NEXT:                   name: "TXNLIST_PGNO",
// DEFAULT-NEXT:                   value: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 9,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[7]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tagged {
// DEFAULT-NEXT:                   kind: Enum,
// DEFAULT-NEXT:                   name: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "db_txnlist_type",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 9,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[8]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tagged {
// DEFAULT-NEXT:                   kind: Struct,
// DEFAULT-NEXT:                   name: Some(
// DEFAULT-NEXT:                       "__db_lsn",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 16,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[9]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tagged {
// DEFAULT-NEXT:                   kind: Struct,
// DEFAULT-NEXT:                   name: Some(
// DEFAULT-NEXT:                       "__db_lsn",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "DB_LSN",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 17,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[10]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "__db_lsn",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "u_int32_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "file",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "u_int32_t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "offset",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT: decl[11]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tagged {
// DEFAULT-NEXT:                   kind: Struct,
// DEFAULT-NEXT:                   name: Some(
// DEFAULT-NEXT:                       "__db_txnlist",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 22,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[12]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tagged {
// DEFAULT-NEXT:                   kind: Struct,
// DEFAULT-NEXT:                   name: Some(
// DEFAULT-NEXT:                       "__db_txnlist",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "DB_TXNLIST",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 23,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[13]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "__db_txnlist",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "db_txnlist_type",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "type",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 26,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Struct,
// DEFAULT-NEXT:                               name: None,
// DEFAULT-NEXT:                               body: Some(
// DEFAULT-NEXT:                                   Fields(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           FieldDecl {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Tagged {
// DEFAULT-NEXT:                                                       kind: Struct,
// DEFAULT-NEXT:                                                       name: Some(
// DEFAULT-NEXT:                                                           "__db_txnlist",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarators: [
// DEFAULT-NEXT:                                                   FieldDeclarator {
// DEFAULT-NEXT:                                                       declarator: Pointer {
// DEFAULT-NEXT:                                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                           inner: Name(
// DEFAULT-NEXT:                                                               "le_next",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: 0,
// DEFAULT-NEXT:                                                   header: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           FieldDecl {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Tagged {
// DEFAULT-NEXT:                                                       kind: Struct,
// DEFAULT-NEXT:                                                       name: Some(
// DEFAULT-NEXT:                                                           "__db_txnlist",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarators: [
// DEFAULT-NEXT:                                                   FieldDeclarator {
// DEFAULT-NEXT:                                                       declarator: Pointer {
// DEFAULT-NEXT:                                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                           inner: Pointer {
// DEFAULT-NEXT:                                                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                               inner: Name(
// DEFAULT-NEXT:                                                                   "le_prev",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: 0,
// DEFAULT-NEXT:                                                   header: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "links",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 27,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Union,
// DEFAULT-NEXT:                               name: None,
// DEFAULT-NEXT:                               body: Some(
// DEFAULT-NEXT:                                   Fields(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           FieldDecl {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Tagged {
// DEFAULT-NEXT:                                                       kind: Struct,
// DEFAULT-NEXT:                                                       name: None,
// DEFAULT-NEXT:                                                       body: Some(
// DEFAULT-NEXT:                                                           Fields(
// DEFAULT-NEXT:                                                               [
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Named(
// DEFAULT-NEXT:                                                                               "u_int32_t",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Name(
// DEFAULT-NEXT:                                                                                   "txnid",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Named(
// DEFAULT-NEXT:                                                                               "int32_t",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Name(
// DEFAULT-NEXT:                                                                                   "generation",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Named(
// DEFAULT-NEXT:                                                                               "int32_t",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Name(
// DEFAULT-NEXT:                                                                                   "aborted",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarators: [
// DEFAULT-NEXT:                                                   FieldDeclarator {
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "t",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: 0,
// DEFAULT-NEXT:                                                   header: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           FieldDecl {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Tagged {
// DEFAULT-NEXT:                                                       kind: Struct,
// DEFAULT-NEXT:                                                       name: None,
// DEFAULT-NEXT:                                                       body: Some(
// DEFAULT-NEXT:                                                           Fields(
// DEFAULT-NEXT:                                                               [
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Named(
// DEFAULT-NEXT:                                                                               "u_int32_t",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Name(
// DEFAULT-NEXT:                                                                                   "flags",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Named(
// DEFAULT-NEXT:                                                                               "int32_t",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Name(
// DEFAULT-NEXT:                                                                                   "fileid",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Named(
// DEFAULT-NEXT:                                                                               "u_int32_t",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Name(
// DEFAULT-NEXT:                                                                                   "count",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Integer(
// DEFAULT-NEXT:                                                                               Char {
// DEFAULT-NEXT:                                                                                   signed: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Pointer {
// DEFAULT-NEXT:                                                                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                                   inner: Name(
// DEFAULT-NEXT:                                                                                       "fname",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarators: [
// DEFAULT-NEXT:                                                   FieldDeclarator {
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "d",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: 0,
// DEFAULT-NEXT:                                                   header: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           FieldDecl {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Tagged {
// DEFAULT-NEXT:                                                       kind: Struct,
// DEFAULT-NEXT:                                                       name: None,
// DEFAULT-NEXT:                                                       body: Some(
// DEFAULT-NEXT:                                                           Fields(
// DEFAULT-NEXT:                                                               [
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Named(
// DEFAULT-NEXT:                                                                               "int32_t",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Name(
// DEFAULT-NEXT:                                                                                   "ntxns",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Named(
// DEFAULT-NEXT:                                                                               "int32_t",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Name(
// DEFAULT-NEXT:                                                                                   "maxn",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Named(
// DEFAULT-NEXT:                                                                               "DB_LSN",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Pointer {
// DEFAULT-NEXT:                                                                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                                   inner: Name(
// DEFAULT-NEXT:                                                                                       "lsn_array",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarators: [
// DEFAULT-NEXT:                                                   FieldDeclarator {
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "l",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: 0,
// DEFAULT-NEXT:                                                   header: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           FieldDecl {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Tagged {
// DEFAULT-NEXT:                                                       kind: Struct,
// DEFAULT-NEXT:                                                       name: None,
// DEFAULT-NEXT:                                                       body: Some(
// DEFAULT-NEXT:                                                           Fields(
// DEFAULT-NEXT:                                                               [
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Named(
// DEFAULT-NEXT:                                                                               "int32_t",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Name(
// DEFAULT-NEXT:                                                                                   "nentries",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Named(
// DEFAULT-NEXT:                                                                               "int32_t",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Name(
// DEFAULT-NEXT:                                                                                   "maxentry",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Integer(
// DEFAULT-NEXT:                                                                               Char {
// DEFAULT-NEXT:                                                                                   signed: None,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Pointer {
// DEFAULT-NEXT:                                                                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                                   inner: Name(
// DEFAULT-NEXT:                                                                                       "fname",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Named(
// DEFAULT-NEXT:                                                                               "int32_t",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Name(
// DEFAULT-NEXT:                                                                                   "fileid",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Void,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Pointer {
// DEFAULT-NEXT:                                                                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                                                                   inner: Name(
// DEFAULT-NEXT:                                                                                       "pgno_array",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Named(
// DEFAULT-NEXT:                                                                               "u_int8_t",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Array {
// DEFAULT-NEXT:                                                                                   inner: Name(
// DEFAULT-NEXT:                                                                                       "uid",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   size: Expression(
// DEFAULT-NEXT:                                                                                       IntLit(
// DEFAULT-NEXT:                                                                                           20,
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarators: [
// DEFAULT-NEXT:                                                   FieldDeclarator {
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "p",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: 0,
// DEFAULT-NEXT:                                                   header: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "u",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 31,
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
// DEFAULT-NEXT:               line: 25,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[14]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "log_compare",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Qualified {
// DEFAULT-NEXT:                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                           is_const: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       ty: Named(
// DEFAULT-NEXT:                           "DB_LSN",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Qualified {
// DEFAULT-NEXT:                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                           is_const: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       ty: Named(
// DEFAULT-NEXT:                           "DB_LSN",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           1,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 60,
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
// DEFAULT-NEXT:           name: "__db_txnlist_lsnadd",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "val",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "DB_TXNLIST",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "elp",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "DB_LSN",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "lsnp",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "u_int32_t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "flags",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               For {
// DEFAULT-NEXT:                   init: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "i",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   condition: Some(
// DEFAULT-NEXT:                       Const(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Less,
// DEFAULT-NEXT:                               left: Identifier(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Ternary {
// DEFAULT-NEXT:                                   condition: Unary {
// DEFAULT-NEXT:                                       op: Not,
// DEFAULT-NEXT:                                       value: Binary {
// DEFAULT-NEXT:                                           op: BitAnd,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "flags",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   then_value: Integer(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   else_value: Member {
// DEFAULT-NEXT:                                       base: Member {
// DEFAULT-NEXT:                                           base: Arrow {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "elp",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "u",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           field: "l",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       field: "ntxns",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   increment: Some(
// DEFAULT-NEXT:                       Const(
// DEFAULT-NEXT:                           PostIncrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "i",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "__j",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Named(
// DEFAULT-NEXT:                                       "DB_LSN",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "__tmp",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               PostIncrement(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "val",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       For {
// DEFAULT-NEXT:                           init: Some(
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "__j",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           condition: Some(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: Less,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "__j",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: Sub,
// DEFAULT-NEXT:                                           left: Member {
// DEFAULT-NEXT:                                               base: Member {
// DEFAULT-NEXT:                                                   base: Arrow {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "elp",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "u",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   field: "l",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               field: "ntxns",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           increment: Some(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   PostIncrement(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "__j",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: [
// DEFAULT-NEXT:                               If {
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Less,
// DEFAULT-NEXT:                                           left: Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "log_compare",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Member {
// DEFAULT-NEXT:                                                               base: Member {
// DEFAULT-NEXT:                                                                   base: Arrow {
// DEFAULT-NEXT:                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                           "elp",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       field: "u",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   field: "l",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               field: "lsn_array",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "__j",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   AddrOf(
// DEFAULT-NEXT:                                                       Index {
// DEFAULT-NEXT:                                                           base: Member {
// DEFAULT-NEXT:                                                               base: Member {
// DEFAULT-NEXT:                                                                   base: Arrow {
// DEFAULT-NEXT:                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                           "elp",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       field: "u",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   field: "l",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               field: "lsn_array",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Binary {
// DEFAULT-NEXT:                                                               op: Add,
// DEFAULT-NEXT:                                                               left: Identifier(
// DEFAULT-NEXT:                                                                   "__j",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   1,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   then_branch: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: Assign,
// DEFAULT-NEXT:                                                   target: Identifier(
// DEFAULT-NEXT:                                                       "__tmp",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   value: Index {
// DEFAULT-NEXT:                                                       base: Member {
// DEFAULT-NEXT:                                                           base: Member {
// DEFAULT-NEXT:                                                               base: Arrow {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "elp",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "u",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               field: "l",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           field: "lsn_array",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "__j",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: Assign,
// DEFAULT-NEXT:                                                   target: Index {
// DEFAULT-NEXT:                                                       base: Member {
// DEFAULT-NEXT:                                                           base: Member {
// DEFAULT-NEXT:                                                               base: Arrow {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "elp",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "u",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               field: "l",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           field: "lsn_array",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "__j",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   value: Index {
// DEFAULT-NEXT:                                                       base: Member {
// DEFAULT-NEXT:                                                           base: Member {
// DEFAULT-NEXT:                                                               base: Arrow {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "elp",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "u",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               field: "l",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           field: "lsn_array",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Binary {
// DEFAULT-NEXT:                                                           op: Add,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "__j",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: Assign,
// DEFAULT-NEXT:                                                   target: Index {
// DEFAULT-NEXT:                                                       base: Member {
// DEFAULT-NEXT:                                                           base: Member {
// DEFAULT-NEXT:                                                               base: Arrow {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "elp",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "u",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               field: "l",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           field: "lsn_array",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Binary {
// DEFAULT-NEXT:                                                           op: Add,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "__j",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   value: Identifier(
// DEFAULT-NEXT:                                                       "__tmp",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   else_branch: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Deref(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "lsnp",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Index {
// DEFAULT-NEXT:                               base: Member {
// DEFAULT-NEXT:                                   base: Member {
// DEFAULT-NEXT:                                       base: Arrow {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "elp",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           field: "u",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       field: "l",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   field: "lsn_array",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               index: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "val",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 62,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[16]: Function(
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
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "DB_TXNLIST",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "el",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "DB_LSN",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "lsn",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "lsn_a",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntLit(
// DEFAULT-NEXT:                                           1235,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Member {
// DEFAULT-NEXT:                               base: Member {
// DEFAULT-NEXT:                                   base: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "el",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "u",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   field: "l",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               field: "ntxns",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Sub,
// DEFAULT-NEXT:                               left: Integer(
// DEFAULT-NEXT:                                   1235,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Member {
// DEFAULT-NEXT:                               base: Member {
// DEFAULT-NEXT:                                   base: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "el",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "u",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   field: "l",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               field: "lsn_array",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Identifier(
// DEFAULT-NEXT:                               "lsn_a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__db_txnlist_lsnadd",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   AddrOf(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "el",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   AddrOf(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "lsn",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               1,
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
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__db_txnlist_lsnadd",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   AddrOf(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "el",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   AddrOf(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "lsn",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Binary {
// DEFAULT-NEXT:                               op: Sub,
// DEFAULT-NEXT:                               left: Integer(
// DEFAULT-NEXT:                                   1235,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   1,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
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
// DEFAULT-NEXT:               line: 89,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
