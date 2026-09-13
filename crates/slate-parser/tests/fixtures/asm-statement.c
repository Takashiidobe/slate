void operands(int x, int y, int *p) {
  asm("basic %eax %0");
  __asm__ volatile inline("mov %[in], %0 %% %= %{att%|intel%} %a1 %cc2"
                          : [out] "=&r,{rax}"(x)
                          : [in] "+%-rm,0"(y), "[out]"(*p)
                          : "memory", "cc", "unwind", "%rdx", "not_a_register");
}

void jumps(int x) {
  asm goto("jmp %l[done] %l1 %2" : : "r"(x) : : done, other);
  asm goto("jmp %l0" : : : : done);
  x = 1;
done:
  return;
other:
  x = 0;
}

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "operands",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "x",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "y",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "p",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   GnuAsm {
// DEFAULT-NEXT:                       template: "basic %eax %0",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   GnuAsm {
// DEFAULT-NEXT:                       qualifiers: [
// DEFAULT-NEXT:                           Volatile,
// DEFAULT-NEXT:                           Inline,
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       template: "mov %[in], %0 %% %= %{att%|intel%} %a1 %cc2",
// DEFAULT-NEXT:                       operands: Some(
// DEFAULT-NEXT:                           AsmOperands {
// DEFAULT-NEXT:                               pieces: [
// DEFAULT-NEXT:                                   Text(
// DEFAULT-NEXT:                                       "mov ",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Operand {
// DEFAULT-NEXT:                                       index: 1,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   Text(
// DEFAULT-NEXT:                                       ", ",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Operand {
// DEFAULT-NEXT:                                       index: 0,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   Text(
// DEFAULT-NEXT:                                       " ",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Percent,
// DEFAULT-NEXT:                                   Text(
// DEFAULT-NEXT:                                       " ",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   UniqueId,
// DEFAULT-NEXT:                                   Text(
// DEFAULT-NEXT:                                       " ",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   LBrace,
// DEFAULT-NEXT:                                   Text(
// DEFAULT-NEXT:                                       "att",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Pipe,
// DEFAULT-NEXT:                                   Text(
// DEFAULT-NEXT:                                       "intel",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   RBrace,
// DEFAULT-NEXT:                                   Text(
// DEFAULT-NEXT:                                       " ",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Operand {
// DEFAULT-NEXT:                                       index: 1,
// DEFAULT-NEXT:                                       modifier: Some(
// DEFAULT-NEXT:                                           'a',
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   Text(
// DEFAULT-NEXT:                                       " ",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Operand {
// DEFAULT-NEXT:                                       index: 2,
// DEFAULT-NEXT:                                       modifier: Some(
// DEFAULT-NEXT:                                           'c',
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               outputs: [
// DEFAULT-NEXT:                                   AsmOperand {
// DEFAULT-NEXT:                                       name: Some(
// DEFAULT-NEXT:                                           "out",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       constraint: AsmConstraint {
// DEFAULT-NEXT:                                           alternatives: [
// DEFAULT-NEXT:                                               AsmConstraintAlternative {
// DEFAULT-NEXT:                                                   modifiers: [
// DEFAULT-NEXT:                                                       Overwrite,
// DEFAULT-NEXT:                                                       EarlyClobber,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   location: Letters(
// DEFAULT-NEXT:                                                       "r",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               AsmConstraintAlternative {
// DEFAULT-NEXT:                                                   location: HardRegister(
// DEFAULT-NEXT:                                                       X86(
// DEFAULT-NEXT:                                                           X86Register {
// DEFAULT-NEXT:                                                               spelling: "rax",
// DEFAULT-NEXT:                                                               number: 0,
// DEFAULT-NEXT:                                                               canonical: "ax",
// DEFAULT-NEXT:                                                               width: Some(
// DEFAULT-NEXT:                                                                   Bits64,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       expr: Const(
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               inputs: [
// DEFAULT-NEXT:                                   AsmOperand {
// DEFAULT-NEXT:                                       name: Some(
// DEFAULT-NEXT:                                           "in",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       constraint: AsmConstraint {
// DEFAULT-NEXT:                                           alternatives: [
// DEFAULT-NEXT:                                               AsmConstraintAlternative {
// DEFAULT-NEXT:                                                   modifiers: [
// DEFAULT-NEXT:                                                       ReadWrite,
// DEFAULT-NEXT:                                                       Commutative,
// DEFAULT-NEXT:                                                       Pic,
// DEFAULT-NEXT:                                                   ],
// DEFAULT-NEXT:                                                   location: Letters(
// DEFAULT-NEXT:                                                       "rm",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               AsmConstraintAlternative {
// DEFAULT-NEXT:                                                   location: Matching(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       expr: Const(
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "y",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   AsmOperand {
// DEFAULT-NEXT:                                       constraint: AsmConstraint {
// DEFAULT-NEXT:                                           alternatives: [
// DEFAULT-NEXT:                                               AsmConstraintAlternative {
// DEFAULT-NEXT:                                                   location: Matching(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       expr: Const(
// DEFAULT-NEXT:                                           Deref(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "p",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               clobbers: [
// DEFAULT-NEXT:                                   Memory,
// DEFAULT-NEXT:                                   Cc,
// DEFAULT-NEXT:                                   Unwind,
// DEFAULT-NEXT:                                   Register(
// DEFAULT-NEXT:                                       X86(
// DEFAULT-NEXT:                                           X86Register {
// DEFAULT-NEXT:                                               spelling: "%rdx",
// DEFAULT-NEXT:                                               number: 1,
// DEFAULT-NEXT:                                               canonical: "dx",
// DEFAULT-NEXT:                                               width: Some(
// DEFAULT-NEXT:                                                   Bits64,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Register(
// DEFAULT-NEXT:                                       Other(
// DEFAULT-NEXT:                                           "not_a_register",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "jumps",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: true,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "x",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   GnuAsm {
// DEFAULT-NEXT:                       qualifiers: [
// DEFAULT-NEXT:                           Goto,
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       template: "jmp %l[done] %l1 %2",
// DEFAULT-NEXT:                       operands: Some(
// DEFAULT-NEXT:                           AsmOperands {
// DEFAULT-NEXT:                               pieces: [
// DEFAULT-NEXT:                                   Text(
// DEFAULT-NEXT:                                       "jmp ",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Label(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Text(
// DEFAULT-NEXT:                                       " ",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Label(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Text(
// DEFAULT-NEXT:                                       " ",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Label(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               inputs: [
// DEFAULT-NEXT:                                   AsmOperand {
// DEFAULT-NEXT:                                       constraint: AsmConstraint {
// DEFAULT-NEXT:                                           alternatives: [
// DEFAULT-NEXT:                                               AsmConstraintAlternative {
// DEFAULT-NEXT:                                                   location: Letters(
// DEFAULT-NEXT:                                                       "r",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       expr: Const(
// DEFAULT-NEXT:                                           Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               labels: [
// DEFAULT-NEXT:                                   "done",
// DEFAULT-NEXT:                                   "other",
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Asm(
// DEFAULT-NEXT:                   GnuAsm {
// DEFAULT-NEXT:                       qualifiers: [
// DEFAULT-NEXT:                           Goto,
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       template: "jmp %l0",
// DEFAULT-NEXT:                       operands: Some(
// DEFAULT-NEXT:                           AsmOperands {
// DEFAULT-NEXT:                               pieces: [
// DEFAULT-NEXT:                                   Text(
// DEFAULT-NEXT:                                       "jmp ",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Label(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               labels: [
// DEFAULT-NEXT:                                   "done",
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               1,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Labeled(
// DEFAULT-NEXT:                   "done",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               ReturnVoid,
// DEFAULT-NEXT:               Labeled(
// DEFAULT-NEXT:                   "other",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 8,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
