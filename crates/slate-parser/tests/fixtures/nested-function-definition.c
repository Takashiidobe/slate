void nested_outer(int n) {
#ifdef DOUBLE_INNER
  int inner(int x) {
    return x * 2;
  }
#else
  int inner(int x) {
    return x + 1;
  }
#endif
  inner(n);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES DOUBLED DOUBLE_INNER

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "nested_outer",
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
// DEFAULT-NEXT:                           "n",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               NestedFunction(
// DEFAULT-NEXT:                   FunctionDecl {
// DEFAULT-NEXT:                       ret_type: Integer(
// DEFAULT-NEXT:                           Ranked {
// DEFAULT-NEXT:                               rank: Int,
// DEFAULT-NEXT:                               signed: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       name: "inner",
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Some(
// DEFAULT-NEXT:                                   Name(
// DEFAULT-NEXT:                                       "x",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       body: [
// DEFAULT-NEXT:                           Return(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Add,
// DEFAULT-NEXT:                                   left: Identifier(
// DEFAULT-NEXT:                                       "x",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Integer(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: System,
// DEFAULT-NEXT:                           line: 0,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Call {
// DEFAULT-NEXT:                       callee: Identifier(
// DEFAULT-NEXT:                           "inner",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       arguments: [
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "n",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
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
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN DOUBLED
// DOUBLED: decl[0]: Function(
// DOUBLED-NEXT:       FunctionDecl {
// DOUBLED-NEXT:           ret_type: Void,
// DOUBLED-NEXT:           name: "nested_outer",
// DOUBLED-NEXT:           parameters: [
// DOUBLED-NEXT:               Parameter {
// DOUBLED-NEXT:                   ty: Integer(
// DOUBLED-NEXT:                       Ranked {
// DOUBLED-NEXT:                           rank: Int,
// DOUBLED-NEXT:                           signed: true,
// DOUBLED-NEXT:                       },
// DOUBLED-NEXT:                   ),
// DOUBLED-NEXT:                   declarator: Some(
// DOUBLED-NEXT:                       Name(
// DOUBLED-NEXT:                           "n",
// DOUBLED-NEXT:                       ),
// DOUBLED-NEXT:                   ),
// DOUBLED-NEXT:               },
// DOUBLED-NEXT:           ],
// DOUBLED-NEXT:           body: [
// DOUBLED-NEXT:               NestedFunction(
// DOUBLED-NEXT:                   FunctionDecl {
// DOUBLED-NEXT:                       ret_type: Integer(
// DOUBLED-NEXT:                           Ranked {
// DOUBLED-NEXT:                               rank: Int,
// DOUBLED-NEXT:                               signed: true,
// DOUBLED-NEXT:                           },
// DOUBLED-NEXT:                       ),
// DOUBLED-NEXT:                       name: "inner",
// DOUBLED-NEXT:                       parameters: [
// DOUBLED-NEXT:                           Parameter {
// DOUBLED-NEXT:                               ty: Integer(
// DOUBLED-NEXT:                                   Ranked {
// DOUBLED-NEXT:                                       rank: Int,
// DOUBLED-NEXT:                                       signed: true,
// DOUBLED-NEXT:                                   },
// DOUBLED-NEXT:                               ),
// DOUBLED-NEXT:                               declarator: Some(
// DOUBLED-NEXT:                                   Name(
// DOUBLED-NEXT:                                       "x",
// DOUBLED-NEXT:                                   ),
// DOUBLED-NEXT:                               ),
// DOUBLED-NEXT:                           },
// DOUBLED-NEXT:                       ],
// DOUBLED-NEXT:                       body: [
// DOUBLED-NEXT:                           Return(
// DOUBLED-NEXT:                               Binary {
// DOUBLED-NEXT:                                   op: Mul,
// DOUBLED-NEXT:                                   left: Identifier(
// DOUBLED-NEXT:                                       "x",
// DOUBLED-NEXT:                                   ),
// DOUBLED-NEXT:                                   right: Integer(
// DOUBLED-NEXT:                                       2,
// DOUBLED-NEXT:                                   ),
// DOUBLED-NEXT:                               },
// DOUBLED-NEXT:                           ),
// DOUBLED-NEXT:                       ],
// DOUBLED-NEXT:                       provenance: Provenance {
// DOUBLED-NEXT:                           file: FileId(
// DOUBLED-NEXT:                               0,
// DOUBLED-NEXT:                           ),
// DOUBLED-NEXT:                           kind: System,
// DOUBLED-NEXT:                           line: 0,
// DOUBLED-NEXT:                           header: None,
// DOUBLED-NEXT:                       },
// DOUBLED-NEXT:                   },
// DOUBLED-NEXT:               ),
// DOUBLED-NEXT:               Expr(
// DOUBLED-NEXT:                   Call {
// DOUBLED-NEXT:                       callee: Identifier(
// DOUBLED-NEXT:                           "inner",
// DOUBLED-NEXT:                       ),
// DOUBLED-NEXT:                       arguments: [
// DOUBLED-NEXT:                           Identifier(
// DOUBLED-NEXT:                               "n",
// DOUBLED-NEXT:                           ),
// DOUBLED-NEXT:                       ],
// DOUBLED-NEXT:                   },
// DOUBLED-NEXT:               ),
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
// SLATE-FILECHECK-END DOUBLED
