// SLATE-FILECHECK-DEFINES DEFAULT

/* Copyright (C) 2000  Free Software Foundation  */
/* Contributed by Alexandre Oliva <aoliva@redhat.com> */

int
foo () 
{
  while (1)
    {
      int a;
      char b;
      /* gcse should not merge these asm statements, since their
	 output operands have different modes.  */
      __asm__("":"=r" (a)); __asm__("":"=r" (b));
      if (b)
	return a;
    }
}

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
// DEFAULT-NEXT:                   "foo",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Empty,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               While {
// DEFAULT-NEXT:                   condition: IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 1,
// DEFAULT-NEXT:                           radix: Decimal,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "1",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclaratorKind {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "a",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclaratorKind {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "b",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Asm(
// DEFAULT-NEXT:                           GnuAsm {
// DEFAULT-NEXT:                               template: "",
// DEFAULT-NEXT:                               operands: Some(
// DEFAULT-NEXT:                                   AsmOperands {
// DEFAULT-NEXT:                                       pieces: [],
// DEFAULT-NEXT:                                       outputs: [
// DEFAULT-NEXT:                                           AsmOperand {
// DEFAULT-NEXT:                                               constraint: AsmConstraint {
// DEFAULT-NEXT:                                                   alternatives: [
// DEFAULT-NEXT:                                                       AsmConstraintAlternative {
// DEFAULT-NEXT:                                                           modifiers: [
// DEFAULT-NEXT:                                                               Overwrite,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           location: Letters(
// DEFAULT-NEXT:                                                               "r",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               expr: Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Asm(
// DEFAULT-NEXT:                           GnuAsm {
// DEFAULT-NEXT:                               template: "",
// DEFAULT-NEXT:                               operands: Some(
// DEFAULT-NEXT:                                   AsmOperands {
// DEFAULT-NEXT:                                       pieces: [],
// DEFAULT-NEXT:                                       outputs: [
// DEFAULT-NEXT:                                           AsmOperand {
// DEFAULT-NEXT:                                               constraint: AsmConstraint {
// DEFAULT-NEXT:                                                   alternatives: [
// DEFAULT-NEXT:                                                       AsmConstraintAlternative {
// DEFAULT-NEXT:                                                           modifiers: [
// DEFAULT-NEXT:                                                               Overwrite,
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                           location: Letters(
// DEFAULT-NEXT:                                                               "r",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               expr: Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       If {
// DEFAULT-NEXT:                           condition: Identifier(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_branch: [
// DEFAULT-NEXT:                               Return(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "a",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                           else_branch: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
