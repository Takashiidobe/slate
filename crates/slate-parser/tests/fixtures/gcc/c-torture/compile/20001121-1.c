// SLATE-FILECHECK-DEFINES DEFAULT

extern int bar(int);

int foo(int x)
{
  return 1 + bar(
	({
		int y;
		switch (x)
		{
		case 0: y = 1; break;
		case 1: y = 2; break;
		case 2: y = 3; break;
		case 3: y = 4; break;
		case 4: y = 5; break;
		case 5: y = 6; break;
		default: y = 7; break;
		}
		y;
	})
     );
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "bar",
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
// DEFAULT-NEXT:           line: 1,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[1]: Function(
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
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Add,
// DEFAULT-NEXT:                           left: Integer(
// DEFAULT-NEXT:                               1,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "bar",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [
// DEFAULT-NEXT:                                   StatementExpression(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Int,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "y",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Switch,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           LParen,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           RParen,
// DEFAULT-NEXT:                                           LBrace,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Case,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "0",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Colon,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "y",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Break,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Case,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "1",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Colon,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "y",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Break,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Case,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "2",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Colon,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "y",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Break,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Case,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "3",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Colon,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "y",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "4",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Break,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Case,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "4",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Colon,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "y",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "5",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Break,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Case,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "5",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Colon,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "y",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "6",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Break,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Default,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Colon,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "y",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Equal,
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               "7",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           Keyword(
// DEFAULT-NEXT:                                               Break,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                           RBrace,
// DEFAULT-NEXT:                                           Ident(
// DEFAULT-NEXT:                                               "y",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           Semi,
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 3,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
