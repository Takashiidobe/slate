// SLATE-FILECHECK-DEFINES DEFAULT

void
dr106_1(void *pv, int i)
{
	*pv;
	i ? *pv : *pv;
	*pv, *pv;
}

void
dr106_2(const void *pcv, volatile void *pvv, int i)
{
	*pcv;
	i ? *pcv : *pcv;
	*pcv, *pcv;

	*pvv;
	i ? *pvv : *pvv;
	*pvv, *pvv;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "dr106_1",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Prototype {
// DEFAULT-NEXT:                   parameters: [
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "pv",
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
// DEFAULT-NEXT:                               "i",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Unary {
// DEFAULT-NEXT:                       op: Deref,
// DEFAULT-NEXT:                       operand: Identifier(
// DEFAULT-NEXT:                           "pv",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Conditional {
// DEFAULT-NEXT:                       condition: Identifier(
// DEFAULT-NEXT:                           "i",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       then_value: Some(
// DEFAULT-NEXT:                           Unary {
// DEFAULT-NEXT:                               op: Deref,
// DEFAULT-NEXT:                               operand: Identifier(
// DEFAULT-NEXT:                                   "pv",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       else_value: Unary {
// DEFAULT-NEXT:                           op: Deref,
// DEFAULT-NEXT:                           operand: Identifier(
// DEFAULT-NEXT:                               "pv",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Comma {
// DEFAULT-NEXT:                       left: Unary {
// DEFAULT-NEXT:                           op: Deref,
// DEFAULT-NEXT:                           operand: Identifier(
// DEFAULT-NEXT:                               "pv",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       right: Unary {
// DEFAULT-NEXT:                           op: Deref,
// DEFAULT-NEXT:                           operand: Identifier(
// DEFAULT-NEXT:                               "pv",
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
// DEFAULT-NEXT:               line: 1,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "dr106_2",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Prototype {
// DEFAULT-NEXT:                   parameters: [
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               qualifiers: Qualifiers {
// DEFAULT-NEXT:                                   is_const: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "pcv",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Void,
// DEFAULT-NEXT:                               qualifiers: Qualifiers {
// DEFAULT-NEXT:                                   is_volatile: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "pvv",
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
// DEFAULT-NEXT:                               "i",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Unary {
// DEFAULT-NEXT:                       op: Deref,
// DEFAULT-NEXT:                       operand: Identifier(
// DEFAULT-NEXT:                           "pcv",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Conditional {
// DEFAULT-NEXT:                       condition: Identifier(
// DEFAULT-NEXT:                           "i",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       then_value: Some(
// DEFAULT-NEXT:                           Unary {
// DEFAULT-NEXT:                               op: Deref,
// DEFAULT-NEXT:                               operand: Identifier(
// DEFAULT-NEXT:                                   "pcv",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       else_value: Unary {
// DEFAULT-NEXT:                           op: Deref,
// DEFAULT-NEXT:                           operand: Identifier(
// DEFAULT-NEXT:                               "pcv",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Comma {
// DEFAULT-NEXT:                       left: Unary {
// DEFAULT-NEXT:                           op: Deref,
// DEFAULT-NEXT:                           operand: Identifier(
// DEFAULT-NEXT:                               "pcv",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       right: Unary {
// DEFAULT-NEXT:                           op: Deref,
// DEFAULT-NEXT:                           operand: Identifier(
// DEFAULT-NEXT:                               "pcv",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Unary {
// DEFAULT-NEXT:                       op: Deref,
// DEFAULT-NEXT:                       operand: Identifier(
// DEFAULT-NEXT:                           "pvv",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Conditional {
// DEFAULT-NEXT:                       condition: Identifier(
// DEFAULT-NEXT:                           "i",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       then_value: Some(
// DEFAULT-NEXT:                           Unary {
// DEFAULT-NEXT:                               op: Deref,
// DEFAULT-NEXT:                               operand: Identifier(
// DEFAULT-NEXT:                                   "pvv",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       else_value: Unary {
// DEFAULT-NEXT:                           op: Deref,
// DEFAULT-NEXT:                           operand: Identifier(
// DEFAULT-NEXT:                               "pvv",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Comma {
// DEFAULT-NEXT:                       left: Unary {
// DEFAULT-NEXT:                           op: Deref,
// DEFAULT-NEXT:                           operand: Identifier(
// DEFAULT-NEXT:                               "pvv",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       right: Unary {
// DEFAULT-NEXT:                           op: Deref,
// DEFAULT-NEXT:                           operand: Identifier(
// DEFAULT-NEXT:                               "pvv",
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
// DEFAULT-NEXT:               line: 9,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
