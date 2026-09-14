void exit(int);

#define L 1
int main(void) { exit(L'1' != L'1'); }

// SLATE-FILECHECK-DEFINES GCC

// SLATE-FILECHECK-BEGIN GCC
// GCC: decl[0]: Declaration {
// GCC-NEXT:       declaration: Declaration {
// GCC-NEXT:           specifiers: DeclarationSpecifiers {
// GCC-NEXT:               ty: Void,
// GCC-NEXT:           },
// GCC-NEXT:           declarators: [
// GCC-NEXT:               InitDeclarator {
// GCC-NEXT:                   declarator: Function {
// GCC-NEXT:                       inner: Name(
// GCC-NEXT:                           "exit",
// GCC-NEXT:                       ),
// GCC-NEXT:                       parameters: Prototype {
// GCC-NEXT:                           parameters: [
// GCC-NEXT:                               ParameterDeclaration {
// GCC-NEXT:                                   specifiers: DeclarationSpecifiers {
// GCC-NEXT:                                       ty: Integer(
// GCC-NEXT:                                           Ranked {
// GCC-NEXT:                                               rank: Int,
// GCC-NEXT:                                               signed: true,
// GCC-NEXT:                                           },
// GCC-NEXT:                                       ),
// GCC-NEXT:                                   },
// GCC-NEXT:                                   declarator: Abstract,
// GCC-NEXT:                               },
// GCC-NEXT:                           ],
// GCC-NEXT:                       },
// GCC-NEXT:                   },
// GCC-NEXT:               },
// GCC-NEXT:           ],
// GCC-NEXT:       },
// GCC-NEXT:       provenance: Provenance {
// GCC-NEXT:           file: FileId(
// GCC-NEXT:               3,
// GCC-NEXT:           ),
// GCC-NEXT:           kind: User,
// GCC-NEXT:           line: 0,
// GCC-NEXT:           header: None,
// GCC-NEXT:       },
// GCC-NEXT:   }
// GCC-NEXT: decl[1]: Function(
// GCC-NEXT:       FunctionDefinition {
// GCC-NEXT:           specifiers: DeclarationSpecifiers {
// GCC-NEXT:               ty: Integer(
// GCC-NEXT:                   Ranked {
// GCC-NEXT:                       rank: Int,
// GCC-NEXT:                       signed: true,
// GCC-NEXT:                   },
// GCC-NEXT:               ),
// GCC-NEXT:           },
// GCC-NEXT:           declarator: Function {
// GCC-NEXT:               inner: Name(
// GCC-NEXT:                   "main",
// GCC-NEXT:               ),
// GCC-NEXT:               parameters: Void,
// GCC-NEXT:           },
// GCC-NEXT:           body: [
// GCC-NEXT:               Expr(
// GCC-NEXT:                   Call {
// GCC-NEXT:                       callee: Identifier(
// GCC-NEXT:                           "exit",
// GCC-NEXT:                       ),
// GCC-NEXT:                       arguments: [
// GCC-NEXT:                           Binary {
// GCC-NEXT:                               op: NotEqual,
// GCC-NEXT:                               left: CharLiteral(
// GCC-NEXT:                                   CharLiteral {
// GCC-NEXT:                                       encoding: Wide,
// GCC-NEXT:                                       code_units: [
// GCC-NEXT:                                           49,
// GCC-NEXT:                                       ],
// GCC-NEXT:                                       spelling: "1",
// GCC-NEXT:                                   },
// GCC-NEXT:                               ),
// GCC-NEXT:                               right: CharLiteral(
// GCC-NEXT:                                   CharLiteral {
// GCC-NEXT:                                       encoding: Wide,
// GCC-NEXT:                                       code_units: [
// GCC-NEXT:                                           49,
// GCC-NEXT:                                       ],
// GCC-NEXT:                                       spelling: "1",
// GCC-NEXT:                                   },
// GCC-NEXT:                               ),
// GCC-NEXT:                           },
// GCC-NEXT:                       ],
// GCC-NEXT:                   },
// GCC-NEXT:               ),
// GCC-NEXT:           ],
// GCC-NEXT:           provenance: Provenance {
// GCC-NEXT:               file: FileId(
// GCC-NEXT:                   3,
// GCC-NEXT:               ),
// GCC-NEXT:               kind: User,
// GCC-NEXT:               line: 3,
// GCC-NEXT:               header: None,
// GCC-NEXT:           },
// GCC-NEXT:       },
// GCC-NEXT:   )
// SLATE-FILECHECK-END GCC
