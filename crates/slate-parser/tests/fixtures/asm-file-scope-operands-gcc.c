int x;
asm("%0" : : "r"(x));

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:                       "x",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Asm(
// DEFAULT-NEXT:       GnuAsm {
// DEFAULT-NEXT:           template: "%0",
// DEFAULT-NEXT:           operands: Some(
// DEFAULT-NEXT:               AsmOperands {
// DEFAULT-NEXT:                   pieces: [
// DEFAULT-NEXT:                       Operand {
// DEFAULT-NEXT:                           index: 0,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   inputs: [
// DEFAULT-NEXT:                       AsmOperand {
// DEFAULT-NEXT:                           constraint: AsmConstraint {
// DEFAULT-NEXT:                               alternatives: [
// DEFAULT-NEXT:                                   AsmConstraintAlternative {
// DEFAULT-NEXT:                                       location: Letters(
// DEFAULT-NEXT:                                           "r",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           expr: Identifier(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
