int B;

#ifdef AS_CAST
typedef int A;
int cast_main() {
  (A)(B);
  return 0;
}
#else
int A(int value);
int call_main() {
  (A)(B);
  return 0;
}
#endif
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES CAST AS_CAST

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "B",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               0,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 0,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Conditional(
// DEFAULT-NEXT:       Conditional {
// DEFAULT-NEXT:           branches: [
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Defined(
// DEFAULT-NEXT:                       "AS_CAST",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Typedef {
// DEFAULT-NEXT:                           name: "A",
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 3,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Function(
// DEFAULT-NEXT:                           FunctionDecl {
// DEFAULT-NEXT:                               ret_type: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               name: "cast_main",
// DEFAULT-NEXT:                               body: [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Cast {
// DEFAULT-NEXT:                                               ty: Named(
// DEFAULT-NEXT:                                                   "A",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               declarator: Abstract,
// DEFAULT-NEXT:                                               value: Identifier(
// DEFAULT-NEXT:                                                   "B",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Return(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               provenance: Provenance {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   kind: User,
// DEFAULT-NEXT:                                   line: 4,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               (
// DEFAULT-NEXT:                   Not(
// DEFAULT-NEXT:                       Defined(
// DEFAULT-NEXT:                           "AS_CAST",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   [
// DEFAULT-NEXT:                       Declaration {
// DEFAULT-NEXT:                           declaration: Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarator: Function {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "A",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   parameters: [
// DEFAULT-NEXT:                                       Parameter {
// DEFAULT-NEXT:                                           ty: Integer(
// DEFAULT-NEXT:                                               Ranked {
// DEFAULT-NEXT:                                                   rank: Int,
// DEFAULT-NEXT:                                                   signed: true,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           declarator: Some(
// DEFAULT-NEXT:                                               Name(
// DEFAULT-NEXT:                                                   "value",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           provenance: Provenance {
// DEFAULT-NEXT:                               file: FileId(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               kind: User,
// DEFAULT-NEXT:                               line: 9,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Function(
// DEFAULT-NEXT:                           FunctionDecl {
// DEFAULT-NEXT:                               ret_type: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               name: "call_main",
// DEFAULT-NEXT:                               body: [
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Call {
// DEFAULT-NEXT:                                               callee: Identifier(
// DEFAULT-NEXT:                                                   "A",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               arguments: [
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "B",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Return(
// DEFAULT-NEXT:                                       Const(
// DEFAULT-NEXT:                                           Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                               provenance: Provenance {
// DEFAULT-NEXT:                                   file: FileId(
// DEFAULT-NEXT:                                       0,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   kind: User,
// DEFAULT-NEXT:                                   line: 10,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Name(
// DEFAULT-NEXT:               "B",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               0,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 0,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
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
// DEFAULT-NEXT:                   "A",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Integer(
// DEFAULT-NEXT:                           Ranked {
// DEFAULT-NEXT:                               rank: Int,
// DEFAULT-NEXT:                               signed: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       declarator: Some(
// DEFAULT-NEXT:                           Name(
// DEFAULT-NEXT:                               "value",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               0,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 9,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[2]: Function(
// DEFAULT-NEXT:       ConcreteFunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "call_main",
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "A",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "B",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Integer(
// DEFAULT-NEXT:                           0,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   0,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 10,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN CAST
// CAST: polyvariant:
// CAST-NEXT: decl[0]: Declaration {
// CAST-NEXT:       declaration: Declaration {
// CAST-NEXT:           specifiers: DeclarationSpecifiers {
// CAST-NEXT:               ty: Integer(
// CAST-NEXT:                   Ranked {
// CAST-NEXT:                       rank: Int,
// CAST-NEXT:                       signed: true,
// CAST-NEXT:                   },
// CAST-NEXT:               ),
// CAST-NEXT:           },
// CAST-NEXT:           declarator: Name(
// CAST-NEXT:               "B",
// CAST-NEXT:           ),
// CAST-NEXT:       },
// CAST-NEXT:       provenance: Provenance {
// CAST-NEXT:           file: FileId(
// CAST-NEXT:               0,
// CAST-NEXT:           ),
// CAST-NEXT:           kind: User,
// CAST-NEXT:           line: 0,
// CAST-NEXT:       },
// CAST-NEXT:   }
// CAST-NEXT: decl[1]: Conditional(
// CAST-NEXT:       Conditional {
// CAST-NEXT:           branches: [
// CAST-NEXT:               (
// CAST-NEXT:                   Defined(
// CAST-NEXT:                       "AS_CAST",
// CAST-NEXT:                   ),
// CAST-NEXT:                   [
// CAST-NEXT:                       Typedef {
// CAST-NEXT:                           name: "A",
// CAST-NEXT:                           ty: Integer(
// CAST-NEXT:                               Ranked {
// CAST-NEXT:                                   rank: Int,
// CAST-NEXT:                                   signed: true,
// CAST-NEXT:                               },
// CAST-NEXT:                           ),
// CAST-NEXT:                           provenance: Provenance {
// CAST-NEXT:                               file: FileId(
// CAST-NEXT:                                   0,
// CAST-NEXT:                               ),
// CAST-NEXT:                               kind: User,
// CAST-NEXT:                               line: 3,
// CAST-NEXT:                           },
// CAST-NEXT:                       },
// CAST-NEXT:                       Function(
// CAST-NEXT:                           FunctionDecl {
// CAST-NEXT:                               ret_type: Integer(
// CAST-NEXT:                                   Ranked {
// CAST-NEXT:                                       rank: Int,
// CAST-NEXT:                                       signed: true,
// CAST-NEXT:                                   },
// CAST-NEXT:                               ),
// CAST-NEXT:                               name: "cast_main",
// CAST-NEXT:                               body: [
// CAST-NEXT:                                   Expr(
// CAST-NEXT:                                       Const(
// CAST-NEXT:                                           Cast {
// CAST-NEXT:                                               ty: Named(
// CAST-NEXT:                                                   "A",
// CAST-NEXT:                                               ),
// CAST-NEXT:                                               declarator: Abstract,
// CAST-NEXT:                                               value: Identifier(
// CAST-NEXT:                                                   "B",
// CAST-NEXT:                                               ),
// CAST-NEXT:                                           },
// CAST-NEXT:                                       ),
// CAST-NEXT:                                   ),
// CAST-NEXT:                                   Return(
// CAST-NEXT:                                       Const(
// CAST-NEXT:                                           Integer(
// CAST-NEXT:                                               0,
// CAST-NEXT:                                           ),
// CAST-NEXT:                                       ),
// CAST-NEXT:                                   ),
// CAST-NEXT:                               ],
// CAST-NEXT:                               provenance: Provenance {
// CAST-NEXT:                                   file: FileId(
// CAST-NEXT:                                       0,
// CAST-NEXT:                                   ),
// CAST-NEXT:                                   kind: User,
// CAST-NEXT:                                   line: 4,
// CAST-NEXT:                               },
// CAST-NEXT:                           },
// CAST-NEXT:                       ),
// CAST-NEXT:                   ],
// CAST-NEXT:               ),
// CAST-NEXT:               (
// CAST-NEXT:                   Not(
// CAST-NEXT:                       Defined(
// CAST-NEXT:                           "AS_CAST",
// CAST-NEXT:                       ),
// CAST-NEXT:                   ),
// CAST-NEXT:                   [
// CAST-NEXT:                       Declaration {
// CAST-NEXT:                           declaration: Declaration {
// CAST-NEXT:                               specifiers: DeclarationSpecifiers {
// CAST-NEXT:                                   ty: Integer(
// CAST-NEXT:                                       Ranked {
// CAST-NEXT:                                           rank: Int,
// CAST-NEXT:                                           signed: true,
// CAST-NEXT:                                       },
// CAST-NEXT:                                   ),
// CAST-NEXT:                               },
// CAST-NEXT:                               declarator: Function {
// CAST-NEXT:                                   inner: Name(
// CAST-NEXT:                                       "A",
// CAST-NEXT:                                   ),
// CAST-NEXT:                                   parameters: [
// CAST-NEXT:                                       Parameter {
// CAST-NEXT:                                           ty: Integer(
// CAST-NEXT:                                               Ranked {
// CAST-NEXT:                                                   rank: Int,
// CAST-NEXT:                                                   signed: true,
// CAST-NEXT:                                               },
// CAST-NEXT:                                           ),
// CAST-NEXT:                                           declarator: Some(
// CAST-NEXT:                                               Name(
// CAST-NEXT:                                                   "value",
// CAST-NEXT:                                               ),
// CAST-NEXT:                                           ),
// CAST-NEXT:                                       },
// CAST-NEXT:                                   ],
// CAST-NEXT:                               },
// CAST-NEXT:                           },
// CAST-NEXT:                           provenance: Provenance {
// CAST-NEXT:                               file: FileId(
// CAST-NEXT:                                   0,
// CAST-NEXT:                               ),
// CAST-NEXT:                               kind: User,
// CAST-NEXT:                               line: 9,
// CAST-NEXT:                           },
// CAST-NEXT:                       },
// CAST-NEXT:                       Function(
// CAST-NEXT:                           FunctionDecl {
// CAST-NEXT:                               ret_type: Integer(
// CAST-NEXT:                                   Ranked {
// CAST-NEXT:                                       rank: Int,
// CAST-NEXT:                                       signed: true,
// CAST-NEXT:                                   },
// CAST-NEXT:                               ),
// CAST-NEXT:                               name: "call_main",
// CAST-NEXT:                               body: [
// CAST-NEXT:                                   Expr(
// CAST-NEXT:                                       Const(
// CAST-NEXT:                                           Call {
// CAST-NEXT:                                               callee: Identifier(
// CAST-NEXT:                                                   "A",
// CAST-NEXT:                                               ),
// CAST-NEXT:                                               arguments: [
// CAST-NEXT:                                                   Identifier(
// CAST-NEXT:                                                       "B",
// CAST-NEXT:                                                   ),
// CAST-NEXT:                                               ],
// CAST-NEXT:                                           },
// CAST-NEXT:                                       ),
// CAST-NEXT:                                   ),
// CAST-NEXT:                                   Return(
// CAST-NEXT:                                       Const(
// CAST-NEXT:                                           Integer(
// CAST-NEXT:                                               0,
// CAST-NEXT:                                           ),
// CAST-NEXT:                                       ),
// CAST-NEXT:                                   ),
// CAST-NEXT:                               ],
// CAST-NEXT:                               provenance: Provenance {
// CAST-NEXT:                                   file: FileId(
// CAST-NEXT:                                       0,
// CAST-NEXT:                                   ),
// CAST-NEXT:                                   kind: User,
// CAST-NEXT:                                   line: 10,
// CAST-NEXT:                               },
// CAST-NEXT:                           },
// CAST-NEXT:                       ),
// CAST-NEXT:                   ],
// CAST-NEXT:               ),
// CAST-NEXT:           ],
// CAST-NEXT:       },
// CAST-NEXT:   )
// CAST-NEXT: concrete:
// CAST-NEXT: decl[0]: Declaration {
// CAST-NEXT:       declaration: Declaration {
// CAST-NEXT:           specifiers: DeclarationSpecifiers {
// CAST-NEXT:               ty: Integer(
// CAST-NEXT:                   Ranked {
// CAST-NEXT:                       rank: Int,
// CAST-NEXT:                       signed: true,
// CAST-NEXT:                   },
// CAST-NEXT:               ),
// CAST-NEXT:           },
// CAST-NEXT:           declarator: Name(
// CAST-NEXT:               "B",
// CAST-NEXT:           ),
// CAST-NEXT:       },
// CAST-NEXT:       provenance: Provenance {
// CAST-NEXT:           file: FileId(
// CAST-NEXT:               0,
// CAST-NEXT:           ),
// CAST-NEXT:           kind: User,
// CAST-NEXT:           line: 0,
// CAST-NEXT:       },
// CAST-NEXT:   }
// CAST-NEXT: decl[1]: Typedef {
// CAST-NEXT:       name: "A",
// CAST-NEXT:       ty: Integer(
// CAST-NEXT:           Ranked {
// CAST-NEXT:               rank: Int,
// CAST-NEXT:               signed: true,
// CAST-NEXT:           },
// CAST-NEXT:       ),
// CAST-NEXT:       provenance: Provenance {
// CAST-NEXT:           file: FileId(
// CAST-NEXT:               0,
// CAST-NEXT:           ),
// CAST-NEXT:           kind: User,
// CAST-NEXT:           line: 3,
// CAST-NEXT:       },
// CAST-NEXT:   }
// CAST-NEXT: decl[2]: Function(
// CAST-NEXT:       ConcreteFunctionDecl {
// CAST-NEXT:           ret_type: Integer(
// CAST-NEXT:               Ranked {
// CAST-NEXT:                   rank: Int,
// CAST-NEXT:                   signed: true,
// CAST-NEXT:               },
// CAST-NEXT:           ),
// CAST-NEXT:           name: "cast_main",
// CAST-NEXT:           body: [
// CAST-NEXT:               Expr(
// CAST-NEXT:                   Const(
// CAST-NEXT:                       Cast {
// CAST-NEXT:                           ty: Named(
// CAST-NEXT:                               "A",
// CAST-NEXT:                           ),
// CAST-NEXT:                           declarator: Abstract,
// CAST-NEXT:                           value: Identifier(
// CAST-NEXT:                               "B",
// CAST-NEXT:                           ),
// CAST-NEXT:                       },
// CAST-NEXT:                   ),
// CAST-NEXT:               ),
// CAST-NEXT:               Return(
// CAST-NEXT:                   Const(
// CAST-NEXT:                       Integer(
// CAST-NEXT:                           0,
// CAST-NEXT:                       ),
// CAST-NEXT:                   ),
// CAST-NEXT:               ),
// CAST-NEXT:           ],
// CAST-NEXT:           provenance: Provenance {
// CAST-NEXT:               file: FileId(
// CAST-NEXT:                   0,
// CAST-NEXT:               ),
// CAST-NEXT:               kind: User,
// CAST-NEXT:               line: 4,
// CAST-NEXT:           },
// CAST-NEXT:       },
// CAST-NEXT:   )
// SLATE-FILECHECK-END CAST
