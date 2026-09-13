// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-do compile { target { lp64 || llp64 } } } */
struct A { int b[1]; };

void
foo (struct A *d)
{
  d->b[0] = d->b[-144115188075855873LL] + d->b[11] * d->b[2]
          + d->b[0] % d->b[1025] + d->b[5];
  d->b[0] = d->b[144678138029277184LL] + d->b[0] & d->b[-3] * d->b[053]
          + d->b[7] ^ d->b[-9] + d->b[14] + d->b[9] % d->b[49]
          + d->b[024] + d->b[82] & d->b[4096];
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: decl[0]: Comment(
// DEFAULT-NEXT:       CommentGroup {
// DEFAULT-NEXT:           comments: [
// DEFAULT-NEXT:               Comment {
// DEFAULT-NEXT:                   text: "/* { dg-do compile { target { lp64 || llp64 } } } */",
// DEFAULT-NEXT:                   kind: Block,
// DEFAULT-NEXT:                   loc: Loc {
// DEFAULT-NEXT:                       file: FileId(
// DEFAULT-NEXT:                           3,
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       offset: 1,
// DEFAULT-NEXT:                       length: 52,
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
// DEFAULT-NEXT: decl[1]: Record(
// DEFAULT-NEXT:       RecordDecl {
// DEFAULT-NEXT:           kind: Struct,
// DEFAULT-NEXT:           name: Some(
// DEFAULT-NEXT:               "A",
// DEFAULT-NEXT:           ),
// DEFAULT-NEXT:           fields: [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclarator {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "b",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntLit(
// DEFAULT-NEXT:                                           1,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
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
// DEFAULT-NEXT:               line: 2,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[2]: Function(
// DEFAULT-NEXT:       FunctionDecl {
// DEFAULT-NEXT:           ret_type: Void,
// DEFAULT-NEXT:           name: "foo",
// DEFAULT-NEXT:           parameters: [
// DEFAULT-NEXT:               Parameter {
// DEFAULT-NEXT:                   ty: Tagged {
// DEFAULT-NEXT:                       kind: Struct,
// DEFAULT-NEXT:                       name: Some(
// DEFAULT-NEXT:                           "A",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   declarator: Some(
// DEFAULT-NEXT:                       Pointer {
// DEFAULT-NEXT:                           qualifiers: Qualifiers,
// DEFAULT-NEXT:                           inner: Name(
// DEFAULT-NEXT:                               "d",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Index {
// DEFAULT-NEXT:                               base: Arrow {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "d",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   field: "b",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               index: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: Add,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Index {
// DEFAULT-NEXT:                                           base: Arrow {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "d",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "b",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           index: Unary {
// DEFAULT-NEXT:                                               op: Minus,
// DEFAULT-NEXT:                                               value: Integer(
// DEFAULT-NEXT:                                                   144115188075855873,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: Index {
// DEFAULT-NEXT:                                               base: Arrow {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "d",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "b",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   11,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Index {
// DEFAULT-NEXT:                                               base: Arrow {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "d",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "b",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   2,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Binary {
// DEFAULT-NEXT:                                       op: Rem,
// DEFAULT-NEXT:                                       left: Index {
// DEFAULT-NEXT:                                           base: Arrow {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "d",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "b",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           index: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Index {
// DEFAULT-NEXT:                                           base: Arrow {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "d",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "b",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           index: Integer(
// DEFAULT-NEXT:                                               1025,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Index {
// DEFAULT-NEXT:                                   base: Arrow {
// DEFAULT-NEXT:                                       base: Identifier(
// DEFAULT-NEXT:                                           "d",
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       field: "b",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   index: Integer(
// DEFAULT-NEXT:                                       5,
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Expr(
// DEFAULT-NEXT:                   Const(
// DEFAULT-NEXT:                       Assign {
// DEFAULT-NEXT:                           op: Assign,
// DEFAULT-NEXT:                           target: Index {
// DEFAULT-NEXT:                               base: Arrow {
// DEFAULT-NEXT:                                   base: Identifier(
// DEFAULT-NEXT:                                       "d",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   field: "b",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               index: Integer(
// DEFAULT-NEXT:                                   0,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           value: Binary {
// DEFAULT-NEXT:                               op: BitXor,
// DEFAULT-NEXT:                               left: Binary {
// DEFAULT-NEXT:                                   op: BitAnd,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Index {
// DEFAULT-NEXT:                                           base: Arrow {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "d",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "b",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           index: Integer(
// DEFAULT-NEXT:                                               144678138029277184,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Index {
// DEFAULT-NEXT:                                           base: Arrow {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "d",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "b",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           index: Integer(
// DEFAULT-NEXT:                                               0,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: Index {
// DEFAULT-NEXT:                                               base: Arrow {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "d",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "b",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: Unary {
// DEFAULT-NEXT:                                                   op: Minus,
// DEFAULT-NEXT:                                                   value: Integer(
// DEFAULT-NEXT:                                                       3,
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Index {
// DEFAULT-NEXT:                                               base: Arrow {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "d",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "b",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   43,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Index {
// DEFAULT-NEXT:                                           base: Arrow {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "d",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "b",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           index: Integer(
// DEFAULT-NEXT:                                               7,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: BitAnd,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Add,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: Binary {
// DEFAULT-NEXT:                                                   op: Add,
// DEFAULT-NEXT:                                                   left: Index {
// DEFAULT-NEXT:                                                       base: Arrow {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "d",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           field: "b",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Unary {
// DEFAULT-NEXT:                                                           op: Minus,
// DEFAULT-NEXT:                                                           value: Integer(
// DEFAULT-NEXT:                                                               9,
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Index {
// DEFAULT-NEXT:                                                       base: Arrow {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "d",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           field: "b",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           14,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               right: Binary {
// DEFAULT-NEXT:                                                   op: Rem,
// DEFAULT-NEXT:                                                   left: Index {
// DEFAULT-NEXT:                                                       base: Arrow {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "d",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           field: "b",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           9,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   right: Index {
// DEFAULT-NEXT:                                                       base: Arrow {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "d",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           field: "b",
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: Integer(
// DEFAULT-NEXT:                                                           49,
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: Index {
// DEFAULT-NEXT:                                               base: Arrow {
// DEFAULT-NEXT:                                                   base: Identifier(
// DEFAULT-NEXT:                                                       "d",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   field: "b",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               index: Integer(
// DEFAULT-NEXT:                                                   20,
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: Index {
// DEFAULT-NEXT:                                           base: Arrow {
// DEFAULT-NEXT:                                               base: Identifier(
// DEFAULT-NEXT:                                                   "d",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "b",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           index: Integer(
// DEFAULT-NEXT:                                               82,
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Index {
// DEFAULT-NEXT:                                       base: Arrow {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "d",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           field: "b",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       index: Integer(
// DEFAULT-NEXT:                                           4096,
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
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
// DEFAULT-NEXT:               line: 4,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
