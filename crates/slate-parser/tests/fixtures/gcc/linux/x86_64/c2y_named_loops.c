void named_loops(int n) {
outer:
    for (int i = 0; i < n; ++i) {
inner:
        while (n) {
            if (n == 1) continue outer;
            if (n == 2) break inner;
            break;
        }
        continue;
    }
again:
    do {
        if (n) continue again;
        break again;
    } while (n);
choice:
    switch (n) {
    case 0: break choice;
    default: break;
    }
}

// SLATE-FILECHECK-AST
// SLATE-FILECHECK-DEFINES C2Y
// SLATE-FILECHECK-STD C2Y c2y
// SLATE-FILECHECK-DEFINES GNU2Y
// SLATE-FILECHECK-STD GNU2Y gnu2y
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-ERROR C23
// SLATE-FILECHECK-DEFINES GNU23
// SLATE-FILECHECK-STD GNU23 gnu23
// SLATE-FILECHECK-ERROR GNU23
// SLATE-FILECHECK-IR-ERROR IR
// SLATE-FILECHECK-STD IR c2y

// SLATE-FILECHECK-BEGIN C23
// C23: Error:   × named loop jumps require C2y
// C23: ╰─▶ named loop jumps require C2y
// C23: ╭─[tests/fixtures/gcc/linux/x86_64/c2y_named_loops.c:6:34]
// C23: 5 │         while (n) {
// C23: 6 │             if (n == 1) continue outer;
// C23: ·                                  ─────
// C23: 7 │             if (n == 2) break inner;
// C23: ╰────
// SLATE-FILECHECK-END C23
// SLATE-FILECHECK-BEGIN GNU23
// GNU23: Error:   × named loop jumps require C2y
// GNU23: ╰─▶ named loop jumps require C2y
// GNU23: ╭─[tests/fixtures/gcc/linux/x86_64/c2y_named_loops.c:6:34]
// GNU23: 5 │         while (n) {
// GNU23: 6 │             if (n == 1) continue outer;
// GNU23: ·                                  ─────
// GNU23: 7 │             if (n == 2) break inner;
// GNU23: ╰────
// SLATE-FILECHECK-END GNU23
// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR: × not implemented: named loop jumps
// IR: ╭─[tests/fixtures/gcc/linux/x86_64/c2y_named_loops.c:6:25]
// IR: 5 │         while (n) {
// IR: 6 │             if (n == 1) continue outer;
// IR: ·                         ───────────────
// IR: 7 │             if (n == 2) break inner;
// IR: ╰────
// SLATE-FILECHECK-END IR
// SLATE-FILECHECK-BEGIN C2Y
// C2Y: decl[{{[0-9]+}}]: Function(
// C2Y-NEXT:       FunctionDefinition {
// C2Y-NEXT:           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:               ty: Void,
// C2Y-NEXT:           },
// C2Y-NEXT:           declarator: Function {
// C2Y-NEXT:               inner: Name(
// C2Y-NEXT:                   "named_loops",
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
// C2Y-NEXT:               Labeled {
// C2Y-NEXT:                   label: "outer",
// C2Y-NEXT:                   body: For {
// C2Y-NEXT:                       init: Some(
// C2Y-NEXT:                           Decl(
// C2Y-NEXT:                               Declaration {
// C2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                       ty: Integer(
// C2Y-NEXT:                                           Ranked {
// C2Y-NEXT:                                               rank: Int,
// C2Y-NEXT:                                               signed: true,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   declarators: [
// C2Y-NEXT:                                       InitDeclaratorKind {
// C2Y-NEXT:                                           declarator: Name(
// C2Y-NEXT:                                               "i",
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                           initializer: Some(
// C2Y-NEXT:                                               Expr(
// C2Y-NEXT:                                                   IntegerLiteral(
// C2Y-NEXT:                                                       IntegerLiteral {
// C2Y-NEXT:                                                           value: 0,
// C2Y-NEXT:                                                           radix: Decimal,
// C2Y-NEXT:                                                           suffix: IntegerSuffix {
// C2Y-NEXT:                                                               unsigned: false,
// C2Y-NEXT:                                                               size: None,
// C2Y-NEXT:                                                           },
// C2Y-NEXT:                                                           spelling: "0",
// C2Y-NEXT:                                                       },
// C2Y-NEXT:                                                   ),
// C2Y-NEXT:                                               ),
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ],
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       ),
// C2Y-NEXT:                       condition: Some(
// C2Y-NEXT:                           Binary {
// C2Y-NEXT:                               op: Less,
// C2Y-NEXT:                               left: Identifier(
// C2Y-NEXT:                                   "i",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               right: Identifier(
// C2Y-NEXT:                                   "n",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ),
// C2Y-NEXT:                       increment: Some(
// C2Y-NEXT:                           Unary {
// C2Y-NEXT:                               op: PreIncrement,
// C2Y-NEXT:                               operand: Identifier(
// C2Y-NEXT:                                   "i",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ),
// C2Y-NEXT:                       body: Block(
// C2Y-NEXT:                           [
// C2Y-NEXT:                               Labeled {
// C2Y-NEXT:                                   label: "inner",
// C2Y-NEXT:                                   body: While {
// C2Y-NEXT:                                       condition: Identifier(
// C2Y-NEXT:                                           "n",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                       body: Block(
// C2Y-NEXT:                                           [
// C2Y-NEXT:                                               If {
// C2Y-NEXT:                                                   condition: Binary {
// C2Y-NEXT:                                                       op: Equal,
// C2Y-NEXT:                                                       left: Identifier(
// C2Y-NEXT:                                                           "n",
// C2Y-NEXT:                                                       ),
// C2Y-NEXT:                                                       right: IntegerLiteral(
// C2Y-NEXT:                                                           IntegerLiteral {
// C2Y-NEXT:                                                               value: 1,
// C2Y-NEXT:                                                               radix: Decimal,
// C2Y-NEXT:                                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                                   unsigned: false,
// C2Y-NEXT:                                                                   size: None,
// C2Y-NEXT:                                                               },
// C2Y-NEXT:                                                               spelling: "1",
// C2Y-NEXT:                                                           },
// C2Y-NEXT:                                                       ),
// C2Y-NEXT:                                                   },
// C2Y-NEXT:                                                   then_branch: NamedContinue(
// C2Y-NEXT:                                                       "outer",
// C2Y-NEXT:                                                   ),
// C2Y-NEXT:                                                   else_branch: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               If {
// C2Y-NEXT:                                                   condition: Binary {
// C2Y-NEXT:                                                       op: Equal,
// C2Y-NEXT:                                                       left: Identifier(
// C2Y-NEXT:                                                           "n",
// C2Y-NEXT:                                                       ),
// C2Y-NEXT:                                                       right: IntegerLiteral(
// C2Y-NEXT:                                                           IntegerLiteral {
// C2Y-NEXT:                                                               value: 2,
// C2Y-NEXT:                                                               radix: Decimal,
// C2Y-NEXT:                                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                                   unsigned: false,
// C2Y-NEXT:                                                                   size: None,
// C2Y-NEXT:                                                               },
// C2Y-NEXT:                                                               spelling: "2",
// C2Y-NEXT:                                                           },
// C2Y-NEXT:                                                       ),
// C2Y-NEXT:                                                   },
// C2Y-NEXT:                                                   then_branch: NamedBreak(
// C2Y-NEXT:                                                       "inner",
// C2Y-NEXT:                                                   ),
// C2Y-NEXT:                                                   else_branch: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               Break,
// C2Y-NEXT:                                           ],
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               },
// C2Y-NEXT:                               Continue,
// C2Y-NEXT:                           ],
// C2Y-NEXT:                       ),
// C2Y-NEXT:                   },
// C2Y-NEXT:               },
// C2Y-NEXT:               Labeled {
// C2Y-NEXT:                   label: "again",
// C2Y-NEXT:                   body: DoWhile {
// C2Y-NEXT:                       body: Block(
// C2Y-NEXT:                           [
// C2Y-NEXT:                               If {
// C2Y-NEXT:                                   condition: Identifier(
// C2Y-NEXT:                                       "n",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   then_branch: NamedContinue(
// C2Y-NEXT:                                       "again",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   else_branch: None,
// C2Y-NEXT:                               },
// C2Y-NEXT:                               NamedBreak(
// C2Y-NEXT:                                   "again",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           ],
// C2Y-NEXT:                       ),
// C2Y-NEXT:                       condition: Identifier(
// C2Y-NEXT:                           "n",
// C2Y-NEXT:                       ),
// C2Y-NEXT:                   },
// C2Y-NEXT:               },
// C2Y-NEXT:               Labeled {
// C2Y-NEXT:                   label: "choice",
// C2Y-NEXT:                   body: Switch {
// C2Y-NEXT:                       discriminant: Identifier(
// C2Y-NEXT:                           "n",
// C2Y-NEXT:                       ),
// C2Y-NEXT:                       body: Block(
// C2Y-NEXT:                           [
// C2Y-NEXT:                               SwitchLabel {
// C2Y-NEXT:                                   label: Case(
// C2Y-NEXT:                                       IntegerLiteral(
// C2Y-NEXT:                                           IntegerLiteral {
// C2Y-NEXT:                                               value: 0,
// C2Y-NEXT:                                               radix: Decimal,
// C2Y-NEXT:                                               suffix: IntegerSuffix {
// C2Y-NEXT:                                                   unsigned: false,
// C2Y-NEXT:                                                   size: None,
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               spelling: "0",
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   body: NamedBreak(
// C2Y-NEXT:                                       "choice",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                               SwitchLabel {
// C2Y-NEXT:                                   label: Default,
// C2Y-NEXT:                                   body: Break,
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ],
// C2Y-NEXT:                       ),
// C2Y-NEXT:                   },
// C2Y-NEXT:               },
// C2Y-NEXT:           ],
// C2Y-NEXT:       },
// C2Y-NEXT:   )
// SLATE-FILECHECK-END C2Y
// SLATE-FILECHECK-BEGIN GNU2Y
// GNU2Y: decl[{{[0-9]+}}]: Function(
// GNU2Y-NEXT:       FunctionDefinition {
// GNU2Y-NEXT:           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:               ty: Void,
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           declarator: Function {
// GNU2Y-NEXT:               inner: Name(
// GNU2Y-NEXT:                   "named_loops",
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
// GNU2Y-NEXT:               Labeled {
// GNU2Y-NEXT:                   label: "outer",
// GNU2Y-NEXT:                   body: For {
// GNU2Y-NEXT:                       init: Some(
// GNU2Y-NEXT:                           Decl(
// GNU2Y-NEXT:                               Declaration {
// GNU2Y-NEXT:                                   specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                       ty: Integer(
// GNU2Y-NEXT:                                           Ranked {
// GNU2Y-NEXT:                                               rank: Int,
// GNU2Y-NEXT:                                               signed: true,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   declarators: [
// GNU2Y-NEXT:                                       InitDeclaratorKind {
// GNU2Y-NEXT:                                           declarator: Name(
// GNU2Y-NEXT:                                               "i",
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                           initializer: Some(
// GNU2Y-NEXT:                                               Expr(
// GNU2Y-NEXT:                                                   IntegerLiteral(
// GNU2Y-NEXT:                                                       IntegerLiteral {
// GNU2Y-NEXT:                                                           value: 0,
// GNU2Y-NEXT:                                                           radix: Decimal,
// GNU2Y-NEXT:                                                           suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                               unsigned: false,
// GNU2Y-NEXT:                                                               size: None,
// GNU2Y-NEXT:                                                           },
// GNU2Y-NEXT:                                                           spelling: "0",
// GNU2Y-NEXT:                                                       },
// GNU2Y-NEXT:                                                   ),
// GNU2Y-NEXT:                                               ),
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ],
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                       condition: Some(
// GNU2Y-NEXT:                           Binary {
// GNU2Y-NEXT:                               op: Less,
// GNU2Y-NEXT:                               left: Identifier(
// GNU2Y-NEXT:                                   "i",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               right: Identifier(
// GNU2Y-NEXT:                                   "n",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                       increment: Some(
// GNU2Y-NEXT:                           Unary {
// GNU2Y-NEXT:                               op: PreIncrement,
// GNU2Y-NEXT:                               operand: Identifier(
// GNU2Y-NEXT:                                   "i",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                       body: Block(
// GNU2Y-NEXT:                           [
// GNU2Y-NEXT:                               Labeled {
// GNU2Y-NEXT:                                   label: "inner",
// GNU2Y-NEXT:                                   body: While {
// GNU2Y-NEXT:                                       condition: Identifier(
// GNU2Y-NEXT:                                           "n",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                       body: Block(
// GNU2Y-NEXT:                                           [
// GNU2Y-NEXT:                                               If {
// GNU2Y-NEXT:                                                   condition: Binary {
// GNU2Y-NEXT:                                                       op: Equal,
// GNU2Y-NEXT:                                                       left: Identifier(
// GNU2Y-NEXT:                                                           "n",
// GNU2Y-NEXT:                                                       ),
// GNU2Y-NEXT:                                                       right: IntegerLiteral(
// GNU2Y-NEXT:                                                           IntegerLiteral {
// GNU2Y-NEXT:                                                               value: 1,
// GNU2Y-NEXT:                                                               radix: Decimal,
// GNU2Y-NEXT:                                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                                   unsigned: false,
// GNU2Y-NEXT:                                                                   size: None,
// GNU2Y-NEXT:                                                               },
// GNU2Y-NEXT:                                                               spelling: "1",
// GNU2Y-NEXT:                                                           },
// GNU2Y-NEXT:                                                       ),
// GNU2Y-NEXT:                                                   },
// GNU2Y-NEXT:                                                   then_branch: NamedContinue(
// GNU2Y-NEXT:                                                       "outer",
// GNU2Y-NEXT:                                                   ),
// GNU2Y-NEXT:                                                   else_branch: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               If {
// GNU2Y-NEXT:                                                   condition: Binary {
// GNU2Y-NEXT:                                                       op: Equal,
// GNU2Y-NEXT:                                                       left: Identifier(
// GNU2Y-NEXT:                                                           "n",
// GNU2Y-NEXT:                                                       ),
// GNU2Y-NEXT:                                                       right: IntegerLiteral(
// GNU2Y-NEXT:                                                           IntegerLiteral {
// GNU2Y-NEXT:                                                               value: 2,
// GNU2Y-NEXT:                                                               radix: Decimal,
// GNU2Y-NEXT:                                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                                   unsigned: false,
// GNU2Y-NEXT:                                                                   size: None,
// GNU2Y-NEXT:                                                               },
// GNU2Y-NEXT:                                                               spelling: "2",
// GNU2Y-NEXT:                                                           },
// GNU2Y-NEXT:                                                       ),
// GNU2Y-NEXT:                                                   },
// GNU2Y-NEXT:                                                   then_branch: NamedBreak(
// GNU2Y-NEXT:                                                       "inner",
// GNU2Y-NEXT:                                                   ),
// GNU2Y-NEXT:                                                   else_branch: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               Break,
// GNU2Y-NEXT:                                           ],
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               Continue,
// GNU2Y-NEXT:                           ],
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               Labeled {
// GNU2Y-NEXT:                   label: "again",
// GNU2Y-NEXT:                   body: DoWhile {
// GNU2Y-NEXT:                       body: Block(
// GNU2Y-NEXT:                           [
// GNU2Y-NEXT:                               If {
// GNU2Y-NEXT:                                   condition: Identifier(
// GNU2Y-NEXT:                                       "n",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   then_branch: NamedContinue(
// GNU2Y-NEXT:                                       "again",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   else_branch: None,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               NamedBreak(
// GNU2Y-NEXT:                                   "again",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           ],
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                       condition: Identifier(
// GNU2Y-NEXT:                           "n",
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               Labeled {
// GNU2Y-NEXT:                   label: "choice",
// GNU2Y-NEXT:                   body: Switch {
// GNU2Y-NEXT:                       discriminant: Identifier(
// GNU2Y-NEXT:                           "n",
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                       body: Block(
// GNU2Y-NEXT:                           [
// GNU2Y-NEXT:                               SwitchLabel {
// GNU2Y-NEXT:                                   label: Case(
// GNU2Y-NEXT:                                       IntegerLiteral(
// GNU2Y-NEXT:                                           IntegerLiteral {
// GNU2Y-NEXT:                                               value: 0,
// GNU2Y-NEXT:                                               radix: Decimal,
// GNU2Y-NEXT:                                               suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                   unsigned: false,
// GNU2Y-NEXT:                                                   size: None,
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               spelling: "0",
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   body: NamedBreak(
// GNU2Y-NEXT:                                       "choice",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               SwitchLabel {
// GNU2Y-NEXT:                                   label: Default,
// GNU2Y-NEXT:                                   body: Break,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ],
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:           ],
// GNU2Y-NEXT:       },
// GNU2Y-NEXT:   )
// SLATE-FILECHECK-END GNU2Y
