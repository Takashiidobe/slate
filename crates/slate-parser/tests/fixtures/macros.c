#define VALUE 7
#define TRANSITIVE VALUE
#define ID(x) x
#define JOIN(a, b) a ## b

int ordinary = TRANSITIVE;

#define SELF SELF
int recursive __attribute__((slate_literal(SELF)));

#ifdef SELECT
#define CHOICE 1
int selected __attribute__((slate_literal(CHOICE)));
#else
#define CHOICE 2
int selected __attribute__((slate_literal(CHOICE)));
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES SELECT SELECT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "ordinary",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 7,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "7",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "recursive",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   attributes: [
// DEFAULT-NEXT:                       Unknown {
// DEFAULT-NEXT:                           name: "slate_literal",
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               "SELF",
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "selected",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   attributes: [
// DEFAULT-NEXT:                       Unknown {
// DEFAULT-NEXT:                           name: "slate_literal",
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               "2",
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN SELECT
// SELECT: decl[0]: Declaration(
// SELECT-NEXT:       Declaration {
// SELECT-NEXT:           specifiers: DeclarationSpecifiers {
// SELECT-NEXT:               ty: Integer(
// SELECT-NEXT:                   Ranked {
// SELECT-NEXT:                       rank: Int,
// SELECT-NEXT:                       signed: true,
// SELECT-NEXT:                   },
// SELECT-NEXT:               ),
// SELECT-NEXT:           },
// SELECT-NEXT:           declarators: [
// SELECT-NEXT:               InitDeclaratorKind {
// SELECT-NEXT:                   declarator: Name(
// SELECT-NEXT:                       "ordinary",
// SELECT-NEXT:                   ),
// SELECT-NEXT:                   initializer: Some(
// SELECT-NEXT:                       Expr(
// SELECT-NEXT:                           IntegerLiteral(
// SELECT-NEXT:                               IntegerLiteral {
// SELECT-NEXT:                                   value: 7,
// SELECT-NEXT:                                   radix: Decimal,
// SELECT-NEXT:                                   suffix: IntegerSuffix {
// SELECT-NEXT:                                       unsigned: false,
// SELECT-NEXT:                                       size: None,
// SELECT-NEXT:                                   },
// SELECT-NEXT:                                   spelling: "7",
// SELECT-NEXT:                               },
// SELECT-NEXT:                           ),
// SELECT-NEXT:                       ),
// SELECT-NEXT:                   ),
// SELECT-NEXT:               },
// SELECT-NEXT:           ],
// SELECT-NEXT:       },
// SELECT-NEXT:   )
// SELECT-NEXT: decl[1]: Declaration(
// SELECT-NEXT:       Declaration {
// SELECT-NEXT:           specifiers: DeclarationSpecifiers {
// SELECT-NEXT:               ty: Integer(
// SELECT-NEXT:                   Ranked {
// SELECT-NEXT:                       rank: Int,
// SELECT-NEXT:                       signed: true,
// SELECT-NEXT:                   },
// SELECT-NEXT:               ),
// SELECT-NEXT:           },
// SELECT-NEXT:           declarators: [
// SELECT-NEXT:               InitDeclaratorKind {
// SELECT-NEXT:                   declarator: Name(
// SELECT-NEXT:                       "recursive",
// SELECT-NEXT:                   ),
// SELECT-NEXT:                   attributes: [
// SELECT-NEXT:                       Unknown {
// SELECT-NEXT:                           name: "slate_literal",
// SELECT-NEXT:                           arguments: [
// SELECT-NEXT:                               "SELF",
// SELECT-NEXT:                           ],
// SELECT-NEXT:                       },
// SELECT-NEXT:                   ],
// SELECT-NEXT:               },
// SELECT-NEXT:           ],
// SELECT-NEXT:       },
// SELECT-NEXT:   )
// SELECT-NEXT: decl[2]: Declaration(
// SELECT-NEXT:       Declaration {
// SELECT-NEXT:           specifiers: DeclarationSpecifiers {
// SELECT-NEXT:               ty: Integer(
// SELECT-NEXT:                   Ranked {
// SELECT-NEXT:                       rank: Int,
// SELECT-NEXT:                       signed: true,
// SELECT-NEXT:                   },
// SELECT-NEXT:               ),
// SELECT-NEXT:           },
// SELECT-NEXT:           declarators: [
// SELECT-NEXT:               InitDeclaratorKind {
// SELECT-NEXT:                   declarator: Name(
// SELECT-NEXT:                       "selected",
// SELECT-NEXT:                   ),
// SELECT-NEXT:                   attributes: [
// SELECT-NEXT:                       Unknown {
// SELECT-NEXT:                           name: "slate_literal",
// SELECT-NEXT:                           arguments: [
// SELECT-NEXT:                               "1",
// SELECT-NEXT:                           ],
// SELECT-NEXT:                       },
// SELECT-NEXT:                   ],
// SELECT-NEXT:               },
// SELECT-NEXT:           ],
// SELECT-NEXT:       },
// SELECT-NEXT:   )
// SLATE-FILECHECK-END SELECT
