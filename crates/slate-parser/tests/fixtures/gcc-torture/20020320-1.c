/* PR c/5354 */
/* Verify that GCC preserves relevant stack slots.  */

extern void abort(void);
extern void exit(int);

struct large {
  int x, y[9];
};

int main() {
  int fixed;

  fixed = ({
            int temp1 = 2;
            temp1;
          }) -
          ({
            int temp2 = 1;
            temp2;
          });
  if (fixed != 1)
    abort();

  fixed = ({
            struct large temp3;
            temp3.x = 2;
            temp3;
          }).x -
          ({
            struct large temp4;
            temp4.x = 1;
            temp4;
          }).x;
  if (fixed != 1)
    abort();

  exit(0);
}


// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment {
// DEFAULT-NEXT:       text: "/* PR c/5354 */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 0,
// DEFAULT-NEXT:           length: 15,
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
// DEFAULT-NEXT:       text: "/* Verify that GCC preserves relevant stack slots.  */",
// DEFAULT-NEXT:       loc: Loc {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           offset: 16,
// DEFAULT-NEXT:           length: 54,
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
// DEFAULT-NEXT:           line: 3,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "exit",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: [
// DEFAULT-NEXT:                   Parameter {
// DEFAULT-NEXT:                       ty: Integer(
// DEFAULT-NEXT:                           Ranked {
// DEFAULT-NEXT:                               rank: Int,
// DEFAULT-NEXT:                               signed: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
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
// DEFAULT-NEXT: decl[4]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "large",
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
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 7,
// DEFAULT-NEXT:                           header: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
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
// DEFAULT-NEXT:                           declarator: Array {
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "y",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               size: Expression(
// DEFAULT-NEXT:                                   IntLit(
// DEFAULT-NEXT:                                       9,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       provenance: Provenance {
// DEFAULT-NEXT:                           file: FileId(
// DEFAULT-NEXT:                               3,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           kind: User,
// DEFAULT-NEXT:                           line: 7,
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
// DEFAULT-NEXT:               line: 6,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[5]: Function(
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
// DEFAULT-NEXT:                           "fixed",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "fixed",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Sub,
// DEFAULT-NEXT:                               left: StatementExpression(
// DEFAULT-NEXT:                                   [
// DEFAULT-NEXT:                                       Keyword(
// DEFAULT-NEXT:                                           Int,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Ident(
// DEFAULT-NEXT:                                           "temp1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Equal,
// DEFAULT-NEXT:                                       IntLit(
// DEFAULT-NEXT:                                           "2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Semi,
// DEFAULT-NEXT:                                       Ident(
// DEFAULT-NEXT:                                           "temp1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Semi,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: StatementExpression(
// DEFAULT-NEXT:                                   [
// DEFAULT-NEXT:                                       Keyword(
// DEFAULT-NEXT:                                           Int,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Ident(
// DEFAULT-NEXT:                                           "temp2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Equal,
// DEFAULT-NEXT:                                       IntLit(
// DEFAULT-NEXT:                                           "1",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Semi,
// DEFAULT-NEXT:                                       Ident(
// DEFAULT-NEXT:                                           "temp2",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Semi,
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "fixed",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               1,
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
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Identifier(
// DEFAULT-NEXT:                               "fixed",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Sub,
// DEFAULT-NEXT:                               left: Member {
// DEFAULT-NEXT:                                   base: StatementExpression(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Struct,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "large",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "temp3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "temp3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Dot,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "temp3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   field: "x",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Member {
// DEFAULT-NEXT:                                   base: StatementExpression(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Struct,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "large",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "temp4",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "temp4",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Dot,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "temp4",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   field: "x",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: NotEqual,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "fixed",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               1,
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
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Call {
// DEFAULT-NEXT:                           callee: Identifier(
// DEFAULT-NEXT:                               "exit",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           arguments: [
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 10,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
