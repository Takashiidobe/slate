#ifdef SELECT
#define VALUE 1
#else
#define VALUE 2
#endif

#ifdef SELECT
#define PICK(value) value
#else
#define PICK(value) 4
#endif

#ifdef SELECT
#define TYPE int
#else
#define TYPE char
#endif

int selected = VALUE;
TYPE typed;

int picked(void) {
    return PICK(3);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES SELECT SELECT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "selected",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 18,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "typed",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 19,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "picked",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           4,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 21,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN SELECT
// SELECT: decl[0]: Declaration {
// SELECT-NEXT:       declaration: Declaration {
// SELECT-NEXT:           specifiers: DeclarationSpecifiers {
// SELECT-NEXT:               ty: Integer(
// SELECT-NEXT:                   Ranked {
// SELECT-NEXT:                       rank: Int,
// SELECT-NEXT:                       signed: true,
// SELECT-NEXT:                   },
// SELECT-NEXT:               ),
// SELECT-NEXT:           },
// SELECT-NEXT:           declarators: [
// SELECT-NEXT:               InitDeclarator {
// SELECT-NEXT:                   declarator: Name(
// SELECT-NEXT:                       "selected",
// SELECT-NEXT:                   ),
// SELECT-NEXT:                   initializer: Some(
// SELECT-NEXT:                       Expr(
// SELECT-NEXT:                           Const(
// SELECT-NEXT:                               Integer(
// SELECT-NEXT:                                   1,
// SELECT-NEXT:                               ),
// SELECT-NEXT:                           ),
// SELECT-NEXT:                       ),
// SELECT-NEXT:                   ),
// SELECT-NEXT:               },
// SELECT-NEXT:           ],
// SELECT-NEXT:       },
// SELECT-NEXT:       provenance: Provenance {
// SELECT-NEXT:           file: FileId(
// SELECT-NEXT:               3,
// SELECT-NEXT:           ),
// SELECT-NEXT:           kind: User,
// SELECT-NEXT:           line: 18,
// SELECT-NEXT:           header: None,
// SELECT-NEXT:       },
// SELECT-NEXT:   }
// SELECT-NEXT: decl[1]: Declaration {
// SELECT-NEXT:       declaration: Declaration {
// SELECT-NEXT:           specifiers: DeclarationSpecifiers {
// SELECT-NEXT:               ty: Integer(
// SELECT-NEXT:                   Ranked {
// SELECT-NEXT:                       rank: Int,
// SELECT-NEXT:                       signed: true,
// SELECT-NEXT:                   },
// SELECT-NEXT:               ),
// SELECT-NEXT:           },
// SELECT-NEXT:           declarators: [
// SELECT-NEXT:               InitDeclarator {
// SELECT-NEXT:                   declarator: Name(
// SELECT-NEXT:                       "typed",
// SELECT-NEXT:                   ),
// SELECT-NEXT:               },
// SELECT-NEXT:           ],
// SELECT-NEXT:       },
// SELECT-NEXT:       provenance: Provenance {
// SELECT-NEXT:           file: FileId(
// SELECT-NEXT:               3,
// SELECT-NEXT:           ),
// SELECT-NEXT:           kind: User,
// SELECT-NEXT:           line: 19,
// SELECT-NEXT:           header: None,
// SELECT-NEXT:       },
// SELECT-NEXT:   }
// SELECT-NEXT: decl[2]: Function(
// SELECT-NEXT:       FunctionDecl {
// SELECT-NEXT:           ret_type: Integer(
// SELECT-NEXT:               Ranked {
// SELECT-NEXT:                   rank: Int,
// SELECT-NEXT:                   signed: true,
// SELECT-NEXT:               },
// SELECT-NEXT:           ),
// SELECT-NEXT:           name: "picked",
// SELECT-NEXT:           body: [
// SELECT-NEXT:               Return(
// SELECT-NEXT:                   Const(
// SELECT-NEXT:                       Integer(
// SELECT-NEXT:                           3,
// SELECT-NEXT:                       ),
// SELECT-NEXT:                   ),
// SELECT-NEXT:               ),
// SELECT-NEXT:           ],
// SELECT-NEXT:           provenance: Provenance {
// SELECT-NEXT:               file: FileId(
// SELECT-NEXT:                   3,
// SELECT-NEXT:               ),
// SELECT-NEXT:               kind: User,
// SELECT-NEXT:               line: 21,
// SELECT-NEXT:               header: None,
// SELECT-NEXT:           },
// SELECT-NEXT:       },
// SELECT-NEXT:   )
// SLATE-FILECHECK-END SELECT
