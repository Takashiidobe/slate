int computed_goto(int n) {
  void *labels[2] = { &&L0, &&L1 };
#ifdef USE_COMPUTED_GOTO
  goto *labels[n];
#else
  goto L1;
#endif
L0:
  return 0;
L1:
  return 1;
}

int vla_sum(int n, int arr[n]) {
#ifdef DOUBLE_LOCAL
  int local[n * 2];
#else
  int local[n];
#endif
  return local[0] + arr[0];
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES COMPUTED USE_COMPUTED_GOTO
// SLATE-FILECHECK-DEFINES DOUBLED DOUBLE_LOCAL

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Function(
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
// DEFAULT-NEXT:                   "computed_goto",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Prototype {
// DEFAULT-NEXT:                   parameters: [
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
// DEFAULT-NEXT:                               "n",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Void,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Pointer {
// DEFAULT-NEXT:                                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "labels",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 2,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "2",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   List(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   LabelAddress(
// DEFAULT-NEXT:                                                       "L0",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           InitializerItem {
// DEFAULT-NEXT:                                               designators: [],
// DEFAULT-NEXT:                                               value: Expr(
// DEFAULT-NEXT:                                                   LabelAddress(
// DEFAULT-NEXT:                                                       "L1",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Goto(
// DEFAULT-NEXT:                   "L1",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Labeled {
// DEFAULT-NEXT:                   label: "L0",
// DEFAULT-NEXT:                   body: Return(
// DEFAULT-NEXT:                       IntegerLiteral(
// DEFAULT-NEXT:                           IntegerLiteral {
// DEFAULT-NEXT:                               value: 0,
// DEFAULT-NEXT:                               radix: Decimal,
// DEFAULT-NEXT:                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                   unsigned: false,
// DEFAULT-NEXT:                                   size: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               spelling: "0",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Labeled {
// DEFAULT-NEXT:                   label: "L1",
// DEFAULT-NEXT:                   body: Return(
// DEFAULT-NEXT:                       IntegerLiteral(
// DEFAULT-NEXT:                           IntegerLiteral {
// DEFAULT-NEXT:                               value: 1,
// DEFAULT-NEXT:                               radix: Decimal,
// DEFAULT-NEXT:                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                   unsigned: false,
// DEFAULT-NEXT:                                   size: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               spelling: "1",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
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
// DEFAULT-NEXT: decl[1]: Function(
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
// DEFAULT-NEXT:                   "vla_sum",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Prototype {
// DEFAULT-NEXT:                   parameters: [
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
// DEFAULT-NEXT:                               "n",
// DEFAULT-NEXT:                           ),
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
// DEFAULT-NEXT:                           declarator: Array {
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "arr",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               size: Expression(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "n",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
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
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "local",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Binary {
// DEFAULT-NEXT:                       op: Add,
// DEFAULT-NEXT:                       left: Index {
// DEFAULT-NEXT:                           base: Identifier(
// DEFAULT-NEXT:                               "local",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           index: IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 0,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "0",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       right: Index {
// DEFAULT-NEXT:                           base: Identifier(
// DEFAULT-NEXT:                               "arr",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           index: IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 0,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "0",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
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
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN COMPUTED
// COMPUTED: decl[0]: Function(
// COMPUTED-NEXT:       FunctionDefinition {
// COMPUTED-NEXT:           specifiers: DeclarationSpecifiers {
// COMPUTED-NEXT:               ty: Integer(
// COMPUTED-NEXT:                   Ranked {
// COMPUTED-NEXT:                       rank: Int,
// COMPUTED-NEXT:                       signed: true,
// COMPUTED-NEXT:                   },
// COMPUTED-NEXT:               ),
// COMPUTED-NEXT:           },
// COMPUTED-NEXT:           declarator: Function {
// COMPUTED-NEXT:               inner: Name(
// COMPUTED-NEXT:                   "computed_goto",
// COMPUTED-NEXT:               ),
// COMPUTED-NEXT:               parameters: Prototype {
// COMPUTED-NEXT:                   parameters: [
// COMPUTED-NEXT:                       ParameterDeclaration {
// COMPUTED-NEXT:                           specifiers: DeclarationSpecifiers {
// COMPUTED-NEXT:                               ty: Integer(
// COMPUTED-NEXT:                                   Ranked {
// COMPUTED-NEXT:                                       rank: Int,
// COMPUTED-NEXT:                                       signed: true,
// COMPUTED-NEXT:                                   },
// COMPUTED-NEXT:                               ),
// COMPUTED-NEXT:                           },
// COMPUTED-NEXT:                           declarator: Name(
// COMPUTED-NEXT:                               "n",
// COMPUTED-NEXT:                           ),
// COMPUTED-NEXT:                       },
// COMPUTED-NEXT:                   ],
// COMPUTED-NEXT:               },
// COMPUTED-NEXT:           },
// COMPUTED-NEXT:           body: [
// COMPUTED-NEXT:               Decl(
// COMPUTED-NEXT:                   Declaration {
// COMPUTED-NEXT:                       specifiers: DeclarationSpecifiers {
// COMPUTED-NEXT:                           ty: Void,
// COMPUTED-NEXT:                       },
// COMPUTED-NEXT:                       declarators: [
// COMPUTED-NEXT:                           InitDeclarator {
// COMPUTED-NEXT:                               declarator: Array {
// COMPUTED-NEXT:                                   inner: Pointer {
// COMPUTED-NEXT:                                       qualifiers: Qualifiers,
// COMPUTED-NEXT:                                       inner: Name(
// COMPUTED-NEXT:                                           "labels",
// COMPUTED-NEXT:                                       ),
// COMPUTED-NEXT:                                   },
// COMPUTED-NEXT:                                   size: Expression(
// COMPUTED-NEXT:                                       IntegerLiteral(
// COMPUTED-NEXT:                                           IntegerLiteral {
// COMPUTED-NEXT:                                               value: 2,
// COMPUTED-NEXT:                                               radix: Decimal,
// COMPUTED-NEXT:                                               suffix: IntegerSuffix {
// COMPUTED-NEXT:                                                   unsigned: false,
// COMPUTED-NEXT:                                                   size: None,
// COMPUTED-NEXT:                                               },
// COMPUTED-NEXT:                                               spelling: "2",
// COMPUTED-NEXT:                                           },
// COMPUTED-NEXT:                                       ),
// COMPUTED-NEXT:                                   ),
// COMPUTED-NEXT:                               },
// COMPUTED-NEXT:                               initializer: Some(
// COMPUTED-NEXT:                                   List(
// COMPUTED-NEXT:                                       [
// COMPUTED-NEXT:                                           InitializerItem {
// COMPUTED-NEXT:                                               designators: [],
// COMPUTED-NEXT:                                               value: Expr(
// COMPUTED-NEXT:                                                   LabelAddress(
// COMPUTED-NEXT:                                                       "L0",
// COMPUTED-NEXT:                                                   ),
// COMPUTED-NEXT:                                               ),
// COMPUTED-NEXT:                                           },
// COMPUTED-NEXT:                                           InitializerItem {
// COMPUTED-NEXT:                                               designators: [],
// COMPUTED-NEXT:                                               value: Expr(
// COMPUTED-NEXT:                                                   LabelAddress(
// COMPUTED-NEXT:                                                       "L1",
// COMPUTED-NEXT:                                                   ),
// COMPUTED-NEXT:                                               ),
// COMPUTED-NEXT:                                           },
// COMPUTED-NEXT:                                       ],
// COMPUTED-NEXT:                                   ),
// COMPUTED-NEXT:                               ),
// COMPUTED-NEXT:                           },
// COMPUTED-NEXT:                       ],
// COMPUTED-NEXT:                   },
// COMPUTED-NEXT:               ),
// COMPUTED-NEXT:               ComputedGoto(
// COMPUTED-NEXT:                   Index {
// COMPUTED-NEXT:                       base: Identifier(
// COMPUTED-NEXT:                           "labels",
// COMPUTED-NEXT:                       ),
// COMPUTED-NEXT:                       index: Identifier(
// COMPUTED-NEXT:                           "n",
// COMPUTED-NEXT:                       ),
// COMPUTED-NEXT:                   },
// COMPUTED-NEXT:               ),
// COMPUTED-NEXT:               Labeled {
// COMPUTED-NEXT:                   label: "L0",
// COMPUTED-NEXT:                   body: Return(
// COMPUTED-NEXT:                       IntegerLiteral(
// COMPUTED-NEXT:                           IntegerLiteral {
// COMPUTED-NEXT:                               value: 0,
// COMPUTED-NEXT:                               radix: Decimal,
// COMPUTED-NEXT:                               suffix: IntegerSuffix {
// COMPUTED-NEXT:                                   unsigned: false,
// COMPUTED-NEXT:                                   size: None,
// COMPUTED-NEXT:                               },
// COMPUTED-NEXT:                               spelling: "0",
// COMPUTED-NEXT:                           },
// COMPUTED-NEXT:                       ),
// COMPUTED-NEXT:                   ),
// COMPUTED-NEXT:               },
// COMPUTED-NEXT:               Labeled {
// COMPUTED-NEXT:                   label: "L1",
// COMPUTED-NEXT:                   body: Return(
// COMPUTED-NEXT:                       IntegerLiteral(
// COMPUTED-NEXT:                           IntegerLiteral {
// COMPUTED-NEXT:                               value: 1,
// COMPUTED-NEXT:                               radix: Decimal,
// COMPUTED-NEXT:                               suffix: IntegerSuffix {
// COMPUTED-NEXT:                                   unsigned: false,
// COMPUTED-NEXT:                                   size: None,
// COMPUTED-NEXT:                               },
// COMPUTED-NEXT:                               spelling: "1",
// COMPUTED-NEXT:                           },
// COMPUTED-NEXT:                       ),
// COMPUTED-NEXT:                   ),
// COMPUTED-NEXT:               },
// COMPUTED-NEXT:           ],
// COMPUTED-NEXT:           provenance: Provenance {
// COMPUTED-NEXT:               file: FileId(
// COMPUTED-NEXT:                   3,
// COMPUTED-NEXT:               ),
// COMPUTED-NEXT:               kind: User,
// COMPUTED-NEXT:               line: 0,
// COMPUTED-NEXT:               header: None,
// COMPUTED-NEXT:           },
// COMPUTED-NEXT:       },
// COMPUTED-NEXT:   )
// COMPUTED-NEXT: decl[1]: Function(
// COMPUTED-NEXT:       FunctionDefinition {
// COMPUTED-NEXT:           specifiers: DeclarationSpecifiers {
// COMPUTED-NEXT:               ty: Integer(
// COMPUTED-NEXT:                   Ranked {
// COMPUTED-NEXT:                       rank: Int,
// COMPUTED-NEXT:                       signed: true,
// COMPUTED-NEXT:                   },
// COMPUTED-NEXT:               ),
// COMPUTED-NEXT:           },
// COMPUTED-NEXT:           declarator: Function {
// COMPUTED-NEXT:               inner: Name(
// COMPUTED-NEXT:                   "vla_sum",
// COMPUTED-NEXT:               ),
// COMPUTED-NEXT:               parameters: Prototype {
// COMPUTED-NEXT:                   parameters: [
// COMPUTED-NEXT:                       ParameterDeclaration {
// COMPUTED-NEXT:                           specifiers: DeclarationSpecifiers {
// COMPUTED-NEXT:                               ty: Integer(
// COMPUTED-NEXT:                                   Ranked {
// COMPUTED-NEXT:                                       rank: Int,
// COMPUTED-NEXT:                                       signed: true,
// COMPUTED-NEXT:                                   },
// COMPUTED-NEXT:                               ),
// COMPUTED-NEXT:                           },
// COMPUTED-NEXT:                           declarator: Name(
// COMPUTED-NEXT:                               "n",
// COMPUTED-NEXT:                           ),
// COMPUTED-NEXT:                       },
// COMPUTED-NEXT:                       ParameterDeclaration {
// COMPUTED-NEXT:                           specifiers: DeclarationSpecifiers {
// COMPUTED-NEXT:                               ty: Integer(
// COMPUTED-NEXT:                                   Ranked {
// COMPUTED-NEXT:                                       rank: Int,
// COMPUTED-NEXT:                                       signed: true,
// COMPUTED-NEXT:                                   },
// COMPUTED-NEXT:                               ),
// COMPUTED-NEXT:                           },
// COMPUTED-NEXT:                           declarator: Array {
// COMPUTED-NEXT:                               inner: Name(
// COMPUTED-NEXT:                                   "arr",
// COMPUTED-NEXT:                               ),
// COMPUTED-NEXT:                               size: Expression(
// COMPUTED-NEXT:                                   Identifier(
// COMPUTED-NEXT:                                       "n",
// COMPUTED-NEXT:                                   ),
// COMPUTED-NEXT:                               ),
// COMPUTED-NEXT:                           },
// COMPUTED-NEXT:                       },
// COMPUTED-NEXT:                   ],
// COMPUTED-NEXT:               },
// COMPUTED-NEXT:           },
// COMPUTED-NEXT:           body: [
// COMPUTED-NEXT:               Decl(
// COMPUTED-NEXT:                   Declaration {
// COMPUTED-NEXT:                       specifiers: DeclarationSpecifiers {
// COMPUTED-NEXT:                           ty: Integer(
// COMPUTED-NEXT:                               Ranked {
// COMPUTED-NEXT:                                   rank: Int,
// COMPUTED-NEXT:                                   signed: true,
// COMPUTED-NEXT:                               },
// COMPUTED-NEXT:                           ),
// COMPUTED-NEXT:                       },
// COMPUTED-NEXT:                       declarators: [
// COMPUTED-NEXT:                           InitDeclarator {
// COMPUTED-NEXT:                               declarator: Array {
// COMPUTED-NEXT:                                   inner: Name(
// COMPUTED-NEXT:                                       "local",
// COMPUTED-NEXT:                                   ),
// COMPUTED-NEXT:                                   size: Expression(
// COMPUTED-NEXT:                                       Identifier(
// COMPUTED-NEXT:                                           "n",
// COMPUTED-NEXT:                                       ),
// COMPUTED-NEXT:                                   ),
// COMPUTED-NEXT:                               },
// COMPUTED-NEXT:                           },
// COMPUTED-NEXT:                       ],
// COMPUTED-NEXT:                   },
// COMPUTED-NEXT:               ),
// COMPUTED-NEXT:               Return(
// COMPUTED-NEXT:                   Binary {
// COMPUTED-NEXT:                       op: Add,
// COMPUTED-NEXT:                       left: Index {
// COMPUTED-NEXT:                           base: Identifier(
// COMPUTED-NEXT:                               "local",
// COMPUTED-NEXT:                           ),
// COMPUTED-NEXT:                           index: IntegerLiteral(
// COMPUTED-NEXT:                               IntegerLiteral {
// COMPUTED-NEXT:                                   value: 0,
// COMPUTED-NEXT:                                   radix: Decimal,
// COMPUTED-NEXT:                                   suffix: IntegerSuffix {
// COMPUTED-NEXT:                                       unsigned: false,
// COMPUTED-NEXT:                                       size: None,
// COMPUTED-NEXT:                                   },
// COMPUTED-NEXT:                                   spelling: "0",
// COMPUTED-NEXT:                               },
// COMPUTED-NEXT:                           ),
// COMPUTED-NEXT:                       },
// COMPUTED-NEXT:                       right: Index {
// COMPUTED-NEXT:                           base: Identifier(
// COMPUTED-NEXT:                               "arr",
// COMPUTED-NEXT:                           ),
// COMPUTED-NEXT:                           index: IntegerLiteral(
// COMPUTED-NEXT:                               IntegerLiteral {
// COMPUTED-NEXT:                                   value: 0,
// COMPUTED-NEXT:                                   radix: Decimal,
// COMPUTED-NEXT:                                   suffix: IntegerSuffix {
// COMPUTED-NEXT:                                       unsigned: false,
// COMPUTED-NEXT:                                       size: None,
// COMPUTED-NEXT:                                   },
// COMPUTED-NEXT:                                   spelling: "0",
// COMPUTED-NEXT:                               },
// COMPUTED-NEXT:                           ),
// COMPUTED-NEXT:                       },
// COMPUTED-NEXT:                   },
// COMPUTED-NEXT:               ),
// COMPUTED-NEXT:           ],
// COMPUTED-NEXT:           provenance: Provenance {
// COMPUTED-NEXT:               file: FileId(
// COMPUTED-NEXT:                   3,
// COMPUTED-NEXT:               ),
// COMPUTED-NEXT:               kind: User,
// COMPUTED-NEXT:               line: 13,
// COMPUTED-NEXT:               header: None,
// COMPUTED-NEXT:           },
// COMPUTED-NEXT:       },
// COMPUTED-NEXT:   )
// SLATE-FILECHECK-END COMPUTED
// SLATE-FILECHECK-BEGIN DOUBLED
// DOUBLED: decl[0]: Function(
// DOUBLED-NEXT:       FunctionDefinition {
// DOUBLED-NEXT:           specifiers: DeclarationSpecifiers {
// DOUBLED-NEXT:               ty: Integer(
// DOUBLED-NEXT:                   Ranked {
// DOUBLED-NEXT:                       rank: Int,
// DOUBLED-NEXT:                       signed: true,
// DOUBLED-NEXT:                   },
// DOUBLED-NEXT:               ),
// DOUBLED-NEXT:           },
// DOUBLED-NEXT:           declarator: Function {
// DOUBLED-NEXT:               inner: Name(
// DOUBLED-NEXT:                   "computed_goto",
// DOUBLED-NEXT:               ),
// DOUBLED-NEXT:               parameters: Prototype {
// DOUBLED-NEXT:                   parameters: [
// DOUBLED-NEXT:                       ParameterDeclaration {
// DOUBLED-NEXT:                           specifiers: DeclarationSpecifiers {
// DOUBLED-NEXT:                               ty: Integer(
// DOUBLED-NEXT:                                   Ranked {
// DOUBLED-NEXT:                                       rank: Int,
// DOUBLED-NEXT:                                       signed: true,
// DOUBLED-NEXT:                                   },
// DOUBLED-NEXT:                               ),
// DOUBLED-NEXT:                           },
// DOUBLED-NEXT:                           declarator: Name(
// DOUBLED-NEXT:                               "n",
// DOUBLED-NEXT:                           ),
// DOUBLED-NEXT:                       },
// DOUBLED-NEXT:                   ],
// DOUBLED-NEXT:               },
// DOUBLED-NEXT:           },
// DOUBLED-NEXT:           body: [
// DOUBLED-NEXT:               Decl(
// DOUBLED-NEXT:                   Declaration {
// DOUBLED-NEXT:                       specifiers: DeclarationSpecifiers {
// DOUBLED-NEXT:                           ty: Void,
// DOUBLED-NEXT:                       },
// DOUBLED-NEXT:                       declarators: [
// DOUBLED-NEXT:                           InitDeclarator {
// DOUBLED-NEXT:                               declarator: Array {
// DOUBLED-NEXT:                                   inner: Pointer {
// DOUBLED-NEXT:                                       qualifiers: Qualifiers,
// DOUBLED-NEXT:                                       inner: Name(
// DOUBLED-NEXT:                                           "labels",
// DOUBLED-NEXT:                                       ),
// DOUBLED-NEXT:                                   },
// DOUBLED-NEXT:                                   size: Expression(
// DOUBLED-NEXT:                                       IntegerLiteral(
// DOUBLED-NEXT:                                           IntegerLiteral {
// DOUBLED-NEXT:                                               value: 2,
// DOUBLED-NEXT:                                               radix: Decimal,
// DOUBLED-NEXT:                                               suffix: IntegerSuffix {
// DOUBLED-NEXT:                                                   unsigned: false,
// DOUBLED-NEXT:                                                   size: None,
// DOUBLED-NEXT:                                               },
// DOUBLED-NEXT:                                               spelling: "2",
// DOUBLED-NEXT:                                           },
// DOUBLED-NEXT:                                       ),
// DOUBLED-NEXT:                                   ),
// DOUBLED-NEXT:                               },
// DOUBLED-NEXT:                               initializer: Some(
// DOUBLED-NEXT:                                   List(
// DOUBLED-NEXT:                                       [
// DOUBLED-NEXT:                                           InitializerItem {
// DOUBLED-NEXT:                                               designators: [],
// DOUBLED-NEXT:                                               value: Expr(
// DOUBLED-NEXT:                                                   LabelAddress(
// DOUBLED-NEXT:                                                       "L0",
// DOUBLED-NEXT:                                                   ),
// DOUBLED-NEXT:                                               ),
// DOUBLED-NEXT:                                           },
// DOUBLED-NEXT:                                           InitializerItem {
// DOUBLED-NEXT:                                               designators: [],
// DOUBLED-NEXT:                                               value: Expr(
// DOUBLED-NEXT:                                                   LabelAddress(
// DOUBLED-NEXT:                                                       "L1",
// DOUBLED-NEXT:                                                   ),
// DOUBLED-NEXT:                                               ),
// DOUBLED-NEXT:                                           },
// DOUBLED-NEXT:                                       ],
// DOUBLED-NEXT:                                   ),
// DOUBLED-NEXT:                               ),
// DOUBLED-NEXT:                           },
// DOUBLED-NEXT:                       ],
// DOUBLED-NEXT:                   },
// DOUBLED-NEXT:               ),
// DOUBLED-NEXT:               Goto(
// DOUBLED-NEXT:                   "L1",
// DOUBLED-NEXT:               ),
// DOUBLED-NEXT:               Labeled {
// DOUBLED-NEXT:                   label: "L0",
// DOUBLED-NEXT:                   body: Return(
// DOUBLED-NEXT:                       IntegerLiteral(
// DOUBLED-NEXT:                           IntegerLiteral {
// DOUBLED-NEXT:                               value: 0,
// DOUBLED-NEXT:                               radix: Decimal,
// DOUBLED-NEXT:                               suffix: IntegerSuffix {
// DOUBLED-NEXT:                                   unsigned: false,
// DOUBLED-NEXT:                                   size: None,
// DOUBLED-NEXT:                               },
// DOUBLED-NEXT:                               spelling: "0",
// DOUBLED-NEXT:                           },
// DOUBLED-NEXT:                       ),
// DOUBLED-NEXT:                   ),
// DOUBLED-NEXT:               },
// DOUBLED-NEXT:               Labeled {
// DOUBLED-NEXT:                   label: "L1",
// DOUBLED-NEXT:                   body: Return(
// DOUBLED-NEXT:                       IntegerLiteral(
// DOUBLED-NEXT:                           IntegerLiteral {
// DOUBLED-NEXT:                               value: 1,
// DOUBLED-NEXT:                               radix: Decimal,
// DOUBLED-NEXT:                               suffix: IntegerSuffix {
// DOUBLED-NEXT:                                   unsigned: false,
// DOUBLED-NEXT:                                   size: None,
// DOUBLED-NEXT:                               },
// DOUBLED-NEXT:                               spelling: "1",
// DOUBLED-NEXT:                           },
// DOUBLED-NEXT:                       ),
// DOUBLED-NEXT:                   ),
// DOUBLED-NEXT:               },
// DOUBLED-NEXT:           ],
// DOUBLED-NEXT:           provenance: Provenance {
// DOUBLED-NEXT:               file: FileId(
// DOUBLED-NEXT:                   3,
// DOUBLED-NEXT:               ),
// DOUBLED-NEXT:               kind: User,
// DOUBLED-NEXT:               line: 0,
// DOUBLED-NEXT:               header: None,
// DOUBLED-NEXT:           },
// DOUBLED-NEXT:       },
// DOUBLED-NEXT:   )
// DOUBLED-NEXT: decl[1]: Function(
// DOUBLED-NEXT:       FunctionDefinition {
// DOUBLED-NEXT:           specifiers: DeclarationSpecifiers {
// DOUBLED-NEXT:               ty: Integer(
// DOUBLED-NEXT:                   Ranked {
// DOUBLED-NEXT:                       rank: Int,
// DOUBLED-NEXT:                       signed: true,
// DOUBLED-NEXT:                   },
// DOUBLED-NEXT:               ),
// DOUBLED-NEXT:           },
// DOUBLED-NEXT:           declarator: Function {
// DOUBLED-NEXT:               inner: Name(
// DOUBLED-NEXT:                   "vla_sum",
// DOUBLED-NEXT:               ),
// DOUBLED-NEXT:               parameters: Prototype {
// DOUBLED-NEXT:                   parameters: [
// DOUBLED-NEXT:                       ParameterDeclaration {
// DOUBLED-NEXT:                           specifiers: DeclarationSpecifiers {
// DOUBLED-NEXT:                               ty: Integer(
// DOUBLED-NEXT:                                   Ranked {
// DOUBLED-NEXT:                                       rank: Int,
// DOUBLED-NEXT:                                       signed: true,
// DOUBLED-NEXT:                                   },
// DOUBLED-NEXT:                               ),
// DOUBLED-NEXT:                           },
// DOUBLED-NEXT:                           declarator: Name(
// DOUBLED-NEXT:                               "n",
// DOUBLED-NEXT:                           ),
// DOUBLED-NEXT:                       },
// DOUBLED-NEXT:                       ParameterDeclaration {
// DOUBLED-NEXT:                           specifiers: DeclarationSpecifiers {
// DOUBLED-NEXT:                               ty: Integer(
// DOUBLED-NEXT:                                   Ranked {
// DOUBLED-NEXT:                                       rank: Int,
// DOUBLED-NEXT:                                       signed: true,
// DOUBLED-NEXT:                                   },
// DOUBLED-NEXT:                               ),
// DOUBLED-NEXT:                           },
// DOUBLED-NEXT:                           declarator: Array {
// DOUBLED-NEXT:                               inner: Name(
// DOUBLED-NEXT:                                   "arr",
// DOUBLED-NEXT:                               ),
// DOUBLED-NEXT:                               size: Expression(
// DOUBLED-NEXT:                                   Identifier(
// DOUBLED-NEXT:                                       "n",
// DOUBLED-NEXT:                                   ),
// DOUBLED-NEXT:                               ),
// DOUBLED-NEXT:                           },
// DOUBLED-NEXT:                       },
// DOUBLED-NEXT:                   ],
// DOUBLED-NEXT:               },
// DOUBLED-NEXT:           },
// DOUBLED-NEXT:           body: [
// DOUBLED-NEXT:               Decl(
// DOUBLED-NEXT:                   Declaration {
// DOUBLED-NEXT:                       specifiers: DeclarationSpecifiers {
// DOUBLED-NEXT:                           ty: Integer(
// DOUBLED-NEXT:                               Ranked {
// DOUBLED-NEXT:                                   rank: Int,
// DOUBLED-NEXT:                                   signed: true,
// DOUBLED-NEXT:                               },
// DOUBLED-NEXT:                           ),
// DOUBLED-NEXT:                       },
// DOUBLED-NEXT:                       declarators: [
// DOUBLED-NEXT:                           InitDeclarator {
// DOUBLED-NEXT:                               declarator: Array {
// DOUBLED-NEXT:                                   inner: Name(
// DOUBLED-NEXT:                                       "local",
// DOUBLED-NEXT:                                   ),
// DOUBLED-NEXT:                                   size: Expression(
// DOUBLED-NEXT:                                       Binary {
// DOUBLED-NEXT:                                           op: Mul,
// DOUBLED-NEXT:                                           left: Identifier(
// DOUBLED-NEXT:                                               "n",
// DOUBLED-NEXT:                                           ),
// DOUBLED-NEXT:                                           right: IntegerLiteral(
// DOUBLED-NEXT:                                               IntegerLiteral {
// DOUBLED-NEXT:                                                   value: 2,
// DOUBLED-NEXT:                                                   radix: Decimal,
// DOUBLED-NEXT:                                                   suffix: IntegerSuffix {
// DOUBLED-NEXT:                                                       unsigned: false,
// DOUBLED-NEXT:                                                       size: None,
// DOUBLED-NEXT:                                                   },
// DOUBLED-NEXT:                                                   spelling: "2",
// DOUBLED-NEXT:                                               },
// DOUBLED-NEXT:                                           ),
// DOUBLED-NEXT:                                       },
// DOUBLED-NEXT:                                   ),
// DOUBLED-NEXT:                               },
// DOUBLED-NEXT:                           },
// DOUBLED-NEXT:                       ],
// DOUBLED-NEXT:                   },
// DOUBLED-NEXT:               ),
// DOUBLED-NEXT:               Return(
// DOUBLED-NEXT:                   Binary {
// DOUBLED-NEXT:                       op: Add,
// DOUBLED-NEXT:                       left: Index {
// DOUBLED-NEXT:                           base: Identifier(
// DOUBLED-NEXT:                               "local",
// DOUBLED-NEXT:                           ),
// DOUBLED-NEXT:                           index: IntegerLiteral(
// DOUBLED-NEXT:                               IntegerLiteral {
// DOUBLED-NEXT:                                   value: 0,
// DOUBLED-NEXT:                                   radix: Decimal,
// DOUBLED-NEXT:                                   suffix: IntegerSuffix {
// DOUBLED-NEXT:                                       unsigned: false,
// DOUBLED-NEXT:                                       size: None,
// DOUBLED-NEXT:                                   },
// DOUBLED-NEXT:                                   spelling: "0",
// DOUBLED-NEXT:                               },
// DOUBLED-NEXT:                           ),
// DOUBLED-NEXT:                       },
// DOUBLED-NEXT:                       right: Index {
// DOUBLED-NEXT:                           base: Identifier(
// DOUBLED-NEXT:                               "arr",
// DOUBLED-NEXT:                           ),
// DOUBLED-NEXT:                           index: IntegerLiteral(
// DOUBLED-NEXT:                               IntegerLiteral {
// DOUBLED-NEXT:                                   value: 0,
// DOUBLED-NEXT:                                   radix: Decimal,
// DOUBLED-NEXT:                                   suffix: IntegerSuffix {
// DOUBLED-NEXT:                                       unsigned: false,
// DOUBLED-NEXT:                                       size: None,
// DOUBLED-NEXT:                                   },
// DOUBLED-NEXT:                                   spelling: "0",
// DOUBLED-NEXT:                               },
// DOUBLED-NEXT:                           ),
// DOUBLED-NEXT:                       },
// DOUBLED-NEXT:                   },
// DOUBLED-NEXT:               ),
// DOUBLED-NEXT:           ],
// DOUBLED-NEXT:           provenance: Provenance {
// DOUBLED-NEXT:               file: FileId(
// DOUBLED-NEXT:                   3,
// DOUBLED-NEXT:               ),
// DOUBLED-NEXT:               kind: User,
// DOUBLED-NEXT:               line: 13,
// DOUBLED-NEXT:               header: None,
// DOUBLED-NEXT:           },
// DOUBLED-NEXT:       },
// DOUBLED-NEXT:   )
// SLATE-FILECHECK-END DOUBLED
