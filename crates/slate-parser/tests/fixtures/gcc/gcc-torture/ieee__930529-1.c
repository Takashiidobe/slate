/* { dg-do run } */
void abort(void);
void exit(int);

int main(void) {
  union {
    double        d;
    unsigned char c[8];
  } d;

  d.d = 1.0 / 7.0;

  if (sizeof(char) * 8 == sizeof(double)) {
    if (d.c[0] == 0x92 && d.c[1] == 0x24 && d.c[2] == 0x49 && d.c[3] == 0x92 &&
        d.c[4] == 0x24 && d.c[5] == 0x49 && d.c[6] == 0xc2 && d.c[7] == 0x3f)
      exit(0);
    if (d.c[7] == 0x92 && d.c[6] == 0x24 && d.c[5] == 0x49 && d.c[4] == 0x92 &&
        d.c[3] == 0x24 && d.c[2] == 0x49 && d.c[1] == 0xc2 && d.c[0] == 0x3f)
      exit(0);
#if defined __arm__ || defined __thumb__
    if (d.c[4] == 0x92 && d.c[5] == 0x24 && d.c[6] == 0x49 && d.c[7] == 0x92 &&
        d.c[0] == 0x24 && d.c[1] == 0x49 && d.c[2] == 0xc2 && d.c[3] == 0x3f)
      exit(0);
#endif
    abort();
  }

  exit(0);
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment(
// DEFAULT-NEXT:       CommentGroup {
// DEFAULT-NEXT:           comments: [
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* { dg-do run } */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 0,
// DEFAULT-NEXT:                       length: 19,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
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
// DEFAULT-NEXT: decl[1]: Declaration {
// DEFAULT-NEXT:       declaration: Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "abort",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
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
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclarator {
// DEFAULT-NEXT:                   declarator: Function {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "exit",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       parameters: [
// DEFAULT-NEXT:                           Parameter {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:       provenance: Provenance {
// DEFAULT-NEXT:           file: FileId(
// DEFAULT-NEXT:               3,
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           kind: User,
// DEFAULT-NEXT:           line: 2,
// DEFAULT-NEXT:           header: None,
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[3]: Function(
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
// DEFAULT-NEXT:                           ty: Tagged {
// DEFAULT-NEXT:                               kind: Union,
// DEFAULT-NEXT:                               name: None,
// DEFAULT-NEXT:                               body: Some(
// DEFAULT-NEXT:                                   Fields(
// DEFAULT-NEXT:                                       [
// DEFAULT-NEXT:                                           FieldDecl {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Floating(
// DEFAULT-NEXT:                                                       Double,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarators: [
// DEFAULT-NEXT:                                                   FieldDeclarator {
// DEFAULT-NEXT:                                                       declarator: Name(
// DEFAULT-NEXT:                                                           "d",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: 0,
// DEFAULT-NEXT:                                                   header: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           FieldDecl {
// DEFAULT-NEXT:                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                   ty: Integer(
// DEFAULT-NEXT:                                                       Char {
// DEFAULT-NEXT:                                                           signed: Some(
// DEFAULT-NEXT:                                                               false,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               declarators: [
// DEFAULT-NEXT:                                                   FieldDeclarator {
// DEFAULT-NEXT:                                                       declarator: Array {
// DEFAULT-NEXT:                                                           inner: Name(
// DEFAULT-NEXT:                                                               "c",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           size: Expression(
// DEFAULT-NEXT:                                                               IntLit(
// DEFAULT-NEXT:                                                                   8,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                               provenance: Provenance {
// DEFAULT-NEXT:                                                   file: FileId(
// DEFAULT-NEXT:                                                       0,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   kind: System,
// DEFAULT-NEXT:                                                   line: 0,
// DEFAULT-NEXT:                                                   header: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ],
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "d",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Member {
// DEFAULT-NEXT:                               base: Identifier(
// DEFAULT-NEXT:                                   "d",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               field: "d",
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Div,
// DEFAULT-NEXT:                               left: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           1.0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Float(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       value: Double(
// DEFAULT-NEXT:                                           7.0,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Const(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Equal,
// DEFAULT-NEXT:                           left: Binary {
// DEFAULT-NEXT:                               op: Mul,
// DEFAULT-NEXT:                               left: SizeOfType {
// DEFAULT-NEXT:                                   ty: Integer(
// DEFAULT-NEXT:                                       Char {
// DEFAULT-NEXT:                                           signed: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   declarator: Abstract,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Integer(
// DEFAULT-NEXT:                                   8,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           right: SizeOfType {
// DEFAULT-NEXT:                               ty: Floating(
// DEFAULT-NEXT:                                   Double,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       If {
// DEFAULT-NEXT:                           condition: Const(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: And,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: And,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: And,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: And,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: And,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: And,
// DEFAULT-NEXT:                                                       left: Binary {
// DEFAULT-NEXT:                                                           op: And,
// DEFAULT-NEXT:                                                           left: Binary {
// DEFAULT-NEXT:                                                               op: Equal,
// DEFAULT-NEXT:                                                               left: Index {
// DEFAULT-NEXT:                                                                   base: Member {
// DEFAULT-NEXT:                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                           "d",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       field: "c",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   index: Integer(
// DEFAULT-NEXT:                                                                       0,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   146,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Binary {
// DEFAULT-NEXT:                                                               op: Equal,
// DEFAULT-NEXT:                                                               left: Index {
// DEFAULT-NEXT:                                                                   base: Member {
// DEFAULT-NEXT:                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                           "d",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       field: "c",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   index: Integer(
// DEFAULT-NEXT:                                                                       1,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   36,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Equal,
// DEFAULT-NEXT:                                                           left: Index {
// DEFAULT-NEXT:                                                               base: Member {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "d",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "c",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               index: Integer(
// DEFAULT-NEXT:                                                                   2,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               73,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Binary {
// DEFAULT-NEXT:                                                       op: Equal,
// DEFAULT-NEXT:                                                       left: Index {
// DEFAULT-NEXT:                                                           base: Member {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "d",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               field: "c",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               3,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           146,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Binary {
// DEFAULT-NEXT:                                                   op: Equal,
// DEFAULT-NEXT:                                                   left: Index {
// DEFAULT-NEXT:                                                       base: Member {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "d",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           field: "c",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           4,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       36,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Equal,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Member {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "d",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "c",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Integer(
// DEFAULT-NEXT:                                                       5,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   73,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Index {
// DEFAULT-NEXT:                                               base: Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "d",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "c",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   6,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               194,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Binary {
// DEFAULT-NEXT:                                       op: Equal,
// DEFAULT-NEXT:                                       left: Index {
// DEFAULT-NEXT:                                           base: Member {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "d",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "c",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           index: Integer(
// DEFAULT-NEXT:                                               7,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           63,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_branch: [
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "exit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                           else_branch: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       If {
// DEFAULT-NEXT:                           condition: Const(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: And,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: And,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: And,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: And,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: And,
// DEFAULT-NEXT:                                                   left: Binary {
// DEFAULT-NEXT:                                                       op: And,
// DEFAULT-NEXT:                                                       left: Binary {
// DEFAULT-NEXT:                                                           op: And,
// DEFAULT-NEXT:                                                           left: Binary {
// DEFAULT-NEXT:                                                               op: Equal,
// DEFAULT-NEXT:                                                               left: Index {
// DEFAULT-NEXT:                                                                   base: Member {
// DEFAULT-NEXT:                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                           "d",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       field: "c",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   index: Integer(
// DEFAULT-NEXT:                                                                       7,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   146,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Binary {
// DEFAULT-NEXT:                                                               op: Equal,
// DEFAULT-NEXT:                                                               left: Index {
// DEFAULT-NEXT:                                                                   base: Member {
// DEFAULT-NEXT:                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                           "d",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       field: "c",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   index: Integer(
// DEFAULT-NEXT:                                                                       6,
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               right: Integer(
// DEFAULT-NEXT:                                                                   36,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Equal,
// DEFAULT-NEXT:                                                           left: Index {
// DEFAULT-NEXT:                                                               base: Member {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "d",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "c",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               index: Integer(
// DEFAULT-NEXT:                                                                   5,
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: Integer(
// DEFAULT-NEXT:                                                               73,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Binary {
// DEFAULT-NEXT:                                                       op: Equal,
// DEFAULT-NEXT:                                                       left: Index {
// DEFAULT-NEXT:                                                           base: Member {
// DEFAULT-NEXT:                                                               base: Identifier(
// DEFAULT-NEXT:                                                                   "d",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               field: "c",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           index: Integer(
// DEFAULT-NEXT:                                                               4,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Integer(
// DEFAULT-NEXT:                                                           146,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Binary {
// DEFAULT-NEXT:                                                   op: Equal,
// DEFAULT-NEXT:                                                   left: Index {
// DEFAULT-NEXT:                                                       base: Member {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "d",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           field: "c",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           3,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Integer(
// DEFAULT-NEXT:                                                       36,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Equal,
// DEFAULT-NEXT:                                               left: Index {
// DEFAULT-NEXT:                                                   base: Member {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "d",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       field: "c",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   index: Integer(
// DEFAULT-NEXT:                                                       2,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Integer(
// DEFAULT-NEXT:                                                   73,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: Equal,
// DEFAULT-NEXT:                                           left: Index {
// DEFAULT-NEXT:                                               base: Member {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "d",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "c",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   1,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Integer(
// DEFAULT-NEXT:                                               194,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Binary {
// DEFAULT-NEXT:                                       op: Equal,
// DEFAULT-NEXT:                                       left: Index {
// DEFAULT-NEXT:                                           base: Member {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "d",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "c",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           index: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Integer(
// DEFAULT-NEXT:                                           63,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           then_branch: [
// DEFAULT-NEXT:                               Expr(
// DEFAULT-NEXT:                                   Const(
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "exit",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               Integer(
// DEFAULT-NEXT:                                                   0,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ],
// DEFAULT-NEXT:                           else_branch: None,
// DEFAULT-NEXT:                       },
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
// DEFAULT-NEXT:               line: 4,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
