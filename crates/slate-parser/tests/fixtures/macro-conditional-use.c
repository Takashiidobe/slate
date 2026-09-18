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
// DEFAULT: decl[{{[0-9]+}}]: Declaration(
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
// DEFAULT-NEXT:                   initializer: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 2,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "2",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Char {
// DEFAULT-NEXT:                       signed: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "typed",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "picked",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 4,
// DEFAULT-NEXT:                           radix: Decimal,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "4",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN SELECT
// SELECT: decl[{{[0-9]+}}]: Declaration(
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
// SELECT-NEXT:                   initializer: Some(
// SELECT-NEXT:                       Expr(
// SELECT-NEXT:                           IntegerLiteral(
// SELECT-NEXT:                               IntegerLiteral {
// SELECT-NEXT:                                   value: 1,
// SELECT-NEXT:                                   radix: Decimal,
// SELECT-NEXT:                                   suffix: IntegerSuffix {
// SELECT-NEXT:                                       unsigned: false,
// SELECT-NEXT:                                       size: None,
// SELECT-NEXT:                                   },
// SELECT-NEXT:                                   spelling: "1",
// SELECT-NEXT:                               },
// SELECT-NEXT:                           ),
// SELECT-NEXT:                       ),
// SELECT-NEXT:                   ),
// SELECT-NEXT:               },
// SELECT-NEXT:           ],
// SELECT-NEXT:       },
// SELECT-NEXT:   )
// SELECT-NEXT: decl[{{[0-9]+}}]: Declaration(
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
// SELECT-NEXT:                       "typed",
// SELECT-NEXT:                   ),
// SELECT-NEXT:               },
// SELECT-NEXT:           ],
// SELECT-NEXT:       },
// SELECT-NEXT:   )
// SELECT-NEXT: decl[{{[0-9]+}}]: Function(
// SELECT-NEXT:       FunctionDefinition {
// SELECT-NEXT:           specifiers: DeclarationSpecifiers {
// SELECT-NEXT:               ty: Integer(
// SELECT-NEXT:                   Ranked {
// SELECT-NEXT:                       rank: Int,
// SELECT-NEXT:                       signed: true,
// SELECT-NEXT:                   },
// SELECT-NEXT:               ),
// SELECT-NEXT:           },
// SELECT-NEXT:           declarator: Function {
// SELECT-NEXT:               inner: Name(
// SELECT-NEXT:                   "picked",
// SELECT-NEXT:               ),
// SELECT-NEXT:               parameters: Void,
// SELECT-NEXT:           },
// SELECT-NEXT:           body: [
// SELECT-NEXT:               Return(
// SELECT-NEXT:                   IntegerLiteral(
// SELECT-NEXT:                       IntegerLiteral {
// SELECT-NEXT:                           value: 3,
// SELECT-NEXT:                           radix: Decimal,
// SELECT-NEXT:                           suffix: IntegerSuffix {
// SELECT-NEXT:                               unsigned: false,
// SELECT-NEXT:                               size: None,
// SELECT-NEXT:                           },
// SELECT-NEXT:                           spelling: "3",
// SELECT-NEXT:                       },
// SELECT-NEXT:                   ),
// SELECT-NEXT:               ),
// SELECT-NEXT:           ],
// SELECT-NEXT:       },
// SELECT-NEXT:   )
// SLATE-FILECHECK-END SELECT
