#include <stdio.h>

int main(void) {
  int      d = 5;
  unsigned h = 0xAB;
  printf("{%d} %% \"quoted\" back\\slash %s|%c|%x\n", d, "hi", 'X', h);
  printf("}}%%{{%d}}\n", d);
  printf("%%%%%d%%%%\n", d);
  printf("{{}}%s{{}}\n", "mid");
  return 0;
}




// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "printf",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Qualified {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers {
// DEFAULT-NEXT:                                       is_const: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers {
// DEFAULT-NEXT:                                           is_restrict: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       inner: Abstract,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       variadic: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               4,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: System,
// DEFAULT-NEXT:           line: 170,
// DEFAULT-NEXT:           header: Some(
// DEFAULT-NEXT:               FileId(
// DEFAULT-NEXT:                   4,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Function(
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
// DEFAULT-NEXT:                                   "d",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 5,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "5",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "h",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 171,
// DEFAULT-NEXT:                                               radix: Hex,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "0xAB",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "printf",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           StringLiteral(
// DEFAULT-NEXT:                               StringLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [
// DEFAULT-NEXT:                                       123,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       100,
// DEFAULT-NEXT:                                       125,
// DEFAULT-NEXT:                                       32,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       32,
// DEFAULT-NEXT:                                       34,
// DEFAULT-NEXT:                                       113,
// DEFAULT-NEXT:                                       117,
// DEFAULT-NEXT:                                       111,
// DEFAULT-NEXT:                                       116,
// DEFAULT-NEXT:                                       101,
// DEFAULT-NEXT:                                       100,
// DEFAULT-NEXT:                                       34,
// DEFAULT-NEXT:                                       32,
// DEFAULT-NEXT:                                       98,
// DEFAULT-NEXT:                                       97,
// DEFAULT-NEXT:                                       99,
// DEFAULT-NEXT:                                       107,
// DEFAULT-NEXT:                                       92,
// DEFAULT-NEXT:                                       115,
// DEFAULT-NEXT:                                       108,
// DEFAULT-NEXT:                                       97,
// DEFAULT-NEXT:                                       115,
// DEFAULT-NEXT:                                       104,
// DEFAULT-NEXT:                                       32,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       115,
// DEFAULT-NEXT:                                       124,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       99,
// DEFAULT-NEXT:                                       124,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       120,
// DEFAULT-NEXT:                                       10,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   pieces: [
// DEFAULT-NEXT:                                       "{%d} %% \\\"quoted\\\" back\\\\slash %s|%c|%x\\n",
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "d",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           StringLiteral(
// DEFAULT-NEXT:                               StringLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [
// DEFAULT-NEXT:                                       104,
// DEFAULT-NEXT:                                       105,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   pieces: [
// DEFAULT-NEXT:                                       "hi",
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           CharLiteral(
// DEFAULT-NEXT:                               CharLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [
// DEFAULT-NEXT:                                       88,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   spelling: "X",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "h",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "printf",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           StringLiteral(
// DEFAULT-NEXT:                               StringLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [
// DEFAULT-NEXT:                                       125,
// DEFAULT-NEXT:                                       125,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       123,
// DEFAULT-NEXT:                                       123,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       100,
// DEFAULT-NEXT:                                       125,
// DEFAULT-NEXT:                                       125,
// DEFAULT-NEXT:                                       10,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   pieces: [
// DEFAULT-NEXT:                                       "{{[}][}]}}%%{{\{\{}}%d{{[}][}]}}\\n",
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "d",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "printf",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           StringLiteral(
// DEFAULT-NEXT:                               StringLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       100,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       10,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   pieces: [
// DEFAULT-NEXT:                                       "%%%%%d%%%%\\n",
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "d",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "printf",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           StringLiteral(
// DEFAULT-NEXT:                               StringLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [
// DEFAULT-NEXT:                                       123,
// DEFAULT-NEXT:                                       123,
// DEFAULT-NEXT:                                       125,
// DEFAULT-NEXT:                                       125,
// DEFAULT-NEXT:                                       37,
// DEFAULT-NEXT:                                       115,
// DEFAULT-NEXT:                                       123,
// DEFAULT-NEXT:                                       123,
// DEFAULT-NEXT:                                       125,
// DEFAULT-NEXT:                                       125,
// DEFAULT-NEXT:                                       10,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   pieces: [
// DEFAULT-NEXT:                                       "{{\{\{}}{{[}][}]}}%s{{\{\{}}{{[}][}]}}\\n",
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           StringLiteral(
// DEFAULT-NEXT:                               StringLiteral {
// DEFAULT-NEXT:                                   encoding: Plain,
// DEFAULT-NEXT:                                   code_units: [
// DEFAULT-NEXT:                                       109,
// DEFAULT-NEXT:                                       105,
// DEFAULT-NEXT:                                       100,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   pieces: [
// DEFAULT-NEXT:                                       "mid",
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 0,
// DEFAULT-NEXT:                           radix: Decimal,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "0",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 2,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
