void associations(int n, int value) {
    _Generic(value, int: 1, int (*)[n]: 2);
    _Generic(value, int[*]: 1, default: 0);
    _Generic(value, int (*)[*]: 1, default: 0);
    _Generic(value, int[sizeof(int[*])]: 1, default: 0);
    _Generic(value, struct { int a[*]; }: 1, default: 0);
}

void controlling(int n) {
    int a[1];
    float b[1];
    typedef int V[n];
    _Generic(typeof(a), int[n++]: 0);
    _Generic(typeof(b), int[n++]: 0, default: 1);
    _Generic(V, int[n]: 0);
    _Generic(int (*)[n], int (*)[*]: 0);
}

// SLATE-FILECHECK-AST
// SLATE-FILECHECK-DEFINES C2Y
// SLATE-FILECHECK-STD C2Y c2y
// SLATE-FILECHECK-PREFIX-ARGS C2Y --parse-only
// SLATE-FILECHECK-DEFINES GNU2Y
// SLATE-FILECHECK-STD GNU2Y gnu2y
// SLATE-FILECHECK-PREFIX-ARGS GNU2Y --parse-only

// SLATE-FILECHECK-BEGIN C2Y
// C2Y: tag[{{[0-9]+}}]: TagDefinition {
// C2Y-NEXT:       id: TagId(
// C2Y-NEXT:           [[#TAG0:]],
// C2Y-NEXT:       ),
// C2Y-NEXT:       kind: Struct,
// C2Y-NEXT:       name: None,
// C2Y-NEXT:       body: Record(
// C2Y-NEXT:           [
// C2Y-NEXT:               Field(
// C2Y-NEXT:                   FieldDecl {
// C2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                           ty: Integer(
// C2Y-NEXT:                               Ranked {
// C2Y-NEXT:                                   rank: Int,
// C2Y-NEXT:                                   signed: true,
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                       declarators: [
// C2Y-NEXT:                           FieldDeclaratorKind {
// C2Y-NEXT:                               declarator: Array {
// C2Y-NEXT:                                   inner: Name(
// C2Y-NEXT:                                       "a",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   size: Star,
// C2Y-NEXT:                               },
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:           ],
// C2Y-NEXT:       ),
// C2Y-NEXT:   }
// C2Y-NEXT: decl[{{[0-9]+}}]: Function(
// C2Y-NEXT:       FunctionDefinition {
// C2Y-NEXT:           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:               ty: Void,
// C2Y-NEXT:           },
// C2Y-NEXT:           declarator: Function {
// C2Y-NEXT:               inner: Name(
// C2Y-NEXT:                   "associations",
// C2Y-NEXT:               ),
// C2Y-NEXT:               parameters: Prototype {
// C2Y-NEXT:                   parameters: [
// C2Y-NEXT:                       ParameterDeclarationKind {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   Ranked {
// C2Y-NEXT:                                       rank: Int,
// C2Y-NEXT:                                       signed: true,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Name(
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                       ParameterDeclarationKind {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   Ranked {
// C2Y-NEXT:                                       rank: Int,
// C2Y-NEXT:                                       signed: true,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Name(
// C2Y-NEXT:                               "value",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ],
// C2Y-NEXT:               },
// C2Y-NEXT:           },
// C2Y-NEXT:           body: [
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Generic {
// C2Y-NEXT:                       controlling: Expr(
// C2Y-NEXT:                           Identifier(
// C2Y-NEXT:                               "value",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       ),
// C2Y-NEXT:                       associations: [
// C2Y-NEXT:                           Type {
// C2Y-NEXT:                               ty: TypeName {
// C2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                       ty: Integer(
// C2Y-NEXT:                                           Ranked {
// C2Y-NEXT:                                               rank: Int,
// C2Y-NEXT:                                               signed: true,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   declarator: Abstract,
// C2Y-NEXT:                               },
// C2Y-NEXT:                               value: IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 1,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "1",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           Type {
// C2Y-NEXT:                               ty: TypeName {
// C2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                       ty: Integer(
// C2Y-NEXT:                                           Ranked {
// C2Y-NEXT:                                               rank: Int,
// C2Y-NEXT:                                               signed: true,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   declarator: Array {
// C2Y-NEXT:                                       inner: Grouped(
// C2Y-NEXT:                                           Pointer {
// C2Y-NEXT:                                               qualifiers: Qualifiers,
// C2Y-NEXT:                                               inner: Abstract,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                       size: Expression(
// C2Y-NEXT:                                           Identifier(
// C2Y-NEXT:                                               "n",
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               },
// C2Y-NEXT:                               value: IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 2,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "2",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Generic {
// C2Y-NEXT:                       controlling: Expr(
// C2Y-NEXT:                           Identifier(
// C2Y-NEXT:                               "value",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       ),
// C2Y-NEXT:                       associations: [
// C2Y-NEXT:                           Type {
// C2Y-NEXT:                               ty: TypeName {
// C2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                       ty: Integer(
// C2Y-NEXT:                                           Ranked {
// C2Y-NEXT:                                               rank: Int,
// C2Y-NEXT:                                               signed: true,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   declarator: Array {
// C2Y-NEXT:                                       inner: Abstract,
// C2Y-NEXT:                                       size: Star,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               },
// C2Y-NEXT:                               value: IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 1,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "1",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           Default(
// C2Y-NEXT:                               IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 0,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "0",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Generic {
// C2Y-NEXT:                       controlling: Expr(
// C2Y-NEXT:                           Identifier(
// C2Y-NEXT:                               "value",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       ),
// C2Y-NEXT:                       associations: [
// C2Y-NEXT:                           Type {
// C2Y-NEXT:                               ty: TypeName {
// C2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                       ty: Integer(
// C2Y-NEXT:                                           Ranked {
// C2Y-NEXT:                                               rank: Int,
// C2Y-NEXT:                                               signed: true,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   declarator: Array {
// C2Y-NEXT:                                       inner: Grouped(
// C2Y-NEXT:                                           Pointer {
// C2Y-NEXT:                                               qualifiers: Qualifiers,
// C2Y-NEXT:                                               inner: Abstract,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                       size: Star,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               },
// C2Y-NEXT:                               value: IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 1,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "1",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           Default(
// C2Y-NEXT:                               IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 0,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "0",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Generic {
// C2Y-NEXT:                       controlling: Expr(
// C2Y-NEXT:                           Identifier(
// C2Y-NEXT:                               "value",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       ),
// C2Y-NEXT:                       associations: [
// C2Y-NEXT:                           Type {
// C2Y-NEXT:                               ty: TypeName {
// C2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                       ty: Integer(
// C2Y-NEXT:                                           Ranked {
// C2Y-NEXT:                                               rank: Int,
// C2Y-NEXT:                                               signed: true,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   declarator: Array {
// C2Y-NEXT:                                       inner: Abstract,
// C2Y-NEXT:                                       size: Expression(
// C2Y-NEXT:                                           SizeOfType {
// C2Y-NEXT:                                               ty: TypeName {
// C2Y-NEXT:                                                   specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                                       ty: Integer(
// C2Y-NEXT:                                                           Ranked {
// C2Y-NEXT:                                                               rank: Int,
// C2Y-NEXT:                                                               signed: true,
// C2Y-NEXT:                                                           },
// C2Y-NEXT:                                                       ),
// C2Y-NEXT:                                                   },
// C2Y-NEXT:                                                   declarator: Array {
// C2Y-NEXT:                                                       inner: Abstract,
// C2Y-NEXT:                                                       size: Star,
// C2Y-NEXT:                                                   },
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               },
// C2Y-NEXT:                               value: IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 1,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "1",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           Default(
// C2Y-NEXT:                               IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 0,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "0",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Generic {
// C2Y-NEXT:                       controlling: Expr(
// C2Y-NEXT:                           Identifier(
// C2Y-NEXT:                               "value",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       ),
// C2Y-NEXT:                       associations: [
// C2Y-NEXT:                           Type {
// C2Y-NEXT:                               ty: TypeName {
// C2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                       ty: Tag(
// C2Y-NEXT:                                           Definition(
// C2Y-NEXT:                                               TagId(
// C2Y-NEXT:                                                   [[#TAG0]],
// C2Y-NEXT:                                               ),
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   declarator: Abstract,
// C2Y-NEXT:                               },
// C2Y-NEXT:                               value: IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 1,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "1",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           Default(
// C2Y-NEXT:                               IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 0,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "0",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:           ],
// C2Y-NEXT:       },
// C2Y-NEXT:   )
// C2Y-NEXT: decl[{{[0-9]+}}]: Function(
// C2Y-NEXT:       FunctionDefinition {
// C2Y-NEXT:           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:               ty: Void,
// C2Y-NEXT:           },
// C2Y-NEXT:           declarator: Function {
// C2Y-NEXT:               inner: Name(
// C2Y-NEXT:                   "controlling",
// C2Y-NEXT:               ),
// C2Y-NEXT:               parameters: Prototype {
// C2Y-NEXT:                   parameters: [
// C2Y-NEXT:                       ParameterDeclarationKind {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   Ranked {
// C2Y-NEXT:                                       rank: Int,
// C2Y-NEXT:                                       signed: true,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarator: Name(
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ],
// C2Y-NEXT:               },
// C2Y-NEXT:           },
// C2Y-NEXT:           body: [
// C2Y-NEXT:               Decl(
// C2Y-NEXT:                   Declaration {
// C2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                           ty: Integer(
// C2Y-NEXT:                               Ranked {
// C2Y-NEXT:                                   rank: Int,
// C2Y-NEXT:                                   signed: true,
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                       declarators: [
// C2Y-NEXT:                           InitDeclaratorKind {
// C2Y-NEXT:                               declarator: Array {
// C2Y-NEXT:                                   inner: Name(
// C2Y-NEXT:                                       "a",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   size: Expression(
// C2Y-NEXT:                                       IntegerLiteral(
// C2Y-NEXT:                                           IntegerLiteral {
// C2Y-NEXT:                                               value: 1,
// C2Y-NEXT:                                               radix: Decimal,
// C2Y-NEXT:                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                   unsigned: false,
// C2Y-NEXT:                                                   size: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               spelling: "1",
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Decl(
// C2Y-NEXT:                   Declaration {
// C2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                           ty: Floating(
// C2Y-NEXT:                               Float,
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                       declarators: [
// C2Y-NEXT:                           InitDeclaratorKind {
// C2Y-NEXT:                               declarator: Array {
// C2Y-NEXT:                                   inner: Name(
// C2Y-NEXT:                                       "b",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   size: Expression(
// C2Y-NEXT:                                       IntegerLiteral(
// C2Y-NEXT:                                           IntegerLiteral {
// C2Y-NEXT:                                               value: 1,
// C2Y-NEXT:                                               radix: Decimal,
// C2Y-NEXT:                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                   unsigned: false,
// C2Y-NEXT:                                                   size: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               spelling: "1",
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Decl(
// C2Y-NEXT:                   Declaration {
// C2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                           ty: Integer(
// C2Y-NEXT:                               Ranked {
// C2Y-NEXT:                                   rank: Int,
// C2Y-NEXT:                                   signed: true,
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           storage: Typedef,
// C2Y-NEXT:                       },
// C2Y-NEXT:                       declarators: [
// C2Y-NEXT:                           InitDeclaratorKind {
// C2Y-NEXT:                               declarator: Array {
// C2Y-NEXT:                                   inner: Name(
// C2Y-NEXT:                                       "V",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   size: Expression(
// C2Y-NEXT:                                       Identifier(
// C2Y-NEXT:                                           "n",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Generic {
// C2Y-NEXT:                       controlling: Type {
// C2Y-NEXT:                           ty: TypeName {
// C2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                   ty: TypeOf(
// C2Y-NEXT:                                       Expression(
// C2Y-NEXT:                                           Identifier(
// C2Y-NEXT:                                               "a",
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                               declarator: Abstract,
// C2Y-NEXT:                           },
// C2Y-NEXT:                       },
// C2Y-NEXT:                       associations: [
// C2Y-NEXT:                           Type {
// C2Y-NEXT:                               ty: TypeName {
// C2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                       ty: Integer(
// C2Y-NEXT:                                           Ranked {
// C2Y-NEXT:                                               rank: Int,
// C2Y-NEXT:                                               signed: true,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   declarator: Array {
// C2Y-NEXT:                                       inner: Abstract,
// C2Y-NEXT:                                       size: Expression(
// C2Y-NEXT:                                           Postfix {
// C2Y-NEXT:                                               op: Increment,
// C2Y-NEXT:                                               operand: Identifier(
// C2Y-NEXT:                                                   "n",
// C2Y-NEXT:                                               ),
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               },
// C2Y-NEXT:                               value: IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 0,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "0",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Generic {
// C2Y-NEXT:                       controlling: Type {
// C2Y-NEXT:                           ty: TypeName {
// C2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                   ty: TypeOf(
// C2Y-NEXT:                                       Expression(
// C2Y-NEXT:                                           Identifier(
// C2Y-NEXT:                                               "b",
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                               declarator: Abstract,
// C2Y-NEXT:                           },
// C2Y-NEXT:                       },
// C2Y-NEXT:                       associations: [
// C2Y-NEXT:                           Type {
// C2Y-NEXT:                               ty: TypeName {
// C2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                       ty: Integer(
// C2Y-NEXT:                                           Ranked {
// C2Y-NEXT:                                               rank: Int,
// C2Y-NEXT:                                               signed: true,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   declarator: Array {
// C2Y-NEXT:                                       inner: Abstract,
// C2Y-NEXT:                                       size: Expression(
// C2Y-NEXT:                                           Postfix {
// C2Y-NEXT:                                               op: Increment,
// C2Y-NEXT:                                               operand: Identifier(
// C2Y-NEXT:                                                   "n",
// C2Y-NEXT:                                               ),
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               },
// C2Y-NEXT:                               value: IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 0,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "0",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           Default(
// C2Y-NEXT:                               IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 1,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "1",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Generic {
// C2Y-NEXT:                       controlling: Type {
// C2Y-NEXT:                           ty: TypeName {
// C2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                   ty: Named(
// C2Y-NEXT:                                       "V",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                               declarator: Abstract,
// C2Y-NEXT:                           },
// C2Y-NEXT:                       },
// C2Y-NEXT:                       associations: [
// C2Y-NEXT:                           Type {
// C2Y-NEXT:                               ty: TypeName {
// C2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                       ty: Integer(
// C2Y-NEXT:                                           Ranked {
// C2Y-NEXT:                                               rank: Int,
// C2Y-NEXT:                                               signed: true,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   declarator: Array {
// C2Y-NEXT:                                       inner: Abstract,
// C2Y-NEXT:                                       size: Expression(
// C2Y-NEXT:                                           Identifier(
// C2Y-NEXT:                                               "n",
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               },
// C2Y-NEXT:                               value: IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 0,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "0",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               Expr(
// C2Y-NEXT:                   Generic {
// C2Y-NEXT:                       controlling: Type {
// C2Y-NEXT:                           ty: TypeName {
// C2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                   ty: Integer(
// C2Y-NEXT:                                       Ranked {
// C2Y-NEXT:                                           rank: Int,
// C2Y-NEXT:                                           signed: true,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                               declarator: Array {
// C2Y-NEXT:                                   inner: Grouped(
// C2Y-NEXT:                                       Pointer {
// C2Y-NEXT:                                           qualifiers: Qualifiers,
// C2Y-NEXT:                                           inner: Abstract,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   size: Expression(
// C2Y-NEXT:                                       Identifier(
// C2Y-NEXT:                                           "n",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           },
// C2Y-NEXT:                       },
// C2Y-NEXT:                       associations: [
// C2Y-NEXT:                           Type {
// C2Y-NEXT:                               ty: TypeName {
// C2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                       ty: Integer(
// C2Y-NEXT:                                           Ranked {
// C2Y-NEXT:                                               rank: Int,
// C2Y-NEXT:                                               signed: true,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   declarator: Array {
// C2Y-NEXT:                                       inner: Grouped(
// C2Y-NEXT:                                           Pointer {
// C2Y-NEXT:                                               qualifiers: Qualifiers,
// C2Y-NEXT:                                               inner: Abstract,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                       size: Star,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               },
// C2Y-NEXT:                               value: IntegerLiteral(
// C2Y-NEXT:                                   IntegerLiteral {
// C2Y-NEXT:                                       value: 0,
// C2Y-NEXT:                                       radix: Decimal,
// C2Y-NEXT:                                       suffix: IntegerSuffix {
// C2Y-NEXT:                                           unsigned: false,
// C2Y-NEXT:                                           size: None,
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                       spelling: "0",
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:           ],
// C2Y-NEXT:       },
// C2Y-NEXT:   )
// SLATE-FILECHECK-END C2Y
// SLATE-FILECHECK-BEGIN GNU2Y
// GNU2Y: tag[{{[0-9]+}}]: TagDefinition {
// GNU2Y-NEXT:       id: TagId(
// GNU2Y-NEXT:           [[#TAG0:]],
// GNU2Y-NEXT:       ),
// GNU2Y-NEXT:       kind: Struct,
// GNU2Y-NEXT:       name: None,
// GNU2Y-NEXT:       body: Record(
// GNU2Y-NEXT:           [
// GNU2Y-NEXT:               Field(
// GNU2Y-NEXT:                   FieldDecl {
// GNU2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                           ty: Integer(
// GNU2Y-NEXT:                               Ranked {
// GNU2Y-NEXT:                                   rank: Int,
// GNU2Y-NEXT:                                   signed: true,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       declarators: [
// GNU2Y-NEXT:                           FieldDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Array {
// GNU2Y-NEXT:                                   inner: Name(
// GNU2Y-NEXT:                                       "a",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   size: Star,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:           ],
// GNU2Y-NEXT:       ),
// GNU2Y-NEXT:   }
// GNU2Y-NEXT: decl[{{[0-9]+}}]: Function(
// GNU2Y-NEXT:       FunctionDefinition {
// GNU2Y-NEXT:           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:               ty: Void,
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           declarator: Function {
// GNU2Y-NEXT:               inner: Name(
// GNU2Y-NEXT:                   "associations",
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               parameters: Prototype {
// GNU2Y-NEXT:                   parameters: [
// GNU2Y-NEXT:                       ParameterDeclarationKind {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   Ranked {
// GNU2Y-NEXT:                                       rank: Int,
// GNU2Y-NEXT:                                       signed: true,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Name(
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       ParameterDeclarationKind {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   Ranked {
// GNU2Y-NEXT:                                       rank: Int,
// GNU2Y-NEXT:                                       signed: true,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Name(
// GNU2Y-NEXT:                               "value",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ],
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           body: [
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Generic {
// GNU2Y-NEXT:                       controlling: Expr(
// GNU2Y-NEXT:                           Identifier(
// GNU2Y-NEXT:                               "value",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                       associations: [
// GNU2Y-NEXT:                           Type {
// GNU2Y-NEXT:                               ty: TypeName {
// GNU2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                       ty: Integer(
// GNU2Y-NEXT:                                           Ranked {
// GNU2Y-NEXT:                                               rank: Int,
// GNU2Y-NEXT:                                               signed: true,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   declarator: Abstract,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               value: IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 1,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "1",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           Type {
// GNU2Y-NEXT:                               ty: TypeName {
// GNU2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                       ty: Integer(
// GNU2Y-NEXT:                                           Ranked {
// GNU2Y-NEXT:                                               rank: Int,
// GNU2Y-NEXT:                                               signed: true,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   declarator: Array {
// GNU2Y-NEXT:                                       inner: Grouped(
// GNU2Y-NEXT:                                           Pointer {
// GNU2Y-NEXT:                                               qualifiers: Qualifiers,
// GNU2Y-NEXT:                                               inner: Abstract,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                       size: Expression(
// GNU2Y-NEXT:                                           Identifier(
// GNU2Y-NEXT:                                               "n",
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               value: IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 2,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "2",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Generic {
// GNU2Y-NEXT:                       controlling: Expr(
// GNU2Y-NEXT:                           Identifier(
// GNU2Y-NEXT:                               "value",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                       associations: [
// GNU2Y-NEXT:                           Type {
// GNU2Y-NEXT:                               ty: TypeName {
// GNU2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                       ty: Integer(
// GNU2Y-NEXT:                                           Ranked {
// GNU2Y-NEXT:                                               rank: Int,
// GNU2Y-NEXT:                                               signed: true,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   declarator: Array {
// GNU2Y-NEXT:                                       inner: Abstract,
// GNU2Y-NEXT:                                       size: Star,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               value: IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 1,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "1",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           Default(
// GNU2Y-NEXT:                               IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 0,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "0",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Generic {
// GNU2Y-NEXT:                       controlling: Expr(
// GNU2Y-NEXT:                           Identifier(
// GNU2Y-NEXT:                               "value",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                       associations: [
// GNU2Y-NEXT:                           Type {
// GNU2Y-NEXT:                               ty: TypeName {
// GNU2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                       ty: Integer(
// GNU2Y-NEXT:                                           Ranked {
// GNU2Y-NEXT:                                               rank: Int,
// GNU2Y-NEXT:                                               signed: true,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   declarator: Array {
// GNU2Y-NEXT:                                       inner: Grouped(
// GNU2Y-NEXT:                                           Pointer {
// GNU2Y-NEXT:                                               qualifiers: Qualifiers,
// GNU2Y-NEXT:                                               inner: Abstract,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                       size: Star,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               value: IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 1,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "1",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           Default(
// GNU2Y-NEXT:                               IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 0,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "0",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Generic {
// GNU2Y-NEXT:                       controlling: Expr(
// GNU2Y-NEXT:                           Identifier(
// GNU2Y-NEXT:                               "value",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                       associations: [
// GNU2Y-NEXT:                           Type {
// GNU2Y-NEXT:                               ty: TypeName {
// GNU2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                       ty: Integer(
// GNU2Y-NEXT:                                           Ranked {
// GNU2Y-NEXT:                                               rank: Int,
// GNU2Y-NEXT:                                               signed: true,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   declarator: Array {
// GNU2Y-NEXT:                                       inner: Abstract,
// GNU2Y-NEXT:                                       size: Expression(
// GNU2Y-NEXT:                                           SizeOfType {
// GNU2Y-NEXT:                                               ty: TypeName {
// GNU2Y-NEXT:                                                   specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                                       ty: Integer(
// GNU2Y-NEXT:                                                           Ranked {
// GNU2Y-NEXT:                                                               rank: Int,
// GNU2Y-NEXT:                                                               signed: true,
// GNU2Y-NEXT:                                                           },
// GNU2Y-NEXT:                                                       ),
// GNU2Y-NEXT:                                                   },
// GNU2Y-NEXT:                                                   declarator: Array {
// GNU2Y-NEXT:                                                       inner: Abstract,
// GNU2Y-NEXT:                                                       size: Star,
// GNU2Y-NEXT:                                                   },
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               value: IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 1,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "1",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           Default(
// GNU2Y-NEXT:                               IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 0,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "0",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Generic {
// GNU2Y-NEXT:                       controlling: Expr(
// GNU2Y-NEXT:                           Identifier(
// GNU2Y-NEXT:                               "value",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                       associations: [
// GNU2Y-NEXT:                           Type {
// GNU2Y-NEXT:                               ty: TypeName {
// GNU2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                       ty: Tag(
// GNU2Y-NEXT:                                           Definition(
// GNU2Y-NEXT:                                               TagId(
// GNU2Y-NEXT:                                                   [[#TAG0]],
// GNU2Y-NEXT:                                               ),
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   declarator: Abstract,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               value: IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 1,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "1",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           Default(
// GNU2Y-NEXT:                               IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 0,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "0",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:           ],
// GNU2Y-NEXT:       },
// GNU2Y-NEXT:   )
// GNU2Y-NEXT: decl[{{[0-9]+}}]: Function(
// GNU2Y-NEXT:       FunctionDefinition {
// GNU2Y-NEXT:           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:               ty: Void,
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           declarator: Function {
// GNU2Y-NEXT:               inner: Name(
// GNU2Y-NEXT:                   "controlling",
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               parameters: Prototype {
// GNU2Y-NEXT:                   parameters: [
// GNU2Y-NEXT:                       ParameterDeclarationKind {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   Ranked {
// GNU2Y-NEXT:                                       rank: Int,
// GNU2Y-NEXT:                                       signed: true,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarator: Name(
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ],
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           body: [
// GNU2Y-NEXT:               Decl(
// GNU2Y-NEXT:                   Declaration {
// GNU2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                           ty: Integer(
// GNU2Y-NEXT:                               Ranked {
// GNU2Y-NEXT:                                   rank: Int,
// GNU2Y-NEXT:                                   signed: true,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       declarators: [
// GNU2Y-NEXT:                           InitDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Array {
// GNU2Y-NEXT:                                   inner: Name(
// GNU2Y-NEXT:                                       "a",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   size: Expression(
// GNU2Y-NEXT:                                       IntegerLiteral(
// GNU2Y-NEXT:                                           IntegerLiteral {
// GNU2Y-NEXT:                                               value: 1,
// GNU2Y-NEXT:                                               radix: Decimal,
// GNU2Y-NEXT:                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                   unsigned: false,
// GNU2Y-NEXT:                                                   size: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               spelling: "1",
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Decl(
// GNU2Y-NEXT:                   Declaration {
// GNU2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                           ty: Floating(
// GNU2Y-NEXT:                               Float,
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       declarators: [
// GNU2Y-NEXT:                           InitDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Array {
// GNU2Y-NEXT:                                   inner: Name(
// GNU2Y-NEXT:                                       "b",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   size: Expression(
// GNU2Y-NEXT:                                       IntegerLiteral(
// GNU2Y-NEXT:                                           IntegerLiteral {
// GNU2Y-NEXT:                                               value: 1,
// GNU2Y-NEXT:                                               radix: Decimal,
// GNU2Y-NEXT:                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                   unsigned: false,
// GNU2Y-NEXT:                                                   size: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               spelling: "1",
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Decl(
// GNU2Y-NEXT:                   Declaration {
// GNU2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                           ty: Integer(
// GNU2Y-NEXT:                               Ranked {
// GNU2Y-NEXT:                                   rank: Int,
// GNU2Y-NEXT:                                   signed: true,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           storage: Typedef,
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       declarators: [
// GNU2Y-NEXT:                           InitDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Array {
// GNU2Y-NEXT:                                   inner: Name(
// GNU2Y-NEXT:                                       "V",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   size: Expression(
// GNU2Y-NEXT:                                       Identifier(
// GNU2Y-NEXT:                                           "n",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Generic {
// GNU2Y-NEXT:                       controlling: Type {
// GNU2Y-NEXT:                           ty: TypeName {
// GNU2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                   ty: TypeOf(
// GNU2Y-NEXT:                                       Expression(
// GNU2Y-NEXT:                                           Identifier(
// GNU2Y-NEXT:                                               "a",
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               declarator: Abstract,
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       associations: [
// GNU2Y-NEXT:                           Type {
// GNU2Y-NEXT:                               ty: TypeName {
// GNU2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                       ty: Integer(
// GNU2Y-NEXT:                                           Ranked {
// GNU2Y-NEXT:                                               rank: Int,
// GNU2Y-NEXT:                                               signed: true,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   declarator: Array {
// GNU2Y-NEXT:                                       inner: Abstract,
// GNU2Y-NEXT:                                       size: Expression(
// GNU2Y-NEXT:                                           Postfix {
// GNU2Y-NEXT:                                               op: Increment,
// GNU2Y-NEXT:                                               operand: Identifier(
// GNU2Y-NEXT:                                                   "n",
// GNU2Y-NEXT:                                               ),
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               value: IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 0,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "0",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Generic {
// GNU2Y-NEXT:                       controlling: Type {
// GNU2Y-NEXT:                           ty: TypeName {
// GNU2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                   ty: TypeOf(
// GNU2Y-NEXT:                                       Expression(
// GNU2Y-NEXT:                                           Identifier(
// GNU2Y-NEXT:                                               "b",
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               declarator: Abstract,
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       associations: [
// GNU2Y-NEXT:                           Type {
// GNU2Y-NEXT:                               ty: TypeName {
// GNU2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                       ty: Integer(
// GNU2Y-NEXT:                                           Ranked {
// GNU2Y-NEXT:                                               rank: Int,
// GNU2Y-NEXT:                                               signed: true,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   declarator: Array {
// GNU2Y-NEXT:                                       inner: Abstract,
// GNU2Y-NEXT:                                       size: Expression(
// GNU2Y-NEXT:                                           Postfix {
// GNU2Y-NEXT:                                               op: Increment,
// GNU2Y-NEXT:                                               operand: Identifier(
// GNU2Y-NEXT:                                                   "n",
// GNU2Y-NEXT:                                               ),
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               value: IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 0,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "0",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           Default(
// GNU2Y-NEXT:                               IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 1,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "1",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Generic {
// GNU2Y-NEXT:                       controlling: Type {
// GNU2Y-NEXT:                           ty: TypeName {
// GNU2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                   ty: Named(
// GNU2Y-NEXT:                                       "V",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               declarator: Abstract,
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       associations: [
// GNU2Y-NEXT:                           Type {
// GNU2Y-NEXT:                               ty: TypeName {
// GNU2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                       ty: Integer(
// GNU2Y-NEXT:                                           Ranked {
// GNU2Y-NEXT:                                               rank: Int,
// GNU2Y-NEXT:                                               signed: true,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   declarator: Array {
// GNU2Y-NEXT:                                       inner: Abstract,
// GNU2Y-NEXT:                                       size: Expression(
// GNU2Y-NEXT:                                           Identifier(
// GNU2Y-NEXT:                                               "n",
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               value: IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 0,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "0",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               Expr(
// GNU2Y-NEXT:                   Generic {
// GNU2Y-NEXT:                       controlling: Type {
// GNU2Y-NEXT:                           ty: TypeName {
// GNU2Y-NEXT:                               specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                   ty: Integer(
// GNU2Y-NEXT:                                       Ranked {
// GNU2Y-NEXT:                                           rank: Int,
// GNU2Y-NEXT:                                           signed: true,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               declarator: Array {
// GNU2Y-NEXT:                                   inner: Grouped(
// GNU2Y-NEXT:                                       Pointer {
// GNU2Y-NEXT:                                           qualifiers: Qualifiers,
// GNU2Y-NEXT:                                           inner: Abstract,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   size: Expression(
// GNU2Y-NEXT:                                       Identifier(
// GNU2Y-NEXT:                                           "n",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       associations: [
// GNU2Y-NEXT:                           Type {
// GNU2Y-NEXT:                               ty: TypeName {
// GNU2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                       ty: Integer(
// GNU2Y-NEXT:                                           Ranked {
// GNU2Y-NEXT:                                               rank: Int,
// GNU2Y-NEXT:                                               signed: true,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   declarator: Array {
// GNU2Y-NEXT:                                       inner: Grouped(
// GNU2Y-NEXT:                                           Pointer {
// GNU2Y-NEXT:                                               qualifiers: Qualifiers,
// GNU2Y-NEXT:                                               inner: Abstract,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                       size: Star,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               value: IntegerLiteral(
// GNU2Y-NEXT:                                   IntegerLiteral {
// GNU2Y-NEXT:                                       value: 0,
// GNU2Y-NEXT:                                       radix: Decimal,
// GNU2Y-NEXT:                                       suffix: IntegerSuffix {
// GNU2Y-NEXT:                                           unsigned: false,
// GNU2Y-NEXT:                                           size: None,
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                       spelling: "0",
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:           ],
// GNU2Y-NEXT:       },
// GNU2Y-NEXT:   )
// SLATE-FILECHECK-END GNU2Y
