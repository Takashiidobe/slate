
typedef struct st {
  int a;
} ST;

int __attribute__((noinline, noclone)) foo(ST *s, int c) {
  int first = 1;
  int count = c;
  ST *item  = s;
  int a     = s->a;
  int x;

  while (count--) {
    x = item->a;
    if (first)
      first = 0;
    else if (x >= a)
      return 1;
    a = x;
    item++;
  }
  return 0;
}

extern void abort(void);

int main() {
  ST _1[2] = {{2}, {1}};
  if (foo(_1, 2) != 0)
    abort();
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "st",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       declaration: Declaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 2,
// DEFAULT-NEXT:                           header: None,
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
// DEFAULT-NEXT: decl[1]: Typedef {
// DEFAULT-NEXT:       name: "ST",
// DEFAULT-NEXT:       ty: Tagged {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "st",
// DEFAULT-NEXT:           ),
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
// DEFAULT-NEXT: decl[2]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Integer(
// DEFAULT-NEXT:               Ranked {
// DEFAULT-NEXT:                   rank: Int,
// DEFAULT-NEXT:                   signed: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           name: "foo",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Named(
// DEFAULT-NEXT:                       "ST",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "s",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
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
// DEFAULT-NEXT:                           "c",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT:                           "first",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                           "count",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "c",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "ST",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "item",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "s",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                           "a",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           Expr(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   Arrow {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "s",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "a",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                           "x",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               While {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       PostDecrement(
// DEFAULT-NEXT:                           Identifier(
// DEFAULT-NEXT:                               "count",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "x",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Arrow {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "item",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "a",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       If {
// DEFAULT-NEXT:                           condition: Const(
// DEFAULT-NEXT:                               Identifier(
// DEFAULT-NEXT:                                   "first",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_branch: [
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "first",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                           else_branch: Some(
// DEFAULT-NEXT:                               [
// DEFAULT-NEXT:                                   If {
// DEFAULT-NEXT:                                       condition: Const(
// DEFAULT-NEXT:                                           Binary {
// DEFAULT-NEXT:                                               op: GreaterEqual,
// DEFAULT-NEXT:                                               left: Identifier(
// DEFAULT-NEXT:                                                   "x",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "a",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       then_branch: [
// DEFAULT-NEXT:                                           Return(
// DEFAULT-NEXT:                                               Const(
// DEFAULT-NEXT:                                                   Integer(
// DEFAULT-NEXT:                                                       1,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                       else_branch: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "a",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Identifier(
// DEFAULT-NEXT:                                       "x",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               PostIncrement(
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "item",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
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
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 5,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           attributes: [
// DEFAULT-NEXT:               NoInline,
// DEFAULT-NEXT:               NoClone,
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "abort",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 24,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[4]: Function(
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
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "ST",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarator: Array {
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "_1",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           size: Expression(
// DEFAULT-NEXT:                               IntLit(
// DEFAULT-NEXT:                                   2,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       initializer: Some(
// DEFAULT-NEXT:                           List(
// DEFAULT-NEXT:                               [
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [],
// DEFAULT-NEXT:                                       value: List(
// DEFAULT-NEXT:                                           [
// DEFAULT-NEXT:                                               InitializerItem {
// DEFAULT-NEXT:                                                   designators: [],
// DEFAULT-NEXT:                                                   value: Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Integer(
// DEFAULT-NEXT:                                                               2,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   InitializerItem {
// DEFAULT-NEXT:                                       designators: [],
// DEFAULT-NEXT:                                       value: List(
// DEFAULT-NEXT:                                           [
// DEFAULT-NEXT:                                               InitializerItem {
// DEFAULT-NEXT:                                                   designators: [],
// DEFAULT-NEXT:                                                   value: Expr(
// DEFAULT-NEXT:                                                       Const(
// DEFAULT-NEXT:                                                           Integer(
// DEFAULT-NEXT:                                                               1,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "foo",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   Identifier(
// DEFAULT-NEXT:                                       "_1",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Integer(
// DEFAULT-NEXT:                                       2,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Call {
// DEFAULT-NEXT:                                   callee: Identifier(
// DEFAULT-NEXT:                                       "abort",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   arguments: [],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
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
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 26,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
