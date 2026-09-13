// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-do assemble } */
/* { dg-require-effective-target ptr32plus } */
/* { dg-xfail-if "The array too big" { "h8300-*-*" } { "-mno-h" "-mn" } { "" } } */
/* { dg-require-stack-size "2048*4*4" } */

/* PR c/6957
   This testcase ICEd at -O2 on IA-32, because
   (insn 141 139 142 (set (subreg:SF (reg:QI 72) 0)
	   (plus:SF (reg:SF 73)
	       (reg:SF 76))) 525 {*fop_sf_comm_nosse} (insn_list 134 (nil))
       (expr_list:REG_DEAD (reg:SF 73) (nil)))
   couldn't be reloaded. */

void
foo (unsigned int n, int x, int y, unsigned char *z)
{
  int a, b;
  float c[2048][4];

  switch (x)
    {
    case 0x1906:
      a = b = -1;
      break;
    case 0x190A:
      a = b = -1;
      break;
    case 0x8049:
      a = b = -1;
      break;
    case 0x1907:
      a = 1;
      b = 2;
      break;
    default:
      return;
    }

  if (a >= 0)
    {
      unsigned char *d = z;
      unsigned int i;
      for (i = 0; i < n; i++)
	{
	  do
	    {
	      union
	      {
		float r;
		unsigned int i;
	      }
	      e;
	      e.r = c[i][1];
	      d[a] =
		((e.i >= 0x3f7f0000) ? ((int) e.i <
					    0) ? (unsigned char) 0
		 : (unsigned char) 255 : (e.r =
					  e.r * (255.0F / 256.0F) +
					  32768.0F, (unsigned char) e.i));
	    }
	  while (0);
	  d += y;
	}
    }

  if (b >= 0)
    {
      unsigned char *d = z;
      unsigned int i;
      for (i = 0; i < n; i++)
	{
	  do
	    {
	      union
	      {
		float r;
		unsigned int i;
	      }
	      e;
	      e.r = c[i][2];
	      d[b] =
		((e.i >= 0x3f7f0000) ? ((int) e.i <
					    0) ? (unsigned char) 0
		 : (unsigned char) 255 : (e.r =
					  e.r * (255.0F / 256.0F) +
					  32768.0F, (unsigned char) e.i));
	    }
	  while (0);
	  d += y;
	}
    }
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment(
// DEFAULT-NEXT:       CommentGroup {
// DEFAULT-NEXT:           comments: [
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* { dg-do assemble } */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 1,
// DEFAULT-NEXT:                       length: 24,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* { dg-require-effective-target ptr32plus } */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 26,
// DEFAULT-NEXT:                       length: 47,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* { dg-xfail-if \"The array too big\" { \"h8300-*-*\" } { \"-mno-h\" \"-mn\" } { \"\" } } */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 74,
// DEFAULT-NEXT:                       length: 83,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* { dg-require-stack-size \"2048*4*4\" } */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 158,
// DEFAULT-NEXT:                       length: 42,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* PR c/6957\n   This testcase ICEd at -O2 on IA-32, because\n   (insn 141 139 142 (set (subreg:SF (reg:QI 72) 0)\n\t   (plus:SF (reg:SF 73)\n\t       (reg:SF 76))) 525 {*fop_sf_comm_nosse} (insn_list 134 (nil))\n       (expr_list:REG_DEAD (reg:SF 73) (nil)))\n   couldn't be reloaded. */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 202,
// DEFAULT-NEXT:                       length: 280,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
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
// DEFAULT-NEXT: decl[1]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "foo",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Integer(
// DEFAULT-NEXT:                       Ranked {
// DEFAULT-NEXT:                           rank: Int,
// DEFAULT-NEXT:                           signed: false,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Name(
// DEFAULT-NEXT:                           "n",
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
// DEFAULT-NEXT:                       Char {
// DEFAULT-NEXT:                           signed: Some(
// DEFAULT-NEXT:                               false,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "z",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
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
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "a",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "b",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Floating(
// DEFAULT-NEXT:                               Float,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Array {
// DEFAULT-NEXT:                                       inner: Name(
// DEFAULT-NEXT:                                           "c",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       size: Expression(
// DEFAULT-NEXT:                                           IntLit(
// DEFAULT-NEXT:                                               2048,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntLit(
// DEFAULT-NEXT:                                           4,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Switch {
// DEFAULT-NEXT:                   discriminant: Const(
// DEFAULT-NEXT:                       Identifier(
// DEFAULT-NEXT:                           "x",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       Case(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   6406,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "a",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "b",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Unary {
// DEFAULT-NEXT:                                           op: Minus,
// DEFAULT-NEXT:                                           value: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Break,
// DEFAULT-NEXT:                       Case(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   6410,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "a",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "b",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Unary {
// DEFAULT-NEXT:                                           op: Minus,
// DEFAULT-NEXT:                                           value: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Break,
// DEFAULT-NEXT:                       Case(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   32841,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "a",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Assign {
// DEFAULT-NEXT:                                       op: Assign,
// DEFAULT-NEXT:                                       target: Identifier(
// DEFAULT-NEXT:                                           "b",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       value: Unary {
// DEFAULT-NEXT:                                           op: Minus,
// DEFAULT-NEXT:                                           value: Integer(
// DEFAULT-NEXT:                                               1,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Break,
// DEFAULT-NEXT:                       Case(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Integer(
// DEFAULT-NEXT:                                   6407,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "a",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       1,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Const(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "b",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Integer(
// DEFAULT-NEXT:                                       2,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Break,
// DEFAULT-NEXT:                       Default,
// DEFAULT-NEXT:                       ReturnVoid,
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: GreaterEqual,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "a",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: Some(
// DEFAULT-NEXT:                                               false,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "d",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Const(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "z",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "i",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       For {
// DEFAULT-NEXT:                           init: Some(
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "i",
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
// DEFAULT-NEXT:                                       op: Less,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "i",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Identifier(
// DEFAULT-NEXT:                                           "n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           increment: Some(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   PostIncrement(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "i",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: [
// DEFAULT-NEXT:                               DoWhile {
// DEFAULT-NEXT:                                   body: [
// DEFAULT-NEXT:                                       Decl(
// DEFAULT-NEXT:                                           Declaration {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Tagged {
// DEFAULT-NEXT:                                                       kind: Union,
// DEFAULT-NEXT:                                                       name: None,
// DEFAULT-NEXT:                                                       body: Some(
// DEFAULT-NEXT:                                                           Fields(
// DEFAULT-NEXT:                                                               [
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Floating(
// DEFAULT-NEXT:                                                                               Float,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Name(
// DEFAULT-NEXT:                                                                                   "r",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Integer(
// DEFAULT-NEXT:                                                                               Ranked {
// DEFAULT-NEXT:                                                                                   rank: Int,
// DEFAULT-NEXT:                                                                                   signed: false,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Name(
// DEFAULT-NEXT:                                                                                   "i",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarators: [
// DEFAULT-NEXT:                                                   InitDeclarator {
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "e",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: Assign,
// DEFAULT-NEXT:                                                   target: Member {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "e",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "r",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   value: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "c",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           1,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: Assign,
// DEFAULT-NEXT:                                                   target: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "d",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "a",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   value: Ternary {
// DEFAULT-NEXT:                                                       condition: Binary {
// DEFAULT-NEXT:                                                           op: GreaterEqual,
// DEFAULT-NEXT:                                                           left: Member {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "e",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               field: "i",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               1065287680,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       then_value: Ternary {
// DEFAULT-NEXT:                                                           condition: Binary {
// DEFAULT-NEXT:                                                               op: Less,
// DEFAULT-NEXT:                                                               left: Cast {
// DEFAULT-NEXT:                                                                   ty: Integer(
// DEFAULT-NEXT:                                                                       Ranked {
// DEFAULT-NEXT:                                                                           rank: Int,
// DEFAULT-NEXT:                                                                           signed: true,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                                   value: Member {
// DEFAULT-NEXT:                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                           "e",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       field: "i",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           then_value: Cast {
// DEFAULT-NEXT:                                                               ty: Integer(
// DEFAULT-NEXT:                                                                   Char {
// DEFAULT-NEXT:                                                                       signed: Some(
// DEFAULT-NEXT:                                                                           false,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                               value: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           else_value: Cast {
// DEFAULT-NEXT:                                                               ty: Integer(
// DEFAULT-NEXT:                                                                   Char {
// DEFAULT-NEXT:                                                                       signed: Some(
// DEFAULT-NEXT:                                                                           false,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                               value: Integer(
// DEFAULT-NEXT:                                                                   255,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       else_value: Comma(
// DEFAULT-NEXT:                                                           Assign {
// DEFAULT-NEXT:                                                               op: Assign,
// DEFAULT-NEXT:                                                               target: Member {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "e",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "r",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               value: Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Member {
// DEFAULT-NEXT:                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                               "e",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           field: "r",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Binary {
// DEFAULT-NEXT:                                                                           op: Div,
// DEFAULT-NEXT:                                                                           left: Float(
// DEFAULT-NEXT:                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                   value: Single(
// DEFAULT-NEXT:                                                                                       255.0,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           right: Float(
// DEFAULT-NEXT:                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                   value: Single(
// DEFAULT-NEXT:                                                                                       256.0,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Float(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           value: Single(
// DEFAULT-NEXT:                                                                               32768.0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           Cast {
// DEFAULT-NEXT:                                                               ty: Integer(
// DEFAULT-NEXT:                                                                   Char {
// DEFAULT-NEXT:                                                                       signed: Some(
// DEFAULT-NEXT:                                                                           false,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                               value: Member {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "e",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "i",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: AddAssign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "d",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Identifier(
// DEFAULT-NEXT:                                               "y",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: GreaterEqual,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "b",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Integer(
// DEFAULT-NEXT:                               0,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: Some(
// DEFAULT-NEXT:                                               false,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Pointer {
// DEFAULT-NEXT:                                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                                           inner: Name(
// DEFAULT-NEXT:                                               "d",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       initializer: Some(
// DEFAULT-NEXT:                                           Expr(
// DEFAULT-NEXT:                                               Const(
// DEFAULT-NEXT:                                                   Identifier(
// DEFAULT-NEXT:                                                       "z",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       Decl(
// DEFAULT-NEXT:                           Declaration {
// DEFAULT-NEXT:                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Ranked {
// DEFAULT-NEXT:                                           rank: Int,
// DEFAULT-NEXT:                                           signed: false,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               declarators: [
// DEFAULT-NEXT:                                   InitDeclarator {
// DEFAULT-NEXT:                                       declarator: Name(
// DEFAULT-NEXT:                                           "i",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       For {
// DEFAULT-NEXT:                           init: Some(
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: Assign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "i",
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
// DEFAULT-NEXT:                                       op: Less,
// DEFAULT-NEXT:                                       left: Identifier(
// DEFAULT-NEXT:                                           "i",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: Identifier(
// DEFAULT-NEXT:                                           "n",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           increment: Some(
// DEFAULT-NEXT:                               Const(
// DEFAULT-NEXT:                                   PostIncrement(
// DEFAULT-NEXT:                                       Identifier(
// DEFAULT-NEXT:                                           "i",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: [
// DEFAULT-NEXT:                               DoWhile {
// DEFAULT-NEXT:                                   body: [
// DEFAULT-NEXT:                                       Decl(
// DEFAULT-NEXT:                                           Declaration {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Tagged {
// DEFAULT-NEXT:                                                       kind: Union,
// DEFAULT-NEXT:                                                       name: None,
// DEFAULT-NEXT:                                                       body: Some(
// DEFAULT-NEXT:                                                           Fields(
// DEFAULT-NEXT:                                                               [
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Floating(
// DEFAULT-NEXT:                                                                               Float,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Name(
// DEFAULT-NEXT:                                                                                   "r",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   FieldDecl {
// DEFAULT-NEXT:                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                           ty: Integer(
// DEFAULT-NEXT:                                                                               Ranked {
// DEFAULT-NEXT:                                                                                   rank: Int,
// DEFAULT-NEXT:                                                                                   signed: false,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       declarators: [
// DEFAULT-NEXT:                                                                           FieldDeclarator {
// DEFAULT-NEXT:                                                                               declarator: Name(
// DEFAULT-NEXT:                                                                                   "i",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ],
// DEFAULT-NEXT:                                                                       provenance: Provenance {
// DEFAULT-NEXT:                                                                           file: FileId(
// DEFAULT-NEXT:                                                                               0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           kind: System,
// DEFAULT-NEXT:                                                                           line: 0,
// DEFAULT-NEXT:                                                                           header: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ],
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarators: [
// DEFAULT-NEXT:                                                   InitDeclarator {
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "e",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: Assign,
// DEFAULT-NEXT:                                                   target: Member {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "e",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "r",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   value: Index {
// DEFAULT-NEXT:                                                       base: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "c",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "i",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           2,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       Expr(
// DEFAULT-NEXT:                                           Const(
// DEFAULT-NEXT:                                               Assign {
// DEFAULT-NEXT:                                                   op: Assign,
// DEFAULT-NEXT:                                                   target: Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "d",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "b",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   value: Ternary {
// DEFAULT-NEXT:                                                       condition: Binary {
// DEFAULT-NEXT:                                                           op: GreaterEqual,
// DEFAULT-NEXT:                                                           left: Member {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "e",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               field: "i",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               1065287680,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       then_value: Ternary {
// DEFAULT-NEXT:                                                           condition: Binary {
// DEFAULT-NEXT:                                                               op: Less,
// DEFAULT-NEXT:                                                               left: Cast {
// DEFAULT-NEXT:                                                                   ty: Integer(
// DEFAULT-NEXT:                                                                       Ranked {
// DEFAULT-NEXT:                                                                           rank: Int,
// DEFAULT-NEXT:                                                                           signed: true,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                                   value: Member {
// DEFAULT-NEXT:                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                           "e",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       field: "i",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           then_value: Cast {
// DEFAULT-NEXT:                                                               ty: Integer(
// DEFAULT-NEXT:                                                                   Char {
// DEFAULT-NEXT:                                                                       signed: Some(
// DEFAULT-NEXT:                                                                           false,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                               value: Integer(
// DEFAULT-NEXT:                                                                   0,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           else_value: Cast {
// DEFAULT-NEXT:                                                               ty: Integer(
// DEFAULT-NEXT:                                                                   Char {
// DEFAULT-NEXT:                                                                       signed: Some(
// DEFAULT-NEXT:                                                                           false,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                               value: Integer(
// DEFAULT-NEXT:                                                                   255,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       else_value: Comma(
// DEFAULT-NEXT:                                                           Assign {
// DEFAULT-NEXT:                                                               op: Assign,
// DEFAULT-NEXT:                                                               target: Member {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "e",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "r",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               value: Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Member {
// DEFAULT-NEXT:                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                               "e",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           field: "r",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       right: Binary {
// DEFAULT-NEXT:                                                                           op: Div,
// DEFAULT-NEXT:                                                                           left: Float(
// DEFAULT-NEXT:                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                   value: Single(
// DEFAULT-NEXT:                                                                                       255.0,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           right: Float(
// DEFAULT-NEXT:                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                   value: Single(
// DEFAULT-NEXT:                                                                                       256.0,
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: Float(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           value: Single(
// DEFAULT-NEXT:                                                                               32768.0,
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           Cast {
// DEFAULT-NEXT:                                                               ty: Integer(
// DEFAULT-NEXT:                                                                   Char {
// DEFAULT-NEXT:                                                                       signed: Some(
// DEFAULT-NEXT:                                                                           false,
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                               value: Member {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "e",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "i",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                                   condition: Const(
// DEFAULT-NEXT:                                       Integer(
// DEFAULT-NEXT:                                           0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Assign {
// DEFAULT-NEXT:                                           op: AddAssign,
// DEFAULT-NEXT:                                           target: Identifier(
// DEFAULT-NEXT:                                               "d",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           value: Identifier(
// DEFAULT-NEXT:                                               "y",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 14,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
