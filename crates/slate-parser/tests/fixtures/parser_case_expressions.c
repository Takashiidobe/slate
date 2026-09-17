// SLATE-FILECHECK-DEFINES AST
// SLATE-FILECHECK-STD AST gnu23

int cases(int x) {
    switch (x) {
    case 1 ? 2 : 3: return 1;
    case (0 ? 4 : 5) ... 1 ? 7 : 8: return 2;
    case sizeof(struct { int field : 3; }): return 3;
    default: return 0;
    }
}

// SLATE-FILECHECK-BEGIN AST
// AST: tag[0]: TagDefinition {
// AST-NEXT:       id: TagId(
// AST-NEXT:           0,
// AST-NEXT:       ),
// AST-NEXT:       kind: Struct,
// AST-NEXT:       name: None,
// AST-NEXT:       body: Record(
// AST-NEXT:           [
// AST-NEXT:               Field(
// AST-NEXT:                   FieldDecl {
// AST-NEXT:                       specifiers: DeclarationSpecifiers {
// AST-NEXT:                           ty: Integer(
// AST-NEXT:                               Ranked {
// AST-NEXT:                                   rank: Int,
// AST-NEXT:                                   signed: true,
// AST-NEXT:                               },
// AST-NEXT:                           ),
// AST-NEXT:                       },
// AST-NEXT:                       declarators: [
// AST-NEXT:                           FieldDeclaratorKind {
// AST-NEXT:                               declarator: Name(
// AST-NEXT:                                   "field",
// AST-NEXT:                               ),
// AST-NEXT:                               bit_width: Some(
// AST-NEXT:                                   IntegerLiteral(
// AST-NEXT:                                       IntegerLiteral {
// AST-NEXT:                                           value: 3,
// AST-NEXT:                                           radix: Decimal,
// AST-NEXT:                                           suffix: IntegerSuffix {
// AST-NEXT:                                               unsigned: false,
// AST-NEXT:                                               size: None,
// AST-NEXT:                                           },
// AST-NEXT:                                           spelling: "3",
// AST-NEXT:                                       },
// AST-NEXT:                                   ),
// AST-NEXT:                               ),
// AST-NEXT:                           },
// AST-NEXT:                       ],
// AST-NEXT:                   },
// AST-NEXT:               ),
// AST-NEXT:           ],
// AST-NEXT:       ),
// AST-NEXT:   }
// AST-NEXT: decl[0]: Function(
// AST-NEXT:       FunctionDefinition {
// AST-NEXT:           specifiers: DeclarationSpecifiers {
// AST-NEXT:               ty: Integer(
// AST-NEXT:                   Ranked {
// AST-NEXT:                       rank: Int,
// AST-NEXT:                       signed: true,
// AST-NEXT:                   },
// AST-NEXT:               ),
// AST-NEXT:           },
// AST-NEXT:           declarator: Function {
// AST-NEXT:               inner: Name(
// AST-NEXT:                   "cases",
// AST-NEXT:               ),
// AST-NEXT:               parameters: Prototype {
// AST-NEXT:                   parameters: [
// AST-NEXT:                       ParameterDeclarationKind {
// AST-NEXT:                           specifiers: DeclarationSpecifiers {
// AST-NEXT:                               ty: Integer(
// AST-NEXT:                                   Ranked {
// AST-NEXT:                                       rank: Int,
// AST-NEXT:                                       signed: true,
// AST-NEXT:                                   },
// AST-NEXT:                               ),
// AST-NEXT:                           },
// AST-NEXT:                           declarator: Name(
// AST-NEXT:                               "x",
// AST-NEXT:                           ),
// AST-NEXT:                       },
// AST-NEXT:                   ],
// AST-NEXT:               },
// AST-NEXT:           },
// AST-NEXT:           body: [
// AST-NEXT:               Switch {
// AST-NEXT:                   discriminant: Identifier(
// AST-NEXT:                       "x",
// AST-NEXT:                   ),
// AST-NEXT:                   body: Block(
// AST-NEXT:                       [
// AST-NEXT:                           SwitchLabel {
// AST-NEXT:                               label: Case(
// AST-NEXT:                                   Conditional {
// AST-NEXT:                                       condition: IntegerLiteral(
// AST-NEXT:                                           IntegerLiteral {
// AST-NEXT:                                               value: 1,
// AST-NEXT:                                               radix: Decimal,
// AST-NEXT:                                               suffix: IntegerSuffix {
// AST-NEXT:                                                   unsigned: false,
// AST-NEXT:                                                   size: None,
// AST-NEXT:                                               },
// AST-NEXT:                                               spelling: "1",
// AST-NEXT:                                           },
// AST-NEXT:                                       ),
// AST-NEXT:                                       then_value: Some(
// AST-NEXT:                                           IntegerLiteral(
// AST-NEXT:                                               IntegerLiteral {
// AST-NEXT:                                                   value: 2,
// AST-NEXT:                                                   radix: Decimal,
// AST-NEXT:                                                   suffix: IntegerSuffix {
// AST-NEXT:                                                       unsigned: false,
// AST-NEXT:                                                       size: None,
// AST-NEXT:                                                   },
// AST-NEXT:                                                   spelling: "2",
// AST-NEXT:                                               },
// AST-NEXT:                                           ),
// AST-NEXT:                                       ),
// AST-NEXT:                                       else_value: IntegerLiteral(
// AST-NEXT:                                           IntegerLiteral {
// AST-NEXT:                                               value: 3,
// AST-NEXT:                                               radix: Decimal,
// AST-NEXT:                                               suffix: IntegerSuffix {
// AST-NEXT:                                                   unsigned: false,
// AST-NEXT:                                                   size: None,
// AST-NEXT:                                               },
// AST-NEXT:                                               spelling: "3",
// AST-NEXT:                                           },
// AST-NEXT:                                       ),
// AST-NEXT:                                   },
// AST-NEXT:                               ),
// AST-NEXT:                               body: Return(
// AST-NEXT:                                   IntegerLiteral(
// AST-NEXT:                                       IntegerLiteral {
// AST-NEXT:                                           value: 1,
// AST-NEXT:                                           radix: Decimal,
// AST-NEXT:                                           suffix: IntegerSuffix {
// AST-NEXT:                                               unsigned: false,
// AST-NEXT:                                               size: None,
// AST-NEXT:                                           },
// AST-NEXT:                                           spelling: "1",
// AST-NEXT:                                       },
// AST-NEXT:                                   ),
// AST-NEXT:                               ),
// AST-NEXT:                           },
// AST-NEXT:                           SwitchLabel {
// AST-NEXT:                               label: CaseRange {
// AST-NEXT:                                   start: Paren(
// AST-NEXT:                                       Conditional {
// AST-NEXT:                                           condition: IntegerLiteral(
// AST-NEXT:                                               IntegerLiteral {
// AST-NEXT:                                                   value: 0,
// AST-NEXT:                                                   radix: Decimal,
// AST-NEXT:                                                   suffix: IntegerSuffix {
// AST-NEXT:                                                       unsigned: false,
// AST-NEXT:                                                       size: None,
// AST-NEXT:                                                   },
// AST-NEXT:                                                   spelling: "0",
// AST-NEXT:                                               },
// AST-NEXT:                                           ),
// AST-NEXT:                                           then_value: Some(
// AST-NEXT:                                               IntegerLiteral(
// AST-NEXT:                                                   IntegerLiteral {
// AST-NEXT:                                                       value: 4,
// AST-NEXT:                                                       radix: Decimal,
// AST-NEXT:                                                       suffix: IntegerSuffix {
// AST-NEXT:                                                           unsigned: false,
// AST-NEXT:                                                           size: None,
// AST-NEXT:                                                       },
// AST-NEXT:                                                       spelling: "4",
// AST-NEXT:                                                   },
// AST-NEXT:                                               ),
// AST-NEXT:                                           ),
// AST-NEXT:                                           else_value: IntegerLiteral(
// AST-NEXT:                                               IntegerLiteral {
// AST-NEXT:                                                   value: 5,
// AST-NEXT:                                                   radix: Decimal,
// AST-NEXT:                                                   suffix: IntegerSuffix {
// AST-NEXT:                                                       unsigned: false,
// AST-NEXT:                                                       size: None,
// AST-NEXT:                                                   },
// AST-NEXT:                                                   spelling: "5",
// AST-NEXT:                                               },
// AST-NEXT:                                           ),
// AST-NEXT:                                       },
// AST-NEXT:                                   ),
// AST-NEXT:                                   end: Conditional {
// AST-NEXT:                                       condition: IntegerLiteral(
// AST-NEXT:                                           IntegerLiteral {
// AST-NEXT:                                               value: 1,
// AST-NEXT:                                               radix: Decimal,
// AST-NEXT:                                               suffix: IntegerSuffix {
// AST-NEXT:                                                   unsigned: false,
// AST-NEXT:                                                   size: None,
// AST-NEXT:                                               },
// AST-NEXT:                                               spelling: "1",
// AST-NEXT:                                           },
// AST-NEXT:                                       ),
// AST-NEXT:                                       then_value: Some(
// AST-NEXT:                                           IntegerLiteral(
// AST-NEXT:                                               IntegerLiteral {
// AST-NEXT:                                                   value: 7,
// AST-NEXT:                                                   radix: Decimal,
// AST-NEXT:                                                   suffix: IntegerSuffix {
// AST-NEXT:                                                       unsigned: false,
// AST-NEXT:                                                       size: None,
// AST-NEXT:                                                   },
// AST-NEXT:                                                   spelling: "7",
// AST-NEXT:                                               },
// AST-NEXT:                                           ),
// AST-NEXT:                                       ),
// AST-NEXT:                                       else_value: IntegerLiteral(
// AST-NEXT:                                           IntegerLiteral {
// AST-NEXT:                                               value: 8,
// AST-NEXT:                                               radix: Decimal,
// AST-NEXT:                                               suffix: IntegerSuffix {
// AST-NEXT:                                                   unsigned: false,
// AST-NEXT:                                                   size: None,
// AST-NEXT:                                               },
// AST-NEXT:                                               spelling: "8",
// AST-NEXT:                                           },
// AST-NEXT:                                       ),
// AST-NEXT:                                   },
// AST-NEXT:                               },
// AST-NEXT:                               body: Return(
// AST-NEXT:                                   IntegerLiteral(
// AST-NEXT:                                       IntegerLiteral {
// AST-NEXT:                                           value: 2,
// AST-NEXT:                                           radix: Decimal,
// AST-NEXT:                                           suffix: IntegerSuffix {
// AST-NEXT:                                               unsigned: false,
// AST-NEXT:                                               size: None,
// AST-NEXT:                                           },
// AST-NEXT:                                           spelling: "2",
// AST-NEXT:                                       },
// AST-NEXT:                                   ),
// AST-NEXT:                               ),
// AST-NEXT:                           },
// AST-NEXT:                           SwitchLabel {
// AST-NEXT:                               label: Case(
// AST-NEXT:                                   SizeOfType {
// AST-NEXT:                                       ty: TypeName {
// AST-NEXT:                                           specifiers: DeclarationSpecifiers {
// AST-NEXT:                                               ty: Tag(
// AST-NEXT:                                                   Definition(
// AST-NEXT:                                                       TagId(
// AST-NEXT:                                                           0,
// AST-NEXT:                                                       ),
// AST-NEXT:                                                   ),
// AST-NEXT:                                               ),
// AST-NEXT:                                           },
// AST-NEXT:                                           declarator: Abstract,
// AST-NEXT:                                       },
// AST-NEXT:                                   },
// AST-NEXT:                               ),
// AST-NEXT:                               body: Return(
// AST-NEXT:                                   IntegerLiteral(
// AST-NEXT:                                       IntegerLiteral {
// AST-NEXT:                                           value: 3,
// AST-NEXT:                                           radix: Decimal,
// AST-NEXT:                                           suffix: IntegerSuffix {
// AST-NEXT:                                               unsigned: false,
// AST-NEXT:                                               size: None,
// AST-NEXT:                                           },
// AST-NEXT:                                           spelling: "3",
// AST-NEXT:                                       },
// AST-NEXT:                                   ),
// AST-NEXT:                               ),
// AST-NEXT:                           },
// AST-NEXT:                           SwitchLabel {
// AST-NEXT:                               label: Default,
// AST-NEXT:                               body: Return(
// AST-NEXT:                                   IntegerLiteral(
// AST-NEXT:                                       IntegerLiteral {
// AST-NEXT:                                           value: 0,
// AST-NEXT:                                           radix: Decimal,
// AST-NEXT:                                           suffix: IntegerSuffix {
// AST-NEXT:                                               unsigned: false,
// AST-NEXT:                                               size: None,
// AST-NEXT:                                           },
// AST-NEXT:                                           spelling: "0",
// AST-NEXT:                                       },
// AST-NEXT:                                   ),
// AST-NEXT:                               ),
// AST-NEXT:                           },
// AST-NEXT:                       ],
// AST-NEXT:                   ),
// AST-NEXT:               },
// AST-NEXT:           ],
// AST-NEXT:       },
// AST-NEXT:   )
// SLATE-FILECHECK-END AST
