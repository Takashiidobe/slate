/* PR tree-optimization/102622 */
/* Wrong code introduced due to phi-opt
   introducing undefined signed interger overflow
   with one bit signed integer negation. */

struct f {
  signed t : 1;
};
int g(struct f *a, int t) __attribute__((noipa));
int g(struct f *a, int t) {
  if (t)
    a->t = -1;
  else
    a->t = 0;
  int t1 = a->t;
  if (t1)
    return 1;
  return t1;
}

int main(void) {
  struct f a;
  if (!g(&a, 1))
    __builtin_abort();
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: tag[0]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           0,
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Struct,
// DEFAULT-NEXT:       name: Some(
// DEFAULT-NEXT:           "f",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 5,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[0]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
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
// DEFAULT-NEXT: decl[1]: Declaration {
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
// DEFAULT-NEXT:                           "g",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: Prototype {
// DEFAULT-NEXT:                           parameters: [
// DEFAULT-NEXT:                               ParameterDeclaration {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Tag(
// DEFAULT-NEXT:                                           Reference {
// DEFAULT-NEXT:                                               kind: Struct,
// DEFAULT-NEXT:                                               name: "f",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "a",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               ParameterDeclaration {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Int,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarator: Name(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   attributes: [
// DEFAULT-NEXT:                       NoIpa,
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 8,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Function(
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
// DEFAULT-NEXT:                   "g",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Prototype {
// DEFAULT-NEXT:                   parameters: [
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Tag(
// DEFAULT-NEXT:                                   Reference {
// DEFAULT-NEXT:                                       kind: Struct,
// DEFAULT-NEXT:                                       name: "f",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "t",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Identifier(
// DEFAULT-NEXT:                       "t",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Member {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "a",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   field: "t",
// DEFAULT-NEXT:                                   arrow: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               value: Unary {
// DEFAULT-NEXT:                                   op: Minus,
// DEFAULT-NEXT:                                   operand: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: Some(
// DEFAULT-NEXT:                       [
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Member {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "a",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "t",
// DEFAULT-NEXT:                                       arrow: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   value: IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 0,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "0",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
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
// DEFAULT-NEXT:                                   "t1",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Member {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "a",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           field: "t",
// DEFAULT-NEXT:                                           arrow: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Identifier(
// DEFAULT-NEXT:                       "t1",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 1,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "1",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Identifier(
// DEFAULT-NEXT:                       "t1",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT: decl[3]: Function(
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
// DEFAULT-NEXT:                   "main",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: "f",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Unary {
// DEFAULT-NEXT:                       op: Not,
// DEFAULT-NEXT:                       operand: Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "g",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Unary {
// DEFAULT-NEXT:                                   op: AddrOf,
// DEFAULT-NEXT:                                   operand: Identifier(
// DEFAULT-NEXT:                                       "a",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 1,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "1",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_abort",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
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
// DEFAULT-NEXT:               line: 20,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
