typedef int T;
int get(void);
int identity(int x) { return x; }

void selections(int n) {
    if (int x = get())
        n = x;
    else
        n = x + 1;
    if (T x = get(); x > 0) n = x;
    if (int x = 1)
        if (int y = 2; y) n = x + y;
        else n = x;
    if (int T = get(); T) { T = n; }
    T restored = n;
    if (int a = 1, b = get(); a + b) n = a + b;
    if (int x; x = n) n = x;
    if (int a[] = { 1, 2 }; a[0]) n = a[1];
    if (auto x = get()) n = x;
    if ([[maybe_unused]] int x = n; x) n = x;
    if (int (*f)(int) = identity) n = f(n);
    if (int x = ({ int y = n; y; }); x) n = x;
    switch (int x = get()) {
    case 0: n = x; break;
    default: n = x + 1;
    }
    switch (T x = get(); x + 1) {
    case 1: n = x; break;
    default: break;
    }
    switch (int T = get(); T) { default: T = n; }
    T after_switch = restored;
    if (n) n = after_switch;
    switch (n) { default: break; }
}

// SLATE-FILECHECK-AST
// SLATE-FILECHECK-DEFINES C2Y
// SLATE-FILECHECK-STD C2Y c2y
// SLATE-FILECHECK-DEFINES GNU2Y
// SLATE-FILECHECK-STD GNU2Y gnu2y
// SLATE-FILECHECK-DEFINES NAMES
// SLATE-FILECHECK-STD NAMES c2y
// SLATE-FILECHECK-PREFIX-ARGS NAMES --dump-ir-names
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-ERROR C23
// SLATE-FILECHECK-DEFINES GNU23
// SLATE-FILECHECK-STD GNU23 gnu23
// SLATE-FILECHECK-ERROR GNU23
// SLATE-FILECHECK-IR-ERROR IR
// SLATE-FILECHECK-STD IR c2y

// SLATE-FILECHECK-BEGIN C23
// C23: Error:   × selection statement declarations require C2y in the GCC flavor
// C23: ╰─▶ selection statement declarations require C2y in the GCC flavor
// C23: ╭─[tests/fixtures/gcc/linux/x86_64/c2y_selection_declarations.c:6:9]
// C23: 5 │ void selections(int n) {
// C23: 6 │     if (int x = get())
// C23: ·         ───
// C23: 7 │         n = x;
// C23: ╰────
// SLATE-FILECHECK-END C23
// SLATE-FILECHECK-BEGIN GNU23
// GNU23: Error:   × selection statement declarations require C2y in the GCC flavor
// GNU23: ╰─▶ selection statement declarations require C2y in the GCC flavor
// GNU23: ╭─[tests/fixtures/gcc/linux/x86_64/c2y_selection_declarations.c:6:9]
// GNU23: 5 │ void selections(int n) {
// GNU23: 6 │     if (int x = get())
// GNU23: ·         ───
// GNU23: 7 │         n = x;
// GNU23: ╰────
// SLATE-FILECHECK-END GNU23
// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR: × not implemented: selection statement declarations
// IR: ╭─[tests/fixtures/gcc/linux/x86_64/c2y_selection_declarations.c:6:5]
// IR: 5 │     void selections(int n) {
// IR: 6 │ ╭─▶     if (int x = get())
// IR: 7 │ │           n = x;
// IR: 8 │ │       else
// IR: 9 │ ╰─▶         n = x + 1;
// IR: 10 │         if (T x = get(); x > 0) n = x;
// IR: ╰────
// SLATE-FILECHECK-END IR
// SLATE-FILECHECK-BEGIN C2Y
// C2Y: decl[{{[0-9]+}}]: Declaration(
// C2Y-NEXT:       Declaration {
// C2Y-NEXT:           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:               ty: Integer(
// C2Y-NEXT:                   Ranked {
// C2Y-NEXT:                       rank: Int,
// C2Y-NEXT:                       signed: true,
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               storage: Typedef,
// C2Y-NEXT:           },
// C2Y-NEXT:           declarators: [
// C2Y-NEXT:               InitDeclaratorKind {
// C2Y-NEXT:                   declarator: Name(
// C2Y-NEXT:                       "T",
// C2Y-NEXT:                   ),
// C2Y-NEXT:               },
// C2Y-NEXT:           ],
// C2Y-NEXT:       },
// C2Y-NEXT:   )
// C2Y-NEXT: decl[{{[0-9]+}}]: Declaration(
// C2Y-NEXT:       Declaration {
// C2Y-NEXT:           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:               ty: Integer(
// C2Y-NEXT:                   Ranked {
// C2Y-NEXT:                       rank: Int,
// C2Y-NEXT:                       signed: true,
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:           },
// C2Y-NEXT:           declarators: [
// C2Y-NEXT:               InitDeclaratorKind {
// C2Y-NEXT:                   declarator: Function {
// C2Y-NEXT:                       inner: Name(
// C2Y-NEXT:                           "get",
// C2Y-NEXT:                       ),
// C2Y-NEXT:                       parameters: Void,
// C2Y-NEXT:                   },
// C2Y-NEXT:               },
// C2Y-NEXT:           ],
// C2Y-NEXT:       },
// C2Y-NEXT:   )
// C2Y-NEXT: decl[{{[0-9]+}}]: Function(
// C2Y-NEXT:       FunctionDefinition {
// C2Y-NEXT:           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:               ty: Integer(
// C2Y-NEXT:                   Ranked {
// C2Y-NEXT:                       rank: Int,
// C2Y-NEXT:                       signed: true,
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:           },
// C2Y-NEXT:           declarator: Function {
// C2Y-NEXT:               inner: Name(
// C2Y-NEXT:                   "identity",
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
// C2Y-NEXT:                               "x",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ],
// C2Y-NEXT:               },
// C2Y-NEXT:           },
// C2Y-NEXT:           body: [
// C2Y-NEXT:               Return(
// C2Y-NEXT:                   Identifier(
// C2Y-NEXT:                       "x",
// C2Y-NEXT:                   ),
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
// C2Y-NEXT:                   "selections",
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
// C2Y-NEXT:               IfDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
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
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "x",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       Call {
// C2Y-NEXT:                                           callee: Identifier(
// C2Y-NEXT:                                               "get",
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                           arguments: [],
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   condition: None,
// C2Y-NEXT:                   then_branch: Expr(
// C2Y-NEXT:                       Assign {
// C2Y-NEXT:                           op: Assign,
// C2Y-NEXT:                           target: Identifier(
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           value: Identifier(
// C2Y-NEXT:                               "x",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   else_branch: Some(
// C2Y-NEXT:                       Expr(
// C2Y-NEXT:                           Assign {
// C2Y-NEXT:                               op: Assign,
// C2Y-NEXT:                               target: Identifier(
// C2Y-NEXT:                                   "n",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               value: Binary {
// C2Y-NEXT:                                   op: Add,
// C2Y-NEXT:                                   left: Identifier(
// C2Y-NEXT:                                       "x",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   right: IntegerLiteral(
// C2Y-NEXT:                                       IntegerLiteral {
// C2Y-NEXT:                                           value: 1,
// C2Y-NEXT:                                           radix: Decimal,
// C2Y-NEXT:                                           suffix: IntegerSuffix {
// C2Y-NEXT:                                               unsigned: false,
// C2Y-NEXT:                                               size: None,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                           spelling: "1",
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ),
// C2Y-NEXT:                   ),
// C2Y-NEXT:               },
// C2Y-NEXT:               IfDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
// C2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                           ty: Named(
// C2Y-NEXT:                               "T",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                       declarators: [
// C2Y-NEXT:                           InitDeclaratorKind {
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "x",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       Call {
// C2Y-NEXT:                                           callee: Identifier(
// C2Y-NEXT:                                               "get",
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                           arguments: [],
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   condition: Some(
// C2Y-NEXT:                       Binary {
// C2Y-NEXT:                           op: Greater,
// C2Y-NEXT:                           left: Identifier(
// C2Y-NEXT:                               "x",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           right: IntegerLiteral(
// C2Y-NEXT:                               IntegerLiteral {
// C2Y-NEXT:                                   value: 0,
// C2Y-NEXT:                                   radix: Decimal,
// C2Y-NEXT:                                   suffix: IntegerSuffix {
// C2Y-NEXT:                                       unsigned: false,
// C2Y-NEXT:                                       size: None,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   spelling: "0",
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   then_branch: Expr(
// C2Y-NEXT:                       Assign {
// C2Y-NEXT:                           op: Assign,
// C2Y-NEXT:                           target: Identifier(
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           value: Identifier(
// C2Y-NEXT:                               "x",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   else_branch: None,
// C2Y-NEXT:               },
// C2Y-NEXT:               IfDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
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
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "x",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
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
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   condition: None,
// C2Y-NEXT:                   then_branch: IfDeclaration {
// C2Y-NEXT:                       declaration: Declaration {
// C2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                               ty: Integer(
// C2Y-NEXT:                                   Ranked {
// C2Y-NEXT:                                       rank: Int,
// C2Y-NEXT:                                       signed: true,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           declarators: [
// C2Y-NEXT:                               InitDeclaratorKind {
// C2Y-NEXT:                                   declarator: Name(
// C2Y-NEXT:                                       "y",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   initializer: Some(
// C2Y-NEXT:                                       Expr(
// C2Y-NEXT:                                           IntegerLiteral(
// C2Y-NEXT:                                               IntegerLiteral {
// C2Y-NEXT:                                                   value: 2,
// C2Y-NEXT:                                                   radix: Decimal,
// C2Y-NEXT:                                                   suffix: IntegerSuffix {
// C2Y-NEXT:                                                       unsigned: false,
// C2Y-NEXT:                                                       size: None,
// C2Y-NEXT:                                                   },
// C2Y-NEXT:                                                   spelling: "2",
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ],
// C2Y-NEXT:                       },
// C2Y-NEXT:                       condition: Some(
// C2Y-NEXT:                           Identifier(
// C2Y-NEXT:                               "y",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       ),
// C2Y-NEXT:                       then_branch: Expr(
// C2Y-NEXT:                           Assign {
// C2Y-NEXT:                               op: Assign,
// C2Y-NEXT:                               target: Identifier(
// C2Y-NEXT:                                   "n",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               value: Binary {
// C2Y-NEXT:                                   op: Add,
// C2Y-NEXT:                                   left: Identifier(
// C2Y-NEXT:                                       "x",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   right: Identifier(
// C2Y-NEXT:                                       "y",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ),
// C2Y-NEXT:                       else_branch: Some(
// C2Y-NEXT:                           Expr(
// C2Y-NEXT:                               Assign {
// C2Y-NEXT:                                   op: Assign,
// C2Y-NEXT:                                   target: Identifier(
// C2Y-NEXT:                                       "n",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   value: Identifier(
// C2Y-NEXT:                                       "x",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       ),
// C2Y-NEXT:                   },
// C2Y-NEXT:                   else_branch: None,
// C2Y-NEXT:               },
// C2Y-NEXT:               IfDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
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
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "T",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       Call {
// C2Y-NEXT:                                           callee: Identifier(
// C2Y-NEXT:                                               "get",
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                           arguments: [],
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   condition: Some(
// C2Y-NEXT:                       Identifier(
// C2Y-NEXT:                           "T",
// C2Y-NEXT:                       ),
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   then_branch: Block(
// C2Y-NEXT:                       [
// C2Y-NEXT:                           Expr(
// C2Y-NEXT:                               Assign {
// C2Y-NEXT:                                   op: Assign,
// C2Y-NEXT:                                   target: Identifier(
// C2Y-NEXT:                                       "T",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   value: Identifier(
// C2Y-NEXT:                                       "n",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   else_branch: None,
// C2Y-NEXT:               },
// C2Y-NEXT:               Decl(
// C2Y-NEXT:                   Declaration {
// C2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                           ty: Named(
// C2Y-NEXT:                               "T",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                       declarators: [
// C2Y-NEXT:                           InitDeclaratorKind {
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "restored",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       Identifier(
// C2Y-NEXT:                                           "n",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               IfDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
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
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "a",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
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
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           InitDeclaratorKind {
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "b",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       Call {
// C2Y-NEXT:                                           callee: Identifier(
// C2Y-NEXT:                                               "get",
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                           arguments: [],
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   condition: Some(
// C2Y-NEXT:                       Binary {
// C2Y-NEXT:                           op: Add,
// C2Y-NEXT:                           left: Identifier(
// C2Y-NEXT:                               "a",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           right: Identifier(
// C2Y-NEXT:                               "b",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   then_branch: Expr(
// C2Y-NEXT:                       Assign {
// C2Y-NEXT:                           op: Assign,
// C2Y-NEXT:                           target: Identifier(
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           value: Binary {
// C2Y-NEXT:                               op: Add,
// C2Y-NEXT:                               left: Identifier(
// C2Y-NEXT:                                   "a",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               right: Identifier(
// C2Y-NEXT:                                   "b",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   else_branch: None,
// C2Y-NEXT:               },
// C2Y-NEXT:               IfDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
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
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "x",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   condition: Some(
// C2Y-NEXT:                       Assign {
// C2Y-NEXT:                           op: Assign,
// C2Y-NEXT:                           target: Identifier(
// C2Y-NEXT:                               "x",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           value: Identifier(
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   then_branch: Expr(
// C2Y-NEXT:                       Assign {
// C2Y-NEXT:                           op: Assign,
// C2Y-NEXT:                           target: Identifier(
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           value: Identifier(
// C2Y-NEXT:                               "x",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   else_branch: None,
// C2Y-NEXT:               },
// C2Y-NEXT:               IfDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
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
// C2Y-NEXT:                                   size: Unspecified,
// C2Y-NEXT:                               },
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   List(
// C2Y-NEXT:                                       [
// C2Y-NEXT:                                           InitializerItem {
// C2Y-NEXT:                                               designators: [],
// C2Y-NEXT:                                               value: Expr(
// C2Y-NEXT:                                                   IntegerLiteral(
// C2Y-NEXT:                                                       IntegerLiteral {
// C2Y-NEXT:                                                           value: 1,
// C2Y-NEXT:                                                           radix: Decimal,
// C2Y-NEXT:                                                           suffix: IntegerSuffix {
// C2Y-NEXT:                                                               unsigned: false,
// C2Y-NEXT:                                                               size: None,
// C2Y-NEXT:                                                           },
// C2Y-NEXT:                                                           spelling: "1",
// C2Y-NEXT:                                                       },
// C2Y-NEXT:                                                   ),
// C2Y-NEXT:                                               ),
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                           InitializerItem {
// C2Y-NEXT:                                               designators: [],
// C2Y-NEXT:                                               value: Expr(
// C2Y-NEXT:                                                   IntegerLiteral(
// C2Y-NEXT:                                                       IntegerLiteral {
// C2Y-NEXT:                                                           value: 2,
// C2Y-NEXT:                                                           radix: Decimal,
// C2Y-NEXT:                                                           suffix: IntegerSuffix {
// C2Y-NEXT:                                                               unsigned: false,
// C2Y-NEXT:                                                               size: None,
// C2Y-NEXT:                                                           },
// C2Y-NEXT:                                                           spelling: "2",
// C2Y-NEXT:                                                       },
// C2Y-NEXT:                                                   ),
// C2Y-NEXT:                                               ),
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ],
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   condition: Some(
// C2Y-NEXT:                       Index {
// C2Y-NEXT:                           base: Identifier(
// C2Y-NEXT:                               "a",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           index: IntegerLiteral(
// C2Y-NEXT:                               IntegerLiteral {
// C2Y-NEXT:                                   value: 0,
// C2Y-NEXT:                                   radix: Decimal,
// C2Y-NEXT:                                   suffix: IntegerSuffix {
// C2Y-NEXT:                                       unsigned: false,
// C2Y-NEXT:                                       size: None,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   spelling: "0",
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   then_branch: Expr(
// C2Y-NEXT:                       Assign {
// C2Y-NEXT:                           op: Assign,
// C2Y-NEXT:                           target: Identifier(
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           value: Index {
// C2Y-NEXT:                               base: Identifier(
// C2Y-NEXT:                                   "a",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               index: IntegerLiteral(
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
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   else_branch: None,
// C2Y-NEXT:               },
// C2Y-NEXT:               IfDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
// C2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                           ty: Inferred,
// C2Y-NEXT:                       },
// C2Y-NEXT:                       declarators: [
// C2Y-NEXT:                           InitDeclaratorKind {
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "x",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       Call {
// C2Y-NEXT:                                           callee: Identifier(
// C2Y-NEXT:                                               "get",
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                           arguments: [],
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   condition: None,
// C2Y-NEXT:                   then_branch: Expr(
// C2Y-NEXT:                       Assign {
// C2Y-NEXT:                           op: Assign,
// C2Y-NEXT:                           target: Identifier(
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           value: Identifier(
// C2Y-NEXT:                               "x",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   else_branch: None,
// C2Y-NEXT:               },
// C2Y-NEXT:               IfDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
// C2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                           ty: Integer(
// C2Y-NEXT:                               Ranked {
// C2Y-NEXT:                                   rank: Int,
// C2Y-NEXT:                                   signed: true,
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           attributes: [
// C2Y-NEXT:                               MaybeUnused,
// C2Y-NEXT:                           ],
// C2Y-NEXT:                       },
// C2Y-NEXT:                       declarators: [
// C2Y-NEXT:                           InitDeclaratorKind {
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "x",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       Identifier(
// C2Y-NEXT:                                           "n",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   condition: Some(
// C2Y-NEXT:                       Identifier(
// C2Y-NEXT:                           "x",
// C2Y-NEXT:                       ),
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   then_branch: Expr(
// C2Y-NEXT:                       Assign {
// C2Y-NEXT:                           op: Assign,
// C2Y-NEXT:                           target: Identifier(
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           value: Identifier(
// C2Y-NEXT:                               "x",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   else_branch: None,
// C2Y-NEXT:               },
// C2Y-NEXT:               IfDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
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
// C2Y-NEXT:                               declarator: Function {
// C2Y-NEXT:                                   inner: Grouped(
// C2Y-NEXT:                                       Pointer {
// C2Y-NEXT:                                           qualifiers: Qualifiers,
// C2Y-NEXT:                                           inner: Name(
// C2Y-NEXT:                                               "f",
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                                   parameters: Prototype {
// C2Y-NEXT:                                       parameters: [
// C2Y-NEXT:                                           ParameterDeclarationKind {
// C2Y-NEXT:                                               specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                                   ty: Integer(
// C2Y-NEXT:                                                       Ranked {
// C2Y-NEXT:                                                           rank: Int,
// C2Y-NEXT:                                                           signed: true,
// C2Y-NEXT:                                                       },
// C2Y-NEXT:                                                   ),
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                               declarator: Abstract,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                       ],
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               },
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       Identifier(
// C2Y-NEXT:                                           "identity",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   condition: None,
// C2Y-NEXT:                   then_branch: Expr(
// C2Y-NEXT:                       Assign {
// C2Y-NEXT:                           op: Assign,
// C2Y-NEXT:                           target: Identifier(
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           value: Call {
// C2Y-NEXT:                               callee: Identifier(
// C2Y-NEXT:                                   "f",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               arguments: [
// C2Y-NEXT:                                   Identifier(
// C2Y-NEXT:                                       "n",
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ],
// C2Y-NEXT:                           },
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   else_branch: None,
// C2Y-NEXT:               },
// C2Y-NEXT:               IfDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
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
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "x",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       StatementExpression(
// C2Y-NEXT:                                           [
// C2Y-NEXT:                                               Decl(
// C2Y-NEXT:                                                   Declaration {
// C2Y-NEXT:                                                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                                                           ty: Integer(
// C2Y-NEXT:                                                               Ranked {
// C2Y-NEXT:                                                                   rank: Int,
// C2Y-NEXT:                                                                   signed: true,
// C2Y-NEXT:                                                               },
// C2Y-NEXT:                                                           ),
// C2Y-NEXT:                                                       },
// C2Y-NEXT:                                                       declarators: [
// C2Y-NEXT:                                                           InitDeclaratorKind {
// C2Y-NEXT:                                                               declarator: Name(
// C2Y-NEXT:                                                                   "y",
// C2Y-NEXT:                                                               ),
// C2Y-NEXT:                                                               initializer: Some(
// C2Y-NEXT:                                                                   Expr(
// C2Y-NEXT:                                                                       Identifier(
// C2Y-NEXT:                                                                           "n",
// C2Y-NEXT:                                                                       ),
// C2Y-NEXT:                                                                   ),
// C2Y-NEXT:                                                               ),
// C2Y-NEXT:                                                           },
// C2Y-NEXT:                                                       ],
// C2Y-NEXT:                                                   },
// C2Y-NEXT:                                               ),
// C2Y-NEXT:                                               Expr(
// C2Y-NEXT:                                                   Identifier(
// C2Y-NEXT:                                                       "y",
// C2Y-NEXT:                                                   ),
// C2Y-NEXT:                                               ),
// C2Y-NEXT:                                           ],
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   condition: Some(
// C2Y-NEXT:                       Identifier(
// C2Y-NEXT:                           "x",
// C2Y-NEXT:                       ),
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   then_branch: Expr(
// C2Y-NEXT:                       Assign {
// C2Y-NEXT:                           op: Assign,
// C2Y-NEXT:                           target: Identifier(
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           value: Identifier(
// C2Y-NEXT:                               "x",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   else_branch: None,
// C2Y-NEXT:               },
// C2Y-NEXT:               SwitchDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
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
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "x",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       Call {
// C2Y-NEXT:                                           callee: Identifier(
// C2Y-NEXT:                                               "get",
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                           arguments: [],
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   discriminant: None,
// C2Y-NEXT:                   body: Block(
// C2Y-NEXT:                       [
// C2Y-NEXT:                           SwitchLabel {
// C2Y-NEXT:                               label: Case(
// C2Y-NEXT:                                   IntegerLiteral(
// C2Y-NEXT:                                       IntegerLiteral {
// C2Y-NEXT:                                           value: 0,
// C2Y-NEXT:                                           radix: Decimal,
// C2Y-NEXT:                                           suffix: IntegerSuffix {
// C2Y-NEXT:                                               unsigned: false,
// C2Y-NEXT:                                               size: None,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                           spelling: "0",
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               body: Expr(
// C2Y-NEXT:                                   Assign {
// C2Y-NEXT:                                       op: Assign,
// C2Y-NEXT:                                       target: Identifier(
// C2Y-NEXT:                                           "n",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                       value: Identifier(
// C2Y-NEXT:                                           "x",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           Break,
// C2Y-NEXT:                           SwitchLabel {
// C2Y-NEXT:                               label: Default,
// C2Y-NEXT:                               body: Expr(
// C2Y-NEXT:                                   Assign {
// C2Y-NEXT:                                       op: Assign,
// C2Y-NEXT:                                       target: Identifier(
// C2Y-NEXT:                                           "n",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                       value: Binary {
// C2Y-NEXT:                                           op: Add,
// C2Y-NEXT:                                           left: Identifier(
// C2Y-NEXT:                                               "x",
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                           right: IntegerLiteral(
// C2Y-NEXT:                                               IntegerLiteral {
// C2Y-NEXT:                                                   value: 1,
// C2Y-NEXT:                                                   radix: Decimal,
// C2Y-NEXT:                                                   suffix: IntegerSuffix {
// C2Y-NEXT:                                                       unsigned: false,
// C2Y-NEXT:                                                       size: None,
// C2Y-NEXT:                                                   },
// C2Y-NEXT:                                                   spelling: "1",
// C2Y-NEXT:                                               },
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   ),
// C2Y-NEXT:               },
// C2Y-NEXT:               SwitchDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
// C2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                           ty: Named(
// C2Y-NEXT:                               "T",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                       declarators: [
// C2Y-NEXT:                           InitDeclaratorKind {
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "x",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       Call {
// C2Y-NEXT:                                           callee: Identifier(
// C2Y-NEXT:                                               "get",
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                           arguments: [],
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   discriminant: Some(
// C2Y-NEXT:                       Binary {
// C2Y-NEXT:                           op: Add,
// C2Y-NEXT:                           left: Identifier(
// C2Y-NEXT:                               "x",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           right: IntegerLiteral(
// C2Y-NEXT:                               IntegerLiteral {
// C2Y-NEXT:                                   value: 1,
// C2Y-NEXT:                                   radix: Decimal,
// C2Y-NEXT:                                   suffix: IntegerSuffix {
// C2Y-NEXT:                                       unsigned: false,
// C2Y-NEXT:                                       size: None,
// C2Y-NEXT:                                   },
// C2Y-NEXT:                                   spelling: "1",
// C2Y-NEXT:                               },
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   body: Block(
// C2Y-NEXT:                       [
// C2Y-NEXT:                           SwitchLabel {
// C2Y-NEXT:                               label: Case(
// C2Y-NEXT:                                   IntegerLiteral(
// C2Y-NEXT:                                       IntegerLiteral {
// C2Y-NEXT:                                           value: 1,
// C2Y-NEXT:                                           radix: Decimal,
// C2Y-NEXT:                                           suffix: IntegerSuffix {
// C2Y-NEXT:                                               unsigned: false,
// C2Y-NEXT:                                               size: None,
// C2Y-NEXT:                                           },
// C2Y-NEXT:                                           spelling: "1",
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               body: Expr(
// C2Y-NEXT:                                   Assign {
// C2Y-NEXT:                                       op: Assign,
// C2Y-NEXT:                                       target: Identifier(
// C2Y-NEXT:                                           "n",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                       value: Identifier(
// C2Y-NEXT:                                           "x",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                           Break,
// C2Y-NEXT:                           SwitchLabel {
// C2Y-NEXT:                               label: Default,
// C2Y-NEXT:                               body: Break,
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   ),
// C2Y-NEXT:               },
// C2Y-NEXT:               SwitchDeclaration {
// C2Y-NEXT:                   declaration: Declaration {
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
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "T",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       Call {
// C2Y-NEXT:                                           callee: Identifier(
// C2Y-NEXT:                                               "get",
// C2Y-NEXT:                                           ),
// C2Y-NEXT:                                           arguments: [],
// C2Y-NEXT:                                       },
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:                   discriminant: Some(
// C2Y-NEXT:                       Identifier(
// C2Y-NEXT:                           "T",
// C2Y-NEXT:                       ),
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   body: Block(
// C2Y-NEXT:                       [
// C2Y-NEXT:                           SwitchLabel {
// C2Y-NEXT:                               label: Default,
// C2Y-NEXT:                               body: Expr(
// C2Y-NEXT:                                   Assign {
// C2Y-NEXT:                                       op: Assign,
// C2Y-NEXT:                                       target: Identifier(
// C2Y-NEXT:                                           "T",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                       value: Identifier(
// C2Y-NEXT:                                           "n",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   },
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   ),
// C2Y-NEXT:               },
// C2Y-NEXT:               Decl(
// C2Y-NEXT:                   Declaration {
// C2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// C2Y-NEXT:                           ty: Named(
// C2Y-NEXT:                               "T",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                       declarators: [
// C2Y-NEXT:                           InitDeclaratorKind {
// C2Y-NEXT:                               declarator: Name(
// C2Y-NEXT:                                   "after_switch",
// C2Y-NEXT:                               ),
// C2Y-NEXT:                               initializer: Some(
// C2Y-NEXT:                                   Expr(
// C2Y-NEXT:                                       Identifier(
// C2Y-NEXT:                                           "restored",
// C2Y-NEXT:                                       ),
// C2Y-NEXT:                                   ),
// C2Y-NEXT:                               ),
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   },
// C2Y-NEXT:               ),
// C2Y-NEXT:               If {
// C2Y-NEXT:                   condition: Identifier(
// C2Y-NEXT:                       "n",
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   then_branch: Expr(
// C2Y-NEXT:                       Assign {
// C2Y-NEXT:                           op: Assign,
// C2Y-NEXT:                           target: Identifier(
// C2Y-NEXT:                               "n",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                           value: Identifier(
// C2Y-NEXT:                               "after_switch",
// C2Y-NEXT:                           ),
// C2Y-NEXT:                       },
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   else_branch: None,
// C2Y-NEXT:               },
// C2Y-NEXT:               Switch {
// C2Y-NEXT:                   discriminant: Identifier(
// C2Y-NEXT:                       "n",
// C2Y-NEXT:                   ),
// C2Y-NEXT:                   body: Block(
// C2Y-NEXT:                       [
// C2Y-NEXT:                           SwitchLabel {
// C2Y-NEXT:                               label: Default,
// C2Y-NEXT:                               body: Break,
// C2Y-NEXT:                           },
// C2Y-NEXT:                       ],
// C2Y-NEXT:                   ),
// C2Y-NEXT:               },
// C2Y-NEXT:           ],
// C2Y-NEXT:       },
// C2Y-NEXT:   )
// SLATE-FILECHECK-END C2Y
// SLATE-FILECHECK-BEGIN GNU2Y
// GNU2Y: decl[{{[0-9]+}}]: Declaration(
// GNU2Y-NEXT:       Declaration {
// GNU2Y-NEXT:           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:               ty: Integer(
// GNU2Y-NEXT:                   Ranked {
// GNU2Y-NEXT:                       rank: Int,
// GNU2Y-NEXT:                       signed: true,
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               storage: Typedef,
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           declarators: [
// GNU2Y-NEXT:               InitDeclaratorKind {
// GNU2Y-NEXT:                   declarator: Name(
// GNU2Y-NEXT:                       "T",
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:           ],
// GNU2Y-NEXT:       },
// GNU2Y-NEXT:   )
// GNU2Y-NEXT: decl[{{[0-9]+}}]: Declaration(
// GNU2Y-NEXT:       Declaration {
// GNU2Y-NEXT:           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:               ty: Integer(
// GNU2Y-NEXT:                   Ranked {
// GNU2Y-NEXT:                       rank: Int,
// GNU2Y-NEXT:                       signed: true,
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           declarators: [
// GNU2Y-NEXT:               InitDeclaratorKind {
// GNU2Y-NEXT:                   declarator: Function {
// GNU2Y-NEXT:                       inner: Name(
// GNU2Y-NEXT:                           "get",
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                       parameters: Void,
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:           ],
// GNU2Y-NEXT:       },
// GNU2Y-NEXT:   )
// GNU2Y-NEXT: decl[{{[0-9]+}}]: Function(
// GNU2Y-NEXT:       FunctionDefinition {
// GNU2Y-NEXT:           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:               ty: Integer(
// GNU2Y-NEXT:                   Ranked {
// GNU2Y-NEXT:                       rank: Int,
// GNU2Y-NEXT:                       signed: true,
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           declarator: Function {
// GNU2Y-NEXT:               inner: Name(
// GNU2Y-NEXT:                   "identity",
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
// GNU2Y-NEXT:                               "x",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ],
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:           },
// GNU2Y-NEXT:           body: [
// GNU2Y-NEXT:               Return(
// GNU2Y-NEXT:                   Identifier(
// GNU2Y-NEXT:                       "x",
// GNU2Y-NEXT:                   ),
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
// GNU2Y-NEXT:                   "selections",
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
// GNU2Y-NEXT:               IfDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
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
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "x",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       Call {
// GNU2Y-NEXT:                                           callee: Identifier(
// GNU2Y-NEXT:                                               "get",
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                           arguments: [],
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   condition: None,
// GNU2Y-NEXT:                   then_branch: Expr(
// GNU2Y-NEXT:                       Assign {
// GNU2Y-NEXT:                           op: Assign,
// GNU2Y-NEXT:                           target: Identifier(
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           value: Identifier(
// GNU2Y-NEXT:                               "x",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   else_branch: Some(
// GNU2Y-NEXT:                       Expr(
// GNU2Y-NEXT:                           Assign {
// GNU2Y-NEXT:                               op: Assign,
// GNU2Y-NEXT:                               target: Identifier(
// GNU2Y-NEXT:                                   "n",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               value: Binary {
// GNU2Y-NEXT:                                   op: Add,
// GNU2Y-NEXT:                                   left: Identifier(
// GNU2Y-NEXT:                                       "x",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   right: IntegerLiteral(
// GNU2Y-NEXT:                                       IntegerLiteral {
// GNU2Y-NEXT:                                           value: 1,
// GNU2Y-NEXT:                                           radix: Decimal,
// GNU2Y-NEXT:                                           suffix: IntegerSuffix {
// GNU2Y-NEXT:                                               unsigned: false,
// GNU2Y-NEXT:                                               size: None,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                           spelling: "1",
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               IfDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
// GNU2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                           ty: Named(
// GNU2Y-NEXT:                               "T",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       declarators: [
// GNU2Y-NEXT:                           InitDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "x",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       Call {
// GNU2Y-NEXT:                                           callee: Identifier(
// GNU2Y-NEXT:                                               "get",
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                           arguments: [],
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   condition: Some(
// GNU2Y-NEXT:                       Binary {
// GNU2Y-NEXT:                           op: Greater,
// GNU2Y-NEXT:                           left: Identifier(
// GNU2Y-NEXT:                               "x",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           right: IntegerLiteral(
// GNU2Y-NEXT:                               IntegerLiteral {
// GNU2Y-NEXT:                                   value: 0,
// GNU2Y-NEXT:                                   radix: Decimal,
// GNU2Y-NEXT:                                   suffix: IntegerSuffix {
// GNU2Y-NEXT:                                       unsigned: false,
// GNU2Y-NEXT:                                       size: None,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   spelling: "0",
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   then_branch: Expr(
// GNU2Y-NEXT:                       Assign {
// GNU2Y-NEXT:                           op: Assign,
// GNU2Y-NEXT:                           target: Identifier(
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           value: Identifier(
// GNU2Y-NEXT:                               "x",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   else_branch: None,
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               IfDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
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
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "x",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
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
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   condition: None,
// GNU2Y-NEXT:                   then_branch: IfDeclaration {
// GNU2Y-NEXT:                       declaration: Declaration {
// GNU2Y-NEXT:                           specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                               ty: Integer(
// GNU2Y-NEXT:                                   Ranked {
// GNU2Y-NEXT:                                       rank: Int,
// GNU2Y-NEXT:                                       signed: true,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           declarators: [
// GNU2Y-NEXT:                               InitDeclaratorKind {
// GNU2Y-NEXT:                                   declarator: Name(
// GNU2Y-NEXT:                                       "y",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   initializer: Some(
// GNU2Y-NEXT:                                       Expr(
// GNU2Y-NEXT:                                           IntegerLiteral(
// GNU2Y-NEXT:                                               IntegerLiteral {
// GNU2Y-NEXT:                                                   value: 2,
// GNU2Y-NEXT:                                                   radix: Decimal,
// GNU2Y-NEXT:                                                   suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                       unsigned: false,
// GNU2Y-NEXT:                                                       size: None,
// GNU2Y-NEXT:                                                   },
// GNU2Y-NEXT:                                                   spelling: "2",
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ],
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       condition: Some(
// GNU2Y-NEXT:                           Identifier(
// GNU2Y-NEXT:                               "y",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                       then_branch: Expr(
// GNU2Y-NEXT:                           Assign {
// GNU2Y-NEXT:                               op: Assign,
// GNU2Y-NEXT:                               target: Identifier(
// GNU2Y-NEXT:                                   "n",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               value: Binary {
// GNU2Y-NEXT:                                   op: Add,
// GNU2Y-NEXT:                                   left: Identifier(
// GNU2Y-NEXT:                                       "x",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   right: Identifier(
// GNU2Y-NEXT:                                       "y",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                       else_branch: Some(
// GNU2Y-NEXT:                           Expr(
// GNU2Y-NEXT:                               Assign {
// GNU2Y-NEXT:                                   op: Assign,
// GNU2Y-NEXT:                                   target: Identifier(
// GNU2Y-NEXT:                                       "n",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   value: Identifier(
// GNU2Y-NEXT:                                       "x",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   else_branch: None,
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               IfDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
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
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "T",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       Call {
// GNU2Y-NEXT:                                           callee: Identifier(
// GNU2Y-NEXT:                                               "get",
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                           arguments: [],
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   condition: Some(
// GNU2Y-NEXT:                       Identifier(
// GNU2Y-NEXT:                           "T",
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   then_branch: Block(
// GNU2Y-NEXT:                       [
// GNU2Y-NEXT:                           Expr(
// GNU2Y-NEXT:                               Assign {
// GNU2Y-NEXT:                                   op: Assign,
// GNU2Y-NEXT:                                   target: Identifier(
// GNU2Y-NEXT:                                       "T",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   value: Identifier(
// GNU2Y-NEXT:                                       "n",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   else_branch: None,
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               Decl(
// GNU2Y-NEXT:                   Declaration {
// GNU2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                           ty: Named(
// GNU2Y-NEXT:                               "T",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       declarators: [
// GNU2Y-NEXT:                           InitDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "restored",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       Identifier(
// GNU2Y-NEXT:                                           "n",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               IfDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
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
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "a",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
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
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           InitDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "b",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       Call {
// GNU2Y-NEXT:                                           callee: Identifier(
// GNU2Y-NEXT:                                               "get",
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                           arguments: [],
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   condition: Some(
// GNU2Y-NEXT:                       Binary {
// GNU2Y-NEXT:                           op: Add,
// GNU2Y-NEXT:                           left: Identifier(
// GNU2Y-NEXT:                               "a",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           right: Identifier(
// GNU2Y-NEXT:                               "b",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   then_branch: Expr(
// GNU2Y-NEXT:                       Assign {
// GNU2Y-NEXT:                           op: Assign,
// GNU2Y-NEXT:                           target: Identifier(
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           value: Binary {
// GNU2Y-NEXT:                               op: Add,
// GNU2Y-NEXT:                               left: Identifier(
// GNU2Y-NEXT:                                   "a",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               right: Identifier(
// GNU2Y-NEXT:                                   "b",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   else_branch: None,
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               IfDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
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
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "x",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   condition: Some(
// GNU2Y-NEXT:                       Assign {
// GNU2Y-NEXT:                           op: Assign,
// GNU2Y-NEXT:                           target: Identifier(
// GNU2Y-NEXT:                               "x",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           value: Identifier(
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   then_branch: Expr(
// GNU2Y-NEXT:                       Assign {
// GNU2Y-NEXT:                           op: Assign,
// GNU2Y-NEXT:                           target: Identifier(
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           value: Identifier(
// GNU2Y-NEXT:                               "x",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   else_branch: None,
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               IfDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
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
// GNU2Y-NEXT:                                   size: Unspecified,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   List(
// GNU2Y-NEXT:                                       [
// GNU2Y-NEXT:                                           InitializerItem {
// GNU2Y-NEXT:                                               designators: [],
// GNU2Y-NEXT:                                               value: Expr(
// GNU2Y-NEXT:                                                   IntegerLiteral(
// GNU2Y-NEXT:                                                       IntegerLiteral {
// GNU2Y-NEXT:                                                           value: 1,
// GNU2Y-NEXT:                                                           radix: Decimal,
// GNU2Y-NEXT:                                                           suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                               unsigned: false,
// GNU2Y-NEXT:                                                               size: None,
// GNU2Y-NEXT:                                                           },
// GNU2Y-NEXT:                                                           spelling: "1",
// GNU2Y-NEXT:                                                       },
// GNU2Y-NEXT:                                                   ),
// GNU2Y-NEXT:                                               ),
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                           InitializerItem {
// GNU2Y-NEXT:                                               designators: [],
// GNU2Y-NEXT:                                               value: Expr(
// GNU2Y-NEXT:                                                   IntegerLiteral(
// GNU2Y-NEXT:                                                       IntegerLiteral {
// GNU2Y-NEXT:                                                           value: 2,
// GNU2Y-NEXT:                                                           radix: Decimal,
// GNU2Y-NEXT:                                                           suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                               unsigned: false,
// GNU2Y-NEXT:                                                               size: None,
// GNU2Y-NEXT:                                                           },
// GNU2Y-NEXT:                                                           spelling: "2",
// GNU2Y-NEXT:                                                       },
// GNU2Y-NEXT:                                                   ),
// GNU2Y-NEXT:                                               ),
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ],
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   condition: Some(
// GNU2Y-NEXT:                       Index {
// GNU2Y-NEXT:                           base: Identifier(
// GNU2Y-NEXT:                               "a",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           index: IntegerLiteral(
// GNU2Y-NEXT:                               IntegerLiteral {
// GNU2Y-NEXT:                                   value: 0,
// GNU2Y-NEXT:                                   radix: Decimal,
// GNU2Y-NEXT:                                   suffix: IntegerSuffix {
// GNU2Y-NEXT:                                       unsigned: false,
// GNU2Y-NEXT:                                       size: None,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   spelling: "0",
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   then_branch: Expr(
// GNU2Y-NEXT:                       Assign {
// GNU2Y-NEXT:                           op: Assign,
// GNU2Y-NEXT:                           target: Identifier(
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           value: Index {
// GNU2Y-NEXT:                               base: Identifier(
// GNU2Y-NEXT:                                   "a",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               index: IntegerLiteral(
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
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   else_branch: None,
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               IfDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
// GNU2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                           ty: Inferred,
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       declarators: [
// GNU2Y-NEXT:                           InitDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "x",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       Call {
// GNU2Y-NEXT:                                           callee: Identifier(
// GNU2Y-NEXT:                                               "get",
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                           arguments: [],
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   condition: None,
// GNU2Y-NEXT:                   then_branch: Expr(
// GNU2Y-NEXT:                       Assign {
// GNU2Y-NEXT:                           op: Assign,
// GNU2Y-NEXT:                           target: Identifier(
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           value: Identifier(
// GNU2Y-NEXT:                               "x",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   else_branch: None,
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               IfDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
// GNU2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                           ty: Integer(
// GNU2Y-NEXT:                               Ranked {
// GNU2Y-NEXT:                                   rank: Int,
// GNU2Y-NEXT:                                   signed: true,
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           attributes: [
// GNU2Y-NEXT:                               MaybeUnused,
// GNU2Y-NEXT:                           ],
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       declarators: [
// GNU2Y-NEXT:                           InitDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "x",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       Identifier(
// GNU2Y-NEXT:                                           "n",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   condition: Some(
// GNU2Y-NEXT:                       Identifier(
// GNU2Y-NEXT:                           "x",
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   then_branch: Expr(
// GNU2Y-NEXT:                       Assign {
// GNU2Y-NEXT:                           op: Assign,
// GNU2Y-NEXT:                           target: Identifier(
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           value: Identifier(
// GNU2Y-NEXT:                               "x",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   else_branch: None,
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               IfDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
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
// GNU2Y-NEXT:                               declarator: Function {
// GNU2Y-NEXT:                                   inner: Grouped(
// GNU2Y-NEXT:                                       Pointer {
// GNU2Y-NEXT:                                           qualifiers: Qualifiers,
// GNU2Y-NEXT:                                           inner: Name(
// GNU2Y-NEXT:                                               "f",
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                                   parameters: Prototype {
// GNU2Y-NEXT:                                       parameters: [
// GNU2Y-NEXT:                                           ParameterDeclarationKind {
// GNU2Y-NEXT:                                               specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                                   ty: Integer(
// GNU2Y-NEXT:                                                       Ranked {
// GNU2Y-NEXT:                                                           rank: Int,
// GNU2Y-NEXT:                                                           signed: true,
// GNU2Y-NEXT:                                                       },
// GNU2Y-NEXT:                                                   ),
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                               declarator: Abstract,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                       ],
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       Identifier(
// GNU2Y-NEXT:                                           "identity",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   condition: None,
// GNU2Y-NEXT:                   then_branch: Expr(
// GNU2Y-NEXT:                       Assign {
// GNU2Y-NEXT:                           op: Assign,
// GNU2Y-NEXT:                           target: Identifier(
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           value: Call {
// GNU2Y-NEXT:                               callee: Identifier(
// GNU2Y-NEXT:                                   "f",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               arguments: [
// GNU2Y-NEXT:                                   Identifier(
// GNU2Y-NEXT:                                       "n",
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ],
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   else_branch: None,
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               IfDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
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
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "x",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       StatementExpression(
// GNU2Y-NEXT:                                           [
// GNU2Y-NEXT:                                               Decl(
// GNU2Y-NEXT:                                                   Declaration {
// GNU2Y-NEXT:                                                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                                                           ty: Integer(
// GNU2Y-NEXT:                                                               Ranked {
// GNU2Y-NEXT:                                                                   rank: Int,
// GNU2Y-NEXT:                                                                   signed: true,
// GNU2Y-NEXT:                                                               },
// GNU2Y-NEXT:                                                           ),
// GNU2Y-NEXT:                                                       },
// GNU2Y-NEXT:                                                       declarators: [
// GNU2Y-NEXT:                                                           InitDeclaratorKind {
// GNU2Y-NEXT:                                                               declarator: Name(
// GNU2Y-NEXT:                                                                   "y",
// GNU2Y-NEXT:                                                               ),
// GNU2Y-NEXT:                                                               initializer: Some(
// GNU2Y-NEXT:                                                                   Expr(
// GNU2Y-NEXT:                                                                       Identifier(
// GNU2Y-NEXT:                                                                           "n",
// GNU2Y-NEXT:                                                                       ),
// GNU2Y-NEXT:                                                                   ),
// GNU2Y-NEXT:                                                               ),
// GNU2Y-NEXT:                                                           },
// GNU2Y-NEXT:                                                       ],
// GNU2Y-NEXT:                                                   },
// GNU2Y-NEXT:                                               ),
// GNU2Y-NEXT:                                               Expr(
// GNU2Y-NEXT:                                                   Identifier(
// GNU2Y-NEXT:                                                       "y",
// GNU2Y-NEXT:                                                   ),
// GNU2Y-NEXT:                                               ),
// GNU2Y-NEXT:                                           ],
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   condition: Some(
// GNU2Y-NEXT:                       Identifier(
// GNU2Y-NEXT:                           "x",
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   then_branch: Expr(
// GNU2Y-NEXT:                       Assign {
// GNU2Y-NEXT:                           op: Assign,
// GNU2Y-NEXT:                           target: Identifier(
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           value: Identifier(
// GNU2Y-NEXT:                               "x",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   else_branch: None,
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               SwitchDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
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
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "x",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       Call {
// GNU2Y-NEXT:                                           callee: Identifier(
// GNU2Y-NEXT:                                               "get",
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                           arguments: [],
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   discriminant: None,
// GNU2Y-NEXT:                   body: Block(
// GNU2Y-NEXT:                       [
// GNU2Y-NEXT:                           SwitchLabel {
// GNU2Y-NEXT:                               label: Case(
// GNU2Y-NEXT:                                   IntegerLiteral(
// GNU2Y-NEXT:                                       IntegerLiteral {
// GNU2Y-NEXT:                                           value: 0,
// GNU2Y-NEXT:                                           radix: Decimal,
// GNU2Y-NEXT:                                           suffix: IntegerSuffix {
// GNU2Y-NEXT:                                               unsigned: false,
// GNU2Y-NEXT:                                               size: None,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                           spelling: "0",
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               body: Expr(
// GNU2Y-NEXT:                                   Assign {
// GNU2Y-NEXT:                                       op: Assign,
// GNU2Y-NEXT:                                       target: Identifier(
// GNU2Y-NEXT:                                           "n",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                       value: Identifier(
// GNU2Y-NEXT:                                           "x",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           Break,
// GNU2Y-NEXT:                           SwitchLabel {
// GNU2Y-NEXT:                               label: Default,
// GNU2Y-NEXT:                               body: Expr(
// GNU2Y-NEXT:                                   Assign {
// GNU2Y-NEXT:                                       op: Assign,
// GNU2Y-NEXT:                                       target: Identifier(
// GNU2Y-NEXT:                                           "n",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                       value: Binary {
// GNU2Y-NEXT:                                           op: Add,
// GNU2Y-NEXT:                                           left: Identifier(
// GNU2Y-NEXT:                                               "x",
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                           right: IntegerLiteral(
// GNU2Y-NEXT:                                               IntegerLiteral {
// GNU2Y-NEXT:                                                   value: 1,
// GNU2Y-NEXT:                                                   radix: Decimal,
// GNU2Y-NEXT:                                                   suffix: IntegerSuffix {
// GNU2Y-NEXT:                                                       unsigned: false,
// GNU2Y-NEXT:                                                       size: None,
// GNU2Y-NEXT:                                                   },
// GNU2Y-NEXT:                                                   spelling: "1",
// GNU2Y-NEXT:                                               },
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               SwitchDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
// GNU2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                           ty: Named(
// GNU2Y-NEXT:                               "T",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       declarators: [
// GNU2Y-NEXT:                           InitDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "x",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       Call {
// GNU2Y-NEXT:                                           callee: Identifier(
// GNU2Y-NEXT:                                               "get",
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                           arguments: [],
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   discriminant: Some(
// GNU2Y-NEXT:                       Binary {
// GNU2Y-NEXT:                           op: Add,
// GNU2Y-NEXT:                           left: Identifier(
// GNU2Y-NEXT:                               "x",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           right: IntegerLiteral(
// GNU2Y-NEXT:                               IntegerLiteral {
// GNU2Y-NEXT:                                   value: 1,
// GNU2Y-NEXT:                                   radix: Decimal,
// GNU2Y-NEXT:                                   suffix: IntegerSuffix {
// GNU2Y-NEXT:                                       unsigned: false,
// GNU2Y-NEXT:                                       size: None,
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                                   spelling: "1",
// GNU2Y-NEXT:                               },
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   body: Block(
// GNU2Y-NEXT:                       [
// GNU2Y-NEXT:                           SwitchLabel {
// GNU2Y-NEXT:                               label: Case(
// GNU2Y-NEXT:                                   IntegerLiteral(
// GNU2Y-NEXT:                                       IntegerLiteral {
// GNU2Y-NEXT:                                           value: 1,
// GNU2Y-NEXT:                                           radix: Decimal,
// GNU2Y-NEXT:                                           suffix: IntegerSuffix {
// GNU2Y-NEXT:                                               unsigned: false,
// GNU2Y-NEXT:                                               size: None,
// GNU2Y-NEXT:                                           },
// GNU2Y-NEXT:                                           spelling: "1",
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               body: Expr(
// GNU2Y-NEXT:                                   Assign {
// GNU2Y-NEXT:                                       op: Assign,
// GNU2Y-NEXT:                                       target: Identifier(
// GNU2Y-NEXT:                                           "n",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                       value: Identifier(
// GNU2Y-NEXT:                                           "x",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                           Break,
// GNU2Y-NEXT:                           SwitchLabel {
// GNU2Y-NEXT:                               label: Default,
// GNU2Y-NEXT:                               body: Break,
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               SwitchDeclaration {
// GNU2Y-NEXT:                   declaration: Declaration {
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
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "T",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       Call {
// GNU2Y-NEXT:                                           callee: Identifier(
// GNU2Y-NEXT:                                               "get",
// GNU2Y-NEXT:                                           ),
// GNU2Y-NEXT:                                           arguments: [],
// GNU2Y-NEXT:                                       },
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:                   discriminant: Some(
// GNU2Y-NEXT:                       Identifier(
// GNU2Y-NEXT:                           "T",
// GNU2Y-NEXT:                       ),
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   body: Block(
// GNU2Y-NEXT:                       [
// GNU2Y-NEXT:                           SwitchLabel {
// GNU2Y-NEXT:                               label: Default,
// GNU2Y-NEXT:                               body: Expr(
// GNU2Y-NEXT:                                   Assign {
// GNU2Y-NEXT:                                       op: Assign,
// GNU2Y-NEXT:                                       target: Identifier(
// GNU2Y-NEXT:                                           "T",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                       value: Identifier(
// GNU2Y-NEXT:                                           "n",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   },
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               Decl(
// GNU2Y-NEXT:                   Declaration {
// GNU2Y-NEXT:                       specifiers: DeclarationSpecifiers {
// GNU2Y-NEXT:                           ty: Named(
// GNU2Y-NEXT:                               "T",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                       declarators: [
// GNU2Y-NEXT:                           InitDeclaratorKind {
// GNU2Y-NEXT:                               declarator: Name(
// GNU2Y-NEXT:                                   "after_switch",
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                               initializer: Some(
// GNU2Y-NEXT:                                   Expr(
// GNU2Y-NEXT:                                       Identifier(
// GNU2Y-NEXT:                                           "restored",
// GNU2Y-NEXT:                                       ),
// GNU2Y-NEXT:                                   ),
// GNU2Y-NEXT:                               ),
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   },
// GNU2Y-NEXT:               ),
// GNU2Y-NEXT:               If {
// GNU2Y-NEXT:                   condition: Identifier(
// GNU2Y-NEXT:                       "n",
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   then_branch: Expr(
// GNU2Y-NEXT:                       Assign {
// GNU2Y-NEXT:                           op: Assign,
// GNU2Y-NEXT:                           target: Identifier(
// GNU2Y-NEXT:                               "n",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                           value: Identifier(
// GNU2Y-NEXT:                               "after_switch",
// GNU2Y-NEXT:                           ),
// GNU2Y-NEXT:                       },
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   else_branch: None,
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:               Switch {
// GNU2Y-NEXT:                   discriminant: Identifier(
// GNU2Y-NEXT:                       "n",
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:                   body: Block(
// GNU2Y-NEXT:                       [
// GNU2Y-NEXT:                           SwitchLabel {
// GNU2Y-NEXT:                               label: Default,
// GNU2Y-NEXT:                               body: Break,
// GNU2Y-NEXT:                           },
// GNU2Y-NEXT:                       ],
// GNU2Y-NEXT:                   ),
// GNU2Y-NEXT:               },
// GNU2Y-NEXT:           ],
// GNU2Y-NEXT:       },
// GNU2Y-NEXT:   )
// SLATE-FILECHECK-END GNU2Y
// SLATE-FILECHECK-BEGIN NAMES
// NAMES: bind typedef T = %0
// NAMES-NEXT: bind function get = %1
// NAMES-NEXT: bind function identity = %2
// NAMES-NEXT: bind parameter x = %3
// NAMES-NEXT: bind function selections = %4
// NAMES-NEXT: bind parameter n = %5
// NAMES-NEXT: bind object x#1 = %6
// NAMES-NEXT: bind object x#2 = %7
// NAMES-NEXT: bind object x#3 = %8
// NAMES-NEXT: bind object y = %9
// NAMES-NEXT: bind object T#1 = %10
// NAMES-NEXT: bind object restored = %11
// NAMES-NEXT: bind object a = %12
// NAMES-NEXT: bind object b = %13
// NAMES-NEXT: bind object x#4 = %14
// NAMES-NEXT: bind object a#1 = %15
// NAMES-NEXT: bind object x#5 = %16
// NAMES-NEXT: bind object x#6 = %17
// NAMES-NEXT: bind object f = %18
// NAMES-NEXT: bind object x#7 = %19
// NAMES-NEXT: bind object y#1 = %20
// NAMES-NEXT: bind object x#8 = %21
// NAMES-NEXT: bind object x#9 = %22
// NAMES-NEXT: bind object T#2 = %23
// NAMES-NEXT: bind object after_switch = %24
// NAMES-NEXT: ref parameter x -> %3
// NAMES-NEXT: ref function get -> %1
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object x#1 -> %6
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object x#1 -> %6
// NAMES-NEXT: ref typedef T -> %0
// NAMES-NEXT: ref function get -> %1
// NAMES-NEXT: ref object x#2 -> %7
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object x#2 -> %7
// NAMES-NEXT: ref object y -> %9
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object x#3 -> %8
// NAMES-NEXT: ref object y -> %9
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object x#3 -> %8
// NAMES-NEXT: ref function get -> %1
// NAMES-NEXT: ref object T#1 -> %10
// NAMES-NEXT: ref object T#1 -> %10
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref typedef T -> %0
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref function get -> %1
// NAMES-NEXT: ref object a -> %12
// NAMES-NEXT: ref object b -> %13
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object a -> %12
// NAMES-NEXT: ref object b -> %13
// NAMES-NEXT: ref object x#4 -> %14
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object x#4 -> %14
// NAMES-NEXT: ref object a#1 -> %15
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object a#1 -> %15
// NAMES-NEXT: ref function get -> %1
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object x#5 -> %16
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object x#6 -> %17
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object x#6 -> %17
// NAMES-NEXT: ref function identity -> %2
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object f -> %18
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object y#1 -> %20
// NAMES-NEXT: ref object x#7 -> %19
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object x#7 -> %19
// NAMES-NEXT: ref function get -> %1
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object x#8 -> %21
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object x#8 -> %21
// NAMES-NEXT: ref typedef T -> %0
// NAMES-NEXT: ref function get -> %1
// NAMES-NEXT: ref object x#9 -> %22
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object x#9 -> %22
// NAMES-NEXT: ref function get -> %1
// NAMES-NEXT: ref object T#2 -> %23
// NAMES-NEXT: ref object T#2 -> %23
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref typedef T -> %0
// NAMES-NEXT: ref object restored -> %11
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref parameter n -> %5
// NAMES-NEXT: ref object after_switch -> %24
// NAMES-NEXT: ref parameter n -> %5
// SLATE-FILECHECK-END NAMES
