/* PR rtl-optimization/97421 */
/* { dg-additional-options "-fmodulo-sched -fno-dce -fno-strict-aliasing" } */

static int a, b, c;
int       *d = &c;
int      **e = &d;
int     ***f = &e;
int        main() {
  int h;
  for (a = 2; a; a--)
    for (h = 0; h <= 2; h++)
      for (b = 0; b <= 2; b++)
        ***f = 6;

  if (b != 3)
    __builtin_abort();
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment {
// DEFAULT-NEXT:       text: "/* PR rtl-optimization/97421 */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 0,
// DEFAULT-NEXT:           length: 31,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 0,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Comment {
// DEFAULT-NEXT:       text: "/* { dg-additional-options \"-fmodulo-sched -fno-dce -fno-strict-aliasing\" } */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 32,
// DEFAULT-NEXT:           length: 78,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 1,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "a",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 3,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "b",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 3,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "c",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 3,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[5]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Pointer {
// DEFAULT-NEXT:               qualifiers: Qualifiers,
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "d",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       AddrOf(
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "c",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 4,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[6]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Pointer {
// DEFAULT-NEXT:               qualifiers: Qualifiers,
// DEFAULT-NEXT:               inner: Pointer {
// DEFAULT-NEXT:                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                   inner: Name(
// DEFAULT-NEXT:                       "e",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       AddrOf(
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "d",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
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
// DEFAULT-NEXT: decl[7]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Pointer {
// DEFAULT-NEXT:               qualifiers: Qualifiers,
// DEFAULT-NEXT:               inner: Pointer {
// DEFAULT-NEXT:                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                   inner: Pointer {
// DEFAULT-NEXT:                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "f",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           initializer: Some(
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       AddrOf(
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "e",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 6,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[8]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "main",
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
// DEFAULT-NEXT:                       declarator: Name(
// DEFAULT-NEXT:                           "h",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               For {
// DEFAULT-NEXT:                   init: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "a",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       2,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   condition: Some(
// DEFAULT-NEXT:                       Const(
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   increment: Some(
// DEFAULT-NEXT:                       Const(
// DEFAULT-NEXT:                           PostDecrement(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       For {
// DEFAULT-NEXT:                           init: Some(
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "h",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           condition: Some(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Binary {
// DEFAULT-NEXT:                                       op: LessEqual,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "h",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           2,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           increment: Some(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   PostIncrement(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "h",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: [
// DEFAULT-NEXT:                               For {
// DEFAULT-NEXT:                                   init: Some(
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: Assign,
// DEFAULT-NEXT:                                                   target: Identifier(
// DEFAULT-NEXT:                                                       "b",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   condition: Some(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: LessEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   increment: Some(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           PostIncrement(
// DEFAULT-NEXT:                                               Identifier(
// DEFAULT-NEXT:                                                   "b",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   body: [
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: Assign,
// DEFAULT-NEXT:                                                   target: Deref(
// DEFAULT-NEXT:                                                       Deref(
// DEFAULT-NEXT:                                                           Deref(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "f",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       6,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "__builtin_abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 7,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
